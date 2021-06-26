/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : EdgeStithcer                                       #
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
=RevocapMeshGen::EdgeStitchcer

==概要

RevocapMeshGen::EdgeStitcher クラスは
RevocapMesh::MeshData に格納された線分からなる辺データについて、
それを縫い合わせる三角形または四角形パッチを生成する。

==メソッド一覧

((<EdgeStitcher.new>))
((<setMesh>))
((<setEdges>>)
((<setEdgesWithinDistance>))
((<setEdgesWithInitNodes>))
((<stitchEdgesByTriangles>))
((<stitchEdgesByQuads>))
((<stitchByQuadsWithNodeCount>))
((<getNormalCheck>))
((<setNormalCheck>))

==メソッド

=end
------------------------------------------------------------------------------*/
%{
#include "MeshGen/kmbEdgeStitcher.h"
%}

namespace kmb{

class MeshData;

class EdgeStitcher
{
public:
/**--------------------------------------------------------------------------
=begin
--- EdgeStitcher.new
	辺縫い合わせ処理クラス EdgeStitcher のインスタンスを生成する。
=end
---------------------------------------------------------------------------*/
	EdgeStitcher(void);
	virtual ~EdgeStitcher(void);

/**--------------------------------------------------------------------------
=begin
--- setMesh(mesh)
	EdgeStitcher が対象とするメッシュを設定する。
=end
---------------------------------------------------------------------------*/
	void setMesh(kmb::MeshData* mesh);
/**--------------------------------------------------------------------------
=begin
--- setEdges( forward, backward )
	EdgeStitcher が縫い合わせる対象となる辺データを設定する。
	forward と backward でそれぞれの MeshData における bodyId を与える。
	縫い合わせのパッチを作成する際に forward は順方向、backward は逆方向に走査する。
	それぞれの辺データについて自動的に先頭と終端を調べる。
	片方がループの場合は、もう一方の開始点に一番近い点を開始点にする。
	両方がループの場合は、edge0 の最初の要素の節点を開始点にして、edge1 の開始点は
	edge0 の開始点から最も近い点とする。

--- setEdgesWithinDistance( forward, backward, distance )
	EdgeStitcher が縫い合わせる対象となる辺データを設定する。
	forward と backward に含まれるすべての線分ではなく、
	線分の両端の頂点から対象となる辺に含まれるまでの距離がともに distance 以下の
	線分だけを縫い合わせる対象とする。

---	setEdgesWithInitNodes( forward, forwardInit, backward, backwardInit )
	EdgeStitcher が縫い合わせる対象となる辺データと縫い合わせの開始点を設定する。
	forwardInit は forward が表す辺データにおける節点番号、
	backwardInit は backward が表す辺データにおける節点番号を与え、
	それらの節点から縫い合わせ処理を開始する。

=end
---------------------------------------------------------------------------*/
	bool setEdges(const char* forward,const char* backward);
	bool setEdges( kmb::bodyIdType forward, kmb::bodyIdType backward );
	bool setEdgesWithinDistance(kmb::bodyIdType forward,kmb::bodyIdType backward,double distance);
	bool setEdgesWithInitNodes(kmb::bodyIdType forward,kmb::nodeIdType forwardInit,kmb::bodyIdType backward,kmb::nodeIdType backwardInit);

/**--------------------------------------------------------------------------
=begin
--- stitchEdgesByTriangles()
	三角形で縫い合わせ処理をする。生成した三角形パッチを MeshData に格納してその bodyId を返す。
	端点にたどりつく場合もしくは、開始点に戻る場合に終了する。
	張り合わせる三角形の向きは、線分が三角形の境界となるようにする
	（三角形の向きと線分の向きは逆）。

--- stitchEdgesByQuads()
	四角形で縫い合わせ処理をする。生成した四角形パッチを MeshData に格納してその bodyId を返す。
	端点にたどりつく場合もしくは、開始点に戻る場合に終了する。
	辺の個数の偶奇のパリティが異なる時は、最後の四角形が埋められない場合がある。
	張り合わせる四角形の向きは、線分が四角形の境界となるようにする
	（四角形の向きと線分の向きは逆）。
=end
---------------------------------------------------------------------------*/
	kmb::bodyIdType stitchEdgesByTriangles(void);
	kmb::bodyIdType stitchEdgesByQuads(void);
	kmb::bodyIdType stitchByQuadsWithNodeCount(int fCount,int bCount);

/**--------------------------------------------------------------------------
=begin
--- getNormalCheck()
	縫い合わせる辺の選び方で法線ベクトルを考慮するかどうかを調べる。

--- setNormalCheck(check)
	縫い合わせる辺の選び方で法線ベクトルを考慮するかどうか設定する。
	三次元空間内の平面上に辺データがある場合は check = true とすると、
	生成するパッチデータの向きがそろう。法線ベクトルを考慮しない場合は、
	パッチデータを生成する辺の選び方は三角形の形状で判断するため、
	場合によっては平面内で折れたパッチデータが生成される場合がある。
=end
---------------------------------------------------------------------------*/
	bool getNormalCheck(void);
	void setNormalCheck(bool check);
};

}
