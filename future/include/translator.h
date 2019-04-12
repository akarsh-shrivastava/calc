#ifndef TRANSLATOR_H_INCLUDED
    #define TRANSLATOR_H_INCLUDED
    #include <map>
    #include <vector>
    #include "analyzer.h"
    #include "structs.h"

    struct Translator
    {
        std::map<std::string, std::string> str2dec;
        
        std::vector<Token_struct> token_data;
        std::string current_line;
        std::string decimal_code;

        int line_count;
        int inst_count;
        std::string error_msg;

        Translator(std::vector<Token_struct> data);
        void build_decimal_instr();
        void syntax_error(std::vector<Token_struct>::iterator itr, std::string err_msg);
        bool is_lable(std::string identifier);

    };

#endif