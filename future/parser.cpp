#include <stack>
#include <algorithm>

#include "parser.h"

Parser::Parser(std::vector<std::string> tokens, std::vector<std::string> data)
{
    this->tokens = tokens;
    this->data   = data;
    binop = {"+","-","*","/","%","=","&","|","^","&&","||",
             "+=","-=","*=","/=","%=","&=","|=","^=","<<",">>","<",">","<=",">=","==","!="};

    unop  = {"!","~",/*"-",*/"++","--"};
}

bool Parser::to_postfix()
{
    std::stack<std::string> st;
    for (std::vector<std::string>::iterator i = tokens.begin(); i != tokens.end(); ++i)
    {
        if(*i=="(")
            st.push(*i);
        else if(*i==")" ){
            while(st.top()!="("){
                postfix.push_back(st.top());
                st.pop();
                if(st.empty()){
                    error_msg+="Unexpected ')'\n";
                    return false;
                }
            }
            st.pop();
        }
        else if(std::find(data.begin(), data.end(), *i) != data.end()){
            postfix.push_back(*i);
        }
        else if(std::find(binop.begin(), binop.end(), *i) != binop.end()){
            if(!st.empty()){
                while(get_precedence(*i)>get_precedence(st.top())){
                    postfix.push_back(st.top());
                    st.pop();
                    if(st.empty()||st.top()=="(")
                        break;
                }
            }
            st.push(*i);
        }
        else if(std::find(unop.begin(), unop.end(), *i) != unop.end()){
            if (!st.empty()){
                while(get_precedence(*i)>get_precedence(st.top())){
                    postfix.push_back(st.top());
                    st.pop();
                    if(st.empty()||st.top()=="(")
                        break;
                }
            }
            st.push(*i);
        }
        else{
            error_msg+="Unknown symbol \""+ (*i) +"\"\n";
            return false;
        }
        //check();
    }

    while(!st.empty()){
        postfix.push_back(st.top());
        st.pop();
    }

    return true;
}

int Parser::get_precedence(std::string op)
{
    if(op=="++"||op=="--")                                return 1;
    if(op=="!"||op=="~")                                  return 2;
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
    return 100;
}