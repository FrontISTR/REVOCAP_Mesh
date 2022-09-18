#define BOOST_TEST_MODULE RevocapIOTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include "Test/Test_Common.h"
#include "MeshDB/kmbMeshData.h"
#include "RevocapIO/kmbHecmwIO.h"
#include "RevocapIO/kmbFrontISTRIO.h"
#include "RevocapIO/kmbRevocapIOUtils.h"

BOOST_AUTO_TEST_SUITE(RevocapIO)

BOOST_AUTO_TEST_CASE(STLIO)
{
	int n=0;
	BOOST_CHECK_EQUAL(n,0);
}

BOOST_AUTO_TEST_CASE(FrontISTRIO)
{
	kmb::MeshData* mesh = new kmb::MeshData();
	mesh->beginNode();
	mesh->addNodeWithId(0.0, 0.0, 0.0, 0);
	mesh->addNodeWithId(1.0, 0.0, 0.0, 1);
	mesh->addNodeWithId(0.0, 1.0, 0.0, 10);
	mesh->addNodeWithId(0.0, 0.0, 1.0, 100);
	mesh->endNode();
	kmb::nodeIdType nodes[4] = { 0,1,10,100 };
	mesh->beginElement();
	mesh->addElementWithId(kmb::kTetrahedron, nodes, 10);
	mesh->endElement();
	kmb::DataBindings* data = mesh->createDataBindings("BND0", kmb::DataBindings::kNodeGroup, kmb::PhysicalValue::None, "BOUNDARY");
	data->addId(0);
	kmb::HecmwIO hecmw;
	hecmw.saveToMeshFile_3_6("TestResults/FrontISTRTest_hecIO.msh", mesh);
	hecmw.appendFooterToFile("TestResults/FrontISTRTest_hecIO.msh");
	mesh->clearModel();

	kmb::FrontISTRIO fistr;
	mesh->createDataBindings("BND0", kmb::DataBindings::kNodeGroup, kmb::PhysicalValue::Vector3, "BOUNDARY");
	fistr.loadMesh("TestResults/FrontISTRTest_hecIO.msh", mesh);
	fistr.saveMesh("TestResults/FrontISTRTest_meshdata.msh", mesh);

	delete mesh;
}

BOOST_AUTO_TEST_CASE(RevocapIOUtilsTest)
{
	std::string line = "!SGROUP, SGRP = master";
	std::string val = kmb::RevocapIOUtils::getValue(line, "SGRP");
	std::cout << "[" << val << "]" << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
