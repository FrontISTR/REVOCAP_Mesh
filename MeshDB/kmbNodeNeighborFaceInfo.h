/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : NodeNeighborFaceInfo                                    #
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
//
// �݌v����
//
// �ߓ_�Ɩʁi�v�f�Ƃ��̋Ǐ��ʔԍ��j�̊Ԃ̑Ή����L�^����
// ����ɂ���ē�����ȉ��̏����擾�ł���悤�ɂ���
// * �ߓ_�̎���̖ʂ̌�
// * �ʂɗאڂ��Ă��鑼�̖�
//
#pragma once

#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbFace.h"
#include <map>
#include <vector>
#include <string>

namespace kmb{

class Element;
class DataBindings;
class ElementContainer;

typedef std::multimap< kmb::nodeIdType, kmb::Face > NodeNeighborFace;

class NodeNeighborFaceInfo
{
public:
	NodeNeighborFaceInfo(void);
	virtual ~NodeNeighborFaceInfo(void);
	void clear(void);

	// �t�Ή��̓o�^�ƍ폜
private:
	bool append( kmb::nodeIdType nodeID, kmb::Face faceID );
	bool erase( kmb::nodeIdType nodeID, kmb::Face faceID );
public:
	bool appendCoboundary( kmb::Face face, const kmb::ElementContainer* body );
	bool appendCoboundary( kmb::Face face, const kmb::MeshData* mesh );
	bool appendCoboundary( const kmb::DataBindings* data, const kmb::ElementContainer* body );
	bool appendCoboundary( const kmb::DataBindings* data, const kmb::MeshData* mesh );
	// �t�Ή��̍폜
	bool deleteCoboundary( kmb::Face face, const kmb::ElementContainer* body );

	// �ߖT���A���ӏ��̎擾
	
	// Face ��^���Ă��̋ߖT Face �����o���Aneighbors �ɕԂ�
	// neighbors �͌Ăяo�����ŋ��E�̌��������炩���ߗp�ӂ���
	// �Ăяo�����Ń������̊m�ہE������s������
	// �߂�l�͋ߖT�̌�
	// ���ӁFneighbors �ɓ����Ă���ߖT�̏��Ԃ͈Ӗ��͂Ȃ��i�r���� NULL �͏o�Ă��Ȃ��j
	int getFaceNeighbor( kmb::Face face, kmb::Face *neighbors, const kmb::ElementContainer* elements ) const;

	// Face ��^���Ă�����P�����Ⴂ�v�f�ƍl��������
	// ���E�ԍ� index ���ɐڂ��Ă��� Face ��Ԃ�
	// �߂�l�� false �̂Ƃ��ɂ� neighborFace �͕ύX���Ă��Ȃ����Ƃ�ۏ؂���
	bool getFaceNeighborByIndex( kmb::Face face, int index, const kmb::ElementContainer* elements, kmb::Face &neighborFace ) const;

	// �ߓ_��^���Ă��̎��ӗv�f�̌�
	size_t getFaceCountAroundNode(nodeIdType nodeID) const;
	// ���ׂĂ̋ߖT���̌�
	size_t getSize(void) const;

	// �ߓ_��^���Ă��̋ߖT�v�f�� Iterator ��^����
	NodeNeighborFace::iterator beginIteratorAt(kmb::nodeIdType nodeID){
		return coboundaries.lower_bound(nodeID);
	};
	NodeNeighborFace::iterator endIteratorAt(kmb::nodeIdType nodeID){
		return coboundaries.upper_bound(nodeID);
	};

private:
	NodeNeighborFace	coboundaries;
};

}
