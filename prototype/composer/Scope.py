import enum


class Scope:
    
    class Type(enum.Enum):
        OBJECT = 0
        ARRAY = 1
        STRING = 2
        BOOLEAN = 3
        NUMBER = 4
        NULL = 5
        
    @staticmethod
    def make_object(name: str = None):
        return Scope(name, {}, Scope.Type.OBJECT)
        
    @staticmethod
    def make_array(name: str = None):
        return Scope(name, [], Scope.Type.ARRAY)
        
    @staticmethod
    def make_string(value = None, name: str = None):
        return Scope(name, value, Scope.Type.STRING)
        
    @staticmethod
    def make_number(value = None, name: str = None):
        return Scope(name, value, Scope.Type.NUMBER)
        
    @staticmethod
    def make_boolean(value = None, name: str = None):
        return Scope(name, value, Scope.Type.BOOLEAN)
        
    @staticmethod
    def make_null(name: str = None):
        return Scope(name, value, Scope.Type.NULL)

    def __init__(self, name: str, value, value_type):
        self.name = name
        self.value = value
        self.value_type = value_type