#include <cctype>
#include <string>
#include <vector>
#include "tokenizer.h"
#include "structs.h"


void Tokenizer::end_word(bool isop = false)
{
    if(tok.length()>0)
        tokens.push_back(Token(isop?OPERATOR:get_token_type(tok),tok,line_no));
    tok.clear();
}

std::vector<Token> Tokenizer::get_tokens(std::string code)
{
    bool char_open=false, str_open=false;
    char ch;
    line_no = 1;
    for (int i=0;i<code.length();i++)
    {
        ch=code[i];

        if (ch=='"')
        {
            if (!char_open)
            {
                if(!str_open)
                    end_word();
                str_open=(str_open)?false:true;
            }
        }

        if (ch=='\'')
        {
            if (!str_open)
            {
                if(!char_open)
                    end_word();
                char_open=(char_open)?false:true;
            }
        }

        if (str_open||char_open)
            tok.push_back(ch);
        else if (isspace(ch) && ch!='\n')
            end_word();
        else if (ch=='\n')
        {
            end_word();
            tok.push_back('\n');
            end_word();
            line_no++;
        }
        else if (ispunct(ch) && ch!='_')
        {
            if  (
                    code.substr(i,3) == std::string("<<=") ||
                    code.substr(i,3) == std::string(">>=")
                )
            {
                end_word();
                tok+=code.substr(i,3);
                end_word(true);
                i+=2;
            }
            else if (
                        code.substr(i,2) == std::string("&&")||code.substr(i,2) == std::string("||")||code.substr(i,2) == std::string("+=")||
                        code.substr(i,2) == std::string("-=")||code.substr(i,2) == std::string("*=")||code.substr(i,2) == std::string("/=")||
                        code.substr(i,2) == std::string("%=")||code.substr(i,2) == std::string("&=")||code.substr(i,2) == std::string("|=")||
                        code.substr(i,2) == std::string("^=")||code.substr(i,2) == std::string("<<")||code.substr(i,2) == std::string(">>")||
                        code.substr(i,2) == std::string("<=")||code.substr(i,2) == std::string(">=")||code.substr(i,2) == std::string("==")||
                        code.substr(i,2) == std::string("!=")||code.substr(i,2) == std::string("++")||code.substr(i,2) == std::string("--")
                    )
            {
                end_word();
                tok+=code.substr(i,2);
                end_word(true);
                i++;
            }
            else if(ch=='"' || ch=='\'')
            {
                tok.push_back(ch);
            }
            else 
            {
                end_word();
                tok.push_back(ch);
                end_word(true);
            }
        }
        else if(isalpha(ch)||ch=='_')
        {
            if(isalpha(tok[0])||tok[0]=='_')
            {
                if(isalnum(ch)||ch=='_');
                else
                    end_word();
                tok.push_back(ch);
            }

            else
            {
                end_word();
                tok.push_back(ch);
            }


        }
        else if(isdigit(ch))
        {
            if(isalnum(tok[0])||tok[0]=='_')
            {
                tok.push_back(ch);
            }

            else
            {
                end_word();
                tok.push_back(ch);
            }
        }
        else
            tok.push_back(ch);
    }
    end_word();
    return tokens;
}

TokenType Tokenizer::get_token_type(std::string tok)
{
    if(tok == "\n")     return DELIMITOR;
    if(tok == "data")   return DATA;
    if(tok == "code")   return CODE;
    if(tok == "end")    return END;
    if(tok == "extern") return EXTERN;
    if(tok == "call")   return CALL;
    if(tok == "return") return RETURN;

    if(*(tok.begin()) == '"' && *(tok.end()-1) == '"') return STRINGCONST;
    if(*(tok.begin()) == '\'' && *(tok.end()-1) == '\'') return CHARCONST;

    if(isnumconst(tok)) return NUMCONST;

    return POSSIBLE_IDENTIFIER;
}

bool Tokenizer::isnumconst(std::string tok)
{
    for(std::string::iterator i=tok.begin(); i!=tok.end(); i++)
        if(!isdigit(*i))
            return false;
    return true;
}


/*else if(is_change(ch,code[i+1]))
        {
            tok.push_back(ch);
            end_word();
        }*/
