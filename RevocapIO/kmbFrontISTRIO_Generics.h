#pragma once

#include "RevocapIO/kmbFrontISTRIO.h"
#include "RevocapIO/kmbRevocapIOUtils.h"
#include "MeshDB/kmbFace.h"
#include <fstream>
#include <sstream>
#include <iomanip>

template<typename MContainer>
int kmb::FrontISTRIO::loadMesh(std::string filename, MContainer* mesh)
{
	if (mesh == NULL) {
		return -1;
	}
	std::ifstream input(filename.c_str(), std::ios_base::in);
	if (input.fail()) {
		return -1;
	}
	std::stringstream ss;
	std::string line;
	char comma;  // for comma
	std::getline(input, line);
	while (!input.eof()) {
		if (line.find("!HEADER") == 0) {
			while (!input.eof()) {
				std::getline(input, line);
				if (line.find("!") == 0) {
					break;
				}
			}
		}
		else if (line.find("!NODE") == 0) {
			size_t nodeCount = static_cast<size_t>(atoi(kmb::RevocapIOUtils::getValue(line, "NUM").c_str()));
			std::string name = kmb::RevocapIOUtils::getValue(line, "NGRP");
			typename MContainer::DataContainer* data = NULL;
			if (name.length() > 0) {
				data = mesh->getData(name.c_str());
			}
			kmb::nodeIdType nodeId = kmb::nullNodeId;
			double x = 0.0, y = 0.0, z = 0.0;
			mesh->beginNode(nodeCount);
			while (!input.eof()) {
				std::getline(input, line);
				if (line.find("!") == 0) {
					break;
				}
				else {
					std::istringstream tokenizer(line);
					tokenizer >> nodeId >> comma >> x >> comma >> y >> comma >> z;
					mesh->addNodeWithId(x, y, z, nodeId - offsetNodeId);
					if (data) {
						data->addId(nodeId - offsetNodeId);
					}
				}
			}
			mesh->endNode();
		}
		else if (line.find("!ELEMENT") == 0) {
			std::string name = kmb::RevocapIOUtils::getValue(line, "EGRP");
			ss << kmb::RevocapIOUtils::getValue(line, "TYPE");
			int etypeHec;
			ss >> etypeHec;
			kmb::elementType etype = kmb::FrontISTRIO::getRevocapType(etypeHec);
			if (etype != kmb::UNKNOWNTYPE) {
				kmb::bodyIdType bodyId = mesh->beginElement();
				int len = kmb::Element::getNodeCount(etype);
				kmb::elementIdType elementId = kmb::Element::nullElementId;
				kmb::nodeIdType* nodeTable = new kmb::nodeIdType[len];
				char comma;  // for comma
				while (!input.eof()) {
					std::getline(input, line);
					if (line.find("!") == 0) {
						break;
					}
					else {
						std::istringstream tokenizer(line);
						tokenizer >> elementId >> comma;
						for (int i = 0; i < len; ++i) {
							tokenizer >> nodeTable[i] >> comma;
							nodeTable[i] -= offsetNodeId;
						}
						mesh->addElementWithId(etype, nodeTable, elementId - this->offsetElementId);
					}
				}
				delete[] nodeTable;
				mesh->endElement();
				mesh->setBodyName(bodyId, name);
			}
			else {
				std::getline(input, line);
			}
		}
		else if (line.find("!NGROUP") == 0) {
			std::string name = kmb::RevocapIOUtils::getValue(line, "NGRP");
			typename MContainer::DataContainer* data = NULL;
			data = mesh->getData(name.c_str());
			if (data) {
				kmb::nodeIdType nodeId = kmb::nullNodeId;
				std::istringstream tokenLine;
				std::istringstream tokenDecimal;
				std::string decimal;
				while (std::getline(input, line)) {
					if (line.find("!") == 0) {
						break;
					}
					tokenLine.str(line);
					tokenLine.clear();
					while (std::getline(tokenLine, decimal, ',')) {
						tokenDecimal.str(decimal);
						tokenDecimal.clear();
						tokenDecimal >> nodeId;
						data->addId(nodeId - this->offsetNodeId);
					}
				}
			}
			else {
				std::getline(input, line);
			}
		}
		/*
		else if (line.find("!EGROUP") == 0) {
		std::string name = kmb::RevocapIOUtils::getValue(line, "EGRP");
		kmb::DataBindings* data = NULL;
		if (dataFlag) {
		data = mesh->getDataBindingsPtr(name.c_str());
		}
		if (data) {
		kmb::elementIdType elementId = kmb::Element::nullElementId;
		std::istringstream tokenLine;
		std::istringstream tokenDecimal;
		std::string decimal;
		while (std::getline(input, line)) {
		if (line.find("!") == 0) {
		break;
		}
		tokenLine.str(line);
		tokenLine.clear();
		while (std::getline(tokenLine, decimal, ',')) {
		tokenDecimal.str(decimal);
		tokenDecimal.clear();
		tokenDecimal >> elementId;
		data->addId(elementId - this->offsetElementId);
		}
		}
		}
		else {
		std::getline(input, line);
		}
		}
		else if (line.find("!SGROUP") == 0) {
		std::string name = kmb::RevocapIOUtils::getValue(line, "SGRP");
		kmb::DataBindings* data = NULL;
		if (dataFlag) {
		data = mesh->getDataBindingsPtr(name.c_str());
		}
		if (data) {
		kmb::elementIdType elementId = kmb::Element::nullElementId;
		kmb::idType localId = kmb::nullId;
		kmb::Face f;
		std::istringstream tokenLine;
		std::istringstream tokenDecimal;
		std::string decimal;
		while (std::getline(input, line)) {
		if (line.find("!") == 0) {
		break;
		}
		tokenLine.str(line);
		tokenLine.clear();
		while (!tokenLine.eof()) {
		std::getline(tokenLine, decimal, ',');
		tokenDecimal.str(decimal);
		tokenDecimal.clear();
		tokenDecimal >> elementId;
		std::getline(tokenLine, decimal, ',');
		tokenDecimal.str(decimal);
		tokenDecimal.clear();
		tokenDecimal >> localId;
		kmb::ElementContainer::const_iterator eIter = mesh->findElement(elementId - this->offsetElementId);
		switch (eIter.getType())
		{
		case kmb::TETRAHEDRON:
		case kmb::TETRAHEDRON2:
		f.setId(elementId - this->offsetElementId, tetRmap[localId - 1]);
		data->addId(f);
		break;
		case kmb::WEDGE:
		case kmb::WEDGE2:
		f.setId(elementId - this->offsetElementId, wedRmap[localId - 1]);
		data->addId(f);
		break;
		case kmb::HEXAHEDRON:
		case kmb::HEXAHEDRON2:
		f.setId(elementId - this->offsetElementId, hexRmap[localId - 1]);
		data->addId(f);
		break;
		case kmb::PYRAMID:
		case kmb::PYRAMID2:
		f.setId(elementId - this->offsetElementId, pyrRmap[localId - 1]);
		data->addId(f);
		break;
		default:
		// findElement でエラーの時もここを通る
		break;
		}
		}
		}
		}
		else {
		std::getline(input, line);
		}
		}
		*/
		else {
			std::getline(input, line);
		}
	}
	input.close();
	return 0;
}

