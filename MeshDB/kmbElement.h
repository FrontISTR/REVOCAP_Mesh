/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : Element                                                 #
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
/*
 * 1�����v�f connectionTable
 * 2�����v�f connectionTable faceTable
 * 3�����v�f connectionTable faceTable edgeTable
 *
 * 2���v�f�ɂ��ĖʁA�ӂ�1�����v�f�� SEGMENT2
 * 2�����v�f�� faceTable �̑�3������ index ���Ƀ\�[�g����Ă���
 * 3�����v�f�� edgeTable �̑�3������ index ���Ƀ\�[�g����Ă���
 *
 */

#pragma once

#include <map>
#include <string>

#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElementType.h"

namespace kmb{

class MeshData;
class Tetrahedron;

// �v�f�Ɋւ��鏈���ŁAnode table �݂̂ŏ����ł�����̂ɂ��Ă� Base �N���X�Ƀ��\�b�h��ǉ�����
// Point3D �������� Point2D �Ɉˑ�������̂ɂ��Ă� ElementEvaluator �Ɏ�������

class ElementBase
{
public:
	ElementBase(void){}
	virtual ~ElementBase(void){}
	// static ��
	// size of node table array
	static int getNodeCount(kmb::elementType type);
	// size of vertex ( which equals node count in case an element is 1st degree )
	// vertex count of 2nd element = vertex count of 1st element = node count of 1st element
	static int getVertexCount(kmb::elementType type);
	static int getBoundaryCount(kmb::elementType type);
	static int getEdgeCount(kmb::elementType type);
	static kmb::elementType getBoundaryType(kmb::elementType type,int index);
	static int getBoundaryNodeCount(kmb::elementType type,int index);
	static int getBoundaryVertexCount(kmb::elementType type,int index);
	static kmb::elementType getEdgeType(kmb::elementType type,int index);
	static int getEdgeNodeCount(kmb::elementType type,int index);
	static int getEdgeVertexCount(kmb::elementType type,int index);
	static int getDimension(kmb::elementType type);
	static int getDegree(kmb::elementType type);
	static const std::string getTypeString(kmb::elementType type);

	// remark :call this method by kmb::ElementBase::getType
	static kmb::elementType getType(std::string str);
	static kmb::elementType getLinearType(kmb::elementType type);
	static kmb::elementType getSecondType(kmb::elementType type);
	static int getDiagIndex(kmb::elementType type,int index);
	// basic virtual methods
	virtual kmb::elementType getType(void) const = 0;
	virtual kmb::nodeIdType getCellId(int cellIndex) const = 0;
	virtual bool setCellId(int cellIndex, kmb::nodeIdType nodeId) = 0;
	virtual kmb::nodeIdType operator[](const int i) const = 0;

