/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : BLMGenerator                                       #
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
=RevocapMeshGen::BLMGenerator

==概要

RevocapMesh のメッシュ管理クラス MeshDB に格納された表面メッシュから
境界層メッシュを生成するためのクラスである。

使用例：
 blm = BLMGenerator.new
 blm.setLayer([0.1,0.2,0.3])
 blmid = blm.generate(mesh,0)

==メソッド一覧

((<BLMGenerator.new>))
((<setLayer>))
((<extrude>))
((<intrude>))

==メソッド

=end
------------------------------------------------------------------------------*/
%{
#include "MeshGen/kmbBLMGenerator.h"
%}

namespace kmb{

class MeshData;

class BLMGenerator
{
public:
/**--------------------------------------------------------------------------
=begin
--- BLMGenerator.new
	境界層生成モジュールクラス BLMGenerator のインスタンスを生成する。
=end
---------------------------------------------------------------------------*/
	BLMGenerator(void);
	virtual ~BLMGenerator(void);
/**--------------------------------------------------------------------------
=begin
--- setLayer(layers)
	layers で生成する境界層の厚さ（積算したもの）を配列で与える。
	例：blm.setLayer([0.1,0.2,0.3])
	この場合は、表面メッシュから 0.1 0.2 0.3 の高さのところに境界層を
	与えようとするため、厚さはすべて 0.1 となる。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE setLayer(VALUE layers){
		Check_Type(layers, T_ARRAY);
		int layerNum = RARRAY_LEN(layers);
		double* layerThick = NULL;
		layerThick = new double[layerNum];
		for(int i=0;i<layerNum;++i){
			layerThick[i] = NUM2DBL(rb_ary_entry(layers,i));
		}
		self->setLayer(layerNum,layerThick);
		delete layerThick;
		return Qnil;
	}
}
/**--------------------------------------------------------------------------
=begin
--- extrude(mesh,bodyId)
	mesh の bodyId で与えられる表面メッシュに対して境界層メッシュを生成する。
	戻り値は境界層の外部表面メッシュのBodyIdと境界層メッシュのBodyIdを要素に持つ
	Array 値である。
	例：blm.generate( mesh,0 ) => [1,2]
	このとき BodyId 1 は外部表面メッシュの要素グループIdで BodyId 2 が三角柱要素による境界層の要素グループIdである。
--- intrude(mesh,parentId,bodyId)
	mesh の bodyId で与えられる表面メッシュに対して境界層メッシュを生成する。
	戻り値は境界層の外部表面メッシュのBodyIdと境界層メッシュのBodyIdを要素に持つ
	Array 値である。
	例：blm.generate( mesh,0 ) => [1,2]
	このとき BodyId 1 は外部表面メッシュの要素グループIdで BodyId 2 が三角柱要素による境界層の要素グループIdである。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE extrude(MeshData* mesh,kmb::bodyIdType bodyId){
		self->setMesh(mesh);
		kmb::bodyIdType layerId;
		kmb::bodyIdType boundaryId = bodyId;
		layerId = self->extrude(boundaryId);
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,INT2FIX(boundaryId));
		rb_ary_store(ary,1,INT2FIX(layerId));
		return ary;
	}
	VALUE intrude(MeshData* mesh,kmb::bodyIdType parentId,kmb::bodyIdType bodyId){
		self->setMesh(mesh);
		kmb::bodyIdType layerId;
		kmb::bodyIdType boundaryId = bodyId;
		layerId = self->intrude(parentId,boundaryId);
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,INT2FIX(boundaryId));
		rb_ary_store(ary,1,INT2FIX(layerId));
		return ary;
	}
}

};

}
