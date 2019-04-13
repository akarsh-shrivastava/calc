#ifndef SEGMENTS_H_INCLUDED
    #define SEGMENTS_H_INCLUDED
    #include <vector>
    #include <map>
    #include <string>
    #include "structs.h"

    struct Segments
    {
        std::map<std::string, std::vector<Token>> segments;
        std::string error_msg;

        std::map<std::string, std::vector<Token>> get_segments(std::vector<Token> tokens);
    };

#endif