	// instance ��
	int getNodeCount(void) const;
	int getVertexCount(void) const;
	int getBoundaryCount(void) const;
	int getEdgeCount(void) const;
	kmb::elementType getBoundaryType(int index) const;
	kmb::elementType getEdgeType(int index) const;
	int getBoundaryNodeCount(int index) const;
	int getBoundaryVertexCount(int index) const;
	int getEdgeNodeCount(int index) const;
	int getEdgeVertexCount(int index) const;
	kmb::nodeIdType getBoundaryCellId(int index,int i) const;
	kmb::nodeIdType getEdgeCellId(int index,int i) const;
	int getDimension(void) const;
	int getDegree(void) const;
	const std::string getTypeString(void) const;
	kmb::elementType getLinearType(void) const;
	kmb::elementType getSecondType(void) const;
	int getDiagIndex(int index) const;
	// �֗��֐��n
	bool include(kmb::nodeIdType nodeId) const;
	bool includeVertex(kmb::nodeIdType nodeId) const;
	// �܂܂�Ȃ������� -1 ��Ԃ�
	int indexOf(kmb::nodeIdType nodeId) const;
	int countCommonNode(kmb::ElementBase& other) const;
	bool replaceNodeId(kmb::nodeIdType oldNodeId,kmb::nodeIdType newNodeId);
	int replaceNodeId(std::map<nodeIdType,nodeIdType> &idmap);
	// �ʗv�f�A�ӗv�f�̐���
	kmb::elementType getBoundaryElement(int index,kmb::nodeIdType* cell) const;
	bool getBoundaryElement(int index,kmb::ElementBase& elem) const;
	kmb::elementType getEdgeElement(int index,kmb::nodeIdType* cell) const;
	bool getEdgeElement(int index,kmb::ElementBase& elem) const;
	// index0 �� index1 ���ӂłȂ����Ă��邩�ǂ��� 
	// 1/-1 ��1���v�f�Ƃ݂����ɂȂ����Ă��邩
	// �����͕��ʗv�f�̂Ƃ��݈̂Ӗ�������A���̌��������̌�����������
	// 2 2���v�f�Ƃ݂����ɂȂ����Ă���
	// 0 �Ȃ����Ă��Ȃ�
	int isConnected(int index0,int index1) const;
	//
	// �ʂ��\������ߓ_�̓Y������^���āA���Ԗڂ̖ʂ��𔻒肷��
	// 1 �������̖�
	// -1 �t�����̖�
	// 0 ����ȊO
	//
	int isFace(int index0,int index1,int &faceIndex) const;
	int isFace(int index0,int index1,int index2,int &faceIndex) const;
	int isFace(int index0,int index1,int index2,int index3,int &faceIndex) const;
	int isFace(const kmb::ElementBase &element, int &faceIndex) const;
	// 2�_���猈�܂�ʁA�ӂ���肷��
	int getFaceIndex(int index0,int index1) const;
	// ���Ԃ�
	bool reverse(void);
	static bool reverse( kmb::elementType etype, kmb::nodeIdType* nodes);
	// �R�p�`����
	int divideIntoTriangles(kmb::nodeIdType triangles[][3]) const;
	// �S�ʑ̕���
	int divideIntoTetrahedrons(kmb::nodeIdType tetrahedrons[][4]) const;
	// �ߓ_�ԍ����ŏ��̒��_�i���Ԑߓ_�͊܂߂Ȃ��j�𒲂ׂāA���̐ߓ_�z��̓Y�����ԍ������߂�
	// index �Ԗڂ� Face �Őߓ_�ԍ����ŏ��̐ߓ_�Y�����ԍ������߂�
	int getIndexMinNodeIdOfFace(int index) const;
	int getIndexMinNodeId(void) const;
	// ���ʗv�f�ɂ��āB���Ԑߓ_�̏ꍇ�� nullNodeId ��Ԃ�
	kmb::nodeIdType nextNode(kmb::nodeIdType nodeId) const;
	kmb::nodeIdType previousNode(kmb::nodeIdType nodeId) const;
};

class Element : public ElementBase
{
public:
	// element id of null element
	static kmb::elementIdType nullElementId;
	// maximum size of node table array of elements
	// MAX_NODE_COUNT = sizeof( Hexahedron2.cell ) = 20
	// MAX_NODE_COUNT_DIM2 = sizeof( Quad2.cell ) = 8
	static const int MAX_NODE_COUNT;
	static const int MAX_NODE_COUNT_DIM2;

	Element(kmb::elementType t = UNKNOWNTYPE);
	virtual ~Element(void);

	// clone
	Element* clone() const;
	Element* reverseClone() const;
	// clone with replacing node ids
	Element* clone( std::map< kmb::nodeIdType, kmb::nodeIdType >& nodeMapper ) const;
	Element* reverseClone( std::map< kmb::nodeIdType, kmb::nodeIdType >& nodeMapper ) const;
	// �������\�b�h
	// ary �͔z��łȂ� new �Ŋm�ۂ��ꂽ�|�C���^�������邱��
	// ���̃|�C���^��ێ�����̂ŁA�Ăяo�����ł� delete ���Ȃ����ƁI
	// ary = NULL �̏ꍇ�͂��̃��\�b�h�̒��� new ����B
	static Element* create(kmb::elementType type,kmb::nodeIdType *ary=NULL);
	// get element type
	virtual kmb::elementType getType() const;
public:
	// set/get node table
	virtual bool setCellId(int index,kmb::nodeIdType nodeId);
	virtual kmb::nodeIdType getCellId(int index) const;
	virtual kmb::nodeIdType operator[](const int i) const;
	virtual kmb::nodeIdType& operator[](const int i);
protected:
	elementType type;
	nodeIdType *cell;
};


}
