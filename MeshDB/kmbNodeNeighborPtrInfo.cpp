/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : NodeNeighborPtrInfo                                     #
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
#include "MeshDB/kmbNodeNeighborPtrInfo.h"
#include "MeshDB/kmbElement.h"
#include "MeshDB/kmbElementContainer.h"
#include "MeshDB/kmbMeshData.h"
#include "MeshDB/kmbElementRelation.h"

kmb::NodeNeighborPtrInfo::NodeNeighborPtrInfo(void)
: coboundaries(), admitAnti(true)
{
}

kmb::NodeNeighborPtrInfo::~NodeNeighborPtrInfo(void)
{
}

void
kmb::NodeNeighborPtrInfo::clear(void)
{
	coboundaries.clear();
}

bool
kmb::NodeNeighborPtrInfo::getIgnoreOrientation(void) const
{
	return admitAnti;
}

void
kmb::NodeNeighborPtrInfo::setIgnoreOrientation(bool f)
{
	admitAnti = f;
}

bool
kmb::NodeNeighborPtrInfo::append( kmb::nodeIdType nodeId, kmb::Element* element )
{
	if( element ){
		// 重複しないように登録する
		std::pair< NodeNeighborPtr::iterator, NodeNeighborPtr::iterator >
			bIterPair = coboundaries.equal_range( nodeId );
		NodeNeighborPtr::iterator nIter = bIterPair.first;
		while( nIter != bIterPair.second )
		{
			if(nIter->second == element){
				// 既に登録済み
				return false;
			}
			++nIter;
		}
		coboundaries.insert( std::pair< kmb::nodeIdType, kmb::Element* >(nodeId,element) );
		return true;
	}
	return false;
}

bool
kmb::NodeNeighborPtrInfo::erase( kmb::nodeIdType nodeId, kmb::Element* element )
{
	if( element ){
		// 検索して削除
		std::pair< NodeNeighborPtr::iterator, NodeNeighborPtr::iterator >
			bIterPair = coboundaries.equal_range( nodeId );
		NodeNeighborPtr::iterator nIter = bIterPair.first;
		while( nIter != bIterPair.second )
		{
			if(nIter->second == element){
				// 登録済みのものを削除
				coboundaries.erase( nIter );
				return true;
			}
			++nIter;
		}
	}
	return false;
}

bool
kmb::NodeNeighborPtrInfo::appendCoboundary( kmb::Element* element )
{
	if( element ){
		// この要素が生成する隣接関係を登録する
		const unsigned int len = element->getNodeCount();
		for(unsigned int i=0;i<len;++i){
			kmb::nodeIdType nodeId = element->getNodeId(i);
			this->append(nodeId,element);
		}
		return true;
	}
	return false;
}

bool
kmb::NodeNeighborPtrInfo::appendCoboundary( kmb::ElementContainer* elements )
{
	if( elements == NULL )
		return false;
	kmb::ElementContainer::iterator eIter = elements->begin();
	while( !eIter.isFinished() )
	{
		this->appendCoboundary( eIter.getElement() );
		++eIter;
	}
	return true;
}

bool
kmb::NodeNeighborPtrInfo::appendCoboundary( kmb::MeshData* mesh )
{
	if( mesh == NULL )
		return false;
	const kmb::bodyIdType bodyCount = mesh->getBodyCount();
	for( kmb::bodyIdType id = 0; id < bodyCount; ++id){
		kmb::ElementContainer* body = mesh->getBodyPtr( id );
		appendCoboundary( body );
	}
	return true;
}

bool
kmb::NodeNeighborPtrInfo::deleteCoboundary( kmb::Element* element )
{
	if( element ){
		// この要素が生成する隣接関係を削除する
		const unsigned int len = element->getNodeCount();
		for(unsigned int i=0;i<len;++i){
			kmb::nodeIdType nodeId = element->getNodeId(i);
			this->erase(nodeId,element);
		}
		return true;
	}
	return false;
}

