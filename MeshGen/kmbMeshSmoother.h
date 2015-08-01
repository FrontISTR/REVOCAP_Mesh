/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MeshSmoother                                            #
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

#include "MeshDB/kmbTypes.h"

#include <map>
#include <cstddef>

namespace kmb{

class MeshData;
class Point3DContainer;
class ElementContainer;
class ElementEvaluator;
class NodeNeighborInfo;
class DataBindings;

class MeshSmoother
{
protected:
	kmb::MeshData* mesh;
	kmb::Point3DContainer* shiftPoints;
	std::multimap< kmb::nodeIdType, kmb::nodeIdType > nodeConnectTable;
	// �ɘa�W���i0.0 �̎��ɏd�S�Œu�������A1.0�̎��͂��Ƃ̂܂܁j
	double relax;
public:
	MeshSmoother(kmb::MeshData* mesh);
	virtual ~MeshSmoother(void);
	void clear(void);
	// Laplacian Smoothing �̑ΏۂƂȂ�v�f�O���[�v�̐ݒ�
	// �߂�l�� nodeConnectTable �ɓo�^���ꂽ��
	size_t appendBody(kmb::bodyIdType bodyId);
	size_t appendBody(const kmb::ElementContainer* body);

	// shiftPoints �̏�����
	void init(void);
	// �ߓ_������̗v�f�ɏ]���� Laplacian smoothing ����
	// nodeConnectTable �ɓo�^����Ă��Ȃ�������ړ����Ȃ�
	// �߂�l�͈ړ������ߓ_�̌�
	int smoothingNode(kmb::nodeIdType nodeId);
	int smoothingAllNode(void);
	int smoothingNodeGroup(const char* name,const char* stype=NULL);
	// smoothing �̌��ʂ� points �ɓK�p
	void commit(void);
private:
	// nodeConnectTable �ɓo�^
	bool appendConnectNode(kmb::nodeIdType n0,kmb::nodeIdType n1);
};

}
