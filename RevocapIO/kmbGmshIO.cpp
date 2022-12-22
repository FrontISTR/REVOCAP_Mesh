/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 2.0                          #
# Class Name : GmshIO                                                  #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2015/09/24     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/
#include "RevocapIO/kmbGmshIO.h"
#include "MeshDB/kmbMeshData.h"
#include <fstream>
#include <sstream>
#include <iostream>

kmb::GmshIO::GmshIO() : offsetNodeId(1), offsetElementId(1)
{
}

int kmb::GmshIO::loadMesh(const char* filename,kmb::MeshData* mesh)
{
	if( mesh == NULL ){
		return -1;
	}
	kmb::GmshIO::gmshFormat ver = getVersion(filename);
	std::string line;
	switch(ver){
	case kmb::GmshIO::k2_2: {
		std::cout << "load Gmsh as Ver.2.2 ASCII" << std::endl;
		std::ifstream input(filename, std::ios_base::in);
		if (input.fail()) {
			return -1;
		}
		while (!input.eof()) {
			std::getline(input, line);
			if (line.find("$Nodes") == 0) {
				readNodes_2_2(input, mesh);
			}
			else if (line.find("$Elements") == 0) {
				readElements_2_2(input, mesh);
			}
		}
		break;
	}
	case kmb::GmshIO::k2_2_Binary: {
		std::cout << "load Gmsh as Ver.2.2 Binary" << std::endl;
		std::ifstream input(filename, std::ios_base::in | std::ios_base::binary);
		if (input.fail()) {
			return -1;
		}
		while (!input.eof()) {
			std::getline(input, line);
			if (line.find("$Nodes") == 0) {
				readNodes_2_2_Binary(input, mesh);
			}
			else if (line.find("$Elements") == 0) {
				readElements_2_2_Binary(input, mesh);
			}
		}
		break;
	}
	case kmb::GmshIO::kUndefined:
		break;
	default:
		break;
	}
	return -1;
}

kmb::GmshIO::gmshFormat kmb::GmshIO::getVersion(const char* filename) const
{
	std::ifstream input(filename, std::ios_base::in|std::ios_base::binary);
	if (input.fail()) {
		return kmb::GmshIO::kUndefined;
	}
	std::string line0, line1, line2;
	std::getline( input, line0 );
	std::getline( input, line1 );
	std::getline( input, line2 );
	std::cout << line0 << std::endl;
	std::cout << line1 << std::endl;
	std::cout << line2 << std::endl;
	if( line0.find("$MeshFormat") == 0 &&
		line1.find("2.2 0") == 0 &&
		line2.find("$EndMeshFormat") == 0 ){
		return kmb::GmshIO::k2_2;
	}else
	if (line0.find("$MeshFormat") == 0 &&
		line1.find("2.2 1") == 0 ){
		return kmb::GmshIO::k2_2_Binary;
	}
	return kmb::GmshIO::kUndefined;
}

// $Nodes の次の行から
int kmb::GmshIO::readNodes_2_2(std::ifstream &input,kmb::MeshData* mesh)
{
	int nodeCount = 0;
	kmb::nodeIdType nodeId;
	double x,y,z;
	std::string line;
	if( input.eof() )
		return -1;
	std::getline( input, line );
	std::istringstream tokenizer(line);
	tokenizer >> nodeCount;
	mesh->beginNode();
	for(int i=0;i<nodeCount;++i){
		std::getline( input, line );
		tokenizer.str(line);
		tokenizer.clear();
		tokenizer >> nodeId >> x >> y >> z;
		mesh->addNodeWithId(x,y,z,nodeId-offsetNodeId);
	}
	mesh->endNode();
	std::getline( input, line );
	if( line.find("$EndNodes") != 0 ){
		return -1;
	}
	return nodeCount;
}

// $Nodes の次の行から
int kmb::GmshIO::readNodes_2_2_Binary(std::ifstream &input, kmb::MeshData* mesh)
{
	int nodeCount = 0;
	int nodeId;
	double x, y, z;
	std::string line;
	if (input.eof())
		return -1;
	std::getline(input, line);
	std::istringstream tokenizer(line);
	tokenizer >> nodeCount;
	std::cout << "node count " << nodeCount << std::endl;
	mesh->beginNode();
	for (int i = 0; i < nodeCount; ++i) {
		input.read(reinterpret_cast<char*>(&nodeId), sizeof(int));
		input.read(reinterpret_cast<char*>(&x), sizeof(double));
		input.read(reinterpret_cast<char*>(&y), sizeof(double));
		input.read(reinterpret_cast<char*>(&z), sizeof(double));
		mesh->addNodeWithId(x, y, z, nodeId - offsetNodeId);
	}
	mesh->endNode();
	std::getline(input, line);
	std::cout << line << std::endl;
	if (line.find("$EndNodes") != 0) {
		return -1;
	}
	return nodeCount;
}

