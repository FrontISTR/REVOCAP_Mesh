#define BOOST_TEST_MODULE BucketTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include "Test/Test_Common.h"
#include "Common/kmbCalculator.h"
#include "Geometry/kmbBucket.h"
#include "Geometry/kmbBoxRegion.h"
#include "Geometry/kmbPoint3DContainerMArray.h"
#include "MeshDB/kmbTriangleBucket.h"
#include "MeshDB/kmbTriangleBucketArea.h"
#include "MeshDB/kmbFaceBucketArea.h"
#include "MeshDB/kmbElementContainerMArray.h"
#include "MeshDB/kmbTriangle.h"
#include "MeshDB/kmbMeshDB.h"
#include "RevocapIO/kmbTripatchPcmIO.h"
#include "RevocapIO/kmbRevocapNeutralIO.h"
#include "RevocapIO/kmbTetMeshMIO.h"
#include "RevocapIO/kmbFrontSTRIO.h"

BOOST_AUTO_TEST_SUITE( Bucket )

// 点を格納するバケットをつかって、最近点の探索が正しいこと
BOOST_AUTO_TEST_CASE( Point_Bucket_Nearest )
{
	// 10000 個のランダムな点を格納する
	kmb::Point3DContainerMArray points;
	points.initialize(10000);
	for(int i=0;i<10000;i++){
		points.addPoint( drand(), drand(), drand() );
	}
	kmb::Bucket3DGrid bucket;
	kmb::BoundingBox bbox = points.getBoundingBox();
	bucket.setRegionGrid( bbox, 100 );
	bucket.setContainer( &points );
	bucket.appendAll();
	kmb::Point3D pt;
	for(int i=0;i<100;i++){
		double x = drand();
		double y = drand();
		double z = drand();
		// Bucket3DGrid search
		kmb::nodeIdType bucket_nearestId = kmb::nullNodeId;
		double bucket_dist = bucket.getNearest(x,y,z,bucket_nearestId);
		// naive search
		kmb::Minimizer min;
		kmb::nodeIdType naive_nearestId = kmb::nullNodeId;
		kmb::Point3DContainer::const_iterator pIter = points.begin();
		while( !pIter.isFinished() ){
			pIter.getPoint( pt );
			if( min.update( pt.distanceSq( x, y, z ) ) ){
				naive_nearestId = pIter.getId();
			}
			++pIter;
		}
		double naive_dist = sqrt(min.getMin());
		BOOST_CHECK_EQUAL( bucket_nearestId, naive_nearestId );
		BOOST_CHECK_CLOSE( bucket_dist, naive_dist, 1.0e-8 );
	}
}

// 点を格納する bucket の iterator
BOOST_AUTO_TEST_CASE(Point_Bucket_Iterator)
{
	irand();
	// 10000 個のランダムな点を格納する
	kmb::Point3DContainerMArray points;
	points.initialize(10000);
	for (int i = 0; i<10000; i++) {
		points.addPoint(drand(), drand(), drand());
	}
	kmb::Bucket3DGrid bucket;
	kmb::BoundingBox bbox = points.getBoundingBox();
	bucket.setRegionGrid(bbox, 100);
	bucket.setContainer(&points);
	bucket.appendAll();
	int numX, numY, numZ;
	bucket.getBlockSize(numX, numY, numZ);
	int i = numX / 2;
	int j = numX / 2;
	int k = numX / 2;
	int count = static_cast<int>(bucket.getCount(i, j, k));
	int count0 = 0;
	kmb::Bucket3DGrid::iterator iter = bucket.begin(i,j,k);
	kmb::Bucket3DGrid::iterator end = bucket.end(i,j,k);
	while (iter != end) {
		count0++;
		++iter;
	}
	// naive に数える
	int count1 = 0;
	double x0 = bucket.getX(i);
	double x1 = bucket.getX(i+1);
	double y0 = bucket.getY(j);
	double y1 = bucket.getY(j+1);
	double z0 = bucket.getZ(k);
	double z1 = bucket.getZ(k+1);
	kmb::Point3D p;
	for (int pointId = 0; pointId<10000; pointId++) {
		points.getPoint(pointId, p);
		if (
			x0 <= p.x() && p.x() < x1 &&
			y0 <= p.y() && p.y() < y1 &&
			z0 <= p.z() && p.z() < z1
			)
		{
			++count1;
		}
	}
	BOOST_CHECK_EQUAL(count, count0);
	BOOST_CHECK_EQUAL(count, count1);
	std::cout << "bucket count = " << count << std::endl;
}

