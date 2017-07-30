/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.7                          #
# Class Name : FrontISTRIO                                             #
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
#pragma once

#include "RevocapIO/kmbFrontISTRIO.h"
#include "RevocapIO/kmbFrontISTRMaterial.h"
#include <fstream>
#include <typeinfo>

namespace kmb{

inline std::ostream& operator<<(std::ostream &os, const kmb::FrontISTRData::Orientation &ori)
{
	os << "!ORIENTATION, NAME=" << ori.name << ", ";
	if( typeid(ori) == typeid(const kmb::FrontISTRData::OrientationByCoordinates)){
		os << "DEFINITION=COORDINATES" << std::endl;
	}else if( typeid(ori) == typeid(const kmb::FrontISTRData::OrientationByNodes)){
		os << "DEFINITION=NODES" << std::endl;
	}else{
	}
	return os;
}

inline std::ostream& operator<<(std::ostream &os, const kmb::FrontISTRStep &step)
{
	os << "!STEP";
	switch(step.type){
	case kmb::FrontISTRStep::kSTATIC:
		os << ", TYPE=STATIC";
		break;
	case kmb::FrontISTRStep::kVISCO:
		os << ", TYPE=VISCO";
		break;
	default:
		break;
	}
	os << ", SUBSTEPS=" << step.substeps;
	os << ", CONVERG=" << step.converg;
	os << ", MAXITER=" << step.maxiter;
	os << std::endl;
	os << " " << step.dtime << ", " << step.etime << std::endl;
	std::vector<int>::const_iterator bIter = step.boundaries.begin();
	while( bIter != step.boundaries.end() ){
		os << " BOUNDARY, " << *bIter << std::endl;
		++bIter;
	}
	std::vector<int>::const_iterator lIter = step.loads.begin();
	while( lIter != step.loads.end() ){
		os << " LOAD, " << *lIter << std::endl;
		++lIter;
	}
	std::vector<int>::const_iterator cIter = step.contacts.begin();
	while( cIter != step.contacts.end() ){
		os << " CONTACT, " << *cIter << std::endl;
		++cIter;
	}
	return os;
}

inline std::ostream& operator<<(std::ostream &os, const FrontISTRAnalysis &analysis)
{
	analysis.output(os);
	return os;
}

inline std::ostream& operator<<(std::ostream &os, const kmb::FrontISTRSolver &solver)
{
	os << "!SOLVER";
	switch(solver.method){
	case kmb::FrontISTRSolver::kCG: os << ", METHOD=CG"; break;
	case kmb::FrontISTRSolver::kBiCGSTAB: os << ", METHOD=BiCGSTAB"; break;
	case kmb::FrontISTRSolver::kGMRES: os << ", METHOD=GMRES"; break;
	case kmb::FrontISTRSolver::kGPBiCG: os << ", METHOD=GPBiCG"; break;
	case kmb::FrontISTRSolver::kDIRECT: os << ", METHOD=DIRECT"; break;
	case kmb::FrontISTRSolver::kDIRECTmkl: os << ", METHOD=DIRECTmkl"; break;
	case kmb::FrontISTRSolver::kMUMPS: os << ", METHOD=MUMPS"; break;
	default: break;
	}
	os << ", PRECOND=" << solver.precond;
	if( solver.iterLog ) os << ", ITERLOG=YES";
	if( solver.timeLog ) os << ", TIMELOG=YES";
	if( solver.useJAD ) os << ", USEJAD=YES";
	if( solver.scaling ) os << ", SCALING=YES";
	switch( solver.dumpType ){
	case kmb::FrontISTRSolver::kMM: os << ", DUMPTYPE=MM";
	case kmb::FrontISTRSolver::kCSR: os << ", DUMPTYPE=CSR";
	case kmb::FrontISTRSolver::kBSR: os << ", DUMPTYPE=BSR";
	default: break;
	}
	if( solver.dumpExit ) os << ", DUMPEXIT=YES";
	if( solver.mpcMethod != kmb::FrontISTRSolver::kUNDEF ) os << ", MPCMETHOD=" << solver.mpcMethod;
	if( solver.estCond > 0 ) os << ", ESTCOND=" << solver.estCond;
	os << std::endl;
	if( solver.method == kmb::FrontISTRSolver::kCG ||
		solver.method == kmb::FrontISTRSolver::kBiCGSTAB ||
		solver.method == kmb::FrontISTRSolver::kGMRES ||
		solver.method == kmb::FrontISTRSolver::kGPBiCG )
	{
		os << " " << solver.nIter << ", " << solver.iterPREmax << ", " << solver.nRest << ", " << solver.nColor_in << std::endl;
		os << " " << solver.residue << ", " << solver.sigma_diag << std::endl;
	}
	return os;
}

inline std::ostream& operator<<(std::ostream &os, const kmb::FrontISTRVisual &visual)
{
	os << "!VISUAL";
	switch( visual.method ){
	case kmb::FrontISTRVisual::kPSR: os << ", METHOD=PSR"; break;
	default: break;
	}
	if( visual.start_step > 0 ) os << ", visual_start_step=" << visual.start_step;
	if( visual.end_step > 0 ) os << ", visual_end_step=" << visual.end_step;
	if( visual.interval_step > 0 ) os << ", visual_interval_step=" << visual.interval_step;
	os << std::endl;
	os << "!surface_num = " << visual.surfaces.size() << std::endl;
	std::vector< kmb::FrontISTRVisual::Surface* >::const_iterator sIter = visual.surfaces.begin();
	while( sIter != visual.surfaces.end() ){
		(*sIter)->output(os);
		++sIter;
	}
	visual.output->output(os);
	return os;
}

inline std::ostream& operator<<(std::ostream &os, const kmb::FrontISTRData &dat)
{
	os << "!VERSION" << std::endl;
	os << " " << dat.version << std::endl;
	os << "!SOLUTION, TYPE=";
	switch(dat.solution){
	case kmb::FrontISTRData::kSTATIC: os << "STATIC" << std::endl; break;
	case kmb::FrontISTRData::kNLSTATIC: os << "NLSTATIC" << std::endl; break;
	case kmb::FrontISTRData::kHEAT: os << "HEAT" << std::endl; break;
	case kmb::FrontISTRData::kEIGEN: os << "EIGEN" << std::endl; break;
	case kmb::FrontISTRData::kDYNAMIC: os << "DYNAMIC" << std::endl; break;
	case kmb::FrontISTRData::kSTATICEIGEN: os << "STATICEIGEN" << std::endl; break;
	case kmb::FrontISTRData::kELEMCHECK: os << "ELEMCHECK" << std::endl; break;
	default: os << "STATIC" << std::endl; break;
	}
	if( dat.write_visual.frequency > 0 ){
		os << "!WRITE, VISUAL, FREQUENCY=" << dat.write_visual.frequency << std::endl;
		os << "!OUTPUT_VIS" << std::endl;
		os << " DISP, " << ((dat.output_vis.disp) ? "ON" : "OFF") << std::endl;
		os << " ROT, " << ((dat.output_vis.rot) ? "ON" : "OFF") << std::endl;
		os << " NSTRAIN, " << ((dat.output_vis.nstrain) ? "ON" : "OFF") << std::endl;
		os << " NSTRESS, " << ((dat.output_vis.nstress) ? "ON" : "OFF") << std::endl;
		os << " NMISES, " << ((dat.output_vis.nmises) ? "ON" : "OFF") << std::endl;
		os << " VEL, " << ((dat.output_vis.vel) ? "ON" : "OFF") << std::endl;
		os << " ACC, " << ((dat.output_vis.acc) ? "ON" : "OFF") << std::endl;
	}
	if( dat.write_result.frequency > 0 ){
		os << "!WRITE, RESULT, FREQUENCY=" << dat.write_result.frequency << std::endl;
		os << "!OUTPUT_RES" << std::endl;
		os << " DISP, " << ((dat.output_res.disp) ? "ON" : "OFF") << std::endl;
		os << " ROT, " << ((dat.output_res.rot) ? "ON" : "OFF") << std::endl;
		os << " REACTION, " << ((dat.output_res.reaction) ? "ON" : "OFF") << std::endl;
		os << " NSTRAIN, " << ((dat.output_res.nstrain) ? "ON" : "OFF") << std::endl;
		os << " NSTRESS, " << ((dat.output_res.nstress) ? "ON" : "OFF") << std::endl;
		os << " NMISES, " << ((dat.output_res.nmises) ? "ON" : "OFF") << std::endl;
		os << " ESTRAIN, " << ((dat.output_res.estrain) ? "ON" : "OFF") << std::endl;
		os << " ESTRESS, " << ((dat.output_res.estress) ? "ON" : "OFF") << std::endl;
		os << " EMISES, " << ((dat.output_res.emises) ? "ON" : "OFF") << std::endl;
		os << " ISTRAIN, " << ((dat.output_res.istrain) ? "ON" : "OFF") << std::endl;
		os << " ISTRESS, " << ((dat.output_res.istress) ? "ON" : "OFF") << std::endl;
		os << " PL_ISTRAIN, " << ((dat.output_res.pl_istrain) ? "ON" : "OFF") << std::endl;
		os << " VEL, " << ((dat.output_res.vel) ? "ON" : "OFF") << std::endl;
		os << " ACC, " << ((dat.output_res.acc) ? "ON" : "OFF") << std::endl;
	}
	if( dat.write_log.frequency > 0 ) os << "!WRITE, LOG, FREQUENCY=" << dat.write_log.frequency << std::endl;
	if( dat.restart.frequency != 0 ) os << "!RESTART, FREQUENCY=" << dat.restart.frequency << std::endl;
	if( dat.echo ) os << "!ECHO" << std::endl;
	std::vector< kmb::FrontISTRData::Orientation* >::const_iterator oIter = dat.orientations.begin();
	while( oIter != dat.orientations.end() ){
		kmb::FrontISTRData::Orientation* ori = *oIter;
		ori->output(os);
		++oIter;
	}
	std::vector< kmb::FrontISTRData::Section* >::const_iterator sIter = dat.sections.begin();
	while( sIter != dat.sections.end() ){
		kmb::FrontISTRData::Section* section = *sIter;
		os << "!SECTION, SECNUM=" << section->secnum << ", ORIENTATION=" << section->orientation << std::endl;
		++sIter;
	}
	std::vector< kmb::FrontISTRMaterial* >::const_iterator mIter = dat.materials.begin();
	while( mIter != dat.materials.end() ){
		kmb::FrontISTRMaterial* material = *mIter;
		os << *material;
		++mIter;
	}
	return os;
}

}