int
kmb::NodeNeighborPtrInfo::getElementNeighbor( const kmb::Element* element, kmb::Element** neighbors ) const
{
	if( element == NULL || neighbors == NULL )
		return 0;

	int count = 0;

	// coboundaries から計算する
	// 自分自身の頂点配列から
	// 周辺要素を取り出して、
	// それとの要素間関係を計算する

	const int boundaryNum = element->getBoundaryCount();

	// Segment Segment2 の時は特別
	switch( element->getDimension() )
	{
		case 1:
		{
			for(int i=0;i<2;++i)
			{
				neighbors[i] = NULL;
				const kmb::nodeIdType nodeId = element->getNodeId(i);
				// 頂点ごとの周辺要素との関係を調べる
				std::pair< NodeNeighborPtr::const_iterator, NodeNeighborPtr::const_iterator >
					eIterPair = coboundaries.equal_range( nodeId );
				NodeNeighborPtr::const_iterator eIter = eIterPair.first;
				while( eIter != eIterPair.second && neighbors[i] == NULL )
				{
					// 異なる要素が節点と共有していたら登録する
					if( element != eIter->second ){
						neighbors[ i ] = eIter->second;
						++count;
					}
					++eIter;
				}
			}
		}
		case 2:
		{
			int index = -1;
			int otherIndex = -1;
			for(int i=0;i<boundaryNum;++i){
				neighbors[i] = NULL;
				// Face の最初の頂点で探す
				kmb::nodeIdType nodeId = element->getBoundaryNodeId(i,0);
				// 頂点ごとの周辺要素との関係を調べる
				std::pair< NodeNeighborPtr::const_iterator, NodeNeighborPtr::const_iterator >
					eIterPair = coboundaries.equal_range( nodeId );
				NodeNeighborPtr::const_iterator eIter = eIterPair.first;
				while( eIter != eIterPair.second && neighbors[i] == NULL )
				{
					// 異なる要素が接していたら登録する
					kmb::Element* coElement = eIter->second;
					if( coElement && element != coElement ){
						kmb::ElementRelation::relationType rel =
							kmb::ElementRelation::getRelation( *element, index, *coElement, otherIndex );
						if( index == i &&
							( rel == kmb::ElementRelation::ADJACENT ||
							  (admitAnti && rel == kmb::ElementRelation::ANTIADJACENT) ) ){
							neighbors[i] = coElement;
							++count;
						}
					}
					++eIter;
				}
			}
		}
		case 3:
		{
			int index = -1;
			int otherIndex = -1;
			for(int i=0;i<boundaryNum;++i){
				neighbors[i] = NULL;
				// Face の最初の頂点で探す
				kmb::nodeIdType nodeId = element->getBoundaryNodeId(i,0);
				// 頂点ごとの周辺要素との関係を調べる
				std::pair< NodeNeighborPtr::const_iterator, NodeNeighborPtr::const_iterator >
					eIterPair = coboundaries.equal_range( nodeId );
				NodeNeighborPtr::const_iterator eIter = eIterPair.first;
				while( eIter != eIterPair.second && neighbors[i] == NULL )
				{
					kmb::Element* coElement = eIter->second;
					// 異なる要素が接していたら登録する
					if( coElement && element != coElement ){
						kmb::ElementRelation::relationType rel =
							kmb::ElementRelation::getRelation( *element, index, *coElement, otherIndex );
						if( index == i &&
							( rel == kmb::ElementRelation::ADJACENT ||
							  (admitAnti && rel == kmb::ElementRelation::ANTIADJACENT) ) ){
							neighbors[i] = coElement;
							++count;
						}
					}
					++eIter;
				}
			}
		}
		default:
			break;
	}
	return count;
}

