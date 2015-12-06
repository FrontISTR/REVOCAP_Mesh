/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : NodeNeighborInfo                                        #
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
#include <map>
#include <vector>
#include <string>

namespace kmb{

class Element;
class ElementBase;
class MeshData;
class DataBindings;
class ElementContainer;
class Face;

typedef std::multimap< kmb::nodeIdType, kmb::elementIdType > NodeNeighbor;

class NodeNeighborInfo
{
public:
	NodeNeighborInfo(void);
	virtual ~NodeNeighborInfo(void);

	void clear(void);
	bool getIgnoreOrientation(void) const;
	void setIgnoreOrientation(bool f);

	// elementId vs nodeId table 
private:
	bool append( kmb::nodeIdType nodeID, kmb::elementIdType elementID );
	bool erase( kmb::nodeIdType nodeID, kmb::elementIdType elementID );
public:
	bool appendCoboundary( kmb::elementIdType elementId, const kmb::ElementBase &element );
	bool appendCoboundary( const kmb::ElementContainer* elements );
	bool appendCoboundary( const kmb::MeshData* mesh );
	bool appendCoboundary( const kmb::DataBindings* data, const kmb::ElementContainer* body );
	bool appendCoboundary( const kmb::DataBindings* data, const kmb::MeshData* mesh );
	// erase correspondence
	bool deleteCoboundary( kmb::elementIdType elementId, const kmb::ElementBase &element );

	// �ߖT���A���ӏ��̎擾
	
	// �v�f��^���Ă��̋ߖT�v�f�����o���Aneighbors �ɕԂ�
	// neighbors �͌Ăяo������ element �̋��E�̌��������炩���ߗp�ӂ���
	// �Ăяo�����Ń������̊m�ہE������s������
	// �߂�l�͋ߖT�v�f�̌�
	// ���ӁFneighbors �ɓ����Ă���ߖT�̏��Ԃ͈Ӗ��͂Ȃ��i�r���� NULL �͏o�Ă��Ȃ��j
	int getNeighborElements( kmb::elementIdType elementID, kmb::elementIdType *neighbors, const kmb::ElementContainer* elements ) const;
	int getNeighborElements( kmb::elementIdType elementID, kmb::elementIdType *neighbors, kmb::MeshData* mesh ) const;
	
	// �v�f��^���āA���̋��E�v�f���A�o�^����Ă��� neighborInfo ����T��
	// ������������Ԃ�
	int getElementBoundary( const kmb::ElementBase &element, kmb::elementIdType *boundaries, const kmb::ElementContainer* elements ) const;
	// ���ʗv�f��o�^���Ă��鎞�ɗ��̗v�f element �Ɩʔԍ���^���ċ��E�v�f��T��
	// element �͕K������ elements �Ɋ܂܂�Ă��Ȃ��Ă��悢
	kmb::elementIdType getElementFace( const kmb::ElementBase &element, kmb::idType faceId, const kmb::ElementContainer* elements ) const;
	// ���̗v�f��o�^���Ă��鎞�ɕ��ʗv�f��^���āAelement �Ɩʔԍ���Ԃ�
	// element : ���ʗv�f
	// face : element ��ʂɂ��悤�ȗ��̗v�f�� Face 
	bool getFace( const kmb::ElementBase &element, kmb::Face &face, const kmb::ElementContainer* elements ) const;
	// face ��^���āA����ɐڂ��Ă��� Face adj �����߂�
	bool getAdjacentFace( const kmb::Face &face, kmb::Face &adj, const kmb::ElementContainer* elements ) const;
	bool getAdjacentFace( const kmb::Face &face, kmb::Face &adj, kmb::MeshData* mesh ) const;
public:
	// get adjacent nodes from a node
	void getNodeNeighbor( kmb::nodeIdType nodeId, std::set<kmb::nodeIdType> &neighbors, const kmb::ElementContainer* elements );
	void getNodeNeighbor( kmb::nodeIdType nodeId, std::set<kmb::nodeIdType> &neighbors, kmb::MeshData* mesh );
	// if only 2D elements are appended,
	// get sorted adjacent nodes from a node
	void getCrownNodeNeighbor
		( kmb::nodeIdType nodeID,
			std::vector<kmb::nodeIdType> &neighbors,
			const kmb::ElementContainer* elements );
	// is connected between two nodes
	bool isConnected( kmb::nodeIdType nodeId0, kmb::nodeIdType nodeId1, const kmb::ElementContainer* elements ) const;
	bool isConnected( kmb::nodeIdType nodeId0, kmb::nodeIdType nodeId1, kmb::MeshData* mesh ) const;

	// �ߓ_��^���Ă��̎��ӗv�f�̌�
	size_t getElementCountAroundNode(nodeIdType nodeId) const;
	// ���ׂĂ̋ߖT���̌�
	size_t getSize(void) const;

	// �ߓ_��^���Ă��̋ߖT�v�f�� Iterator ��^����
	NodeNeighbor::iterator beginIteratorAt(kmb::nodeIdType nodeID){
		return coboundaries.lower_bound(nodeID);
	};
	NodeNeighbor::iterator endIteratorAt(kmb::nodeIdType nodeID){
		return coboundaries.upper_bound(nodeID);
	};
	NodeNeighbor::const_iterator beginIteratorAt(kmb::nodeIdType nodeID) const{
		return coboundaries.lower_bound(nodeID);
	};
	NodeNeighbor::const_iterator endIteratorAt(kmb::nodeIdType nodeID) const{
		return coboundaries.upper_bound(nodeID);
	};

	// �ߓ_��^���Ă��̎��ӗv�f�̏W����Ԃ�
	void getSurroundingElements(kmb::nodeIdType nodeId, std::vector<kmb::elementIdType> &elements) const;
	// given two nodes, return elements which have both nodes
	void getSurroundingElementsOf2Node(kmb::nodeIdType nodeId0, kmb::nodeIdType nodeId1, std::vector<kmb::elementIdType> &elements) const;

	// �ߓ_��^���Ĕ����v���ɐڂ��Ă��镽�ʗv�f��Ԃ�
	// ���ʗv�f�̏ꍇ�̂�
	kmb::elementIdType getNextElementAroundNode( kmb::ElementContainer* triangles, kmb::nodeIdType nodeID, kmb::elementIdType elemID, bool clockwise=false) const;

	// �����v�f�̏ꍇ�̂�
	// �[�_�����߂�
	void getEndsOfEdge( const kmb::ElementContainer* edges, kmb::nodeIdType& firstID, kmb::nodeIdType& endID ) const;

	NodeNeighbor::iterator beginNodeIterator(void);
	NodeNeighbor::iterator endNodeIterator(void);
private:
	NodeNeighbor coboundaries;
	bool admitAnti;
};

}
