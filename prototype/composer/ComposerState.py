from enum import Enum

class ComposerState(Enum):
    START = 0
    ERROR = 1
    OBJ_START = 2
    OBJ_HAS_KEY_STRING = 3
    OBJ_HAS_KEY = 4
    OBJ_HAS_VALUE = 5
    ARRAY_START = 6
    ARRAY_HAS_VALUE = 7