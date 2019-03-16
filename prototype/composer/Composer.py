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
        ARRAY_START = 7
        ARRAY_HAS_VALUE = 8

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
        elif self._state == Composer.State.ARRAY_START:
            self._array_start_state(token)
        elif self._state == Composer.State.ARRAY_HAS_VALUE:
            self._array_has_value_state(token)
        elif self._state == Composer.State.ERROR:
            self._error_state(token)
        elif self._state == Composer.State.FINIEHD:
            raise ComposerError("Json object is finished")

    def _start_state(self, token: Token):
        if token.type == Token.Type.STRING:
            self._stack.append(Scope.make_string(token.data))
            self._state == Composer.State.FINIEHD
        elif token.type == Token.Type.NUMBER:
            self._stack.append(Scope.make_number(token.data))
            self._state == Composer.State.FINIEHD
        elif token.type == Token.Type.BOOLEAN:
            self._stack.append(Scope.make_boolean(token.data))
            self._state == Composer.State.FINIEHD
        elif token.type == Token.Type.NULL:
            self._stack.append(Scope.make_null())
            self._state == Composer.State.FINIEHD
        elif token.type == Token.Type.BEGIN_OBJECT:
            self._stack.append(Scope.make_object())
            self._state = Composer.State.OBJ_START
        elif token.type == Token.Type.BEGIN_ARRAY:
            self._stack.append(Scope.make_array())
            self._state = Composer.State.ARRAY_START

    def _obj_start_state(self, token: Token):
        
        # }
        if token.type == Token.Type.END_OBJECT:
            # If root is empty object
            if self._is_current_root():
                self._state = Composer.State.FINIEHD
            elif self._is_parent_object():
                self._state = Composer.State.OBJ_HAS_VALUE
            elif self._is_parent_array():
                self._state = Composer.State.ARRAY_HAS_VALUE
            else:
                raise ComposerError("wrong place for token: {}".format(token))
        elif token.type == Token.Type.STRING:
            self._key = token.data
            self._state = Composer.State.OBJ_HAS_KEY_STRING
        else:
            raise ComposerError("unexpected token {}".format(token))

    def _obj_has_key_string_state(self, token: Token):
        if token.type == Token.Type.KEY_VALUE_SEAPARATOR:
            self._state = Composer.State.OBJ_HAS_KEY
        else:
            raise ComposerError("unexpected token {}".format(token))

    def _obj_has_key_state(self, token: Token):
        if token.type == Token.Type.BEGIN_OBJECT:
            self._stack.append(Scope.make_object(self._key))
            self._state = Composer.State.OBJ_START
        elif token.type == Token.Type.BEGIN_ARRAY:
            self._stack.append(Scope.make_array(self._key))
            self._state = Composer.State.ARRAY_START
        elif token.type == Token.Type.STRING:
            self._stack.append(Scope.make_string(token.data, self._key))
            self._state = Composer.State.OBJ_HAS_VALUE
        elif token.type == Token.Type.NUMBER:
            self._stack.append(Scope.make_number(token.data, self._key))
            self._state = Composer.State.OBJ_HAS_VALUE
        elif token.type == Token.Type.BOOLEAN:
            self._stack.append(Scope.make_boolean(token.data, self._key))
            self._state = Composer.State.OBJ_HAS_VALUE
        elif token.type == Token.Type.NULL:
            self._stack.append(Scope.make_null(self._key))
            self._state = Composer.State.OBJ_HAS_VALUE

    def _obj_has_value_state(self, token: Token):
        if token.type == Token.Type.END_OBJECT:
            if self._is_current_root():
                self._state = Composer.State.FINIEHD
            elif self._is_parent_array():
                self._state = Composer.State.ARRAY_HAS_VALUE
            elif self._is_parent_object():
                current_scope = self._stack.pop()
                parent_scope = self._peek()
                parent_scope.value[current_scope.name] = current_scope.value
                self._state = Composer.State.OBJ_START
        elif token.type == Token.Type.VALUE_SEPARATOR:
            current_scope = self._stack.pop()
            parent_scope = self._peek()
            parent_scope.value[current_scope.name] = current_scope.value
            self._state = Composer.State.OBJ_START
            
            print(parent_scope.value)

    def _array_start_state(self, token: Token):
        pass

    def _array_has_value_state(self, token: Token):
        pass

    def _error_state(self, token: Token):
        pass
        
    def _peek(self) -> Scope:
        return self._stack[len(self._stack) - 1]

    def _is_current_root(self) -> bool:
        return len(self._stack) == 1

    def _is_parent_object(self) -> bool:
        parent = self._stack[len(self._stack) - 2]
        return parent.value_type == Scope.Type.OBJECT

    def _is_parent_array(self) -> bool:
        parent = self._stack[len(self._stack) - 2]
        return parent.value_type == Scope.Type.ARRAY