/*
// 1000 個のランダムな三角形を TriangleBucket と TriangleBucketArea に格納
// バケットごとの格納されている三角形が一致していることを確認
BOOST_AUTO_TEST_CASE( Triangle_Bucket_Area )
{
	kmb::Point3DContainer* points = new kmb::Point3DContainerMArray();
	kmb::ElementContainer* elements = new kmb::ElementContainerMArray(1000);
	kmb::BoxRegion box(-3.0, -3.0, -3.0, 5.0, 5.0, 5.0);
	points->initialize(3000);
	for(int i=0;i<1000;++i){
		points->addPoint( drand(6.0,-4.0), drand(6.0,-4.0), drand(6.0,-4.0) );
		points->addPoint( drand(6.0,-4.0), drand(6.0,-4.0), drand(6.0,-4.0) );
		points->addPoint( drand(6.0,-4.0), drand(6.0,-4.0), drand(6.0,-4.0) );
		elements->addElement(new kmb::Triangle(3*i,3*i+1,3*i+2));
	}

	kmb::TriangleBucket bucket( box, 8, 10, 12 );
	BOOST_CHECK_EQUAL( 8*10*12, bucket.getBlockSize() );
	BOOST_CHECK_EQUAL( static_cast<size_t>(0u), bucket.getCount() );
	bucket.setContainer( points, elements );
	bucket.appendAll();

	kmb::TriangleBucketArea bucketA( box, 8, 10, 12 );
	bucketA.setContainer( points, elements );
	bucketA.appendAll();

	for(int i=0;i<8;++i){
		for(int j=0;j<10;++j){
			for(int k=0;k<12;++k){
				BOOST_CHECK_EQUAL( bucket.getCount(i,j,k), bucketA.getCount(i,j,k) );
			}
		}
	}
	bucket.clear();
	bucketA.clear();
	delete points;
	delete elements;
}

// tetra を格納するバケットで、ある点が含まれる要素の結果がナイーブサーチと一致する
BOOST_AUTO_TEST_CASE( Tetra_Bucket )
{
	kmb::MeshDB* mesh = new kmb::MeshDB();
	kmb::TetMeshMIO tetmesh;
	tetmesh.loadFromFile( "data/box0.msh",mesh);
	BOOST_CHECK_EQUAL( static_cast<size_t>(6220u), mesh->getElementCount(0) );
	BOOST_CHECK_EQUAL( static_cast<size_t>(1368u), mesh->getNodeCount() );

	kmb::ElementBucket bucket;
	bucket.setContainer( mesh->getNodes(), mesh->getBodyPtr(0) );
	kmb::BoundingBox bbox = mesh->getBoundingBox();
	bbox.expandDiameter(1.2);
	bucket.setRegion( bbox );
	bucket.setGridSize( 30, 30, 30 );
	bucket.appendAll();

	kmb::ElementEvaluator evaluator( mesh->getNodes() );

	// Bucket で調べた点とループで調べた点が一致すること
	// 100 回調べる
	kmb::Point3D pt;

	for(int i=0;i<100;++i){
		double x = drand(bbox.maxX(),bbox.minX());
		double y = drand(bbox.maxY(),bbox.minY());
		double z = drand(bbox.maxZ(),bbox.minZ());

		// Bucket Search
		kmb::elementIdType bucketElementId = bucket.searchElement(x,y,z);

		// Naive Search
		kmb::Maximizer maximizer;
		kmb::elementIdType naiveElementId = kmb::Element::nullElementId;
		kmb::ElementContainer::iterator eIter = mesh->getBodyPtr(0)->begin();
		while( !eIter.isFinished() ){
			if( maximizer.update( evaluator.getMinInnerVolume( eIter, x, y, z ) ) ){
				naiveElementId = eIter.getId();
			}
			++eIter;
		}
		if( maximizer.getMax() < 0.0 ){
			naiveElementId = kmb::Element::nullElementId;
		}

		BOOST_CHECK_EQUAL( naiveElementId, bucketElementId );
	}

	delete mesh;
}
*/
/*
// Bucket に格納されている面積と元の面積が一致することをチェック
BOOST_AUTO_TEST_CASE( Face_Bucket_Area )
{
	kmb::MeshDB* mesh = new kmb::MeshDB();
	kmb::FrontSTRIO fstrio;
	mesh->beginModel();
	int res = fstrio.loadFromMeshFileV4("Advance/TestData/concentric_cylinder_model.msh",mesh);
	mesh->endModel();
	kmb::BoxRegion box(-0.31,-0.31,-0.025,0.31,0.31,0.515);
	kmb::FaceBucketArea bucket;
	bucket.setRegion(box);
	bucket.setGridSize(62,62,54);

	kmb::DataBindings* boundary = mesh->createDataBindings("Boundary",kmb::DataBindings::FaceGroup,kmb::PhysicalValue::None,"Brep",-1);
	for( kmb::bodyIdType bodyId = 0; bodyId < mesh->getBodyCount(); ++bodyId ){
		kmb::ElementContainer* body = mesh->getBodyPtr(bodyId);
		if( body ){
			if( body->getDimension() == 2 ){
				// Shell はそのまま登録する
				if( boundary && boundary->getBindingMode() == kmb::DataBindings::FaceGroup ){
					kmb::ElementContainer::iterator eIter = body->begin();
					while( !eIter.isFinished() ){
						boundary->addId( kmb::Face(eIter.getId(),-1));
						++eIter;
					}
				}
				printf("REVOCAP Legato Boundary %u\n", static_cast<unsigned int>(mesh->getIdCount("Boundary")));
			}
		}
	}
	bucket.setContainer( mesh, boundary );
	bucket.appendAll();

	bucket.clear();
	delete mesh;
}
*/
/*
BOOST_AUTO_TEST_CASE( Triangle_Bucket_Screw )
{
	kmb::MeshDB* mesh = new kmb::MeshDB();
	kmb::TripatchPcmIO tripatch;
	tripatch.loadFromFile( "data/screw.pcm", mesh );
	BOOST_CHECK_EQUAL( static_cast<size_t>(2210u), mesh->getNodeCount() );
	BOOST_CHECK_EQUAL( static_cast<size_t>(4416u), mesh->getElementCount(0) );
	kmb::BoundingBox bbox = mesh->getBoundingBox();
	double d = bbox.diameter() / 200;
	int xnum = static_cast<int>(bbox.rangeX() / d);
	int ynum = static_cast<int>(bbox.rangeY() / d);
	int znum = static_cast<int>(bbox.rangeZ() / d);
	bbox.expand(1.2);
	kmb::TriangleBucket tbucket(bbox,xnum,ynum,znum);
	tbucket.setContainer( mesh->getNodes(), mesh->getBodyPtr(0) );
	tbucket.appendAll();
	kmb::MeshDB* meshOut = new kmb::MeshDB();
	meshOut->beginModel();
	meshOut->beginNode( (xnum+1) * (ynum+1) * (znum+1) );
	for(int i=0;i<=xnum;++i){
		for(int j=0;j<=ynum;++j){
			for(int k=0;k<=znum;++k){
				double x = bbox.minX() + (bbox.rangeX() * i ) / xnum;
				double y = bbox.minY() + (bbox.rangeY() * j ) / ynum;
				double z = bbox.minZ() + (bbox.rangeZ() * k ) / znum;
				meshOut->addNode( x, y, z );
			}
		}
	}
	meshOut->endNode();
	kmb::nodeIdType nodes[8];
	meshOut->beginElement();
	for(int i=0;i<xnum;++i){
		for(int j=0;j<ynum;++j){
			for(int k=0;k<znum;++k){
				if( tbucket.getCount(i,j,k) > 0 ){
					nodes[0] = i     * (ynum+1) * (znum+1) + j     * (znum+1) + k;
					nodes[1] = (i+1) * (ynum+1) * (znum+1) + j     * (znum+1) + k;
					nodes[2] = (i+1) * (ynum+1) * (znum+1) + (j+1) * (znum+1) + k;
					nodes[3] = i     * (ynum+1) * (znum+1) + (j+1) * (znum+1) + k;
					nodes[4] = i     * (ynum+1) * (znum+1) + j     * (znum+1) + k + 1;
					nodes[5] = (i+1) * (ynum+1) * (znum+1) + j     * (znum+1) + k + 1;
					nodes[6] = (i+1) * (ynum+1) * (znum+1) + (j+1) * (znum+1) + k + 1;
					nodes[7] = i     * (ynum+1) * (znum+1) + (j+1) * (znum+1) + k + 1;
					meshOut->addElement( kmb::HEXAHEDRON, nodes );
				}
			}
		}
	}
	meshOut->endElement();
	meshOut->createDataBindings( "Normal", kmb::DataBindings::ElementVariable, kmb::PhysicalValue::Vector3 );
	meshOut->endModel();
	kmb::RevocapNeutralIO neutralIO;
	neutralIO.saveToRNFFile( "output/screwBucket.rnf", meshOut );
	delete mesh;
	delete meshOut;
}
*/

BOOST_AUTO_TEST_CASE(Point_Bucket_Index)
{
	kmb::Point3DContainerMArray points;
	points.initialize(2000);
	for (int i = 0; i<1000; i++) {
		points.addPoint(drand(), drand(), drand());
	}
	// 1000 から 1999 まで
	// 0 から 999 までと
	// 同じ座標に格納する
	for (int i = 0; i<1000; i++) {
		kmb::Point3D pt;
		points.getPoint(i, pt);
		points.addPoint(pt.x(), pt.y(), pt.z());
	}
	kmb::Bucket3DGrid bucket;
	kmb::BoundingBox bbox = points.getBoundingBox();
	// 全体の大きさが 100 程度になるように bucket を分割する
	bucket.setRegionGrid(bbox, 100);
	bucket.setContainer(&points);
	bucket.appendAll();
	for (kmb::nodeIdType i = 0; i<1000; i++) {
		kmb::Point3D pt;
		points.getPoint(i, pt);
		int index = bucket.getIndex(pt.x(), pt.y(), pt.z());
		BOOST_CHECK(index >= 0);
	}
}

BOOST_AUTO_TEST_SUITE_END()
