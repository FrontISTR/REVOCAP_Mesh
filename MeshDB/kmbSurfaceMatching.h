/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : Matching                                                #
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
 * �ړI
 * Surface �� FaceGroup �̏��Ԃƌ����܂ō��߂��}�b�`���O
 * Surface �ő���Ȃ��Ƃ���̓_�~�[�̖ʂ�ǉ����邱�Ƃ��\
 * ����
 * Surface �� bodyId
 * FaceGroup
 * �o��
 * FaceGroup �� face �� Surface �� elementId �̊Ԃ̑Ή�
 */

#pragma once

#include <string>
#include <map>
#include <set>
#include "MeshDB/kmbTypes.h"

namespace kmb{

class MeshData;
class ElementContainer;
class DataBindings;
class NodeNeighborFaceInfo;
class Face;
class Collision;
class Matrix;
class Permutation;

class SurfaceMatching
{
private:
	kmb::MeshData* mesh;
	kmb::bodyIdType masterId;
	kmb::ElementContainer* masterSurf;
	std::string slaveName;
	kmb::DataBindings* slaveFaceGroup;
	kmb::ElementContainer* slaveElements;
	// FaceGroup �̋ߖT���
	kmb::NodeNeighborFaceInfo* neighborInfo;
	kmb::Matrix* distanceMatrix;
	kmb::elementIdType* elementIds;
	kmb::Face* faces;
	kmb::Collision* collision;
	kmb::DataBindings* insertedNodes;
	struct rotatedElement{
		kmb::elementIdType elementId;
		int index;
	};
	std::map< kmb::Face, rotatedElement > mapping;
	// �z�� elementIds �� index �ɂ��ڑ��֌W
	std::set< std::pair< int, int > > connectionTable;
public:
	SurfaceMatching(void);
	virtual ~SurfaceMatching(void);
	void setMesh(kmb::MeshData* mesh,const char* insNodes=NULL);
	// facegroup => surface �̃}�b�`���O���s��
	void setPair(kmb::bodyIdType bodyId,const char* faceGroup);
	kmb::elementIdType getMatchingElementId(kmb::Face f,int &index);
	// ���ׂĂ� Face �ɑ΂��đΉ�����v�f������悤�ɁA�Ȃ���΃_�~�[��ǉ�����B
	// MatchingElement ���������āA�Ȃ���� appendDummyElement ���Ăяo���B
	// �ǉ����� element �̌���Ԃ�
	size_t constructDummyElements(void);
private:
	void clear(void);
	void clearPairInfo(void);
	// �������ŏ��ɂȂ�悤�ȑg�ݍ��킹��T��
	bool calcMapping(void);
	// f �� match ����v�f�����݂��Ȃ��Ƃ��ɁA���E�̋ߖT�̐ߓ_���g���� masterSurf �Ƀ_�~�[�v�f�̒ǉ�
	// �ǉ������Ƃ��͐ߓ_���W�͂Q�d������Ă���̂ŁA���Ƃ���ߓ_�����炷����
	// �ǉ������v�f�̌����� f �ƍ����悤�ɂ���A���Ȃ킿 getMatchingElementId �œ����� index = 0
	kmb::elementIdType appendDummyElement(kmb::Face f);
	// nodeId n0 �𕡐����� n1 ������Ă����Ԃ�
	// insertedNodes �ɓo�^
	kmb::nodeIdType duplicateNode(kmb::nodeIdType n0);
	// �ʑ��������悤�ȑΉ����ǂ������`�F�b�N����
	// ����Ȃ��ꍇ�͈ʑ�������Ȃ����� perm �� index �̂����̑傫������Ԃ�
	// �ʑ��������ꍇ�� -1 ��Ԃ�
	// ����ȊO�̃G���[�� -2 ��Ԃ�
	int checkTopologicalMapping( kmb::Permutation& perm ) const;
};

}
