#ifndef CSPARSER_H_INCLUDED
    #define CSPARSER_H_INCLUDED
    #define VNULL (std::vector<std::string>::iterator)NULL
    #include <string>
    #include <vector>
    #include <algorithm>
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

    struct registers
    {
        inline static std::map<std::string, bool> avail= {{std::string("%rdi"), true},
                                                          {std::string("%rsi"), true},
                                                          {std::string("%rdx"), true},
                                                          {std::string("%rcx"), true},
                                                          {std::string("%r8") , true},
                                                          {std::string("%r9") , true},
                                                          {std::string("%r10"), true},
                                                          {std::string("%r11"), true},
                                                          {std::string("%r12"), true},
                                                          {std::string("%r13"), true} };
        inline static std::map<int, std::string> regs = {{1, std::string("%rdi")},
                                                         {2, std::string("%rsi")},
                                                         {3, std::string("%rdx")},
                                                         {4, std::string("%rcx")},
                                                         {5, std::string("%r8") },
                                                         {6, std::string("%r9") },
                                                         {7, std::string("%r10")},
                                                         {8, std::string("%r11")},
                                                         {9, std::string("%r12")},
                                                         {10,std::string("%r13")} };
        inline static std::string curr="";
        static bool alloc(){
            for (int i=1; i<=avail.size(); i++)
            {
                if(avail[regs[i]])
                {
                    curr = regs[i];
                    avail[regs[i]] = false;
                    return true;
                }
            }
            return false;
        }
        static bool free(std::string reg){
            if(avail[reg]){
                return false;
            }
            else{
                avail[reg] = true;
                return true;
            }
        }
        static std::string get(){
            return curr;
        }
    };
    struct Csparser
    {
        std::vector<Token> code;
        std::string cs_code;
        bool valid;
        bool proceed;
        std::string error_msg;
        std::vector<Token> expression;
        std::vector<Token> postfix;

        Csparser(std::vector<Token> code);
        void validate();
        int get_precedence(std::string op);
        void gen_postfix();
        void gen_code();
        Token get_opcode(Token op,Token operand1, Token operand2);
        void evaluate_exp();
        void tac_gen();
        std::string get_cs_asm();
    };
#endif