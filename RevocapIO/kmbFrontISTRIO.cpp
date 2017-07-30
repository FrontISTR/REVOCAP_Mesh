/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.7                          #
# Class Name : FrontSTRIO                                              #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2016/03/22     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/

#include "RevocapIO/kmbFrontISTRIO.h"
#include "Common/kmbEnvInfo.h"
#include "Common/kmbContainer.h"
#include "MeshDB/kmbMeshData.h"
#include <fstream>
#include <limits>
#include <cmath>

kmb::FrontISTRData::~FrontISTRData(void)
{
	clear();
}

void kmb::FrontISTRData::clear(void)
{
	kmb::clearVector<kmb::FrontISTRData::Orientation>(orientations);
	kmb::clearVector<kmb::FrontISTRData::Section>(sections);
	kmb::clearVector<kmb::FrontISTRMaterial>(materials);
	kmb::clearVector<kmb::FrontISTRStep>(steps);
}

kmb::FrontISTRMaterial::~FrontISTRMaterial(void)
{
	clear();
}

void kmb::FrontISTRMaterial::clear(void)
{
	kmb::clearVector<kmb::FrontISTRMaterialItem>(materials);
}

void kmb::FrontISTRMaterialItem::output(std::ostream &os) const
{
}

void kmb::FrontISTRMaterial_Elastic_Isotropic::output(std::ostream &os) const
{
	os << "!ELASTIC, TYPE=ISOTROPIC, DEPENDENCIES=0" << std::endl;
	os << " " << youngs << ", " << poisson << std::endl;
}

void kmb::FrontISTRMaterial_Elastic_Isotropic_Dependent::output(std::ostream &os) const
{
	os << "!ELASTIC, TYPE=ISOTROPIC, DEPENDENCIES=1" << std::endl;
	std::vector< struct kmb::FrontISTRMaterial_Elastic_Isotropic_Dependent::Elastic >::const_iterator mIter = this->elastics.begin();
	while( mIter != this->elastics.end() ){
		os << " " << mIter->youngs << ", " << mIter->poisson << ", " << mIter->temperature << std::endl;
		++mIter;
	}
}

void kmb::FrontISTRMaterial_Plastic_Mises_Bilinear::output(std::ostream &os) const
{
	os << "!PLASTIC, YIELD=MISES, HARDEN=BILINEAR, DEPENDENCIES=0" << std::endl;
	os << " " << yield0 << ", " << h << std::endl;
}

void kmb::FrontISTRMaterial_Plastic_Mises_MultiLinear::output(std::ostream &os) const
{
	os << "!PLASTIC, YIELD=MISES, HARDEN=MULTILINEAR, DEPENDENCIES=0" << std::endl;
	std::vector< kmb::FrontISTRMaterial_Plastic_Mises_MultiLinear::StressStrain >::const_iterator sIter = this->ss_points.begin();
	while( sIter != this->ss_points.end() ){
		os << " " << sIter->yield << ", " << sIter->pstrain << std::endl;
		++sIter;
	}
}

void kmb::FrontISTRMaterial_Plastic_Mises_Swift::output(std::ostream &os) const
{
	os << "!PLASTIC, YIELD=MISES, HARDEN=SWIFT, DEPENDENCIES=0" << std::endl;
	os << " " << this->epsilon0 << ", " << this->k << ", " << this->n << std::endl;
}

void kmb::FrontISTRMaterial_Plastic_Mises_RambergOsgood::output(std::ostream &os) const
{
	os << "!PLASTIC, YIELD=MISES, HARDEN=RAMBERG-OSGOOD, DEPENDENCIES=0" << std::endl;
	os << " " << this->epsilon0 << ", " << this->d << ", " << this->n << std::endl;
}

void kmb::FrontISTRMaterial_Plastic_Mises_Kinematic::output(std::ostream &os) const
{
	os << "!PLASTIC, YIELD=MISES, HARDEN=KINEMATIC, DEPENDENCIES=0" << std::endl;
	os << " " << this->yield0 << ", " << this->kinematic_coeff << std::endl;
}

void kmb::FrontISTRMaterial_Plastic_Mises_Combined::output(std::ostream &os) const
{
	os << "!PLASTIC, YIELD=MISES, HARDEN=COMBINED, DEPENDENCIES=0" << std::endl;
	os << " " << this->yield0 << ", " << this->h << ", " << this->kinematic_coeff << std::endl;
}

