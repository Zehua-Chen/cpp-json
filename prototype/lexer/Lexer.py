import enum
from .Token import Token
from .LexerException import LexerException
from .NumberLexer import NumberLexer
from .PrimitiveLexer import PrimitiveLexer
from .StringLexer import StringLexer


class State(enum.Enum):
    START = 0
    STRING = 1
    FALSE = 2
    TRUE = 3
    NULL = 4
    NUMBER = 5


class Lexer:

    def __init__(self, text: str):
        # current letter and iterator
        self.current_iter = iter(text)
        self.current_letter = next(self.current_iter)
        # used for recording token
        self.token: Token = None
        # state information
        self.state = State.START
        self.sub_lexer = None
        self.should_continue = True
        # end of file
        self.is_finished = False
        self.has_token = False

        self._primitive = ""
        self._primitive_index = 0

    def take_letter(self, letter: str):
        self.has_token = False
        self.current_letter = letter

        self._handle_state()

    def _handle_state(self):

        if self.state == State.START:
            self._start_state()
        elif self.state == State.STRING:
            self._string_state()
        elif self.state == State.TRUE:
            self._true_state()
        elif self.state == State.FALSE:
            self._false_state()
        elif self.state == State.NULL:
            self._null_state()
        elif self.state == State.NUMBER:
            self._number_state()

    def _start_state(self):

        # {
        if self.current_letter == "{":
            self.token = Token.being_object()
            self.has_token = True
        # }
        elif self.current_letter == "}":
            self.token = Token.end_object()
            self.has_token = True
        # [
        elif self.current_letter == "[":
            self.token = Token.begin_array()
            self.has_token = True
        # ]
        elif self.current_letter == "]":
            self.token = Token.end_array()
            self.has_token = True
        # string
        elif self.current_letter == "'" or self.current_letter == "\"":
            self.state = State.STRING
            self.sub_lexer = StringLexer(self.current_letter)
        # ,
        elif self.current_letter == ",":
            self.token = Token.value_separator()
            self.has_token = True
        # :
        elif self.current_letter == ":":
            self.token = Token.key_value_separator()
            self.has_token = True
        # If matching false
        elif self.current_letter == "f":
            self.state = State.FALSE
            # self.sub_lexer = PrimitiveLexer(1, "false")
            self._primitive = "false"
            self._primitive_index = 1
        # if matching true
        elif self.current_letter == "t":
            self.state = State.TRUE
            # self.sub_lexer = PrimitiveLexer(1, "true")
            self._primitive = "true"
            self._primitive_index = 1
        # if matching null
        elif self.current_letter == "n":
            self.state = State.NULL
            self._primitive = "null"
            self._primitive_index = 1
        elif self.current_letter == " " \
                or self.current_letter == "\t" \
                or self.current_letter == "\n":
            pass
        # negative numbers
        elif self.current_letter == "-":
            self.state = State.NUMBER
            self.sub_lexer = NumberLexer(-1)
        # number
        elif self.current_letter.isnumeric():
            self.state = State.NUMBER
            self.sub_lexer = NumberLexer(
                sign=1, pre_dot=int(self.current_letter))
        # unexpected characters
        else:
            raise LexerException(self.current_letter)

    def _string_state(self):

        self.sub_lexer.take_letter(self.current_letter)

        if self.sub_lexer.is_completed:
            self.token = Token.string(self.sub_lexer.to_string())
            self.has_token = True
            self.state = State.START

        # self.current_letter = next(self.current_iter)

    def _true_state(self):

        if self._primitive[self._primitive_index] == self.current_letter:
            if self._primitive_index == len(self._primitive) - 1:
                self.token = Token.boolean(True)
                self.has_token = True
                self.state = State.START
            else:
                self._primitive_index += 1
        else:
            raise LexerException(self.current_letter)

    def _false_state(self):

        if self._primitive[self._primitive_index] == self.current_letter:
            if self._primitive_index == len(self._primitive) - 1:
                self.token = Token.boolean(False)
                self.has_token = True
                self.state = State.START
            else:
                self._primitive_index += 1
        else:
            raise LexerException(self.current_letter)

    def _null_state(self):

        if self._primitive[self._primitive_index] == self.current_letter:
            if self._primitive_index == len(self._primitive) - 1:
                self.token = Token.null()
                self.has_token = True
                self.state = State.START
            else:
                self._primitive_index += 1
        else:
            raise LexerException(self.current_letter)

    def _number_state(self):

        print("number state, letter = {}".format(self.current_letter))

        # End number
        if self.current_letter == "," \
                or self.current_letter == "}" \
                or self.current_letter == "]" \
                or self.current_letter == "\n" \
                or self.current_letter == "\r" \
                or self.current_letter == " ":
            self.token = Token.number(self.sub_lexer.to_number())
            self.state = State.START
            self.has_token = True
            return
        # - after e
        elif self.current_letter == "-":
            if self.sub_lexer.state == NumberLexer.State.AFTER_E:
                self.sub_lexer.scale_sign = -1
            else:
                raise LexerException(self.current_letter)
        # + after e
        elif self.current_letter == "+":
            if self.sub_lexer.state == NumberLexer.State.AFTER_E:
                self.sub_lexer.scale_sign = 1
            else:
                raise LexerException(self.current_letter)
        # . dot
        elif self.current_letter == ".":
            self.sub_lexer.go_after_dec_point()
        # e
        elif self.current_letter == "e" or self.current_letter == "E":
            self.sub_lexer.go_after_e()
        # other numbers
        elif self.current_letter.isnumeric():
            self.sub_lexer.take_number(int(self.current_letter))
        # unexpected letter
        else:
            raise LexerException(self.current_letter)
