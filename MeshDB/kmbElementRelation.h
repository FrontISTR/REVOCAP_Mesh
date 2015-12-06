/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : ElementRelation                                         #
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

#include "MeshDB/kmbElementContainer.h"
#include "MeshDB/kmbElement.h"

#include <map>

namespace kmb{

class Face;

class ElementRelation
{
public:
	// �v�f�Ԃ̊֌W
	enum relationType{
		UNKNOWNRELATION = -1,// �_���I�ɂ��肦�Ȃ��֌W
		// ���������̗v�f�ɂ��Ă̊֌W
		EQUAL,			// �������܂߂ē�����
		REVERSE,		// �t�����œ�����
		ADJACENT,		// �ڂ���
		ANTIADJACENT,	// �t�����Őڂ���
		// ���肪���������������P�����ꍇ
		BOUNDARY,		// ���E�ɂȂ��Ă���
		ANTIBOUNDARY,	// �t�����̋��E�ɂȂ��Ă���
		// ���肪���������������P�Ⴂ�ꍇ
		COBOUNDARY,		// ���E�Ƃ��Ċ܂�
		ANTICOBOUNDARY, // �t�����̋��E�Ƃ��Ċ܂�
		// ��L�̂�����ł��Ȃ����A���L���Ă�����̂�����Ƃ�
		COMMONEDGE,		// ���L�ӂ���
		COMMONNODE,		// ���L���_����
		NOCOMMON,		// ���L�Ȃ�
		OTHERRELATION   // ���̑��̊֌W
	};
	// �v�f�Ԃ̊֌W�𕶎���ɕϊ�
	static std::string	getRelationTypeString(kmb::ElementRelation::relationType rel);
	
	// �v�f�Ԃ̊֌W�����߂�
	// �ڂ��Ă��鎞�͂ǂ̖ʂŐڂ��Ă��邩�� index0 index1 �̒l�ŕԂ�
	static kmb::ElementRelation::relationType
		getRelation(const kmb::ElementBase &eIter0, int &index0,
					const kmb::ElementBase &eIter1, int &index1 );

	// ��������v���Ă��邩�ǂ���
	static kmb::ElementRelation::relationType
		getSegmentRelation
			(int a0,int a1,int b0,int b1,int &aIndex,int &bIndex);
	// �O�p�`����v���Ă��邩�ǂ���
	static kmb::ElementRelation::relationType
		getTriangleRelation
			(int a0,int a1,int a2,int b0,int b1,int b2,int &aIndex,int &bIndex);
	// �l�p�`����v���Ă��邩�ǂ���
	static kmb::ElementRelation::relationType
		getQuadRelation
			(int a0,int a1,int a2,int a3,int b0,int b1,int b2,int b3,int &aIndex,int &bIndex);
	// �O�p�`�Ǝl�p�`�̊Ԃ̊֌W
	static kmb::ElementRelation::relationType
		getTriangleQuadRelation
			(int a0,int a1,int a2,int b0,int b1,int b2,int b3,int &aIndex,int &bIndex);
	// �l�ʑ̂���v���Ă��邩�ǂ���
	static kmb::ElementRelation::relationType
		getTetrahedronRelation
			(int a0,int a1,int a2,int a3,int b0,int b1,int b2,int b3,int &aIndex,int &bIndex);
	// �O�p�`�Ǝl�ʑ̂̊֌W
	// �P�_���L�̎� => aIndex bIndex �͋��L����ߓ_�� index
	// �Q�_���L�̎� => aIndex �� Edge �� index �AbIndex �� Face �� index
	static kmb::ElementRelation::relationType
		getTriangleTetrahedronRelation
			(int a0,int a1,int a2,int b0,int b1,int b2,int b3,int &aIndex,int &bIndex);

	// Relation ���ׂĂ����߂��ɐڂ��Ă��邩�ǂ����݂̂𔻒肷��ȈՔ�
	static bool isAdjacent(const kmb::ElementBase &eIter0, int &index0, const kmb::ElementBase &eIter1, int &index1 );

	// ����
	static bool clone( const kmb::ElementBase &eIter, kmb::nodeIdType *clonedCell );
	static bool clone( const kmb::ElementBase &eIter, kmb::nodeIdType *clonedCell, std::map< kmb::nodeIdType, kmb::nodeIdType >& nodeMapper );
	// �������t�ɂ���
	static bool reverse( const kmb::ElementBase &eIter, kmb::nodeIdType *reversedCell );
	static bool reverse( const kmb::ElementBase &eIter, kmb::nodeIdType *reversedCell, std::map< kmb::nodeIdType, kmb::nodeIdType >& nodeMapper );
	// ���ʗv�f
	static kmb::elementType common( const kmb::ElementBase &eIter0, const kmb::ElementBase &eIter1, kmb::nodeIdType *commonCell );
	static kmb::elementType common( kmb::Face f0, kmb::Face f1, const kmb::ElementContainer* elements, kmb::nodeIdType *commonCell );

protected:
	// ����������
	// ��P�����̌^�ŕ���
	static kmb::ElementRelation::relationType getRelationForSegment
		(const kmb::ElementBase &eIter0, int &index0, const kmb::ElementBase &eIter1, int &index1 );
	// common edge �̎� index0 = triangle �� face id
	// common node �̎� index0 = triangle �̐ߓ_�� index
	static kmb::ElementRelation::relationType getRelationForTriangle
		(const kmb::ElementBase &eIter0, int &index0, const kmb::ElementBase &eIter1, int &index1 );
	static kmb::ElementRelation::relationType getRelationForQuad
		(const kmb::ElementBase &eIter0, int &index0, const kmb::ElementBase &eIter1, int &index1 );
	static kmb::ElementRelation::relationType getRelationForTetrahedron
		(const kmb::ElementBase &eIter0, int &index0, const kmb::ElementBase &eIter1, int &index1 );
};

}
