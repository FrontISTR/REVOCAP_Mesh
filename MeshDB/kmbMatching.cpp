/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : Matching                                                #
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
#include <cmath>
#include <set>
#include "MeshDB/kmbMatching.h"
#include "MeshDB/kmbMeshDB.h"
#include "MeshDB/kmbElementContainer.h"
#include "MeshDB/kmbElementRelation.h"
#include "MeshDB/kmbNodeNeighborInfo.h"
#include "MeshDB/kmbNodeNeighborFaceInfo.h"
#include "MeshDB/kmbDataBindings.h"
#include "MeshDB/kmbTriangle.h"
#include "MeshDB/kmbPolygon.h"
#include "Geometry/kmbPoint3DContainer.h"
#include "Common/kmbCalculator.h"

kmb::Matching::Matching(void)
{
}

kmb::Matching::~Matching(void)
{
}

double
kmb::Matching::getDistanceEdgeToNode( kmb::MeshDB* mesh, kmb::bodyIdType bodyId, kmb::nodeIdType nodeId ) const
{
	kmb::elementIdType nearestId = kmb::Element::nullElementId;
	double param = 0.0;
	kmb::Point3D point;
	if( mesh && mesh->getNode(nodeId,point) ){
		kmb::ElementContainer* body = mesh->getBodyPtr(bodyId);
		return getDistanceEdgeToNodeWithParam(mesh->getNodes(),body,&point,nearestId,param);
	}else{
		// DBL_MAX
		return getDistanceEdgeToNodeWithParam(mesh->getNodes(),NULL,NULL,nearestId,param);
	}
}

double
kmb::Matching::getDistanceEdgeToEdge( kmb::MeshDB* mesh, kmb::bodyIdType edgeId0, kmb::bodyIdType edgeId1 ) const
{
	kmb::AverageCalculator ave0, ave1;
	kmb::ElementContainer* edges0 = NULL;
	kmb::ElementContainer* edges1 = NULL;
	if( mesh &&
		(edges0=mesh->getBodyPtr(edgeId0)) != NULL &&
		edges0->isUniqueDim(1) &&
		(edges1=mesh->getBodyPtr(edgeId1)) != NULL &&
		edges1->isUniqueDim(1) )
	{
		// edge0 ÇÃêﬂì_Ç∆ edge1 ÇÃóvëfÇÃä‘ÇÃãóó£
		kmb::ElementContainer::iterator eIter0 = edges0->begin();
		while( eIter0 != edges0->end() )
		{
			// 1éüå≥óvëfÇÃénì_Ç∆ÇÃãóó£ÇÃïΩãœÇÇ∆ÇÈ
			ave0.add( getDistanceEdgeToNode( mesh, edgeId1 , eIter0.getCellId(0) ) );
			++eIter0;
		}
		// edge1 ÇÃêﬂì_Ç∆ edge0 ÇÃóvëfÇÃä‘ÇÃãóó£
		kmb::ElementContainer::iterator eIter1 = edges1->begin();
		while( eIter1 != edges1->end() )
		{
			// 1éüå≥óvëfÇÃénì_Ç∆ÇÃãóó£ÇÃïΩãœÇÇ∆ÇÈ
			ave1.add( getDistanceEdgeToNode( mesh, edgeId0, eIter1.getCellId(0) ) );
			++eIter1;
		}
	}
	return 0.5*(ave0.getAverage() + ave1.getAverage());
}

