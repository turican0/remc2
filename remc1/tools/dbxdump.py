import os, subprocess, sys

# dbxdump.py <mc1|hw> <code IDA addr> <outdir> name=ida_data_addr:len ... [name=@linear:len]
# runs the original game in dosbox-x-remc1 and dumps memory when the code address runs (IDA addresses, hex)
SCR = os.path.dirname(os.path.abspath(__file__))
DBX = r"c:\prenos\dosbox-x-remc1\bin\x64\Release"
variant, code, outdir = sys.argv[1], int(sys.argv[2], 16), sys.argv[3]
os.makedirs(outdir, exist_ok=True)
items = []
for arg in sys.argv[4:]:
    name, rest = arg.split("=")
    addr, length = rest.split(":")
    linear = int(addr[1:], 16) if addr.startswith("@") else int(addr, 16) + 0x1BE000
    items.append("%X,%X,%X,%s" % (code + 0x1D1000, linear, int(length, 16), os.path.join(outdir, variant + "_" + name + ".bin")))
env = dict(os.environ, REMC1_DUMP=";".join(items), REMC1_DUMP_EXIT="1")
conf = os.environ.get("DBXCONF") or os.path.join(SCR, "carpet.conf" if variant == "mc1" else "hidden.conf")
try:
    subprocess.run([os.path.join(DBX, "dosbox-x.exe"), "-conf", conf], cwd=DBX, env=env, timeout=240,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
except subprocess.TimeoutExpired:
    print("timeout")
for i in items:
    f = i.split(",", 3)[3]
    print(os.path.basename(f), os.path.getsize(f) if os.path.exists(f) else "missing")
