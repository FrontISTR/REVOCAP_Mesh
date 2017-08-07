#pragma once
#include "RevocapIO/kmbTripatchIO.h"
#include "Geometry/kmbIdTypes.h"
#include "MeshDB/kmbElement.h"

#include <fstream>
#include <sstream>
#include <iomanip>

template<typename MContainer>
int kmb::TripatchIO::loadPatch(std::string filename, MContainer* mesh)
{
	if( mesh == NULL ){
		return -1;
	}
	std::ifstream input( filename.c_str(), std::ios_base::in );
	if( input.fail() ){
		return -1;
	}
	std::string line;
	unsigned int nodeCount = 0;
	unsigned int dummy;
	unsigned int bodyCount = 0;
	{
		std::getline( input, line );
		std::istringstream tokenizer(line);
		tokenizer >> nodeCount >> dummy >> bodyCount;
	}
	size_t i;
	double x=0.0,y=0.0,z=0.0;
	mesh->beginNode( nodeCount );
	for(i = 0;i<nodeCount;++i){
		std::getline( input, line );
		std::istringstream tokenizer(line);
		tokenizer >> x >> y >> z;
		mesh->addNode(x,y,z);
	}
	mesh->endNode();
	unsigned int elementCount = 0;
	kmb::nodeIdType nodes[3] = {kmb::nullNodeId,kmb::nullNodeId,kmb::nullNodeId};
	for(unsigned int j=0;j<bodyCount;++j){
		std::getline( input, line );
		std::istringstream tokenizer(line);
		tokenizer >> elementCount >> dummy >> dummy;
		mesh->beginElement( elementCount );
		for(i = 0;i<elementCount;++i){
			std::getline( input, line );
			tokenizer.str(line);
			tokenizer.clear();
			tokenizer >> nodes[0] >> nodes[1] >> nodes[2];
			mesh->addElement( kmb::TRIANGLE, nodes );
		}
		mesh->endElement();
	}
	input.close();
	return 0;
}

template<typename MContainer>
int kmb::TripatchIO::savePatch(std::string filename, const MContainer* mesh)
{
	if( mesh == NULL ){
		return -1;
	}
	int patchCount = 0;
	kmb::bodyIdType bodyCount = mesh->getBodyCount();
	for(kmb::bodyIdType bodyId = 0;bodyId<bodyCount;++bodyId){
		size_t elemCount = mesh->getElementCount(bodyId);
		size_t triCount = mesh->getElementCountByType(bodyId,kmb::kTriangle);
		if( elemCount == triCount && elemCount > 0 ){
			++patchCount;
		}
	}
	std::ofstream output( filename.c_str(), std::ios_base::out );
	if( output.fail() ){
		return -1;
	}
	output << mesh->getNodeCount() << " " << 0 << " " << patchCount << std::endl;
	typename MContainer::nodeIterator nIter = mesh->beginNodeIterator();
	typename MContainer::nodeIterator nIterEnd = mesh->endNodeIterator();
	while( nIter != nIterEnd ){
		output << std::setprecision(8) << nIter.x() << " " << nIter.y() << " " << nIter.z() << std::endl;
		++nIter;
	}
	size_t elementCount = 0;
	for(kmb::bodyIdType bodyId = 0;bodyId<bodyCount;++bodyId){
		size_t elemCount = mesh->getElementCount(bodyId);
		size_t triCount = mesh->getElementCountByType(bodyId,kmb::kTriangle);
		if( elemCount == triCount && elemCount > 0 ){
			output << elemCount << " " << 0 << " " << 0 << std::endl;
			typename MContainer::elementIterator eIter = mesh->beginElementIterator(bodyId);
			typename MContainer::elementIterator eIterEnd = mesh->endElementIterator(bodyId);
			while( eIter != eIterEnd ){
				output <<
					eIter[0] << " " <<
					eIter[1] << " " <<
					eIter[2] << std::endl;
				++eIter;
			}
		}
	}
	return 0;
}
