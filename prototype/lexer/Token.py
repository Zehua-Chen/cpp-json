import enum


class Token:
    
    class Type(enum.Enum):
        BEGIN_OBJECT = 0
        END_OBJECT = 1
        BEGIN_ARRAY = 2
        END_ARRAY = 3
        STRING = 4
        BOOLEAN = 5
        NUMBER = 6
        NULL = 7
        KEY_VALUE_SEAPARATOR = 8
        VALUE_SEPARATOR = 9
        
    @staticmethod
    def being_object():
        return Token(Token.Type.BEGIN_OBJECT)
        
    @staticmethod
    def end_object():
        return Token(Token.Type.END_OBJECT)
        
    @staticmethod
    def begin_array():
        return Token(Token.Type.BEGIN_ARRAY)
        
    @staticmethod
    def end_array():
        return Token(Token.Type.END_ARRAY)
        
    @staticmethod
    def string(value: str):
        return Token(Token.Type.STRING, data=value)
        
    @staticmethod
    def boolean(value: bool):
        return Token(Token.Type.STRING, data=value)
        
    @staticmethod
    def number(value: float):
        return Token(Token.Type.STRING, data=value)
        
    @staticmethod
    def null():
        return Token(Token.Type.NULL)
        
    @staticmethod
    def key_value_separator():
        return Token(Token.Type.KEY_VALUE_SEAPARATOR)
        
    @staticmethod
    def value_separator():
        return Token(Token.Type.VALUE_SEPARATOR)

    def __init__(self, token_type, data=None):
        self.token_type = token_type
        self.data = data

    def __str__(self):
        
        if self.data == None:
            return str(self.token_type.name)
        else:
            return "{}: {}".format(self.token_type.name, self.data)
