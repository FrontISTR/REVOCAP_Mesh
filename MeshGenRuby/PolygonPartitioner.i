/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : PolygonPartitioner                                 #
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
/*------------------------------------------------------------------------------
=begin
=RevocapMesh::PolygonPartition

=end
---------------------------------------------------------------------------*/

%{
#include "MeshGen/kmbPolygonPartitioner.h"
%}

namespace kmb{

class PolygonPartitioner
{
public:
	PolygonPartitioner(void);
	virtual ~PolygonPartitioner(void);
	kmb::bodyIdType partitionToTriangles(kmb::MeshDB* mesh,kmb::bodyIdType edgeId);
	kmb::bodyIdType partitionToTriangles(kmb::MeshDB* mesh,kmb::bodyIdType edgeId,const kmb::FramedPlane &plane);
};

}
