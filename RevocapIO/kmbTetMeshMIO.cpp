/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : TetMeshMIO                                              #
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
#include "RevocapIO/kmbTetMeshMIO.h"
#include "MeshDB/kmbMeshData.h"
#include "MeshDB/kmbElementContainerArray.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

namespace kmb {

	template<>
	int TetMeshMIO::loadFromFile(const char* filename, kmb::MeshData* mesh)
	{
		if (mesh == NULL) {
			return -1;
		}
		else {
			kmb::nodeIdType tmp[20];
			kmb::nodeIdType ary[20];
			std::fill(tmp, tmp + 20, kmb::nullNodeId);
			std::fill(ary, ary + 20, kmb::nullNodeId);
			std::ifstream input(filename, std::ios_base::in);
			if (input.fail()) {
				return -1;
			}
			size_t i;
			size_t count = 0;
			std::string line;
			nodeIdType nodeId;
			std::getline(input, line);
			size_t elementCount = atol(line.c_str());
			mesh->beginElement(elementCount);
			for (i = 0; i < elementCount; ++i) {
				count = 0;
				std::getline(input, line);
				std::istringstream tokenizer(line);
				while (tokenizer >> nodeId) {
					if (count < 20) {
						tmp[count] = nodeId;
					}
					++count;
				}
				switch (count) {
				case 4:
					mesh->addElement(kmb::TETRAHEDRON, tmp);
					break;
				case 10:
					ary[0] = tmp[0];
					ary[1] = tmp[1];
					ary[2] = tmp[2];
					ary[3] = tmp[3];
					ary[6] = tmp[4]; // 01
					ary[5] = tmp[5]; // 02
					ary[7] = tmp[6]; // 03
					ary[4] = tmp[7]; // 12
					ary[9] = tmp[8]; // 23
					ary[8] = tmp[9]; // 13
					mesh->addElement(kmb::TETRAHEDRON2, ary);
					break;
				case 6:
					mesh->addElement(kmb::WEDGE, tmp);
					break;
				case 8:
					mesh->addElement(kmb::HEXAHEDRON, tmp);
					break;
				case 20:
					ary[0] = tmp[0];
					ary[1] = tmp[1];
					ary[2] = tmp[2];
					ary[3] = tmp[3];
					ary[4] = tmp[4];
					ary[5] = tmp[5];
					ary[6] = tmp[6];
					ary[7] = tmp[7];
					ary[8] = tmp[8];
					ary[9] = tmp[9];
					ary[10] = tmp[10];
					ary[11] = tmp[11];
					ary[16] = tmp[12];
					ary[17] = tmp[13];
					ary[18] = tmp[14];
					ary[19] = tmp[15];
					ary[12] = tmp[16];
					ary[13] = tmp[17];
					ary[14] = tmp[18];
					ary[15] = tmp[19];
					mesh->addElement(kmb::HEXAHEDRON2, ary);
					break;
				default:
					break;
				}
			}
			mesh->endElement();
			unsigned int nodeCount = 0;
			double x = 0.0, y = 0.0, z = 0.0;
			input >> nodeCount;
			mesh->beginNode(nodeCount);
			for (i = 0; i < nodeCount; ++i) {
				input >> x >> y >> z;
				mesh->addNode(x, y, z);
			}
			mesh->endNode();
			unsigned int bodyCount = 0;
			kmb::elementIdType elementId = kmb::Element::nullElementId;
			input >> bodyCount;
			for (i = 0; i < bodyCount; ++i) {
				input >> elementCount;
				if (i > 0) {
					kmb::bodyIdType bodyId = mesh->beginElement(elementCount);
					mesh->endElement();
					// move するので offset しない
					// offset してから元からある要素を move しようとすると
					// 負の添え字に代入することになる
					mesh->getBodyPtr(bodyId)->setOffsetId(0);
				}
				for (unsigned int j = 0; j < elementCount; ++j) {
					input >> elementId;
					mesh->moveElement(0, static_cast<kmb::bodyIdType>(i), elementId);
				}
			}
			input.close();
			return 0;
		}
	}

