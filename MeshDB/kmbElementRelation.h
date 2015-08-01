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
	// óvëfä‘ÇÃä÷åW
	enum relationType{
		UNKNOWNRELATION = -1,// ò_óùìIÇ…Ç†ÇËÇ¶Ç»Ç¢ä÷åW
		// ìØÇ∂éüå≥ÇÃóvëfÇ…Ç¬Ç¢ÇƒÇÃä÷åW
		EQUAL,			// å¸Ç´Ç‡ä‹ÇﬂÇƒìôÇµÇ¢
		REVERSE,		// ãtå¸Ç´Ç≈ìôÇµÇ¢
		ADJACENT,		// ê⁄Ç∑ÇÈ
		ANTIADJACENT,	// ãtå¸Ç´Ç≈ê⁄Ç∑ÇÈ
		// ëäéËÇ™é©ï™ÇÊÇËÇ‡éüå≥Ç™ÇPçÇÇ¢èÍçá
		BOUNDARY,		// ã´äEÇ…Ç»Ç¡ÇƒÇ¢ÇÈ
		ANTIBOUNDARY,	// ãtå¸Ç´ÇÃã´äEÇ…Ç»Ç¡ÇƒÇ¢ÇÈ
		// ëäéËÇ™é©ï™ÇÊÇËÇ‡éüå≥Ç™ÇPí·Ç¢èÍçá
		COBOUNDARY,		// ã´äEÇ∆ÇµÇƒä‹Çﬁ
		ANTICOBOUNDARY, // ãtå¸Ç´ÇÃã´äEÇ∆ÇµÇƒä‹Çﬁ
		// è„ãLÇÃÇ¢Ç∏ÇÍÇ≈Ç‡Ç»Ç¢Ç™ÅAã§óLÇµÇƒÇ¢ÇÈÇ‡ÇÃÇ™Ç†ÇÈÇ∆Ç´
		COMMONEDGE,		// ã§óLï”Ç†ÇË
		COMMONNODE,		// ã§óLí∏ì_Ç†ÇË
		NOCOMMON,		// ã§óLÇ»Çµ
		OTHERRELATION   // ÇªÇÃëºÇÃä÷åW
	};
	// óvëfä‘ÇÃä÷åWÇï∂éöóÒÇ…ïœä∑
	static std::string	getRelationTypeString(kmb::ElementRelation::relationType rel);
	
	// óvëfä‘ÇÃä÷åWÇãÅÇﬂÇÈ
	// ê⁄ÇµÇƒÇ¢ÇÈéûÇÕÇ«ÇÃñ Ç≈ê⁄ÇµÇƒÇ¢ÇÈÇ©Ç index0 index1 ÇÃílÇ≈ï‘Ç∑
	static kmb::ElementRelation::relationType
		getRelation(const kmb::ElementBase &eIter0, int &index0,
					const kmb::ElementBase &eIter1, int &index1 );

	// ê¸ï™Ç™àÍívÇµÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
	static kmb::ElementRelation::relationType
		getSegmentRelation
			(int a0,int a1,int b0,int b1,int &aIndex,int &bIndex);
	// éOäpå`Ç™àÍívÇµÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
	static kmb::ElementRelation::relationType
		getTriangleRelation
			(int a0,int a1,int a2,int b0,int b1,int b2,int &aIndex,int &bIndex);
	// éläpå`Ç™àÍívÇµÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
	static kmb::ElementRelation::relationType
		getQuadRelation
			(int a0,int a1,int a2,int a3,int b0,int b1,int b2,int b3,int &aIndex,int &bIndex);
	// éOäpå`Ç∆éläpå`ÇÃä‘ÇÃä÷åW
	static kmb::ElementRelation::relationType
		getTriangleQuadRelation
			(int a0,int a1,int a2,int b0,int b1,int b2,int b3,int &aIndex,int &bIndex);
	// élñ ëÃÇ™àÍívÇµÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
	static kmb::ElementRelation::relationType
		getTetrahedronRelation
			(int a0,int a1,int a2,int a3,int b0,int b1,int b2,int b3,int &aIndex,int &bIndex);
	// éOäpå`Ç∆élñ ëÃÇÃä÷åW
	// ÇPì_ã§óLÇÃéû => aIndex bIndex ÇÕã§óLÇ∑ÇÈêﬂì_ÇÃ index
	// ÇQì_ã§óLÇÃéû => aIndex ÇÕ Edge ÇÃ index ÅAbIndex ÇÕ Face ÇÃ index
	static kmb::ElementRelation::relationType
		getTriangleTetrahedronRelation
			(int a0,int a1,int a2,int b0,int b1,int b2,int b3,int &aIndex,int &bIndex);

	// Relation Ç∑Ç◊ÇƒÇãÅÇﬂÇ∏Ç…ê⁄ÇµÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©ÇÃÇ›ÇîªíËÇ∑ÇÈä»à’î≈
	static bool isAdjacent(const kmb::ElementBase &eIter0, int &index0, const kmb::ElementBase &eIter1, int &index1 );

	// ï°êª
	static bool clone( const kmb::ElementBase &eIter, kmb::nodeIdType *clonedCell );
	static bool clone( const kmb::ElementBase &eIter, kmb::nodeIdType *clonedCell, std::map< kmb::nodeIdType, kmb::nodeIdType >& nodeMapper );
	// å¸Ç´ÇãtÇ…Ç∑ÇÈ
	static bool reverse( const kmb::ElementBase &eIter, kmb::nodeIdType *reversedCell );
	static bool reverse( const kmb::ElementBase &eIter, kmb::nodeIdType *reversedCell, std::map< kmb::nodeIdType, kmb::nodeIdType >& nodeMapper );
	// ã§í óvëf
	static kmb::elementType common( const kmb::ElementBase &eIter0, const kmb::ElementBase &eIter1, kmb::nodeIdType *commonCell );
	static kmb::elementType common( kmb::Face f0, kmb::Face f1, const kmb::ElementContainer* elements, kmb::nodeIdType *commonCell );

protected:
	// ì‡ïîâ∫êøÇØ
	// ëÊÇPà¯êîÇÃå^Ç≈ï™óﬁ
	static kmb::ElementRelation::relationType getRelationForSegment
		(const kmb::ElementBase &eIter0, int &index0, const kmb::ElementBase &eIter1, int &index1 );
	// common edge ÇÃéû index0 = triangle ÇÃ face id
	// common node ÇÃéû index0 = triangle ÇÃêﬂì_ÇÃ index
	static kmb::ElementRelation::relationType getRelationForTriangle
		(const kmb::ElementBase &eIter0, int &index0, const kmb::ElementBase &eIter1, int &index1 );
	static kmb::ElementRelation::relationType getRelationForQuad
		(const kmb::ElementBase &eIter0, int &index0, const kmb::ElementBase &eIter1, int &index1 );
	static kmb::ElementRelation::relationType getRelationForTetrahedron
		(const kmb::ElementBase &eIter0, int &index0, const kmb::ElementBase &eIter1, int &index1 );
};

}