double
kmb::Matching::getDistanceEdgeToEdge( kmb::MeshDB* mesh0, kmb::bodyIdType edgeId0, kmb::MeshDB* mesh1, kmb::bodyIdType edgeId1 ) const
{
	kmb::AverageCalculator ave0, ave1;
	kmb::ElementContainer* edges0 = NULL;
	kmb::ElementContainer* edges1 = NULL;
	kmb::elementIdType nearestId = kmb::Element::nullElementId;
	double param = 0.0;
	kmb::Node node;

	if( mesh0 && mesh1 &&
		(edges0 = mesh0->getBodyPtr(edgeId0)) != NULL &&
		edges0->isUniqueDim(1) &&
		(edges1 = mesh1->getBodyPtr(edgeId1)) != NULL &&
		edges1->isUniqueDim(1) )
	{
		// edge0 ÇÃêﬂì_Ç∆ edge1 ÇÃóvëfÇÃä‘ÇÃãóó£
		kmb::ElementContainer::iterator eIter0 = edges0->begin();
		while( !eIter0.isFinished() )
		{
			if( mesh0->getNode(eIter0.getCellId(0),node) )
			{
				// 1éüå≥óvëfÇÃénì_Ç∆ÇÃãóó£ÇÃïΩãœÇÇ∆ÇÈ
				ave0.add( getDistanceEdgeToNodeWithParam( mesh1->getNodes(),edges1,&node,nearestId,param) );
			}
			++eIter0;
		}
		// edge1 ÇÃêﬂì_Ç∆ edge0 ÇÃóvëfÇÃä‘ÇÃãóó£
		kmb::ElementContainer::iterator eIter1 = edges1->begin();
		while( !eIter1.isFinished() )
		{
			if( mesh1->getNode(eIter1.getCellId(0),node) )
			{
				// 1éüå≥óvëfÇÃénì_Ç∆ÇÃãóó£ÇÃïΩãœÇÇ∆ÇÈ
				ave1.add( getDistanceEdgeToNodeWithParam( mesh0->getNodes(),edges0,&node,nearestId,param) );
			}
			++eIter1;
		}
	}
	return 0.5*(ave0.getAverage() + ave1.getAverage());
}

double
kmb::Matching::getDistanceDataToData(kmb::MeshDB* mesh,const char* key0,const char* key1)
{
	double dist = DBL_MAX;
	if( mesh )
	{
		kmb::DataBindings* data0 = mesh->getDataBindingsPtr( key0 );
		kmb::DataBindings* data1 = mesh->getDataBindingsPtr( key1 );
		if( data0 && data1 ){
			kmb::BoundingBox bbox0;
			kmb::BoundingBox bbox1;
			mesh->getBoundingBoxOfData( bbox0, data0 );
			mesh->getBoundingBoxOfData( bbox1, data1 );
			dist = sqrt( bbox0.distanceSq( bbox1 ) );
		}
	}
	return dist;
}

