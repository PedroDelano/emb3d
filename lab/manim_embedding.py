"""Manim scene: embedding lookup (token -> vector).

Render with:
    uv run manim -ql --format=gif manim_embedding.py Embedding
"""

from manim import *

INK    = "#222222"
MUTE   = "#8A8A8A"
DIM    = "#BEBEBE"
ACCENT = "#E67E22"
BLUE   = "#2E86DE"
BG     = "#FAFAFA"

config.background_color = BG
config.pixel_width  = 960
config.pixel_height = 540
config.frame_rate   = 30

VOCAB = ["the", "linux", "kernel", "network", "server"]
TARGET_IDX = 1   # "linux"

# a V x D embedding matrix of plausible-looking values
EMB = [
    [ 0.12, -0.34,  0.77,  0.02],
    [ 0.65, -0.18,  0.04,  0.44],   # "linux" -> the row we'll pluck
    [ 0.59, -0.21,  0.08,  0.50],   # "kernel" (nearby to "linux"!)
    [-0.11,  0.83,  0.22,  0.05],
    [ 0.28,  0.15, -0.41,  0.62],
]

CELL_W = 0.95
CELL_H = 0.55


def cell(value, color=INK, fill=None, w=CELL_W, h=CELL_H, fontsize=18):
    box = Rectangle(width=w, height=h, stroke_color=color, stroke_width=2)
    if fill is not None:
        box.set_fill(fill, opacity=1.0)
    lbl = Text(f"{value:+.2f}", font="DejaVu Sans Mono",
               color=color, font_size=fontsize)
    lbl.move_to(box.get_center())
    return VGroup(box, lbl)


def labeled_row(values, color=INK, fill=None):
    cells = VGroup(*[cell(v, color=color, fill=fill) for v in values])
    cells.arrange(RIGHT, buff=0.05)
    return cells


