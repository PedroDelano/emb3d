"""Manim scene: how a hashmap works.

Fixed version:
  - Big, readable hash() bubble with visible computation.
  - Collisions resolved by prepending to a chain; nodes never overlap.
  - Hash value is shown for each insert.

Render with:
    uv run manim -ql --format=gif manim_hashmap.py Hashmap
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

# ---- layout constants --------------------------------------------------
N_SLOTS   = 5
SLOT_W    = 1.0
SLOT_H    = 0.75
SLOT_GAP  = 0.08

NODE_W    = 1.3
NODE_H    = 0.75
NODE_GAP  = 0.55          # horizontal gap between adjacent nodes in a chain
SLOT_TO_NODE_GAP = 0.5    # horizontal gap between slot and its head node

# demo sequence: (key, raw_hash, slot, is_collision)
# raw_hash is a plausible output of a multiplicative hash (value * 13 + char, summed).
# We choose values so raw_hash % 5 matches the intended slot.
DEMO = [
    ("cat",  113, 3, False),   # 113 % 5 = 3
    ("dog",  106, 1, False),   # 106 % 5 = 1
    ("bat",  138, 3, True),    # 138 % 5 = 3  -> collision with "cat"
    ("fish",  90, 0, False),   #  90 % 5 = 0
]


# ---- helpers -----------------------------------------------------------
def slot_box(i, color=INK):
    box = Rectangle(width=SLOT_W, height=SLOT_H,
                    stroke_color=color, stroke_width=3)
    idx = Text(str(i), font="DejaVu Sans Mono",
               color=MUTE, font_size=18)
    idx.move_to(box.get_center())
    return VGroup(box, idx)


def node_mobject(key, color=INK, fill=None, width=NODE_W, height=NODE_H):
    box = Rectangle(width=width, height=height,
                    stroke_color=color, stroke_width=3)
    if fill is not None:
        box.set_fill(fill, opacity=1.0)
    lbl = Text(f'"{key}"', font="DejaVu Sans Mono",
               color=color, font_size=24)
    lbl.move_to(box.get_center())
    return VGroup(box, lbl)


def arrow_between(a, b, color=INK, width=3):
    return Arrow(
        a.get_right() + RIGHT * 0.03,
        b.get_left()  + LEFT  * 0.03,
        buff=0, color=color, stroke_width=width,
        max_tip_length_to_length_ratio=0.22,
    )


# ---- scene -------------------------------------------------------------
class Hashmap(Scene):
    def construct(self):
        self.camera.background_color = BG

        # -------- title ------------------------------------------------
        title = Text("Hashmap: a key becomes a slot index",
                     font="DejaVu Sans", color=INK, font_size=28, weight=BOLD)
        title.to_edge(UP, buff=0.35)
        self.play(Write(title), run_time=0.7)

        # -------- slots -------------------------------------------------
        slots = VGroup(*[slot_box(i) for i in range(N_SLOTS)])
        slots.arrange(DOWN, buff=SLOT_GAP)
        slots.move_to([-5.3, -0.3, 0])
        slots_label = Text("slots", font="DejaVu Sans Mono",
                           color=MUTE, font_size=18)
        slots_label.next_to(slots, UP, buff=0.22)

        self.play(
            LaggedStart(*[FadeIn(s, shift=RIGHT * 0.2) for s in slots],
                        lag_ratio=0.08),
            FadeIn(slots_label),
            run_time=1.0,
        )

        # -------- the hash() bubble (big, readable) -------------------
        hash_bubble = RoundedRectangle(
            width=5.0, height=2.0, corner_radius=0.22,
            stroke_color=BLUE, stroke_width=3,
        ).set_fill(BLUE, opacity=0.08)
        hash_bubble.move_to([1.9, 1.9, 0])

        hash_title = Text("hash(key) % 5", font="DejaVu Sans Mono",
                          color=BLUE, font_size=26, weight=BOLD)
        hash_title.move_to(hash_bubble.get_top() + DOWN * 0.38)

        hash_hint = Text("hash(str) = sum(c * 13^i) over chars",
                         font="DejaVu Sans", color=MUTE, font_size=16)
        hash_hint.next_to(hash_title, DOWN, buff=0.18)

        self.play(
            FadeIn(hash_bubble),
            Write(hash_title),
            FadeIn(hash_hint),
            run_time=0.8,
        )
        self.wait(0.8)

        # -------- helper that animates one insert ----------------------
        # chains[slot] = list of node mobjects in that slot, head first
        chains: dict[int, list[VGroup]] = {i: [] for i in range(N_SLOTS)}
        caption = [None]   # mutable holder so nested fn can replace it

        def set_caption(new_text, color=INK, weight=BOLD, size=22, font="DejaVu Sans Mono"):
            new = Text(new_text, font=font, color=color,
                       font_size=size, weight=weight)
            new.to_edge(DOWN, buff=0.35)
            if caption[0] is None:
                caption[0] = new
                self.play(FadeIn(new), run_time=0.3)
            else:
                self.play(Transform(caption[0], new), run_time=0.3)

        def do_insert(key, raw_hash, slot_idx, is_collision):
            set_caption(f'insert("{key}")',
                        color=ACCENT if is_collision else INK)

            # (1) key appears on the far right
            src_key = node_mobject(key, color=INK)
            src_key.move_to([5.2, 1.9, 0])
            self.play(FadeIn(src_key, shift=LEFT * 0.3), run_time=0.35)

            # (2) it travels into the hash bubble
            compute_pos = hash_bubble.get_bottom() + UP * 0.48
            self.play(
                src_key.animate.move_to(compute_pos).scale(0.85),
                run_time=0.55,
            )

            # (3) show the computation: `hash("key") = raw % 5 = slot_idx`
            comp_1 = Text(f'hash("{key}") = {raw_hash} % 5',
                          font="DejaVu Sans Mono", color=INK, font_size=22)
            comp_1.next_to(hash_bubble, DOWN, buff=0.25)
            comp_2 = Text(f'hash("{key}") = {raw_hash} % 5 = {slot_idx}',
                          font="DejaVu Sans Mono", color=ACCENT,
                          font_size=22, weight=BOLD)
            comp_2.next_to(hash_bubble, DOWN, buff=0.25)

            self.play(FadeIn(comp_1), run_time=0.45)
            self.wait(0.4)
            self.play(Transform(comp_1, comp_2), run_time=0.45)
            self.wait(0.5)

            # (4) place the node in (or at the head of) the chain at slot_idx
            target_slot = slots[slot_idx]
            slot_y = target_slot.get_center()[1]

            # final target geometry for this chain after this insert
            existing = chains[slot_idx]
            new_head = node_mobject(
                key,
                color=ACCENT if is_collision else INK,
                fill=("#FFE6CC" if is_collision else None),
            )
            # position the new head immediately to the right of the slot
            head_x = target_slot.get_right()[0] + SLOT_TO_NODE_GAP + NODE_W / 2
            new_head.move_to([head_x, slot_y, 0])

            if existing:
                # shift existing nodes (and their slot arrow) rightward to make room
                shift_amount = (NODE_W + NODE_GAP)
                old_slot_arrow = getattr(self, f"slot_arrow_{slot_idx}", None)
                movers = VGroup(*existing)
                chain_arrows = getattr(self, f"chain_arrows_{slot_idx}", [])
                self.play(
                    movers.animate.shift(RIGHT * shift_amount),
                    *([old_slot_arrow.animate.shift(RIGHT * 0)]
                      if old_slot_arrow is not None else []),
                    *[a.animate.shift(RIGHT * shift_amount) for a in chain_arrows],
                    run_time=0.45,
                )
                # We also need to remove the OLD slot->old_head arrow and
                # replace it with a new slot->new_head arrow, because the old
                # arrow's target just moved away.
                if old_slot_arrow is not None:
                    self.remove(old_slot_arrow)

            # animate the key sliding from the hash bubble to its slot
            self.play(
                Transform(src_key, new_head),
                run_time=0.6,
            )

            # arrow from slot to new head
            slot_arrow = arrow_between(target_slot, new_head, color=INK)
            self.play(GrowArrow(slot_arrow), run_time=0.35)
            setattr(self, f"slot_arrow_{slot_idx}", slot_arrow)

            # if this was a collision, draw a chain arrow from new head to
            # the previous head (which has been shifted right already)
            if existing:
                chain_arr = arrow_between(new_head, existing[0], color=ACCENT)
                self.play(GrowArrow(chain_arr), run_time=0.35)
                cas = getattr(self, f"chain_arrows_{slot_idx}", [])
                cas.append(chain_arr)
                setattr(self, f"chain_arrows_{slot_idx}", cas)

            # update bookkeeping: new head goes at index 0
            chains[slot_idx].insert(0, new_head)

            # clean computation text
            self.play(FadeOut(comp_1), run_time=0.3)

        # -------- run the demo -----------------------------------------
        for key, raw_hash, slot_idx, is_collision in DEMO:
            do_insert(key, raw_hash, slot_idx, is_collision)
            self.wait(0.25)

        # -------- closing beat -----------------------------------------
        closing = Text("Same slot? Chain them. Average O(1) lookup.",
                       font="DejaVu Sans", color=MUTE, font_size=22)
        closing.to_edge(DOWN, buff=0.35)
        self.play(Transform(caption[0], closing), run_time=0.6)
        self.wait(1.8)
