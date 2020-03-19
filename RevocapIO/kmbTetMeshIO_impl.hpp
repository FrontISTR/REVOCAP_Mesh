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
					for (int j = 0; j < 4; j++) {
						nodes[j] = allElements[i * 20 + j];
					}
					mesh->addElement(kmb::kTetrahedron, nodes);
				}
				else if (allElements[i * 20 + 6] == kmb::nullNodeId) {
					for (int j = 0; j < 6; j++) {
						nodes[j] = allElements[i * 20 + j];
					}
					mesh->addElement(kmb::kPrism, nodes);
				}
				else if (allElements[i * 20 + 8] == kmb::nullNodeId) {
					for (int j = 0; j < 8; j++) {
						nodes[j] = allElements[i * 20 + j];
					}
					mesh->addElement(kmb::kHexahedron, nodes);
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
				else{
					for (int j = 0; j < 20; j++) {
						nodes[j] = allElements[i * 20 + j];
					}
					mesh->addElement(kmb::kHexahedron2, nodes);
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
						for (int j = 0; j < 4; j++) {
							nodes[j] = allElements[elementId * 20 + j];
						}
						mesh->addElement(kmb::kTetrahedron, nodes);
					}
					else if (allElements[i * 20 + 6] == kmb::nullNodeId) {
						for (int j = 0; j < 6; j++) {
							nodes[j] = allElements[elementId * 20 + j];
						}
						mesh->addElement(kmb::kPrism, nodes);
					}
					else if (allElements[i * 20 + 8] == kmb::nullNodeId) {
						for (int j = 0; j < 8; j++) {
							nodes[j] = allElements[elementId * 20 + j];
						}
						mesh->addElement(kmb::kHexahedron, nodes);
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
					else {
						for (int j = 0; j < 20; j++) {
							nodes[j] = allElements[elementId * 20 + j];
						}
						mesh->addElement(kmb::kHexahedron2, nodes);
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
	if (mesh == nullptr || mesh->getNodeCount() == 0) {
		return -1;
	}
	std::cout << "TetMeshIO saveMesh" << std::endl;
	int volumeCount = 0;
	int elementCount = 0;

	kmb::bodyIdType bodyCount = static_cast<kmb::bodyIdType>(mesh->getBodyCount());
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; bodyId++) {
		int tetCount = mesh->getElementCountByType(bodyId, kmb::TETRAHEDRON);
		int tet2Count = mesh->getElementCountByType(bodyId, kmb::TETRAHEDRON2);
		int hexCount = mesh->getElementCountByType(bodyId, kmb::HEXAHEDRON);
		int hex2Count = mesh->getElementCountByType(bodyId, kmb::HEXAHEDRON2);
		int wedCount = mesh->getElementCountByType(bodyId, kmb::WEDGE);
		int eCount = tetCount + tet2Count + hexCount + hex2Count + wedCount;
		if (eCount > 0) {
			volumeCount++;
			elementCount += eCount;
		}
	}
	std::ofstream output(filename, std::ios_base::out);
	if (output.fail()) {
		std::cout << "Save Error : Can't Open File " << filename << "." << std::endl;
		return -1;
	}
	std::cout << "element count " << elementCount << std::endl;
	std::cout << "volume count " << volumeCount << std::endl;
	std::cout << "node count " << mesh->getNodeCount() << std::endl;
	output << elementCount << std::endl;
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; ++bodyId) {
		const typename MContainer::ElementContainer* body = mesh->getElementContainer(bodyId);
		if (body) {
			typename MContainer::elementIterator eIter = body->begin();
			while (eIter != body->end()) {
				switch (eIter.getType()) {
				case kmb::TETRAHEDRON:
					output <<
						eIter[0] << " " <<
						eIter[1] << " " <<
						eIter[2] << " " <<
						eIter[3] << std::endl;
					break;
				case kmb::TETRAHEDRON2:
					output <<
						eIter[0] << " " <<
						eIter[1] << " " <<
						eIter[2] << " " <<
						eIter[3] << " " <<
						eIter[6] << " " <<
						eIter[5] << " " <<
						eIter[7] << " " <<
						eIter[4] << " " <<
						eIter[9] << " " <<
						eIter[8] << std::endl;
					break;
				case kmb::WEDGE:
					output <<
						eIter[0] << " " <<
						eIter[1] << " " <<
						eIter[2] << " " <<
						eIter[3] << " " <<
						eIter[4] << " " <<
						eIter[5] << std::endl;
					break;
				case kmb::HEXAHEDRON:
					output <<
						eIter[0] << " " <<
						eIter[1] << " " <<
						eIter[2] << " " <<
						eIter[3] << " " <<
						eIter[4] << " " <<
						eIter[5] << " " <<
						eIter[6] << " " <<
						eIter[7] << std::endl;
					break;
				case kmb::HEXAHEDRON2:
					output <<
						eIter[0] << " " <<
						eIter[1] << " " <<
						eIter[2] << " " <<
						eIter[3] << " " <<
						eIter[4] << " " <<
						eIter[5] << " " <<
						eIter[6] << " " <<
						eIter[7] << " " <<
						eIter[8] << " " <<
						eIter[9] << " " <<
						eIter[10] << " " <<
						eIter[11] << " " <<
						eIter[16] << " " <<
						eIter[17] << " " <<
						eIter[18] << " " <<
						eIter[19] << " " <<
						eIter[12] << " " <<
						eIter[13] << " " <<
						eIter[14] << " " <<
						eIter[15] << std::endl;
					break;
				default:
					break;
				}
				++eIter;
			}
		}
	}
	output << mesh->getNodeCount() << std::endl;
	typename MContainer::nodeIterator nIter = mesh->beginNodeIterator();
	typename MContainer::nodeIterator nIterEnd = mesh->endNodeIterator();
	while (nIter != nIterEnd) {
		output << nIter.x() << " " << nIter.y() << " " << nIter.z() << std::endl;
		++nIter;
	}
	output << volumeCount << std::endl;
	// メッシュが保持する id ではなく、ファイルに出力した順番を出力する
	kmb::elementIdType elementId = 0;
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; ++bodyId) {
		int tetCount = mesh->getElementCountByType(bodyId, kmb::TETRAHEDRON);
		int tet2Count = mesh->getElementCountByType(bodyId, kmb::TETRAHEDRON2);
		int hexCount = mesh->getElementCountByType(bodyId, kmb::HEXAHEDRON);
		int hex2Count = mesh->getElementCountByType(bodyId, kmb::HEXAHEDRON2);
		int wedCount = mesh->getElementCountByType(bodyId, kmb::WEDGE);
		int eCount = tetCount + tet2Count + hexCount + hex2Count + wedCount;
		const typename MContainer::ElementContainer* body = mesh->getElementContainer(bodyId);
		if (body && eCount > 0) {
			output << eCount << std::endl;
			typename MContainer::elementIterator eIter = body->begin();
			while (eIter != body->end()) {
				switch (eIter.getType()) {
				case kmb::TETRAHEDRON:
				case kmb::TETRAHEDRON2:
				case kmb::WEDGE:
				case kmb::HEXAHEDRON:
				case kmb::HEXAHEDRON2:
					output << elementId << std::endl;
					++elementId;
					break;
				default:
					break;
				}
				++eIter;
			}
		}
	}
	output.close();
	return 0;
}
