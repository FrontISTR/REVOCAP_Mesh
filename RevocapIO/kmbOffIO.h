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

class OffIO
{
public:
	OffIO(void){};
	~OffIO(void){};
	template<typename MContainer>
	int loadPatch(const char* filename,MContainer* mesh);
	template<typename MContainer>
	int savePatch(const char* filename,const MContainer* mesh);
};

}
