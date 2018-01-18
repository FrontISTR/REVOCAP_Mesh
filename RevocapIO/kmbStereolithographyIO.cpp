#include "RevocapIO/kmbStereolithographyIO.h"

#include <fstream>
#include <iostream>

kmb::StereolithographyIO::fileType kmb::StereolithographyIO::checkFormat(std::string filename)
{
	std::ifstream input( filename.c_str(), std::ios_base::in|std::ios_base::binary );
	input.seekg(0,std::ios::beg);
	char buf[80];
 	input.read(buf,80);
	if (strncmp(buf, "solid", 5)==0) {
		for (int i = 0; i < 80; ++i) {
			if (buf[i] == '\n' || buf[i] == '\r') {
				input.close();
				return kmb::StereolithographyIO::kASCII;
			}
		}
	}
 	input.close();
	return kmb::StereolithographyIO::kBINARY;
}