int
kmb::NodeNeighborPtrInfo::getElementNeighborFace( const kmb::Element* element, kmb::Element** neighbors, int *faces ) const
{
	if( element == NULL || neighbors == NULL )
		return 0;

	int count = 0;

	// coboundaries から計算する
	// 自分自身の頂点配列から
	// 周辺要素を取り出して、
	// それとの要素間関係を計算する

	const int boundaryNum = element->getBoundaryCount();

	// Segment Segment2 の時は特別
	switch( element->getDimension() )
	{
		case 1:
		{
			for(int i=0;i<2;++i)
			{
				neighbors[i] = NULL;
				faces[i] = -1;
				const kmb::nodeIdType nodeId = element->getNodeId(i);
				// 頂点ごとの周辺要素との関係を調べる
				std::pair< NodeNeighborPtr::const_iterator, NodeNeighborPtr::const_iterator >
					eIterPair = coboundaries.equal_range( nodeId );
				NodeNeighborPtr::const_iterator eIter = eIterPair.first;
				while( eIter != eIterPair.second && neighbors[i] == NULL )
				{
					// 異なる要素が節点と共有していたら登録する
					if( element != eIter->second ){
						neighbors[ i ] = eIter->second;
						faces[i] = ((*eIter->second)[0] == nodeId)? 0 : 1;
						++count;
					}
					++eIter;
				}
			}
		}
		case 2:
		{
			int index = -1;
			int otherIndex = -1;
			for(int i=0;i<boundaryNum;++i){
				neighbors[i] = NULL;
				faces[i] = -1;
				// Face の最初の頂点で探す
				kmb::nodeIdType nodeId = element->getBoundaryNodeId(i,0);
				// 頂点ごとの周辺要素との関係を調べる
				std::pair< NodeNeighborPtr::const_iterator, NodeNeighborPtr::const_iterator >
					eIterPair = coboundaries.equal_range( nodeId );
				NodeNeighborPtr::const_iterator eIter = eIterPair.first;
				while( eIter != eIterPair.second && neighbors[i] == NULL )
				{
					// 異なる要素が接していたら登録する
					kmb::Element* coElement = eIter->second;
					if( coElement && element != coElement ){
						kmb::ElementRelation::relationType rel =
							kmb::ElementRelation::getRelation( *element, index, *coElement, otherIndex );
						if( index == i &&
							( rel == kmb::ElementRelation::ADJACENT ||
							  (admitAnti && rel == kmb::ElementRelation::ANTIADJACENT) ) ){
							neighbors[i] = coElement;
							faces[i] = otherIndex;
							++count;
						}
					}
					++eIter;
				}
			}
		}
		case 3:
		{
			int index = -1;
			int otherIndex = -1;
			for(int i=0;i<boundaryNum;++i){
				neighbors[i] = NULL;
				faces[i] = -1;
				// Face の最初の頂点で探す
				kmb::nodeIdType nodeId = element->getBoundaryNodeId(i,0);
				// 頂点ごとの周辺要素との関係を調べる
				std::pair< NodeNeighborPtr::const_iterator, NodeNeighborPtr::const_iterator >
					eIterPair = coboundaries.equal_range( nodeId );
				NodeNeighborPtr::const_iterator eIter = eIterPair.first;
				while( eIter != eIterPair.second && neighbors[i] == NULL )
				{
					kmb::Element* coElement = eIter->second;
					// 異なる要素が接していたら登録する
					if( coElement && element != coElement ){
						kmb::ElementRelation::relationType rel =
							kmb::ElementRelation::getRelation( *element, index, *coElement, otherIndex );
						if( index == i &&
							( rel == kmb::ElementRelation::ADJACENT ||
							  (admitAnti && rel == kmb::ElementRelation::ANTIADJACENT) ) ){
							neighbors[i] = coElement;
							faces[i] = otherIndex;
							++count;
						}
					}
					++eIter;
				}
			}
		}
		default:
			break;
	}
	return count;
}

void
kmb::NodeNeighborPtrInfo::getNodeNeighbor( kmb::nodeIdType nodeId, std::set<kmb::nodeIdType> &neighbors ) const
{
	// 周辺要素を取り出して計算する
	std::pair< NodeNeighborPtr::const_iterator, NodeNeighborPtr::const_iterator >
		eIterPair = coboundaries.equal_range( nodeId );
	NodeNeighborPtr::const_iterator eIter = eIterPair.first;
	while( eIter != eIterPair.second )
	{
		kmb::Element* element = eIter->second;
		if( element ){
			const int len = element->getNodeCount();
			// 局所座標を取得
			int nodeIndex = element->indexOf(nodeId);
			for(int i=0;i<len;++i){
				if( i != nodeIndex &&
					element->isConnected(i,nodeIndex) != 0 )
				{
					neighbors.insert(element->getNodeId(i));
				}
			}
		}
		++eIter;
	}
}

bool
kmb::NodeNeighborPtrInfo::isConnected( kmb::nodeIdType nodeId0, kmb::nodeIdType nodeId1 ) const
{
	bool retVal = false;

	// get elements around node0
	std::pair< NodeNeighborPtr::const_iterator, NodeNeighborPtr::const_iterator >
		eIterPair = coboundaries.equal_range( nodeId0 );
	NodeNeighborPtr::const_iterator eIter = eIterPair.first;
	while( eIter != eIterPair.second )
	{
		const kmb::Element* element = eIter->second;
		if( element )
		{
			int ind0 = element->indexOf(nodeId0);
			int ind1 = element->indexOf(nodeId1);
			if( element->isConnected(ind0,ind1) ){
				retVal = true;
				break;
			}
		}
		++eIter;
	}
	return retVal;
}

size_t
kmb::NodeNeighborPtrInfo::getElementCountAroundNode(nodeIdType nodeId) const
{
	return this->coboundaries.count(nodeId);
}

size_t
kmb::NodeNeighborPtrInfo::getSize(void) const
{
	return this->coboundaries.size();
}

kmb::NodeNeighborPtr::iterator
kmb::NodeNeighborPtrInfo::beginNodeIterator(void)
{
	return this->coboundaries.begin();
}

kmb::NodeNeighborPtr::iterator
kmb::NodeNeighborPtrInfo::nextNodeIterator(kmb::nodeIdType nodeId)
{
	return this->coboundaries.upper_bound(nodeId);
}

kmb::NodeNeighborPtr::iterator
kmb::NodeNeighborPtrInfo::endNodeIterator(void)
{
	return this->coboundaries.end();
}

