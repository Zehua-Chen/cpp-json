import enum


class Scope:
    
    class Type(enum.Enum):
        OBJECT = 0
        ARRAY = 1
        PRIMITIVE = 2
        
    @staticmethod
    def new_object(name: str = None):
        return Scope(name, {}, Scope.Type.OBJECT)
        
    @staticmethod
    def new_array(name: str = None):
        return Scope(name, [], Scope.Type.ARRAY)
        
    @staticmethod
    def new_primitive(name: str = None, value = None):
        return Scope(name, value, Scope.Type.PRIMITIVE)

    def __init__(self, name: str, value, type):
        self.name = name
        self.value = value
        self.type = type