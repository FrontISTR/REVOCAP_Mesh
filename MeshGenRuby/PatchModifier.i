/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : PatchModifier                                      #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
=begin
=RevocapMeshGen::PatchModifier

==概要

表面パッチの修正を行うためのクラス。

==メソッド一覧

((<PatchModifier.new>))


==メソッド

=end
------------------------------------------------------------------------------*/
%{
#include "MeshGen/kmbPatchModifier.h"
%}

namespace kmb{

class MeshData;

class PatchModifier
{
public:
/**--------------------------------------------------------------------------
=begin
--- PatchModifier.new(mesh,bodyId)
    mesh の bodyId を与えて、表面パッチの処理を行う。
=end
---------------------------------------------------------------------------*/
	PatchModifier(kmb::MeshData* mesh,kmb::bodyIdType bodyId);
	virtual ~PatchModifier(void);
/**--------------------------------------------------------------------------
=begin
--- triangleEdgeSwap(triangle0,triangle1)
	三角形 triangle0 と triangle1 について、これらが接している時に
	接している辺と対角線を交換する。
	[0,1,3] + [1,2,3] => [0,1,2] + [2,3,0]
	交換できたかの結果を true / false で返す。
=end
-----------------------------------------------------------------------------*/
	bool triangleEdgeSwap(kmb::elementIdType tri0, kmb::elementIdType tri1 );
/**--------------------------------------------------------------------------
=begin
--- edgeContract(nodeId0,nodeId1)
	nodeId1 を nodeId0 に潰す
=end
-----------------------------------------------------------------------------*/
	bool edgeContract(kmb::nodeIdType node0, kmb::nodeIdType node1);
/**--------------------------------------------------------------------------
=begin
--- quadEdgeSwap(quad0, quad1, orientation=true)
	四角形 quad0 と quad1 について、これらが接している時に、辺を交換する。
	辺の交換の仕方は２通りあるため、orientation で区別する。
	[0,1,2,3] + [0,3,4,5] => [1,2,3,4] + [0,1,4,5]  (orientation=true)
                          => [0,1,2,5] + [2,3,4,5]  (orientation=false)
=end
-----------------------------------------------------------------------------*/
	bool quadEdgeSwap(kmb::elementIdType quad0, kmb::elementIdType quad1, bool orientation=true);
	kmb::elementIdType quadTrisect(kmb::elementIdType quad0, kmb::elementIdType quad1, bool orientation=true );
/**--------------------------------------------------------------------------
=begin
--- uniformOrientation(elemId=-1)
    パッチの面の向きをそろえる。
    elemId で基準となる三角形の Id を与える。
    戻り値は裏返した要素の個数。
=end
-----------------------------------------------------------------------------*/
	int uniformOrientation(kmb::elementIdType elementId=kmb::Element::nullElementId);


/**--------------------------------------------------------------------------
=begin
--- getAngleByIndex(elementId,index)
=end
-----------------------------------------------------------------------------*/
	double getAngleByIndex(kmb::elementIdType elementId,int index) const;
/**--------------------------------------------------------------------------
=begin
--- getNeighborElements(elementId)
--- getNeighborElement(face)
=end
-----------------------------------------------------------------------------*/
%extend{
	VALUE getNeighborElements(kmb::elementIdType elementId) const{
		VALUE ary = rb_ary_new();
		kmb::elementIdType neighbor[6];
		int res = self->getNeighborElements(elementId,neighbor);
		for(int i=0;i<res;++i){
			rb_ary_push(ary,INT2FIX( neighbor[i] ));
		}
		return ary;
	}
	kmb::elementIdType getNeighborElement(VALUE face) const{
		if( TYPE(face) == T_ARRAY ){
			kmb::elementIdType eId = FIX2INT(rb_ary_entry(face,0));
			kmb::idType lId = FIX2INT(rb_ary_entry(face,1));
			kmb::Face f(eId,lId);
			return self->getNeighborElement(f);
		}else{
			return kmb::Element::nullElementId;
		}
	}
}
};

}
