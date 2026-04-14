"""Manim scene: backpropagation for the one-layer embedding model.

Walks through:
  1. Predicted distribution  (ŷ)  vs.  one-hot target  (y).
  2. (ŷ - y) is the gradient at the pre-softmax scores.
  3. Outer product with aᵀ (context vector) yields dL/dW.
  4. Scale by η.
  5. W ← W - η · dL/dW.  Show a couple of weights drift.

Render with:
    uv run manim -ql --format=gif manim_backprop.py Backprop
"""

from manim import *

INK    = "#222222"
MUTE   = "#8A8A8A"
DIM    = "#BEBEBE"
ACCENT = "#E67E22"
BLUE   = "#2E86DE"
GREEN  = "#27AE60"
BG     = "#FAFAFA"

config.background_color = BG
config.pixel_width  = 960
config.pixel_height = 540
config.frame_rate   = 30

# tiny model: vocab_size=3, emb_size=3
VOCAB = ["the", "linux", "kernel"]
Y_HAT  = [0.50, 0.30, 0.20]   # prediction
Y_TRUE = [0.00, 0.00, 1.00]   # target is "kernel" (idx 2)
A_VEC  = [0.40, -0.20, 0.30]  # context vector a (shape 1x3 for outer product)

# derived
DIFF   = [round(yh - y, 2) for yh, y in zip(Y_HAT, Y_TRUE)]     # ŷ - y
LR     = 0.5  # dramatic for visibility
OUTER  = [[round(DIFF[i] * A_VEC[j], 3) for j in range(3)] for i in range(3)]
GRAD   = [[round(LR * OUTER[i][j], 3) for j in range(3)] for i in range(3)]
W_BEFORE = [
    [ 0.12, -0.20,  0.05],
    [ 0.30,  0.14, -0.08],
    [-0.15,  0.22,  0.40],
]
W_AFTER = [
    [round(W_BEFORE[i][j] - GRAD[i][j], 3) for j in range(3)]
    for i in range(3)
]

CELL_W = 0.85
CELL_H = 0.58


def cell(value, color=INK, fill=None, fontsize=18, w=CELL_W, h=CELL_H):
    box = Rectangle(width=w, height=h, stroke_color=color, stroke_width=2)
    if fill is not None:
        box.set_fill(fill, opacity=1.0)
    lbl = Text(f"{value:+.2f}", font="DejaVu Sans Mono",
               color=color, font_size=fontsize)
    lbl.move_to(box.get_center())
    return VGroup(box, lbl)


def vec_column(values, color=INK, fill=None):
    g = VGroup(*[cell(v, color=color, fill=fill) for v in values])
    g.arrange(DOWN, buff=0.05)
    return g


def mat(values, color=INK, fill=None):
    rows = VGroup()
    for r in values:
        row = VGroup(*[cell(v, color=color, fill=fill) for v in r])
        row.arrange(RIGHT, buff=0.05)
        rows.add(row)
    rows.arrange(DOWN, buff=0.05)
    return rows


