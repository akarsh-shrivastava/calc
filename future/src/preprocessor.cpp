#include <string>
#include <iterator>
#include "structs.h"
#include "preprocessor.h"

Preprocessor::Preprocessor(std::string c)
{
    code=c;
}

void Preprocessor::preprocess()
{
    //remove_backslash();
    remove_comments();
    //replace_macros();
}

/*void Preprocessor::remove_backslash()
{
    std::string::iterator itr;
    for(itr=code.begin();itr<code.end();itr++)
    {
        if (*itr == '\\' && *(itr+1) =='\n' )
        {
            code.erase(itr);
            code.erase(itr);

        }
    }
}*/

void Preprocessor::remove_comments()
{
    Flag_struct flags;
    std::string::iterator itr;
    for(itr=code.begin();itr<code.end();itr++)
    {
        if (*itr=='"')
            if (!flags.char_open)
                flags.str_open=(flags.str_open)? 0:1;
        if (*itr=='\'')
            if (!flags.str_open)
                flags.char_open=(flags.char_open)? 0:1;
        
        if (flags.char_open==0&&flags.str_open==0)
        {
            if (*itr == '/' && *(itr+1) =='/' )
            {
                while ((*itr)!='\n')
                {
                    code.erase(itr);    
                }
            }
        }
    }

    for(itr=code.begin();itr<code.end();itr++)
    {
        if (*itr == '/' && *(itr+1) =='*' )
        {
            while (!(((*itr)=='*')&&((*(itr+1))=='/')))
            {
                code.erase(itr);
            }
            code.erase(itr);
            code.erase(itr);
        }
    }
}

/*
void Preprocessor::replace_consts()
{
    std::string::iterator i;
    for(i=code.begin();i<code.end();i++)
    {
        if (*i=='"')
            if (!flags.char_open)
                flags.str_open=(flags.str_open)? 0:1;
        if (*i=='\'')
            if (!flags.str_open)
                flags.char_open=(flags.char_open)? 0:1;

        if (flags.char_open==0&&flags.str_open==0)
        {
            if(*i=='#')
            {
                int f=1;
                std::string::iterator j=i-1;
                while(j!=';'&&f==1)
                {
                    if(isspace(*j));
                    else
                        f=0;
                }

                if (f==1)
                {
                    if(*(i+1)=='d'&&*(i+2)=='e'&&*(i+3)=='f'&&*(i+4)=='i'&&*(i+5)=='n'&&*(i+6)=='e')
                    {
                        //regex se hi karna hoga
                    }
                }
            }
        }
    }
}*/

std::string Preprocessor::get_preprocessed_code()
{
    return code;
}
