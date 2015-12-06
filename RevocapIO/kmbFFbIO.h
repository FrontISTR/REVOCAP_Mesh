/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : FFbIO                                                   #
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
//
// FrontFlow/blue GF1 �`��
//
// HISTORY �f�[�^�̓ǂݍ��ݕ��@
// (1) NEWSET �ɋL�q����Ă��镶������擾
// (2) eachHistory( filename, ["TIME","MAXIMUM DIVERGENT"] ){ |ary| }
//     �� TIME �� MAXIMUM DIVERGENT �̒l�� Array �ɂ��� yield ����
//

//
// �z��w�b�_�ƃL�[���[�h
//
// #FLT_ARY
//  *GRID_2D  �ߓ_���W (X,Y)
//  *GRID_3D  �ߓ_���W (X,Y,Z)
//  *BC_IV2D  �������x (U,V)
//  *BC_IV3D  �������x (U,V,W)
//  *BC_WV2D  �Ǒ��x   (U,V)
//  *BC_WV3D  �Ǒ��x   (U,V,W)
//  *BC_TMPV  ���E���x
//  *BC_HFXV  ���E�M����
//  *TIME_PS  ����
//  *VELO_2D  ���x     (U,V)
//  *VELO_3D  ���x     (U,V,W)
//  *PRES_2E  �v�f�ɂ����鈳��(2D)
//  *PRES_3E  �v�f�ɂ����鈳��(3D)
//  *PRES_3D  �ߓ_�ɂ����鈳��(3D)
//  *BC_PGET  �I�[�o�[�Z�b�g�ߓ_�̋Ǐ����W  (��,��,��)
//  *BC_EGET  �I�[�o�[�Z�b�g�v�f�̋Ǐ����W  (��,��,��)
//  *HISTORY  (���ԃX�e�b�v,�����̔��U�̍ő�l,�S���̋�ԕ��ϒl,...)
// #INT_ARY
//  *NODE_2D  �v�f�ڑ��f�[�^(2D)
//  *NODE_3D  �v�f�ڑ��f�[�^(3D)
//  *ELM_TYP  ���g�p
//  *BC_INLT  �������E�����ߓ_���X�g
//  *BC_WALL  �ǋ��E�����ߓ_���X�g
//  *BC_SYMT  �Ώ̋��E�����ߓ_���X�g
//  *BC_CYCL  �������E�����ߓ_���X�g
//  *BC_BODY  ���̗͌v�Z���E�����ߓ_���X�g
//  *BC_FREE  ���R���E�ߓ_���X�g
//  *BC_MWAL  �ړ����E�ߓ_���X�g => BCVector
//  *BC_INTR  �̈�ԒʐM���E�ߓ_���X�g (�ߓ_�ԍ�,����̈�ԍ�,����ߓ_�ԍ�)
//  *BC_TMPN  ���x���E�����ߓ_���X�g => BCTemp
//  *BC_HFXN  �M�������E�����ߓ_���X�g => BCFlux
//  *STEP_PS  ���ԃX�e�b�v
//  *BC_PSET  �I�[�o�[�Z�b�g���E�ߓ_���X�g (�ߓ_�ԍ�,�v�f�ԍ�,�̈�ԍ�)
//  *BC_ESET  �I�[�o�[�Z�b�g���E�v�f���X�g (�v�f�ԍ�,�v�f�ԍ�,�̈�ԍ�)
//  *ELM_ATRE ���W�n�ԍ�
//  *PT_NODE  �ߓ_��
//  *PT_ELEM  �v�f��
//  *BC_INTR  �̈�ԒʐM���E (�ߓ_�ԍ�,����̗̈�ԍ�,����̐ߓ_�ԍ�)

// �|�X�g�f�[�^�ɂ��Ă͓ǂݍ��܂�Ă���v�f�E�ߓ_�ȏ�̃f�[�^�͓ǂ܂Ȃ�

#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "MeshDB/kmbElement.h"
#include "RevocapIO/kmbFortranIO.h"

