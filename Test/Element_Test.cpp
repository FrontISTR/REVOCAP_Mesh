#define BOOST_TEST_MODULE ElementTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include "Test/Test_Common.h"
#include "MeshDB/kmbMeshData.h"

BOOST_AUTO_TEST_SUITE(Element)

BOOST_AUTO_TEST_CASE(Tetraherdon)
{
	kmb::MeshData mesh;
	mesh.beginModel();
	mesh.beginNode(4);
	mesh.addNode( 0.0, 0.0, 0.0 );
	mesh.addNode( 1.0, 0.0, 0.0 );
	mesh.addNode( 0.0, 1.0, 0.0 );
	mesh.addNode( 0.0, 0.0, 1.0 );
	mesh.endNode();
	mesh.beginElement(1);
	int tetra[] = {0,1,2,3};
	mesh.addElement(kmb::kTetrahedron,tetra);
	mesh.endElement();
	mesh.endModel();
	BOOST_CHECK_EQUAL( mesh.getBodyCount(), 1 );
	BOOST_CHECK_EQUAL( mesh.getNodeCount(), 4 );
	BOOST_CHECK_EQUAL( mesh.getElementCount(0), 1 );
	kmb::Point3D pt;
	mesh.getNode(0,pt);
	BOOST_CHECK_EQUAL( pt.x(), 0.0 );
	BOOST_CHECK_EQUAL( pt.y(), 0.0 );
	BOOST_CHECK_EQUAL( pt.z(), 0.0 );
	mesh.getNode(1,pt);
	BOOST_CHECK_EQUAL( pt.x(), 1.0 );
	BOOST_CHECK_EQUAL( pt.y(), 0.0 );
	BOOST_CHECK_EQUAL( pt.z(), 0.0 );
	mesh.getNode(2,pt);
	BOOST_CHECK_EQUAL( pt.x(), 0.0 );
	BOOST_CHECK_EQUAL( pt.y(), 1.0 );
	BOOST_CHECK_EQUAL( pt.z(), 0.0 );
	mesh.getNode(3,pt);
	BOOST_CHECK_EQUAL( pt.x(), 0.0 );
	BOOST_CHECK_EQUAL( pt.y(), 0.0 );
	BOOST_CHECK_EQUAL( pt.z(), 1.0 );
	kmb::ElementContainer::iterator e = mesh.findElement(0);
	BOOST_CHECK_EQUAL( e.getType(), kmb::kTetrahedron );
	BOOST_CHECK_EQUAL( e[0], 0 );
	BOOST_CHECK_EQUAL( e[1], 1 );
	BOOST_CHECK_EQUAL( e[2], 2 );
	BOOST_CHECK_EQUAL( e[3], 3 );
}

BOOST_AUTO_TEST_CASE(Tetraherdon_Coordinate)
{
	kmb::MeshData mesh;
	mesh.beginModel();
	mesh.beginNode(4);
	mesh.addNode( drand(0.1,-0.1), drand(0.1,-0.1), drand(0.1,-0.1) );
	mesh.addNode( drand(1.1, 0.9), drand(0.1,-0.1), drand(0.1,-0.1) );
	mesh.addNode( drand(0.1,-0.1), drand(1.1, 0.9), drand(0.1,-0.1) );
	mesh.addNode( drand(0.1,-0.1), drand(0.1,-0.1), drand(1.1, 0.9) );
	mesh.endNode();
	mesh.beginElement(1);
	int tetra[] = {0,1,2,3};
	mesh.addElement(kmb::kTetrahedron,tetra);
	mesh.endElement();
	mesh.endModel();
	kmb::ElementEvaluator eval(mesh.getNodes());
	for(int count=0;count<100;count++){
		double nat0[3];
		double nat1[3];
		nat0[0] = drand();
		nat0[1] = drand();
		nat0[2] = drand();
		double sum = nat0[0] + nat0[1] + nat0[2];
		nat0[0] /= sum;
		nat0[1] /= sum;
		nat0[2] /= sum;
		kmb::Point3D pt0;
		kmb::Point3D pt1;
		kmb::ElementContainer::iterator e = mesh.findElement(0);
		eval.getPhysicalCoordinates(e,nat0[0],nat0[1],nat0[2],pt0);
		eval.getNaturalCoordinates(e,pt0.x(),pt0.y(),pt0.z(),nat1);
		eval.getPhysicalCoordinates(e,nat1[0],nat1[1],nat1[2],pt1);
		BOOST_CHECK_CLOSE(pt1.x(),pt0.x(),1.0e-10);
		BOOST_CHECK_CLOSE(pt1.y(),pt0.y(),1.0e-10);
		BOOST_CHECK_CLOSE(pt1.z(),pt0.z(),1.0e-10);
		BOOST_CHECK_CLOSE(nat1[0],nat0[0],1.0e-10);
		BOOST_CHECK_CLOSE(nat1[1],nat0[1],1.0e-10);
		BOOST_CHECK_CLOSE(nat1[2],nat0[2],1.0e-10);
	}
}

