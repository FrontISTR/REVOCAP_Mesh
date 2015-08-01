/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MeshDB                                                  #
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
// ÉÅÉbÉVÉÖÇÃà ëäÇ…ä÷Ç∑ÇÈèàóù

#include "MeshDB/kmbMeshDB.h"
#include "MeshDB/kmbElementContainer.h"
#include "MeshDB/kmbTetrahedron.h"
#include "MeshDB/kmbMeshOperation.h"
#include "MeshDB/kmbMatching.h"
#include "MeshDB/kmbPolygonPartitioner.h"

#include "Geometry/kmbFramedPlane.h"

//------------------- ãﬂñTèÓïÒéÊìæ ------------------------

void
kmb::MeshDB::getSurroundingElements(nodeIdType nodeId,std::vector<elementIdType>& elements,bool cacheOnly) const
{
	if( this->neighborInfo.getSize() > 0 ){
		this->neighborInfo.getSurroundingElements(nodeId,elements);
	}else if(cacheOnly == false){
		const kmb::bodyIdType bodyCount = this->getBodyCount();
		for( kmb::bodyIdType id = 0; id < bodyCount; ++id){
			const kmb::ElementContainer* body = this->getBodyPtr(id);
			if( body != NULL ){
				kmb::ElementContainer::const_iterator eIter = body->begin();
				while( eIter != body->end() )
				{
					if( eIter.include( nodeId ) )
					{
						elements.push_back( eIter.getId() );
					}
					++eIter;
				}
			}
		}
	}
}


//------------------- éOäpå`ï™äÑ --------------------------

void
kmb::MeshDB::triangulation(kmb::bodyIdType bodyId)
{
	if( meshOperation == NULL ){
		meshOperation = new kmb::MeshOperation(this);
	}
	this->meshOperation->triangulation(bodyId);
}

//------------------- ëΩäpå`ï™äÑ --------------------------//

kmb::bodyIdType
kmb::MeshDB::polygonPartition(bodyIdType polygonID,kmb::FramedPlane &plane)
{
	kmb::bodyIdType bodyID = kmb::Body::nullBodyId;

	kmb::Body* body = this->getBodyPtr( polygonID );
	if( body && body->isUniqueDim(1) ){
		// polygon ÇÃì_Ç2éüå≥éÀâeÇµÇƒ points Ç…äiî[Ç∑ÇÈ
		kmb::Point2DContainerMap points;
		std::set< kmb::nodeIdType > nodeSet;
		body->getNodesOfBody( nodeSet );
		std::set< kmb::nodeIdType >::iterator nIter = nodeSet.begin();
		kmb::Point3D point;
		while( nIter != nodeSet.end() )
		{
			kmb::nodeIdType nodeId = (*nIter);
			++nIter;
			if( this->getNode( nodeId, point ) ){
				kmb::Point2D uv = plane.transformTo2D( point );
				points.addPoint( uv, nodeId );
			}
		}
		kmb::PolygonPartitioner partitioner;
		partitioner.setPoints( &points );
		partitioner.setInitialPolygon( body );
		kmb::ElementContainerMap* triangles = NULL;
		triangles = new kmb::ElementContainerMap;
		partitioner.partition( *triangles );

		if( triangles->getCount() > 0 )
		{
			bodyID = this->appendBody( triangles );
		}else{
			delete triangles;
		}
	}

	return bodyID;
}
