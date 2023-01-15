#include "RevocapIO/kmbFrontISTRIO.h"
#include "MeshDB/kmbMeshData.h"
#include "RevocapIO/kmbRevocapIOUtils.h"
#include "MeshDB/kmbDataBindings.h"
#include "MeshDB/kmbFace.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstring>

#ifdef _WIN32
#include "RevocapIO/kmbCharacterCode.h"
#endif

namespace kmb{
template<>
int FrontISTRIO::loadMesh(std::string filename, kmb::MeshData* mesh)
{
#ifdef _WIN32
	filename = kmb::CharacterCode::utf8_to_multi(filename);
#endif
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
		if (line.find("!!") == 0 || line.find("#") == 0) {
			// 無視する
			std::getline(input, line);
		}
		else if (line.find("!HEADER") == 0) {
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
			kmb::MeshData::DataContainer* data = NULL;
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
			kmb::MeshData::DataContainer* data = NULL;
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
		else if (line.find("!SGROUP") == 0) {
			std::string name = kmb::RevocapIOUtils::getValue(line, "SGRP");
			kmb::MeshData::DataContainer* data = NULL;
			data = mesh->getData(name.c_str());
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
						kmb::MeshData::elementIterator eIter = mesh->findElement(elementId - this->offsetElementId);
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
		else if (line.find("!INCLUDE")==0) {
			// 外部ファイルを再帰的に読む
			std::string subfilename = kmb::RevocapIOUtils::getValue(line, "INPUT");
			loadMesh(subfilename, mesh);
			std::getline(input, line);
		}
		else {
			std::getline(input, line);
		}
	}
	input.close();
	return 0;
}

template<>
int FrontISTRIO::saveMesh(std::string filename, const kmb::MeshData* mesh)
{
#ifdef _WIN32
	filename = kmb::CharacterCode::utf8_to_multi(filename);
#endif
	if (mesh == NULL) {
		return -1;
	}
	std::ofstream output(filename.c_str(), std::ios_base::out);
	if (output.fail()) {
		return -1;
	}
	output << "!HEADER, VER=3" << std::endl;
	output << " HECMW_MSH File generated by REVOCAP" << std::endl;
	// 節点の出力
	output << "!NODE" << std::endl;
	kmb::Point3D pt;
	kmb::MeshData::nodeIterator nIter = mesh->beginNodeIterator();
	kmb::MeshData::nodeIterator nIterEnd = mesh->endNodeIterator();
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
	// 要素の出力
	kmb::bodyIdType bodyCount = static_cast<kmb::bodyIdType>(mesh->getBodyCount());
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; bodyId++) {
		const kmb::MeshData::ElementContainer* body = mesh->getElementContainer(bodyId);
		if (body == NULL) continue;
		std::string name = body->getName();
		if (name.length() == 0) continue;
		// 要素の種類ごとに出力する
		size_t tetNum = body->getCountByType(kmb::kTetrahedron);
		if (tetNum > 0) {
			output << "!ELEMENT, TYPE=341";
			if( name != "ALL") output << ", EGRP=" << name;
			output << std::endl;
			kmb::MeshData::elementIterator eIter = body->begin();
			kmb::MeshData::elementIterator eIterEnd = body->end();
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
		if (tet2Num > 0) {
			output << "!ELEMENT, TYPE=342";
			if (name != "ALL") output << ", EGRP=" << name;
			output << std::endl;
			kmb::MeshData::elementIterator eIter = body->begin();
			kmb::MeshData::elementIterator eIterEnd = body->end();
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
		if (hexaNum > 0) {
			output << "!ELEMENT, TYPE=361";
			if (name != "ALL") output << ", EGRP=" << name;
			output << std::endl;
			kmb::MeshData::elementIterator eIter = body->begin();
			kmb::MeshData::elementIterator eIterEnd = body->end();
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
		if (hexa2Num > 0) {
			output << "!ELEMENT, TYPE=362";
			if (name != "ALL") output << ", EGRP=" << name;
			output << std::endl;
			kmb::MeshData::elementIterator eIter = body->begin();
			kmb::MeshData::elementIterator eIterEnd = body->end();
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
		if (prismNum > 0) {
			output << "!ELEMENT, TYPE=351";
			if (name != "ALL") output << ", EGRP=" << name;
			output << std::endl;
			kmb::MeshData::elementIterator eIter = body->begin();
			kmb::MeshData::elementIterator eIterEnd = body->end();
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
		if (prism2Num > 0) {
			output << "!ELEMENT, TYPE=352";
			if (name != "ALL") output << ", EGRP=" << name;
			output << std::endl;
			kmb::MeshData::elementIterator eIter = body->begin();
			kmb::MeshData::elementIterator eIterEnd = body->end();
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
		size_t pyramid2Num = body->getCountByType(kmb::kPyramid2);

		size_t triNum = body->getCountByType(kmb::kTriangle);
		if (triNum > 0) {
			output << "!ELEMENT, TYPE=231";
			if (name != "ALL") output << ", EGRP=" << name;
			output << std::endl;
			kmb::MeshData::elementIterator eIter = body->begin();
			kmb::MeshData::elementIterator eIterEnd = body->end();
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
			output << "!ELEMENT, TYPE=232";
			if (name != "ALL") output << ", EGRP=" << name;
			output << std::endl;
			kmb::MeshData::elementIterator eIter = body->begin();
			kmb::MeshData::elementIterator eIterEnd = body->end();
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
			output << "!ELEMENT, TYPE=241";
			if (name != "ALL") output << ", EGRP=" << name;
			output << std::endl;
			kmb::MeshData::elementIterator eIter = body->begin();
			kmb::MeshData::elementIterator eIterEnd = body->end();
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
			output << "!ELEMENT, TYPE=242";
			if (name != "ALL") output << ", EGRP=" << name;
			output << std::endl;
			kmb::MeshData::elementIterator eIter = body->begin();
			kmb::MeshData::elementIterator eIterEnd = body->end();
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
		const kmb::MeshData::DataContainer* data = mesh->getData(name);
		if (data) {
			std::string tag = data->getTag();
			if ( data->getBindingMode() == kmb::DataBindings::kNodeGroup &&
				(
				tag == "BOUNDARY" ||
				tag == "SPRING" ||
				tag == "CLOAD" ||
				tag == "TEMPERATURE" ||
				tag == "FIXTEMP" ||
				tag == "CFLUX" ||
				tag == "CONTACT" ||
				tag == "INITIAL" ||
				tag == "MPC"
				)
			)
			{
				output << "!NGROUP, NGRP=" << name << std::endl;
				kmb::MeshData::dataIterator nIter = data->begin();
				while (nIter != data->end()) {
					output << " " << nIter.getId() + offsetNodeId << std::endl;
					++nIter;
				}
			}
			else if (data->getBindingMode() == kmb::DataBindings::kFaceGroup &&
				(
				tag == "DLOAD" ||
				tag == "DFLUX" ||
				tag == "FILM" ||
				tag == "RADIATE" ||
				tag == "CONTACT" ||
				tag == "MPC"
				)
			)
			{
				output << "!SGROUP, SGRP=" << name << std::endl;
				kmb::Face f;
				kmb::MeshData::dataIterator fIter = data->begin();
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
			else if (data->getBindingMode() == kmb::DataBindings::kGlobal) {
				if (tag == "MPC") {
					output << "!INCLUDE, INPUT=" << name << ".mpc" << std::endl;
				}
				else if (tag == "CONTACT") {
					output << "!INCLUDE, INPUT=" << name << ".contact" << std::endl;
				}
			}
		}
		++dIter;
	}
	// section
	std::string::size_type pos = filename.find_last_of("\\/")+1;
	std::string basename = filename.substr(
		pos,
		filename.find_last_of('.')-pos);
	output << "!INCLUDE, INPUT=" << basename << ".sec" << std::endl;
	output << "!END" << std::endl;
	return 0;
}

template<>
int FrontISTRIO::loadResultAscii(std::string filename, kmb::MeshData* mesh)
{
	std::ifstream input(filename, std::ios_base::in);
	if (input.fail()) {
		return -1;
	}
	int nodeCount = 0;
	int elementCount = 0;
	int nodeValueCount = 0;
	int elementValueCount = 0;
	std::vector<int> nodeValDims;
	std::vector<int> elementValDims;
	std::string line;
	// 最初の１行
	if (std::getline(input, line)) {
		if (line.find("*fstrresult 2.0") != 0) {
			std::cout << "Warning Hecmw Result file format error." << std::endl;
			input.close();
			return -1;
		}
	}
	// *data 行まで空読み
	while (std::getline(input, line)) {
		if (line.find("*data") == 0) {
			break;
		}
	}
	// nodecount , elementcount
	if (std::getline(input, line)) {
		std::istringstream tokenizer(line);
		tokenizer >> nodeCount;
		tokenizer >> elementCount;
	}
	// node value , element value
	if (std::getline(input, line)) {
		std::istringstream tokenizer(line);
		tokenizer >> nodeValueCount;
		tokenizer >> elementValueCount;
	}
	mesh->clearTargetData();
	std::cout << "nodeValueCount " << nodeValueCount << std::endl;
	if (nodeValueCount > 0) {
		// node value dims
		while (nodeValDims.size() < nodeValueCount && std::getline(input, line)) {
			std::cout << "node values dim " << line << std::endl;
			std::istringstream tokenizer(line);
			int d = 0;
			while (tokenizer >> d) {
				nodeValDims.push_back(d);
			}
		}
		// node value names
		int dim = 0;
		for (int i = 0; i < nodeValueCount; ++i) {
			kmb::RevocapIOUtils::readOneLine(input, line);
			std::cout << "node value name " << line << std::endl;
			switch (nodeValDims[i]) {
			case 1:
				if (!mesh->hasData(line.c_str())) {
					mesh->createDataBindings(line.c_str(), kmb::DataBindings::NodeVariable, kmb::PhysicalValue::Scalar, "post");
				}
				mesh->appendTargetData(line.c_str());
				dim += 1;
				break;
			case 2:
				if (!mesh->hasData(line.c_str())) {
					mesh->createDataBindings(line.c_str(), kmb::DataBindings::NodeVariable, kmb::PhysicalValue::Vector2, "post");
				}
				mesh->appendTargetData(line.c_str());
				dim += 2;
				break;
			case 3:
				if (!mesh->hasData(line.c_str())) {
					mesh->createDataBindings(line.c_str(), kmb::DataBindings::NodeVariable, kmb::PhysicalValue::Vector3, "post");
				}
				mesh->appendTargetData(line.c_str());
				dim += 3;
				break;
			case 6:
				if (!mesh->hasData(line.c_str())) {
					mesh->createDataBindings(line.c_str(), kmb::DataBindings::NodeVariable, kmb::PhysicalValue::Tensor6, "post");
				}
				mesh->appendTargetData(line.c_str());
				dim += 6;
				break;
			default:
				break;
			}
		}
		// node value
		std::cout << "nodeCount " << nodeCount << " dim " << dim << std::endl;
		kmb::nodeIdType nodeId = kmb::nullNodeId;
		std::istringstream tokenizer;
		double* v = new double[dim];
		for (int i = 0; i < nodeCount; ++i) {
			kmb::RevocapIOUtils::readOneLine(input, line);
			tokenizer.str(line);
			tokenizer.clear();
			tokenizer >> nodeId;
			kmb::RevocapIOUtils::readOneLine(input, line);
			tokenizer.str(line);
			tokenizer.clear();
			for (int j = 0; j < dim; ++j) {
				if (tokenizer.eof()) {
					kmb::RevocapIOUtils::readOneLine(input, line);
					tokenizer.str(line);
					tokenizer.clear();
				}
				tokenizer >> v[j];
			}
			mesh->setMultiPhysicalValues(nodeId - this->offsetNodeId, v);
		}
		delete[] v;
	}
	mesh->clearTargetData();
	std::cout << "elementValueCount " << elementValueCount << std::endl;
	if (elementValueCount > 0) {
		// element value dims
		while ( elementValDims.size() < elementValueCount && std::getline(input, line)) {
			std::cout << "element values dim " << line << std::endl;
			std::istringstream tokenizer(line);
			int d=0;
			while (tokenizer >> d) {
				elementValDims.push_back(d);
			}
		}
		// element value names
		int dim = 0;
		for (int i = 0; i < elementValueCount; ++i) {
			kmb::RevocapIOUtils::readOneLine(input, line);
			std::cout << "element value name " << line << std::endl;
			switch (elementValDims[i]) {
			case 1:
				if (!mesh->hasData(line.c_str())) {
					mesh->createDataBindings(line.c_str(), kmb::DataBindings::ElementVariable, kmb::PhysicalValue::Scalar, "post");
				}
				mesh->appendTargetData(line.c_str());
				dim += 1;
				break;
			case 2:
				if (!mesh->hasData(line.c_str())) {
					mesh->createDataBindings(line.c_str(), kmb::DataBindings::ElementVariable, kmb::PhysicalValue::Vector2, "post");
				}
				mesh->appendTargetData(line.c_str());
				dim += 2;
				break;
			case 3:
				if (!mesh->hasData(line.c_str())) {
					mesh->createDataBindings(line.c_str(), kmb::DataBindings::ElementVariable, kmb::PhysicalValue::Vector3, "post");
				}
				mesh->appendTargetData(line.c_str());
				dim += 3;
				break;
			case 6:
				if (!mesh->hasData(line.c_str())) {
					mesh->createDataBindings(line.c_str(), kmb::DataBindings::ElementVariable, kmb::PhysicalValue::Tensor6, "post");
				}
				mesh->appendTargetData(line.c_str());
				dim += 6;
				break;
			default:
				break;
			}
		}
		// element value
		kmb::elementIdType elementId = kmb::Element::nullElementId;
		std::istringstream tokenizer;
		double* v = new double[dim];
		for (int i = 0; i < elementCount; ++i) {
			kmb::RevocapIOUtils::readOneLine(input, line);
			tokenizer.str(line);
			tokenizer.clear();
			tokenizer >> elementId;
			kmb::RevocapIOUtils::readOneLine(input, line);
			tokenizer.str(line);
			tokenizer.clear();
			for (int j = 0; j < dim; ++j) {
				if (tokenizer.eof()) {
					kmb::RevocapIOUtils::readOneLine(input, line);
					tokenizer.str(line);
					tokenizer.clear();
				}
				tokenizer >> v[j];
			}
			mesh->setMultiPhysicalValues(elementId - this->offsetElementId, v);
		}
		delete[] v;
	}
	input.close();
	// あえて clearTargetData をしない => boundary extractor で表面に値を引き継がせるため
	return 0;
}

template<>
int FrontISTRIO::loadResultBinary(std::string filename, kmb::MeshData* mesh)
{
	std::ifstream input(filename, std::ios_base::in | std::ios_base::binary);
	if (input.fail()) {
		return -1;
	}
	long nodeCount = 0;
	long elementCount = 0;
	long nodeValueCount = 0;
	long elementValueCount = 0;
	int dim = 0;
	std::vector<int> nodeValDims;
	std::vector<int> elementValDims;
	// 最初の１行
	char buf[128];
	input.read(buf, 19);
	buf[19] = '\0';
	if (strncmp("HECMW_BINARY_RESULT", buf, 19) != 0) {
		input.close();
		return -1;
	}
	// long の大きさ = 4 byte = 32 bit を仮定
	input.read(buf, 2);
	buf[2] = '\0';
	std::cout << "HECMW BINARY RESULT INTEGERSIZE : " << buf << std::endl;
	// *fstrresult 2.0
	input.get(buf, 128, '\0');
	buf[15] = '\0';
	if (strncmp("*fstrresult 2.0", buf, 15) != 0){
		input.close();
		return -2;
	}
	input.read(buf, 1);
	// *comment
	input.get(buf, 128, '\0');
	input.read(buf, 1);
	input.get(buf, 128, '\0');
	input.read(buf, 1);
	// *global
	input.get(buf, 128, '\0');
	input.read(buf, 1);
	input.read(buf, sizeof(long));
	input.read(buf, sizeof(long));
	// *TOTALTIME
	input.get(buf, 128, '\0');
	input.read(buf, 1);
	input.read(buf, sizeof(double));
	// *data
	input.get(buf, 128, '\0');
	input.read(buf, 1);

	input.read(reinterpret_cast<char*>(&nodeCount), sizeof(long));
	input.read(reinterpret_cast<char*>(&elementCount), sizeof(long));
	input.read(reinterpret_cast<char*>(&nodeValueCount), sizeof(long));
	input.read(reinterpret_cast<char*>(&elementValueCount), sizeof(long));

	// 節点データ
	mesh->clearTargetData();
	for (int i = 0; i < nodeValueCount; ++i) {
		long dl = 0;
		input.read(reinterpret_cast<char*>(&dl), sizeof(long));
		int d = static_cast<int>(dl);
		nodeValDims.push_back(d);
	}
	dim = 0;
	for (int i = 0; i < nodeValueCount; ++i) {
		input.get(buf, 128, '\0');
		switch (nodeValDims[i]) {
		case 1:
			if (!mesh->hasData(buf)) {
				mesh->createDataBindings(buf, kmb::DataBindings::NodeVariable, kmb::PhysicalValue::Scalar, "post");
			}
			mesh->appendTargetData(buf);
			dim += 1;
			break;
		case 2:
			if (!mesh->hasData(buf)) {
				mesh->createDataBindings(buf, kmb::DataBindings::NodeVariable, kmb::PhysicalValue::Vector2, "post");
			}
			mesh->appendTargetData(buf);
			dim += 2;
			break;
		case 3:
			if (!mesh->hasData(buf)) {
				mesh->createDataBindings(buf, kmb::DataBindings::NodeVariable, kmb::PhysicalValue::Vector3, "post");
			}
			mesh->appendTargetData(buf);
			dim += 3;
			break;
		case 6:
			if (!mesh->hasData(buf)) {
				mesh->createDataBindings(buf, kmb::DataBindings::NodeVariable, kmb::PhysicalValue::Tensor6, "post");
			}
			mesh->appendTargetData(buf);
			dim += 6;
			break;
		default:
			break;
		}
		input.read(buf, 1);
	}
	if (nodeValueCount > 0) {
		kmb::nodeIdType nodeId = kmb::nullNodeId;
		double* v = new double[dim];
		for (int i = 0; i < nodeCount; ++i) {
			long il = 0;
			input.read(reinterpret_cast<char*>(&il), sizeof(long));
			nodeId = static_cast<kmb::nodeIdType>(il);
			input.read(reinterpret_cast<char*>(v), sizeof(double)*dim);
			mesh->setMultiPhysicalValues(nodeId - this->offsetNodeId, v);
		}
		delete[] v;
	}

	// 要素データ
	mesh->clearTargetData();
	for (int i = 0; i < elementValueCount; ++i) {
		long dl = 0;
		input.read(reinterpret_cast<char*>(&dl), sizeof(long));
		int d = static_cast<int>(dl);
		elementValDims.push_back(d);
	}
	dim = 0;
	for (int i = 0; i < elementValueCount; ++i) {
		input.get(buf, 128, '\0');
		switch (elementValDims[i]) {
		case 1:
			if (!mesh->hasData(buf)) {
				mesh->createDataBindings(buf, kmb::DataBindings::ElementVariable, kmb::PhysicalValue::Scalar, "post");
			}
			mesh->appendTargetData(buf);
			dim += 1;
			break;
		case 2:
			if (!mesh->hasData(buf)) {
				mesh->createDataBindings(buf, kmb::DataBindings::ElementVariable, kmb::PhysicalValue::Vector2, "post");
			}
			mesh->appendTargetData(buf);
			dim += 2;
			break;
		case 3:
			if (!mesh->hasData(buf)) {
				mesh->createDataBindings(buf, kmb::DataBindings::ElementVariable, kmb::PhysicalValue::Vector3, "post");
			}
			mesh->appendTargetData(buf);
			dim += 3;
			break;
		case 6:
			if (!mesh->hasData(buf)) {
				mesh->createDataBindings(buf, kmb::DataBindings::ElementVariable, kmb::PhysicalValue::Tensor6, "post");
			}
			mesh->appendTargetData(buf);
			dim += 6;
			break;
		default:
			break;
		}
		input.read(buf, 1);
	}
	if (elementValueCount > 0) {
		kmb::elementIdType elementId = kmb::Element::nullElementId;
		double* v = new double[dim];
		for (int i = 0; i < elementCount; ++i) {
			long il = 0;
			input.read(reinterpret_cast<char*>(&il), sizeof(long));
			elementId = static_cast<kmb::elementIdType>(il);
			input.read(reinterpret_cast<char*>(v), sizeof(double)*dim);
			mesh->setMultiPhysicalValues(elementId - this->offsetElementId, v);
		}
		delete[] v;
	}

	input.close();
	// あえて clearTargetData をしない => boundary extractor で表面に値を引き継がせるため
	return 0;
}

template<>
int FrontISTRIO::loadResult(std::string filename, kmb::MeshData* mesh)
{
#ifdef _WIN32
	filename = kmb::CharacterCode::utf8_to_multi(filename);
#endif
	if (mesh == NULL) {
		return -1;
	}
	// 最初の１行でバイナリチェック
	std::ifstream input(filename, std::ios_base::in | std::ios_base::binary);
	if (input.fail()) {
		return -1;
	}
	char buf[64];
	input.read(buf, 19);
	buf[19] = '\0';
	if (strncmp("HECMW_BINARY_RESULT", buf, 19) == 0) {
		input.close();
		return loadResultBinary(filename, mesh);
	}
	else if (strncmp("*fstrresult 2.0", buf, 15) == 0) {
		input.close();
		return loadResultAscii(filename, mesh);
	}
	input.close();
	return -2;
}

}