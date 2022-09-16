#define BOOST_TEST_MODULE MeshDataTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include "Test/Test_Common.h"
#include "MeshDB/kmbMeshData.h"

BOOST_AUTO_TEST_SUITE(MeshData)

BOOST_AUTO_TEST_CASE(BodyName)
{
	kmb::MeshData mesh;
	mesh.beginModel();
	mesh.beginElement();
	mesh.endElement();
	mesh.beginElement();
	mesh.endElement();
	mesh.endModel();
	mesh.setBodyName(1,"hoge");
	BOOST_CHECK_EQUAL( mesh.getBodyName(0), "" );
	BOOST_CHECK_EQUAL( mesh.getBodyName(1), "hoge" );
}

BOOST_AUTO_TEST_SUITE_END()

