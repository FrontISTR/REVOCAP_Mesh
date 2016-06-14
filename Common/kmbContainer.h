/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.7                          #
# Common Definitions                                                   #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2016/03/25     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/

#pragma once

#include <vector>

namespace kmb{

template<class T>
void clearVector(std::vector<T*> container){
	std::vector< T* >::iterator iter = container.begin();
	while( iter != container.end() ){
		T* t = *iter;
		if( t != NULL ){
			delete t;
		}
		++iter;
	}
	container.clear();
}

}
