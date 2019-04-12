#ifndef STRUCTS_H_INCLUDED
    #define STRUCTS_H_INCLUDED
    #include <string>
    #include <vector>
    #include <string>
    #include <map>

    struct Flag_struct
    {
        unsigned char str_open : 1;
        unsigned char char_open : 1;

        Flag_struct();
    };

    struct Token_struct
    {
        std::string tok;
        std::string type;

        void print();
    };

    struct Keywords
    {
        inline static std::string delimitor = "\n";
        inline static std::string seperator = ",";
        inline static std::string colon     = ":";

        inline static std::string cs_addr_open   = "{";
        inline static std::string cs_addr_close  = "}";
        inline static std::string es_addr_open   = "(";
        inline static std::string es_addr_close  = ")";
        inline static std::string ds_addr_open   = "[";
        inline static std::string ds_addr_close  = "]";

        inline static std::vector<std::string> instructions_2addr  =  {
                                                                          "mov",
                                                                          "add","sub","mul","div","mod",
                                                                          "and","or","xor",
                                                                          "cmp","def"
                                                                      };

        inline static std::vector<std::string> instructions_1addr  =  {
                                                                          "not","neg",
                                                                          "read","write",
                                                                          "push","pop"
                                                                      };

        inline static std::vector<std::string> jump                =  {
                                                                          "jmp"
                                                                      };

        inline static std::vector<std::string> jump_conditions     =  {
                                                                            "eq"  ,  "neq",
                                                                           "grt"  ,  "grteq",
                                                                          "less"  ,  "lesseq",
                                                                          "always"
                                                                      };

        inline static std::vector<std::string> segments      =  {
                                                                    ".code",".data"
                                                                };

        static std::map<std::string, std::string> labels;
    };
#endif