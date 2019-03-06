class PrimitiveLexer:
    
    def __init__(self, index: int, primtive: str):
        self.index = index
        self.primitive = primtive
        self.completed = False
        
    def take_letter(self, letter: str):
        
        # if not at last letter
        if self.index < len(self.primitive) - 1:
            if self.primitive[self.index] == letter:
                self.index += 1
            else:
                raise LexerException(letter)
        # if at last letter
        else:
            if self.primitive[self.index] == letter:
                self.completed = True
            else:
                raise LexerException(letter)