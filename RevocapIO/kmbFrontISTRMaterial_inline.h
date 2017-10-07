/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.7                          #
# Class Name : FrontISTRMaterial                                       #
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

#include "RevocapIO/kmbFrontISTRMaterial.h"
#include <fstream>
#include <typeinfo>

namespace kmb{

inline std::ostream& operator<<(std::ostream &os, const kmb::FrontISTRMaterialItem &mat)
{
	mat.output(os);
	return os;
}

inline std::ostream& operator<<(std::ostream &os, const kmb::FrontISTRMaterial &mat)
{
	os << "!MATERIAL, NAME=" << mat.name << std::endl;
	std::vector< kmb::FrontISTRMaterialItem* >::const_iterator mIter = mat.materials.begin();
	while( mIter != mat.materials.end() ){
		const kmb::FrontISTRMaterialItem* mat = *mIter;
		os << (*mat);
		++mIter;
	}
	return os;
}


}
