/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : Match                                              #
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
=RevocapMesh::Matching

RevocapMesh::MeshDB に格納されたメッシュに対する形状マッチング処理を行うためのクラスである

==メソッド一覧

((<Matching.new>))
((<getDistanceEdgeToNode>))
((<getDistanceEdgeToEdge>))
((<getDistanceDataToData>))
((<getMatchingEdgePair>))
((<getSurfaceRelation>))
((<getFaceRelation>))
((<nodeMatchingOnBody>))
((<nodeMatchingBetweenBodies>))

=end
------------------------------------------------------------------------------*/

%{
#include "MeshDB/kmbMatching.h"
%}

namespace kmb{

class MeshDB;

class Matching
{
public:
/**--------------------------------------------------------------------------
=begin
--- Matching.new
	形状マッチング処理クラス Matching のインスタンスを生成する。
=end
---------------------------------------------------------------------------*/
	Matching(void);
	virtual ~Matching(void);
/**--------------------------------------------------------------------------
=begin
--- getDistanceEdgeToNode( mesh, bodyId, nodeId )
	MeshDB mesh の bodyId で与えられる1次元要素グループに対して、
	節点ID nodeId で与えられる節点との距離を返す。
	bodyId で与えられる要素グループが1次元でない時、nodeId で表わされる
	節点が存在しないときなどは、無限大を返す。
=end
---------------------------------------------------------------------------*/
	double getDistanceEdgeToNode( kmb::MeshDB* mesh, kmb::bodyIdType bodyId, kmb::nodeIdType nodeId );
/**--------------------------------------------------------------------------
=begin
--- getDistanceEdgeToEdge( mesh, bodyId0, bodyId1 )
	MeshDB mesh の bodyId0, bodyId1 で与えられる1次元要素グループに対して、
	それらのなす距離を返す。
	ここでの1次元要素グループ間の距離の定義は
	 1/2 * ( 節点に関する平均 (bodyId0 の節点と bodyId1 の距離) + 節点に関する平均 (bodyId1 の節点と bodyId0 の距離) )
	である。
--- getDistanceEdgeToEdge( mesh0, bodyId0, mesh1, bodyId1 )
	MeshDB mesh0 の bodyId0、MeshDB mesh1 の bodyId1 で与えられる1次元要素グループに対して、
	それらのなす距離を返す。
	ここでの1次元要素グループ間の距離の定義は
	 1/2 * ( 節点に関する平均 (bodyId0 の節点と bodyId1 の距離) + 節点に関する平均 (bodyId1 の節点と bodyId0 の距離) )
	である。
=end
---------------------------------------------------------------------------*/
	double getDistanceEdgeToEdge( kmb::MeshDB* mesh, kmb::bodyIdType edgeId0, kmb::bodyIdType edgeId1 );
	double getDistanceEdgeToEdge( kmb::MeshDB* mesh0, kmb::bodyIdType edgeId0, kmb::MeshDB* mesh1, kmb::bodyIdType edgeId1 );
/**-----------------------------------------------------------------------
=begin
--- getDistanceDataToData(mesh,key0,key1)
	key0 と key1 で与えられる Data の BoundingBox の間の距離を求める。
=end
--------------------------------------------------------------------------*/
	double getDistanceDataToData(kmb::MeshDB* mesh,const char* key0,const char* key1);

/**--------------------------------------------------------------------------
=begin
--- getMatchingEdgePair( mesh0, mesh1, tolerance )
	MeshDB mesh0 と mesh1 に含まれる1次元要素グループ間の距離が tolerance 以下になるような
	組を探索して、それを bodyId の Array の Array で返す。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getMatchingEdgePair( kmb::MeshDB& mesh0, kmb::MeshDB &mesh1, double tolerance ){
		VALUE ary = rb_ary_new();
		kmb::bodyIdType bodyCount0 = mesh0.getBodyCount();
		kmb::bodyIdType bodyCount1 = mesh1.getBodyCount();
		for(kmb::bodyIdType bodyId0 = 0; bodyId0 < bodyCount0; ++bodyId0){
			for(kmb::bodyIdType bodyId1 = 0; bodyId1 < bodyCount1; ++bodyId1){
				if( self->getDistanceEdgeToEdge( &mesh0, bodyId0, &mesh1, bodyId1 ) < tolerance ){
					VALUE item = rb_ary_new();
					rb_ary_push( item, INT2NUM(bodyId0) );
					rb_ary_push( item, INT2NUM(bodyId1) );
					rb_ary_push( ary, item );
				}
			}
		}
		return ary;
	}
}
/**-----------------------------------------------------------------------
=begin
--- getSurfaceRelation( mesh, bodyID0, bodyID1 )
	平面要素のみからなる二つのボディに対して、一致していたら 1
	逆向きなら -1 それ以外なら 0 を返す。
	形状ではなく、三角形の位相から判別する。
=end
--------------------------------------------------------------------------*/
%extend{
	int getSurfaceRelation( kmb::MeshDB* mesh, kmb::bodyIdType surfId0, kmb::bodyIdType surfId1 ){
		kmb::ElementRelation::relationType rel =
			self->getSurfaceRelation( mesh, surfId0, surfId1 );
		switch(rel){
			case kmb::ElementRelation::EQUAL:
				return 1;
			case kmb::ElementRelation::REVERSE:
				return -1;
			default:
				return 0;
		}
	}
}
/**-----------------------------------------------------------------------
=begin
--- getFaceRelation( mesh, face0, face1 )
	FaceGroup の二つの Data に対して平面として一致していたら 1
	逆向きなら -1 それ以外なら 0 を返す。
=end
--------------------------------------------------------------------------*/
%extend{
	int getFaceRelation( kmb::MeshDB* mesh, const char* faceId0, const char* faceId1 ){
		kmb::ElementRelation::relationType rel =
			self->getFaceRelation( mesh, faceId0, faceId1 );
		switch(rel){
			case kmb::ElementRelation::EQUAL:
				return 1;
			case kmb::ElementRelation::REVERSE:
				return -1;
			default:
				return 0;
		}
	}
}
/**-----------------------------------------------------------------------
=begin
--- nodeMatchingOnBody(mesh0, bodyId0, mesh1, tolerance, coupleName)
	mesh0 の bodyId0 上の節点を mesh1 のすべての節点の中から
	距離が tolerance 以下のものを探して、mesh0 の coupleName の Data に登録する。
	 mesh0.setPhysicalValueAtId( coupleName, mesh0 の nodeId, mesh1 の nodeId )
	coupleName の Data が NodeVariable Integer 値で既に存在する場合はその Data に追加する。
	他のモードや他の値の Data が存在する場合は何もしない。
	戻り値は登録した値の個数。
=end
--------------------------------------------------------------------------*/
	int nodeMatchingOnBody(kmb::MeshDB* mesh0, kmb::bodyIdType bodyId0, kmb::MeshDB* mesh1, double tolerance, const char* coupleName);
/**-----------------------------------------------------------------------
=begin
--- nodeMatchingBetweenBodies(mesh0, bodyId0, mesh1, bodyId1, tolerance, coupleName)
	mesh0 の bodyId0 上の節点を mesh1 の bodyId1 上の節点の中から
	距離が tolerance 以下のものを探して、mesh0 の coupleName の Data に登録する。
	 mesh0.setPhysicalValueAtId( coupleName, mesh0 の nodeId, mesh1 の nodeId )
	coupleName の Data が NodeVariable Integer 値で既に存在する場合はその Data に追加する。
	他のモードや他の値の Data が存在する場合は何もしない。
	戻り値は登録した値の個数。
=end
--------------------------------------------------------------------------*/
	int nodeMatchingBetweenBodies(kmb::MeshDB* mesh0, kmb::bodyIdType bodyId0, kmb::MeshDB* mesh1, kmb::bodyIdType bodyId1, double tolerance, const char* coupleName);

};

}

