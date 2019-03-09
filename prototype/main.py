from lexer import Lexer, Token, LexerException
from composer import Composer


json = "{ 'a': true, 'b': false, 'c': null, 'd': -12.22e-2, 'e': '\\u1111\\u1010' }"
    
lexer = Lexer(json)
composer = Composer()

print("Input Json")
print(json)

while not lexer.is_finished:
    lexer.extract_token()
    composer.take_token(lexer.token)
    
root = composer.root

print("Output Json Object")
print(root)