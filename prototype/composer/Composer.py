from enum import Enum
from lexer import Token
from .ComposerError import ComposerError
from .Scope import Scope


class Composer:

    class State(Enum):
        START = 0
        ERROR = 1
        FINIEHD = 2
        OBJ_START = 3
        OBJ_HAS_KEY_STRING = 4
        OBJ_HAS_KEY = 5
        OBJ_HAS_VALUE = 6
        OBJ_READY = 7
        ARRAY_START = 8
        ARRAY_HAS_VALUE = 9
        ARRAY_READY = 10

    def __init__(self):
        self._stack = []
        self._key = None
        self._value = None
        self._state = Composer.State.START

    def __str__(self):
        return "state = {}".format(self._state)

    def root(self):
        if len(self._stack) == 0:
            return None

        return self._stack[len(self._stack) - 1].value

    def take_token(self, token: Token):
        
        # print("token = {}".format(token))
        
        if self._state == Composer.State.START:
            self._start_state(token)
        elif self._state == Composer.State.OBJ_START:
            self._obj_start_state(token)
        elif self._state == Composer.State.OBJ_HAS_KEY_STRING:
            self._obj_has_key_string_state(token)
        elif self._state == Composer.State.OBJ_HAS_KEY:
            self._obj_has_key_state(token)
        elif self._state == Composer.State.OBJ_HAS_VALUE:
            self._obj_has_value_state(token)
        elif self._state == Composer.State.OBJ_READY:
            self._obj_ready(token)
        elif self._state == Composer.State.ARRAY_START:
            self._array_start_state(token)
        elif self._state == Composer.State.ARRAY_HAS_VALUE:
            self._array_has_value_state(token)
        elif self._state == Composer.State.ARRAY_READY:
            self._array_ready(token)
        elif self._state == Composer.State.ERROR:
            self._error_state(token)
        elif self._state == Composer.State.FINIEHD:
            raise ComposerError("Json object is finished")

    def _start_state(self, token: Token):
        # Finish with string
        if token.type == Token.Type.STRING:
            self._push(Scope.make_string(token.data))
            self._state == Composer.State.FINIEHD
        # Finish with number
        elif token.type == Token.Type.NUMBER:
            self._push(Scope.make_number(token.data))
            self._state == Composer.State.FINIEHD
        # Finish with boolean
        elif token.type == Token.Type.BOOLEAN:
            self._push(Scope.make_boolean(token.data))
            self._state == Composer.State.FINIEHD
        # Finish with null
        elif token.type == Token.Type.NULL:
            self._push(Scope.make_null())
            self._state == Composer.State.FINIEHD
        # Start with a root object
        elif token.type == Token.Type.BEGIN_OBJECT:
            self._push(Scope.make_object())
            self._state = Composer.State.OBJ_START
        # Start with a root array
        elif token.type == Token.Type.BEGIN_ARRAY:
            self._push(Scope.make_array())
            self._state = Composer.State.ARRAY_START

    def _obj_start_state(self, token: Token):
        
        # end object
        if token.type == Token.Type.END_OBJECT:
            # If void context, finish root object
            if self._is_context_void():
                self._state = Composer.State.FINIEHD
            # if object context, finish current object
            elif self._is_context_object():
                self._state = Composer.State.OBJ_HAS_VALUE
            # if array context, finish current object
            elif self._is_context_array():
                self._state = Composer.State.ARRAY_HAS_VALUE
            # "end object" cannot be here
            else:
                raise ComposerError("wrong place for token: {}".format(token))
        # string
        elif token.type == Token.Type.STRING:
            self._key = token.data
            self._state = Composer.State.OBJ_HAS_KEY_STRING
        # Token type not allowed
        else:
            raise ComposerError("unexpected token {}".format(token))

    def _obj_has_key_string_state(self, token: Token):
        # :, finish key
        if token.type == Token.Type.KEY_VALUE_SEAPARATOR:
            self._state = Composer.State.OBJ_HAS_KEY
        # token type not allowed
        else:
            raise ComposerError("unexpected token {}".format(token))

    def _obj_has_key_state(self, token: Token):
        # start new object
        if token.type == Token.Type.BEGIN_OBJECT:
            self._stack.append(Scope.make_object(self._key))
            self._state = Composer.State.OBJ_START
        # start new array
        elif token.type == Token.Type.BEGIN_ARRAY:
            self._stack.append(Scope.make_array(self._key))
            self._state = Composer.State.ARRAY_START
        # start new string
        elif token.type == Token.Type.STRING:
            self._stack.append(Scope.make_string(token.data, self._key))
            self._state = Composer.State.OBJ_HAS_VALUE
        # start new number
        elif token.type == Token.Type.NUMBER:
            self._stack.append(Scope.make_number(token.data, self._key))
            self._state = Composer.State.OBJ_HAS_VALUE
        # start new boolean
        elif token.type == Token.Type.BOOLEAN:
            self._stack.append(Scope.make_boolean(token.data, self._key))
            self._state = Composer.State.OBJ_HAS_VALUE
        # start new null
        elif token.type == Token.Type.NULL:
            self._stack.append(Scope.make_null(self._key))
            self._state = Composer.State.OBJ_HAS_VALUE

    def _obj_has_value_state(self, token: Token):
        
        current_scope = self._stack.pop()
        parent_scope = self._peek()
        parent_scope.value[current_scope.name] = current_scope.value
        
        # end object
        if token.type == Token.Type.END_OBJECT:
            # if context is void, finish
            if self._is_context_void():
                self._state = Composer.State.FINIEHD
            # if context is array, move to array has value
            elif self._is_context_array():
                self._state = Composer.State.ARRAY_HAS_VALUE
            # if context is object, do nothing
            elif self._is_context_object():
                pass
        # value separator, start a new round of key-value pair recording
        elif token.type == Token.Type.VALUE_SEPARATOR:
            self._state = Composer.State.OBJ_READY
            
    def _obj_ready(self, token: Token):
        if token.type == Token.Type.STRING:
            self._key = token.data
            self._state = Composer.State.OBJ_HAS_KEY_STRING
        else:
            raise ComposerError("cannot have {} here".format(token))

    def _array_start_state(self, token: Token):
        # insert new object into the array
        if token.type == Token.Type.BEGIN_OBJECT:
            self._push(Scope.make_object())
            self._state = Composer.State.OBJ_START
        # insert new array into the array
        elif token.type == Token.Type.BEGIN_ARRAY:
            self._push(Scope.make_array())
            self._state = Composer.State.ARRAY_START
        # end array
        elif token.type == Token.Type.END_ARRAY:
            if self._is_context_object():
                self._state = Composer.State.OBJ_HAS_VALUE
            elif self._is_context_array():
                self._state = Composer.State.ARRAY_HAS_VALUE
            elif self._is_context_void():
                self._state = Composer.State.FINIEHD
        elif token.type == Token.Type.STRING:
            self._push(Scope.make_string(token.data))
            self._state = Composer.State.ARRAY_HAS_VALUE
        elif token.type == Token.Type.NUMBER:
            self._push(Scope.make_string(token.data))
            self._state = Composer.State.ARRAY_HAS_VALUE
        elif token.type == Token.Type.BOOLEAN:
            self._push(Scope.make_string(token.data))
            self._state = Composer.State.ARRAY_HAS_VALUE
        elif token.type == Token.Type.NULL:
            self._push(Scope.make_null())
            self._state = Composer.State.ARRAY_HAS_VALUE

    def _array_has_value_state(self, token: Token):
        current_scope = self._pop()
        parent_scope = self._peek()
        parent_scope.value.append(current_scope.value)
        
        if token.type == Token.Type.END_ARRAY:
            if self._is_context_array():
                pass
            elif self._is_context_object():
                self._state = Composer.State.OBJ_HAS_VALUE
            elif self._is_context_void():
                self._state = Composer.State.FINIEHD
        elif token.type == Token.Type.VALUE_SEPARATOR:
            self._state = Composer.State.ARRAY_READY
            
    def _array_ready(self, token: Token):
        # insert new object into the array
        if token.type == Token.Type.BEGIN_OBJECT:
            self._push(Scope.make_object())
            self._state = Composer.State.OBJ_START
        # insert new array into the array
        elif token.type == Token.Type.BEGIN_ARRAY:
            self._push(Scope.make_array())
            self._state = Composer.State.ARRAY_START
        elif token.type == Token.Type.STRING:
            self._push(Scope.make_string(token.data))
            self._state = Composer.State.ARRAY_HAS_VALUE
        elif token.type == Token.Type.NUMBER:
            self._push(Scope.make_string(token.data))
            self._state = Composer.State.ARRAY_HAS_VALUE
        elif token.type == Token.Type.BOOLEAN:
            self._push(Scope.make_string(token.data))
            self._state = Composer.State.ARRAY_HAS_VALUE
        elif token.type == Token.Type.NULL:
            self._push(Scope.make_null())
            self._state = Composer.State.ARRAY_HAS_VALUE

    def _error_state(self, token: Token):
        pass
        
    def _push(self, scope: Scope):
        self._stack.append(scope)
        
    def _pop(self) -> Scope:
        return self._stack.pop()
        
    def _peek(self) -> Scope:
        return self._stack[len(self._stack) - 1]

    def _is_context_void(self) -> bool:
        return len(self._stack) == 1

    def _is_context_object(self) -> bool:
        parent_scope = self._stack[len(self._stack) - 2]
        return parent_scope.value_type == Scope.Type.OBJECT

    def _is_context_array(self) -> bool:
        parent_scope = self._stack[len(self._stack) - 2]
        return parent_scope.value_type == Scope.Type.ARRAY
