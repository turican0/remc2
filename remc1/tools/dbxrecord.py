import os, subprocess, sys

# dbxrecord.py <level> <out.rec> [play] [--asgame] [--scale N]: the original MC1 (CARPET.EXE -custom -level <level>) in
# dosbox-x-remc1 with the input of player 0 recorded at 3C9D0 (10 bytes a frame); with "play" the file is played
# instead. The port plays the file with the env REMC1_PLAY=<out.rec> and the same level (remc2 -game mc1 -custom
# -level <level>).
# --asgame: the level as in the game started from the menu (the menu initialised, the English texts, the level won
# check, the results and the menu after the level, see AS_GAME_POKES; the port: -asgame). --scale N: the window of
# DOSBox N times the game (normalNx, the config has 2).
SCR = os.path.dirname(os.path.abspath(__file__))
DBX = r"c:\prenos\dosbox-x-remc1\bin\x64\Release"
# at the call of 34B40 (340D9) the start of -custom patched: jnz 34113 -> nop nop (InitMenu 4A980 called), jmp 3411A ->
# jmp 3411C (then the -custom start), push 2575D8 (ftext) -> 257580 (etext) at 3411F; at the first 20FB0 the flags of
# -custom (settings 583A0 + 1, 100h: no level won check) cleared and the menu item byte_12CBCE 5 (results after a level)
AS_GAME_POKES = "2050D9,205113,9090;2050D9,20511A,EB00;2050D9,20511F,80752500;1F1FB0,583A1,00;1F1FB0,2EABCE,05"
args = [a for a in sys.argv[1:]]
asGame = "--asgame" in args
scale = None
if "--scale" in args:
    scale = int(args[args.index("--scale") + 1])
    del args[args.index("--scale"):args.index("--scale") + 2]
args = [a for a in args if a != "--asgame"]
level, out = args[0], os.path.abspath(args[1])
mode = "REMC1_PLAY" if len(args) > 2 and args[2] == "play" else "REMC1_RECORD"
conf = os.path.join(SCR, "rec%s.conf" % level)
subprocess.run([sys.executable, os.path.join(SCR, "mkconf.py"), "CARPET.EXE -custom -level %s" % level, conf], check=True)
if scale:
    text = open(conf).read()
    text = "\n".join(("scaler=normal%dx" % scale) if line.strip().startswith("scaler=") else line for line in text.splitlines())
    open(conf, "w").write(text + "\n")
env = dict(os.environ)
env[mode] = "20D9D0,26C400,7413,A,%s" % out
if asGame:
    env["REMC1_POKE"] = AS_GAME_POKES
try:
    subprocess.run([os.path.join(DBX, "dosbox-x.exe"), "-conf", conf], cwd=DBX, env=env)
finally:
    os.remove(conf)
if os.path.exists(out):
    print(out, os.path.getsize(out) // 10, "frames")
