#include <stack>
#include "csparser.h"

Csparser::Csparser(std::vector<Token> code)
{
    this->code = code;
    bool valid = true;
    bool proceed = true;
}

std::string Csparser::get_cs_asm()
{
    CsState current_state = CS_BEGIN;
    std::vector<Token> q;
    //std::stack<CsState> state_stack;
    for (std::vector<Token>::iterator i = code.begin(); i != code.end(); ++i)
    {
        if(i->type == STRINGCONST)
        {
            error_msg+= std::string("Error at line "+std::to_string(i->line)+": String operations not supported. Ignoring rest of the line\n");
            current_state = CS_ERROR;
        }
        else if(current_state == CS_BEGIN)
        {
            if(i->type==CODE || i->type==END || i->type==DELIMITOR)
                continue;
            if(i->type==EXTERN)
                current_state = CS_EXTERN_DECL;
            else if(i->type==RETURN)
                current_state = CS_RETURN;
        }
        else if(current_state == CS_EXTERN_DECL)
        {
            if(i->type==POSSIBLE_IDENTIFIER)
            {
                current_state = CS_EXTERN_NAME_RECEIVED;
                q.push_back(*i);
            }
            else
            {
                error_msg+= std::string("Error at line "+std::to_string(i->line)+": Expected identifier. Ignoring rest of the line\n");
                current_state = CS_ERROR;
            }
        }
        else if(current_state == CS_EXTERN_NAME_RECEIVED)
        {
            if(i->type==DELIMITOR)
            {
                current_state = CS_EXTERN_DECL_CORRECT;
                q.push_back(*i);
            }
            else
            {
                error_msg+= std::string("Error at line "+std::to_string(i->line)+": Expected delimitor. Ignoring rest of the line\n");
                current_state = CS_ERROR;
            }
        }
        else if(current_state == CS_RETURN)
        {
            if(i->type==NUMCONST)
            {
                current_state = CS_RETURN_NUM;
                q.push_back(*i);
            }
            else if(i->type==POSSIBLE_IDENTIFIER)
            {
                if(data_symbol_table.count(i->lexeme))
                {
                    current_state = CS_RETURN_VAR;
                    q.push_back(*i);
                }
                else
                {
                    error_msg+= std::string("Error at line "+std::to_string(i->line)+": Unknown symbol \""+i->lexeme+"\". Ignoring rest of the line\n");
                    current_state = CS_ERROR;
                }
            }
            else
            {
                error_msg+= std::string("Error at line "+std::to_string(i->line)+": Expected a numeric constant or variable name after return. Ignoring rest of the line\n");
                current_state = CS_ERROR;
            }
        }
        else if(current_state==CS_RETURN_VAR || current_state==CS_RETURN_NUM)
        {
            if(i->type == DELIMITOR)
            {
                current_state = CS_RETURN_CORRECT;
                q.push_back(*i);
            }
            else
            {
                error_msg+= std::string("Error at line "+std::to_string(i->line)+": Expected delimitor. Ignoring rest of the line\n");
                current_state = CS_ERROR;
            }
        }


        /* generating codes for correct cases */
        
        if(current_state == CS_EXTERN_DECL_CORRECT)
        {
            std::vector<Token>::iterator str_itr = q.begin();
            if(!data_symbol_table.count(str_itr->lexeme) && !func_symbol_table.count(str_itr->lexeme))
            {
                while(!q.empty())
                {
                    if(str_itr->type == POSSIBLE_IDENTIFIER)
                    {
                        cs_code+=(std::string("        extern ")+str_itr->lexeme);
                        func_symbol_table[str_itr->lexeme] = 0;
                    }
                    else if(str_itr->type == DELIMITOR)
                    {
                        cs_code+="\n";
                    }
                    q.erase(str_itr);
                }
                current_state = CS_BEGIN;
            }
            else
            {
                error_msg+=("Error at line "+std::to_string(str_itr->line)+": Symbol \""+str_itr->lexeme+"\" redefined\n");
                current_state = CS_ERROR;
            }
        }
        else if(current_state == CS_RETURN_CORRECT)
        {
            std::string ret_val = (q.begin()->type==NUMCONST)?std::string("$")+q.begin()->lexeme:(q.begin()->lexeme+"(%rip)");
            
                cs_code+="        movq "+ret_val+", %rax\n        popq %rbp\n        ret\n";
                q.clear();
            current_state = CS_BEGIN;
        }



        /* handling error */
        if(current_state == CS_ERROR)
        {
            q.clear();
            current_state = CS_BEGIN;
            while(i->type!=DELIMITOR && i!=(code.end()-1)) i++;
            proceed = false;
        }
    }

    std::cerr<<error_msg<<std::endl;
    return cs_code;

}