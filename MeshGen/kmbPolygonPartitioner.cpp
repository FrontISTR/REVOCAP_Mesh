/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : PolygonPartitioner                                      #
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
#include "MeshGen/kmbPolygonPartitioner.h"
#include "MeshDB/kmbElementContainer.h"
#include "MeshDB/kmbTriangle.h"
#include "MeshDB/kmbPolygon.h"
#include "MeshDB/kmbSegment.h"
#include "MeshDB/kmbMeshDB.h"
#include "MeshDB/kmbTypes.h"
#include "Geometry/kmbGeometry.h"

#include <map>
#include <vector>

// local struct
struct  nodeStackType{
	kmb::nodeIdType nodeId;
	bool isLeft;    // true if left side
};

kmb::PolygonPartitioner::PolygonPartitioner(void)
: points(NULL)
, initialPolygon(NULL)
{
}

kmb::PolygonPartitioner::~PolygonPartitioner(void)
{
	clear();
}

void
kmb::PolygonPartitioner::clear(void)
{
	if( initialPolygon != NULL )
		delete initialPolygon;
}

// 単体実行用
kmb::bodyIdType
kmb::PolygonPartitioner::partitionToTriangles(kmb::MeshDB* mesh,kmb::bodyIdType edgeId)
{
	kmb::bodyIdType triId = kmb::Body::nullBodyId;
	kmb::Body* edges = NULL;
	if( mesh && mesh->getNodeDim() == 2 &&
		(edges = mesh->getBodyPtr(edgeId) ) != NULL &&
		edges->isUniqueType( kmb::SEGMENT ) )
	{
		kmb::ElementContainerMap* triangles = NULL;
		triangles = new kmb::ElementContainerMap();
		setPoints( mesh->getNode2Ds() );
		setInitialPolygon( edges );
		partition( *triangles );
		triId = mesh->appendBody( triangles );
	}
	return triId;
}

kmb::bodyIdType
kmb::PolygonPartitioner::partitionToTriangles(kmb::MeshDB* mesh,kmb::bodyIdType edgeId,const kmb::FramedPlane &plane)
{
	kmb::bodyIdType triId = kmb::Body::nullBodyId;
	kmb::Body* edges = NULL;
	if( mesh &&
		(edges = mesh->getBodyPtr(edgeId) ) != NULL &&
		edges->isUniqueType( kmb::SEGMENT ) )
	{
		// polygon の点を2次元射影して points に格納する
		kmb::Point2DContainerMap points;
		std::set< kmb::nodeIdType > nodeSet;
		edges->getNodesOfBody( nodeSet );
		std::set< kmb::nodeIdType >::iterator nIter = nodeSet.begin();
		kmb::Point3D point;
		while( nIter != nodeSet.end() )
		{
			kmb::nodeIdType nodeId = (*nIter);
			++nIter;
			if( mesh->getNode( nodeId, point ) ){
				kmb::Point2D uv = plane.transformTo2D( point );
				points.addPoint( uv, nodeId );
			}
		}
		kmb::ElementContainerMap* triangles = NULL;
		triangles = new kmb::ElementContainerMap();
		setPoints( &points );
		setInitialPolygon( edges );
		partition( *triangles );
		triId = mesh->appendBody( triangles );
	}
	return triId;
}

void
kmb::PolygonPartitioner::setPoints(const kmb::Point2DContainer* targetPoints)
{
	this->points = targetPoints;
}

void
kmb::PolygonPartitioner::setInitialPolygon(kmb::ElementContainer* edges)
{
	if( edges && edges->isUniqueType( kmb::SEGMENT ) ){
		if( initialPolygon != NULL ){
			delete initialPolygon;
			initialPolygon = NULL;
		}
		initialPolygon = new kmb::Polygon();
		initialPolygon->setEdges( edges );
	}
}

size_t
kmb::PolygonPartitioner::getCount(void) const
{
	if( initialPolygon != NULL )
		return this->initialPolygon->getSize();
	else
		return 0;
}

