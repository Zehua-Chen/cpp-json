import enum
from lexer import Token
from .ComposerError import ComposerError
from .ComposerState import ComposerState
from .Scope import Scope

class Composer:
    
    def __init__(self):
        self._stack = []
        self._key = None
        self._value = None
        self._state = ComposerState.START
        
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
        if self._state == ComposerState.START:
            self._start_state(token)
        elif self._state == ComposerState.OBJ_START:
            self._obj_start_state(token)
        elif self._state == ComposerState.OBJ_HAS_KEY_STRING:
            self._obj_has_key_string_state(token)
        elif self._state == ComposerState.OBJ_HAS_KEY:
            self._obj_has_key_state(token)
        elif self._state == ComposerState.OBJ_HAS_VALUE:
            self._obj_has_value_state(token)
        elif self._state == ComposerState.ARRAY_START:
            self._array_start_state(token)
        elif self._state == ComposerState.ARRAY_HAS_VALUE:
            self._array_has_value_state(token)
        elif self._state == ComposerState.ERROR:
            self._error_state(token)
            
    def _start_state(self, token: Token):
        pass
        
    def _obj_start_state(self, token: Token):
        pass
        
    def _obj_has_key_string_state(self, token: Token):
        pass
        
    def _obj_has_key_state(self, token: Token):
        pass
        
    def _obj_has_value_state(self, token: Token):
        pass
        
    def _array_start_state(self, token: Token):
        pass
        
    def _array_has_value_state(self, token: Token):
        pass
        
    def _error_state(self, token: Token):
        pass
        
    def _set_state(self, state, data=None):
        if state == Composer.State.OBJ_START:
            self._stack.append(Scope(name=self._key, value={}))
        elif state == Composer.State.OBJ_HAS_KEY_STRING:
            self._key = data
        
        self._state = state