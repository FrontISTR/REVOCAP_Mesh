/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : RevocapCouplerIO                                        #
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
// REVOCAP_Coupler �����p����f�[�^�̂h�^�n
//
// �̈敪����̃t�@�C��
// ���Id=�擪�̐���
// �Ǐ�Id=�t�@�C���ɕ\��鏇��(0,1,2,...)
//

#pragma once

#include "MeshDB/kmbElement.h"
#include <fstream>

namespace kmb{

class MeshData;

class RevocapCouplerIO
{
private:
	kmb::nodeIdType offsetNodeId;
	kmb::elementIdType offsetElementId;
	// �v�f��ǂݍ���� globalId �œo�^
	kmb::bodyIdType loadElement( std::ifstream &input, kmb::elementType etype, size_t elementCount, kmb::MeshData* mesh );
	size_t loadNode( std::ifstream &input, size_t nodeCount, kmb::MeshData* mesh );
	size_t loadLocalNode( std::ifstream &input, size_t nodeCount, kmb::MeshData* mesh );
public:
	RevocapCouplerIO( kmb::nodeIdType offsetNodeId=0, kmb::elementIdType offsetElementId=0 );
	virtual ~RevocapCouplerIO(void);
	// mesh �ɂ�globalId�Ŋi�[����
	int loadFromPartitionFile(const char* filename,kmb::MeshData* mesh);
	// �ߓ_�݂̂�ǂݍ���
	// Id ��localId�Ŋi�[����
	// globalId�� Data �Ƃ��� name = "GlobalId" �ŎQ��
	int loadLocalNodesFromPartitionFile(const char* filename,kmb::MeshData* mesh);
};

}
