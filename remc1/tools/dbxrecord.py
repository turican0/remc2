import os, subprocess, sys

# dbxrecord.py <level> <out.rec> [play]: the original MC1 (CARPET.EXE -custom -level <level>) in dosbox-x-remc1 with
# the input of player 0 recorded at 3C9D0 (10 bytes a frame); with "play" the file is played instead. The port plays
# the file with the env REMC1_PLAY=<out.rec> and the same level (remc2 -game mc1 -custom -level <level>).
SCR = os.path.dirname(os.path.abspath(__file__))
DBX = r"c:\prenos\dosbox-x-remc1\bin\x64\Release"
level, out = sys.argv[1], os.path.abspath(sys.argv[2])
mode = "REMC1_PLAY" if len(sys.argv) > 3 and sys.argv[3] == "play" else "REMC1_RECORD"
conf = os.path.join(SCR, "rec%s.conf" % level)
subprocess.run([sys.executable, os.path.join(SCR, "mkconf.py"), "CARPET.EXE -custom -level %s" % level, conf], check=True)
env = dict(os.environ)
env[mode] = "20D9D0,26C400,7413,A,%s" % out
subprocess.run([os.path.join(DBX, "dosbox-x.exe"), "-conf", conf], cwd=DBX, env=env)
if os.path.exists(out):
    print(out, os.path.getsize(out) // 10, "frames")
