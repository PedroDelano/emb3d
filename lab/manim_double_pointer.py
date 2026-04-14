"""Manim scene: visualising `double **data` as a pointer to a pointer to a double.

Render a GIF with:
    uv run manim -ql --format=gif manim_double_pointer.py DoublePointer
"""

from manim import *

INK    = "#222222"
MUTE   = "#8A8A8A"
DIM    = "#BEBEBE"
ACCENT = "#E67E22"
BG     = "#FAFAFA"

# reasonable defaults for a short explainer clip
config.background_color = BG
config.pixel_width  = 960
config.pixel_height = 540
config.frame_rate   = 30


def labeled_box(text, width=1.7, height=0.8, color=INK, fill=None, font_size=24,
                font="DejaVu Sans Mono"):
    box = Rectangle(width=width, height=height, stroke_color=color, stroke_width=3)
    if fill is not None:
        box.set_fill(fill, opacity=1.0)
    t = Text(text, font=font, color=color, font_size=font_size)
    t.move_to(box.get_center())
    return VGroup(box, t)


class DoublePointer(Scene):
    def construct(self):
        self.camera.background_color = BG

        # -------- title ------------------------------------------------
        title = Text("A matrix in C is a pointer to a pointer",
                     font="DejaVu Sans", color=INK, font_size=28, weight=BOLD)
        title.to_edge(UP, buff=0.4)
        self.play(Write(title), run_time=0.8)

        # -------- the `data` handle ------------------------------------
        data_handle = labeled_box("data", color=INK, font_size=28)
        data_handle.move_to([-5.2, 0, 0])
        data_type = Text("double **", font="DejaVu Sans", color=MUTE, font_size=20)
        data_type.next_to(data_handle, UP, buff=0.15)
        self.play(FadeIn(data_handle, shift=UP * 0.2), FadeIn(data_type), run_time=0.7)
        self.wait(0.8)

        # -------- row-pointer array ------------------------------------
        rp_labels = ["double *", "double *", "double *"]
        row_ptrs = VGroup(*[labeled_box(lbl, color=INK, font_size=20) for lbl in rp_labels])
        row_ptrs.arrange(DOWN, buff=0.35)
        row_ptrs.move_to([-1.2, 0, 0])
        array_caption = Text("array of row pointers",
                             font="DejaVu Sans", color=MUTE, font_size=18)
        array_caption.next_to(row_ptrs, UP, buff=0.2)

        arrow_data = Arrow(
            data_handle.get_right() + RIGHT * 0.1,
            row_ptrs[1].get_left() + LEFT * 0.1,
            color=INK, buff=0, stroke_width=4,
            max_tip_length_to_length_ratio=0.12,
        )

        self.play(
            LaggedStart(
                *[FadeIn(rp, shift=LEFT * 0.2) for rp in row_ptrs],
                lag_ratio=0.2,
            ),
            FadeIn(array_caption),
            GrowArrow(arrow_data),
            run_time=1.2,
        )
        self.wait(0.7)

        # -------- each row's cells -------------------------------------
        values = [
            [1.00, 0.50, 0.25, 0.10],
            [0.80, 0.42, 3.14, 0.70],
            [0.33, 0.91, 0.04, 0.62],
        ]
        rows_cells = []
        row_arrows = []
        for i, rp in enumerate(row_ptrs):
            cells = VGroup(*[
                labeled_box(f"{v:.2f}", width=1.0, height=0.8, color=INK, font_size=20)
                for v in values[i]
            ])
            cells.arrange(RIGHT, buff=0.08)
            cells.next_to(rp, RIGHT, buff=1.1)
            rows_cells.append(cells)
            arr = Arrow(
                rp.get_right() + RIGHT * 0.05,
                cells.get_left() + LEFT * 0.05,
                color=INK, buff=0, stroke_width=3,
                max_tip_length_to_length_ratio=0.15,
            )
            row_arrows.append(arr)

        self.play(
            *[GrowArrow(a) for a in row_arrows],
            *[FadeIn(c, shift=RIGHT * 0.2) for c in rows_cells],
            run_time=1.2,
        )
        self.wait(0.8)

        # -------- code caption at the bottom ---------------------------
        code_line = Text("data[i][j]   // reach one cell, two dereferences",
                         font="DejaVu Sans Mono", color=MUTE, font_size=22)
        code_line.to_edge(DOWN, buff=0.35)
        self.play(FadeIn(code_line), run_time=0.5)
        self.wait(0.6)

        # -------- first dereference: data[1] --------------------------
        # dim the other rows
        dimmed = VGroup(
            row_ptrs[0], row_ptrs[2],
            rows_cells[0], rows_cells[2],
            row_arrows[0], row_arrows[2],
        )
        self.play(dimmed.animate.set_opacity(0.25), run_time=0.4)

        # highlight data handle + path + row_ptrs[1]
        arrow_data_hl = arrow_data.copy().set_color(ACCENT).set_stroke(width=6)
        row_ptr_hl_box = row_ptrs[1][0].copy().set_stroke(color=ACCENT, width=5)
        row_ptr_hl_box.set_fill(ACCENT, opacity=0.15)
        row_ptr_hl_label = Text("data[1]", font="DejaVu Sans Mono",
                                color=ACCENT, font_size=22, weight=BOLD)
        row_ptr_hl_label.move_to(row_ptrs[1].get_center())

        deref1_label = Text("data[1]  →  row pointer 1",
                            font="DejaVu Sans Mono", color=ACCENT, font_size=24,
                            weight=BOLD)
        deref1_label.next_to(title, DOWN, buff=0.35)

        self.play(
            Create(arrow_data_hl),
            FadeIn(row_ptr_hl_box),
            FadeOut(row_ptrs[1][1]),
            FadeIn(row_ptr_hl_label),
            Write(deref1_label),
            run_time=1.0,
        )
        self.wait(1.0)

        # -------- second dereference: data[1][2] -----------------------
        row_arrow_hl = row_arrows[1].copy().set_color(ACCENT).set_stroke(width=6)
        target_cell = rows_cells[1][2]
        cell_hl_box = target_cell[0].copy().set_stroke(color=ACCENT, width=5)
        cell_hl_box.set_fill(ACCENT, opacity=0.20)
        cell_hl_label = Text("3.14", font="DejaVu Sans Mono",
                             color=ACCENT, font_size=22, weight=BOLD)
        cell_hl_label.move_to(target_cell.get_center())

        deref2_label = Text("data[1][2]  →  3.14",
                            font="DejaVu Sans Mono", color=ACCENT, font_size=24,
                            weight=BOLD)
        deref2_label.move_to(deref1_label)

        self.play(
            Transform(deref1_label, deref2_label),
            Create(row_arrow_hl),
            FadeIn(cell_hl_box),
            FadeOut(target_cell[1]),
            FadeIn(cell_hl_label),
            run_time=1.0,
        )
        self.wait(1.4)

        # -------- closing beat -----------------------------------------
        closing = Text("Two pointer hops to reach one number.",
                       font="DejaVu Sans", color=MUTE, font_size=20)
        closing.to_edge(DOWN, buff=0.35)
        self.play(Transform(code_line, closing), run_time=0.8)
        self.wait(1.8)
