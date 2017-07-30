#define BOOST_TEST_MODULE FrontISTRTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include <iostream>
#include "Test/testCommon.h"
#include "RevocapIO/kmbFrontISTRIO.h"
#include "MeshDB/kmbMeshData.h"

BOOST_AUTO_TEST_SUITE( FrontISTR )

BOOST_AUTO_TEST_CASE( FrontISTRIO_Material )
{
	std::cout << "FrontISTR Material Test" << std::endl;
	kmb::FrontISTRMaterial mat;
	mat.name = "MAT0";
	kmb::FrontISTRMaterial_Elastic_Isotropic* elastic = new kmb::FrontISTRMaterial_Elastic_Isotropic;
	elastic->poisson = 0.3;
	elastic->youngs = 7.0e10;
	mat.materials.push_back( elastic );
	std::cout << mat;
}

BOOST_AUTO_TEST_CASE( FrontISTRIO_cnt )
{
	std::cout << "FrontISTR Control Test" << std::endl;
	kmb::MeshData mesh;
	kmb::FrontISTRData data;
	kmb::FrontISTRIO fistr;
	kmb::FrontISTRData::OrientationByCoordinates* ori = new kmb::FrontISTRData::OrientationByCoordinates();
	ori->name = "C1";
	ori->a[0] = 0.0; ori->a[1] = 0.0; ori->a[2] = 0.0;
	ori->b[0] = 0.0; ori->b[1] = 1.0; ori->b[2] = 0.0;
	ori->c[0] = 0.0; ori->c[1] = 0.0; ori->c[2] = 1.0;
	data.orientations.push_back(ori);
	kmb::FrontISTRMaterial* mat = new kmb::FrontISTRMaterial();
	mat->name = "MAT0";
	kmb::FrontISTRMaterial_Elastic_Isotropic* elastic = new kmb::FrontISTRMaterial_Elastic_Isotropic;
	elastic->poisson = 0.3;
	elastic->youngs = 7.0e10;
	mat->materials.push_back( elastic );
	data.materials.push_back( mat );
	data.analysis = new kmb::FrontISTRStaticAnalysis();
	kmb::FrontISTRVisual::BoundarySurface* surface = new kmb::FrontISTRVisual::BoundarySurface();
	kmb::FrontISTRVisual::SmoothContour* display = new kmb::FrontISTRVisual::SmoothContour();
	display->comp_name = "MISES_STRESS";
	surface->deform_comp_name = "DISPLACEMENT";
	surface->display = display;
	data.visual.surfaces.push_back(surface);
	data.visual.output = new kmb::FrontISTRVisual::BMPOutput();
	fistr.saveCntFile("test.cnt",&data,&mesh);
}

BOOST_AUTO_TEST_SUITE_END()
