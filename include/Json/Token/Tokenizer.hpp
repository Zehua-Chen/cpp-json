//
//  Tokenizer.hpp
//  CPP
//
//  Created by Zehua Chen on 4/15/19.
//  Copyright © 2019 Zehua Chen. All rights reserved.
//

#pragma once 

namespace json::token
{
template<typename CharT>
class Tokenizer
{
public:
    Tokenizer();
    
    /** 
     * Take a iterator to extract letter to process
     * @param iter the iterator, if the tokenizer does not need another pass
     * on the letter, the iterator will be incremented. Otherwise, the 
     * tokenizer will be untouched
     */
    template<typename IterT>
    void take(IterT &iter);
private:

};
} // json::token

// Implementations

namespace json::token
{
template<typename CharT>
template<typename IterT>
void Tokenizer<CharT>::take(IterT &iter)
{
    
}
} // json::token
