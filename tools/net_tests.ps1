<#
    Automated multiplayer network tests for remc2.

    Runs a series of sessions - two instances, or three where the scenario needs a
    survivor after one node goes away - each disturbing the connection in a different
    way, and reports whether the nodes still agreed with each other.

    Every instance that was started is judged: it has to reach the game, exchange turns
    and report no wedged NCB.  Checking only the nodes that happened to keep talking is
    how a three-player run with one silent node used to be recorded as a pass.

    The game itself needs no interaction: --auto_test drives the menus (skips the intros,
    enters the network game, confirms the session and starts the level once both players
    are present) and --network_debug writes one log per instance.

    Usage:
        .\net_tests.ps1                       # all tests
        .\net_tests.ps1 -Only drop,delay      # selected tests
        .\net_tests.ps1 -List                 # just list what would run

    Requires two runnable folders, one per instance (the second is created by copying the
    first; large data folders are linked rather than copied).
#>

param(
    [ValidateSet("Debug","Release")]
    [string]   $Config   = "Debug",
    [string]   $GameDir,      # runnable folder of the first instance (has the game data)
    [string]   $GameDir2,     # second instance; created from the first if missing
    [string]   $GameDir3,     # third instance, for the scenarios that need three players
    [string]   $Exe,          # freshly built binary to test
    [int]      $ServerPort = 3030,
    [int]      $ClientData = 3032,
    [int]      $Client2Data= 3033,
    [string[]] $Only     = @(),
    [switch]   $List,
    [switch]   $KeepLogs
)

$ErrorActionPreference = "Stop"

# Everything is derived from where this script lives (<repo>\tools), so the tests run
# from any checkout.  Each path can still be overridden on the command line.
$RepoRoot = Split-Path -Parent $PSScriptRoot
if (-not $GameDir)  { $GameDir  = Join-Path $RepoRoot "x64\$Config" }
if (-not $GameDir2) { $GameDir2 = Join-Path $RepoRoot "x64\${Config}2" }
if (-not $GameDir3) { $GameDir3 = Join-Path $RepoRoot "x64\${Config}3" }
# The build writes the binary into the runnable folder itself.  remc2\x64\$Config is an
# older output location that can still hold a copy from weeks ago, and copying that over the
# fresh build is how a test ends up measuring the wrong binary - which it then reports as a
# failure of the code under test.
if (-not $Exe)      { $Exe      = Join-Path $RepoRoot "x64\$Config\remc2.exe" }