kmb::ElementRelation::relationType
kmb::Matching::getSurfaceRelation
( kmb::MeshDB* mesh, kmb::bodyIdType surfId0, kmb::bodyIdType surfId1 )
{
	kmb::ElementRelation::relationType rel = kmb::ElementRelation::UNKNOWNRELATION;
	kmb::ElementContainer* surf0 = NULL;
	kmb::ElementContainer* surf1 = NULL;
	if( mesh == NULL ||
		(surf0 = mesh->getBodyPtr(surfId0)) == NULL ||
		(surf1 = mesh->getBodyPtr(surfId1)) == NULL )
	{
		return rel;
	}

	// ïΩñ óvëfÇÃÇ›Ç»ÇÈÇ±Ç∆ÇÉ`ÉFÉbÉNÇ∑ÇÈ
	if( surf0 == NULL || !surf0->isUniqueDim(2) ||
		surf1 == NULL || !surf1->isUniqueDim(2) )
	{
		return rel;
	}

	const size_t len0 = surf0->getCount();
	const size_t len1 = surf1->getCount();

	if( len0 != len1 ){
		return rel;
	}

	unsigned int eqCount = 0;	// àÍívÇµÇƒÇ¢ÇÈå¬êî
	unsigned int revCount = 0;	// ãtå¸Ç´ÇÃå¬êî

	// ãﬂñTèÓïÒÇçÏÇËíºÇ∑
	// surf0 Ç©ÇÁãﬂñTèÓïÒÇçÏÇ¡Çƒ
	// surf1 ÇÃóvëfÇÃí∏ì_Çä‹Çﬁ surf0 ÇÃóvëfÇåüçıÇµÇƒ
	// àÍívÇ∑ÇÈ or ãtå¸Ç´ÇÃÇ‡ÇÃÇíTÇ∑
	kmb::NodeNeighborInfo neighborInfo;
	neighborInfo.appendCoboundary( surf0 );

	kmb::ElementContainer::iterator eIter = surf1->begin();
	while( eIter != surf1->end() )
	{
		bool findFlag = false;
		// ç≈èâÇÃí∏ì_ÇæÇØÇ≈å©ÇÍÇŒè\ï™
		kmb::nodeIdType nodeID = eIter.getCellId(0);
		// í∏ì_ÇÃé¸ï”óvëfÇ∆ÇÃä÷åWÇí≤Ç◊ÇÈ
		kmb::NodeNeighbor::iterator iter = neighborInfo.beginIteratorAt(nodeID);
		while( iter != neighborInfo.endIteratorAt(nodeID) )
		{
			kmb::elementIdType nei_id = iter->second;
			kmb::ElementContainer::iterator nei = surf0->find(nei_id);
			if( !nei.isFinished() ){
				int i0,i1;
				kmb::ElementRelation::relationType rel01 =
					kmb::ElementRelation::getRelation( eIter, i0, nei, i1 );
				switch( rel01 ){
					case kmb::ElementRelation::EQUAL:
						++eqCount;
						findFlag = true;
						break;
					case kmb::ElementRelation::REVERSE:
						++revCount;
						findFlag = true;
						break;
					default:
						break;
				}
				if( eqCount > 0 && revCount > 0){
					return kmb::ElementRelation::UNKNOWNRELATION;
				}
			}
			if( findFlag ){
				break;
			}
			++iter;
		}
		// í∏ì_ÇÃé¸ï”óvëfÇÃàÍívÇ∑ÇÈÇ‹ÇΩÇÕãtå¸Ç´ÇÃóvëfÇ™Ç»Ç¢
		if( !findFlag ){
			return kmb::ElementRelation::UNKNOWNRELATION;
		}
		++eIter;
	}

	if( eqCount == len0 ){
		return kmb::ElementRelation::EQUAL;
	}

	if( revCount == len0 ){
		return kmb::ElementRelation::REVERSE;
	}

	return kmb::ElementRelation::UNKNOWNRELATION;
}

