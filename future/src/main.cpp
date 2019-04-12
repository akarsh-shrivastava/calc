#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>

#include "preprocessor.h"
#include "tokenizer.h"
#include "analyzer.h"
#include "translator.h"
#include "dectobin.h"

int main(int n,char** args)
{
    std::string filename(args[1]);
    std::size_t dot_pos = filename.find(".");

    if (filename.substr(dot_pos) != ".lsm")
    {
        std::cout<<"invalid file type\n";
        exit(0);
    }
    filename = filename.substr(0,dot_pos);

    std::ifstream f(args[1]);
    std::string code="";
    char ch;

    if(!f)
    {
        std::cout<<"No such file\n";
        exit(0);
    }
    else
    {
        while(f.get(ch))
            code.push_back(ch);
        f.close();
        /*std::cout<<code;*/
    }


    Preprocessor p(code);
    p.preprocess();
    code.clear();
    code=p.get_preprocessed_code();

    Tokenizer t;
    std::vector<std::string> tokens=t.tokenize(code);

    Analyzer a;
    a.build_token_data(tokens);

    Translator tr(a.token_data);
    tr.build_decimal_instr();
    if (tr.error_msg.size() == 0)
    {
        std::ofstream fout((filename+".del").c_str());
        fout<<tr.decimal_code;
        fout.close();        
    }
    else
    {
        std::cout<<tr.error_msg<<"\n\n";
        exit(0);
    }

    Dectobin d(filename+".del");
    d.create_bin(false);

    return 0;
}
