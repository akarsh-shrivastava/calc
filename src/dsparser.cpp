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
        case 1  : return "        movq %rdi, "+data+"(%rip)\n";
        case 2  : return "        movq %rsi, "+data+"(%rip)\n";
        case 3  : return "        movq %rdx, "+data+"(%rip)\n";
        case 4  : return "        movq %rcx, "+data+"(%rip)\n";
        case 5  : return "        movq %r8, "+data+"(%rip)\n";
        case 6  : return "        movq %r9, "+data+"(%rip)\n";
        default : return "        movq "+std::to_string((curr_arg_no-6+1)*8)+"(%rbp), %rax\n        movq %rax, "+data+"(%rip)\n";
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
                ds_code+=("    "+str_itr->lexeme+": .quad 0\n");
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
                ds_code+=("    "+str_itr->lexeme+": .quad 0\n");
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
        return std::string(".data\n")+ds_code+"\n";
    }
    else
    {
        std::cerr<<error_msg;
        return std::string("");
    }

}