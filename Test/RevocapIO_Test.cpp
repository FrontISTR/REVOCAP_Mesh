#define BOOST_TEST_MODULE RevocapIOTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include "Test/Test_Common.h"
#include "MeshDB/kmbMeshData.h"
#include "RevocapIO/kmbMicroAVSIO.h"
#include "RevocapIO/kmbHecmwIO.h"
#include "RevocapIO/kmbFrontISTRIO.h"
#include "RevocapIO/kmbRevocapIOUtils.h"

BOOST_AUTO_TEST_SUITE(RevocapIO)

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
	hecmw.saveToMeshFile_3_6("FrontISTRTest_hecIO.msh", mesh);
	hecmw.appendFooterToFile("FrontISTRTest_hecIO.msh");
	mesh->clearModel();
	delete mesh;

	mesh = new kmb::MeshData();
	kmb::FrontISTRIO fistr;
	mesh->createDataBindings("BND0", kmb::DataBindings::kNodeGroup, kmb::PhysicalValue::Vector3, "BOUNDARY");
	fistr.loadMesh("FrontISTRTest_hecIO.msh", mesh);
	fistr.saveMesh("FrontISTRTest_meshdata.msh", mesh);
	BOOST_CHECK_EQUAL(mesh->getNodeCount(),4);
	BOOST_CHECK_EQUAL(mesh->getElementCount(), 1);
	BOOST_CHECK(mesh->hasData("BND0"));
	BOOST_CHECK_EQUAL(mesh->getIdCount("BND0"), 1);
	BOOST_CHECK_EQUAL(mesh->getData("BND0")->getBindingMode(), kmb::DataBindings::kNodeGroup);
	BOOST_CHECK(mesh->getData("BND0")->hasId(0));
	delete mesh;
}

BOOST_AUTO_TEST_CASE(MicroAVSIO_MeshData)
{
	kmb::MeshData* mesh0 = new kmb::MeshData();
	mesh0->beginNode();
	mesh0->addNode(0.0, 0.0, 0.0);
	mesh0->addNode(1.0, 0.0, 0.0);
	mesh0->addNode(0.0, 1.0, 0.0);
	mesh0->addNode(0.0, 0.0, 1.0);
	mesh0->endNode();
	kmb::nodeIdType nodeTable[] = {0,1,2,3};
	mesh0->beginElement();
	mesh0->addElement( kmb::TETRAHEDRON, nodeTable );
	mesh0->endElement();
	BOOST_CHECK_EQUAL(mesh0->getNodeCount(),4);
	BOOST_CHECK_EQUAL(mesh0->getElementCount(),1);
	BOOST_CHECK_EQUAL(mesh0->getBodyCount(),1);
	kmb::MicroAVSIO avs;
	avs.saveToFile("test.inp",mesh0);
	kmb::MeshData* mesh1 = new kmb::MeshData();
	avs.loadFromFile("test.inp",mesh1);
	BOOST_CHECK_EQUAL(mesh1->getNodeCount(),4);
	BOOST_CHECK_EQUAL(mesh1->getElementCount(),1);
	delete mesh0;
	delete mesh1;
}

BOOST_AUTO_TEST_CASE(RevocapIOUtilsTest)
{
	std::string line = "!SGROUP, SGRP = master";
	std::string val = kmb::RevocapIOUtils::getValue(line, "SGRP");
	BOOST_CHECK_EQUAL(val, "master");
}

BOOST_AUTO_TEST_SUITE_END()
