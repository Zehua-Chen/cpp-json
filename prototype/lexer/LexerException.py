class LexerException(BaseException):

    def __init__(self, letter: str):
        self.letter = letter

    def __str__(self):
        return "unexpected letter: {}".format(self.letter)