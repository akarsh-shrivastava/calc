#include "segments.h"

std::map<std::string, std::vector<Token>> Segments::get_segments(std::vector<Token> tokens)
{
    std::vector<Token> data, code;
    for (std::vector<Token>::iterator i = tokens.begin(); i != tokens.end(); ++i)
    {
        if(i->type == DATA)
        {
            while(i->type!=END && i!= tokens.end())
            {
                if(i->type == CODE)
                {
                    error_msg+=("Error at line "+std::to_string(i->line)+": Code Segment can't be inside Data Segment. Use \"end\" after Data segment\n");
                    goto end_loop;
                }
                data.push_back(*i);
                i++;
            }
            if(i==tokens.end())
            {
                error_msg+=("Error at line "+std::to_string((i-1)->line+1)+": No end statement found after reaching end of file\n");
                goto end_loop;
            }
            data.push_back(*i);

        }
        else if(i->type == CODE)
        {
            while(i->type!=END && i!= tokens.end())
            {
                if(i->type == DATA)
                {
                    error_msg+=("Error at line "+std::to_string(i->line)+": Data Segment can't be inside Code Segment. Use \"end\" after Code segment\n");
                    goto end_loop;
                }
                code.push_back(*i);
                i++;
            }
            if(i==tokens.end())
            {
                error_msg+=("Error at line "+std::to_string((i-1)->line+1)+": No end statement found after reaching end of file\n");
                goto end_loop;
            }
            code.push_back(*i);

        }
    }
    end_loop:;
    if(error_msg.size()==0)
    {
        segments["data"] = data;
        segments["code"] = code;
        return segments;
    }
    else
    {
        std::cerr<<error_msg;
        return segments;
    }
}