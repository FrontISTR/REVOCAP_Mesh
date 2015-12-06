/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : HecmwIO                                                 #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
# Software Name : RevocapMesh version 1.2                              #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2008/03/14     #
#                                           T. Takeda   2008/03/14     #
#                                           K. Amemiya  2008/03/14     #
#                                                                      #
#      Contact Address: RSS21 Project, IIS, The University of Tokyo    #
#                                                                      #
#      "Innovative General-Purpose Coupled Analysis System"            #
#                                                                      #
----------------------------------------------------------------------*/
#pragma once

#include <string>
#include <vector>
#include "MeshDB/kmbElement.h"

namespace kmb{

class MeshData;
class ElementContainer;
class DataBindings;

class HecmwIO
{
public:
	enum solutionType{
		STATIC,
		NLSTATIC,
		HEAT,
		EIGEN,
		DYNAMIC
	};
private:
	struct egrpData{
		std::string egrpName;
		std::string materialName;
		size_t egrpCount;
		kmb::bodyIdType bodyId;
	};
	// �ǂݍ��ނƂ��Ɉ����āA�������ނƂ��ɑ���
	// �t�@�C���� id �� 1 ����n�܂鎞�́A������ offset �� 1 �ɂ���
	kmb::nodeIdType offsetNodeId;
	kmb::elementIdType offsetElementId;
	solutionType soltype;
	// true �̂Ƃ��� spectype �Ɉ˂炸�ɏo�͂���
	bool dataFlag;
	bool resElementFlag;
	bool dummySectionFlag;
	std::string resHeader;
	std::vector< kmb::HecmwIO::egrpData > egrpInfo;
public:
	HecmwIO( kmb::nodeIdType offsetNodeId=1, kmb::elementIdType offsetElementId=1);
	~HecmwIO(void);
	void clear(void);

	// HEC msh �t�@�C����ǂݍ���
	// ���ׂĂ� element �͂ǂ����� section �Ɋ܂܂�Ă���Ƃ���
	// SECTION �Œ�`����Ă��� EGRP �� MeshData �� Body �̒P�ʂɂ���
	// MATERIAL => BodyGroup, Hash�l (Data �̖��O�� MATERIAL �̖��O�œo�^)
	//             young
	//             poisson
	//             density
	//             linear_exp
	//             specific_heat
	//             heat_conductivity
	// SECTION => BodyVariable, Hash�l (Data �̖��O�� SpecType �� SECTION)
	//         TYPE => "SOLID" or "SHELL" or "INTERFACE"
	//         THICKNESS => float
	//         INTEGPOINTS => integer
	//         GAPCON => float
	//         GAPRAD1 => float
	//         GAPRAD2 => float
	int loadFromFile(const char* filename,kmb::MeshData* mesh);
	int loadFromMW3File(const char* filename,kmb::MeshData* mesh) const;
	int loadFromFRSFile(const char* filename,kmb::MeshData* mesh) const;
	int loadFromResFile(const char* filename,kmb::MeshData* mesh) const;
	int loadFromResFileItem(const char* filename,kmb::DataBindings* data,const char* name) const;
	// �A�X�L�[�f�[�^�̓ǂݍ���
	int loadFromResAsciiFile(const char* filename,kmb::MeshData* mesh) const;
	// �o�C�i���f�[�^�̓ǂݍ���
	int loadFromResBinFile(const char* filename,kmb::MeshData* mesh) const;
	// ���b�V���t�@�C���o��
	int saveToFile(const char* filename,const kmb::MeshData* mesh) const;
	// �o�C�i���`���ŏo��
	int saveToResFile(const char* filename,kmb::MeshData* mesh) const;
	// MW3 �`���ŏo��
	int saveToFileMW3(const char* filename,const kmb::MeshData* mesh,const char* partName) const;
	// Ver.3.6 �`��
	int saveToMeshFile_3_6(const char* filename,const kmb::MeshData* mesh) const;
	// Ver.4.4 �`��
	int saveToMeshFile_4_4(const char* filename,const kmb::MeshData* mesh,const char* partName) const;
	// master �̐ߓ_�� slave �̗v�f�ŕ]������ equation data ���o�͂���
	// ��Βl�� thresh �ȉ��̍��͏o�͂��Ȃ�
	// �v�f���W�� outThresh �ȉ��̍�������Ε��������o�͂��Ȃ�
	int appendEquationToFile(const char* filename,const kmb::MeshData* mesh,const char* master,const char* slave,double thresh=1.0e-10,double outThresh=-1.0e-6) const;
	// SGROUP �� msh �t�@�C���ɒǉ�����
	int appendSgroupToFile(const char* filename,const kmb::MeshData* mesh,const char* sgroup,const char* stype=NULL) const;
	int appendFooterToFile(const char* filename) const;
	solutionType setSolutionType( const char* soltype );
	const char* getSolutionType( void ) const;
	// NGROUP EGROUP SGROUP ��ǂݏ�������ꍇ�� true
	void setDataFlag(bool flag);
	// res �t�@�C���� element �̌��ʂ�ǂޏꍇ�� true
	void setResElementFlag(bool flag);
	// �_�~�[�� SECTION ��ǉ�����ꍇ�� true
	void setDummySectionFlag(bool flag);