// $Elements の次の行から
// 次元ごとにまとめる
int kmb::GmshIO::readElements_2_2(std::ifstream &input,kmb::MeshData* mesh)
{
	int elementCount = 0;
	kmb::elementIdType elementId;
	int etype;
	int numberOfTags;
	int tag;
	kmb::nodeIdType nodes[125];
	std::string line;
	if( input.eof() )
		return -1;
	std::getline( input, line );
	std::istringstream tokenizer(line);
	tokenizer >> elementCount;
	kmb::ElementContainer* body0=NULL;
	kmb::ElementContainer* body1=NULL;
	kmb::ElementContainer* body2=NULL;
	kmb::ElementContainer* body3=NULL;
	for(int i=0;i<elementCount;++i){
		std::getline( input, line );
		tokenizer.str(line);
		tokenizer.clear();
		tokenizer >> elementId >> etype >> numberOfTags;
		for(int j=0;j<numberOfTags;j++){
			tokenizer >> tag;
		}
		switch(etype){
		case kmb::GmshIO::kPoint:
			if(body0==NULL){
				kmb::bodyIdType bodyId=mesh->beginElement();
				mesh->endElement();
				body0=mesh->getBodyPtr(bodyId);
			}
			tokenizer >> nodes[0]; nodes[0] -= offsetNodeId;
			body0->addElement(kmb::VERTEX,nodes,elementId-offsetElementId);
			break;
		case kmb::GmshIO::kLine:
		case kmb::GmshIO::kLine3:
			if(body1==NULL){
				kmb::bodyIdType bodyId=mesh->beginElement();
				mesh->endElement();
				body1=mesh->getBodyPtr(bodyId);
			}
			switch(etype){
			case kmb::GmshIO::kLine:
				tokenizer >> nodes[0]; nodes[0] -= offsetNodeId;
				tokenizer >> nodes[1]; nodes[1] -= offsetNodeId;
				body1->addElement(kmb::SEGMENT,nodes,elementId-offsetElementId);
				break;
			case kmb::GmshIO::kLine3:
				tokenizer >> nodes[0]; nodes[0] -= offsetNodeId;
				tokenizer >> nodes[1]; nodes[1] -= offsetNodeId;
				tokenizer >> nodes[2]; nodes[2] -= offsetNodeId;
				body1->addElement(kmb::SEGMENT2,nodes,elementId-offsetElementId);
				break;
			default: break;
			}
			break;
		case kmb::GmshIO::kTriangle:
		case kmb::GmshIO::kTriangle6:
			if(body2==NULL){
				kmb::bodyIdType bodyId=mesh->beginElement();
				mesh->endElement();
				body2=mesh->getBodyPtr(bodyId);
			}
			switch(etype){
			case kmb::GmshIO::kTriangle:
				tokenizer >> nodes[0]; nodes[0] -= offsetNodeId;
				tokenizer >> nodes[1]; nodes[1] -= offsetNodeId;
				tokenizer >> nodes[2]; nodes[2] -= offsetNodeId;
				body2->addElement(kmb::TRIANGLE,nodes,elementId-offsetElementId);
				break;
			case kmb::GmshIO::kTriangle6:
				tokenizer >> nodes[0]; nodes[0] -= offsetNodeId;
				tokenizer >> nodes[1]; nodes[1] -= offsetNodeId;
				tokenizer >> nodes[2]; nodes[2] -= offsetNodeId;
				tokenizer >> nodes[3]; nodes[3] -= offsetNodeId;
				tokenizer >> nodes[4]; nodes[4] -= offsetNodeId;
				tokenizer >> nodes[5]; nodes[5] -= offsetNodeId;
				body2->addElement(kmb::TRIANGLE2,nodes,elementId-offsetElementId);
				break;
			default: break;
			}
			break;
		case kmb::GmshIO::kTetrahedron:
		case kmb::GmshIO::kTetrahedron10:
			if(body3==NULL){
				kmb::bodyIdType bodyId=mesh->beginElement();
				mesh->endElement();
				body3=mesh->getBodyPtr(bodyId);
			}
			switch(etype){
			case kmb::GmshIO::kTetrahedron:
				tokenizer >> nodes[0]; nodes[0] -= offsetNodeId;
				tokenizer >> nodes[1]; nodes[1] -= offsetNodeId;
				tokenizer >> nodes[2]; nodes[2] -= offsetNodeId;
				tokenizer >> nodes[3]; nodes[3] -= offsetNodeId;
				body3->addElement(kmb::TETRAHEDRON,nodes,elementId-offsetElementId);
				break;
			case kmb::GmshIO::kTetrahedron10:
				tokenizer >> nodes[0]; nodes[0] -= offsetNodeId;
				tokenizer >> nodes[1]; nodes[1] -= offsetNodeId;
				tokenizer >> nodes[2]; nodes[2] -= offsetNodeId;
				tokenizer >> nodes[3]; nodes[3] -= offsetNodeId;
				tokenizer >> nodes[6]; nodes[6] -= offsetNodeId;
				tokenizer >> nodes[4]; nodes[4] -= offsetNodeId;
				tokenizer >> nodes[5]; nodes[5] -= offsetNodeId;
				tokenizer >> nodes[7]; nodes[7] -= offsetNodeId;
				tokenizer >> nodes[9]; nodes[9] -= offsetNodeId;
				tokenizer >> nodes[8]; nodes[8] -= offsetNodeId;
				body3->addElement(kmb::TETRAHEDRON2,nodes,elementId-offsetElementId);
				break;
			default: break;
			}
			break;
		default: break;
		}
	}
	std::getline( input, line );
	if( line.find("$EndElements") != 0 ){
		return -1;
	}
	return elementCount;
}