kmb::ElementRelation::relationType
kmb::Matching::getFaceRelation
( kmb::MeshDB* mesh, const char* faceId0, const char* faceId1 )
{
	// FaceGroup Ç‹ÇΩÇÕ FaceVariable Ç≈Ç†ÇÈÇ±Ç∆ÇämîFÇ∑ÇÈ
	kmb::DataBindings* face0 = mesh->getDataBindingsPtr(faceId0);
	kmb::DataBindings* face1 = mesh->getDataBindingsPtr(faceId1);
	if( mesh == NULL || face0 == NULL || face1 == NULL ){
		return kmb::ElementRelation::UNKNOWNRELATION;
	}
	const kmb::DataBindings::bindingMode mode0 = face0->getBindingMode();
	const kmb::DataBindings::bindingMode mode1 = face1->getBindingMode();
	if( ( mode0 != kmb::DataBindings::FaceGroup && mode0 != kmb::DataBindings::FaceVariable ) ||
		( mode1 != kmb::DataBindings::FaceGroup && mode1 != kmb::DataBindings::FaceVariable ) )
	{
		return kmb::ElementRelation::UNKNOWNRELATION;
	}

	const size_t len0 = face0->getIdCount();
	const size_t len1 = face1->getIdCount();

	if( len0 != len1 ){
		return kmb::ElementRelation::UNKNOWNRELATION;
	}
	
	unsigned int eqCount = 0;	// àÍívÇµÇƒÇ¢ÇÈå¬êî
	unsigned int revCount = 0;	// ãtå¸Ç´ÇÃå¬êî

	// ãﬂñTèÓïÒÇçÏÇËíºÇ∑
	// face0 Ç©ÇÁãﬂñTèÓïÒÇçÏÇ¡Çƒ
	// face1 ÇÃóvëfÇÃí∏ì_Çä‹Çﬁ face0 ÇÃóvëfÇåüçıÇµÇƒ
	// àÍívÇ∑ÇÈ or ãtå¸Ç´ÇÃÇ‡ÇÃÇíTÇ∑
	kmb::NodeNeighborFaceInfo coboundary;

	coboundary.appendCoboundary( face0, mesh );
	kmb::DataBindings::iterator fIter = face1->begin();
	kmb::DataBindings::iterator fEnd = face1->end();
	kmb::Face f;
	int count = 0;
	while( fIter != fEnd ){
		bool findFlag = false;
		fIter.getFace( f );
		kmb::Element* element = f.createElement(mesh);
		if( element != NULL ){
			// ç≈èâÇÃí∏ì_ÇæÇØÇ≈å©ÇÍÇŒè\ï™
			kmb::nodeIdType nodeID = element->getCellId(0);
			// í∏ì_ÇÃé¸ï”Ç∆ÇÃä÷åWÇí≤Ç◊ÇÈ
			kmb::NodeNeighborFace::iterator iter = coboundary.beginIteratorAt(nodeID);
			kmb::NodeNeighborFace::iterator iEnd = coboundary.endIteratorAt(nodeID);
			while( iter != iEnd )
			{
				kmb::Face nei_f = iter->second;
				kmb::Element* nei = nei_f.createElement(mesh);
				if( nei != NULL ){
					int i0,i1;
					kmb::ElementRelation::relationType rel =
						kmb::ElementRelation::getRelation( *element, i0, *nei, i1 );
					switch( rel ){
						case kmb::ElementRelation::EQUAL:
							++eqCount;
							findFlag = true;
							break;
						case kmb::ElementRelation::REVERSE:
							++revCount;
							findFlag = true;
							break;
						default:
							break;
					}
					if( eqCount > 0 && revCount > 0){
						return kmb::ElementRelation::UNKNOWNRELATION;
					}
					delete nei;
				}
				if( findFlag ){
					break;
				}
				++iter;
			}
			delete element;
		}
		// í∏ì_ÇÃé¸ï”óvëfÇÃàÍívÇ∑ÇÈÇ‹ÇΩÇÕãtå¸Ç´ÇÃóvëfÇ™Ç»Ç¢
		if( !findFlag ){
			return kmb::ElementRelation::UNKNOWNRELATION;
		}
		++count;
		++fIter;
	}

	if( eqCount == len0 ){
		return kmb::ElementRelation::EQUAL;
	}

	if( revCount == len0 ){
		return kmb::ElementRelation::REVERSE;
	}

	return kmb::ElementRelation::UNKNOWNRELATION;
}

double
kmb::Matching::getDistanceEdgeToNodeWithParam
(const kmb::Point3DContainer* points0, kmb::ElementContainer* edge0,
 const kmb::Point3D* point1, kmb::elementIdType &elemId, double &param)
{
	kmb::Minimizer minimizer;

	elemId = kmb::Element::nullElementId; // ç≈è¨Ç™å©Ç¬Ç©ÇÁÇ»Ç¢

	kmb::Point3D n0;
	kmb::Point3D n1;

	if( points0 && edge0 && edge0->isUniqueDim(1) && point1 )
	{
		kmb::ElementContainer::iterator eIter = edge0->begin();
		while( eIter != edge0->end() )
		{
			if( points0->getPoint( eIter.getCellId(0), n0 ) &&
				points0->getPoint( eIter.getCellId(1), n1 ) ){
				// t ÇÕ node0 Ç∆ node1 ÇÃê¸ï™Ç [0,1] Ç…ëŒâûÇ≥ÇπÇΩÇ∆Ç´ÇÃ
				// êÇê¸ÇÃë´ÇÃÉpÉâÉÅÅ[É^ÇÃíl
				double temp;
				double distSq = point1->distanceSqToSegment( n0, n1, temp );
				// ç≈è¨ílÇéÊÇÈ elemID ÇäoÇ¶ÇƒÇ®Ç≠
				if( minimizer.update(distSq) )
				{
					elemId = eIter.getId();
					param = temp;
				}
			}
			++eIter;
		}
	}
	return sqrt(minimizer.getMin());
}

