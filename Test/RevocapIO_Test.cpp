#define BOOST_TEST_MODULE RevocapIOTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include <fstream>
#include "MeshDB/kmbMeshData.h"
#include "RevocapIO/kmbMicroAVSIO.h"

BOOST_AUTO_TEST_SUITE(RevocapIO)

BOOST_AUTO_TEST_CASE(STLIO)
{
	int n=0;
	BOOST_CHECK_EQUAL(n,0);
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

BOOST_AUTO_TEST_SUITE_END()
