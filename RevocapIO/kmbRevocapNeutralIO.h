/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : RevocapNeutralIO                                        #
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
/*
 * �����ł� YAML �`���ŏ����ꂽ REVOCAP Neutral Yaml Mesh Format ��
 * YAML ���C�u�������g�킸�ɓǂݍ��ރ��\�b�h����������
 *
 * ���̃o�[�W�����ł́A�L�q����Ă���^�O�̏o�Ă��鏇�Ԃ����ߑł��ɂȂ��Ă���̂Œ���
 */
/**
 * REVOCAP Neutral Yaml Mesh Format �d�l
 * # ���擪�̍s�̓R�����g
 * node �^�O�͐ߓ_
 * node/size �͐ߓ_�̌�
 * node/coordinate �͍��W�z��
 * node/coordinate[i] �� [id,x,y,z]
 * bodyCount �͗v�f�O���[�v�̌�
 * element �^�O�͗v�f�O���[�v�̔z��
 * element[i] �͗v�f�O���[�v
 * element[i]/size �͗v�f�O���[�v�Ɋ܂܂��v�f
 * element[i]/connectivity �͗v�f�O���[�v�Ɋ܂܂��v�f�̐ߓ_�z��
 * element[i]/connectivity[j] �� [id,type,node[0],node[1],..,node[k]]
 */
#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace kmb{

class MeshData;

class RevocapNeutralIO
{
public:
	RevocapNeutralIO(void);
	virtual ~RevocapNeutralIO(void);
	int loadFromRNFFile(const char* filename,kmb::MeshData* mesh);
	int saveToRNFFile(const char* filename,kmb::MeshData* mesh);
	int saveHeader(const char* filename);
	int appendHeader(const char* filename);
	int appendDataToRNFFile(const char* filename,kmb::MeshData* mesh,const char* name,const char* stype=NULL);

	template <typename T> int loadParameterFromRNFFile(const char* filename,int num, const char** tags,T &param)
	{
		if( tags == NULL || num <= 0 ){
			std::cerr << "loadParameterFromRNFFile argument error" << std::endl;
			return -1;
		}
		std::ifstream input( filename, std::ios_base::in );
		if( input.fail() ){
			std::cerr << "loadParameterFromRNFFile file error" << std::endl;
			return -1;
		}
		int res = readParam(input,num,tags,param);
		input.close();
		return res;
	}
protected:
	int writeHeader(std::ofstream &output);
	int readNode(std::ifstream &input,kmb::MeshData* mesh);
	int readElement(std::ifstream &input,kmb::MeshData* mesh);
	int readData(std::ifstream &input,kmb::MeshData* mesh);
	int readShape(std::ifstream &input,kmb::MeshData* mesh);
	template <typename T> int readParam(std::ifstream &input,int num, const char** tags,T &param)
	{
		std::string line;
		std::string token;
		std::string tag(tags[0]);
		tag.append(":");
		while( !std::getline( input, line ).eof() ){
			if( line.size() == 0 || line[0] == '#' ){
				continue;
			}
			std::istringstream tokenizer(line);
			tokenizer >> token;
			if( token == tag ){
				if( num == 1 ){
					tokenizer >> param;
					return 0;
				}else{
					return readParam(input,num-1,&tags[1],param);
				}
			}
		}
		return -1;
	}
};

}
