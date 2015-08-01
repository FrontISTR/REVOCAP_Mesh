/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : NodeNeighborInfo                                        #
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
#include <map>
#include <vector>
#include <string>

namespace kmb{

class Element;
class ElementBase;
class MeshData;
class DataBindings;
class ElementContainer;
class Face;

typedef std::multimap< kmb::nodeIdType, kmb::elementIdType > NodeNeighbor;

class NodeNeighborInfo
{
public:
	NodeNeighborInfo(void);
	virtual ~NodeNeighborInfo(void);

	void clear(void);
	bool getIgnoreOrientation(void) const;
	void setIgnoreOrientation(bool f);

	// elementId vs nodeId table 
private:
	bool append( kmb::nodeIdType nodeID, kmb::elementIdType elementID );
	bool erase( kmb::nodeIdType nodeID, kmb::elementIdType elementID );
public:
	bool appendCoboundary( kmb::elementIdType elementId, const kmb::ElementBase &element );
	bool appendCoboundary( const kmb::ElementContainer* elements );
	bool appendCoboundary( const kmb::MeshData* mesh );
	bool appendCoboundary( const kmb::DataBindings* data, const kmb::ElementContainer* body );
	bool appendCoboundary( const kmb::DataBindings* data, const kmb::MeshData* mesh );
	// erase correspondence
	bool deleteCoboundary( kmb::elementIdType elementId, const kmb::ElementBase &element );

	// ‹ß–Tî•ñAü•Óî•ñ‚Ìæ“¾
	
	// —v‘f‚ğ—^‚¦‚Ä‚»‚Ì‹ß–T—v‘f‚ğæ‚èo‚µAneighbors ‚É•Ô‚·
	// neighbors ‚ÍŒÄ‚Ño‚µ‘¤‚Å element ‚Ì‹«ŠE‚ÌŒÂ”‚¾‚¯‚ ‚ç‚©‚¶‚ß—pˆÓ‚µ‚Ä
	// ŒÄ‚Ño‚µ‘¤‚Åƒƒ‚ƒŠ‚ÌŠm•ÛE‰ğ•ú‚ğs‚¤‚±‚Æ
	// –ß‚è’l‚Í‹ß–T—v‘f‚ÌŒÂ”
	// ’ˆÓFneighbors ‚É“ü‚Á‚Ä‚¢‚é‹ß–T‚Ì‡”Ô‚ÍˆÓ–¡‚Í‚È‚¢i“r’†‚É NULL ‚Ío‚Ä‚±‚È‚¢j
	int getNeighborElements( kmb::elementIdType elementID, kmb::elementIdType *neighbors, const kmb::ElementContainer* elements ) const;
	int getNeighborElements( kmb::elementIdType elementID, kmb::elementIdType *neighbors, kmb::MeshData* mesh ) const;
	
