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


	int layerNum;
	double* layerThick;
	MeshData* mesh;
public:
	BLMGenerator(void);
	BLMGenerator(MeshData* mesh);
	BLMGenerator(MeshData* mesh,int layerNum,double* layerThick);
	virtual ~BLMGenerator(void);













	kmb::bodyIdType extrude(kmb::bodyIdType &boundaryId);
	kmb::bodyIdType intrude(kmb::bodyIdType parentId,kmb::bodyIdType &boundaryId);






	kmb::bodyIdType extrudeFromData(const char* faceGroup);
	kmb::bodyIdType intrudeFromData(const char* faceGroup);





	bool generateInnerFromData(const char* faceGroup,kmb::bodyIdType &layerId);





	bool generateInner(kmb::bodyIdType bodyId,kmb::bodyIdType parentId,kmb::bodyIdType &layerId);






	void setLayer(int layerNum,double* thicks);

	void setMesh(MeshData* mesh);
protected:















	kmb::nodeIdType getLayerNodeId(kmb::nodeIdType outerNodeId, kmb::nodeIdType innerNodeId, int layerIndex ) const;






	kmb::Face appendLayerElements( kmb::elementType etype, kmb::nodeIdType* oldNodeId, bool outer=true);
private:
	mutable std::map< std::pair<kmb::nodeIdType,kmb::nodeIdType>, kmb::nodeIdType* > layerNodes;
	void clearLayerNodes(void);

	std::map< kmb::nodeIdType, kmb::nodeIdType > nodeMapper;
	kmb::nodeIdType getDuplicatedNodeId(kmb::nodeIdType nodeId);
};

}