//
// y 座標の monotonisity を考えた時のタイプ
// y 座標が等しい時の評価の仕方は x 座標で判断する
// すなわち全体を少しだけ時計回りに回転したとして考える
//
// 2次元平面上、多角形は反時計回りに並んでいるものとする
//
// START    0       END  -1     1
//        ／ ＼            ＼ ／
//       1     -1            0
//
// SPLIT    0       MERGE  1    -1 
//        ／ ＼             ＼ ／
//      -1     1              0
//
// ごくまれに数値誤差で上記のうちのいずれかと判定しても area = 0 となってスルーすることがある
// 
kmb::PolygonPartitioner::vertexType
kmb::PolygonPartitioner::getVertexType(const kmb::Point2D& previousPoint,const kmb::Point2D& targetPoint,const kmb::Point2D& nextPoint)
{
	double area = kmb::Point2D::area( previousPoint, targetPoint, nextPoint );

	// できるだけ生の座標値の大小だけから判断せよ
	if      (   // LARGER_X( previousPoint, nextPoint ) &&
				area >= 0.0 &&
				LARGER_Y( targetPoint, previousPoint ) &&
				LARGER_Y( targetPoint, nextPoint ) ){
		return kmb::PolygonPartitioner::kSTART;
	}else if(   // LARGER_X( nextPoint, previousPoint ) && 
				area <= 0.0 && 
				LARGER_Y( targetPoint, previousPoint ) && 
				LARGER_Y( targetPoint, nextPoint ) ){
		return kmb::PolygonPartitioner::kSPLIT;
	}else if(   // LARGER_X( nextPoint, previousPoint ) &&
				area >= 0.0 &&
				LARGER_Y( previousPoint, targetPoint ) &&
				LARGER_Y( nextPoint,     targetPoint ) ){
		return kmb::PolygonPartitioner::kEND;
	}else if(   // LARGER_X( previousPoint, nextPoint ) &&
				area <= 0.0 &&
				LARGER_Y( previousPoint, targetPoint ) &&
				LARGER_Y( nextPoint,     targetPoint ) ){
		return kmb::PolygonPartitioner::kMERGE;
	}else{
		return kmb::PolygonPartitioner::kREGULAR;
	}
}

kmb::PolygonPartitioner::vertexType
kmb::PolygonPartitioner::getVertexType(kmb::Polygon* polygon,kmb::nodeIdType nodeId) const
{
	if( points == NULL || polygon == NULL || !polygon->include(nodeId) )
		return kmb::PolygonPartitioner::kUNKNOWN;

	kmb::nodeIdType previousNodeId = polygon->getNodeId(nodeId,-1);
	kmb::nodeIdType nextNodeId = polygon->getNodeId(nodeId,1);

	kmb::Point2D p0,p1,p2;
	if( points->getPoint( previousNodeId, p0 ) &&
		points->getPoint( nodeId, p1 ) &&
		points->getPoint( nextNodeId, p2 ) )
	{
		return getVertexType(p0,p1,p2);
	}else{
		return kmb::PolygonPartitioner::kUNKNOWN;
	}
}

kmb::elementIdType
kmb::PolygonPartitioner::getNearestSegmentWithSameLevel(kmb::nodeIdType nodeId, bool left) const
{
	kmb::elementIdType nearSegment = kmb::Element::nullElementId;
	const kmb::ElementContainer* edges = initialPolygon->getEdges();
	if( points == NULL || edges == NULL )
		return nearSegment;

	kmb::Point2D point;
	if( !points->getPoint( nodeId, point ) ){
		return nearSegment;
	}

	double nearX = (left) ? -DBL_MAX : DBL_MAX;

	kmb::ElementContainer::const_iterator eIter = edges->begin();
	while( eIter != edges->end() )
	{
		if( !eIter.include(nodeId) )
		{
			kmb::Point2D p0,p1;
			if( points->getPoint( eIter.getCellId(0), p0 ) && points->getPoint( eIter.getCellId(1), p1 ) ){
				if( ( LARGER_Y( p0, point ) && LARGER_Y( point, p1 ) ) ||
					( LARGER_Y( point, p0 ) && LARGER_Y( p1, point ) ) )
				{
					if( left && ( LARGER_X( point, p0 ) || LARGER_X( point, p1 ) ) ){
						// 線分 p0 と p1 上の y 座標が point と等しくなる x 座標の値
						double x  = (p1.x() - p0.x()) * (point.y() - p0.y() ) / ( p1.y() - p0.y() ) + p0.x();
						if( nearX <= x && x < point.x() ){
							nearX = x;
							nearSegment = eIter.getId();
						}
					}else if( !left && ( LARGER_X( p0, point ) || LARGER_X( p1, point ) ) ){
						// 線分 p0 と p1 上の y 座標が point と等しくなる x 座標の値
						double x  = (p1.x() - p0.x()) * (point.y() - p0.y() ) / ( p1.y() - p0.y() ) + p0.x();
						if( nearX >= x && x > point.x() ){
							nearX = x;
							nearSegment = eIter.getId();
						}
					}
				}
			}
		}
		++eIter;
	}

	return nearSegment;
}

