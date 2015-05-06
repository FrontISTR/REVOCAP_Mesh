/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 2.0                          #
# Class Name : StringUtils                                             #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2015/02/20     #
#                                                                      #
----------------------------------------------------------------------*/
#pragma once
#include <string>

namespace kmb{

class StringUtils{
public:


	static void replaceString(std::string& str,std::string from,std::string to){
		std::string::size_type pos( str.find( from ) );
		while( pos != std::string::npos )
		{
			str.replace( pos, from.length(), to );
			pos = str.find( from, pos + to.length() );
		}
	}


	static void chompString(std::string& str,std::string sep="\n\r"){
		std::string::size_type pos( str.rfind( sep ) );
		if( pos != std::string::npos )
		{
			str.replace( pos, str.length() - pos, "");
		}
	}
};

}
