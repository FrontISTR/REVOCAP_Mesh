/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : BodyOperation                                           #
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

namespace kmb{

class Point3DContainer;
class ElementContainer;
class Plane;

class BodyOperation
{
public:
	BodyOperation(const Point3DContainer* nodes);
	virtual ~BodyOperation(void);
	// 2ŸŒ³—v‘fƒOƒ‹[ƒv body ‚ÉŠÜ‚Ü‚ê‚é—v‘f‚Ì•Ó‚ğ
	// SEGMENT —v‘f‚Æ‚µ‚Ä result ‚É“o˜^‚·‚é
	// result == NULL ‚¾‚ÆŒÂ”‚¾‚¯•Ô‚·
	size_t getEdgesOfBody(const kmb::ElementContainer* body,kmb::ElementContainer* result=NULL) const;
	// body ‚ÉŠÜ‚Ü‚ê‚é—v‘f‚Ì•Ó‚Ì‚¤‚¿Aplane ‚ÆŒğ·‚·‚é‚à‚Ì‚ğ
	// SEGMENT —v‘f‚Æ‚µ‚Ä result ‚É“o˜^‚·‚é
	// result == NULL ‚¾‚ÆŒÂ”‚¾‚¯•Ô‚·
	size_t getEdgesIntersectPlane(const kmb::ElementContainer* body, const kmb::Plane* plane,kmb::ElementContainer* result=NULL) const;
private:
	const Point3DContainer* points;
};

}
