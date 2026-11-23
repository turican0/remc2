import argparse, os, random, shutil, struct, subprocess, sys, tempfile

# mkregression.py <mc1|mc1hw> <levels...> [--frames 2000] [--every 100] [--input actions|idle]: the regression tests of
# MC1 / HW for remc2-regression-test (memimages/regressions-<game>/level<NNN>, NNN = level + 1): input.rec (the input
# of player 0, 10 bytes a frame) played in the original in dosbox-x-remc1 (branch mc1, REMC1_PLAY at 3C9D0), the state
# dumped every --every frames at 20FB0 (REMC1_DUMP_EVERY) and packed to reference.binz by remc2-regression-test
# --pack_mc1. --input actions: a random flight with all spells (the cheat commands 30), idle: no input. --as_game: the
# level as started from the menu (as dbxrecord.py --asgame; options.txt "-asgame" for the port).
TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(TOOLS, '..', '..'))
DBX = r'c:\prenos\dosbox-x-remc1\bin\x64\Release'

# the addresses of the dumps (linear in DOSBox): the state pointer AE400 / AE3F0 holds 1F690 and the settings pointer
# AE408 / AE3F8 583A0 in both games; render: the 3D view (2A700 / 2A740), 'ret' written there by --no_render
GAMES = {
    'mc1': { 'exe': 'CARPET.EXE', 'play': '20D9D0,26C400', 'maps': (0x28A1E0, 0x29A1E0, 0x2BA1E0), 'render': 0x1FB700 },
    'mc1hw': { 'exe': 'HIDDEN.EXE', 'play': '20DD10,26C3F0', 'maps': (0x28A1D0, 0x29A1D0, 0x2BA1D0), 'render': 0x1FB740 },
}
# --as_game (as dbxrecord.py --asgame, the port -asgame), MC1 only: at the call of 34B40 (340D9) the start of -custom
# patched: jnz 34113 -> nop nop (InitMenu 4A980 called), jmp 3411A -> jmp 3411C (then the -custom start), push 2575D8
# (ftext) -> 257580 (etext) at 3411F; at the first 20FB0 the flags of -custom (settings 583A0 + 1, 100h: no level won
# check) cleared and the menu item byte_12CBCE 5 (results after a level)
AS_GAME_POKES = '2050D9,205113,9090;2050D9,20511A,EB00;2050D9,20511F,80752500;1F1FB0,583A1,00;1F1FB0,2EABCE,05'


def actions_input(frames, seed, spells=tuple(range(24))):
    # a random flight: the cheats (all spells, mana), keys of spells, the spells grid and the map, both buttons
    rnd = random.Random(seed)
    rec = bytearray()
    segment_end = 0
    mode_end = -1
    mode = 0
    move_x = move_y = 0
    buttons = 0
    for f in range(frames):
        command, param, param2 = 0, 0, 0
        if f == 0:
            command = 1
        elif f == 50:
            command, param = 30, 1
        elif f == 55:
            command, param = 30, 2
        elif f == 60:
            command, param, param2 = 23, 1, 0
        elif f == 61:
            command, param, param2 = 23, 2, 16
        if mode and f == mode_end:
            if mode == 2:
                command, param = rnd.choice(((21, rnd.choice(spells)), (22, rnd.choice(spells))))
            else:
                command, param = 20, 0
            mode = 0
            segment_end = f + 1
        elif not mode and f >= 100 and f >= segment_end:
            segment_end = f + rnd.randint(40, 250)
            move_x = rnd.choice((0, 0, 0, 10, -10, 25, -25, 40))
            move_y = rnd.choice((0, 0, 0, 8, -8, 15, -15))
            buttons = rnd.choice((1, 1, 1, 1, 0, 2, 1 | 4, 1 | 8))
            r = rnd.random()
            if r < 0.2:
                mode, mode_end = 2, f + rnd.randint(10, 60)
                command, param = 20, 2
            elif r < 0.3:
                mode, mode_end = 4, f + rnd.randint(10, 60)
                command, param = 20, 4
            elif r < 0.4:
                command, param, param2 = 23, rnd.randint(0, 9), rnd.choice(spells)
            else:
                command, param = rnd.choice(((21, rnd.choice(spells)), (22, rnd.choice(spells))))
        b = buttons if not mode else 0
        if f >= 100 and not mode:
            if rnd.random() < 0.06:
                b |= 0x10
            if rnd.random() < 0.04:
                b |= 0x20
        on = f >= 100 and not mode
        rec += struct.pack('<Bbbbb B4s', command, param, param2, move_x if on else 0, move_y if on else 0,
                           b if f >= 100 else 0, bytes(4))
    return bytes(rec)


def idle_input(frames):
    # the player joins (command 1), then nothing
    return bytes([1] + [0] * 9) + bytes(10 * (frames - 1))


GAME = r'c:\prenos\dosbox-x-remc1\mc1-orig'


def copy_game(target):
    # the original copies its data files to C:\CARPET.CD at every start and reads them while it runs: each DOSBox gets
    # its own copy of the game (without the intro movies), so DOSBoxes in parallel do not read a file being rewritten
    shutil.copytree(GAME, target, ignore=shutil.ignore_patterns('INTRO'))


