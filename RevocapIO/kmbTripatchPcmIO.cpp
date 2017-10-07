/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : TripatchPcmIO                                           #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
# Software Name : RevocapMesh version 1.2                              #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2008/03/14     #
#                                           T. Takeda   2008/03/14     #
#                                           K. Amemiya  2008/03/14     #
#                                                                      #
#      Contact Address: RSS21 Project, IIS, The University of Tokyo    #
#                                                                      #
#      "Innovative General-Purpose Coupled Analysis System"            #
#                                                                      #
----------------------------------------------------------------------*/
#include "RevocapIO/kmbTripatchPcmIO.h"
#include "RevocapIO/kmbTripatchIO.h"

int kmb::TripatchPcmIO::loadFromFile(const char* filename,MeshData* mesh)
{
	kmb::TripatchIO tripatch;
	return tripatch.loadPatch(filename,mesh);
}

// packOption = 0 : そのまま出力
// packOption = 1 : すべてを一つにまとめて出力
// 空の body は出力しない
int
kmb::TripatchPcmIO::saveToFile(const char* filename,const kmb::MeshData* mesh,int packOption)
{
	if( packOption == 0 ){
		kmb::TripatchIO tripatch;
		return tripatch.savePatch(filename,mesh);
	}else if( packOption == 1 ){
		kmb::TripatchIO tripatch;
		return tripatch.savePatchPacked(filename,mesh);
	}else{
		return -1;
	}
}

