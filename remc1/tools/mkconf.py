import sys

# mkconf.py <exe> <out.conf>: dosbox.conf of dosbox-x-remc1 with the normal core, a Pentium and autoexec starting <exe>
src = open(r"c:\prenos\dosbox-x-remc1\vs2015\dosbox.conf").read()
head, _, _ = src.partition("[autoexec]")
lines = []
for line in head.splitlines():
    s = line.strip()
    if s.startswith("core=") or s.startswith("core:") or s.startswith("core "):
        line = "core=normal"
    if s.startswith("cputype="):
        line = "cputype=pentium"  # the port detects a fast CPU (Pentium with FPU), as the MC2 port
    lines.append(line)
lines += ["[autoexec]", "mount c c:/prenos/dosbox-x-remc1/mc1-orig/", "c:", sys.argv[1], ""]
open(sys.argv[2], "w").write("\n".join(lines))
