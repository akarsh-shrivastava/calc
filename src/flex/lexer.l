%{
    #include <vector>
    #include <string>
    #include <fstream>
    #include <iostream>
    #include "structs.h"
    extern std::vector<Token> tokens;
    int line_no=1;
%}

NUM [1-9][0-9]*|[0]

ABO [+]|[-]|[/]|[*]|[%]|[&]|[|]|[\^]|[<][<]|[>][>]
CPO [>]|[<]|[>][=]|[<][=]|[=][=]|[!][=]
ASO [=]|[+][=]|[-][=]|[/][=]|[*][=]|[%][=]|[&][=]|[\|][=]|[\^][=]|[<][<][=]|[>][>][=]
LGO &&|\|\|
UNO [+][+]|[-][-]|[~]|[!]|[u][-]

WS  [\ ]|[\t]

%option noyywrap

%%
\n                             {tokens.push_back(Token(DELIMITOR, YYText(), line_no)); line_no++;}

{NUM}                          {tokens.push_back(Token(NUMCONST, YYText(), line_no));}
\"([^"])*\"                    {tokens.push_back(Token(STRCONST, YYText(), line_no));}
'.'                            {tokens.push_back(Token(CHRCONST, YYText(), line_no));}

{CPO}|{UNO}|{ASO}|{LGO}|{ABO}  {tokens.push_back(Token(OPERATOR, YYText(), line_no));}

[(]                            {tokens.push_back(Token(PAREN_OPEN , YYText(), line_no));}
[)]                            {tokens.push_back(Token(PAREN_CLOSE, YYText(), line_no));}

arg                            {tokens.push_back(Token(ARG, YYText(), line_no));}

data                           {tokens.push_back(Token(DATA, YYText(), line_no));}
code                           {tokens.push_back(Token(CODE, YYText(), line_no));}
end                            {tokens.push_back(Token(END , YYText(), line_no));}

return                         {tokens.push_back(Token(RETURN, YYText(), line_no));}
extern                         {tokens.push_back(Token(EXTERN, YYText(), line_no));}

[a-zA-z_][a-zA-Z0-9_]*         {tokens.push_back(Token(POSSIBLE_IDENTIFIER, YYText(), line_no));}

({WS})+                        {}
.                              {tokens.push_back(Token(UNKNOWN, YYText(), line_no));}
%%

void lexer(std::string filename)
{
    std::ifstream fin(filename.c_str());
    yyFlexLexer lexer1(fin, std::cout);

    lexer1.yylex();
}
