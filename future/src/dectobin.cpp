#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <cstdlib>

#include "dectobin.h"

Dectobin::Dectobin(std::string file_name)
{
    filename = file_name;
}

void Dectobin::create_bin(bool keep = true)
{
    std::size_t dot_pos = filename.find(".");

    if (filename.substr(dot_pos) != ".del")
    {
        std::cout<<"invalid file type\n";
        exit(0);
    }

    std::ifstream fin(filename.c_str() ,std::ios::in);
    std::string code="";

    if(!fin)
    {
        std::cout<<"File not found";
        exit(0);
    }
    else
    {
        char ch;
        while(fin.get(ch))
            code.push_back(ch);
        fin.close();
    }


    filename = filename.substr(0,dot_pos);

    
    std::ofstream fout;
    fout.open( (filename+".ahl").c_str(), std::ios::binary );
    std::stringstream ss(code);
    unsigned int int_inp;
    unsigned char ch;


    ch = 253;
    fout.write((char*)&ch,1);
    ch = 254;
    fout.write((char*)&ch,1);

    while(ss>>int_inp)
    {
        ch=(unsigned char)int_inp;
        fout.write((char*)&ch,1);
    }
    ch=EOF;
    fout.write((char*)&ch,1);
    fout.close();

    if(!keep)
    {
        std::string remove_command("rm ");
        remove_command += (filename +".del");
        system(remove_command.c_str());
    }
}