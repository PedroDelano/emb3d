"""Manim scene: what is a linked list.

Render with:
    uv run manim -ql --format=gif manim_linked_list.py LinkedList
"""

from manim import *

INK    = "#222222"
MUTE   = "#8A8A8A"
DIM    = "#BEBEBE"
ACCENT = "#E67E22"
BG     = "#FAFAFA"

config.background_color = BG
config.pixel_width  = 960
config.pixel_height = 540
config.frame_rate   = 30

CELL_W  = 1.1
CELL_H  = 0.9


def node(value, color=INK, fill=None):
    """One node: [ data | next ].  Returns a VGroup of (data_box, next_box, value_label)."""
    data_box = Rectangle(width=CELL_W, height=CELL_H, stroke_color=color, stroke_width=3)
    next_box = Rectangle(width=CELL_W * 0.7, height=CELL_H,
                         stroke_color=color, stroke_width=3)
    if fill is not None:
        data_box.set_fill(fill, opacity=1.0)
        next_box.set_fill(fill, opacity=1.0)
    next_box.next_to(data_box, RIGHT, buff=0)
    val = Text(str(value), font="DejaVu Sans Mono", color=color, font_size=24)
    val.move_to(data_box.get_center())
    dot = Dot(radius=0.06, color=color).move_to(next_box.get_center())
    return VGroup(data_box, next_box, val, dot)


def node_data_box(n):
    return n[0]


def node_next_box(n):
    return n[1]


def node_dot(n):
    return n[3]


