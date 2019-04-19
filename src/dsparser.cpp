#include "dsparser.h"

Dsparser::Dsparser(std::vector<Token> data)
{
    this->data = data;
    proceed = true;
}

std::string Dsparser::get_ds_asm()
{
    DsState current_state = DS_BEGIN;
    std::vector<Token> q;
    for (std::vector<Token>::iterator i = data.begin(); i != data.end(); ++i)
    {
        if(current_state == DS_BEGIN)
        {
            if(i->type==DATA  || i->type==END || i->type==DELIMITOR)
                continue;
            if(i->type == POSSIBLE_IDENTIFIER)
            {
                q.push_back(*i);
                current_state = DS_GOT_IDEN;
            }
            else
            {
                error_msg+=("Error at line "+std::to_string(i->line)+": Expected variable declaration. Ignoring rest of the line\n");
                current_state = DS_ERROR;
            }
        }
        else if(current_state == DS_GOT_IDEN)
        {
            if(i->type == DELIMITOR)
            {
                q.push_back(*i);
                current_state = DS_CORRECT_LINE;
            }
            else
            {
                error_msg+=("Error at line "+std::to_string(i->line)+": Expected delimitor after variable name. Ignoring rest of the line\n");
                current_state = DS_ERROR;
            }

        }
        if(current_state == DS_ERROR)
        {
            q.clear();
            current_state = DS_BEGIN;
            while(i->type!=DELIMITOR && i!=(data.end()-1)) i++;
            proceed = false;
        }

        if(current_state == DS_CORRECT_LINE)
        {
            std::vector<Token>::iterator str_itr = q.begin();
            if(!symbol_table.count(str_itr->lexeme))
            {
                while(!q.empty())
                {
                    if(str_itr->type == POSSIBLE_IDENTIFIER)
                    {
                        code+=("    "+str_itr->lexeme+" dq 0");
                        symbol_table[str_itr->lexeme] = 0;
                    }
                    else if(str_itr->type == DELIMITOR)
                        code+="\n";
                    q.erase(str_itr);
                }
            }
            else
            {
                error_msg+=("Error at line "+std::to_string(str_itr->line)+": Symbol \""+str_itr->lexeme+"\" redefined\n");
                proceed = false;
                q.clear();
            }
            current_state = DS_BEGIN;
        }
    }
    if(proceed)
    {
        return std::string("segment .data\n")+code+"\n";
    }
    else
    {
        std::cerr<<error_msg;
        return std::string("");
    }

}