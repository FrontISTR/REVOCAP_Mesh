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

#include "Geometry/kmbIdTypes.h"
#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElement.h"
#include <string>
#include <vector>
#include <ostream>

namespace kmb{

class FrontISTRIO{
protected:
	kmb::nodeIdType offsetNodeId;
	kmb::elementIdType offsetElementId;
	size_t nodeCount;
	size_t elementCount;
	int getGroupId(std::string name,std::string stype);
	// 面の変換 Revo => Fistr
	static int tetFmap[];
	static int wedFmap[];
	static int hexFmap[];
	static int pyrFmap[];
	// 面の変換 Fistr => Revo
	static int tetRmap[];
	static int wedRmap[];
	static int hexRmap[];
	static int pyrRmap[];
public:
	FrontISTRIO();
	template<typename MeshT>
	int loadMesh(std::string filename, MeshT* mesh);
	template<typename MeshT>
	int saveMesh(std::string filename, const MeshT* mesh);
	template<typename MeshT>
	int loadResult(std::string filename, MeshT* mesh);
	kmb::elementIdType getOffsetElementId(void) const;
	void setOffsetElementId(kmb::elementIdType offset);
protected:
	// 内部の変数に個数を代入する
	void count(std::string filename);
	static kmb::elementType getRevocapType(int etypeHec);
	template<typename MeshT>
	int loadResultBinary(std::string filename, MeshT* mesh);
	template<typename MeshT>
	int loadResultAscii(std::string filename, MeshT* mesh);
};

}
