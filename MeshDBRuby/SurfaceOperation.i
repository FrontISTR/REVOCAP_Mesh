/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : SurfaceOperation                                   #
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
=RevocapMesh::SurfaceOperation

RevocapMesh::MeshDB に格納された表面メッシュの処理を行うクラス。
SurfaceOperaion クラスでは、平面要素（三角形または四角形）で与えられた場合、
立体要素と面番号の組で与えられた場合、それぞれに対して、
角度を与えてその角度以上に曲がっている表面を分割する機能が実装されている。

==メソッド一覧

((<SurfaceOperation.new>))
((<setMesh>))
((<divideSurface>))
((<divideSurfaceWithRidge>))
((<divideFaceGroup>))
((<divideFaceGroupWithRidge>))
((<decomposeSurface>))
((<decomposeFaceGroup>))
((<faceGroup2Surface>))
((<surface2FaceGroup>))

=end
------------------------------------------------------------------------------*/

%{
#include "MeshDB/kmbSurfaceOperation.h"
%}


#pragma once

#include <map>
#include <vector>
#include <string>
#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElement.h"

namespace kmb{

class SurfaceOperation
{
public:
/**--------------------------------------------------------------------------
=begin
--- SurfaceOperation.new
	表面メッシュ処理クラス SurfaceOperation のインスタンスを生成する。
=end
------------------------------------------------------------------------------*/
	SurfaceOperation(void);
	virtual ~SurfaceOperation(void);
/**--------------------------------------------------------------------------
=begin
--- setMesh(mesh)
	対象となる MeshDB を与える。
=end
------------------------------------------------------------------------------*/
	void setMesh(kmb::MeshDB* mesh);
/**--------------------------------------------------------------------------
=begin
--- setPermitReverse(f)
	表面から面グループを抽出するときに、逆向きを許すか否か
	規定値は true なので許さないときは false にする
=end
------------------------------------------------------------------------------*/
	void setPermitReverse(bool f);
/**--------------------------------------------------------------------------
=begin
--- divideSurface(bodyId,angle)
--- divideSurfaceWithRidge(bodyId,angle)
	平面要素からなる bodyId のボディについて、角度 angle を閾値として
	分割する。
	divideSurface は分割した要素グループの Id を Array に入れて返す。
	divideSurfaceWithRidge は分割したボディ Id と稜線のボディ Id を Hash に入れて返す。
	もとの bodyId には稜線だけからなる要素グループが入る。
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE divideSurface(kmb::bodyIdType bodyId,double angle){
		std::vector<kmb::bodyIdType> surfIds;
		self->divideSurface(bodyId,angle,surfIds);
		VALUE ary = rb_ary_new();
		int i=0;
		for(std::vector<kmb::bodyIdType>::iterator p = surfIds.begin();
			p != surfIds.end(); ++p)
		{
			kmb::bodyIdType surfId = (*p);
			rb_ary_store(ary,i,INT2FIX(surfId));
			++i;
		}
		return ary;
	}

	VALUE divideSurfaceWithRidge(kmb::bodyIdType bodyId,double angle){
		std::map<kmb::bodyIdType,kmb::bodyIdType> surfIds;
		self->divideSurfaceWithRidge(bodyId,angle,surfIds);
		VALUE hash = rb_hash_new();
		for(std::map<kmb::bodyIdType,kmb::bodyIdType>::iterator p = surfIds.begin();
			p != surfIds.end(); ++p)
		{
			rb_hash_aset( hash,INT2FIX(p->first),INT2FIX(p->second) );
		}
		return hash;
	}
}
/**-------------------------------------------------------------------------
=begin
--- divideFaceGroup(name,angle)
--- divideFaceGroupWithRidge(name,angle)
	Face からなる Data を angle を閾値として分割する。
	divideFaceGroup は分割した Face に、もとの name に _N （Nは数字）を付加した名前をつけて、
	分割面の名前をArray に格納して返す。
	divideFaceGroupWithRidge は分割した Face の Data 名を key として、
	稜線 Body の BodyId を value とし、もとの Face に対しては全体の稜線 Body の
	BodyId を value とする Hash を返す。

=end
---------------------------------------------------------------------------*/
%extend{
	VALUE divideFaceGroup(const char* name,double angle){
		std::vector<std::string> faceNames;
		self->divideFaceGroup(name,angle,faceNames);
		VALUE ary = rb_ary_new();
		int i=0;
		std::vector<std::string>::iterator sIter = faceNames.begin();
		while( sIter != faceNames.end() )
		{
			std::string facename = (*sIter);
			rb_ary_store(ary,i,rb_str_new2(facename.c_str()));
			++i;
			++sIter;
		}
		return ary;
	}
	VALUE divideFaceGroupWithRidge(const char* name,double angle){
		std::map<std::string,kmb::bodyIdType> faceNames;
		kmb::bodyIdType bodyId = self->divideFaceGroupWithRidge(name,angle,faceNames);
		VALUE hash = rb_hash_new();
		rb_hash_aset( hash, rb_str_new2(name), INT2FIX(bodyId) );
		std::map<std::string,kmb::bodyIdType>::iterator sIter = faceNames.begin();
		while( sIter != faceNames.end() )
		{
			rb_hash_aset( hash, rb_str_new2((sIter->first).c_str()) ,INT2FIX(sIter->second) );
			++sIter;
		}
		return hash;
	}
}
/**-------------------------------------------------------------------------
=begin
--- decomposeSurface(bodyId)
	連結成分に分ける。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE decomposeSurface(kmb::bodyIdType bodyId){
		std::set<kmb::bodyIdType> compIds;
		self->decomposeSurface(bodyId,compIds);
		VALUE ary = rb_ary_new();
		std::set<kmb::bodyIdType>::iterator bIter = compIds.begin();
		while( bIter != compIds.end() )
		{
			kmb::bodyIdType compId = (*bIter);
			rb_ary_push(ary,INT2FIX(compId));
			++bIter;
		}
		return ary;
	}
}
/**-------------------------------------------------------------------------
=begin
--- decomposeFaceGroup(faceGroup)
	連結成分に分ける。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE decomposeFaceGroup(const char* faceGroup){
		std::vector<std::string> faceNames;
		self->decomposeFaceGroup(faceGroup,faceNames);
		VALUE ary = rb_ary_new();
		std::vector<std::string>::iterator sIter = faceNames.begin();
		while( sIter != faceNames.end() )
		{
			std::string facename = (*sIter);
			rb_ary_push(ary,rb_str_new2(facename.c_str()));
			++sIter;
		}
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
--- faceGroup2Surface(name)
	FaceGroup から surface を生成する
--- surface2FaceGroup(bodyId,parentBodyId,name);
	parentBodyId の Body の境界になっている bodyId の Body が与えられた時に、
	name の FaceGroup を生成する。
=end
-----------------------------------------------------------------------------*/
	kmb::bodyIdType faceGroup2Surface(const char* name);
%extend{
	bool surface2FaceGroup(kmb::bodyIdType bodyId,VALUE parent,const char* name){
		if( TYPE(parent) == T_STRING ){
			return self->surface2FaceGroup(bodyId,StringValuePtr(parent),name);
		}else if( TYPE(parent) == T_FIXNUM ){
			return self->surface2FaceGroup(bodyId,FIX2INT(parent),name);
		}
		return false;
	}
}
	void setSurface(kmb::MeshDB* mesh,kmb::bodyIdType bodyId);
	enum vertexType{
		Unknown,
		CONVEX,     // 頂点の周りの要素の角度の和が 360 度より小さい＆凸
		CONCAVE,    // 頂点の周りの要素の角度の和が 360 度より小さい＆凹
		FLAT,       // 頂点の周りの要素の角度の和が 360 度
		HYPERBOLIC, // 頂点の周りの要素の角度の和が 360 度より大きい
	};
	kmb::SurfaceOperation::vertexType getVertexType(kmb::nodeIdType nodeId);
};

}

