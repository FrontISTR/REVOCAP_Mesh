/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MicroAVSIO                                              #
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
// tetra �̏����� REVOCAP �Ƌt�ł��邱�Ƃɒ��ӂ��邱��
//
// REVOCAP AVS
// 0       1
// 1       2
// 3       3
// 2       4
#pragma once

#include "MeshDB/kmbTypes.h"
#include <fstream>

namespace kmb{

class MeshData;
#ifndef REVOCAP_SUPPORT_RUBY
class StructuredGrid;
#endif

class MicroAVSIO
{
private:
	enum dataType{
		DATA = 0x01,
		GEOM = 0x02,
		DATA_GEOM = 0x03,
		Undefined
	};
	dataType dtype;
	kmb::nodeIdType nodeOffset;
	kmb::elementIdType elementOffset;
	// �x�N�g���l�Ƃ��ĉ���
	bool asVector3;
	bool summary;
	// avs �t�@�C���̃o�[�W����
	int version;
	int nodeCount;
	int elementCount;
	int nodeDimCount;
	int elementDimCount;
protected:
	// �ŏ��̃R�����g�ȊO�̍s���P�s�̎��Aver 9
	// ����ȊO�̎��� ver 8
	// -1 �Ȃ�G���[
	int getVersion(std::ifstream &input) const;
	// �X�e�b�v����Ԃ�
	int readHeader(std::ifstream &input);
	int skipGeom(std::ifstream &input);
	int readGeom(std::ifstream &input,kmb::MeshData* mesh);
	int readData(std::ifstream &input,kmb::MeshData* mesh);
	// �f�[�^�̖��O�𒊏o����
	void getDataName(std::string &line);
#ifndef REVOCAP_SUPPORT_RUBY
	// Field �f�[�^�Q�ƌ^
	int readCoord(int n,std::string filename,std::string filetype,int skip,int offset,int stride,kmb::StructuredGrid* grid);
	int readVariable(std::string label,int n,std::string filename,std::string filetype,int skip,int offset,int stride,kmb::StructuredGrid* grid);
#endif
public:
	MicroAVSIO(void);
	virtual ~MicroAVSIO(void);
	int loadFromFile(const char* filename,kmb::MeshData* mesh);
#ifndef REVOCAP_SUPPORT_RUBY
	int loadFromFile(const char* filename,kmb::StructuredGrid* grid);
#endif
	// grid �͓ǂ܂��� data �����ǂ�
	int loadPostFromFile(const char* filename,kmb::MeshData* mesh);
	int saveToFile(const char* filename,kmb::MeshData* mesh);
	int saveToFile_V8(const char* filename,kmb::MeshData* mesh);
	// �i�[����Ă���f�[�^�� 3 �����Ȃ����Ɏ����I�Ƀx�N�g���l�ɂ���
	void setReadAsVector3( bool flag );
	// FrontISTR �� MicroAVS �`���������I�Ƀx�N�g���l�ɂ���
	void setSummary( bool flag );
};

}