%{
#include "MeshDB/kmbSurfaceMatching.h"
%}

namespace kmb{

class MeshData;

class SurfaceMatching
{
public:
	SurfaceMatching(void);
	virtual ~SurfaceMatching(void);
/**-----------------------------------------------------------------------
=begin
--- setMesh(mesh,insNodes=nil)
	mesh に登録してある面グループと表面要素でマッチングを行う。
	insNodes はダミー要素を追加したときに追加した節点を覚えておくためのデータ名。
	insNodes の NodeGroup がなければ自動的に作る。
=end
--------------------------------------------------------------------------*/
	void setMesh(kmb::MeshData* mesh,const char* insNodes=NULL);
	void setPair(kmb::bodyIdType bodyId,const char* faceGroup);
	size_t constructDummyElements(void);
%extend{
	VALUE getMatchingElementId(VALUE v){
		VALUE ary = rb_ary_new();
		int index = -1;
		kmb::elementIdType elemId = self->getMatchingElementId( faceFromVALUE(v), index );
		if(elemId != kmb::Element::nullElementId){
			rb_ary_store( ary, 0, INT2FIX(elemId) );
			rb_ary_store( ary, 1, INT2FIX(index) );
			return ary;
		}else{
			return Qnil;
		}
	}
}

};

}
