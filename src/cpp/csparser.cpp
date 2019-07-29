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
                gen_postfix();
                postfix.clear();
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


void Csparser::gen_postfix()
{
    std::stack <Token> optrs;
    for (std::vector<Token>::iterator i = expression.begin(); i != expression.end(); ++i)
    {
        if(data_symbol_table.count(i->lexeme) || i->type==NUMCONST)
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

    /*for (std::vector<Token>::iterator i = postfix.begin(); i != postfix.end(); ++i)
    {
        std::cout<<i->lexeme;
    }*/


    if (!optrs.empty()) proceed = false;
    else gen_code();
    postfix.clear();
}



void Csparser::gen_code()
{
    std::stack<Token> st;
    for (std::vector<Token>::iterator i = postfix.begin(); i != postfix.end(); ++i)
    {
        if(i->type == POSSIBLE_IDENTIFIER)
        {
            if(!data_symbol_table.count(i->lexeme))
            {
                error_msg += "Error at line "+std::to_string(i->line)+": Unknown symbol \""+i->lexeme+"\"\n";
                proceed = false;
            }
            st.push(*i);
        }
        else if(i->type == NUMCONST)
            st.push(*i);
        else
        {
            std::vector<Token> operands;
            if (st.empty())
            {
                error_msg += "Error at line "+std::to_string(i->line)+": Misplaced operator \""+i->lexeme+"\"\n";
                proceed = false;
                continue;
            }
            operands.push_back(st.top()); st.pop();
            operands.push_back(st.top()); st.pop();
            st.push(get_opcode(*i, operands[1], operands[0]));
        }
    }
}

Token Csparser::get_opcode(Token op,Token operand1, Token operand2)
{
    std::string opcode, reg=registers::get(), code="";
    if(op.lexeme=="++")    opcode+="not supported";
    if(op.lexeme=="--")    opcode+="not supported";
    if(op.lexeme=="!")     opcode+="not supported";
    if(op.lexeme=="~")     opcode+="not supported";
    if(op.lexeme=="u-")    opcode+="not supported";
    if(op.lexeme=="%")     opcode+="not supported";
    if(op.lexeme=="/")     opcode+="divq";
    if(op.lexeme=="*")     opcode+="mulq";
    if(op.lexeme=="+")     opcode+="addq";
    if(op.lexeme=="-")     opcode+="subq";
    if(op.lexeme=="<<")    opcode+="shlq";
    if(op.lexeme==">>")    opcode+="shrq";
    if(op.lexeme=="&")     opcode+="andq";
    if(op.lexeme=="^")     opcode+="xorq";
    if(op.lexeme=="|")     opcode+="orq";
    if(op.lexeme=="=")     opcode+="movq";
    if(op.lexeme=="<"||op.lexeme==">"||op.lexeme==">="||op.lexeme=="<="||op.lexeme=="!="||op.lexeme=="==")    opcode+="not suppprted";
    if(op.lexeme=="&&")    opcode+="not supported";
    if(op.lexeme=="||")    opcode+="not supported";
    if(op.lexeme=="+=")    opcode+="not supported";
    if(op.lexeme=="-=")    opcode+="not supported";
    if(op.lexeme=="*=")    opcode+="not supported";
    if(op.lexeme=="/=")    opcode+="not supported";
    if(op.lexeme=="%=")    opcode+="not supported";
    if(op.lexeme=="&=")    opcode+="not supported";
    if(op.lexeme=="|=")    opcode+="not supported";
    if(op.lexeme=="^=")    opcode+="not supported";
    if(op.lexeme=="<<=")   opcode+="not supported";
    if(op.lexeme==">>=")   opcode+="not supported";

    if(opcode == "not supported")
    {
        error_msg+="Error at line "+std::to_string(op.line)+": Operator "+op.lexeme+" not supported.\n";
        proceed=false;
        return Token(REGISTER, registers::get(), op.line);
    }

    if(opcode == "movq")
    {
        if(operand1.type!=POSSIBLE_IDENTIFIER)
        {
            error_msg+="Error at line "+std::to_string(op.line)+": Bad lvalue (lvalue must be a variable).\n";
            proceed=false;
            return Token(REGISTER, "%rdi", op.line);
        }
        else
        {
            code+="        movq ";
            switch(operand2.type)
            {
                case REGISTER: code+=operand2.lexeme; break;
                case POSSIBLE_IDENTIFIER: code+=operand2.lexeme+"(%rip)"; break;
                case NUMCONST: code+="$"+operand2.lexeme; break;
            }
            code+=", "+operand1.lexeme+"(%rip)\n";
            cs_code+=code;
            return Token(POSSIBLE_IDENTIFIER, operand1.lexeme, operand1.line);
        }
    }
    switch(operand1.type)
    {
        case POSSIBLE_IDENTIFIER:   if(registers::alloc()){
                                        reg=registers::get();
                                        code+="        movq "+operand1.lexeme+"(%rip), "+reg+"\n";
                                    }
                                    else
                                    {
                                        error_msg+="Error at line "+std::to_string(op.line)+": Too many operations in this line. Break them into multiple lines.\n";
                                        proceed=false;
                                        return Token(REGISTER, registers::get(), op.line);
                                    }; break;
        case NUMCONST:              if(registers::alloc()){
                                        reg=registers::get();
                                        code+="        movq $"+operand1.lexeme+", "+reg+"\n";
                                    }
                                    else
                                    {
                                        error_msg+="Error at line "+std::to_string(op.line)+": Too many operations in this line. Break them into multiple lines.\n";
                                        proceed=false;
                                        return Token(REGISTER, registers::get(), op.line);
                                    }; break;
        case REGISTER:              reg = operand1.lexeme; break;
    }
    code+="        "+opcode;
    switch(operand2.type)
    {
        case POSSIBLE_IDENTIFIER: code+=" "+operand2.lexeme+"(%rip)"+", "+reg+"\n"; break;
        case NUMCONST           : code+=" $"+operand2.lexeme+", "+reg+"\n"; break;
        case REGISTER           : code+=" "+operand2.lexeme+", "+reg+"\n"; registers::free(operand2.lexeme); break;
    }

    cs_code+=code;

    return Token(REGISTER, reg, op.line);

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
        ||op=="%="||op=="&="||op=="|="||op=="^="
        ||op=="<<="||op==">>="                        )   return 14;
    return -1;
}