/*
int
kmb::Matching::matchNodeToNode
(kmb::Point3DContainer* points0, const kmb::Point3DContainer* points1,
	const double tolerance, std::map< kmb::nodeIdType, kmb::nodeIdType> &nodeMapper)
{
	if( points0 == NULL || points1 == NULL ){
		return 0;
	}

	kmb::Point3D point;
	kmb::Point3D nearestPoint;
	kmb::Point3DContainer::iterator pIter = points0->begin();
	while( pIter != points0->end() )
	{
		if( pIter.getPoint( point ) ){
			kmb::nodeIdType nearestId = kmb::nullNodeId;
			double d = points1->getNearest( &point, nearestPoint, nearestId );
			if( d < tolerance )
			{
				nodeMapper.insert( std::pair<kmb::nodeIdType,kmb::nodeIdType>( pIter.getId(), nearestId ) );
			}
		}
		++pIter;
	}
	return static_cast<int>(nodeMapper.size());
}
*/

// nodeMapper[ nodeId of mesh1 ] = nodeId of mesh0 
// mesh1 ÇÃ edgeId Ç≈ó^Ç¶ÇÁÇÍÇÈ Body è„ÇÃêﬂì_Ç…ëŒÇµÇƒ
// mesh0 ÇÃ 1 éüå≥ Body Ç∆ÇÃ nodeMatching ÇçsÇ§
int
kmb::Matching::matchNodeToNodeOnEdges
(kmb::MeshDB* mesh0, kmb::MeshDB* mesh1, kmb::bodyIdType edgeId,
 const double tolerance, std::map< kmb::nodeIdType, kmb::nodeIdType> &nodeMapper)
{
	if( mesh0 == NULL || mesh1 == NULL ){
		return 0;
	}
	kmb::ElementContainer* body = mesh1->getBodyPtr(edgeId);

	// get all nodes
	std::set< kmb::nodeIdType > nodeSet;
	if( body )
	{
		body->getNodesOfBody( nodeSet );
	}

	kmb::Node node;
	std::set< kmb::nodeIdType >::iterator nIter = nodeSet.begin();
	while( nIter != nodeSet.end() )
	{
		const kmb::nodeIdType nodeId = (*nIter);
		if( mesh1->getNode( nodeId, node ) )
		{
			// é©ï™é©êgÇÃ 1éüå≥ÇÃ Body Ç…Ç¬Ç¢ÇƒÇ∑Ç◊Çƒí≤Ç◊ÇÈ
			kmb::bodyIdType bodyCount = mesh0->getBodyCount();
			for(kmb::bodyIdType bodyId = 0; bodyId < bodyCount; ++bodyId){
				kmb::ElementContainer* body1 = mesh0->getBodyPtr( bodyId );
				if( body1 && body1->isUniqueDim(1) ){
					kmb::nodeIdType matchId = kmb::nullNodeId;
					double dist = mesh0->getNearestNodeInBody( node, bodyId, matchId );
					if( dist < tolerance )
					{
						nodeMapper.insert( std::pair< kmb::nodeIdType, kmb::nodeIdType >( nodeId, matchId ) );
					}
				}
			}
		}
		++nIter;
	}
	return static_cast<int>(nodeSet.size() - nodeMapper.size());
}

