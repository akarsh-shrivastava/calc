#ifndef DSPARSER_H_INCLUDED
    #define DSPARSER_H_INCLUDED
    #include <string>
    #include <vector>
    #include "structs.h"

    extern std::map<std::string, int> symbol_table;

    enum DsState
    {
        DS_BEGIN,
        DS_GOT_IDEN,
        DS_CORRECT_LINE,
        DS_ERROR
    };

    struct Dsparser
    {
        std::vector<Token> data;
        std::string code;
        bool proceed;
        std::string error_msg;

        Dsparser(std::vector<Token> data);
        std::string get_ds_asm();
    };

#endif