void kmb::FrontISTRMaterial_Plastic_MohrCoulomb_Bilinear::output(std::ostream &os) const
{
	os << "!PLASTIC, YIELD=Mohr-Coulomb, HARDEN=BILINEAR, DEPENDENCIES=0" << std::endl;
	os << " " << visco << ", " << fai << ", " <<h << std::endl;
}

void kmb::FrontISTRMaterial_Plastic_MohrCoulomb_MultiLinear::output(std::ostream &os) const
{
	os << "!PLASTIC, YIELD=Mohr-Coulomb, HARDEN=MULTILINEAR, DEPENDENCIES=0" << std::endl;
	os << " " << fai << std::endl;
	std::vector< struct kmb::FrontISTRMaterial_Plastic_MohrCoulomb_MultiLinear::StressStrain >::const_iterator sIter = this->ss_points.begin();
	while( sIter != this->ss_points.end() ){
		os << " " << sIter->pstrain << ", " << sIter->visco << std::endl;
		++sIter;
	}
}

void kmb::FrontISTRMaterial_Plastic_DruckerPrager_Bilinear::output(std::ostream &os) const
{
	os << "!PLASTIC, YIELD=Drucker-Prager, HARDEN=BILINEAR, DEPENDENCIES=0" << std::endl;
	os << " " << visco << ", " << fai << ", " <<h << std::endl;
}

void kmb::FrontISTRMaterial_Plastic_DruckerPrager_MultiLinear::output(std::ostream &os) const
{
	os << "!PLASTIC, YIELD=Drucker-Prager, HARDEN=MULTILINEAR, DEPENDENCIES=0" << std::endl;
	os << " " << fai << std::endl;
	std::vector< struct kmb::FrontISTRMaterial_Plastic_DruckerPrager_MultiLinear::StressStrain >::const_iterator sIter = this->ss_points.begin();
	while( sIter != this->ss_points.end() ){
		os << " " << sIter->pstrain << ", " << sIter->visco << std::endl;
		++sIter;
	}
}

void kmb::FrontISTRMaterial_Hyperelastic_Neohooke::output(std::ostream &os) const
{
	os << "!HYPERELASTIC, TYPE=NEOHOOKE" << std::endl;
	os << " " << c10 << ", " << d << std::endl;
}

void kmb::FrontISTRMaterial_Hyperelastic_MooneyRivlin::output(std::ostream &os) const
{
	os << "!HYPERELASTIC, TYPE=MOONEY-RIVLIN" << std::endl;
	os << " " << c10 << ", " << c01 << ", " << d << std::endl;
}

void kmb::FrontISTRMaterial_Hyperelastic_ArrudaBoyce::output(std::ostream &os) const
{
	os << "!HYPERELASTIC, TYPE=ARRUDA-BOYCE" << std::endl;
	os << " " << mu << ", " << lambda_m << ", " << d << std::endl;
}

void kmb::FrontISTRMaterial_Viscoelastic::output(std::ostream &os) const
{
	os << "!VISCOELASTIC" << std::endl;
	os << " " << g << ", " << t << std::endl;
	if( trs.def != kmb::FrontISTRMaterial_Viscoelastic::kUNDEF ){
		os << "!TRS, DEFINITION=";
		switch(trs.def){
		case kmb::FrontISTRMaterial_Viscoelastic::kWLF:
			os << "WLF" << std::endl;
			break;
		case kmb::FrontISTRMaterial_Viscoelastic::kARRHENUS:
			os << "ARRHENUS" << std::endl;
			break;
		default:
			break;
		}
		os << " " << trs.theta0 << ", " << trs.c1 << ", " << trs.c2 << std::endl;
	}
}

void kmb::FrontISTRMaterial_Creep_Norton::output(std::ostream &os) const
{
	os << "!CREEP, TYPE=NORTON, DEPENDENCIES=0" << std::endl;
	os << " " << a << ", " << n << ", " << m << std::endl;
}

void kmb::FrontISTRMaterial_Density::output(std::ostream &os) const
{
	os << "!DENSITY" << std::endl;
	os << " " << density << std::endl;
}

void kmb::FrontISTRMaterial_Expansion_Isotropic::output(std::ostream &os) const
{
	os << "!EXPANSION_COEFF, TYPE=ISOTROPIC, DENPENDENCIES=0" << std::endl;
	os << " " << expansion << std::endl;
}

