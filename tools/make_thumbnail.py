"""Generate a YouTube-style thumbnail for the Heartbeat Monitor project."""
import math, sys, os

try:
    from PIL import Image, ImageDraw, ImageFont
except ImportError:
    import subprocess
    subprocess.check_call([sys.executable, "-m", "pip", "install", "Pillow", "-q"])
    from PIL import Image, ImageDraw, ImageFont

W, H = 1280, 720

# ---------- background gradient ----------
top = (11, 16, 38)
bot = (4, 7, 16)
img = Image.new("RGB", (W, H))
px = img.load()
for y in range(H):
    t = y / (H - 1)
    r = int(top[0] + (bot[0] - top[0]) * t)
    g = int(top[1] + (bot[1] - top[1]) * t)
    b = int(top[2] + (bot[2] - top[2]) * t)
    for x in range(W):
        px[x, y] = (r, g, b)
d = ImageDraw.Draw(img, "RGBA")

# ---------- faint ECG grid ----------
for x in range(0, W, 40):
    d.line([(x, 0), (x, H)], fill=(20, 60, 70, 35), width=1)
for y in range(0, H, 40):
    d.line([(0, y), (W, y)], fill=(20, 60, 70, 35), width=1)

def lerp(a, b, t):
    return tuple(int(a[i] + (b[i] - a[i]) * t) for i in range(3))

# ---------- glowing heart ----------
heart = []
for i in range(0, 630):
    t = i * 0.01
    x = 16 * math.sin(t) ** 3
    y = 13 * math.cos(t) - 5 * math.cos(2 * t) - 2 * math.cos(3 * t) - math.cos(4 * t)
    heart.append((x, -y))
cx, cy, sc = 250, 320, 9.2
pts = [(cx + x * sc, cy + y * sc) for (x, y) in heart]
# glow
for grow, ga in [(34, 60), (26, 90), (18, 140)]:
    d.polygon([(cx + (x - cx) * (grow / 18.0), cy + (y - cy) * (grow / 18.0)) for (x, y) in pts],
              fill=(220, 30, 50, ga))
d.polygon(pts, fill=(225, 35, 55))
d.polygon(pts, outline=(255, 120, 140), width=3)

# ---------- ECG waveform with glow ----------
baseline = 470
beats = [430, 930]
wave = []
for x in range(0, W):
    y = baseline
    for c in beats:
        dist = abs(x - c)
        if dist < 34:
            y = baseline - 210 * (1 - dist / 34)
        elif dist < 70:
            y = baseline + 40 * (1 - (dist - 34) / 36)
    wave.append((x, y))
# glow underlay
for w, col in [(16, (20, 120, 60, 70)), (10, (30, 200, 90, 120)), (4, (120, 255, 170, 255))]:
    d.line(wave, fill=col, width=w, joint="curve")

# ---------- fonts ----------
def font(paths, size):
    for p in paths:
        try:
            return ImageFont.truetype(p, size)
        except Exception:
            continue
    return ImageFont.load_default()

title_f = font(["C:/Windows/Fonts/IMPACT.TTF", "C:/Windows/Fonts/arialbd.ttf"], 122)
sub_f = font(["C:/Windows/Fonts/arialbd.ttf", "C:/Windows/Fonts/arial.ttf"], 46)
badge_f = font(["C:/Windows/Fonts/arial.ttf", "C:/Windows/Fonts/arialbd.ttf"], 34)
oled_f = font(["C:/Windows/Fonts/consolab.ttf", "C:/Windows/Fonts/arialbd.ttf"], 40)

def center_text(text, y, fnt, fill, sh=None):
    tw = d.textlength(text, font=fnt)
    x = (W - tw) / 2
    if sh:
        d.text((x + 4, y + 4), text, font=fnt, fill=sh)
    d.text((x, y), text, font=fnt, fill=fill)

# ---------- title ----------
center_text("HEARTBEAT MONITOR", 60, title_f, (255, 255, 255), sh=(180, 20, 35))
center_text("WITH  ALERT  SYSTEM", 195, sub_f, (90, 255, 170))

# ---------- OLED-style readout (right) ----------
ox, oy, ow, oh = 880, 250, 330, 200
d.rounded_rectangle([ox, oy, ox + ow, oy + oh], radius=14, fill=(8, 22, 16), outline=(40, 120, 80), width=4)
d.text((ox + 30, oy + 28), "OLED", font=badge_f, fill=(60, 160, 110))
d.text((ox + 30, oy + 70), "BPM  75", font=oled_f, fill=(120, 255, 180))
d.text((ox + 30, oy + 130), "NORMAL", font=oled_f, fill=(90, 255, 150))

# ---------- bottom badge ----------
center_text("ARDUINO   •   ESP32   •   WOKWI   •   PULSE SENSOR", 650, badge_f, (170, 185, 205))

# ---------- save ----------
out = r"C:\Users\harah\Desktop\heartbeat_monitor_thumbnail.png"
os.makedirs(os.path.dirname(out), exist_ok=True)
img.save(out)
print("SAVED:", out, img.size)