	template<>
	int TetMeshMIO::saveToFile(const char* filename, const kmb::MeshData* mesh)
	{
		if (mesh == NULL || !mesh->getNodes()) {
			return -1;
		}
		else {
			std::ofstream output(filename, std::ios_base::out);
			if (output.fail()) {
				return -1;
			}
			size_t elementCount = 0;
			size_t volumeCount = 0;
			kmb::bodyIdType bodyCount = mesh->getBodyCount();
			size_t* elementCountEach = new size_t[bodyCount];
			for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; ++bodyId) {
				const kmb::Body* body = mesh->getBodyPtr(bodyId);
				elementCountEach[bodyId] = 0;
				if (body && body->getCount() > 0) {
					elementCountEach[bodyId] += body->getCountByType(kmb::TETRAHEDRON);
					elementCountEach[bodyId] += body->getCountByType(kmb::TETRAHEDRON2);
					elementCountEach[bodyId] += body->getCountByType(kmb::HEXAHEDRON);
					elementCountEach[bodyId] += body->getCountByType(kmb::HEXAHEDRON2);
					elementCountEach[bodyId] += body->getCountByType(kmb::WEDGE);
					elementCount += elementCountEach[bodyId];
					if (elementCountEach[bodyId] > 0) {
						++volumeCount;
					}
				}
			}
			output << elementCount << std::endl;
			for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; ++bodyId) {
				const kmb::Body* body = mesh->getBodyPtr(bodyId);
				if (body) {
					kmb::ElementContainer::const_iterator eIter = body->begin();
					while (eIter != body->end()) {
						switch (eIter.getType()) {
						case kmb::TETRAHEDRON:
							output <<
								eIter.getNodeId(0) << " " <<
								eIter.getNodeId(1) << " " <<
								eIter.getNodeId(2) << " " <<
								eIter.getNodeId(3) << std::endl;
							break;
						case kmb::TETRAHEDRON2:
							output <<
								eIter.getNodeId(0) << " " <<
								eIter.getNodeId(1) << " " <<
								eIter.getNodeId(2) << " " <<
								eIter.getNodeId(3) << " " <<
								eIter.getNodeId(6) << " " <<
								eIter.getNodeId(5) << " " <<
								eIter.getNodeId(7) << " " <<
								eIter.getNodeId(4) << " " <<
								eIter.getNodeId(9) << " " <<
								eIter.getNodeId(8) << std::endl;
							break;
						case kmb::WEDGE:
							output <<
								eIter.getNodeId(0) << " " <<
								eIter.getNodeId(1) << " " <<
								eIter.getNodeId(2) << " " <<
								eIter.getNodeId(3) << " " <<
								eIter.getNodeId(4) << " " <<
								eIter.getNodeId(5) << std::endl;
							break;
						case kmb::HEXAHEDRON:
							output <<
								eIter.getNodeId(0) << " " <<
								eIter.getNodeId(1) << " " <<
								eIter.getNodeId(2) << " " <<
								eIter.getNodeId(3) << " " <<
								eIter.getNodeId(4) << " " <<
								eIter.getNodeId(5) << " " <<
								eIter.getNodeId(6) << " " <<
								eIter.getNodeId(7) << std::endl;
							break;
						case kmb::HEXAHEDRON2:
							output <<
								eIter.getNodeId(0) << " " <<
								eIter.getNodeId(1) << " " <<
								eIter.getNodeId(2) << " " <<
								eIter.getNodeId(3) << " " <<
								eIter.getNodeId(4) << " " <<
								eIter.getNodeId(5) << " " <<
								eIter.getNodeId(6) << " " <<
								eIter.getNodeId(7) << " " <<
								eIter.getNodeId(8) << " " <<
								eIter.getNodeId(9) << " " <<
								eIter.getNodeId(10) << " " <<
								eIter.getNodeId(11) << " " <<
								eIter.getNodeId(16) << " " <<
								eIter.getNodeId(17) << " " <<
								eIter.getNodeId(18) << " " <<
								eIter.getNodeId(19) << " " <<
								eIter.getNodeId(12) << " " <<
								eIter.getNodeId(13) << " " <<
								eIter.getNodeId(14) << " " <<
								eIter.getNodeId(15) << std::endl;
							break;
						default:
							break;
						}
						++eIter;
					}
				}
			}
			output << mesh->getNodeCount() << std::endl;
			double x, y, z;
			kmb::Point3DContainer::const_iterator nIter = mesh->getNodes()->begin();
			kmb::Point3DContainer::const_iterator nIterEnd = mesh->getNodes()->end();
			while (nIter != nIterEnd) {
				if (nIter.getXYZ(x, y, z)) {
					output << x << " " << y << " " << z << std::endl;
				}
				++nIter;
			}
			output << volumeCount << std::endl;
			// メッシュが保持する id ではなく、ファイルに出力した順番を出力する
			kmb::elementIdType elementId = 0;
			for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; ++bodyId) {
				const kmb::Body* body = mesh->getBodyPtr(bodyId);
				if (body && elementCountEach[bodyId] > 0) {
					output << elementCountEach[bodyId] << std::endl;
					kmb::ElementContainer::const_iterator eIter = body->begin();
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
			delete[] elementCountEach;
			output.close();
		}
		return 0;
	}

}