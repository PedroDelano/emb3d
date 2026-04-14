"""Generate an animated GIF illustrating `double **data` — a pointer to a
pointer to a double — as used for the matrix struct in the emb3d project.

Frames:
 1. Just the `data` handle, labeled `double **`.
 2. `data` points to an array of row pointers (each `double *`).
 3. Each row pointer points to a contiguous row of doubles.
 4. Full static structure.
 5. First dereference: `data[1]` picks a row pointer.
 6. Second dereference: `data[1][2]` picks a cell.
 7. Hold on the final picture with the value highlighted.
"""

from PIL import Image, ImageDraw, ImageFont

W, H = 960, 520
BG = (250, 250, 250)
INK = (34, 34, 34)
MUTE = (140, 140, 140)
ACCENT = (230, 126, 34)   # orange — used for highlights
DIM = (200, 200, 200)

# --- fonts ---------------------------------------------------------------
def _font(size, bold=False):
    candidates = [
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf" if bold
        else "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf" if bold
        else "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    ]
    for path in candidates:
        try:
            return ImageFont.truetype(path, size)
        except OSError:
            continue
    return ImageFont.load_default()

F_TITLE = _font(22, bold=True)
F_LABEL = _font(16)
F_MONO  = _font(16)
F_MONO_B = _font(16, bold=True)
F_SMALL = _font(13)
F_BIG   = _font(28, bold=True)

# --- layout --------------------------------------------------------------
# data handle
DATA_BOX = (60, 230, 170, 280)

# row-pointer array (3 rows), center-left
RP_X0, RP_Y0 = 320, 140
RP_W, RP_H   = 110, 50
RP_GAP       = 20
N_ROWS       = 3

def row_ptr_box(i):
    y = RP_Y0 + i * (RP_H + RP_GAP)
    return (RP_X0, y, RP_X0 + RP_W, y + RP_H)

# row of doubles, to the right of each row pointer
ROW_X0     = 560
CELL_W, CELL_H = 70, 50
CELL_GAP   = 6
N_COLS     = 4

# the row values shown per row (just for readability)
VALUES = [
    [1.00, 0.50, 0.25, 0.10],
    [0.80, 0.42, 3.14, 0.70],   # data[1][2] = 3.14 → the highlighted cell
    [0.33, 0.91, 0.04, 0.62],
]

def cell_box(i, j):
    y_center = (row_ptr_box(i)[1] + row_ptr_box(i)[3]) / 2
    y = int(y_center - CELL_H / 2)
    x = ROW_X0 + j * (CELL_W + CELL_GAP)
    return (x, y, x + CELL_W, y + CELL_H)

# --- drawing helpers -----------------------------------------------------
def _text_wh(d, text, font):
    # PIL compatibility: textbbox (new), textsize (old).
    if hasattr(d, "textbbox"):
        l, t, r, b = d.textbbox((0, 0), text, font=font)
        return r - l, b - t
    return d.textsize(text, font=font)

def draw_box(d, box, label, label_font, color=INK, fill=None, text_color=None):
    if fill:
        d.rectangle(box, fill=fill, outline=color, width=2)
    else:
        d.rectangle(box, outline=color, width=2)
    tw, th = _text_wh(d, label, label_font)
    cx = (box[0] + box[2]) / 2 - tw / 2
    cy = (box[1] + box[3]) / 2 - th / 2
    d.text((cx, cy), label, fill=(text_color or color), font=label_font)

def arrow(d, x1, y1, x2, y2, color=INK, width=2, head=10):
    d.line((x1, y1, x2, y2), fill=color, width=width)
    # arrowhead
    import math
    ang = math.atan2(y2 - y1, x2 - x1)
    ax1 = x2 - head * math.cos(ang - math.pi / 7)
    ay1 = y2 - head * math.sin(ang - math.pi / 7)
    ax2 = x2 - head * math.cos(ang + math.pi / 7)
    ay2 = y2 - head * math.sin(ang + math.pi / 7)
    d.polygon([(x2, y2), (ax1, ay1), (ax2, ay2)], fill=color)

def right_mid(box):
    return (box[2], (box[1] + box[3]) / 2)

def left_mid(box):
    return (box[0], (box[1] + box[3]) / 2)

def top_mid(box):
    return ((box[0] + box[2]) / 2, box[1])

# --- frame builder -------------------------------------------------------
def base(title):
    img = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(img)
    d.text((30, 20), title, fill=INK, font=F_TITLE)
    # footer code line
    d.text((30, H - 36), "matrix->data[i][j]   //  a pointer to a pointer to a double",
           fill=MUTE, font=F_MONO)
    return img, d

def draw_data_handle(d, highlight=False):
    color = ACCENT if highlight else INK
    draw_box(d, DATA_BOX, "data", F_MONO_B, color=color)
    d.text((DATA_BOX[0], DATA_BOX[1] - 26), "double **", fill=MUTE, font=F_SMALL)

def draw_row_pointers(d, highlight_row=None, dim=False):
    for i in range(N_ROWS):
        box = row_ptr_box(i)
        if highlight_row == i:
            draw_box(d, box, f"data[{i}]", F_MONO_B, color=ACCENT,
                     fill=(255, 242, 224))
        elif dim:
            draw_box(d, box, f"double *", F_MONO, color=DIM, text_color=DIM)
        else:
            draw_box(d, box, f"double *", F_MONO)
    # bracket/label for "array of row pointers"
    d.text((RP_X0, RP_Y0 - 28),
           "array of row pointers", fill=MUTE, font=F_SMALL)

def draw_cells(d, highlight=None, rows_drawn=None, dim_rows=None):
    rows_drawn = rows_drawn if rows_drawn is not None else list(range(N_ROWS))
    dim_rows = dim_rows or []
    for i in rows_drawn:
        for j in range(N_COLS):
            box = cell_box(i, j)
            label = f"{VALUES[i][j]:.2f}"
            if highlight == (i, j):
                draw_box(d, box, label, F_MONO_B, color=ACCENT,
                         fill=(255, 224, 180))
            elif i in dim_rows:
                draw_box(d, box, label, F_MONO, color=DIM, text_color=DIM)
            else:
                draw_box(d, box, label, F_MONO)

def arrow_data_to_array(d, color=INK, target_row=None):
    x1, y1 = right_mid(DATA_BOX)
    if target_row is None:
        # point to the whole array — aim at left mid of center row
        x2, y2 = left_mid(row_ptr_box(1))
    else:
        x2, y2 = left_mid(row_ptr_box(target_row))
    arrow(d, x1 + 4, y1, x2 - 4, y2, color=color, width=2)

def arrows_rows_to_cells(d, rows=None, color=INK):
    rows = rows if rows is not None else list(range(N_ROWS))
    for i in rows:
        x1, y1 = right_mid(row_ptr_box(i))
        x2, y2 = left_mid(cell_box(i, 0))
        arrow(d, x1 + 4, y1, x2 - 4, y2, color=color, width=2)

# --- frames --------------------------------------------------------------
def frame1():
    img, d = base("A matrix, in C, is a pointer to a pointer")
    draw_data_handle(d)
    d.text((60, 310),
           "`data` is a handle. It doesn't hold\nthe matrix — it points to\nsomething that points to it.",
           fill=INK, font=F_LABEL)
    return img

def frame2():
    img, d = base("Step 1: data points to an array of row pointers")
    draw_data_handle(d)
    draw_row_pointers(d)
    arrow_data_to_array(d)
    d.text((60, 310),
           "Each slot in the array is itself\na pointer (`double *`) — one per\nrow of the matrix.",
           fill=INK, font=F_LABEL)
    return img

def frame3():
    img, d = base("Step 2: each row pointer points to a run of doubles")
    draw_data_handle(d)
    draw_row_pointers(d)
    draw_cells(d)
    arrow_data_to_array(d)
    arrows_rows_to_cells(d)
    d.text((60, 310),
           "The actual numbers live here,\ncontiguous in memory, one row\nat a time.",
           fill=INK, font=F_LABEL)
    return img

def frame4():
    img, d = base("To reach one cell you dereference twice")
    draw_data_handle(d)
    draw_row_pointers(d)
    draw_cells(d)
    arrow_data_to_array(d)
    arrows_rows_to_cells(d)
    d.text((60, 310),
           "data[i]      -> row pointer i\ndata[i][j]   -> the cell at (i, j)",
           fill=INK, font=F_MONO)
    return img

def frame5():
    """First dereference: data[1] highlighted."""
    img, d = base("First dereference:  data[1]  ->  row pointer 1")
    draw_data_handle(d, highlight=True)
    draw_row_pointers(d, highlight_row=1)
    draw_cells(d, dim_rows=[0, 2])
    arrow_data_to_array(d, color=ACCENT, target_row=1)
    arrows_rows_to_cells(d, rows=[0, 2], color=DIM)
    arrows_rows_to_cells(d, rows=[1], color=ACCENT)
    d.text((60, 310),
           "data[1] gives us the pointer\nto the 1st row of the matrix.\n(Still a pointer, not a value.)",
           fill=INK, font=F_LABEL)
    return img

def frame6():
    """Second dereference: data[1][2] highlighted."""
    img, d = base("Second dereference:  data[1][2]  ->  a double")
    draw_data_handle(d, highlight=True)
    draw_row_pointers(d, highlight_row=1)
    draw_cells(d, highlight=(1, 2), dim_rows=[0, 2])
    arrow_data_to_array(d, color=ACCENT, target_row=1)
    arrows_rows_to_cells(d, rows=[0, 2], color=DIM)
    arrows_rows_to_cells(d, rows=[1], color=ACCENT)
    d.text((60, 310),
           "data[1][2] indexes into that row\nand finally gives us the actual\ndouble stored at column 2.",
           fill=INK, font=F_LABEL)
    return img

def frame7():
    """Hold on the final value."""
    img, d = base("And there's the value.")
    draw_data_handle(d, highlight=True)
    draw_row_pointers(d, highlight_row=1)
    draw_cells(d, highlight=(1, 2), dim_rows=[0, 2])
    arrow_data_to_array(d, color=ACCENT, target_row=1)
    arrows_rows_to_cells(d, rows=[0, 2], color=DIM)
    arrows_rows_to_cells(d, rows=[1], color=ACCENT)
    # big value readout
    d.text((60, 300), "data[1][2] = ", fill=INK, font=F_MONO_B)
    d.text((230, 295), "3.14", fill=ACCENT, font=F_BIG)
    d.text((60, 355),
           "Two pointer hops to reach one\nnumber — no free lunch in C.",
           fill=MUTE, font=F_LABEL)
    return img

# --- assemble ------------------------------------------------------------
frames = [frame1(), frame2(), frame3(), frame4(), frame5(), frame6(), frame7()]
# per-frame duration (ms)
durations = [1800, 1800, 1800, 1800, 2400, 2400, 3000]

out = "/home/pedro/Documents/personal/emb3d/lab/double_pointer.gif"
frames[0].save(
    out,
    save_all=True,
    append_images=frames[1:],
    duration=durations,
    loop=0,
    optimize=True,
    disposal=2,
)
print(f"wrote {out}")
