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


	size_t nodeSplitByVolumes(void);



	int duplicateNodeIdOfBody(kmb::bodyIdType bodyId,const char* coupleName=NULL);


	size_t uniteNodes(double thresh);

	int triangulation(kmb::bodyIdType bodyId);



	static bool isParent(const kmb::ElementContainer* child,const kmb::ElementContainer* parent,const kmb::NodeNeighborInfo &neighbors);

	static int surface2FaceGroup(const kmb::ElementContainer* child,const kmb::ElementContainer* parent,const kmb::NodeNeighborInfo &neighborInfo,kmb::DataBindings* faceGroup);
protected:

	static kmb::elementIdType getParentElement(const kmb::ElementBase &elem,const kmb::ElementContainer* parent,const kmb::NodeNeighborInfo &neighbors);
};

}
