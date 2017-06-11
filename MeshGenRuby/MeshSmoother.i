/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : MeshSmoother                                       #
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
=RevocapMeshGen::MeshSmoother

==概要

RevocapMeshGen::MeshSmoother クラスは
RevocapMesh::MeshData に格納されたメッシュについて Laplacian smoothing を
行うものである。

使い方：
 smoother = RevocapMeshGen::MeshSmoother.new(mesh)
 smoother.appendBody(bodyId)
 smoother.init                     # smoothing 用の一時保存用の領域確保
 smoother.smoothingNode(nodeId)    # 必要に応じた nodeId について繰り返す
 smoother.commit                   # smoothing した結果を節点座標に反映する

==メソッド一覧

((<MeshSmoother.new>))
((<appendBody>))
((<init>))
((<commit>))
((<smoothingNode>))
((<smoothingAllNode>))
((<smoothingNodeGroup>))


=end
------------------------------------------------------------------------------*/
%{
#include "MeshGen/kmbMeshSmoother.h"
%}

namespace kmb{

class MeshData;

class MeshSmoother
{
public:
/**--------------------------------------------------------------------------
=begin
--- MeshSmoother.new(mesh)
	メッシュ円滑化モジュール RevocapMeshGen::MeshSmoother のインスタンスを生成する。
	メッシュと円滑化の対象を mesh で与える。
=end
---------------------------------------------------------------------------*/
	MeshSmoother(kmb::MeshData* mesh);
	virtual ~MeshSmoother(void);
	size_t appendBody(kmb::bodyIdType bodyId);
/**--------------------------------------------------------------------------
=begin
--- init
	メッシュ円滑化モジュールの初期化
=end
---------------------------------------------------------------------------*/
	void init(void);
/**--------------------------------------------------------------------------
=begin
--- commit
	メッシュ円滑化の結果を節点配列に反映する
=end
---------------------------------------------------------------------------*/
	void commit(void);
/**--------------------------------------------------------------------------
=begin
--- smoothingNode(nodeId)
	節点番号 nodeId の点を円滑化する。
	このメソッドを実行しただけでは節点配列には反映しない。
=end
---------------------------------------------------------------------------*/
	void smoothingNode(kmb::nodeIdType nodeId);
	void smoothingAllNode(void);
	void smoothingNodeGroup(const char* name,const char* stype=NULL);
};

}