kmb::nodeIdType
kmb::PolygonPartitioner::getHelperNode( kmb::nodeIdType nodeID, kmb::elementIdType leftID, kmb::elementIdType rightID, kmb::PolygonPartitioner::vertexType vType) const
{
	kmb::nodeIdType helperNodeId = kmb::nullNodeId;
	const kmb::ElementContainer* eCon = NULL;
	if( initialPolygon == NULL || points == NULL ||
		(eCon = initialPolygon->getEdges()) == NULL )
	{
		return helperNodeId;
	}
	kmb::ElementContainer::const_iterator leftEdge = eCon->find(leftID);
	kmb::ElementContainer::const_iterator rightEdge = eCon->find(rightID);
	if( leftEdge.isFinished() || rightEdge.isFinished() )
	{
		return helperNodeId;
	}
	kmb::Point2D l0,l1,r0,r1,point;
	if( !points->getPoint( leftEdge.getCellId(0), l0 )
		|| !points->getPoint( leftEdge.getCellId(1), l1 )
		|| !points->getPoint( rightEdge.getCellId(0), r0 )
		|| !points->getPoint( rightEdge.getCellId(1), r1 )
		|| !points->getPoint( nodeID, point ) )
	{
		return helperNodeId;
	}

	kmb::Point2D helperNode;
	switch( vType )
	{
	case kSPLIT:
		{
			if( LARGER_Y( l0, r1 ) ){
				helperNode = r1;
				helperNodeId = rightEdge.getCellId(1);
			}else{
				helperNode = l0;
				helperNodeId = leftEdge.getCellId(0);
			}
		}
		break;
	case kMERGE:
		{
			if( LARGER_Y( l1, r0 ) ){
				helperNode = r0;
				helperNodeId = rightEdge.getCellId(0);
			}else{
				helperNode = l1;
				helperNodeId = leftEdge.getCellId(1);
			}
		}
		break;
	default:
		break;
	}
	kmb::Point2DContainer::const_iterator pIter = points->begin();
	while( pIter != points->end() )
	{
		const kmb::nodeIdType targetNodeId = pIter.getId();
		if( targetNodeId != nodeID && initialPolygon->include(targetNodeId) )
		{
			kmb::Point2D targetNode;
			// left と right の間にあるか
			if( pIter.getPoint( targetNode ) &&
				Point2D::area( l0, l1, targetNode ) > 0.0 && Point2D::area( r0, r1, targetNode ) > 0.0 )
			{
				// node の y 座標と nearY の間にあるか
				switch( vType ){
					case kmb::PolygonPartitioner::kSPLIT:
						{
							if( LARGER_Y( targetNode, point ) && LARGER_Y( helperNode, targetNode ) ){ //&&
//								LARGER_X( targetNode, helperNode ) && LARGER_X( point, targetNode ) ){
								helperNode = targetNode;
								helperNodeId = targetNodeId;
							}
						}
						break;
					case kmb::PolygonPartitioner::kMERGE:
						{
							if( LARGER_Y( point, targetNode ) && LARGER_Y( targetNode, helperNode ) ){ //&&
//								LARGER_X( helperNode, targetNode ) && LARGER_X( targetNode, point ) ){
								helperNode = targetNode;
								helperNodeId = targetNodeId;
							}
						}
						break;
					default:
						break;
				}
			}
		}
		++pIter;
	}
	return helperNodeId;
}

