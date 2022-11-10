#define BOOST_TEST_MODULE MeshBrepTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include "Test/Test_Common.h"
#include "MeshDB/kmbMeshData.h"
#include "MeshDB/kmbMeshBrep.h"
#include "MeshDB/kmbMeshOperation.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(MeshBrep)

BOOST_AUTO_TEST_CASE(UpdateBrep)
{
	kmb::MeshData* mesh0 = createCubeMesh(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	kmb::MeshData* mesh1 = createCubeMesh(-1.0, -1.0, -1.0, 2.0, 2.0, 0.0);
	kmb::MeshBrep* brep0 = kmb::MeshBrep::create3DFaceModel(mesh0,120.0);
	BOOST_CHECK_EQUAL( mesh0->getBodyCount(), 2 );
	BOOST_CHECK_EQUAL( brep0->getVolumeCount(), 1 );
	BOOST_CHECK_EQUAL( brep0->getFaceCount(0), 6 );
	kmb::MeshOperation meshOp(mesh0);
	meshOp.mergeMesh(mesh1);
	brep0->update3DFaceModel(mesh0,120.0);
	BOOST_CHECK_EQUAL( mesh0->getBodyCount(), 4 );
	BOOST_CHECK_EQUAL( brep0->getVolumeCount(), 2 );
	BOOST_CHECK_EQUAL( brep0->getFaceCount(0), 6 );
	BOOST_CHECK_EQUAL( brep0->getFaceCount(2), 6 );
	delete brep0;
	delete mesh0;
	delete mesh1;
}

BOOST_AUTO_TEST_SUITE_END()

