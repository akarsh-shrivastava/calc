#include "dsparser.h"

Dsparser::Dsparser(std::vector<Token> data)
{
    this->data = data;
    proceed = true;
    curr_arg_no = 0;
}

std::string Dsparser::get_arg_register(std::string data)
{
    curr_arg_no++;
    switch(curr_arg_no)
    {
        case 1  : return "        mov ["+data+"], rdi\n";
        case 2  : return "        mov ["+data+"], rsi\n";
        case 3  : return "        mov ["+data+"], rdx\n";
        case 4  : return "        mov ["+data+"], rcx\n";
        case 5  : return "        mov ["+data+"], r8\n";
        case 6  : return "        mov ["+data+"], r9\n";
        default : return "        mov rax, [rbp+"+std::to_string((curr_arg_no-6+1)*8)+"]\n        mov ["+data+"], rax\n";
    }
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
            else if(i->type == ARG)
            {
                q.push_back(*i);
                current_state = DS_ARG;
            }
            else
            {
                error_msg+=("Error at line "+std::to_string(i->line)+": Expected delimitor after variable name. Ignoring rest of the line\n");
                current_state = DS_ERROR;
            }

        }
        else if(current_state == DS_ARG)
        {
            if(i->type == DELIMITOR)
            {
                q.push_back(*i);
                current_state = DS_ARG_CORRECT;
            }
            else
            {
                error_msg+=("Error at line "+std::to_string(i->line)+": Expected delimitor after argument declaration. Ignoring rest of the line\n");
                current_state = DS_ERROR;
            }
        }
        

        if(current_state == DS_CORRECT_LINE)
        {
            std::vector<Token>::iterator str_itr = q.begin();
            if(!data_symbol_table.count(str_itr->lexeme))
            {
                ds_code+=("    "+str_itr->lexeme+" dq 0\n");
                data_symbol_table[str_itr->lexeme] = 0;
                q.clear();
                current_state = DS_BEGIN;
            }
            else
            {
                error_msg+=("Error at line "+std::to_string(str_itr->line)+": Symbol \""+str_itr->lexeme+"\" redefined\n");
                current_state = DS_ERROR;
            }
        }
        else if(current_state == DS_ARG_CORRECT)
        {
            std::vector<Token>::iterator str_itr = q.begin();
            if(!data_symbol_table.count(str_itr->lexeme))
            {
                ds_code+=("    "+str_itr->lexeme+" dq 0\n");
                data_symbol_table[str_itr->lexeme] = 0;
                text_section+=get_arg_register(str_itr->lexeme);
                q.clear();
                current_state = DS_BEGIN;
            }
            else
            {
                error_msg+=("Error at line "+std::to_string(str_itr->line)+": Symbol \""+str_itr->lexeme+"\" redefined\n");
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
    
    }
    



    if(proceed)
    {
        return std::string("section .data\n")+ds_code+"\n";
    }
    else
    {
        std::cerr<<error_msg;
        return std::string("");
    }

}