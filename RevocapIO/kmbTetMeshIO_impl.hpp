#pragma once

#include "RevocapIO/kmbTetMeshIO.h"
#include "Geometry/kmbIdTypes.h"
#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElement.h"

#include <fstream>
#include <sstream>
#include <iostream>

template<typename MContainer>
int kmb::TetMeshIO::loadMesh(std::string filename, MContainer* mesh)
{
	std::cout << "TetMeshIO loadMesh" << std::endl;
	if (mesh == NULL) {
		return -1;
	}
	std::ifstream input(filename.c_str(), std::ios_base::in);
	if (input.fail()) {
		std::cout << "Load Error : Can't Open File " << filename << "." << std::endl;
		return -1;
	}
	std::string line;
	kmb::nodeIdType* allElements = NULL;
	unsigned int allElementCount = 0;
	{
		std::getline(input, line);
		std::istringstream tokenizer(line);
		tokenizer >> allElementCount;
		// 要素当たりの節点数の最大 = 20
		allElements = new kmb::nodeIdType[20 * allElementCount];
		std::fill_n(allElements, 20 * allElementCount, kmb::nullNodeId);
		for (unsigned int i = 0; i < allElementCount; ++i) {
			std::getline(input, line);
			tokenizer.str(line);
			tokenizer.clear();
			int j = 0;
			while (tokenizer >> allElements[i*20+j]) {
				j++;
			}
		}
	}
	{
		unsigned int nodeCount = 0;
		std::getline(input, line);
		std::istringstream tokenizer(line);
		tokenizer >> nodeCount;
		double x = 0.0, y = 0.0, z = 0.0;
		mesh->beginNode(nodeCount);
		for (unsigned int i = 0; i < nodeCount; ++i) {
			std::getline(input, line);
			std::istringstream tokenizer(line);
			tokenizer >> x >> y >> z;
			mesh->addNode(x, y, z);
		}
		mesh->endNode();
	}
	{
		kmb::bodyIdType bodyCount = 0;
		kmb::elementIdType elementCount = 0;
		kmb::nodeIdType nodes[20];
		std::fill_n(nodes, 20, kmb::nullNodeId);
		input >> bodyCount;
		std::cout << "body count = " << bodyCount << std::endl;
		kmb::elementIdType elementId;
		if (bodyCount == 0) {
			mesh->beginElement(allElementCount);
			for (unsigned int i = 0; i < allElementCount; ++i) {
				if (allElements[i * 20 + 4] == kmb::nullNodeId) {
					nodes[0] = allElements[i * 20];
					nodes[1] = allElements[i * 20 + 1];
					nodes[2] = allElements[i * 20 + 2];
					nodes[3] = allElements[i * 20 + 3];
					mesh->addElement(kmb::kTetrahedron, nodes);
				}
				else if (allElements[i * 20 + 10] == kmb::nullNodeId) {
					nodes[0] = allElements[i * 20];
					nodes[1] = allElements[i * 20 + 1];
					nodes[2] = allElements[i * 20 + 2];
					nodes[3] = allElements[i * 20 + 3];
					nodes[6] = allElements[i * 20 + 4];
					nodes[5] = allElements[i * 20 + 5];
					nodes[7] = allElements[i * 20 + 6];
					nodes[4] = allElements[i * 20 + 7];
					nodes[9] = allElements[i * 20 + 8];
					nodes[8] = allElements[i * 20 + 9];
					mesh->addElement(kmb::kTetrahedron2, nodes);
				}
			}
			mesh->endElement();
		}
		else {
			for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; ++bodyId) {
				input >> elementCount;
				std::cout << "element count = " << elementCount << std::endl;
				mesh->beginElement(elementCount);
				for (int i = 0; i < elementCount; ++i) {
					input >> elementId;
					if (allElements[elementId * 20 + 4] == kmb::nullNodeId) {
						nodes[0] = allElements[elementId * 20];
						nodes[1] = allElements[elementId * 20 + 1];
						nodes[2] = allElements[elementId * 20 + 2];
						nodes[3] = allElements[elementId * 20 + 3];
						mesh->addElement(kmb::kTetrahedron, nodes);
					}
					else if (allElements[elementId * 20 + 10] == kmb::nullNodeId) {
						nodes[0] = allElements[elementId * 20];
						nodes[1] = allElements[elementId * 20 + 1];
						nodes[2] = allElements[elementId * 20 + 2];
						nodes[3] = allElements[elementId * 20 + 3];
						nodes[6] = allElements[elementId * 20 + 4];
						nodes[5] = allElements[elementId * 20 + 5];
						nodes[7] = allElements[elementId * 20 + 6];
						nodes[4] = allElements[elementId * 20 + 7];
						nodes[9] = allElements[elementId * 20 + 8];
						nodes[8] = allElements[elementId * 20 + 9];
						mesh->addElement(kmb::kTetrahedron2, nodes);
					}
				}
				mesh->endElement();
			}
		}
	}
	delete[] allElements;
	input.close();
	return 0;
}

template<typename MContainer>
int kmb::TetMeshIO::saveMesh(std::string filename, const MContainer* mesh)
{
	std::cout << "TetMeshIO saveMesh" << std::endl;
	kmb::bodyIdType bodyCount = static_cast<kmb::bodyIdType>(mesh->getBodyCount());
	std::cout << "body count = " << bodyCount << std::endl;
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; bodyId++) {
		std::cout << "element count = " << mesh->getElementCount(bodyId) << std::endl;
	}
	return 0;
}
