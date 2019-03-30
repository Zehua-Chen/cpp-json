import enum


class NumberLexer:

    class State(enum.Enum):
        BEFORE_DEC_POINT = 0
        AFTER_DEC_POINT = 1
        AFTER_E = 2

    def __init__(self, sign = 1, pre_dot = 0):
        self.sign = sign
        self.pre_dot = pre_dot
        self.after_dec_point = 0
        self.distance_after_dec_point = 0
        self.scale = 0
        self.scale_sign = 1
        self.state = NumberLexer.State.BEFORE_DEC_POINT

    def to_number(self) -> float:
        scale = self.scale * self.scale_sign
        return self.sign * (self.pre_dot + self.after_dec_point) * (10 ** scale)

    def take_number(self, number: int):

        if self.state == NumberLexer.State.BEFORE_DEC_POINT:
            self._add_before_dec_point(number)
        elif self.state == NumberLexer.State.AFTER_DEC_POINT:
            self._add_after_dec_point(number)
        elif self.state == NumberLexer.State.AFTER_E:
            self._add_scale(number)

    def go_after_dec_point(self):
        self.state = NumberLexer.State.AFTER_DEC_POINT
        self.distance_after_dec_point = 1

    def go_after_e(self):
        self.state = NumberLexer.State.AFTER_E

    def _add_after_dec_point(self, value: int):
        self.after_dec_point += value * \
            (1 / (10 ** self.distance_after_dec_point))
        self.distance_after_dec_point += 1

    def _add_before_dec_point(self, value: int):
        self.pre_dot *= 10
        self.pre_dot += value

    def _add_scale(self, value: int):
        self.scale *= 10
        self.scale += value