namespace kmb{

class MeshData;

class FFbIO : public FortranIO
{
private:
	enum boundaryType{
		INLET,
		FREE,
		WALL,
		MOVINGWALL,
		SYMMETRIC,
		CYCLIC,
		TEMP,
		HEAT,
		BODY,
		CUSTOM,       // �ėp���E�����ݒ�p
		A_WALL,
		A_NRBC,
		INTERCONNECT
	};
	std::vector< kmb::nodeIdType > inlet;
	std::vector< kmb::nodeIdType > movingWall;
	std::vector< kmb::nodeIdType > temp;
	std::vector< kmb::nodeIdType > heat;
	bool readPost;
	// c_str �ŕԂ�����������Ȃ��悤��
	std::string header;
	// �ǂݍ��ނƂ��Ɉ����āA�������ނƂ��ɑ���
	// �t�@�C���� id �� 1 ����n�܂鎞�́A������ offset �� 1 �ɂ���
	kmb::nodeIdType offsetNodeId;
	kmb::elementIdType offsetElementId;
public:
	FFbIO(void);
	virtual ~FFbIO(void);
	int loadFromMeshFile(const char* filename,kmb::MeshData* mesh);
	int loadFromBoundaryFile(const char* filename,kmb::MeshData* mesh);
	// ����̃X�e�b�v�̂��̂�ǂ� step = -1 �̎��͕K���ǂ�
	int loadPostStep(const char* filename,kmb::MeshData* mesh,int step=-1);
	const char* loadHeader(const char* filename);
	// �߂�l�͎l�ʑ̃��f���������� 4�A�Z�ʑ̃��f���������� 6
	// �����̏ꍇ�� 8
	int saveToMeshFile(const char* filename,const kmb::MeshData* mesh);
	int saveToMeshFileVer6(const char* filename,const kmb::MeshData* mesh);
	// REVOCAP �Ǝ��g��
	// stype = "CUSTOM" �� NodeGroup �� BoundaryFile �ɏo�͂���
	int saveToBoundaryFile(const char* filename,const kmb::MeshData* mesh);
	int saveToBoundaryFileAcoustic(const char* filename,const kmb::MeshData* mesh);
protected:
	// read �n�� header �̎��̍s����
	// write �n�� header ���܂߂ďo��
	// unformatted endian check
	bool checkFormat(std::ifstream &input);
	// #A_GF_V1 or #U_GF_V1 �̌�̃R�����g
	int readHeader(std::ifstream &input,kmb::MeshData* mesh);
	int readHeader(std::ifstream &input,std::string &str);
	int writeHeader(std::ofstream &output,const kmb::MeshData* mesh);
	int writeFooter(std::ofstream &output,const kmb::MeshData* mesh);
	// #NEW_SET �̌�̃R�����g�̉���
	int readNewSet(std::ifstream &input,kmb::MeshData* mesh);
	int writeNewSet(std::ofstream &output,const kmb::MeshData* mesh);
	// �ߓ_
	int readGrid3D(std::ifstream &input,kmb::MeshData* mesh);
	int writeGrid3D(std::ofstream &output,const kmb::MeshData* mesh);
	// �v�f
	int readNode2D(std::ifstream &input,kmb::MeshData* mesh);
	int readNode3D(std::ifstream &input,kmb::MeshData* mesh);
	int writeNode2D(std::ofstream &output,const kmb::MeshData* mesh,kmb::bodyIdType bodyId);
	int writeNode3D(std::ofstream &output,const kmb::MeshData* mesh,kmb::bodyIdType bodyId);
	int writeNode2DVer6(std::ofstream &output,const kmb::MeshData* mesh,kmb::bodyIdType bodyId);
	int writeNode3DVer6(std::ofstream &output,const kmb::MeshData* mesh,kmb::bodyIdType bodyId);
	// ���E��
	// BC_INLT BC_MWAL : NodeVariable, Vector3
	// BC_FREE BC_WALL BC_CYCL BC_SYMT BCA_WAL BCA_NOR : NodeGroup
	// BC_INTR : NodeVariable, Array of Integer
	int readBC(std::ifstream &input,kmb::MeshData* mesh,boundaryType btype,const char* name);
	int readBCV(std::ifstream &input,kmb::MeshData* mesh,boundaryType btype,const char* name);
	int readBC3D(std::ifstream &input,kmb::MeshData* mesh,boundaryType btype,const char* name);
	int readBCINTR(std::ifstream &input,kmb::MeshData* mesh,boundaryType btype,const char* name);
	int writeBC(std::ofstream &output,const kmb::MeshData* mesh,boundaryType btype,const char* name="BC_INLT");
	// ���͒l
	int readPres2E(std::ifstream &input,kmb::MeshData* mesh);
	int readPres3E(std::ifstream &input,kmb::MeshData* mesh);
	int readPres3D(std::ifstream &input,kmb::MeshData* mesh);
	// ���x
	int readVelo3D(std::ifstream &input,kmb::MeshData* mesh);
	int readVelo2D(std::ifstream &input,kmb::MeshData* mesh);
	// ���̑��̈�ʓI�ȃL�[���[�h�Ή�
	int readHeaderFlt(std::ifstream &input,kmb::MeshData* mesh);
	int readHeaderInt(std::ifstream &input,kmb::MeshData* mesh);
	int writeHeaderFlt(std::ofstream &output,const kmb::MeshData* mesh,const char* name,const char* comment);
	int writeHeaderInt(std::ofstream &output,const kmb::MeshData* mesh,const char* name,const char* comment);
};

}
