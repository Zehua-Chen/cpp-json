#include "Json/Token.hpp"
#include "Json/BasicValue.hpp"

namespace json::assemble
{
template<typename CharT>
class Assembler
{
public:
    void operator()(const json::token::Token<CharT> &token);
    BasicValue<CharT> &&root();
};
}