void kmb::FrontISTRMaterial_Expansion_Orthotropic::output(std::ostream &os) const
{
	os << "!EXPANSION_COEFF, TYPE=ORTHOTROPIC, DENPENDENCIES=0" << std::endl;
	os << " " << alpha11 << ", " << alpha22 << ", " << alpha33 << std::endl;
}

void kmb::FrontISTRAnalysis::output(std::ostream &os) const
{
}

void kmb::FrontISTRStaticAnalysis::output(std::ostream &os) const
{
	os << "!STATIC" << std::endl;
}

void kmb::FrontISTREigenAnalysis::output(std::ostream &os) const
{
	os << "!EIGEN" << std::endl;
	os << " " << nset << ", " << lcztol << ", " << lczmax << std::endl;
}

void kmb::FrontISTRHeatAnalysis::output(std::ostream &os) const
{
	os << "!HEAT" << std::endl;
	os << " " << dt;
	if( dt > 0 ){
		os << ", " << etime << ", " << dtmin << ", " << deltmx << ", " << itmax << ", " << eps;
	}
	os << std::endl;
}

void kmb::FrontISTRData::Orientation::output(std::ostream& os) const
{
}

void kmb::FrontISTRData::OrientationByCoordinates::output(std::ostream& os) const
{
	os << "!ORIENTATION, NAME=" << this->name << ", DEFINITION=COORDINATES" << std::endl;
	os << " " << a[0] << ", " << a[1] << ", " << a[2] << std::endl;
	os << " " << b[0] << ", " << b[1] << ", " << b[2] << std::endl;
	os << " " << c[0] << ", " << c[1] << ", " << c[2] << std::endl;
}

void kmb::FrontISTRData::OrientationByNodes::output(std::ostream& os) const
{
	os << "!ORIENTATION, NAME=" << this->name << ", DEFINITION=NODES" << std::endl;
	os << " " << a << ", " << b << ", " << c << std::endl;
}

kmb::FrontISTRSolver::FrontISTRSolver(void)
	: method(kmb::FrontISTRSolver::kCG)
	, precond(kmb::FrontISTRSolver::kSSOR)
	, iterLog(false)
	, timeLog(false)
	, useJAD(false)
	, scaling(false)
	, dumpType(kmb::FrontISTRSolver::kNONE)
	, dumpExit(false)
	, mpcMethod(kmb::FrontISTRSolver::kUNDEF)
	, estCond(0)
	, nIter(100)
	, iterPREmax(1)
	, nRest(10)
	, nColor_in(10)
	, residue(1.0e-8)
	, sigma_diag(1.0)
{}

kmb::FrontISTRVisual::FrontISTRVisual(void)
	: method(kmb::FrontISTRVisual::kPSR)
	, start_step(-1)
	, end_step(-1)
	, interval_step(-1)
	, output(NULL)
{
}

kmb::FrontISTRVisual::~FrontISTRVisual(void)
{
	kmb::clearVector<kmb::FrontISTRVisual::Surface>(surfaces);
	if( output != NULL ) delete output;
}

void kmb::FrontISTRVisual::Display::output(std::ostream &os) const
{
	os << "!display_method = " << this->method << std::endl;
}

void kmb::FrontISTRVisual::SmoothContour::output(std::ostream &os) const
{
	os << "!display_method = " << this->method << std::endl;
	os << "!color_comp_name = " << this->comp_name << std::endl;
	os << "!color_subcomp = " << this->subcomp << std::endl;
}

void kmb::FrontISTRVisual::LineContour::output(std::ostream &os) const
{
	os << "!display_method = " << this->method << std::endl;
	os << "!isoline_number = " << this->isoline_number << std::endl;
	os << "!isoline_color = " << this->isoline_color[0] << ", " << this->isoline_color[1] << ", " << this->isoline_color[2] << std::endl;
}

void kmb::FrontISTRVisual::SmoothLineContour::output(std::ostream &os) const
{
	os << "!display_method = " << this->method << std::endl;
	os << "!color_comp_name = " << this->comp_name << std::endl;
	os << "!color_subcomp = " << this->subcomp << std::endl;
	os << "!isoline_number = " << this->isoline_number << std::endl;
	os << "!isoline_color = " << this->isoline_color[0] << ", " << this->isoline_color[1] << ", " << this->isoline_color[2] << std::endl;
}