	void setResHeader(const char* header);
	void setOffsetElementId(kmb::elementIdType offset);
	void setOffsetNodeId(kmb::nodeIdType offset);
	kmb::elementIdType getOffsetElementId(void) const;
	kmb::nodeIdType getOffsetNodeId(void) const;

	// �Ǐ��ʔԍ��̕ϊ�
	int correctLocalFaceId( kmb::MeshData* mesh, const char* faceName, const char* stype=NULL );
protected:
	// type �̕ϊ�
	// TETRAHEDRON <=> 341
	static int getHECType( kmb::elementType etype );
	static kmb::elementType getRevocapType( int etype );

	// �J�[�h(!KEYWORD) �̎��̍s����ǂ�ŁA���̃J�[�h��ǂݍ��񂾂炻�̍s�� line �ɑ�����ĕԂ�
	// Header
	int readHeader( std::ifstream &input, std::string &line ) const;
	// Node : mesh = NULL �̎��͌�������Ԃ�
	size_t readNode( std::ifstream &input, std::string &line, kmb::MeshData* mesh ) const;
	// Element : body = NULL �̎��͌�������Ԃ�
	size_t readElement( std::ifstream &input, std::string &line, kmb::ElementContainer* body ) const;
	// Section : mesh = NULL �̎��͋�ǂ�
	// �ǂނƂ��� mesh �� bodyId => Hash �� DataVariable "SECTION" ���쐬����
	// SECTION �̂��т� Body �𐶐����āAbodyId �𔭍s����
	// Hash �̓��e��
	//  TYPE => "SOLID" or "SHELL" or "INTERFACE"
	//  THICKNESS => �X�J���[�l
	//  INTEGPOINTS => �����l
	//  GAPCON => �X�J���[�l
	//  GAPRAD1 => �X�J���[�l
	//  GAPRAD2 => �X�J���[�l
	kmb::bodyIdType readSection( std::ifstream &input, std::string &line, kmb::MeshData* mesh );
	// Material : mesh = NULL �̎��͋�ǂ�
	int readMaterial( std::ifstream &input, std::string &line, kmb::MeshData* mesh ) const;
	// EGroup : mesh = NULL �̎��� egrpInfo �ւ̓o�^�̂�
	size_t readEGroup( std::ifstream &input, std::string &line, kmb::MeshData* mesh, kmb::ElementContainer* parentBody=NULL );
	size_t readNGroup( std::ifstream &input, std::string &line, kmb::MeshData* mesh ) const;
	size_t readSGroup( std::ifstream &input, std::string &line, kmb::MeshData* mesh ) const;

	void writeHeader( std::ofstream &output, std::string version="" ) const;
	void writeNode( std::ofstream &output, const kmb::MeshData* mesh, std::string partName="" ) const;
	void writeElement( std::ofstream &output, const kmb::MeshData* mesh, kmb::bodyIdType bodyId, std::string partName="" ) const;
	void writeMaterial( std::ofstream &output, const kmb::MeshData* mesh ) const;
	void writeSection( std::ofstream &output, const kmb::MeshData* mesh, kmb::bodyIdType bodyId ) const;
	void writeEGroup( std::ofstream &output, const kmb::MeshData* mesh, std::string partName="" ) const;
	void writeNGroup( std::ofstream &output, const kmb::MeshData* mesh, std::string partName="" ) const;
	void writeSGroup( std::ofstream &output, const kmb::MeshData* mesh, std::string partName="" ) const;
	void writeAmplitude( std::ofstream &output, const kmb::MeshData* mesh, std::string partName="" ) const;
	void writeContactPair( std::ofstream &output, const kmb::MeshData* mesh, std::string card="!CONTACT_PAIR", std::string partName="" ) const;
	void writeAssemblyPair( std::ofstream &output, const kmb::MeshData* mesh, std::string partName="" ) const;
	void writeZero( std::ofstream &output, const kmb::MeshData* mesh ) const;
	void writeInitialCondition( std::ofstream &output, const kmb::MeshData* mesh ) const;

	int getEgrpInfoIndex( std::string name ) const;
	int createEgrpInfo( std::string name, std::string matname );

	// �ʂ̕ϊ� Fstr => Revo
	static int tetRmap[];
	static int wedRmap[];
	static int hexRmap[];
	static int pyrRmap[];
	// �ʂ̕ϊ� Revo => Fstr
	static int tetFmap[];
	static int wedFmap[];
	static int hexFmap[];
	static int pyrFmap[];
};

}