class Backprop(Scene):
    def construct(self):
        self.camera.background_color = BG

        # -------- title ------------------------------------------------
        title = Text("Backprop: nudge the weights to be less wrong",
                     font="DejaVu Sans", color=INK, font_size=28, weight=BOLD)
        title.to_edge(UP, buff=0.35)
        self.play(Write(title), run_time=0.7)

        # helper: a two-line label (subtitle + variable) centered above a column
        def stacked_label(sub, var, color=INK, sub_size=16, var_size=26,
                          var_weight=BOLD):
            s = Text(sub, font="DejaVu Sans Mono", color=MUTE, font_size=sub_size)
            v = Text(var, font="DejaVu Sans Mono", color=color,
                     font_size=var_size, weight=var_weight)
            g = VGroup(s, v).arrange(DOWN, buff=0.08)
            return g

        # -------- phase 1: ŷ, y, and the gap --------------------------
        y_hat = vec_column(Y_HAT, color=INK)
        y_hat.move_to([-4.5, 0.3, 0])
        y_hat_lbl = stacked_label("prediction", "ŷ")
        y_hat_lbl.next_to(y_hat, UP, buff=0.25)

        y_true = vec_column(Y_TRUE, color=INK)
        y_true.move_to([-2.3, 0.3, 0])
        y_true_lbl = stacked_label("target", "y")
        y_true_lbl.next_to(y_true, UP, buff=0.25)

        minus = Text("-", font="DejaVu Sans Mono", color=INK,
                     font_size=40, weight=BOLD)
        minus.move_to([(y_hat.get_right()[0] + y_true.get_left()[0]) / 2,
                       y_hat.get_center()[1], 0])

        self.play(
            FadeIn(y_hat, shift=DOWN * 0.2), FadeIn(y_hat_lbl),
            run_time=0.5,
        )
        self.play(
            FadeIn(y_true, shift=DOWN * 0.2), FadeIn(y_true_lbl),
            FadeIn(minus),
            run_time=0.5,
        )

        # inline annotation: point at the "1.00" row of y to say "kernel"
        target_mark = Text('<- "kernel"', font="DejaVu Sans Mono",
                           color=MUTE, font_size=16)
        target_mark.next_to(y_true[2], RIGHT, buff=0.18)
        self.play(FadeIn(target_mark, shift=RIGHT * 0.15), run_time=0.35)
        self.wait(0.7)

        # subtract: result (ŷ - y)
        diff = vec_column(DIFF, color=ACCENT, fill="#FFE6CC")
        diff.move_to([0.8, 0.3, 0])
        diff_lbl = stacked_label("gradient at z", "ŷ - y",
                                 color=ACCENT, var_size=22)
        diff_lbl.next_to(diff, UP, buff=0.25)

        eq1 = Text("=", font="DejaVu Sans Mono", color=INK,
                   font_size=40, weight=BOLD)
        eq1.move_to([(y_true.get_right()[0] + target_mark.get_right()[0]) / 2
                     + 0.7,
                     diff.get_center()[1], 0])

        self.play(
            FadeIn(eq1),
            FadeIn(diff, shift=LEFT * 0.2), FadeIn(diff_lbl),
            run_time=0.7,
        )
        self.wait(1.0)

        # caption
        cap1 = Text("Step 1:  how wrong were we at each vocab slot?",
                    font="DejaVu Sans Mono", color=MUTE, font_size=20)
        cap1.to_edge(DOWN, buff=0.4)
        self.play(FadeIn(cap1), run_time=0.4)
        self.wait(1.0)

        # -------- phase 2: outer product with aᵀ ----------------------
        # clear phase 1 except the diff column
        phase1 = VGroup(y_hat, y_hat_lbl, y_true, y_true_lbl, minus,
                        target_mark, eq1, diff_lbl)
        self.play(FadeOut(phase1), run_time=0.5)

        # relabel and move diff to the left so it's the column of the outer product
        new_diff_lbl = Text("ŷ - y", font="DejaVu Sans Mono",
                            color=ACCENT, font_size=22, weight=BOLD)
        self.play(
            diff.animate.move_to([-4.5, 0.3, 0]),
            run_time=0.5,
        )
        new_diff_lbl.next_to(diff, UP, buff=0.25)
        self.play(FadeIn(new_diff_lbl), run_time=0.25)

        # aᵀ as a row of cells
        a_row = VGroup(*[cell(v, color=INK) for v in A_VEC])
        a_row.arrange(RIGHT, buff=0.05)
        a_row.move_to([-2.2, 1.2, 0])
        a_lbl = Text("aᵀ  (context)", font="DejaVu Sans Mono",
                     color=INK, font_size=20)
        a_lbl.next_to(a_row, UP, buff=0.2)

        times = Text("×", font="DejaVu Sans Mono", color=INK,
                     font_size=34, weight=BOLD)
        times.move_to([(diff.get_right()[0] + a_row.get_left()[0]) / 2,
                       diff.get_center()[1], 0])

        self.play(
            FadeIn(a_row, shift=DOWN * 0.15),
            FadeIn(a_lbl),
            FadeIn(times),
            run_time=0.6,
        )
        self.wait(0.5)

        # outer product result
        outer = mat(OUTER, color=ACCENT, fill="#FFE6CC")
        outer.move_to([2.0, 0.3, 0])
        outer_lbl = Text("dL/dW  =  (ŷ - y) · aᵀ",
                         font="DejaVu Sans Mono",
                         color=ACCENT, font_size=20, weight=BOLD)
        outer_lbl.next_to(outer, UP, buff=0.25)

        eq2 = Text("=", font="DejaVu Sans Mono", color=INK,
                   font_size=40, weight=BOLD)
        eq2.move_to([(a_row.get_right()[0] + outer.get_left()[0]) / 2,
                     outer.get_center()[1], 0])

        self.play(FadeIn(eq2), run_time=0.25)
        self.play(
            LaggedStart(
                *[FadeIn(r, shift=LEFT * 0.2) for r in outer],
                lag_ratio=0.1,
            ),
            FadeIn(outer_lbl),
            run_time=1.0,
        )
        self.wait(0.6)

        cap2 = Text("Step 2:  outer product gives the gradient for W",
                    font="DejaVu Sans Mono", color=MUTE, font_size=20)
        cap2.to_edge(DOWN, buff=0.4)
        self.play(Transform(cap1, cap2), run_time=0.4)
        self.wait(1.0)

        # -------- phase 3: scale by η and apply update ---------------
        # fade phase 2 left side, focus on the gradient matrix
        self.play(
            FadeOut(VGroup(diff, new_diff_lbl, a_row, a_lbl, times, eq2)),
            run_time=0.4,
        )
        # move outer to the right of center
        self.play(outer.animate.move_to([3.8, 0.3, 0]),
                  outer_lbl.animate.next_to(
                      outer.copy().move_to([3.8, 0.3, 0]), UP, buff=0.25),
                  run_time=0.5)

        # W on the left
        w_mat = mat(W_BEFORE, color=INK)
        w_mat.move_to([-2.5, 0.3, 0])
        w_lbl = Text("W (embedding matrix)", font="DejaVu Sans Mono",
                     color=INK, font_size=20, weight=BOLD)
        w_lbl.next_to(w_mat, UP, buff=0.25)
        self.play(
            FadeIn(w_mat, shift=RIGHT * 0.2),
            FadeIn(w_lbl),
            run_time=0.6,
        )

        minus2 = Text("-  η ·", font="DejaVu Sans Mono", color=INK,
                      font_size=28, weight=BOLD)
        minus2.move_to([(w_mat.get_right()[0] + outer.get_left()[0]) / 2,
                        outer.get_center()[1], 0])
        eta_note = Text(f"η = {LR}", font="DejaVu Sans Mono",
                        color=MUTE, font_size=18)
        eta_note.next_to(minus2, DOWN, buff=0.35)

        self.play(FadeIn(minus2), FadeIn(eta_note), run_time=0.35)

        cap3 = Text("Step 3:  W ← W - η · dL/dW",
                    font="DejaVu Sans Mono", color=ACCENT,
                    font_size=22, weight=BOLD)
        cap3.to_edge(DOWN, buff=0.4)
        self.play(Transform(cap1, cap3), run_time=0.4)
        self.wait(0.8)

        # apply the update: morph each W cell's value toward W_AFTER
        new_w = mat(W_AFTER, color=GREEN)
        new_w.move_to(w_mat.get_center())

        # animate each cell's text update + a brief flash on changed cells
        anims = []
        for i in range(3):
            for j in range(3):
                old = w_mat[i][j]
                new = new_w[i][j]
                if abs(W_AFTER[i][j] - W_BEFORE[i][j]) > 0.005:
                    anims.append(Transform(old, new))
                else:
                    anims.append(Transform(old, new.copy().set_color(INK)))
        self.play(*anims, run_time=1.0)
        self.wait(0.6)

        # -------- closing beat ----------------------------------------
        closing = Text("A tiny step toward being less wrong.  Repeat ×millions.",
                       font="DejaVu Sans", color=MUTE, font_size=20)
        closing.to_edge(DOWN, buff=0.4)
        self.play(Transform(cap1, closing), run_time=0.6)
        self.wait(1.8)