void kmb::FrontISTRVisual::SolidColor::output(std::ostream &os) const
{
	os << "!display_method = " << this->method << std::endl;
	os << "!specified_color = " << this->specified_color << std::endl;
}

void kmb::FrontISTRVisual::SolidContour::output(std::ostream &os) const
{
	os << "!display_method = " << this->method << std::endl;
	os << "!isoline_number = " << this->isoline_number << std::endl;
	os << "!isoline_color = " << this->isoline_color[0] << ", " << this->isoline_color[1] << ", " << this->isoline_color[2] << std::endl;
}

kmb::FrontISTRVisual::Surface::Surface(void)
	: style(kmb::FrontISTRVisual::Surface::kUNDEF)
	, display(NULL)
	, initial_style(kmb::FrontISTRVisual::Surface::kSHADING)
	, deform_style(kmb::FrontISTRVisual::Surface::kNONE)
{
}

kmb::FrontISTRVisual::Surface::Surface(kmb::FrontISTRVisual::Surface::SurfaceStyle _style)
	: style(_style)
	, display(NULL)
	, initial_style(kmb::FrontISTRVisual::Surface::kSHADING)
	, deform_style(kmb::FrontISTRVisual::Surface::kNONE)
{
}

kmb::FrontISTRVisual::Surface::~Surface(void)
{
	if( display != NULL ) delete display;
}

kmb::FrontISTRVisual::BoundarySurface::BoundarySurface(void)
	: kmb::FrontISTRVisual::Surface(kmb::FrontISTRVisual::Surface::kBOUNDARY)
{
}

kmb::FrontISTRVisual::Isosurface::Isosurface(void)
	: kmb::FrontISTRVisual::Surface(kmb::FrontISTRVisual::Surface::kISOSURF)
{
}

kmb::FrontISTRVisual::QuadraticSurface::QuadraticSurface(void)
	: kmb::FrontISTRVisual::Surface(kmb::FrontISTRVisual::Surface::kQUADRATIC)
{
}

void kmb::FrontISTRVisual::Surface::output(std::ostream &os) const
{
	os << "!SURFACE" << std::endl;
	os << "!surface_style = " << this->style << std::endl;
	this->display->output(os);
	os << "!initial_style = " << this->initial_style << std::endl;
	os << "!deform_style = " << this->deform_style << std::endl;
	os << "!initial_line_color = " << initial_line_color[0] << ", " << initial_line_color[1] << ", " << initial_line_color[2] << std::endl;
	os << "!deform_line_color = " << deform_line_color[0] << ", " << deform_line_color[1] << ", " << deform_line_color[2] << std::endl;
	if( deform_scale > 0.0 ) os << "!deform_scale = " << this->deform_scale << std::endl;
	os << "!deform_comp_name = " << deform_comp_name << std::endl;
}

void kmb::FrontISTRVisual::BoundarySurface::output(std::ostream &os) const
{
	kmb::FrontISTRVisual::Surface::output(os);
}

void kmb::FrontISTRVisual::Isosurface::output(std::ostream &os) const
{
	kmb::FrontISTRVisual::Surface::output(os);
	os << "!data_comp_name = " << this->data_comp_name << std::endl;
	os << "!data_subcomp = " << this->data_subcomp << std::endl;
	os << "!iso_value = " << this->iso_value << std::endl;
}

void kmb::FrontISTRVisual::QuadraticSurface::output(std::ostream &os) const
{
	kmb::FrontISTRVisual::Surface::output(os);
	os << "!method = 5" << std::endl;
	os << "!coef = " << coef[0] << ", " << coef[1] << ", " << coef[2] << ", " << coef[3] << ", " << coef[4] << ", " << coef[5] << ", " << coef[6] << ", " << coef[7] << ", " << coef[8] << ", " << coef[9] << std::endl;
}

