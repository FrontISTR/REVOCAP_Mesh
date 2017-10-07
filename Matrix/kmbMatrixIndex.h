/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MatrixIndex                                             #
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
