from lexer import Lexer, Token, LexerException
from composer import Composer


json = """
{ 
  'a': true, 
  'aa': false,
  'aaa': null,
  'b': ['a', 'a'], 
  'c': '12', 
  'd': 22.2 
}
"""
    
lexer = Lexer(json)
composer = Composer()

print("Input Json")
print(json)

for letter in json:
    lexer.take_letter(letter)
    
    if lexer.has_token:
        composer.take_token(lexer.token)
        print(composer)

root = composer.root()

print("Output Json Object")
print(root)