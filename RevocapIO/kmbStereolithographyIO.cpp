#include "RevocapIO/kmbStereolithographyIO.h"

#include <fstream>
#include <iostream>
#include <cstring>

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

int kmb::StereolithographyIO::countFacet(std::string filename)
{
	std::ifstream input(filename.c_str(), std::ios_base::in);
	if (input.fail()) {
		std::cout << "Load Error : Can't Open File " << filename << "." << std::endl;
		return -1;
	}
	std::string str;
	int count = 0;
	while (std::getline(input, str)) {
		if (str.find("endfacet") != std::string::npos) {
			count++;
		}
	}
	input.close();
	return count;
}
