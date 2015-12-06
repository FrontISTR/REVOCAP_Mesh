/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : BLMGenerator                                            #
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

#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElement.h"

#include <vector>

namespace kmb{

class MeshData;
class ElementContainer;
class Face;

class BLMGenerator
{
protected:
	// �w�̌��ƌ���
	// layerThick �͏��������ɕ���ł��邱��
	int layerNum;
	double* layerThick;
	MeshData* mesh;
public:
	BLMGenerator(void);
	BLMGenerator(MeshData* mesh);
	BLMGenerator(MeshData* mesh,int layerNum,double* layerThick);
	virtual ~BLMGenerator(void);

	// extrude => �����o��
	//   �\�ʃ��b�V���̊O���ɑw���b�V����ǉ����Ă���
	//   ���͂̕\�ʃ��b�V���͈ړ��������ɐߓ_�ԍ����ς��Ȃ�
	//   �o�͂̊O���ɂ͐V�����ߓ_�ԍ��̕\�ʃ��b�V�����ł���
	// intrude => �������݁i�\�ʊg���j
	//   �\�ʃ��b�V����c��܂��ē����ɑw���b�V����ǉ����Ă���
	//   �o�͂̊O���ɂ͓��͂Ɠ����ߓ_�ԍ��̕\�ʃ��b�V���������
	//   �ߓ_�ԍ��͕ς����ɍ��W�l������ς��Ă���
	//   �����̃��b�V�� parentId �͕\�ʂ̐ߓ_�ԍ����u����������
	// intrudeB => �������݁i�\�ʌŒ�j
	//   �\�ʃ��b�V�����Œ肵�ē����ɑw���b�V����ǉ����Ă���
	//   �o�͂̊O���ɂ͓��͂Ɠ����ߓ_�ԍ��̕\�ʃ��b�V���������
	//   ���Ƃ��Ƃ̃��b�V�����Ђ��܂��đw���b�V����}�����Ă���
	//   �����̃��b�V�� parentId �͕\�ʂ̐ߓ_�ԍ����u����������
	// boundalyId   �ΏۂƂȂ�\�ʃ��b�V���� Id�i���o�͌��p�j
	// parentId     �������ݑΏۂƂȂ�{�����[�����b�V���� Id
	// �߂�l       ���������w���b�V���� Id
	kmb::bodyIdType extrude(kmb::bodyIdType &boundaryId);
	kmb::bodyIdType intrude(kmb::bodyIdType parentId,kmb::bodyIdType &boundaryId);
	kmb::bodyIdType intrudeB(kmb::bodyIdType parentId,kmb::bodyIdType &boundaryId);

	// �ʃf�[�^��^����
	// extrude => �����o��
	// intrude => ��������
	// �߂�l  ���������w���b�V���� Id
	// �^�����ʃf�[�^�͑w�̊O���̖ʂɂȂ�悤�ɍX�V�����
	kmb::bodyIdType extrudeFromData(const char* faceGroup);
	kmb::bodyIdType intrudeFromData(const char* faceGroup);
	kmb::bodyIdType intrudeBFromData(const char* faceGroup);

	// �o�^���� MeshData �� BLM �𐶐�����i���͖ʂ̓����j
	// �^����ꂽ faceGroup �͐V�������E�ʂɍX�V�����itargetBodyId���j
	// ����ȊO�̋��E�ʂ͕ω����Ȃ����Ƃ�ۏ؂���
	// faceGroup �̕\�ʗ̈�̓����ɂ��� BLM ����邱�Ƃɒ��ӂ���
	bool generateInnerFromData(const char* faceGroup,kmb::bodyIdType &layerId);

	// �o�^���� MeshData �� BLM �𐶐�����i���͖ʂ̓����j
	// �^����ꂽ bodyId �͐V�������E�ʂɍX�V�����
	// ����ȊO�̋��E�ʂ͕ω����Ȃ����Ƃ�ۏ؂���
	// ���͕\�ʂ̓����ɂ��� BLM ����邱�Ƃɒ��ӂ���
	bool generateInner(kmb::bodyIdType bodyId,kmb::bodyIdType parentId,kmb::bodyIdType &layerId);

	// layer �͐ώZ�ŗ^����
	// �Ⴆ�� thicks = [0.1, 0.2, 0.3, 0.5, 0.8]
	// setLayer(5,thicks)
	// �̂悤��
	// thicks �̃������Ǘ��͌Ăяo�������ӔC������
	void setLayer(int layerNum,double* thicks);

	void setMesh(MeshData* mesh);
protected:
	// getLayerNodeId( outerNodeId, innerNodeId, layerIndex );
	// 0 <= layerIndex <= layerNum
	// layerIndex = 0             : innerNodeId
	// layerIndex = layerNum      : outerNodeId
	// 0 < layerIndex < layerNum  : innerNode �� outerNode �� layerThick �̊Ԋu��
	//                              ���������ߓ_�� nodeId ��Ԃ�
	//                              �܂��ߓ_���Ȃ���΍��A���ɂ���΃L���b�V������
	//                              �f�[�^����Ԃ�
	// �L���b�V���̓��e��
	// layers = layerNodes[ std::pair(innerNodeId,outerNodeId) ];
	// layers[0] : layerIndex = 1
	// layers[1] : layerIndex = 2
	// layers[2] : layerIndex = 3
	// .....
	// layers[layerNum-2] : layerIndex = layerNum-1
	kmb::nodeIdType getLayerNodeId(kmb::nodeIdType outerNodeId, kmb::nodeIdType innerNodeId, int layerIndex ) const;


	// �v�f�̌^ (etype, oldNodeId) �ɑ΂��� nodeMapper �ŗ^������Ή��������
	// �V�ߓ_�Ƌ��ߓ_�̊Ԃɗv�f��o�^����Ă��� mesh �� addElement �Œǉ�
	// ���̃��\�b�h�� beginElement �� endElement �ɋ���Ŏg������
	// oldNodeId ���O�����猩����Ƃ� outer = true
	kmb::Face appendLayerElements( kmb::elementType etype, kmb::nodeIdType* oldNodeId, bool outer=true);
private:
	mutable std::map< std::pair<kmb::nodeIdType,kmb::nodeIdType>, kmb::nodeIdType* > layerNodes;
	void clearLayerNodes(void);
	// ��d�������ߓ_�̑Ή�
	// original => duplicated
	std::map< kmb::nodeIdType, kmb::nodeIdType > nodeMapper;
	kmb::nodeIdType getDuplicatedNodeId(kmb::nodeIdType nodeId);
};

}
