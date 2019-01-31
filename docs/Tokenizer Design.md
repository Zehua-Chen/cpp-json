# Tokenizer

* The tokenizer is supposed to iterate through a json string, and emit tokens.
* To avoid allocating heap memory, tokens are emitted through a callback function;

````json
{
  "name": "philosophor",
  "age": "12",
  "experiences": [ "a", "b" ]
}
````

Is supposed to give the following tokens

````cpp
{
  { TokenType::beginObject },
  { TokenType::key, "name" },
  { TokenType::value, "philosophor" },
  { TokenType::key, "age" },
  { TokenType::value, "12" },
  { TokenType::key, "experiences" },
  { TokenType::beginArray },
  { TokenType::value, "a" },
  { TokenType::value, "b" },
  { TokenType::endArray },
  { TokenType::endObject }
}
````

## Tokenizing

* The tokenizer parse json string by iterating through letters;

### Triggers

* The tokenizer send tokens using the after encountering triggers;
* There are two types of triggers:
  * Characters: `{`, `}`, `[`, `]`, `:`, `,`, `\n`
  * Strings: `//`, `/*`, `*/`
  
#### Handling Both Strings and Characters Being Triggers

* `_inspectCharacter` looks at a letter and see if a trigger has "occured";
* `_inspectExistingToken` looks at the current buffer 
and see if a trigger has "occured";
* After stepping on a letter, `_inspectCharacter` will be called;
  * `_inspectCharacter` returns true, when further processing is needed;
  * Right now, if buffer is appended, the function returns true;
* If `_inspectCharacter` returns true, `_inspectExistingToken` 
will be called to look at the existing buffer and invoke callbacks if needed;

### Contexts

* Many characters needs to be omitted or buffered depending on context. 
Like spaces and `\n`;
* There are four contexts:
  * `singleLineComment`;
  * `multiLineComment`;
  * `string`;
  * `undefined`;
* The tokenizer is set in `undefined` at beginning
* The tokenizer
  * Enters `singleLineComment` after the buffer 
has already contains `//`, and went back to `undefined` after encountering `\n`;
  * Enters `multiLineComment` after the buffer;
has already contains `/*`, and went back to `undefined` when the buffer ends
with `*/`;
  * Enters `string` after encountering `"` or `'` and went back to `undefined` 
after encountering another `"` or `'`
* In `multiLineComment` and `string`, all letters will be buffered
* In `singleLineComment`, all letters except `\n` will be buffered