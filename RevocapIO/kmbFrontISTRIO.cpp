/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.7                          #
# Class Name : FrontSTRIO                                              #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2016/03/22     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/

#include "RevocapIO/kmbFrontISTRIO.h"
#include "Common/kmbEnvInfo.h"
#include "Common/kmbContainer.h"
#include "MeshDB/kmbMeshData.h"
#include <fstream>
#include <limits>
#include <cmath>

// face table converter REVO => Fistr
int kmb::FrontISTRIO::tetFmap[] = {3,4,2,1};
int kmb::FrontISTRIO::wedFmap[] = {1,2,3,4,5};
int kmb::FrontISTRIO::hexFmap[] = {1,2,3,4,5,6};
int kmb::FrontISTRIO::pyrFmap[] = {2,3,4,5,1};

// face table converter Fistr => REVO
int kmb::FrontISTRIO::tetRmap[] = {3,2,0,1};
int kmb::FrontISTRIO::wedRmap[] = {0,1,2,3,4};
int kmb::FrontISTRIO::hexRmap[] = {0,1,2,3,4,5};
int kmb::FrontISTRIO::pyrRmap[] = {4,0,1,2,3};

kmb::FrontISTRIO::FrontISTRIO()
	: offsetNodeId(1)
	, offsetElementId(1)
	, nodeCount(0)
	, elementCount(0)
{
}

kmb::elementIdType kmb::FrontISTRIO::getOffsetElementId(void) const
{
	return offsetElementId;
}

void kmb::FrontISTRIO::setOffsetElementId(kmb::elementIdType offset)
{
	offsetElementId = offset;
}

void kmb::FrontISTRIO::count(std::string filename)
{
	nodeCount = 0;
	elementCount = 0;
	std::ifstream input(filename.c_str(), std::ios_base::in);
	if (input.fail()) {
		return;
	}
	std::string line;
	std::getline(input, line);
	while (!input.eof()) {
		if (line.find("!NODE") == 0) {
			while (!input.eof()) {
				std::getline(input, line);
				if (line.find("!") == 0) {
					break;
				}
				nodeCount++;
			}
		}
		else if (line.find("!ELEMENT") == 0) {
			while (!input.eof()) {
				std::getline(input, line);
				if (line.find("!") == 0) {
					break;
				}
				elementCount++;
			}
		}
	}
	input.close();
}

// template<typename MContainer>
kmb::elementType kmb::FrontISTRIO::getRevocapType(int etype)
{
	switch (etype)
	{
	case 111:	return kmb::SEGMENT;
	case 112:	return kmb::SEGMENT2;
	case 231:	return kmb::TRIANGLE;
	case 232:	return kmb::TRIANGLE2;
	case 241:	return kmb::QUAD;
	case 242:	return kmb::QUAD2;
	case 341:	return kmb::TETRAHEDRON;
	case 342:	return kmb::TETRAHEDRON2;
	case 351:	return kmb::WEDGE;
	case 352:	return kmb::WEDGE2;
	case 361:	return kmb::HEXAHEDRON;
	case 362:	return kmb::HEXAHEDRON2;
	case 371:	return kmb::PYRAMID;
	case 372:	return kmb::PYRAMID2;
	case 731:	return kmb::TRIANGLE;
	case 732:	return kmb::TRIANGLE2;
	case 741:	return kmb::QUAD;
	case 742:	return kmb::QUAD2;
	}
	return kmb::UNKNOWNTYPE;
}


