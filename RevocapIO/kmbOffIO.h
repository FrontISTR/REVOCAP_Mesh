/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.7                          #
# Class Name : OffIO                                                   #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2015/12/15     #
#                                                                      #
#      Contact Address: RSS21 Project, IIS, The University of Tokyo    #
#                                                                      #
#      "Innovative General-Purpose Coupled Analysis System"            #
#                                                                      #
----------------------------------------------------------------------*/
#pragma once

namespace kmb{

class MeshData;

class OffIO
{
public:
	OffIO(void){};
	~OffIO(void){};
	template<typename MeshT>
	int loadFile(const char* filename,MeshT* mesh);
	template<typename MeshT>
	int saveFile(const char* filename,const MeshT* mesh);
};

}
