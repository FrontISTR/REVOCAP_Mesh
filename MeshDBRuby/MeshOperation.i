/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.7                          #
# Ruby Class Name : MeshOperation                                      #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2013/05/29     #
#                                                                      #
----------------------------------------------------------------------*/

%{
#include "MeshDB/kmbMeshOperation.h"
%}


namespace kmb{

class MeshOperation
{
public:
	MeshOperation(kmb::MeshData* _mesh);
	virtual ~MeshOperation(void);
	size_t nodeSplitByVolumes(void);
};

}