// mesh1 ÇÃ bodyId1 Ç≈ó^Ç¶ÇÁÇÍÇÈ Body è„ÇÃêﬂì_Ç…ëŒÇµÇƒ
// mesh0 ÇÃ ìØÇ∂éüå≥ÇÃ Body Ç∆ÇÃ nodeMatching ÇçsÇ§
int
kmb::Matching::matchNodeToNodeOnBody
(kmb::MeshDB* mesh0, kmb::MeshDB* mesh1, kmb::bodyIdType bodyId1,
 const double tolerance, std::map< kmb::nodeIdType, kmb::nodeIdType> &nodeMapper)
{
	if( mesh0 == NULL || mesh1 == NULL ){
		return 0;
	}
	kmb::ElementContainer* body1 = mesh1->getBodyPtr(bodyId1);

	// get all nodes
	std::set< kmb::nodeIdType > nodeSet;
	if( body1==NULL ){
		return 0;
	}
	body1->getNodesOfBody( nodeSet );
	int dimOfBody = body1->getDimension();

	kmb::Node node;
	std::set< kmb::nodeIdType >::iterator nIter = nodeSet.begin();
	while( nIter != nodeSet.end() )
	{
		const kmb::nodeIdType nodeId = (*nIter);
		if( mesh1->getNode( nodeId, node ) )
		{
			// mesh0 ÇÃ body1 Ç∆ìØÇ∂éüå≥ÇÃ Body Ç…Ç¬Ç¢ÇƒÇ∑Ç◊Çƒí≤Ç◊ÇÈ
			kmb::bodyIdType bodyCount = mesh0->getBodyCount();
			for(kmb::bodyIdType bodyId0 = 0; bodyId0 < bodyCount; ++bodyId0){
				kmb::ElementContainer* body0 = mesh0->getBodyPtr( bodyId0 );
				if( body0 && body0->isUniqueDim( dimOfBody ) ){
					kmb::nodeIdType matchId = kmb::nullNodeId;
					double dist = mesh0->getNearestNodeInBody( node, bodyId0, matchId );
					if( dist < tolerance )
					{
						nodeMapper.insert( std::pair< kmb::nodeIdType, kmb::nodeIdType >( nodeId, matchId ) );
					}
				}
			}
		}
		++nIter;
	}
	return static_cast<int>(nodeSet.size() - nodeMapper.size());
}

kmb::bodyIdType
kmb::Matching::searchMatchingEdge
(kmb::MeshDB* mesh0,kmb::MeshDB* mesh1,kmb::bodyIdType edgeId,double tolerance)
{
	kmb::bodyIdType matchingId = kmb::Body::nullBodyId;
	if( mesh0 && mesh1 ){
		const kmb::bodyIdType bodyCount = mesh0->getBodyCount();
		for(kmb::bodyIdType i = 0; i < bodyCount; ++i){
			if( getDistanceEdgeToEdge( mesh0, i, mesh1, edgeId ) < tolerance )
			{
				matchingId = i;
				break;
			}
		}
	}
	return matchingId;
}

