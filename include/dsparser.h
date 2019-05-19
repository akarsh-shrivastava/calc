#ifndef DSPARSER_H_INCLUDED
    #define DSPARSER_H_INCLUDED
    #include <string>
    #include <vector>
    #include "structs.h"

    extern std::map<std::string, int> data_symbol_table;
    extern std::string text_section;
    enum DsState
    {
        DS_BEGIN,
        DS_GOT_IDEN,
        DS_ARG,
        DS_CORRECT_LINE,
        DS_ARG_CORRECT,
        DS_ERROR
    };

    struct Dsparser
    {
        std::vector<Token> data;
        std::vector<Token> code;
        std::string ds_code;
        bool proceed;
        std::string error_msg;
        int curr_arg_no;


        Dsparser(std::vector<Token> data, std::vector<Token> code);
        std::string get_arg_register(std::string data);

        std::string get_ds_asm();
    };

#endif