// $Elements の次の行から
// 次元ごとにまとめる
int kmb::GmshIO::readElements_2_2_Binary(std::ifstream &input, kmb::MeshData* mesh)
{
	int elementCount = 0;
	std::string line;
	if (input.eof())
		return -1;
	std::getline(input, line);
	std::istringstream tokenizer(line);
	tokenizer >> elementCount;
	std::cout << "element count " << elementCount << std::endl;
	kmb::elementIdType elementId;
	int etype;
	int numberOfTags;
	int tag;
	int subCount;
	kmb::nodeIdType nodes[20];
	kmb::ElementContainer* body0 = NULL;
	kmb::ElementContainer* body1 = NULL;
	kmb::ElementContainer* body2 = NULL;
	kmb::ElementContainer* body3 = NULL;
	int count=0;
	while (count < elementCount) {
		input.read(reinterpret_cast<char*>(&etype), sizeof(int));
		input.read(reinterpret_cast<char*>(&subCount), sizeof(int));
		input.read(reinterpret_cast<char*>(&numberOfTags), sizeof(int));
		std::cout << etype << " " << subCount << " " << numberOfTags << std::endl;
		for (int j = 0; j < numberOfTags; j++) {
			input.read(reinterpret_cast<char*>(&tag), sizeof(int));
		}
		switch (etype) {
		case kmb::GmshIO::kPoint:
			if (body0 == NULL) {
				kmb::bodyIdType bodyId = mesh->beginElement();
				mesh->endElement();
				body0 = mesh->getBodyPtr(bodyId);
			}
			for (int i = 0; i < subCount; ++i) {
				input.read(reinterpret_cast<char*>(&elementId), sizeof(int));
				input.read(reinterpret_cast<char*>(nodes), sizeof(int));
				nodes[0] -= offsetNodeId;
				body0->addElement(kmb::VERTEX, nodes, elementId - offsetElementId);
				++count;
			}
		break;
		case kmb::GmshIO::kLine:
		case kmb::GmshIO::kLine3:
			if (body1 == NULL) {
				kmb::bodyIdType bodyId = mesh->beginElement();
				mesh->endElement();
				body1 = mesh->getBodyPtr(bodyId);
			}
			switch (etype) {
			case kmb::GmshIO::kLine:
				for (int i = 0; i < subCount; ++i) {
					input.read(reinterpret_cast<char*>(&elementId), sizeof(int));
					input.read(reinterpret_cast<char*>(nodes), 2*sizeof(int));
					for (int k = 0; k < 2; k++) {
						nodes[k] -= offsetNodeId;
					}
					body1->addElement(kmb::SEGMENT, nodes, elementId - offsetElementId);
					++count;
				}
				break;
			case kmb::GmshIO::kLine3:
				for (int i = 0; i < subCount; ++i) {
					input.read(reinterpret_cast<char*>(&elementId), sizeof(int));
					input.read(reinterpret_cast<char*>(nodes), 3 * sizeof(int));
					for (int k = 0; k < 3; k++) {
						nodes[k] -= offsetNodeId;
					}
					body1->addElement(kmb::SEGMENT2, nodes, elementId - offsetElementId);
					++count;
				}
				break;
			default: break;
			}
			break;
		case kmb::GmshIO::kTriangle:
		case kmb::GmshIO::kTriangle6:
			if (body2 == NULL) {
				kmb::bodyIdType bodyId = mesh->beginElement();
				mesh->endElement();
				body2 = mesh->getBodyPtr(bodyId);
			}
			switch (etype) {
			case kmb::GmshIO::kTriangle:
				for (int i = 0; i < subCount; ++i) {
					input.read(reinterpret_cast<char*>(&elementId), sizeof(int));
					input.read(reinterpret_cast<char*>(nodes), 3 * sizeof(int));
					for (int k = 0; k < 3; k++) {
						nodes[k] -= offsetNodeId;
					}
					body2->addElement(kmb::TRIANGLE, nodes, elementId - offsetElementId);
					++count;
				}
				break;
			case kmb::GmshIO::kTriangle6:
				for (int i = 0; i < subCount; ++i) {
					input.read(reinterpret_cast<char*>(&elementId), sizeof(int));
					input.read(reinterpret_cast<char*>(nodes), 6 * sizeof(int));
					for (int k = 0; k < 6; k++) {
						nodes[k] -= offsetNodeId;
					}
					body2->addElement(kmb::TRIANGLE2, nodes, elementId - offsetElementId);
					++count;
				}
				break;
			default: break;
			}
			break;
		case kmb::GmshIO::kTetrahedron:
		case kmb::GmshIO::kTetrahedron10:
			if (body3 == NULL) {
				kmb::bodyIdType bodyId = mesh->beginElement();
				mesh->endElement();
				body3 = mesh->getBodyPtr(bodyId);
			}
			switch (etype) {
			case kmb::GmshIO::kTetrahedron:
				for (int i = 0; i < subCount; ++i) {
					input.read(reinterpret_cast<char*>(&elementId), sizeof(int));
					input.read(reinterpret_cast<char*>(nodes), 4 * sizeof(int));
					for (int k = 0; k < 4; k++) {
						nodes[k] -= offsetNodeId;
					}
					body3->addElement(kmb::TETRAHEDRON, nodes, elementId - offsetElementId);
					++count;
				}
				break;
			case kmb::GmshIO::kTetrahedron10:
				for (int i = 0; i < subCount; ++i) {
					input.read(reinterpret_cast<char*>(&elementId), sizeof(int));
					input.read(reinterpret_cast<char*>(&nodes[0]), sizeof(int));
					input.read(reinterpret_cast<char*>(&nodes[1]), sizeof(int));
					input.read(reinterpret_cast<char*>(&nodes[2]), sizeof(int));
					input.read(reinterpret_cast<char*>(&nodes[3]), sizeof(int));
					input.read(reinterpret_cast<char*>(&nodes[6]), sizeof(int));
					input.read(reinterpret_cast<char*>(&nodes[5]), sizeof(int));
					input.read(reinterpret_cast<char*>(&nodes[4]), sizeof(int));
					input.read(reinterpret_cast<char*>(&nodes[7]), sizeof(int));
					input.read(reinterpret_cast<char*>(&nodes[9]), sizeof(int));
					input.read(reinterpret_cast<char*>(&nodes[8]), sizeof(int));
					for (int k = 0; k < 10; k++) {
						nodes[k] -= offsetNodeId;
					}
					body3->addElement(kmb::TETRAHEDRON2, nodes, elementId - offsetElementId);
					++count;
				}
				break;
			default: break;
			}
			break;
		default: break;
		}
	}
	std::getline(input, line);
	if (line.find("$EndElements") != 0) {
		return -1;
	}
	return elementCount;
}