template<typename MContainer>
int kmb::FrontISTRIO::saveMesh(std::string filename, const MContainer* mesh)
{
	if (mesh == NULL) {
		return -1;
	}
	std::ofstream output(filename.c_str(), std::ios_base::out);
	if (output.fail()) {
		return -1;
	}
	output << "!HEADER, VER=3" << std::endl;
	output << " HECMW_MSH File generated by REVOCAP" << std::endl;
	output << "!NODE" << std::endl;
	kmb::Point3D pt;
	typename MContainer::nodeIterator nIter = mesh->beginNodeIterator();
	typename MContainer::nodeIterator nIterEnd = mesh->endNodeIterator();
	output.setf(std::ios::fixed);
	while (nIter != nIterEnd) {
		if (nIter.getPoint(pt)) {
			output << std::setw(9) << nIter.getId() + offsetNodeId << ", "
				<< std::scientific
				<< std::setprecision(8)
				<< pt.x() << ", " << pt.y() << ", " << pt.z() << std::endl;
		}
		++nIter;
	}
	kmb::bodyIdType bodyCount = static_cast<kmb::bodyIdType>(mesh->getBodyCount());
	std::cout << "Body Count = " << bodyCount << std::endl;
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; bodyId++) {
		const typename MContainer::ElementContainer* body = mesh->getElementContainer(bodyId);
		if (body == NULL) continue;
		std::string name = body->getName();
		std::cout << "Name = " << name << std::endl;
		std::cout << "Element Count = " << body->getCount() << std::endl;
		size_t tetNum = body->getCountByType(kmb::kTetrahedron);
		std::cout << "tet = " << tetNum << std::endl;
		if (tetNum > 0) {
			output << "!ELEMENT, TYPE=341, EGRP=" << name << std::endl;
			typename MContainer::elementIterator eIter = body->begin();
			typename MContainer::elementIterator eIterEnd = body->end();
			while (eIter != eIterEnd) {
				if (eIter.getType() == kmb::kTetrahedron) {
					output << std::setw(9) << eIter.getId() + offsetElementId << ", " <<
						eIter[0] + offsetNodeId << ", " <<
						eIter[1] + offsetNodeId << ", " <<
						eIter[2] + offsetNodeId << ", " <<
						eIter[3] + offsetNodeId << std::endl;
				}
				++eIter;
			}
		}
		size_t tet2Num = body->getCountByType(kmb::kTetrahedron2);
		std::cout << "tet2 = " << tet2Num << std::endl;
		if (tet2Num > 0) {
			output << "!ELEMENT, TYPE=342, EGRP=" << name << std::endl;
			typename MContainer::elementIterator eIter = body->begin();
			typename MContainer::elementIterator eIterEnd = body->end();
			while (eIter != eIterEnd) {
				if (eIter.getType() == kmb::kTetrahedron2) {
					output << std::setw(9) << eIter.getId() + offsetElementId << ", " <<
						eIter[0] + offsetNodeId << ", " <<
						eIter[1] + offsetNodeId << ", " <<
						eIter[2] + offsetNodeId << ", " <<
						eIter[3] + offsetNodeId << ", " <<
						eIter[4] + offsetNodeId << ", " <<
						eIter[5] + offsetNodeId << ", " <<
						eIter[6] + offsetNodeId << ", " <<
						eIter[7] + offsetNodeId << ", " <<
						eIter[8] + offsetNodeId << ", " <<
						eIter[9] + offsetNodeId << std::endl;
				}
				++eIter;
			}
		}
		size_t hexaNum = body->getCountByType(kmb::kHexahedron);
		std::cout << "hexa = " << hexaNum << std::endl;
		if (hexaNum > 0) {
			output << "!ELEMENT, TYPE=361, EGRP=" << name << std::endl;
			typename MContainer::elementIterator eIter = body->begin();
			typename MContainer::elementIterator eIterEnd = body->end();
			while (eIter != eIterEnd) {
				if (eIter.getType() == kmb::kHexahedron) {
					output << std::setw(9) << eIter.getId() + offsetElementId << ", " <<
						eIter[0] + offsetNodeId << ", " <<
						eIter[1] + offsetNodeId << ", " <<
						eIter[2] + offsetNodeId << ", " <<
						eIter[3] + offsetNodeId << ", " <<
						eIter[4] + offsetNodeId << ", " <<
						eIter[5] + offsetNodeId << ", " <<
						eIter[6] + offsetNodeId << ", " <<
						eIter[7] + offsetNodeId << std::endl;
				}
				++eIter;
			}
		}
		size_t hexa2Num = body->getCountByType(kmb::kHexahedron2);
		std::cout << "hexa2 = " << hexa2Num << std::endl;
		if (hexa2Num > 0) {
			output << "!ELEMENT, TYPE=362, EGRP=" << name << std::endl;
			typename MContainer::elementIterator eIter = body->begin();
			typename MContainer::elementIterator eIterEnd = body->end();
			while (eIter != eIterEnd) {
				if (eIter.getType() == kmb::kHexahedron2) {
					output << std::setw(9) << eIter.getId() + offsetElementId << ", " <<
						eIter[0] + offsetNodeId << ", " <<
						eIter[1] + offsetNodeId << ", " <<
						eIter[2] + offsetNodeId << ", " <<
						eIter[3] + offsetNodeId << ", " <<
						eIter[4] + offsetNodeId << ", " <<
						eIter[5] + offsetNodeId << ", " <<
						eIter[6] + offsetNodeId << ", " <<
						eIter[7] + offsetNodeId << ", " <<
						eIter[8] + offsetNodeId << ", " <<
						eIter[9] + offsetNodeId << ", " <<
						eIter[10] + offsetNodeId << ", " <<
						eIter[11] + offsetNodeId << ", " <<
						eIter[12] + offsetNodeId << ", " <<
						eIter[13] + offsetNodeId << ", " <<
						eIter[14] + offsetNodeId << ", " <<
						eIter[15] + offsetNodeId << ", " <<
						eIter[16] + offsetNodeId << ", " <<
						eIter[17] + offsetNodeId << ", " <<
						eIter[18] + offsetNodeId << ", " <<
						eIter[19] + offsetNodeId << std::endl;
				}
				++eIter;
			}
		}
		size_t prismNum = body->getCountByType(kmb::kPrism);
		std::cout << "prism = " << prismNum << std::endl;
		if (prismNum > 0) {
			output << "!ELEMENT, TYPE=351, EGRP=" << name << std::endl;
			typename MContainer::elementIterator eIter = body->begin();
			typename MContainer::elementIterator eIterEnd = body->end();
			while (eIter != eIterEnd) {
				if (eIter.getType() == kmb::kPrism) {
					output << std::setw(9) << eIter.getId() + offsetElementId << ", " <<
						eIter[0] + offsetNodeId << ", " <<
						eIter[1] + offsetNodeId << ", " <<
						eIter[2] + offsetNodeId << ", " <<
						eIter[3] + offsetNodeId << ", " <<
						eIter[4] + offsetNodeId << ", " <<
						eIter[5] + offsetNodeId << std::endl;
				}
				++eIter;
			}
		}
		size_t prism2Num = body->getCountByType(kmb::kPrism2);
		std::cout << "prism2 = " << prism2Num << std::endl;
		if (prism2Num > 0) {
			output << "!ELEMENT, TYPE=352, EGRP=" << name << std::endl;
			typename MContainer::elementIterator eIter = body->begin();
			typename MContainer::elementIterator eIterEnd = body->end();
			while (eIter != eIterEnd) {
				if (eIter.getType() == kmb::kPrism2) {
					output << std::setw(9) << eIter.getId() + offsetElementId << ", " <<
						eIter[0] + offsetNodeId << ", " <<
						eIter[1] + offsetNodeId << ", " <<
						eIter[2] + offsetNodeId << ", " <<
						eIter[3] + offsetNodeId << ", " <<
						eIter[4] + offsetNodeId << ", " <<
						eIter[5] + offsetNodeId << ", " <<
						eIter[6] + offsetNodeId << ", " <<
						eIter[7] + offsetNodeId << ", " <<
						eIter[8] + offsetNodeId << ", " <<
						eIter[9] + offsetNodeId << ", " <<
						eIter[10] + offsetNodeId << ", " <<
						eIter[11] + offsetNodeId << ", " <<
						eIter[12] + offsetNodeId << ", " <<
						eIter[13] + offsetNodeId << ", " <<
						eIter[14] + offsetNodeId << std::endl;
				}
				++eIter;
			}
		}
		size_t pyramidNum = body->getCountByType(kmb::kPyramid);
		std::cout << "pyramid = " << pyramidNum << std::endl;
		size_t pyramid2Num = body->getCountByType(kmb::kPyramid2);

		std::cout << "pyramid2 = " << pyramid2Num << std::endl;
		size_t triNum = body->getCountByType(kmb::kTriangle);

		std::cout << "tri = " << triNum << std::endl;
		if (triNum > 0) {
			output << "!ELEMENT, TYPE=231, EGRP=" << name << std::endl;
			typename MContainer::elementIterator eIter = body->begin();
			typename MContainer::elementIterator eIterEnd = body->end();
			while (eIter != eIterEnd) {
				if (eIter.getType() == kmb::kTriangle) {
					output << std::setw(9) << eIter.getId() + offsetElementId << ", " <<
						eIter[0] + offsetNodeId << ", " <<
						eIter[1] + offsetNodeId << ", " <<
						eIter[2] + offsetNodeId << std::endl;
				}
				++eIter;
			}
		}
		size_t tri2Num = body->getCountByType(kmb::kTriangle2);
		std::cout << "tri2 = " << tri2Num << std::endl;
		if (tri2Num > 0) {
			output << "!ELEMENT, TYPE=232, EGRP=" << name << std::endl;
			typename MContainer::elementIterator eIter = body->begin();
			typename MContainer::elementIterator eIterEnd = body->end();
			while (eIter != eIterEnd) {
				if (eIter.getType() == kmb::kTriangle2) {
					output << std::setw(9) << eIter.getId() + offsetElementId << ", " <<
						eIter[0] + offsetNodeId << ", " <<
						eIter[1] + offsetNodeId << ", " <<
						eIter[2] + offsetNodeId << ", " <<
						eIter[3] + offsetNodeId << ", " <<
						eIter[4] + offsetNodeId << ", " <<
						eIter[5] + offsetNodeId << std::endl;
				}
				++eIter;
			}
		}
		size_t quadNum = body->getCountByType(kmb::kQuadrilateral);
		std::cout << "quad = " << quadNum << std::endl;
		if (quadNum > 0) {
			output << "!ELEMENT, TYPE=241, EGRP=" << name << std::endl;
			typename MContainer::elementIterator eIter = body->begin();
			typename MContainer::elementIterator eIterEnd = body->end();
			while (eIter != eIterEnd) {
				if (eIter.getType() == kmb::kQuadrilateral) {
					output << std::setw(9) << eIter.getId() + offsetElementId << ", " <<
						eIter[0] + offsetNodeId << ", " <<
						eIter[1] + offsetNodeId << ", " <<
						eIter[2] + offsetNodeId << ", " <<
						eIter[3] + offsetNodeId << std::endl;
				}
				++eIter;
			}
		}
		size_t quad2Num = body->getCountByType(kmb::kQuadrilateral2);
		std::cout << "quad2 = " << quad2Num << std::endl;
		if (quad2Num > 0) {
			output << "!ELEMENT, TYPE=242, EGRP=" << name << std::endl;
			typename MContainer::elementIterator eIter = body->begin();
			typename MContainer::elementIterator eIterEnd = body->end();
			while (eIter != eIterEnd) {
				if (eIter.getType() == kmb::kQuadrilateral2) {
					output << std::setw(9) << eIter.getId() + offsetElementId << ", " <<
						eIter[0] + offsetNodeId << ", " <<
						eIter[1] + offsetNodeId << ", " <<
						eIter[2] + offsetNodeId << ", " <<
						eIter[3] + offsetNodeId << ", " <<
						eIter[4] + offsetNodeId << ", " <<
						eIter[5] + offsetNodeId << ", " <<
						eIter[6] + offsetNodeId << ", " <<
						eIter[7] + offsetNodeId << std::endl;
				}
				++eIter;
			}
		}
	}
	std::vector<std::string> names = mesh->getDataNames();
	std::vector<std::string>::iterator dIter = names.begin();
	while (dIter != names.end()) {
		std::string name = *dIter;
		std::cout << name << std::endl;
		const typename MContainer::DataContainer* data = mesh->getData(name);
		if (data) {
			std::string tag = data->getTag();
			if (tag == "BOUNDARY" ||
				tag == "SPRING" ||
				tag == "CLOAD" ||
				tag == "FIXTEMP" ||
				tag == "CFLUX" ||
				tag == "TEMPERATURE")
			{
				output << "!NGROUP, NGRP=" << name << std::endl;
				typename MContainer::dataIterator nIter = data->begin();
				while (nIter != data->end()) {
					output << " " << nIter.getId() + offsetNodeId << std::endl;
					++nIter;
				}
			}
			if (tag == "DLOAD")
			{
				output << "!SGROUP, SGRP=" << name << std::endl;
				kmb::Face f;
				typename MContainer::dataIterator fIter = data->begin();
				while (fIter != data->end()) {
					fIter.getFace(f);
					fIter.getFace(f);
					switch (mesh->findElement(f.getElementId()).getType())
					{
					case kmb::TETRAHEDRON:
					case kmb::TETRAHEDRON2:
						output << f.getElementId() + offsetElementId << "," << tetFmap[f.getLocalFaceId()] << std::endl;
						break;
					case kmb::WEDGE:
					case kmb::WEDGE2:
						output << f.getElementId() + offsetElementId << "," << wedFmap[f.getLocalFaceId()] << std::endl;
						break;
					case kmb::HEXAHEDRON:
					case kmb::HEXAHEDRON2:
						output << f.getElementId() + offsetElementId << "," << hexFmap[f.getLocalFaceId()] << std::endl;
						break;
					case kmb::PYRAMID:
					case kmb::PYRAMID2:
						output << f.getElementId() + offsetElementId << "," << pyrFmap[f.getLocalFaceId()] << std::endl;
						break;
					default:
						break;
					}
					++fIter;
				}
			}
		}
		++dIter;
	}
	output << "!INCLUDE, INPUT=section_info.txt" << std::endl;
	output << "!END" << std::endl;
	return 0;
}
