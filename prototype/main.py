from lexer import Lexer, Token, LexerException


json = "{ 'a': true, 'b': false, 'c': null, 'd': -12.22e-2 }"
lexer = Lexer(json)

while not lexer.is_finished:
    lexer.extract_token()
    print(lexer.token)