# ---------------------------------------------------------------------------
# Test catalogue.  'extraHost'/'extraClient' are appended to that instance's command
# line; 'runSeconds' is how long the session is left alone before it is judged.
# ---------------------------------------------------------------------------
# Note on what is and is not simulated: the link is TCP, so nothing can be lost,
# duplicated or delivered out of order - it is pointless to pretend otherwise.  What an
# unreliable line really costs the game is time: a lost segment is retransmitted and
# everything queued behind it waits, which the 'stall' scenarios reproduce.
$Tests = @(
    @{ name = "baseline";        desc = "undisturbed session";                extraHost=@();                        extraClient=@();                        runSeconds=60 }
    @{ name = "delay";           desc = "30 ms on every message";             extraHost=@("--net_delay","30");      extraClient=@("--net_delay","30");      runSeconds=60 }
    @{ name = "delay-client";    desc = "only the client is slowed down";     extraHost=@();                        extraClient=@("--net_delay","60");      runSeconds=60 }
    @{ name = "jitter";          desc = "latency varying by 0-150 ms";        extraHost=@("--net_jitter","150");    extraClient=@("--net_jitter","150");    runSeconds=60 }
    @{ name = "stall";           desc = "300 ms pause every 50th message";    extraHost=@("--net_stall","300","--net_stall_every","50");
                                                                              extraClient=@("--net_stall","300","--net_stall_every","50");                 runSeconds=60 }
    @{ name = "stall-heavy";     desc = "800 ms pause every 15th message";    extraHost=@("--net_stall","800","--net_stall_every","15");
                                                                              extraClient=@("--net_stall","800","--net_stall_every","15");                 runSeconds=60 }
    @{ name = "mixed";           desc = "latency, jitter and stalls";         extraHost=@("--net_delay","20","--net_jitter","80","--net_stall","400","--net_stall_every","40");
                                                                              extraClient=@("--net_delay","20","--net_jitter","80","--net_stall","400","--net_stall_every","40"); runSeconds=60 }
    # The client must not run ahead while it is waiting for the host: a long pause in the
    # host's replies has to hold the client back, not let it simulate extra turns.
    @{ name = "host-freeze";     desc = "host unresponsive for 3 s at a time";extraHost=@();                        extraClient=@("--net_stall","3000","--net_stall_every","40"); runSeconds=60; checkLockstep=$true }
    # 4 s, not 8: HEARTBEAT_TIMEOUT_MS is 5 s, so a longer silence IS a dead peer by
    # definition and the session is torn down - measured as "PollSessions: timeout 5001ms".
    # Asking the game to survive an outage longer than its own dead-peer timeout is a
    # contradiction, and lengthening that timeout would blunt the detection every
    # *-vanishes / *-dies-* scenario relies on.  4 s is the longest freeze that can still be
    # ridden out, with a second to spare.
    @{ name = "host-freeze-long";desc = "host unresponsive for 4 s at a time";extraHost=@();                        extraClient=@("--net_stall","4000","--net_stall_every","25"); runSeconds=60; checkLockstep=$true }
    @{ name = "client-quits";    desc = "client leaves, host must carry on";  extraHost=@();                        extraClient=@("--quit_after","20");     runSeconds=60; expect="hostSurvives" }
    @{ name = "host-quits";      desc = "host leaves, client must carry on";  extraHost=@("--quit_after","20");     extraClient=@();                        runSeconds=60; expect="clientSurvives" }
    @{ name = "client-vanishes"; desc = "client is killed outright";          extraHost=@();                        extraClient=@();                        runSeconds=60; killAfter="client" }
    @{ name = "host-vanishes";   desc = "host is killed outright";            extraHost=@();                        extraClient=@();                        runSeconds=60; killAfter="host" }
    @{ name = "link-lost";       desc = "client tears its own link down";     extraHost=@();                        extraClient=@("--net_kill_after","20"); runSeconds=60 }

    # Two matches back to back in ONE process.  Every other scenario plays once and exits, so
    # nothing until now touched what has to be torn down and rebuilt between matches: data
    # sessions, pending NCB commands and the name registrations, all keyed by NCB pointers the
    # game reuses.  Leaving a level goes back to the menu (not out of the program), so the
    # second match runs through the same code a player would.
    #
    # This is the long-standing "cannot start another network game after finishing one".  Two
    # causes are fixed, and the second match now genuinely runs - both nodes rejoin, the host
    # rolls the level, and the exchange count roughly doubles (840-860 against 410-440 when it
    # does not get there):
    #   - the session was never handed back when a match ended, so the next one was refused by
    #     NetworkInitConnection_7308F before a single packet went out;
    #   - oldConnected[] was never cleared between matches, so RemoveDeadClients opened the
    #     second match by declaring every peer of the first one dead and hanging up the
    #     LISTENs that had just been armed.
    #
    # The third cause was a data race, and it is fixed too: measured 9 runs out of 9 after it,
    # against 3 out of 7 before.
    #
    # The instance that vanished at the match boundary was killed by AddressSanitizer, which
    # ends the process through internal__exit(1) - no CRT shutdown, no atexit, no exception
    # filter, nothing in the Windows event log.  That is why it looked like a clean exit and
    # why every ordinary way of tracing the termination came back empty; it is worth knowing
    # for the next one of these.  What ASan caught was a heap-use-after-free in Unpack_Message
    # called from PollCtrlSocket: the game thread walked into locUpdateNetworkSingleThread()
    # from DeleteNetwork() while the network thread was already inside it, and the control
    # receive buffer they then shared has no lock.  See the comment on that function in
    # port_net.cpp.  The dying-in-varying-places symptom fits a corruption, as suspected here
    # before - it just was not the NCBs, and ASan found it without needing PageHeap.
    @{ name = "two-matches";     desc = "two matches in one process"; matches=2; matchSeconds=20; runSeconds=80; expect="playsAllMatches" }

    # The same thing again with one more match, because the bug above lives at the BOUNDARY
    # between matches, not inside one.  two-matches crosses a boundary once per run, so it
    # samples the race once; this crosses it twice.  A race that comes back therefore shows up
    # here roughly twice as often, and the second boundary is also the first one that starts
    # from state a previous teardown rebuilt rather than from a fresh process - which is where
    # a half-cleared table would show and a single boundary cannot tell you anything about.
    #
    # Nothing in the game had to change for this: --auto_test_matches is a count, and
    # EventsFunctions.cpp loops on it (g_autotest_match + 1 < AutoTestMatches()).
    #
    # 20 s of play per match plus the menu, lobby and level load either side of each one; the
    # 120 s budget leaves as much slack again as two-matches has.
    @{ name = "three-matches";   desc = "three matches in one process"; matches=3; matchSeconds=20; runSeconds=120; expect="playsAllMatches" }

    # Three players.  With two, losing one leaves nobody to carry on with, so these are the
    # scenarios that say whether the session survives a node going away - and, when the node
    # that goes is the server, whether somebody else takes the role over.
    #
    # The undisturbed three-player session comes first: until it holds, none of the
    # kill scenarios below mean anything.  They used to "pass" with the third node
    # wedged in a RECEIVE that never completed, because only the two nodes that did
    # exchange turns were ever looked at.
    @{ name = "baseline-3p";        desc = "3p: undisturbed session";              players=3; runSeconds=60 }
    # waitPlayers is deliberately HIGHER than the number of instances, so the host never
    # reaches its start condition and everybody really is still in the lobby when the kill
    # lands.  It used to be 2 against 3 players, which did the opposite: the host started the
    # level as soon as two were present and the third was left behind in the lobby, never
    # exchanging anything.  That is what "3rd: 0/0" was recording - and since the third node
    # was also the one being killed, the scenario passed without ever testing it.
    #
    # Nobody can start the level once the host is gone either (the survivors still fall short
    # of the required count), so what is asserted is what a lobby-phase death can actually
    # show: the survivors elect a new server, find each other again, and keep exchanging.
    @{ name = "client-dies-lobby";  desc = "3p: a client is killed in the lobby";  players=3; killWhen="lobby"; killTarget="client2"; waitPlayers=4; runSeconds=70; expect="restKeepPlaying" }
    @{ name = "client-dies-game";   desc = "3p: a client is killed in the game";   players=3; killWhen="game";  killTarget="client2"; waitPlayers=3; runSeconds=70; expect="restKeepPlaying" }
    # Kills the MIDDLE player, not the last one.  Both scenarios above take out client2 - the
    # highest slot - which leaves the survivors packed at 0..N-1, so a hole in the middle of
    # the player array was never exercised at all.  Here slot 1 goes and 0 and 2 stay.
    #
    # NOT a regression test for the level-start bound: measured against a binary without that
    # fix, this still passes.  The bound is worked out once, when the level starts, and at
    # that moment all three players are still present and contiguous - the hole only appears
    # afterwards.  What this one covers is that a gap opening DURING play is survivable,
    # which is worth having on its own.  The scenario below is the one that guards the fix.
    @{ name = "client-dies-mid";    desc = "3p: the middle player is killed";      players=3; killWhen="game";  killTarget="client";  waitPlayers=3; runSeconds=70; expect="restKeepPlaying" }

    # THE regression test for the level-start bound, with no server hand-over involved.
    #
    # The middle player is killed while everyone is still in the lobby, so the level starts
    # with slots 0 and 2 occupied and a hole at 1.  That is the only shape that gets the bound
    # wrong: worked out as a COUNT it comes to 2, so slot 2 - a live player - falls outside
    # every loop, never spawns, and dies on its own status bar.  Taking the highest occupied
    # slot instead gives 3 and it plays.
    #
    # The kill has to land after the third player has registered and before the host rolls the
    # level, hence the pinned time rather than the usual 'lobby' 12 s.  The host's bar is 2, so
    # it starts once the two survivors are settled - the connected-mask debounce makes it wait
    # for that rather than firing on the stale count.
    # This used to be unreliable - 2 passes in 5 - and the failure was always the same: the
    # survivors exchanged 184-186 turns and stopped, with the third instance on 1 of 2.  Two
    # faults, both since fixed, and neither in the transport:
    #   - NetworkGetState_74FE1 read a completion code of NRC_BUFLEN as "not connected", so one
    #     oversized message had RemoveDeadClients() hang up a live session.  It only bit the
    #     node with nothing to send next, which is why the host shrugged the same code off;
    #   - the host started the level on a COUNT of connections, which includes itself and goes
    #     on including a killed peer until the five-second heartbeat expires.  "Me and a corpse"
    #     therefore reached the bar of two exactly like "me and the survivor", and the level was
    #     rolled 1.2 s after the kill, while the survivor's LISTEN was still unfinished -
    #     NetworkCancelAll_7449C() then cancelled it on the way in.  The start now also requires
    #     NetworkAllRosterPeersConnected(), which asks by name instead of by number.
    # Measured 6 passes in 6 after both, against 2 in 5 before.  Still worth running more than
    # once if it ever fails again: what it exercises is a timing window, not a fixed sequence.
    @{ name = "lobby-hole-at-start"; desc = "3p: middle player dies before the level starts"; players=3; killWhen="lobby"; killAtSeconds=8; killTarget="client"; waitPlayers=2; runSeconds=70; expect="restStartLevel" }
    # The clients' lower bar (2) only bites once one of them has taken the server role over,
    # so this asserts the whole chain: the host dies in the lobby, the survivors elect a new
    # server, find each other again, roll the level and go on playing it.
    @{ name = "server-dies-lobby";  desc = "3p: the server is killed in the lobby";players=3; killWhen="lobby"; killTarget="host";    waitPlayers=4; waitPlayersClients=2; runSeconds=70; expect="restStartLevel" }
    # The server leaves a FINISHED game and the rest have to start another one without it.
    #
    # Reported from real play and missing here: server-dies-* take the server out in the MIDDLE
    # of a session, where every pair already holds a session and the survivors simply carry on;
    # two-matches and three-matches play again but with everybody still present.  Neither covers
    # the gap between two matches, which is where the game rebuilds its connections from nothing
    # and the node they were all pointing at is gone.
    #
    # The host is taken out at 40 s: match 0 starts around 13 s and runs 15, so by then everybody
    # is back in the lobby setting up match 1.  Judged by restPlayAllMatches rather than
    # restStartLevel, because a survivor that does everything right finishes the last match and
    # leaves the application - which the "is it still running" half of restStartLevel reads as
    # "died with the host" on a run that was in fact perfect.  What is asserted is the whole
    # chain: the survivors elect a new server, re-aim their connections at it, roll the level,
    # and play every remaining match with real traffic in it.
    @{ name = "server-quits-between-matches"; desc = "3p: the server leaves after the first match";
                                    players=3; matches=3; matchSeconds=15; killWhen="lobby"; killAtSeconds=40;
                                    killTarget="host"; waitPlayers=2; runSeconds=80; expect="restPlayAllMatches" }
    @{ name = "server-dies-game";   desc = "3p: the server is killed in the game"; players=3; killWhen="game";  killTarget="host";    waitPlayers=3; runSeconds=70; expect="restKeepPlaying" }
)

