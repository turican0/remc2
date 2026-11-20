import sys
from PIL import Image
scr, pal, w, h, out = sys.argv[1], sys.argv[2], int(sys.argv[3]), int(sys.argv[4]), sys.argv[5]
p = open(pal, 'rb').read()[:768]
mx = max(p)
p = bytes(min(255, c * 4) for c in p) if mx < 64 else p
im = Image.frombytes('P', (w, h), open(scr, 'rb').read()[:w * h])
im.putpalette(p)
im.convert('RGB').resize((w * 3, h * 3), Image.NEAREST).save(out)
