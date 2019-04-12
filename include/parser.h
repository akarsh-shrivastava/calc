#ifndef PARSER_H_INCLUDED
	#define PARSER_H_INCLUDED
	#include <vector>
	#include <string>

	//extern 
	//extern std::vector<std::string> func;
	struct Parser
	{
		std::vector<std::string> grammar;
		std::vector<std::string> binop;
		std::vector<std::string> unop;

		std::vector<std::string> data;


		std::vector<std::string> tokens;
		std::vector<std::string> st;
		std::string error_msg;

		std::vector<std::string> postfix;

		Parser(std::vector<std::string> tokens, std::vector<std::string>);
		bool to_postfix();
		int get_precedence(std::string op);
	};
#endif