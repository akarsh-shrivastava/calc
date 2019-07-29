#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "structs.h"
#include "segments.h"
#include "dsparser.h"
#include "csparser.h"

std::vector<Token> tokens;
std::map<std::string, int> data_symbol_table;
std::map<std::string, int> func_symbol_table;
std::string text_section;
std::string data_section;


void lexer(std::string);

int main(int argc, char** argv)
{
    std::string filename(argv[1]);
    std::size_t dot_pos = filename.find(".");

    std::string code="";
    char ch;

 
    std::ifstream f(argv[1]);
    if(!f)
    {
        std::cerr<<"No such file\n";
        exit(1);
    }
    f.close();
    if (filename.substr(dot_pos) != ".calc")
    {
        std::cerr<<"invalid file type\n";
        exit(1);
    }
    filename = filename.substr(0,dot_pos);



    lexer(filename+".calc");

    Segments segment;
    std::map<std::string, std::vector<Token>> segments = segment.get_segments(tokens);


    text_section="\
\
.text\n\
    .globl "+filename.substr(filename.rfind("/")+1)+"\n\
    "+filename.substr(filename.rfind("/")+1)+":\n\
        pushq %rbp\n\
        movq %rsp, %rbp\n\
";

    Dsparser dsparser(segments["data"], segments["code"]);
    data_section = dsparser.get_ds_asm();

    Csparser csparser(segments["code"]);
    text_section += csparser.get_cs_asm();

    std::cout<<data_section<<"\n";
    std::cout<<text_section<<"\n\n";




    return 0;
}





    /*std::cout<<"\n\nData Segment:\n";
    for (std::vector<Token>::iterator i = segments["data"].begin(); i != segments["data"].end(); ++i)
    {
        i->print();
    }
    std::cout<<"\nCode Segment:\n";
    for (std::vector<Token>::iterator i = segments["code"].begin(); i != segments["code"].end(); ++i)
    {
        i->print();
    }*/

    /*std::vector<std::string> data={"a","b","c","d"};

    Parser parser(tokens, data);
    parser.to_postfix();
    std::vector<std::string> postfix = parser.postfix;
    std::cout<<std::endl;
    for (std::vector<std::string>::iterator i = postfix.begin(); i != postfix.end(); ++i)
    {
        std::cout<<*i<<std::endl;
    }

    std::cout<<parser.error_msg<<std::endl;*/