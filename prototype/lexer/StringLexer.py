import enum


class StringLexerNotImplementedException(Exception):
    
    def __str__(self):
        return "Escape sequence not implemented"


class StringLexer:

    class State(enum.Enum):
        STRING = 0
        ESCAPE = 1
        HEX = 2

    def __init__(self, finish_letter: str):
        self.finish_letter = finish_letter
        self.is_completed = False
        self.buffer = ""
        self.state = StringLexer.State.STRING
        # Hex
        self.hex_value = 0
        self.hex_count = 0

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
                self.state = StringLexer.State.HEX
            else:
                raise StringLexerNotImplementedException()
        elif self.state == StringLexer.State.HEX:
            value = int(letter, base=16)
            self.hex_value *= 16
            self.hex_value += value
            self.hex_count += 1
            
            if self.hex_count == 4:
                self.buffer += "(number: {})".format(self.hex_value)
                self.state = StringLexer.State.STRING
                self.hex_count = 0
                self.hex_value = 0

    def to_string(self) -> str:
        return self.buffer