bool
kmb::PolygonPartitioner::partition( kmb::ElementContainer &body )
{
	bool retVal = true;
	if( points == NULL || initialPolygon == NULL || initialPolygon->isClosed() == false ){
		retVal = false;
		return retVal;
	}
	std::vector< std::pair<kmb::nodeIdType, kmb::nodeIdType > > diagonals;
	// split と merge node を探す

	kmb::Point2DContainer::const_iterator pIter = points->begin();
	while( pIter != points->end() ){
		const kmb::nodeIdType nodeId = pIter.getId();
		kmb::PolygonPartitioner::vertexType vtype = getVertexType( initialPolygon, nodeId );
		switch( vtype )
		{
		case kmb::PolygonPartitioner::kSPLIT:
			{
				kmb::elementIdType left = getNearestSegmentWithSameLevel(nodeId,true);
				kmb::elementIdType right = getNearestSegmentWithSameLevel(nodeId,false);
				kmb::nodeIdType helper = getHelperNode( nodeId, left, right, vtype );
				if( helper != kmb::nullNodeId ){
					diagonals.push_back( std::pair<kmb::nodeIdType, kmb::nodeIdType>(nodeId,helper) );
				}
				break;
			}
		case kmb::PolygonPartitioner::kMERGE:
			{
				kmb::elementIdType left = getNearestSegmentWithSameLevel(nodeId,true);
				kmb::elementIdType right = getNearestSegmentWithSameLevel(nodeId,false);
				kmb::nodeIdType helper = getHelperNode( nodeId, left, right, vtype );
				if( helper != kmb::nullNodeId ){
					diagonals.push_back( std::pair<kmb::nodeIdType, kmb::nodeIdType>(nodeId,helper) );
				}
				break;
			}
		default:
			break;
		}
		++pIter;
	}

	{
		// monotone 化
		std::vector< kmb::Polygon* > polygons;
		initialPolygon->dividePolygonsByDiagonals( points, diagonals, polygons );
		retVal &= (polygons.size() > 0);
		// monotone polygon を triangulation
		std::vector< kmb::Polygon* >::iterator gIter = polygons.begin();
		while( gIter != polygons.end() )
		{
			kmb::Polygon* polygon = *gIter;
			if( polygon != NULL ){
				retVal &= triangulateMonotonePolygon(polygon,body);
				delete polygon;
				polygon = NULL;
			}
			++gIter;
		}
		polygons.clear();
	}
	return retVal;
}

