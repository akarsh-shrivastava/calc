#include <cctype>
#include <string>
#include <vector>
#include "tokenizer.h"
#include "structs.h"


int Tokenizer::is_change(char x, char y)
{
    if (isalpha(x)&&isalpha(y))
        return 0;
    if (isdigit(x)&&isdigit(y))
        return 0;
    if (isspace(x)&&isspace(y))
        return 0;
    if (ispunct(x)&&ispunct(y))
        return 0;
    return 1;
}


void Tokenizer::end_word()
{
    if(tok.length()>0)
        tokens.push_back(tok);
    tok.clear();
}

std::vector<std::string> Tokenizer::tokenize(std::string code)
{
    Flag_struct flags;
    char ch;
    for (int i=0;i<code.length();i++)
    {
        ch=code[i];

        if (ch=='"')
        {
            if (!flags.char_open)
            {
                if(!flags.str_open)
                    end_word();
                flags.str_open=(flags.str_open)?0:1;
            }
        }

        if (ch=='\'')
        {
            if (!flags.str_open)
            {
                if(!flags.char_open)
                    end_word();
                flags.char_open=(flags.char_open)?0:1;
            }
        }

        if (flags.str_open||flags.char_open)
            tok.push_back(ch);
        else if (isspace(ch) && ch!='\n')
            end_word();
        else if (ch=='\n')
        {
            end_word();
            tok.push_back(ch);
            end_word();
        }
        else if (ch=='('||ch==')'||ch=='{'||ch=='}'||ch=='['||ch==']'||ch==',')
        {
            end_word();
            tok.push_back(ch);
            end_word();
        }
        else if (ispunct(ch)&&ch!='_')
        {
            if (tok.length()==0);
            else if(isalnum(tok[0])||tok[0]=='_')
                end_word();
            else if (tok.length()==2)
                end_word();

            tok.push_back(ch);

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

    return tokens;
}

/*else if(is_change(ch,code[i+1]))
        {
            tok.push_back(ch);
            end_word();
        }*/