int kmb::GmshIO::saveGeo(std::string filename, const kmb::MeshData* mesh)
{
	if (mesh == NULL) {
		return -1;
	}
	std::ofstream output(filename.c_str(), std::ios_base::out);
	if (output.fail()) {
		return -1;
	}
	// Point
	kmb::Point3D pt;
	kmb::MeshData::nodeIterator nIter = mesh->beginNodeIterator();
	kmb::MeshData::nodeIterator nIterEnd = mesh->endNodeIterator();
	while (nIter != nIterEnd) {
		if (nIter.getPoint(pt)) {
			output << "Point(" << nIter.getId() + offsetNodeId << ") = "
				<< "(" << pt.x() << ", " << pt.y() << ", " << pt.z() << ");" << std::endl;
		}
		++nIter;
	}
	// Line
	// node pair => line index
	// node pair (n0,n1) は nodeId の昇順 n0 < n1
	std::map< std::pair<kmb::nodeIdType, kmb::nodeIdType>, int > lines;
	int lineIndex = 0;
	kmb::bodyIdType bodyCount = mesh->getBodyCount();
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; bodyId++) {
		if (mesh->getDimension(bodyId)==2) {
			kmb::MeshData::elementIterator eIter = mesh->beginElementIterator(bodyId);
			kmb::MeshData::elementIterator eIterEnd = mesh->endElementIterator(bodyId);
			while (eIter != eIterEnd) {
				int boundaryCount = eIter.getBoundaryCount();
				for (int boundaryIndex = 0; boundaryIndex < boundaryCount; ++boundaryIndex) {
					kmb::nodeIdType n0 = eIter.getBoundaryNodeId(boundaryIndex, 0);
					kmb::nodeIdType n1 = eIter.getBoundaryNodeId(boundaryIndex, 1);
					if (n0 > n1) std::swap(n0, n1);
					std::pair<kmb::nodeIdType, kmb::nodeIdType> pair = std::make_pair(n0, n1);
					if (lines.find(pair) == lines.end()) {
						++lineIndex;
						lines.insert(std::make_pair(pair,lineIndex));
					}
				}
				++eIter;
			}
		}
	}
	std::map< std::pair<kmb::nodeIdType, kmb::nodeIdType>, int >::iterator lIter = lines.begin();
	std::map< std::pair<kmb::nodeIdType, kmb::nodeIdType>, int >::iterator lIterEnd = lines.end();
	while (lIter != lIterEnd) {
		output << "Line(" << lIter->second << ") = {"
			<< lIter->first.first + offsetNodeId << "," << lIter->first.second + offsetNodeId << "};" << std::endl;
		++lIter;
	}
	// Curve
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; bodyId++) {
		if (mesh->getDimension(bodyId) == 2) {
			kmb::MeshData::elementIterator eIter = mesh->beginElementIterator(bodyId);
			kmb::MeshData::elementIterator eIterEnd = mesh->endElementIterator(bodyId);
			while (eIter != eIterEnd) {
				output << "Curve Loop(" << eIter.getId() + offsetElementId << ") = {";
				int boundaryCount = eIter.getBoundaryCount();
				for (int boundaryIndex = 0; boundaryIndex < boundaryCount; ++boundaryIndex) {
					kmb::nodeIdType n0 = eIter.getBoundaryNodeId(boundaryIndex, 0);
					kmb::nodeIdType n1 = eIter.getBoundaryNodeId(boundaryIndex, 1);
					bool negative = false;
					if (n0 > n1) {
						std::swap(n0, n1);
						negative = true;
					}
					std::pair<kmb::nodeIdType, kmb::nodeIdType> pair = std::make_pair(n0, n1);
					int lIndex = lines.find(pair)->second;
					if (negative) lIndex *= -1;
					output << lIndex;
					if (boundaryIndex != boundaryCount - 1) {
						output << ",";
					}
				}
				output << "};" << std::endl;
				++eIter;
			}
		}
	}
	// Plane
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; bodyId++) {
		if (mesh->getDimension(bodyId) == 2) {
			kmb::MeshData::elementIterator eIter = mesh->beginElementIterator(bodyId);
			kmb::MeshData::elementIterator eIterEnd = mesh->endElementIterator(bodyId);
			while (eIter != eIterEnd) {
				output << "Plane Surface(" << eIter.getId() + offsetElementId << ") = {" << eIter.getId() + offsetElementId << "};" << std::endl;
				++eIter;
			}
		}
	}
	// Surface
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; bodyId++) {
		if (mesh->getDimension(bodyId) == 2) {
			output << "Surface Loop(" << bodyId + offsetElementId << ") = {";
			kmb::MeshData::elementIterator eIter = mesh->beginElementIterator(bodyId);
			kmb::MeshData::elementIterator eIterEnd = mesh->endElementIterator(bodyId);
			while (eIter != eIterEnd) {
				output << eIter.getId() + offsetElementId;
				++eIter;
				if (eIter != eIterEnd) {
					output << ",";
				}
			}
			output << "};" << std::endl;
		}
	}
	// Volume
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; bodyId++) {
		if (mesh->getDimension(bodyId) == 2) {
			output << "Volume(" << bodyId + offsetElementId << ") = {" << bodyId + offsetElementId << "};" << std::endl;
		}
	}
	output.close();
	return 0;
}