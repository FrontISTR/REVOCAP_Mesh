/*----------------------------------------------------------------------
#                                                                      #
<<<<<<< HEAD:Refiner/rcapRefinerStruct.h
# Software Name : REVOCAP_Refiner version 1.1                          #
# Program Name : rcapRefinerStruct                                     #
=======
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MatrixIndex                                             #
>>>>>>> master:Matrix/kmbMatrixIndex.h
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2016/09/19     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/
<<<<<<< HEAD:Refiner/rcapRefinerStruct.h
#ifndef REVOCAP_REFINER_STRUCT_HEADER
#define REVOCAP_REFINER_STRUCT_HEADER

#include "rcapRefiner.h"
#include "Geometry/kmbIdTypes.h"

#include <vector>

namespace kmb{
	class MeshData;
	class MeshRefiner;
	class MiddleNodeManager;
	class Surface3D;
	class MeshSmoother;
}

struct rcapRefinerInfo
{
	kmb::MeshData* mesh;
	kmb::MeshRefiner* refiner;
	kmb::MeshRefiner* boundaryRefiner;
	kmb::MiddleNodeManager* middleMan;
	kmb::MeshSmoother* smoother;
	std::vector< kmb::Surface3D* > surfaces;
	int32_t nodeOffset;
	int32_t elementOffset;
	int32_t maxElementId;
	int32_t maxRefinedElementId;
};

#endif /* REVOCAP_REFINER_STRUCT_HEADER */
=======

#pragma once

namespace kmb {

	// 順序は Row Major
	// i*colSize+j
	class MatrixIndex {
	public:
		int rowIndex;
		int colIndex;
		MatrixIndex(int i, int j)
			: rowIndex(i)
			, colIndex(j)
		{}
		bool operator<(const MatrixIndex &other)const {
			return (rowIndex < other.rowIndex) ||
				(rowIndex == other.rowIndex && colIndex < other.colIndex);
		}
	};

}
>>>>>>> master:Matrix/kmbMatrixIndex.h
