/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.7                          #
# Class Name : MeshOperation                                           #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2013/05/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/
#pragma once

#include "MeshDB/kmbTypes.h"
#include <map>
#include <cstddef>

namespace kmb{

class MeshData;
class NodeNeighborInfo;
class ElementBase;
class ElementContainer;
class DataBindings;

class MeshOperation
{
protected:
	kmb::MeshData* mesh;
public:
	MeshOperation(kmb::MeshData* _mesh);
	virtual ~MeshOperation(void);
	// �̈悲�Ƃɐߓ_�ԍ����d�����Ȃ��悤�ɂ���
	// �\����͗p�̃��f���̏ꍇ�A�ٍޗ��Ԃɗ͂��`���Ȃ��Ȃ�̂Œ��ӂ���B
	size_t nodeSplitByVolumes(void);
	// body ��̐ߓ_�ԍ��𕡐�
	// coupleName �� NULL �ł͂Ȃ��Ƃ��́AorgNodeId => newNodeId �̑Ή��� Data �Ƃ��ċL�^����
	// �߂�l�͒u���������ߓ_�̌���Ԃ�
	int duplicateNodeIdOfBody(kmb::bodyIdType bodyId,const char* coupleName=NULL);
	// ������ thresh �ȉ��̐ߓ_����ɂ܂Ƃ߂�A�֘A����v�f������������
	// �������A�����v�f���ɋ߂��ߓ_������ꍇ���`�F�b�N���Ȃ��̂ŁA�s���ȗv�f���ł���\��������
	size_t uniteNodes(double thresh);
	// �O�p�`����
	int triangulation(kmb::bodyIdType bodyId);

	// static �֐�
	// �e�q�֌W
	static bool isParent(const kmb::ElementContainer* child,const kmb::ElementContainer* parent,const kmb::NodeNeighborInfo &neighbors);
	// surface ���� facegroup �ւ̕ϊ�
	static int surface2FaceGroup(const kmb::ElementContainer* child,const kmb::ElementContainer* parent,const kmb::NodeNeighborInfo &neighborInfo,kmb::DataBindings* faceGroup);
protected:
	// �e�v�f�̌���
	static kmb::elementIdType getParentElement(const kmb::ElementBase &elem,const kmb::ElementContainer* parent,const kmb::NodeNeighborInfo &neighbors);
};

}
