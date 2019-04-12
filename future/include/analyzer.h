#ifndef ANALYZER_H_INCLUDED
    #define ANALYZER_H_INCLUDED
    #include <vector>
    #include <string>
    #include <iterator>

    #include "structs.h"

    struct Analyzer
    {
        std::vector<Token_struct> token_data;

        void build_token_data(std::vector<std::string> tokens);
        bool is_possible_identifier(std::string tok);
        bool is_string(std::string tok);
        bool is_const(std::string tok);
        std::string get_type(std::vector<std::string>::iterator itr);
        
        void print_token_data();
    };
#endif