void kmb::FrontISTRVisual::Output::output(std::ostream &os) const
{
	os << "!output_type = ";
	switch( this->output_type ){
	case kmb::FrontISTRVisual::Output::kAVS: os << "AVS" << std::endl;
	case kmb::FrontISTRVisual::Output::kBMP: os << "BMP" << std::endl;
	case kmb::FrontISTRVisual::Output::kCOMPLETE_AVS: os << "COMPLETE_AVS" << std::endl;
	case kmb::FrontISTRVisual::Output::kCOMPLETE_REORDER_AVS: os << "COMPLETE_REORDER_AVS" << std::endl;
	case kmb::FrontISTRVisual::Output::kSEPARATE_COMPLETE_AVS: os << "SEPARATE_COMPLETE_AVS" << std::endl;
	case kmb::FrontISTRVisual::Output::kCOMPLETE_MICROAVS: os << "COMPLETE_MICROAVS" << std::endl;
	case kmb::FrontISTRVisual::Output::kBIN_COMPLETE_AVS: os << "BIN_COMPLETE_AVS" << std::endl;
	case kmb::FrontISTRVisual::Output::kFSTR_FEMAP_NEUTRAL: os << "FSTR_FEMAP_NEUTRAL" << std::endl;
	default: break;
	}
}

void kmb::FrontISTRVisual::BMPOutput::output(std::ostream &os) const
{
	os << "!output_type = BMP" << std::endl;
	os << "!x_resolution = " << this->x_resolution << std::endl;
	os << "!y_resolution = " << this->y_resolution << std::endl;
	os << "!viewpoint = " << this->viewpoint[0] << ", " << this->viewpoint[1] << ", " << this->viewpoint[2] << std::endl;
	os << "!look_at_point = " << this->look_at_point[0] << ", " << this->look_at_point[1] << ", " << this->look_at_point[2] << std::endl;
	os << "!up_direction = " << this->up_direction[0] << ", " << this->up_direction[1] << ", " << this->up_direction[2] << std::endl;
	os << "!ambient_coef = " << this->ambient << std::endl;
	os << "!diffuse_coef = " << this->diffuse << std::endl;
	os << "!specular_coef = " << this->specular << std::endl;
	if( color_mapping_bar ){
		os << "!color_mapping_bar_on = on" << std::endl;
		if( scale_marking ) os << "!scale_marking_on = on" << std::endl;
		if( num_of_scales ) os << "!num_of_scales = " << num_of_scales << std::endl;
		os << "!font_size = " << this->font_size << std::endl;
		os << "!font_color = " << this->font_color[0] << ", "<< this->font_color[1] << ", " << this->font_color[2] <<  std::endl;
		os << "!background_color = " << this->background_color[0] << ", " << this->background_color[1] << ", " << this->background_color[2] <<  std::endl;
	}
}

// face table converter REVO => Fistr
int kmb::FrontISTRIO::tetFmap[] = {3,4,2,1};
int kmb::FrontISTRIO::wedFmap[] = {1,2,3,4,5};
int kmb::FrontISTRIO::hexFmap[] = {1,2,3,4,5,6};
int kmb::FrontISTRIO::pyrFmap[] = {2,3,4,5,1};

// face table converter Fistr => REVO
int kmb::FrontISTRIO::tetRmap[] = {3,2,0,1};
int kmb::FrontISTRIO::wedRmap[] = {0,1,2,3,4};
int kmb::FrontISTRIO::hexRmap[] = {0,1,2,3,4,5};
int kmb::FrontISTRIO::pyrRmap[] = {4,0,1,2,3};

kmb::FrontISTRIO::FrontISTRIO()
	: offsetNodeId(1)
	, offsetElementId(1)
	, nodeCount(0)
	, elementCount(0)
{
}

