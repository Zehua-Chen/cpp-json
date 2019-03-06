from Token import Token
from Lexer import Lexer, LexerException, State


json = "{ 'a': true, 'b': false, 'c': null, 'd': -12.22e2 }"
lexer = Lexer(json)

while not lexer.is_finished:
    lexer.extract_token()
    print(lexer.token)