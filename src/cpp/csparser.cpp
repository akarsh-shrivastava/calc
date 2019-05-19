#include <stack>
#include "csparser.h"

Csparser::Csparser(std::vector<Token> code)
{
    this->code = code;
    this->valid = true;   /// probably not needed
    this->proceed = true;
}

std::string Csparser::get_cs_asm()
{
    CsState current_state = CS_BEGIN;
    std::vector<Token> q;
    //std::stack<CsState> state_stack;
    for (std::vector<Token>::iterator i = code.begin(); i != code.end(); ++i)
    {
        if(i->type == STRCONST)
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
            else
            {
                for(;i->type!=DELIMITOR;i++)
                    expression.push_back(*i);
                gen_prefix();                
            }
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
                for (std::vector<Token>::iterator itr = code.begin(); itr != code.end(); ++itr)
                {
                    if(itr->lexeme == str_itr->lexeme)
                        i->type = FUNCTION;
                }
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


void Csparser::gen_prefix()
{
    std::stack <Token> optrs;
    for (std::vector<Token>::iterator i = expression.begin(); i != expression.end(); ++i)
    {
        if(data_symbol_table.count(i->lexeme))
            postfix.push_back(*i);
        else if(get_precedence(i->lexeme)!=-1)
        {
            while((!optrs.empty()) && (optrs.top().type!=PAREN_OPEN) && (get_precedence(optrs.top().lexeme)<=get_precedence(i->lexeme)))
            {
                postfix.push_back(optrs.top());
                optrs.pop();
            }
            optrs.push(*i);
        }
        else if(i->type == PAREN_OPEN)
            optrs.push(*i);
        else if(i->type == PAREN_CLOSE)
        {
            for( ;(!optrs.empty())&&(optrs.top().type!=PAREN_OPEN); optrs.pop())
                postfix.push_back(optrs.top());
            if(optrs.empty())
            {
                error_msg += "Error at line "+std::to_string(i->line)+": Unbalanced parentheses \n";
                proceed = false;
                break;
            }
            optrs.pop();
        }
        else
        {
            error_msg += "Error at line "+std::to_string(i->line)+": Unknown symbol \""+i->lexeme+"\"\n";
            proceed = false;
            break;
        }
    }
    while(!optrs.empty())
    {
        if(optrs.top().type == PAREN_OPEN)
        {
            error_msg += "Error at line "+std::to_string(expression.begin()->line)+": Unbalanced parentheses \n";
            proceed = false;
            break;
        }
        postfix.push_back(optrs.top());
        optrs.pop();
    }
    if (!optrs.empty()) proceed = false;
    for (std::vector<Token>::iterator i = postfix.begin(); i != postfix.end(); ++i)
    {
        std::cout<<i->lexeme;
    }
}

struct registers
{
    std::vector<std::string> regs;
    std::vector<std::string>::iterator i;
    registers(){
        regs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9", "%r10", "%r11", "%r12", "%r13"};
        i = regs.begin();
    }
    bool operator++(){
        if(i==regs.end()-1) return false;
        i++; return true;
    }
    bool operator--(){
        if(i==regs.begin()) return false;
        i--; return true;
    }
    std::string get(){
        return *i;
    }
} curr;

void Csparser::gen_code()
{
    stack<Token> st;
    
}


int Csparser::get_precedence(std::string op)
{
    if(op=="++"||op=="--")                                return 1;
    if(op=="!"||op=="~"||op=="u-")                        return 2;
    if(op=="%"||op=="/"||op=="*")                         return 3;
    if(op=="+"||op=="-")                                  return 4;
    if(op=="<<"||op==">>")                                return 5;
    if(op=="<"||op==">"||op==">="||op=="<=")              return 6;
    if(op=="!="||op=="==")                                return 7;
    if(op=="&")                                           return 8;
    if(op=="^")                                           return 9;
    if(op=="|")                                           return 10;
    if(op=="&&")                                          return 11;
    if(op=="||")                                          return 12;
    if(op=="="||op=="+="||op=="-="||op=="*="||op=="/="
        ||op=="%="||op=="&="||op=="|="||op=="^=")         return 14;
    return -1;
}