int
kmb::Matching::nodeMatchingBetweenBodies(kmb::MeshDB* mesh0, kmb::bodyIdType bodyId0, kmb::MeshDB* mesh1, kmb::bodyIdType bodyId1, double tolerance, const char* coupleName)
{
	int counter = 0;
	kmb::Body* body0(NULL);
	if( mesh0 == NULL ||
		(body0=mesh0->getBodyPtr(bodyId0)) == NULL ||
		mesh1 == NULL ||
		mesh1->getBodyPtr(bodyId1) == NULL ||
		coupleName == NULL )
	{
		return counter;
	}

	kmb::DataVariable<kmb::nodeIdType,kmb::IntegerValue>* coupleData = NULL;
	kmb::DataBindings* data = mesh0->getDataBindingsPtr(coupleName);
	if( data ){
		if( data->getBindingMode() == kmb::DataBindings::NodeVariable &&
				data->getValueType() == kmb::PhysicalValue::Integer ){
			coupleData =
				reinterpret_cast< kmb::DataVariable<kmb::nodeIdType,kmb::IntegerValue>* >(data);
		}
	}else{
		coupleData =
			reinterpret_cast< kmb::DataVariable<kmb::nodeIdType,kmb::IntegerValue>* >
			( mesh0->createDataBindings
				(coupleName,
				 kmb::DataBindings::NodeVariable,
				 kmb::PhysicalValue::Integer,
				 "import", bodyId0 ) );
	}

	if( coupleData == NULL ){
		return counter;
	}

	kmb::Node node;
	// mesh0 ÇÃ body0 è„ÇÃêﬂì_Ç mesh1 ÇÃ body1 Ç©ÇÁíTÇ∑
	kmb::ElementContainer::iterator eIter = body0->begin();
	while( !eIter.isFinished() ){
		const int len = eIter.getNodeCount();
		for(int i=0;i<len;++i){
			kmb::nodeIdType myId = eIter.getCellId(i);
			if( !coupleData->hasId( myId ) && mesh0->getNode( myId, node ) ){
				kmb::nodeIdType matchId = kmb::nullNodeId;
				double dist = mesh1->getNearestNodeInBody( node, bodyId1, matchId );
				if( dist < tolerance ){
					++counter;
					coupleData->setPhysicalValue( myId, new kmb::IntegerValue(matchId) );
				}
			}
		}
		++eIter;
	}
	return counter;
}

int
kmb::Matching::nodeMatchingOnBody(kmb::MeshDB* mesh0, kmb::bodyIdType bodyId0, kmb::MeshDB* mesh1, double tolerance, const char* coupleName)
{
	int counter = 0;
	kmb::Body *body0 = NULL;
	if( mesh0 == NULL ||
		mesh1 == NULL ||
		(body0=mesh0->getBodyPtr(bodyId0)) == NULL ||
		coupleName == NULL )
	{
		return counter;
	}

	kmb::DataVariable<kmb::nodeIdType,kmb::IntegerValue>* coupleData = NULL;
	kmb::DataBindings* data = mesh0->getDataBindingsPtr(coupleName);
	if( data ){
		if( data->getBindingMode() == kmb::DataBindings::NodeVariable &&
				data->getValueType() == kmb::PhysicalValue::Integer ){
			coupleData =
				reinterpret_cast< kmb::DataVariable<kmb::nodeIdType,kmb::IntegerValue>* >(data);
		}
	}else{
		coupleData =
			reinterpret_cast< kmb::DataVariable<kmb::nodeIdType,kmb::IntegerValue>* >
			( mesh0->createDataBindings
				(coupleName,
				 kmb::DataBindings::NodeVariable,
				 kmb::PhysicalValue::Integer,
				 "import", bodyId0 ) );
	}

	if( coupleData == NULL ){
		return counter;
	}

	kmb::Node node;
	// mesh0 ÇÃ body0 è„ÇÃêﬂì_Ç mesh1 ÇÃêﬂì_ëSëÃÇ©ÇÁíTÇ∑
	kmb::ElementContainer::iterator eIter = body0->begin();
	while( !eIter.isFinished() ){
		const int len = eIter.getNodeCount();
		for(int i=0;i<len;++i){
			kmb::nodeIdType myId = eIter.getCellId(i);
			if( !coupleData->hasId( myId ) && mesh0->getNode( myId, node ) ){
				kmb::nodeIdType matchId = kmb::nullNodeId;
				double dist = mesh1->getNearestNode( node.x(), node.y(), node.z(), matchId );
				if( dist < tolerance ){
					++counter;
					coupleData->setPhysicalValue( myId, new kmb::IntegerValue(matchId) );
				}
			}
		}
		++eIter;
	}
	return counter;
}