if ($List) {
    $Tests | ForEach-Object { "{0,-16} {1}" -f $_.name, $_.desc }
    return
}

if ($Only.Count -gt 0) {
    # Called through -File, "a,b" arrives as one string, so split it apart again.
    $wanted = $Only | ForEach-Object { $_ -split ',' } | Where-Object { $_ -ne '' }
    $Tests = @($Tests | Where-Object { $wanted -contains $_.name })
    if ($Tests.Count -eq 0) { throw "No test matches: $($wanted -join ', ')" }
}

# ---------------------------------------------------------------------------
function Stop-AllInstances {
    Get-Process remc2 -ErrorAction SilentlyContinue | Stop-Process -Force
    Start-Sleep -Seconds 2
}

function Initialize-InstanceFolder([string]$target) {
    # Mirror the runnable folder; the bulky read-only data is linked, not duplicated.
    if (Test-Path (Join-Path $target "remc2.exe")) { return }
    Write-Host "Preparing instance folder $target ..." -ForegroundColor DarkGray
    New-Item -ItemType Directory -Path $target -Force | Out-Null
    $linked = @('NETHERW','CD_Files','fixed-ingame-graphics','font','graphics','memimages','music-ogg','kiss')
    foreach ($item in Get-ChildItem $GameDir) {
        $dest = Join-Path $target $item.Name
        if ($item.PSIsContainer) {
            if ($linked -contains $item.Name) { cmd /c "mklink /J `"$dest`" `"$($item.FullName)`"" | Out-Null }
            else { Copy-Item $item.FullName $dest -Recurse -Force }
        }
        elseif ($item.Name -notlike 'net_messages_log1_*') { Copy-Item $item.FullName $dest -Force }
    }
}

# Longest pause a scenario can deliberately impose on one message, so the judgement can
# tell an injected wait apart from a genuinely wedged NCB.
function Get-InjectedHoldMs($argList) {
    $d = 0; $j = 0; $s = 0
    if (-not $argList) { return 0 }
    for ($i = 0; $i -lt $argList.Count - 1; $i++) {
        switch ($argList[$i]) {
            '--net_delay'  { $d = [int]$argList[$i + 1] }
            '--net_jitter' { $j = [int]$argList[$i + 1] }
            '--net_stall'  { $s = [int]$argList[$i + 1] }
        }
    }
    return $d + $j + $s
}

function Get-NetLog([string]$folder, [int]$port) {
    Join-Path $folder ("net_messages_log1_{0}.txt" -f $port)
}

function Measure-Session([string]$logPath, [int]$sinceMs = -1) {
    # Reads one instance's log and reports what the network layer actually did.
    $result = [ordered]@{
        reachedGame = $false; sent = 0; queued = 0; delivered = 0
        backlog = 0; maxQueue = 0; stalled = 0
        disconnects = 0; fatal = 0; stuck = 0
        deliveredAfterKill = 0; tookOver = $false; startedLevel = $false
        matchesEntered = 0; deliveredLastMatch = 0
        maxGap = 0; maxGapAt = 0
    }
    if (-not (Test-Path $logPath)) { return $result }
    $text = Get-Content $logPath -Raw
    $result.reachedGame = $text -match 'host starts the level|level selection reached'
    $result.sent        = ([regex]::Matches($text, 'SendNetwork: sent')).Count
    $result.queued      = ([regex]::Matches($text, 'queued DIRECT_SEND')).Count
    $result.delivered   = ([regex]::Matches($text, 'RECEIVE.*DELIVERED')).Count
    $result.stalled     = ([regex]::Matches($text, 'NETFAULT: stalling')).Count
    $result.disconnects = ([regex]::Matches($text, 'disconnected')).Count
    $result.fatal       = ([regex]::Matches($text, 'FATAL')).Count
    # An NCB the game is still spinning on.  This is what a wedged node looks like from
    # the outside, and it has to fail the scenario rather than pass quietly.
    $result.stuck       = ([regex]::Matches($text, 'STUCK')).Count
    $result.backlog     = $result.queued - $result.delivered
    $queues = [regex]::Matches($text, 'queue=(\d+)') | ForEach-Object { [int]$_.Groups[1].Value }
    if ($queues.Count -gt 0) { $result.maxQueue = ($queues | Measure-Object -Maximum).Maximum }
    $result.tookOver    = $text -match 'TAKEOVER'
    $result.startedLevel = $text -match 'host starts the level'

    # Every node logs "entering network game" once per match, so this counts the matches this
    # instance actually took part in - and the exchanges counted from the LAST of them say
    # whether the rebuilt session carried turns, rather than the first match's traffic
    # flattering the total.
    $entries = [regex]::Matches($text, '(?m)^(\d+)\|AUTOTEST: entering network game')
    $result.matchesEntered = $entries.Count
    if ($entries.Count -gt 0) {
        $lastEntryMs = [int]$entries[$entries.Count - 1].Groups[1].Value
        foreach ($m in [regex]::Matches($text, '(?m)^(\d+)\|.*(RECEIVE.*DELIVERED|SendNetwork: sent)')) {
            if ([int]$m.Groups[1].Value -ge $lastEntryMs) { $result.deliveredLastMatch++ }
        }
    }

    # The largest hole in this instance's own timeline.  Both threads log through the same
    # writer and the network thread ticks every couple of milliseconds, so a run that is being
    # given CPU and disk has no hole worth mentioning - measured over four clean baseline-3p
    # runs, the worst was 999 ms and every one of them was during start-up.  A hole of several
    # seconds means this process was not running, which is a different thing from anything the
    # counters above can describe.
    $prevMs = -1
    foreach ($m in [regex]::Matches($text, '(?m)^(\d+)\|')) {
        $t = [int]$m.Groups[1].Value
        if ($prevMs -ge 0 -and ($t - $prevMs) -gt $result.maxGap) {
            $result.maxGap   = $t - $prevMs
            $result.maxGapAt = $prevMs
        }
        $prevMs = $t
    }

    # Every log line starts with milliseconds since that instance started, so "what happened
    # after the node was killed" can be counted without a second log.
    if ($sinceMs -ge 0) {
        foreach ($m in [regex]::Matches($text, '(?m)^(\d+)\|.*(RECEIVE.*DELIVERED|SendNetwork: sent)')) {
            if ([int]$m.Groups[1].Value -ge $sinceMs) { $result.deliveredAfterKill++ }
        }
    }
    return $result
}

function Invoke-Test($test) {
    Stop-AllInstances
    $players = if ($test.players) { [int]$test.players } else { 2 }
    $dirs = @($GameDir, $GameDir2); if ($players -ge 3) { $dirs += $GameDir3 }
    foreach ($d in $dirs) {
        $dest = Join-Path $d "remc2.exe"
        # An instance that has just been killed can still hold its own exe for a moment -
        # more so under AddressSanitizer, which has a teardown of its own - and the copy then
        # throws.  With $ErrorActionPreference = "Stop" that ends the whole suite, which is
        # how a 23-scenario run once stopped at the fifth.  Wait for the handle to go.
        if ((Resolve-Path $Exe).Path -ne $dest) {
            $copied = $false
            for ($try = 1; $try -le 10 -and -not $copied; $try++) {
                try { Copy-Item $Exe $dest -Force -ErrorAction Stop; $copied = $true }
                catch {
                    if ($try -eq 5) { Stop-AllInstances }   # something is still running
                    Start-Sleep -Milliseconds 500
                }
            }
            if (-not $copied) { throw "Cannot refresh $dest - it is held by another process." }
        }
        Get-ChildItem $d -Filter 'net_messages_log1_*.txt' -ErrorAction SilentlyContinue |
            Remove-Item -Force -ErrorAction SilentlyContinue
    }

    # The host waits for this many players before it starts the level.  A lobby scenario
    # holds the game there by asking for one more than will ever arrive.
    $wait = if ($test.waitPlayers) { [int]$test.waitPlayers } else { $players }

    # The clients may be given a LOWER bar than the host, which is what lets a lobby-phase
    # server death end with somebody actually rolling a level.  While the host is alive the
    # clients' figure is inert - the gate also requires being the server, and they are not.
    # Once the host dies and a survivor takes the role over, its own bar applies and it
    # starts the level.  So this exercises the hand-over of the server role end to end,
    # without the game code needing to know anything about the test.
    $waitClients = if ($test.waitPlayersClients) { [int]$test.waitPlayersClients } else { $wait }

    # Not every scenario adds arguments; an absent entry must not become a null in the list.
    $extraHost   = if ($test.extraHost)   { $test.extraHost }   else { @() }
    $extraClient = if ($test.extraClient) { $test.extraClient } else { @() }

    # Multi-match scenarios play, leave to the menu, and start again in the same process.
    $matchArgs = @()
    if ($test.matches) {
        $matchArgs = @('--auto_test_matches', "$($test.matches)",
                       '--auto_test_match_seconds', "$(if ($test.matchSeconds) { $test.matchSeconds } else { 20 })")
    }

    $commonBase   = @('--network','--network_debug','--auto_test','--auto_test_players')
    $commonHost   = $commonBase + @("$wait")
    $commonClient = $commonBase + @("$waitClients")
    $hostArgs   = $commonHost   + @('server', "$ServerPort")                              + $matchArgs + $extraHost
    $clientArgs = $commonClient + @('client', '127.0.0.1', "$ServerPort", "$ClientData")  + $matchArgs + $extraClient
    $client2Args= $commonClient + @('client', '127.0.0.1', "$ServerPort", "$Client2Data") + $matchArgs

    $procs = @{}
    $started = @{}
    $started.host = Get-Date
    $procs.host   = Start-Process -FilePath (Join-Path $GameDir  "remc2.exe") -ArgumentList $hostArgs   -WorkingDirectory $GameDir  -PassThru
    Start-Sleep -Seconds 2
    $started.client = Get-Date
    $procs.client = Start-Process -FilePath (Join-Path $GameDir2 "remc2.exe") -ArgumentList $clientArgs -WorkingDirectory $GameDir2 -PassThru
    if ($players -ge 3) {
        Start-Sleep -Seconds 2
        $started.client2 = Get-Date
        $procs.client2 = Start-Process -FilePath (Join-Path $GameDir3 "remc2.exe") -ArgumentList $client2Args -WorkingDirectory $GameDir3 -PassThru
    }

    # A scenario may pull the plug on one node: either while the game is still in the level
    # selection, or once it is running.
    if ($test.killWhen -or $test.killAfter) {
        $target = if ($test.killTarget) { $test.killTarget } else { $test.killAfter }
        # killAtSeconds pins the moment exactly, for scenarios that need to land between two
        # events rather than just "some time in the lobby".
        $when   = switch ($test.killWhen) {
            'lobby' { 12 }                                  # menus are through, level not started
            'game'  { 30 }                                  # level has been running a while
            default { [int]($test.runSeconds / 2) }
        }
        if ($test.killAtSeconds) { $when = [int]$test.killAtSeconds }
        Start-Sleep -Seconds $when
        $killedAt = Get-Date
        $victim = $procs[$target]
        if ($victim -and -not $victim.HasExited) { Stop-Process -Id $victim.Id -Force }
        Start-Sleep -Seconds ([math]::Max(10, $test.runSeconds - $when))
    }
    else {
        Start-Sleep -Seconds $test.runSeconds
    }

    $alive = @{}
    foreach ($k in $procs.Keys) { $alive[$k] = -not $procs[$k].HasExited }
    foreach ($k in $procs.Keys) { if (-not $procs[$k].HasExited) { Stop-Process -Id $procs[$k].Id -Force } }
    Start-Sleep -Seconds 1

    # Offset of the kill within each instance's own log timeline.
    function KillOffset([string]$who) {
        if (-not $killedAt -or -not $started[$who]) { return -1 }
        return [int]((New-TimeSpan -Start $started[$who] -End $killedAt).TotalMilliseconds)
    }
    $h  = Measure-Session (Get-NetLog $GameDir  $ServerPort)  (KillOffset 'host')
    $c  = Measure-Session (Get-NetLog $GameDir2 $ClientData)  (KillOffset 'client')
    $c2 = if ($players -ge 3) { Measure-Session (Get-NetLog $GameDir3 $Client2Data) (KillOffset 'client2') } else { $null }

    # ---- judgement -------------------------------------------------------
    $problems = @()
    $killed   = if ($test.killTarget) { $test.killTarget } else { $test.killAfter }
    $survivors = @{}
    foreach ($k in @('host','client','client2')) {
        if ($procs.ContainsKey($k) -and $k -ne $killed) { $survivors[$k] = $true }
    }
    $stats = @{ host = $h; client = $c; client2 = $c2 }

    # True where a node is meant to go away, or tear its own link down, part way through.
    $leavesEarly = $test.expect -or $test.killAfter -or $test.killWhen -or ($extraClient -contains '--net_kill_after')

    # Judge every instance that was started, not just the host and the first client.  The
    # third node used to be looked at only through 'Injected', so a session where it never
    # got into the game - or sat wedged in a RECEIVE - still counted as a pass.
    # A wedged NCB is only meaningful where nothing was supposed to hold the game up.
    # WaitForNcb_diag reports STUCK after 3 s, so a scenario that injects a longer pause
    # trips it on purpose, and so does a killed peer - that is only noticed once the 5 s
    # heartbeat timeout expires.  Demanding zero reports there would fail the scenarios
    # for doing exactly what they were written to do.
    $maxHold = [math]::Max((Get-InjectedHoldMs $extraHost), (Get-InjectedHoldMs $extraClient))
    $checkStuck = (-not $leavesEarly) -and ($maxHold -lt 2000)

    foreach ($k in @('host','client','client2')) {
        if (-not $procs.ContainsKey($k)) { continue }
        $s = $stats[$k]; if (-not $s) { continue }
        if (-not $s.reachedGame)  { $problems += "$k did not reach the game" }
        if ($s.fatal -gt 0)       { $problems += "FATAL in $k log" }
        if ($checkStuck -and $s.stuck -gt 0) { $problems += "$k has $($s.stuck) stuck NCB report(s)" }
        if ($s.backlog -gt 5)     { $problems += "$k is $($s.backlog) messages behind" }
    }

    # A session that stops exchanging has frozen, which a backlog check alone will not
    # catch: if nothing is received, nothing piles up either.  Scale the expectation with
    # the time the session was actually given, and skip it where a side leaves by design.
    if (-not $leavesEarly) {
        $minExchanges = [int]($test.runSeconds * 2)   # deliberately generous
        foreach ($k in @('host','client','client2')) {
            if (-not $procs.ContainsKey($k)) { continue }
            $s = $stats[$k]; if (-not $s) { continue }
            if ($s.delivered -lt $minExchanges) { $problems += "$k stalled ($($s.delivered) exchanges)" }
        }
    }

    # Lockstep: neither side may take turns the other has not seen.  Each exchange is one
    # turn, so the two counts have to stay close no matter how slow the link gets.
    if ($test.checkLockstep) {
        $lead = [math]::Abs($h.sent - $c.sent)
        $worst = [math]::Max($h.sent, $c.sent)
        if ($worst -gt 0 -and $lead -gt [math]::Max(5, $worst * 0.1)) {
            $problems += "one side ran $lead turns ahead (host $($h.sent), client $($c.sent))"
        }
    }

    switch ($test.expect) {
        'hostSurvives'   { if (-not $alive.host)   { $problems += "host did not survive the client leaving" } }
        'clientSurvives' { if (-not $alive.client) { $problems += "client did not survive the host leaving" } }
        # Whoever was not killed has to still be running, and the ones that are left have to
        # get themselves into the level - which after the server was killed means one of them
        # took the role over.
        'restStartLevel' {
            foreach ($k in $survivors.Keys) {
                if (-not $alive[$k]) { $problems += "$k died with the $killed" }
            }
            # Reaching the level selection is not enough - somebody has to actually start the
            # level afterwards, which after the server was killed means one of the survivors
            # took the role over.
            $anyStarted = $false
            foreach ($k in $survivors.Keys) { if ($stats[$k] -and $stats[$k].startedLevel) { $anyStarted = $true } }
            if (-not $anyStarted) { $problems += "nobody started the level after the $killed went away" }
            # ...and the match has to GO ON, not merely begin: same bar as restKeepPlaying,
            # so "rolled a level and then froze" cannot pass as success.
            foreach ($k in $survivors.Keys) {
                if ($alive[$k] -and $stats[$k].deliveredAfterKill -lt 20) {
                    $problems += "$k stopped exchanging after the $killed was killed ($($stats[$k].deliveredAfterKill))"
                }
            }
        }
        # Every instance has to have played BOTH matches, and the second one has to have
        # carried real traffic.  Counting total exchanges would not do: the first match alone
        # produces plenty, so a session that never came back would still look busy.
        'playsAllMatches' {
            # Deliberately no "still running" check: the last match ends by leaving the
            # program, so both instances are SUPPOSED to be gone by the time this is judged.
            $wantMatches = if ($test.matches) { [int]$test.matches } else { 2 }
            foreach ($k in @('host','client','client2')) {
                if (-not $procs.ContainsKey($k)) { continue }
                $s = $stats[$k]; if (-not $s) { continue }
                if ($s.matchesEntered -lt $wantMatches) {
                    $problems += "$k played $($s.matchesEntered) of $wantMatches matches"
                }
                elseif ($s.deliveredLastMatch -lt 20) {
                    $problems += "$k exchanged almost nothing in the last match ($($s.deliveredLastMatch))"
                }
            }
        }
        # The survivors go on WITHOUT the node that left, all the way to the end of the
        # scenario's match list.  Deliberately no "still running" check, for the same reason
        # playsAllMatches has none: the last match ends by leaving the program, so a survivor
        # that did everything asked of it is supposed to be gone by the time this is judged.
        # Measured with the token fix in: both survivors played all 3 matches, one of them took
        # the role over and rolled level 50 for match 2, and that match carried 1121 exchanges.
        'restPlayAllMatches' {
            $wantMatches = if ($test.matches) { [int]$test.matches } else { 2 }
            foreach ($k in $survivors.Keys) {
                $s = $stats[$k]; if (-not $s) { continue }
                if ($s.matchesEntered -lt $wantMatches) {
                    $problems += "$k played $($s.matchesEntered) of $wantMatches matches after the $killed went away"
                }
                elseif ($s.deliveredLastMatch -lt 20) {
                    $problems += "$k exchanged almost nothing in the last match ($($s.deliveredLastMatch))"
                }
            }
            # One of them has to have taken the role over: the survivors were all clients while
            # the node that left was there, so a survivor logging a level start can only be the
            # match it hosted itself.
            $anyStarted = $false
            foreach ($k in $survivors.Keys) { if ($stats[$k] -and $stats[$k].startedLevel) { $anyStarted = $true } }
            if (-not $anyStarted) { $problems += "nobody started the level after the $killed went away" }
        }
        # ...and here the survivors have to still be exchanging turns afterwards.
        'restKeepPlaying' {
            foreach ($k in $survivors.Keys) {
                if (-not $alive[$k]) { $problems += "$k died with the $killed" }
                elseif ($stats[$k].deliveredAfterKill -lt 20) {
                    $problems += "$k stopped exchanging after the $killed was killed ($($stats[$k].deliveredAfterKill))"
                }
            }
        }
    }
    # A side that was killed must not leave the other one stuck.
    if ($test.killAfter -eq 'host'   -and -not $alive.client) { $problems += "client died with the host" }
    if ($test.killAfter -eq 'client' -and -not $alive.host)   { $problems += "host died with the client" }

    # A freeze that stops EVERY instance at once is the machine, not the game.
    #
    # Seen for real: baseline-3p failed with "client has 2 stuck NCB reports" and the host 391
    # exchanges behind.  In the logs the host had a 20.5 s hole with nothing in it at all - not
    # even the network thread's per-tick lines - and both clients had holes of 15.4 s and
    # 15.9 s.  Lining the three timelines up on a 1.8 s hiccup they all shared put the ends of
    # those holes within 100 ms of each other: three separate processes started running again
    # at the same instant, which nothing inside any one of them can arrange.  Windows had
    # stopped the world; Defender had done a cloud lookup on the freshly built binary minutes
    # earlier and Windows Update was installing packages either side of the run.  The same
    # binary then passed the same scenario four times in a row.
    #
    # Reported as STALL rather than FAIL, and the caller runs the scenario again: a suite that
    # takes half an hour should not be thrown away because an update landed in the middle of
    # it, and - the point of the whole thing - a machine freeze must not be filed as a network
    # defect and sent somebody to look for it in the code.  A node that is genuinely wedged
    # keeps logging, so it does not look like this; and a scenario that kills a node does not
    # either, because the killed instance's log simply ends rather than resuming.  Two stalls
    # in a row are reported as a failure, so a machine that cannot hold a run steady is not
    # quietly ignored.
    $judged = @()
    foreach ($k in @('host','client','client2')) {
        if ($procs.ContainsKey($k) -and $stats[$k]) { $judged += $stats[$k] }
    }
    # Above any pause the scenario itself asked for, so an injected freeze is not mistaken for
    # the machine stopping.
    $stallFloor = [math]::Max(3000, $maxHold + 2000)
    $everyoneStalled = ($judged.Count -ge 2) -and -not ($judged | Where-Object { $_.maxGap -lt $stallFloor })
    if ($problems.Count -gt 0 -and $everyoneStalled) {
        $sizes = ($judged | ForEach-Object { "$([int]($_.maxGap / 1000))s" }) -join " / "
        $problems = @("the machine stopped every instance at once (holes of $sizes in the logs, nothing in them)") + $problems
    }

    return [pscustomobject]@{
        Name     = $test.name
        Result   = if ($problems.Count -eq 0) { "PASS" } elseif ($everyoneStalled) { "STALL" } else { "FAIL" }
        Detail   = if ($problems.Count -eq 0) { "" } else { $problems -join "; " }
        HostQ    = "$($h.delivered)/$($h.queued) maxQ=$($h.maxQueue)"
        ClientQ  = "$($c.delivered)/$($c.queued) maxQ=$($c.maxQueue)"
        Injected = if ($c2) { "3rd: $($c2.delivered)/$($c2.queued)" } else { "stalls: $($h.stalled + $c.stalled)" }
    }
}

# ---------------------------------------------------------------------------
if (-not (Test-Path $Exe)) {
    throw "Game binary not found: $Exe`nBuild the $Config configuration first, or pass -Exe <path>."
}
if (-not (Test-Path (Join-Path $GameDir "NETHERW"))) {
    throw "Game data not found in $GameDir`nPass -GameDir <folder containing NETHERW>."
}
Initialize-InstanceFolder $GameDir2
if ($Tests | Where-Object { $_.players -eq 3 }) { Initialize-InstanceFolder $GameDir3 }

Write-Host ""
Write-Host "remc2 network tests - $($Tests.Count) scenario(s), $Config build" -ForegroundColor Cyan
Write-Host "  binary:   $Exe"
Write-Host "  instance: $GameDir"
Write-Host "  instance: $GameDir2"
Write-Host ""

$results = @()
$index   = 0
$suiteStarted = Get-Date
foreach ($test in $Tests) {
    $index++
    # Printed BEFORE the scenario runs, without a newline, so somebody watching the console
    # can see which one is in progress rather than staring at nothing for a minute.
    Write-Host ("  [{0,2}/{1}] {2,-20} {3}" -f $index, $Tests.Count, $test.name, $test.desc) -NoNewline
    $started = Get-Date
    $r = Invoke-Test $test
    $took = [int]((New-TimeSpan -Start $started -End (Get-Date)).TotalSeconds)
    # The machine stopped the instances rather than the game going wrong - see the note in
    # Invoke-Test.  Nothing was measured, so measure it again; if the second run stalls too,
    # the machine is not fit to run the suite and that is reported rather than retried away.
    if ($r.Result -eq "STALL") {
        Write-Host ("  STALL {0,3}s" -f $took) -ForegroundColor DarkYellow
        Write-Host "        $($r.Detail)" -ForegroundColor DarkGray
        Write-Host ("  [{0,2}/{1}] {2,-20} {3}" -f $index, $Tests.Count, $test.name, "running it again") -NoNewline
        $started = Get-Date
        $r = Invoke-Test $test
        $took = [int]((New-TimeSpan -Start $started -End (Get-Date)).TotalSeconds)
        if ($r.Result -eq "STALL") { $r.Result = "FAIL" }
    }
    $results += $r
    if ($r.Result -eq "PASS") { Write-Host ("  PASS  {0,3}s" -f $took) -ForegroundColor Green }
    else {
        Write-Host ("  FAIL  {0,3}s" -f $took) -ForegroundColor Red
        Write-Host "        $($r.Detail)" -ForegroundColor Yellow
    }
}
$suiteTook = [int]((New-TimeSpan -Start $suiteStarted -End (Get-Date)).TotalMinutes)

Stop-AllInstances

Write-Host ""
$results | Format-Table Name, Result, HostQ, ClientQ, Injected, Detail -AutoSize
# @() around the filter: with exactly one match Where-Object returns a bare object whose
# .Count is empty, so a single failing scenario reported "" failures and exited 0 - the run
# looked green precisely when one thing was broken.
$failed = @($results | Where-Object Result -eq "FAIL").Count
Write-Host ""
if ($failed -eq 0) { Write-Host "All $($results.Count) scenarios passed in $suiteTook min." -ForegroundColor Green }
else               { Write-Host "$failed of $($results.Count) scenarios failed ($suiteTook min)." -ForegroundColor Red }
Write-Host "Per-instance logs:"
Write-Host "  $GameDir\net_messages_log1_$ServerPort.txt"
Write-Host "  $GameDir2\net_messages_log1_$ClientData.txt"
Write-Host "  $GameDir3\net_messages_log1_$Client2Data.txt   (three-player scenarios)"
exit $failed
