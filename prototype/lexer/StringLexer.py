class StringLexer:
    
    def __init__(self, finish_letter: str):
        self.finish_letter = finish_letter
        self.is_completed = False
        self.buffer = ""
        
    def take_letter(self, letter: str):
        
        if letter == self.finish_letter:
            self.is_completed = True
        else:
            self.buffer += letter
            
    def to_string(self):
        return self.buffer