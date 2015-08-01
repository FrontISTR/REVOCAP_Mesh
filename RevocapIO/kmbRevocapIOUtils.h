/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : RevocapIOUtils                                          #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/
#pragma once

#include "Common/kmbCommon.h"

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace kmb{

class RevocapIOUtils
{
public:
	template<typename T> static T reverseEndian(T t){
		std::reverse(
			reinterpret_cast<uint8_t*>(&t),
			reinterpret_cast<uint8_t*>(&t) + sizeof(t)
		);
		return t;
	}
	// "key0 = val0, key1 = val1, ..." �Ƃ���������� key0 ��^���� val0 ��Ԃ�
	// keyCaseSensitive = false �̎��́Akey ���������ɕς��Ē��ׂ�
	static std::string getValue( std::string exp, std::string key, bool keyCaseSensitive=true );
	// "key0, key1 = val1, ..." �Ƃ���������� key0 �܂��� key1 �����邩�ǂ����𒲂ׂ�
	// keyCaseSensitive = false �̎��́Akey ���������ɕς��Ē��ׂ�
	static bool hasKey( std::string exp, std::string key, bool keyCaseSensitive=true );
	// stl �� stl::getline �� safe ��
	// linux ���� \r ���c���Ă��܂��o�O������
	static std::istream& readOneLine(std::istream &input, std::string &str);
	// �R���}�ƃX�y�[�X���X�L�b�v����
	static std::istringstream& skipComma(std::istringstream &tokenizer);
};

}
