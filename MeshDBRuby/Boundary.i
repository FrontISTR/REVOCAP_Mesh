/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : BoundaryExtractor                                  #
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
=RevocapMesh::BoundaryExtractor

RevocapMesh::MeshDB に格納されたメッシュに対して、その境界面を抽出する
ための処理を行うクラスである。

RevocapMesh では、解析コードの要求に応じて、ボリュームメッシュから表面メッシュの
抽出方法として、表面を平面要素（三角形または四角形）で与える方法と、
立体要素と面番号の組で与える２通りのやり方が可能である。
SurfaceOperation クラスを用いると、それぞれに対して角度で分割することも可能である。

==メソッド一覧

((<BoundaryExtractor.new>))
((<setMesh>))
((<setReverseMode>))
((<getReverseMode>))
((<appendBody>))
((<appendData>))
((<eraseData>))
((<getBoundary>))
((<getBoundaryFace>))
((<getBoundaryComponents>))
((<clear>))

=end
------------------------------------------------------------------------------*/

%{
#include "MeshDB/kmbBoundaryExtractor.h"
%}

namespace kmb{

class BoundaryExtractor
{
public:
/**--------------------------------------------------------------------------
=begin
--- BoundaryExtractor.new
	境界抽出クラス BoundaryExtractor のインスタンスを生成する。
=end
------------------------------------------------------------------------------*/
	BoundaryExtractor(void);
	virtual ~BoundaryExtractor(void);
/**--------------------------------------------------------------------------
=begin
--- setMesh(mesh)
	対象となる MeshDB を与える。
=end
------------------------------------------------------------------------------*/
	void setMesh(kmb::MeshData* mesh);
/**--------------------------------------------------------------------------
=begin
--- setReverseMode(mode)
    境界抽出の時に裏返っている要素を許す場合は true を設定する。
--- getReverseMode
    境界抽出の時に裏返っている要素を許すか動かを出力する。
=end
------------------------------------------------------------------------------*/
	void setReverseMode(bool mode);
	bool getReverseMode(void) const;
/**--------------------------------------------------------------------------
=begin
--- appendBody(bodyId)
	境界抽出を行う Body を追加する。
=end
------------------------------------------------------------------------------*/
	void appendBody(kmb::bodyIdType bodyId);
/**--------------------------------------------------------------------------
=begin
--- appendData(name)
	境界抽出を行う ElementGroup を追加する。
=end
------------------------------------------------------------------------------*/
	void appendData(const char* name);
/**--------------------------------------------------------------------------
=begin
--- eraseData(name)
	境界から除去する面を指定する ElementGroup / FaceGroup を与える。
	reverse = true の時は、逆向きの面が登録されていれば除去する。
=end
------------------------------------------------------------------------------*/
	void eraseData(const char* name,bool reverse=false);
/**--------------------------------------------------------------------------
=begin
--- getBoundary(bodyId)
--- getBoundary()
	境界を抽出して MeshDB に格納し、その BodyId を返す。
	例：あるメッシュの境界を求める場合
	 ex = BoundaryExtractor.new
	 ex.setMesh(mesh)
	 ex.appendBody(bodyId)
	 boundaryId = ex.getBoundary(bodyId)
	例：複数の Body について境界面抽出する場合
	 ex = BoundaryExtractor.new
	 ex.setMesh(mesh)
	 ex.appendBody(bodyId0)
	 ex.appendBody(bodyId1)
	 boundaryId0 = ex.getBoundary(bodyId0)
	 boundaryId1 = ex.getBoundary(bodyId1)
	このときは、bodyId0 と bodyId1 で表される２つメッシュの境界面は
	boundaryId0 boundaryId1 には含まない。
	getBoundary の引数を省略した場合はすべての Body に含まれる要素について境界面を抽出する。
--- getBoundaryComponents(bodyId)
	境界を抽出して連結成分に分けて MeshDB に格納し、その BodyId の Array を返す。
=end
------------------------------------------------------------------------------*/
	kmb::bodyIdType getBoundary(kmb::bodyIdType bodyId);
	kmb::bodyIdType getBoundary(void);
%extend{
	VALUE getBoundaryComponents(kmb::bodyIdType bodyId){
		kmb::bodyIdType* boundIds = NULL;
		int num = self->getBoundaryComponents(bodyId,boundIds);
		VALUE ary = rb_ary_new();
		for(int i=0;i<num;++i){
			rb_ary_push(ary,INT2NUM(boundIds[i]));
		}
		if( boundIds ){
			delete[] boundIds;
		}
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
--- getBoundaryFace(bodyId,name)
	境界を name と言う名前の Data の Face の集合として抽出する。
	モードは FaceGroup 値の型は None になる。
=end
------------------------------------------------------------------------------*/
	bool getBoundaryFace(kmb::bodyIdType bodyId,const char* name);
/**--------------------------------------------------------------------------
=begin
--- isClosed(bodyId)
	boundary がない状態 = closed かどうかの判定
	判定のためにはあらかじめ appendBody しておくことが必要
=end
------------------------------------------------------------------------------*/
	bool isClosed(kmb::bodyIdType bodyId);

/**--------------------------------------------------------------------------
=begin
--- clear
	MeshDB および境界抽出の対象となる Body の情報を消去する。
=end
------------------------------------------------------------------------------*/
	void clear(void);
};

}

/**--------------------------------------------------------------------------
=begin
==使用例
=== 三角形で表面を抽出する方法

appendBody メソッドで表面を抽出する対象となる Body を与えて、getBoudary メソッドで
境界面を生成しその BodyID を取り出す。

例：
TetMesh 形式の四面体メッシュ test.msh から表面メッシュを抽出して、Tripatch 形式の patch ファイルを出力する

  require 'RevocapMesh'
  require 'MeshIO/TetMeshIO'
  require 'MeshIO/TripatchIO'

  mesh = RevocapMesh::MeshDB.new
  tetmesh = TetMeshIO.new
  tripatch = TripatchIO.new
  tetmesh.loadFromFile('test.msh',mesh)
  bext = RevocapMesh::BoundaryExtractor.new
  bext.setMesh(mesh)
  bext.appendBody(0)
  surfID = bext.getBoundary(0)
  tripatch.bodyID = surfID
  tripatch.saveToFile('test.pch',mesh)

=== 四面体要素と面番号の組で表面を抽出する方法

appendBody メソッドで表面を抽出する対象となる Body を与えて、getBoudaryFace メソッドで
文字列を引数として与え、要素と面番号の組の集合を FaceGroup モードの Data で生成する。

例：
TetMesh 形式の四面体メッシュから要素と面番号の組で表面を抽出する。

  require 'RevocapMesh'
  require 'MeshIO/TetMeshIO'

  mesh = RevocapMesh::MeshDB.new
  tetmesh = TetMeshIO.new
  tripatch = TripatchIO.new
  tetmesh.loadFromFile('test.msh',mesh)
  bext = RevocapMesh::BoundaryExtractor.new
  bext.setMesh(mesh)
  bext.appendBody(0)
  surfID = bext.getBoundaryFace(0,'wall')
  mesh.extractBoundaryFace(0,"wall")
  mesh.eachId("wall"){ |face|
    # face[0] が表面の要素番号
    # face[1] が表面の面番号
  }

=end
------------------------------------------------------------------------------*/