int kmb::FrontISTRIO::saveCntFile(const char* filename,const kmb::FrontISTRData* fdata,const kmb::MeshData* mesh)
{
	std::ofstream output(filename);
	std::string time_string;
	kmb::EnvInfo::getCurrentTimeString(time_string);

	output << "###############################################" << std::endl;
	output << "# Control File for FrontISTR                  #" << std::endl;
	output << "# generated by REVOCAP at " << time_string << " #" << std::endl;
	//                                   YYYY/mm/dd HH:MM:SS
	output << "###############################################" << std::endl;
	output << *fdata;
	if( fdata->analysis ){
		output << "#####################" << std::endl;
		output << "# Analysis Controls #" << std::endl;
		output << "#####################" << std::endl;
		output << *(fdata->analysis);
	}
	// BOUNDARY
	output << "#######################" << std::endl;
	output << "# Boundary Conditions #" << std::endl;
	output << "#######################" << std::endl;
	const std::multimap< std::string, kmb::DataBindings* > mapper = mesh->getDataBindingsMap();
	std::multimap< std::string, kmb::DataBindings*>::const_iterator dIter = mapper.begin();
	{
		// tag = "BOUNDARY" でループする
		while( dIter != mapper.end() ){
			kmb::DataBindings* data = dIter->second;
			if( data != NULL && data->getSpecType().compare("BOUNDARY") == 0 )
			{
				output << "!BOUNDARY" << std::endl;
				std::string name = dIter->first;
				kmb::PhysicalValue* val = data->getPhysicalValue();
				double tensor[6] = {std::numeric_limits<float>::quiet_NaN()};
				val->getValue(tensor);
				for(int i=0;i<6;++i){
					if( isfinite(tensor[i]) ){
						output << " " << name << ", " << (i+1) << ", " << (i+1) << ", " << tensor[i] << std::endl;
					}
				}
			}
			++dIter;
		}
	}
	{
		// tag = "DIM" でループする
		std::multimap< std::string, kmb::DataBindings*>::const_iterator dIter = mapper.begin();
		while( dIter != mapper.end() ){
			kmb::DataBindings* data = dIter->second;
			if( data != NULL && data->getSpecType().compare("DIM") == 0 )
			{
				output << "!BOUNDARY" << std::endl;
				std::string name = dIter->first;
				kmb::PhysicalValue* val = data->getPhysicalValue();
				double dim[3] = {std::numeric_limits<float>::quiet_NaN()};
				val->getValue(dim);
				for(int i=0;i<3;++i){
					if( isfinite(dim[i]) ){
						output << " ALL, " << (i+1) << ", " << (i+1) << ", " << dim[i] << std::endl;
					}
				}
			}
			++dIter;
		}
	}
	{
		// tag = "CLOAD" でループする
		std::multimap< std::string, kmb::DataBindings*>::const_iterator dIter = mapper.begin();
		while( dIter != mapper.end() ){
			kmb::DataBindings* data = dIter->second;
			std::string name = dIter->first;
			if( data != NULL && data->getSpecType().compare("CLOAD") == 0 )
			{
				if( data->getBindingMode() == kmb::DataBindings::NodeGroup && data->getValueType() == kmb::PhysicalValue::Vector3 )
				{
					output << "!CLOAD" << std::endl;
					double cload[3] = {
						std::numeric_limits<float>::quiet_NaN(),
						std::numeric_limits<float>::quiet_NaN(),
						std::numeric_limits<float>::quiet_NaN()
					};
					data->getPhysicalValue()->getValue( cload );
					for(int i=0;i<3;++i){
						if( isfinite(cload[i]) && cload[i] != 0.0 ){
							output << " " << name << ", " << i+1 << ", " <<
								std::showpoint << cload[i] << std::endl;
						}
					}
				}
				else if( data->getBindingMode() == kmb::DataBindings::NodeVariable && data->getValueType() == kmb::PhysicalValue::Vector3 )
				{
					output << "!CLOAD" << std::endl;
					double cload[3] = {
						std::numeric_limits<float>::quiet_NaN(),
						std::numeric_limits<float>::quiet_NaN(),
						std::numeric_limits<float>::quiet_NaN()
					};
					kmb::DataBindings::const_iterator nIter = data->begin();
					while( !nIter.isFinished() ){
						kmb::nodeIdType nodeId = nIter.getId();
						if( nIter.getValue( cload ) ){
							for(int i=0;i<3;++i){
								output << " " << nodeId + this->offsetNodeId << ", " << i+1 << ", " <<
									std::showpoint << cload[i] << std::endl;
							}
						}
						++nIter;
					}
				}
			}
			++dIter;
		}
	}
	{
		// tag = "DLOAD" でループする
		std::multimap< std::string, kmb::DataBindings*>::const_iterator dIter = mapper.begin();
		while( dIter != mapper.end() ){
			kmb::DataBindings* data = dIter->second;
			std::string name = dIter->first;
			if( data != NULL && data->getSpecType().compare("DLOAD") == 0 )
			{
				output << "!DLOAD" << std::endl;
				kmb::Face f;
				double pres = 0.0;
				kmb::bodyIdType bodyId = data->getTargetBodyId();
				if( data->getBindingMode() == kmb::DataBindings::FaceVariable ){
					kmb::DataBindings::const_iterator fIter = data->begin();
					while( !fIter.isFinished() ){
						fIter.getFace(f);
						fIter.getValue(&pres);
						kmb::ElementContainer::const_iterator eIter = mesh->findElement(f.getElementId(),bodyId);
						switch(eIter.getType()){
						case kmb::TETRAHEDRON:
						case kmb::TETRAHEDRON2:
							output << " " << eIter.getId() + this->offsetElementId << ", " <<
								"P" << tetFmap[f.getLocalFaceId()] << ", " <<
								std::showpoint << pres << std::endl;
							break;
						case kmb::HEXAHEDRON:
						case kmb::HEXAHEDRON2:
							output << " " << eIter.getId() + this->offsetElementId << ", " <<
								"P" << hexFmap[f.getLocalFaceId()] << ", " <<
								std::showpoint << pres << std::endl;
							break;
						case kmb::WEDGE:
						case kmb::WEDGE2:
							output << " " << eIter.getId() + this->offsetElementId << ", " <<
								"P" << wedFmap[f.getLocalFaceId()] << ", " <<
								std::showpoint << pres << std::endl;
							break;
						case kmb::PYRAMID:
						case kmb::PYRAMID2:
							output << " " << eIter.getId() + this->offsetElementId << ", " <<
								"P" << pyrFmap[f.getLocalFaceId()] << ", " <<
								std::showpoint << pres << std::endl;
							break;
						default:
							break;
						}
						++fIter;
					}
				}
				else if( data->getBindingMode() == kmb::DataBindings::FaceGroup ){
					data->getPhysicalValue()->getValue( &pres );
					output << " " << name << ", " <<
						std::showpoint << pres << std::endl;
				}
			}
			++dIter;
		}
	}
	if( fdata->steps.size() > 0 ){
		output << "#################" << std::endl;
		output << "# Step Controls #" << std::endl;
		output << "#################" << std::endl;
		std::vector<kmb::FrontISTRStep*>::const_iterator sIter = fdata->steps.begin();
		while( sIter != fdata->steps.end() ){
			output << **sIter;
			++sIter;
		}
	}
	output << "###################" << std::endl;
	output << "# Solver Controls #" << std::endl;
	output << "###################" << std::endl;
	output << fdata->solver;
	output << "#########################" << std::endl;
	output << "# Post Process Controls #" << std::endl;
	output << "#########################" << std::endl;
	output << fdata->visual;
	output << "!END" << std::endl;
	return EXIT_SUCCESS;
}

