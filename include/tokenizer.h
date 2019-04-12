#ifndef TOKENIZER_H_INCLUDED
    #define TOKENIZER_H_INCLUDED
    #include <string>
    #include <vector>
    #include "structs.h"

    struct Tokenizer
    {
        std::string tok;
        std::vector<Token> tokens;
        int line_no;

        void end_word(bool isop);
        std::vector<Token> get_tokens(std::string code);
        TokenType get_token_type(std::string tok);
    };
#endif