/**--------------------------------------------------------------------------
=begin
==使用例
=== 平面要素で与えられた表面メッシュを分割する方法

直方体のようなメッシュから表面メッシュの抽出を行うと、直方体の表面全体が
1つのボディとして得られるが、直方体の6面に分割して境界条件を設定する場合などに
行う。

例：
TetMesh 形式の四面体メッシュから表面メッシュを抽出して分割する
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
  surfId = bext.getBoundary(0)
  surfOp = RevocapMesh::SurfaceOperation.new
  surfOp.setMesh(mesh)
  divSurfs = surfOp.divideSurface(surfId,60)

60度を閾値として surfId で表される表面メッシュを分割する。divSurfs は
分割された表面メッシュの BodyId が格納されている。

例：
TetMesh 形式の四面体メッシュから表面メッシュを抽出して分割してその境界線メッシュも取得する
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
  surfId = bext.getBoundary(0)
  surfOp = RevocapMesh::SurfaceOperation.new
  surfOp.setMesh(mesh)
  divSurfs = surfOp.divideSurfaceWithRidge(surfId,60)

60度を閾値として surfId で表される表面メッシュを分割する。divSurfs は
分割された表面メッシュとその境界の線メッシュの BodyId の組が Hash として格納されている。

  divSurfs.each{ |k,v|
    # k = 分割された表面メッシュの BodyId
    # v = BodyId k の表面メッシュの境界の線メッシュ
  }
  # surId = 境界となった線メッシュ全体

例：
TetMesh 形式の四面体メッシュから要素と面番号の組で表面を抽出して分割する。

  require 'RevocapMesh'
  require 'MeshIO/TetMeshIO'

  mesh = RevocapMesh::MeshDB.new
  tetmesh = TetMeshIO.new
  tripatch = TripatchIO.new
  tetmesh.loadFromFile('test.msh',mesh)
  bext = RevocapMesh::BoundaryExtractor.new
  bext.setMesh(mesh)
  bext.appendBody(0)
  bext.getBoundaryFace(0,"wall")
  surfOp = RevocapMesh::SurfaceOperation.new
  surfOp.setMesh(mesh)
  divSurfs = surfOp.divideFaceGroup("wall",60)
  divSurfs.each{ |name|
    mesh.eachId(name){ |face|
      # 分割された FaceGroup モードの Data Name において
      # face[0] が表面の要素番号
      # face[1] が表面の面番号
    }
  }

例：
TetMesh 形式の四面体メッシュから要素と面番号の組で表面を抽出して分割し、境界線メッシュも取得する。

  require 'RevocapMesh'
  require 'MeshIO/TetMeshIO'

  mesh = RevocapMesh::MeshDB.new
  tetmesh = TetMeshIO.new
  tripatch = TripatchIO.new
  tetmesh.loadFromFile('test.msh',mesh)
  bext = RevocapMesh::BoundaryExtractor.new
  bext.setMesh(mesh)
  bext.appendBody(0)
  bext.getBoundaryFace(0,"wall")
  surfOp = RevocapMesh::SurfaceOperation.new
  surfOp.setMesh(mesh)
  divSurfs = surfOp.divideFaceGroupWithRidge("wall",60)
  divSurfs.each{ |name,ridge|
    mesh.eachId(name){ |face|
      # 分割された FaceGroup モードの Data Name において
      # face[0] が表面の要素番号
      # face[1] が表面の面番号
    }
    # ridge = name の FaceGroup の境界線メッシュ
  }

=end
------------------------------------------------------------------------------*/