void kmb::FrontISTRIO::count(std::string filename)
{
	nodeCount = 0;
	elementCount = 0;
	std::ifstream input(filename, std::ios_base::in);
	if (input.fail()) {
		return;
	}
	std::string line;
	std::getline(input, line);
	while (!input.eof()) {
		if (line.find("!NODE") == 0) {
			while (!input.eof()) {
				std::getline(input, line);
				if (line.find("!") == 0) {
					break;
				}
				nodeCount++;
			}
		}
		else if (line.find("!ELEMENT") == 0) {
			while (!input.eof()) {
				std::getline(input, line);
				if (line.find("!") == 0) {
					break;
				}
				elementCount++;
			}
		}
	}
	input.close();
}

// template<typename MContainer>
kmb::elementType kmb::FrontISTRIO::getRevocapType(int etype)
{
	switch (etype)
	{
	case 111:	return kmb::SEGMENT;
	case 112:	return kmb::SEGMENT2;
	case 231:	return kmb::TRIANGLE;
	case 232:	return kmb::TRIANGLE2;
	case 241:	return kmb::QUAD;
	case 242:	return kmb::QUAD2;
	case 341:	return kmb::TETRAHEDRON;
	case 342:	return kmb::TETRAHEDRON2;
	case 351:	return kmb::WEDGE;
	case 352:	return kmb::WEDGE2;
	case 361:	return kmb::HEXAHEDRON;
	case 362:	return kmb::HEXAHEDRON2;
	case 371:	return kmb::PYRAMID;
	case 372:	return kmb::PYRAMID2;
	case 731:	return kmb::TRIANGLE;
	case 732:	return kmb::TRIANGLE2;
	case 741:	return kmb::QUAD;
	case 742:	return kmb::QUAD2;
	}
	return kmb::UNKNOWNTYPE;
}

void kmb::FrontISTRIO::writeDummyMaterial(std::ostream& output)
{
	output << "!MATERIAL, NAME=MAT, ITEM=1" << std::endl;
	output << "!ITEM=1, SUBITEM=2" << std::endl;
	output << " 0.0, 0.0" << std::endl;
}