bool
kmb::PolygonPartitioner::triangulateMonotonePolygon
(kmb::Polygon* polygon, kmb::ElementContainer &body )
{
	bool retVal = true;
	// 三角形登録用
	kmb::nodeIdType cells[3] = {kmb::nullNodeId,kmb::nullNodeId,kmb::nullNodeId};
	const kmb::ElementContainer* eCon = NULL;
	if( polygon == NULL || points == NULL ||
		(eCon = polygon->getEdges()) == NULL ||
		eCon->getCount() == 0 )
	{
		retVal = false;
		return retVal;
	}

	// START Vertex END Vertex を探す
	kmb::nodeIdType startVertex = kmb::nullNodeId;
	kmb::nodeIdType endVertex = kmb::nullNodeId;

	std::set< kmb::nodeIdType > nodeSet;
	eCon->getNodesOfBody( nodeSet );
	std::set< kmb::nodeIdType >::iterator nIter = nodeSet.begin();
	while( nIter != nodeSet.end() )
	{
		kmb::nodeIdType nodeId = (*nIter);
		kmb::PolygonPartitioner::vertexType vtype = getVertexType(polygon,nodeId);
		switch( vtype ){
			case kmb::PolygonPartitioner::kSTART:
				startVertex = nodeId;
				break;
			case kmb::PolygonPartitioner::kEND:
				endVertex = nodeId;
				break;
			default:
				break;
		}
		++nIter;
	}

	if( startVertex == kmb::nullNodeId || endVertex == kmb::nullNodeId ){
		retVal = false;
		return retVal;
	}
	kmb::nodeIdType top = startVertex;
	kmb::nodeIdType left = polygon->getNodeId(startVertex,1);  // 計算中の y 座標が最小の点（左側）
	kmb::nodeIdType right = polygon->getNodeId(startVertex,-1); // 計算中の y 座標が最小の点（右側）
	std::vector< nodeStackType > nodeStack;                    // 計算中の（まだ三角形化していない）点の Stack
	Point2D leftPoint,rightPoint;
	points->getPoint( left, leftPoint );
	points->getPoint( right, rightPoint );
	if( LARGER_Y( leftPoint, rightPoint ) ){
		nodeStackType stack0 = {top,false};
		nodeStackType stack1 = {left,true};
		nodeStack.push_back( stack0 );
		nodeStack.push_back( stack1 );
		right = top;
	}else{
		nodeStackType stack0 = {top,true};
		nodeStackType stack1 = {right,false};
		nodeStack.push_back( stack0 );
		nodeStack.push_back( stack1 );
		left = top;
	}

	while( true ){
		size_t stackSize = nodeStack.size();
		if( stackSize < 2 ){
			break;
		}
		// y 座標が次に大きな点
		kmb::nodeIdType left_next = polygon->getNodeId(left,1);
		kmb::nodeIdType right_next = polygon->getNodeId(right,-1);
		if( !points->getPoint( left_next, leftPoint ) || !points->getPoint( right_next, rightPoint ) ){
			// monotone ならばおこらないはず・・・
			retVal &= false;
			break;
		}
		nodeStackType nextNode;
		Point2D nextPoint;
		if( LARGER_Y( leftPoint, rightPoint ) ){
			nextNode.nodeId = left_next;
			nextNode.isLeft = true;
			left = left_next;
			nextPoint = leftPoint;
		}else{
			nextNode.nodeId = right_next;
			nextNode.isLeft = false;
			right = right_next;
			nextPoint = rightPoint;
		}

		nodeStackType last0 = nodeStack[ stackSize-1 ];
		nodeStackType last1 = nodeStack[ stackSize-2 ];
		Point2D lastPoint0,lastPoint1;
		points->getPoint( last0.nodeId,lastPoint0 );
		points->getPoint( last1.nodeId,lastPoint1 );
		if( last0.isLeft != nextNode.isLeft ){
			// 最終ノードと次のノードが異なる側
			// nodeStack のすべてに対して
			// nextNode, stack0, stack1 の形の三角形を追加
			// stack には最終ノードと次のノードだけを残す
			for(unsigned int i=0;i<stackSize-1;++i){
				nodeStackType n0 = nodeStack[i];
				nodeStackType n1 = nodeStack[i+1];
				Point2D p0,p1;
				points->getPoint( n0.nodeId, p0 );
				points->getPoint( n1.nodeId, p1 );
				if( n1.isLeft ){
					double area = kmb::Point2D::area( nextPoint, p0, p1 );
					if( area >= 0.0 ){
						cells[0] = nextNode.nodeId;
						cells[1] = n0.nodeId;
						cells[2] = n1.nodeId;
						body.addElement( kmb::TRIANGLE, cells );
					}
				}else{
					double area = kmb::Point2D::area( nextPoint, p1, p0 );
					if( area >= 0.0 ){
						cells[0] = nextNode.nodeId;
						cells[1] = n1.nodeId;
						cells[2] = n0.nodeId;
						body.addElement( kmb::TRIANGLE, cells );
					}
				}
			}
			nodeStack.clear();
			nodeStack.push_back( last0 );
			nodeStack.push_back( nextNode );
			if( nextNode.isLeft ){
				left = nextNode.nodeId;
				right = last0.nodeId;
			}else{
				right = nextNode.nodeId;
				left = last0.nodeId;
			}
		}else{
			// 最終ノードと次のノードが同じ側
			// nodeStack の最終から順に2つずつ取って来て
			// 次のノードとの面積が正ならば三角形を登録して
			// 最終ノードを pop する
			// area < 0 になるか nodeStack の個数が 1 になるまで繰り返し
			while( true ){
				double area = ( last0.isLeft ) ?
					kmb::Point2D::area( nextPoint, lastPoint1, lastPoint0 ) :
					kmb::Point2D::area( nextPoint, lastPoint0, lastPoint1 ) ;
				if( area >= 0.0 ){
					if( last0.isLeft ){
						cells[0] = nextNode.nodeId;
						cells[1] = last1.nodeId;
						cells[2] = last0.nodeId;
						body.addElement( kmb::TRIANGLE, cells );
					}else{
						cells[0] = nextNode.nodeId;
						cells[1] = last0.nodeId;
						cells[2] = last1.nodeId;
						body.addElement( kmb::TRIANGLE, cells );
					}
					nodeStack.pop_back();
					if( nodeStack.size() == 1 ){
						nodeStack.push_back( nextNode );
						if( nextNode.isLeft ){
							left = nextNode.nodeId;
						}else{
							right = nextNode.nodeId;
						}
						break;
					}
				}
				else
				{
					nodeStack.push_back( nextNode );
					if( nextNode.isLeft ){
						left = nextNode.nodeId;
					}else{
						right = nextNode.nodeId;
					}
					break;
				}
				stackSize = nodeStack.size();
				last0 = nodeStack[ stackSize-1 ];
				last1 = nodeStack[ stackSize-2 ];
				points->getPoint( last0.nodeId,lastPoint0 );
				points->getPoint( last1.nodeId,lastPoint1 );
			}
		}
		if( left == endVertex || right == endVertex ){
			break;
		}
	}
	return retVal;
}
