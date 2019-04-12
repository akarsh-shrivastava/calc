#include <vector>
#include <string>
#include <iterator>
#include <stack>
#include <queue>
#include "translator.h"
#include "structs.h"

Translator::Translator(std::vector<Token_struct> data)
{
    str2dec =   {
                    {"mov_m",  "1"},  {"mov_l",  "2"},

                    {"add_m",  "3"},  {"add_l",  "4"},
                    {"sub_m",  "5"},  {"sub_l",  "6"},
                    {"mul_m",  "7"},  {"mul_l",  "8"},
                    {"div_m",  "9"},  {"div_l", "10"},
                    {"mod_m", "11"},  {"mod_l", "12"},

                    {"and_m", "13"},  {"and_l", "14"},
                    {"or_m",  "15"},  {"or_l",  "16"},
                    {"not_m", "17"},
                    {"neg_m", "18"},
                    {"xor_m", "19"},  {"xor_l", "20"},
                    
                    {"cmp_m", "21"},  {"cmp_l", "22"},

                    {"jmp",   "23"},

                    {"read_m", "24"},  {"write_m", "25"},
                    {"push_m", "26"},  {"pop_m", "27"},


                    {"less"  , "1"},
                    {"eq"    , "2"},
                    {"lesseq", "3"},
                    {"grt"   , "4"},
                    {"neq"   , "5"},
                    {"grteq" , "6"},
                    {"always", "7"}

                };


    token_data   = data;
    current_line = "";
    decimal_code = "";
    line_count   = 1;
    inst_count   = 2;
    error_msg    = "";
}

bool Translator::is_lable(std::string identifier)
{
    std::map<std::string, std::string>::iterator map_itr;

    for(map_itr=Keywords::labels.begin(); map_itr!=Keywords::labels.end(); map_itr++)
    {
        if (identifier == map_itr->first)
            return true;
    }

    return false;
}

void Translator::syntax_error(std::vector<Token_struct>::iterator itr, std::string err_msg)
{
    error_msg += "Error at line "+std::to_string(line_count)+": "+err_msg+"\n";
    while(itr->tok != "\n") itr++;
    line_count++;
    current_line.clear();
}

void Translator::build_decimal_instr()
{
    std::string status;
    std::vector<Token_struct>::iterator itr;
    for (itr = token_data.begin(); itr != token_data.end(); itr++)
    {
        if (itr->tok == "\n")
        {
            line_count++;
        }
        else if (itr->type == "instruction_1addr")
        {
            current_line += (str2dec[itr->tok+"_m"] + "\n");
            itr++;
            if(itr->type == "ds_addr_open")
            {
                itr++;
                if (itr->type == "constant")
                {
                    current_line += (itr->tok + "\n");
                    itr++;
                    if (itr->type == "ds_addr_close")
                    {
                        itr++;
                        if (itr->type == "delimitor")
                        {
                            current_line += "0\n";
                            decimal_code += current_line;
                            current_line.clear();
                            line_count++;
                            inst_count += 3;
                        }
                        else
                        {
                            syntax_error(itr, "expected end of line");
                        }
                    }
                    else
                    {
                        syntax_error(itr, "address marker ([) not closed");
                    }
                }
                else
                {
                    syntax_error(itr, "not a valid address");
                }
            }
            else
            {
                syntax_error(itr, "expected an address");
            }
        }
        else if (itr->type == "instruction_2addr")
        {
            std::string buff(itr->tok);
            itr++;
            if (itr->type == "ds_addr_open")
            {
                itr++;
                if (itr->type == "constant")
                {
                    current_line += (itr->tok+"\n");
                    itr++;
                    if(itr->type == "ds_addr_close")
                    {
                        itr++;
                        if (itr->type == "seperator")
                        {
                            itr++;
                            if(itr->type == "ds_addr_open")
                            {
                                itr++;
                                if (itr->type == "constant")
                                {
                                    current_line += (itr->tok + "\n");
                                    itr++;
                                    if (itr->type == "ds_addr_close")
                                    {
                                        itr++;
                                        if (itr->type == "delimitor")
                                        {
                                            current_line = (str2dec[buff+"_m"] + "\n" + current_line);
                                            decimal_code += current_line;
                                            current_line.clear();
                                            line_count++;
                                            inst_count += 3;
                                        }
                                        else
                                        {
                                            syntax_error(itr, "expected end of line");
                                        }
                                    }
                                    else
                                    {
                                        syntax_error(itr, "address marker ([) not closed");
                                    }
                                }
                                else
                                {
                                    syntax_error(itr, "not a valid address");
                                }
                            }
                            else if (itr->type == "constant")
                            {
                                current_line += (itr->tok + "\n");
                                itr++;
                                if (itr->type == "delimitor")
                                {
                                    current_line = (str2dec[buff+"_l"] + "\n" + current_line);
                                    decimal_code += current_line;
                                    current_line.clear();
                                    line_count++;
                                    inst_count += 3;
                                }
                                else
                                {
                                    syntax_error(itr, "expected end of line after second argument");
                                }
                            }
                            else
                            {
                                syntax_error(itr, "expected an address or a constant as the second argument");
                            }
                        }
                        else
                        {
                            syntax_error(itr, "expected comma after first address");
                        }
                    }
                    else
                    {
                        syntax_error(itr, "address marker ([) not closed");
                    }
                }
                else
                {
                    syntax_error(itr, "not a valid address");
                }
            }
            else
            {
                syntax_error(itr, "expected an address as the first address");
            }
        }
        else if (itr->type == "jump")
        {
            current_line += (str2dec[itr->tok]+"\n");
            itr++;
            if(itr->type == "jump_condition")
            {
                current_line += (str2dec[itr->tok]+"\n");
                itr++;
                if(itr->type == "possible_identifier")
                {
                    if(is_lable(itr->tok))
                    {
                        current_line += (Keywords::labels[itr->tok]+"\n");
                        itr++;
                        if(itr->type == "delimitor")
                        {
                            decimal_code+=current_line;
                            current_line.clear();
                            line_count++;
                            inst_count+=3;
                        }
                        else
                        {
                            syntax_error(itr, "expected end of line");
                        }
                    }
                    else
                    {
                        syntax_error(itr, "undefined label");
                    }
                }
                else
                {
                    syntax_error(itr, "expected label");
                }
            }
            else
            {
                syntax_error(itr, "expected condition after jump");
            }
        }
        else if (itr->type == "possible_identifier")
        {
            if(is_lable(itr->tok))
            {
                syntax_error(itr, "label name already used");
            }
            else
            {
                Keywords::labels[itr->tok] = std::to_string(inst_count);
                itr++;
                if(itr->type == "colon")
                {
                    itr++;
                    if(itr->type == "delimitor")
                    {
                        line_count++;
                    }
                    else
                    {
                        syntax_error(itr, "expected end of line");
                    }
                }
                else
                {
                    syntax_error(itr, "expected a colon");
                }
            }
        }
        else
        {
            syntax_error(itr,"expected an instruction or a label");
        }
    }
}
