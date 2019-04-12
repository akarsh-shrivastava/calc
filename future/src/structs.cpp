#include <iostream>
#include "structs.h"

Flag_struct::Flag_struct()
{
    str_open=0;
    char_open=0;
}

void Token_struct::print()
{
    std::cout<<"{\n    "<<tok<<"\n    "<<type<<"\n}\n\n\n";
}

std::map<std::string, std::string> Keywords::labels={};