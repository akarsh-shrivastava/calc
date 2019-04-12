#ifndef PREPROCCESSOR_H_INCLUDED
    #define PREPROCCESSOR_H_INCLUDED

    #include <string>

    struct Preprocessor
    {
        std::string code;

        Preprocessor(std::string c);

        void preprocess();
        void remove_comments();
        void replace_macros();
        void remove_backslash();
        void replace_consts();
        std::string get_preprocessed_code();
    };
#endif