class LinkedList(Scene):
    def construct(self):
        self.camera.background_color = BG

        # -------- title ------------------------------------------------
        title = Text("A linked list is a chain of nodes",
                     font="DejaVu Sans", color=INK, font_size=30, weight=BOLD)
        title.to_edge(UP, buff=0.45)
        self.play(Write(title), run_time=0.8)

        # -------- a single node: anatomy -------------------------------
        anatomy = node("42", color=INK)
        anatomy.move_to(ORIGIN)

        brace_data = Brace(node_data_box(anatomy), DOWN, buff=0.15, color=MUTE)
        brace_next = Brace(node_next_box(anatomy), DOWN, buff=0.15, color=MUTE)
        data_lbl = Text("data", font="DejaVu Sans Mono", color=MUTE, font_size=20)
        next_lbl = Text("next", font="DejaVu Sans Mono", color=MUTE, font_size=20)
        data_lbl.next_to(brace_data, DOWN, buff=0.1)
        next_lbl.next_to(brace_next, DOWN, buff=0.1)

        self.play(FadeIn(anatomy, shift=UP * 0.2), run_time=0.6)
        self.play(
            GrowFromCenter(brace_data), GrowFromCenter(brace_next),
            FadeIn(data_lbl), FadeIn(next_lbl),
            run_time=0.8,
        )
        self.wait(1.2)

        caption = Text("Each node stores a value + a pointer to the next node.",
                       font="DejaVu Sans", color=MUTE, font_size=20)
        caption.to_edge(DOWN, buff=0.45)
        self.play(FadeIn(caption), run_time=0.5)
        self.wait(1.0)

        # -------- build a chain of three -------------------------------
        self.play(
            FadeOut(brace_data), FadeOut(brace_next),
            FadeOut(data_lbl), FadeOut(next_lbl),
            run_time=0.4,
        )

        # move `anatomy` to be first in a three-node chain (left side)
        n1 = anatomy
        n2 = node("13", color=INK)
        n3 = node("7", color=INK)
        chain = VGroup(n1.copy(), n2, n3).arrange(RIGHT, buff=1.0)
        chain.move_to([0, 0, 0])
        # animate n1 moving to chain position and new nodes appearing
        self.play(
            Transform(n1, chain[0]),
            run_time=0.6,
        )
        self.play(
            FadeIn(n2, shift=RIGHT * 0.2),
            FadeIn(n3, shift=RIGHT * 0.2),
            run_time=0.7,
        )

        # arrows from each next-box to the following node
        def next_arrow(src, dst, color=INK, width=4):
            return Arrow(
                node_dot(src).get_center(),
                node_data_box(dst).get_left(),
                buff=0.1, color=color, stroke_width=width,
                max_tip_length_to_length_ratio=0.18,
            )

        a12 = next_arrow(n1, n2)
        a23 = next_arrow(n2, n3)

        # NULL sentinel on the last next
        null_text = Text("NULL", font="DejaVu Sans Mono", color=MUTE, font_size=20)
        null_text.next_to(node_next_box(n3), RIGHT, buff=0.7)
        a3null = Arrow(
            node_dot(n3).get_center(),
            null_text.get_left(),
            buff=0.1, color=MUTE, stroke_width=3,
            max_tip_length_to_length_ratio=0.18,
        )

        # "head" label above n1
        head_label = Text("head", font="DejaVu Sans Mono", color=ACCENT,
                          font_size=22, weight=BOLD)
        head_label.next_to(n1, UP, buff=0.35)
        head_arrow = Arrow(
            head_label.get_bottom(),
            node_data_box(n1).get_top(),
            buff=0.08, color=ACCENT, stroke_width=3,
            max_tip_length_to_length_ratio=0.25,
        )

        self.play(
            GrowArrow(a12), GrowArrow(a23), GrowArrow(a3null),
            FadeIn(null_text),
            run_time=0.9,
        )
        self.play(FadeIn(head_label), GrowArrow(head_arrow), run_time=0.5)

        cap2 = Text("head -> 42 -> 13 -> 7 -> NULL",
                    font="DejaVu Sans Mono", color=MUTE, font_size=22)
        cap2.to_edge(DOWN, buff=0.45)
        self.play(Transform(caption, cap2), run_time=0.5)
        self.wait(1.5)

        # -------- insertion at head (O(1)) -----------------------------
        cap3 = Text("Insert at head:  new->next = head;  head = new;",
                    font="DejaVu Sans Mono", color=ACCENT, font_size=22,
                    weight=BOLD)
        cap3.to_edge(DOWN, buff=0.45)
        self.play(Transform(caption, cap3), run_time=0.5)

        # shift the existing chain rightward to make room
        existing = VGroup(n1, n2, n3, a12, a23, a3null, null_text,
                          head_label, head_arrow)

        new_node = node("99", color=ACCENT)
        new_node.set_color(ACCENT)
        # place new_node where n1 currently is
        new_node.move_to(n1.get_center())
        # shift the existing chain + new node rightward so everything fits
        shift_vec = RIGHT * 1.8
        self.play(
            existing.animate.shift(shift_vec),
            run_time=0.6,
        )

        # now new_node slides in from the left of n1's new position
        new_node.move_to(n1.get_center() + LEFT * 2.6)
        self.play(FadeIn(new_node, shift=RIGHT * 0.3), run_time=0.5)

        # draw new_node.next = head (pointer to n1)
        a_new = Arrow(
            node_dot(new_node).get_center(),
            node_data_box(n1).get_left(),
            buff=0.1, color=ACCENT, stroke_width=4,
            max_tip_length_to_length_ratio=0.2,
        )
        self.play(GrowArrow(a_new), run_time=0.6)

        # move "head" label + arrow to point at new_node
        new_head_label = head_label.copy().next_to(new_node, UP, buff=0.35)
        new_head_arrow = Arrow(
            new_head_label.get_bottom(),
            node_data_box(new_node).get_top(),
            buff=0.08, color=ACCENT, stroke_width=3,
            max_tip_length_to_length_ratio=0.25,
        )
        self.play(
            Transform(head_label, new_head_label),
            Transform(head_arrow, new_head_arrow),
            run_time=0.7,
        )
        self.wait(1.3)

        # -------- traversal --------------------------------------------
        cap4 = Text("Search:  walk next -> next -> next until NULL.",
                    font="DejaVu Sans Mono", color=INK, font_size=22,
                    weight=BOLD)
        cap4.to_edge(DOWN, buff=0.45)
        self.play(Transform(caption, cap4), run_time=0.5)

        # cursor highlight that hops along each node in turn
        def highlight_box(n):
            return node_data_box(n).copy().set_stroke(color=ACCENT, width=6)\
                .set_fill(ACCENT, opacity=0.18)

        cur = highlight_box(new_node)
        self.play(FadeIn(cur), run_time=0.3)
        self.wait(0.4)
        self.play(Transform(cur, highlight_box(n1)), run_time=0.45)
        self.wait(0.3)
        self.play(Transform(cur, highlight_box(n2)), run_time=0.45)
        self.wait(0.3)
        self.play(Transform(cur, highlight_box(n3)), run_time=0.45)
        self.wait(0.4)

        # land on NULL — fade cursor out, pulse NULL
        self.play(FadeOut(cur), null_text.animate.set_color(ACCENT), run_time=0.4)
        self.wait(0.4)

        # -------- closing beat -----------------------------------------
        closing = Text("O(1) insert at head.   O(n) search.",
                       font="DejaVu Sans", color=MUTE, font_size=22)
        closing.to_edge(DOWN, buff=0.45)
        self.play(Transform(caption, closing), run_time=0.6)
        self.wait(1.8)