	// —v‘f‚ğ—^‚¦‚ÄA‚»‚Ì‹«ŠE—v‘f‚ğA“o˜^‚³‚ê‚Ä‚¢‚é neighborInfo ‚©‚ç’T‚·
	// Œ©‚Â‚©‚Á‚½ŒÂ”‚ğ•Ô‚·
	int getElementBoundary( const kmb::ElementBase &element, kmb::elementIdType *boundaries, const kmb::ElementContainer* elements ) const;
	// •½–Ê—v‘f‚ğ“o˜^‚µ‚Ä‚¢‚é‚É—§‘Ì—v‘f element ‚Æ–Ê”Ô†‚ğ—^‚¦‚Ä‹«ŠE—v‘f‚ğ’T‚·
	// element ‚Í•K‚¸‚µ‚à elements ‚ÉŠÜ‚Ü‚ê‚Ä‚¢‚È‚­‚Ä‚à‚æ‚¢
	kmb::elementIdType getElementFace( const kmb::ElementBase &element, kmb::idType faceId, const kmb::ElementContainer* elements ) const;
	// —§‘Ì—v‘f‚ğ“o˜^‚µ‚Ä‚¢‚é‚É•½–Ê—v‘f‚ğ—^‚¦‚ÄAelement ‚Æ–Ê”Ô†‚ğ•Ô‚·
	// element : •½–Ê—v‘f
	// face : element ‚ğ–Ê‚É‚à‚Â‚æ‚¤‚È—§‘Ì—v‘f‚Ì Face 
	bool getFace( const kmb::ElementBase &element, kmb::Face &face, const kmb::ElementContainer* elements ) const;
	// face ‚ğ—^‚¦‚ÄA‚»‚ê‚ÉÚ‚µ‚Ä‚¢‚é Face adj ‚ğ‹‚ß‚é
	bool getAdjacentFace( const kmb::Face &face, kmb::Face &adj, const kmb::ElementContainer* elements ) const;
	bool getAdjacentFace( const kmb::Face &face, kmb::Face &adj, kmb::MeshData* mesh ) const;
public:
	// get adjacent nodes from a node
	void getNodeNeighbor( kmb::nodeIdType nodeId, std::set<kmb::nodeIdType> &neighbors, const kmb::ElementContainer* elements );
	void getNodeNeighbor( kmb::nodeIdType nodeId, std::set<kmb::nodeIdType> &neighbors, kmb::MeshData* mesh );
	// if only 2D elements are appended,
	// get sorted adjacent nodes from a node
	void getCrownNodeNeighbor
		( kmb::nodeIdType nodeID,
			std::vector<kmb::nodeIdType> &neighbors,
			const kmb::ElementContainer* elements );
	// is connected between two nodes
	bool isConnected( kmb::nodeIdType nodeId0, kmb::nodeIdType nodeId1, const kmb::ElementContainer* elements ) const;
	bool isConnected( kmb::nodeIdType nodeId0, kmb::nodeIdType nodeId1, kmb::MeshData* mesh ) const;

	// ß“_‚ğ—^‚¦‚Ä‚»‚Ìü•Ó—v‘f‚ÌŒÂ”
	size_t getElementCountAroundNode(nodeIdType nodeId) const;
	// ‚·‚×‚Ä‚Ì‹ß–Tî•ñ‚ÌŒÂ”
	size_t getSize(void) const;

	// ß“_‚ğ—^‚¦‚Ä‚»‚Ì‹ß–T—v‘f‚Ì Iterator ‚ğ—^‚¦‚é
	NodeNeighbor::iterator beginIteratorAt(kmb::nodeIdType nodeID){
		return coboundaries.lower_bound(nodeID);
	};
	NodeNeighbor::iterator endIteratorAt(kmb::nodeIdType nodeID){
		return coboundaries.upper_bound(nodeID);
	};
	NodeNeighbor::const_iterator beginIteratorAt(kmb::nodeIdType nodeID) const{
		return coboundaries.lower_bound(nodeID);
	};
	NodeNeighbor::const_iterator endIteratorAt(kmb::nodeIdType nodeID) const{
		return coboundaries.upper_bound(nodeID);
	};

	// ß“_‚ğ—^‚¦‚Ä‚»‚Ìü•Ó—v‘f‚ÌW‡‚ğ•Ô‚·
	void getSurroundingElements(kmb::nodeIdType nodeId, std::vector<kmb::elementIdType> &elements) const;
	// given two nodes, return elements which have both nodes
	void getSurroundingElementsOf2Node(kmb::nodeIdType nodeId0, kmb::nodeIdType nodeId1, std::vector<kmb::elementIdType> &elements) const;

	// ß“_‚ğ—^‚¦‚Ä”½Œv‰ñ‚è‚ÉÚ‚µ‚Ä‚¢‚é•½–Ê—v‘f‚ğ•Ô‚·
	// •½–Ê—v‘f‚Ìê‡‚Ì‚İ
	kmb::elementIdType getNextElementAroundNode( kmb::ElementContainer* triangles, kmb::nodeIdType nodeID, kmb::elementIdType elemID, bool clockwise=false) const;

	// ü•ª—v‘f‚Ìê‡‚Ì‚İ
	// ’[“_‚ğ‹‚ß‚é
	void getEndsOfEdge( const kmb::ElementContainer* edges, kmb::nodeIdType& firstID, kmb::nodeIdType& endID ) const;

	NodeNeighbor::iterator beginNodeIterator(void);
	NodeNeighbor::iterator endNodeIterator(void);
private:
	NodeNeighbor coboundaries;
	bool admitAnti;
};

}
