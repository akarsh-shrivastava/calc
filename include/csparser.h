#ifndef CSPARSER_H_INCLUDED
    #define CSPARSER_H_INCLUDED
    #include <string>
    #include <vector>
    #include "structs.h"

    extern std::map<std::string, int> data_symbol_table;
    extern std::map<std::string, int> func_symbol_table;
    
    enum CsState
    {
        CS_BEGIN,
        CS_EXTERN_DECL,
        CS_EXTERN_NAME_RECEIVED,
        CS_EXTERN_DECL_CORRECT,
        CS_CORRECT_LINE,
        CS_RETURN,
        CS_RETURN_NUM,
        CS_RETURN_VAR,
        CS_RETURN_CORRECT,
        CS_PAREN,
        CS_ERROR
    };
    struct Csparser
    {
        std::vector<Token> code;
        std::string cs_code;
        bool valid;
        bool proceed;
        std::string error_msg;

        Csparser(std::vector<Token> code);
        void validate();
        std::string get_cs_asm();
    };
#endif