class Embedding(Scene):
    def construct(self):
        self.camera.background_color = BG

        # -------- title ------------------------------------------------
        title = Text("Embedding lookup: token ID -> vector",
                     font="DejaVu Sans", color=INK, font_size=28, weight=BOLD)
        title.to_edge(UP, buff=0.35)
        self.play(Write(title), run_time=0.7)

        # -------- the input word --------------------------------------
        word_box = RoundedRectangle(width=1.8, height=0.85, corner_radius=0.18,
                                    stroke_color=INK, stroke_width=3)
        word_box.set_fill("#FFFFFF", opacity=1.0)
        word_txt = Text('"linux"', font="DejaVu Sans Mono",
                        color=INK, font_size=26, weight=BOLD)
        word_txt.move_to(word_box.get_center())
        word = VGroup(word_box, word_txt)
        word.move_to([-5.6, 1.5, 0])

        self.play(FadeIn(word, shift=RIGHT * 0.2), run_time=0.5)
        self.wait(0.4)

        # -------- tokenizer step --------------------------------------
        tok_box = RoundedRectangle(width=2.2, height=0.85, corner_radius=0.18,
                                   stroke_color=BLUE, stroke_width=3)
        tok_box.set_fill(BLUE, opacity=0.08)
        tok_lbl = Text("tokenizer", font="DejaVu Sans Mono",
                       color=BLUE, font_size=22, weight=BOLD)
        tok_lbl.move_to(tok_box.get_center())
        tokenizer = VGroup(tok_box, tok_lbl)
        tokenizer.move_to([-3.0, 1.5, 0])

        arr_word_tok = Arrow(
            word.get_right() + RIGHT * 0.05,
            tokenizer.get_left() + LEFT * 0.05,
            buff=0, color=INK, stroke_width=3,
            max_tip_length_to_length_ratio=0.2,
        )

        self.play(GrowArrow(arr_word_tok), FadeIn(tokenizer), run_time=0.5)

        # token id appears
        tid_lbl = Text("token_id = 1", font="DejaVu Sans Mono",
                       color=ACCENT, font_size=22, weight=BOLD)
        tid_lbl.next_to(tokenizer, DOWN, buff=0.25)
        self.play(FadeIn(tid_lbl, shift=DOWN * 0.15), run_time=0.4)
        self.wait(0.5)

        # -------- the embedding matrix --------------------------------
        matrix_rows = VGroup(*[labeled_row(row) for row in EMB])
        matrix_rows.arrange(DOWN, buff=0.06)
        matrix_rows.move_to([1.6, -0.55, 0])

        # left-side labels showing token index + word
        idx_labels = VGroup()
        for i, row in enumerate(matrix_rows):
            lbl = Text(f'{i}  "{VOCAB[i]}"', font="DejaVu Sans Mono",
                       color=MUTE, font_size=16)
            lbl.next_to(row, LEFT, buff=0.2)
            idx_labels.add(lbl)

        matrix_caption = Text("embedding_matrix  (V x D)",
                              font="DejaVu Sans Mono", color=MUTE, font_size=18)
        matrix_caption.next_to(matrix_rows, UP, buff=0.25)

        self.play(
            FadeIn(matrix_caption),
            LaggedStart(
                *[FadeIn(r, shift=UP * 0.1) for r in matrix_rows],
                lag_ratio=0.06,
            ),
            LaggedStart(
                *[FadeIn(l) for l in idx_labels],
                lag_ratio=0.06,
            ),
            run_time=1.2,
        )
        self.wait(0.5)

        # -------- highlight the selected row --------------------------
        target_row = matrix_rows[TARGET_IDX]
        target_label = idx_labels[TARGET_IDX]

        # build the highlighted overlay row
        highlight = VGroup(*[
            cell(v, color=ACCENT, fill="#FFE6CC") for v in EMB[TARGET_IDX]
        ])
        highlight.arrange(RIGHT, buff=0.05)
        highlight.move_to(target_row.get_center())

        deref_cap = Text('embedding_matrix->data[1]',
                         font="DejaVu Sans Mono",
                         color=ACCENT, font_size=22, weight=BOLD)
        deref_cap.to_edge(DOWN, buff=0.4)

        # flash an indicator arrow from token_id to the row
        pick_arrow = Arrow(
            tid_lbl.get_bottom() + DOWN * 0.1,
            highlight.get_left() + LEFT * 0.15,
            buff=0, color=ACCENT, stroke_width=3,
            max_tip_length_to_length_ratio=0.12,
        )
        self.play(
            GrowArrow(pick_arrow),
            FadeIn(highlight),
            target_label.animate.set_color(ACCENT).set_weight(BOLD),
            FadeIn(deref_cap),
            run_time=0.8,
        )
        # dim the other rows for clarity
        others = VGroup(*[r for i, r in enumerate(matrix_rows) if i != TARGET_IDX])
        other_labels = VGroup(*[l for i, l in enumerate(idx_labels) if i != TARGET_IDX])
        self.play(
            others.animate.set_opacity(0.3),
            other_labels.animate.set_opacity(0.3),
            run_time=0.4,
        )
        self.wait(0.8)

        # -------- pull the row out as "the embedding" -----------------
        # create a copy of the highlighted row on the right side
        out_row = VGroup(*[
            cell(v, color=ACCENT, fill="#FFE6CC") for v in EMB[TARGET_IDX]
        ])
        out_row.arrange(RIGHT, buff=0.05)
        out_row.move_to([1.6, -2.8, 0])

        out_label = Text('embedding("linux")',
                         font="DejaVu Sans Mono",
                         color=ACCENT, font_size=22, weight=BOLD)
        out_label.next_to(out_row, LEFT, buff=0.3)

        # a ghost copy of `highlight` that animates down into `out_row`
        ghost = highlight.copy()
        self.play(
            Transform(ghost, out_row),
            FadeIn(out_label),
            run_time=0.9,
        )
        self.wait(0.8)

        # -------- closing caption -------------------------------------
        closing = Text("One pointer hop: data[token_id] is already a vector.",
                       font="DejaVu Sans", color=MUTE, font_size=20)
        closing.to_edge(DOWN, buff=0.4)
        self.play(Transform(deref_cap, closing), run_time=0.6)
        self.wait(1.8)
