import enum


class StringLexerNotImplementedException(Exception):
    
    def __str__(self):
        return "Escape sequence not implemented"


class StringLexer:

    class State(enum.Enum):
        STRING = 0
        ESCAPE = 1
        HEX_DIGIS = 2

    def __init__(self, finish_letter: str):
        self.finish_letter = finish_letter
        self.is_completed = False
        self.buffer = ""
        self.state = StringLexer.State.STRING

    def take_letter(self, letter: str):

        if self.state == StringLexer.State.STRING:
            if letter == self.finish_letter:
                self.is_completed = True
            elif letter == "\\":
                self.state = StringLexer.State.ESCAPE
            else:
                self.buffer += letter
        elif self.state == StringLexer.State.ESCAPE:
            if letter == "\"":
                self.buffer += "\""
                self.state = StringLexer.State.STRING
            elif letter == "\\":
                self.buffer += "\\"
                self.state = StringLexer.State.STRING
            elif letter == "n":
                self.buffer += "\n"
                self.state = StringLexer.State.STRING
            elif letter == "u":
                self.state = StringLexer.State.HEX_DIGIS
            else:
                raise StringLexerNotImplementedException()
        elif self.state == StringLexer.State.HEX_DIGIS:
            pass

    def to_string(self) -> str:
        return self.buffer
