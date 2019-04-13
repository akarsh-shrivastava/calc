#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "structs.h"
#include "tokenizer.h"
#include "segments.h"
#include "dsparser.h"

std::map<std::string, int> symbol_table;

int main(int argc, char** argv)
{
    std::string filename(argv[1]);
    std::size_t dot_pos = filename.find(".");

    std::string code="";
    char ch;


    std::ifstream f(argv[1]);
    if (filename.substr(dot_pos) != ".calc")
    {
        std::cerr<<"invalid file type\n";
        exit(1);
    }
    filename = filename.substr(0,dot_pos);

    if(!f)
    {
        std::cerr<<"No such file\n";
        exit(1);
    }
    while(f.get(ch))
        code.push_back(ch);
    f.close();

    
    Tokenizer tokenizer;
    std::vector<Token> tokens = tokenizer.get_tokens(code);

    Segments segment;
    std::map<std::string, std::vector<Token>> segments = segment.get_segments(tokens);

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

    Dsparser dsparser(segments["data"]);
    std::string ds_asm = dsparser.get_ds_asm();

    std::cout<<"\n\n\n"<<ds_asm<<"\n\n";




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

    return 0;
}