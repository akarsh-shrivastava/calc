#ifndef TOKENIZER_H_INCLUDED
    #define TOKENIZER_H_INCLUDED
    #include <string>
    #include <vector>

    struct Tokenizer
    {
        std::string tok;
        std::vector<std::string> tokens;

        int is_change(char x, char y);
        void end_word();
        std::vector<std::string> tokenize(std::string code);
    };
#endif