#ifndef DECTOBIN_H_INCLUDED
	#define DECTOBIN_H_INCLUDED
	#include <string>

	struct Dectobin
	{
		std::string filename;

		Dectobin (std::string file_name);
		void create_bin(bool keep);		
	};
#endif
