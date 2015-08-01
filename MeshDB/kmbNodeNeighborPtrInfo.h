/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : NodeNeighborPtrInfo                                     #
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
 * kmb::NodeNeighborInfo �Ɠ����̋@�\
 * �������AelementId �Ƃ̑Ή��ł͂Ȃ��āAkmb::Element* �Ƃ̑Ή���ۑ�����
 */

#pragma once

#include "MeshDB/kmbTypes.h"
#include <map>
#include <cstddef>

namespace kmb{

class Element;
class ElementContainer;
class MeshData;

// Delaunay �� Advancing Front �̏����ߖT���̐����ɗp���邽��
// �ۑ�����|�C���^�� const �ł͂Ȃ�

typedef std::multimap< kmb::nodeIdType, kmb::Element* > NodeNeighborPtr;

class NodeNeighborPtrInfo
{
public:
	NodeNeighborPtrInfo(void);
	virtual ~NodeNeighborPtrInfo(void);

	void clear(void);
	bool getIgnoreOrientation(void) const;
	void setIgnoreOrientation(bool f);
private:
	// nodeId vs Element* table
	bool append( kmb::nodeIdType nodeId, kmb::Element* element );
	bool erase( kmb::nodeIdType nodeId, kmb::Element* element );
public:
	bool appendCoboundary( kmb::Element* element );
	bool appendCoboundary( kmb::ElementContainer* elements );
	bool appendCoboundary( kmb::MeshData* mesh );
	// erase correspondence
	bool deleteCoboundary( kmb::Element* element );

	// �ߖT���A���ӏ��̎擾
	
	// �v�f��^���Ă��̋ߖT�v�f�����o���Aneighbors �ɕԂ�
	// neighbors �͌Ăяo������ element �̋��E�̌��������炩���ߗp�ӂ���
	// �Ăяo�����Ń������̊m�ہE������s������
	// �߂�l�͋ߖT�v�f�̌�
	int getElementNeighbor( const kmb::Element* element, kmb::Element** neighbors ) const;
	int getElementNeighborFace( const kmb::Element* element, kmb::Element** neighbors, int *faces ) const;
	// get adjacent nodes from a node
	void getNodeNeighbor( kmb::nodeIdType nodeId, std::set<kmb::nodeIdType> &neighbors ) const;
	// is connected between two nodes
	bool isConnected( kmb::nodeIdType nodeId0, kmb::nodeIdType nodeId1 ) const;
	// get the number of elements around the given node
	size_t getElementCountAroundNode(kmb::nodeIdType nodeId) const;
	// get the number of correspondence
	size_t getSize(void) const;

	// get the iterator of elements around the given node
	NodeNeighborPtr::iterator beginIteratorAt(kmb::nodeIdType nodeId){
		return coboundaries.lower_bound(nodeId);
	};
	NodeNeighborPtr::iterator endIteratorAt(kmb::nodeIdType nodeId){
		return coboundaries.upper_bound(nodeId);
	};

	NodeNeighborPtr::iterator beginNodeIterator(void);
	NodeNeighborPtr::iterator nextNodeIterator(kmb::nodeIdType nodeId);
	NodeNeighborPtr::iterator endNodeIterator(void);
private:
	NodeNeighborPtr	coboundaries;
	bool admitAnti;
};

}
