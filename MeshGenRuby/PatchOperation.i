/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : PatchOperation                                     #
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
=RevocapMesh::PatchOperation

RevocapMesh::PatchOperation クラスは
RevocapMesh::MeshDB に格納された表面パッチに対して、すき間を埋める、
向きをそろえるなどの位相処理・幾何処理を行うためのクラスである。

形状データ (IGES) からパッチデータを生成するには、このクラスではなく
RevocapPatch モジュールを使う。

==メソッド一覧

((<PatchOperation.new>))
((<uniformOrientation>))
((<isClosed>))
((<subdivideByEdge>))
((<subdivideByEdgeWithPatch>))
((<divideByPlane>))

=end
------------------------------------------------------------------------------*/

%{
#include "MeshGen/kmbPatchOperation.h"
%}

namespace kmb{

class PatchOperation
{
public:
/**--------------------------------------------------------------------------
=begin
--- PatchOperation.new
	パッチ処理クラス PatchOperation のインスタンスを生成する。
=end
---------------------------------------------------------------------------*/
	PatchOperation(void);
	virtual ~PatchOperation(void);
/**--------------------------------------------------------------------------
=begin
--- uniformOrientation(mesh,patch,elementId=-1)
	MeshDB の bodyId が patch で与えられる表面パッチに対して、
	要素の向きを一元化する。
	戻り値は向きを変えた要素の個数。
	elementId は基準となる要素の Id を与え、その要素と隣接している要素の向きを
	そろえていく。省略した場合は自動的に決める。
	表面パッチが連結していない時には elementId を明示的に与える必要がある。
=end
---------------------------------------------------------------------------*/
	int uniformOrientation(kmb::MeshDB* mesh,kmb::bodyIdType bodyId,kmb::elementIdType elementId=kmb::Element::nullElementId);
/**--------------------------------------------------------------------------
=begin
--- subdivideByEdge(mesh,edgeId,tolerance=Float::MAX,globalTolerance=Float::MAX)
	edgeId 上の節点で他の１次元 Body 上の線分を細分する。
	戻り値は edge と形状マッチする mesh の 1 次元 Body の Id を返す。
--- subdivideByEdgeWithPatch(mesh,edgeId,tolerance=Float::MAX,globalTolerance=Float::MAX)
	edgeId 上の節点で他の１次元 Body 上の線分を細分する。
	さらにその１次元 Body を境界に持つような三角形についても edgeId 上の節点で細分する。
	戻り値は edge と形状マッチする mesh の 1 次元 Body の Id を返す。
=end
-----------------------------------------------------------------------------*/
	kmb::bodyIdType subdivideByEdge(kmb::MeshDB* mesh,kmb::bodyIdType edgeId,double tolerance=DBL_MAX,double globalTolerance=DBL_MAX);
	kmb::bodyIdType subdivideByEdgeWithPatch(kmb::MeshDB* mesh,kmb::bodyIdType edgeId,double tolerance=DBL_MAX,double globalTolerance=DBL_MAX);
/**--------------------------------------------------------------------------
=begin
--- divideByPlane(mesh,bodyId,a,b,c,d,cap=false,duplicate=false)
--- divideByPlane(mesh,bodyId,plane,cap=false,duplilcate=false)
	三角形からなる Body を ax+by+cz+d=0 の平面で分割する。
	plane は RevocapMesh::FramedPlane.new(a,b,c,d) で生成する
	cap = true のときは分割した面にふたをする。
	duplicate = true のときは、分割した面の節点を二重化する。
	分割した正領域と負領域の三角形 BodyID の組と
	ふたをするのに成功したかの真偽値の3つ組みを返す。
=end
-----------------------------------------------------------------------------*/
%extend{
	VALUE divideByPlane(kmb::MeshDB* mesh,kmb::bodyIdType bodyId,double a,double b,double c,double d,bool capFlag=false,bool duplicateFlag=false){
		kmb::FramedPlane plane(a,b,c,d);
		VALUE ary = rb_ary_new();
		kmb::bodyIdType positiveID;
		kmb::bodyIdType negativeID;
		bool capped = self->divideByPlane(mesh,bodyId,plane,positiveID,negativeID,capFlag,duplicateFlag);
		rb_ary_push(ary,INT2NUM(positiveID));
		rb_ary_push(ary,INT2NUM(negativeID));
		rb_ary_push(ary,(capped ? Qtrue : Qfalse));
		return ary;
	}
	VALUE divideByPlane(kmb::MeshDB* mesh,kmb::bodyIdType bodyId,kmb::FramedPlane &plane,bool capFlag=false,bool duplicateFlag=false){
		VALUE ary = rb_ary_new();
		kmb::bodyIdType positiveID;
		kmb::bodyIdType negativeID;
		bool capped = self->divideByPlane(mesh,bodyId,plane,positiveID,negativeID,capFlag,duplicateFlag);
		rb_ary_push(ary,INT2NUM(positiveID));
		rb_ary_push(ary,INT2NUM(negativeID));
		rb_ary_push(ary,(capped ? Qtrue : Qfalse));
		return ary;
	}
}
};

}
