import enum
from lexer import Token
from .ComposerException import ComposerException
from .Scope import Scope

class Composer:
    
    class State(enum.Enum):
        START = 0
        STRING = 1
        OTHER_PRIMITIVE = 2
        HAS_KEY = 3
        HAS_VALUE = 4
        ERROR = 5

    def __init__(self):
        self._stack = []
        self._key = None
        self._value = None
        self._state = Composer.State.START
        
    def __str__(self):
        return "state = {}, key = {}, value = {}".format(
            self._state,
            self._key,
            self._value
        )

    def root(self):
        return self._stack[len(self._stack) - 1].value

    def take_token(self, token: Token):
        
        if token.type == Token.Type.STRING:
            self._string_token(token)
        elif token.type == Token.Type.NUMBER:
            self._other_primitive_token(token)
        elif token.type == Token.Type.BOOLEAN:
            self._other_primitive_token(token)
        elif token.type == Token.Type.NULL:
            self._other_primitive_token(token)
        # ,
        elif token.type == Token.Type.VALUE_SEPARATOR:
            self._value_separator_token(token)
        # :
        elif token.type == Token.Type.KEY_VALUE_SEAPARATOR:
            self._key_value_separator_token(token)
        elif token.type == Token.Type.BEGIN_ARRAY:
            self._begin_array_token(token)
        elif token.type == Token.Type.BEGIN_OBJECT:
            self._begin_object_token(token)
        elif token.type == Token.Type.END_ARRAY:
            self._end_array_token(token)
        elif token.type == Token.Type.END_OBJECT:
            self._end_object_token(token)
        else:
            print("Token unrecognized = {}".format(token.type))
            
    def _string_token(self, token: Token):
        if self._state == Composer.State.START:
            self._value = token.data
            self._state = Composer.State.STRING
        elif self._state == Composer.State.HAS_KEY:
            self._value = token.data
            self._state = Composer.State.HAS_VALUE
        else:
            self._state = Composer.State.ERROR
            raise ComposerException("string should not be here")
    
    def _other_primitive_token(self, token: Token):
        if self._state == Composer.State.START:
            self._value = token.data
            self._state = Composer.State.OTHER_PRIMITIVE
        elif self._state == Composer.State.HAS_KEY:
            self._value = token.data
            self._state = Composer.State.HAS_VALUE
        
    def _key_value_separator_token(self, token: Token):
        if self._state == Composer.State.STRING:
            self._key = self._value
            self._value = None
            self._state = Composer.State.HAS_KEY
        else:
            self._state = Composer.State.ERROR
            raise ComposerException(": should not be here")
        
    def _value_separator_token(self, token: Token):
        self._insert_value_into_top()
        self._state = Composer.State.START
        self._key = None
        self._value = None
        
    def _begin_object_token(self, token: Token):
        if self._state == Composer.State.START:
            self._stack.append(Scope.new_object())
        elif self._state == Composer.State.HAS_KEY:
            self._stack.append(Scope.new_object(self._key))
            self._state = Composer.State.START
        
    def _begin_array_token(self, token: Token):
        if self._state == Composer.State.START:
            self._stack.append(Scope.new_array())
        elif self._state == Composer.State.HAS_KEY:
            self._stack.append(Scope.new_array(self._key))
            self._state = Composer.State.START
        
    def _end_object_token(self, token: Token):
        if self._state == Composer.State.HAS_VALUE:
            self._insert_value_into_top()
            self._state = Composer.State.START
        
    def _end_array_token(self, token: Token):
        pass
        
    def _insert_value_into_top(self):
        peek = self._stack[len(self._stack) - 1]
        
        if peek.type == Scope.Type.OBJECT:
            peek.value[self._key] = self._value
        elif peek.type == Scope.Type.ARRAY:
            peek.value.append(self._value)
        else:
            self._state = Composer.State.ERROR
            raise ComposerException("cannot insert token")