/*
BOOST_AUTO_TEST_CASE(Tetraherdon2_Coordinate)
{
	kmb::MeshData mesh;
	mesh.beginModel();
	mesh.beginNode(10);
	mesh.addNode( drand(0.1,-0.1), drand(0.1,-0.1), drand(0.1,-0.1) );
	mesh.addNode( drand(1.1, 0.9), drand(0.1,-0.1), drand(0.1,-0.1) );
	mesh.addNode( drand(0.1,-0.1), drand(1.1, 0.9), drand(0.1,-0.1) );
	mesh.addNode( drand(0.1,-0.1), drand(0.1,-0.1), drand(1.1, 0.9) );
	mesh.addNode( drand(0.6, 0.4), drand(0.6, 0.4), drand(0.1,-0.1) );
	mesh.addNode( drand(0.1,-0.1), drand(0.6, 0.4), drand(0.1,-0.1) );
	mesh.addNode( drand(0.6, 0.4), drand(0.1,-0.1), drand(0.1,-0.1) );
	mesh.addNode( drand(0.1,-0.1), drand(0.1,-0.1), drand(0.6, 0.4) );
	mesh.addNode( drand(0.6, 0.4), drand(0.1,-0.1), drand(0.6, 0.4) );
	mesh.addNode( drand(0.1,-0.1), drand(0.6, 0.4), drand(0.6, 0.4) );
	mesh.endNode();
	mesh.beginElement(1);
	int tetra2[] = {0,1,2,3,4,5,6,7,8,9};
	mesh.addElement(kmb::kTetrahedron2,tetra2);
	mesh.endElement();
	mesh.endModel();
	kmb::ElementEvaluator eval(mesh.getNodes());
	for(int count=0;count<100;count++){
		double nat0[3];
		double nat1[3];
		nat0[0] = drand();
		nat0[1] = drand();
		nat0[2] = drand();
		double sum = nat0[0] + nat0[1] + nat0[2];
		nat0[0] /= sum;
		nat0[1] /= sum;
		nat0[2] /= sum;
		kmb::Point3D pt0;
		kmb::Point3D pt1;
		kmb::ElementContainer::iterator e = mesh.findElement(0);
		eval.getPhysicalCoordinates(e,nat0[0],nat0[1],nat0[2],pt0);
		eval.getNaturalCoordinates(e,pt0.x(),pt0.y(),pt0.z(),nat1);
		eval.getPhysicalCoordinates(e,nat1[0],nat1[1],nat1[2],pt1);
		BOOST_CHECK_CLOSE(pt1.x(),pt0.x(),1.0e-10);
		BOOST_CHECK_CLOSE(pt1.y(),pt0.y(),1.0e-10);
		BOOST_CHECK_CLOSE(pt1.z(),pt0.z(),1.0e-10);
		BOOST_CHECK_CLOSE(nat1[0],nat0[0],1.0e-10);
		BOOST_CHECK_CLOSE(nat1[1],nat0[1],1.0e-10);
		BOOST_CHECK_CLOSE(nat1[2],nat0[2],1.0e-10);
	}
}
*/

BOOST_AUTO_TEST_SUITE_END()


