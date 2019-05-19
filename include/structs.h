#ifndef STRUCTS_H_INCLUDED
    #define STRUCTS_H_INCLUDED
    #include <iostream>
    #include <string>
    #include <vector>
    #include <map>

    enum TokenType
    {
        DELIMITOR,
        OPERATOR,
        
        NUMCONST,
        STRCONST,
        CHRCONST,
        
        PAREN_OPEN,
        PAREN_CLOSE,
        
        ARG,
        
        DATA,
        CODE,
        END,
        
        EXTERN,
        RETURN,

        POSSIBLE_IDENTIFIER,
        VARIABLE,
        FUNCTION,
        UNKNOWN,
    };

    struct Token
    {
        TokenType type;
        std::string lexeme;
        int line;

        Token(TokenType tok, std::string lex, int l) : type(tok), lexeme(lex), line(l) {}
        void print() {  
            std::cout<<type<<") ";
            std::cout<<((lexeme=="\n")?"\\n":lexeme);
            std::cout<<"    ";
            std::cout<<line<<"\n";
        }
    };

#endif