def make_conf(exe_line, out, game):
    subprocess.run([sys.executable, os.path.join(TOOLS, 'mkconf.py'), exe_line, out], check=True)
    text = open(out).read().replace('\ncycles=auto\n', '\ncycles=max\n')
    text = text.replace('mount c c:/prenos/dosbox-x-remc1/mc1-orig/', 'mount c %s/' % game.replace('\\', '/'))
    open(out, 'w').write(text)


def make_test(game, level, frames, every, kind, pack_exe, out_root, seed=None, spells=tuple(range(24)), render=True, input_file=None,
              as_game=False):
    g = GAMES[game]
    folder = os.path.join(out_root, 'level%03d' % (level + 1))
    os.makedirs(folder, exist_ok=True)
    options = os.path.join(folder, 'options.txt')
    if as_game:
        assert game == 'mc1'
        open(options, 'w').write('-asgame\n')
    elif os.path.exists(options):
        os.remove(options)
    if seed is None:
        seed = (100 if game == 'mc1' else 200) + level
    data = actions_input(frames + 10, seed, spells) if kind == 'actions' else idle_input(frames + 10)
    if input_file:
        data = open(input_file, 'rb').read()
    open(os.path.join(folder, 'input.rec'), 'wb').write(data)
    work = tempfile.mkdtemp(prefix='mc1reg')
    try:
        conf = os.path.join(work, 'dbx.conf')
        game_dir = os.path.join(work, 'game')
        copy_game(game_dir)
        make_conf('%s -custom -level %d' % (g['exe'], level), conf, game_dir)
        dumps = [
            '1F1FB0,1F694,4,%s' % os.path.join(work, 'rand.bin'),
            '1F1FB0,26AF3,280A0,%s' % os.path.join(work, 'entities.bin'),
            '1F1FB0,22A9B,4008,%s' % os.path.join(work, 'players.bin'),
            '1F1FB0,583A0,8E7E,%s' % os.path.join(work, 'settings.bin'),
        ] + ['1F1FB0,%X,10000,%s' % (a, os.path.join(work, n)) for a, n in zip(g['maps'], ('terrain.bin', 'height.bin', 'angle.bin'))]
        env = dict(os.environ)
        env['REMC1_PLAY'] = '%s,7413,A,%s' % (g['play'], os.path.join(folder, 'input.rec'))
        env['REMC1_DUMP'] = ';'.join(dumps)
        env['REMC1_DUMP_EVERY'] = str(every)
        env['REMC1_DUMP_HIT'] = str(frames)
        env['REMC1_DUMP_EXIT'] = '1'
        pokes = []
        if not render:
            # the original without the 3D view: the HW one overwrites its tmap cache and divides by a frame of height 0
            pokes.append('1F1FB0,%X,C3' % g['render'])
        if as_game:
            pokes.append(AS_GAME_POKES)
        if pokes:
            env['REMC1_POKE'] = ';'.join(pokes)
        subprocess.run([os.path.join(DBX, 'dosbox-x.exe'), '-conf', conf], cwd=DBX, env=env,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=3600)
        reference = os.path.join(folder, 'reference.binz')
        entities = os.path.join(work, 'entities.bin')
        dumped = os.path.getsize(entities) // 0x280A0 if os.path.exists(entities) else 0
        if dumped != frames // every:
            # the original crashed or DOSBox timed out: no partial reference
            print('%s level %d: FAILED (%d of %d dumps)' % (game, level, dumped, frames // every), flush=True)
            return False
        result = subprocess.run([pack_exe, '--pack_mc1', work, reference, str(every)])
        ok = result.returncode == 0 and os.path.exists(reference)
        print('%s level %d: %s' % (game, level, ('%d bytes' % os.path.getsize(reference)) if ok else 'FAILED'), flush=True)
        return ok
    finally:
        shutil.rmtree(work, ignore_errors=True)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('game', choices=sorted(GAMES))
    parser.add_argument('levels', nargs='+', type=int)
    parser.add_argument('--frames', type=int, default=2000)
    parser.add_argument('--every', type=int, default=100)
    parser.add_argument('--input', choices=('actions', 'idle'), default='actions')
    parser.add_argument('--pack_exe', default=os.path.join(ROOT, 'x64', 'Release', 'remc2-regression-test.exe'))
    parser.add_argument('--out', default=None)
    parser.add_argument('--seed', type=int, default=None)
    parser.add_argument('--no_render', action='store_true', help='the original without the 3D view')
    parser.add_argument('--input_file', default=None, help='the input.rec to use (10 bytes a frame)')
    parser.add_argument('--spells', default=None, help='the spells of the actions input, e.g. 0-15,17')
    parser.add_argument('--as_game', action='store_true', help='the level as started from the menu (MC1)')
    args = parser.parse_args()
    out_root = os.path.abspath(args.out or os.path.join(ROOT, 'remc2-regression-test', 'memimages', 'regressions-' + args.game))  # DOSBox runs in its own folder
    spells = tuple(range(24))
    if args.spells:
        spells = []
        for part in args.spells.split(','):
            a, _, b = part.partition('-')
            spells += range(int(a), int(b or a) + 1)
        spells = tuple(spells)
    failed = 0
    for level in args.levels:
        if not make_test(args.game, level, args.frames, args.every, args.input, args.pack_exe, out_root, args.seed, spells, not args.no_render, args.input_file,
                         args.as_game):
            failed += 1
    sys.exit(1 if failed else 0)


main()
