/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : MeshDB                                             #
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
/**--------------------------------------------------------------------
=begin
= RevocapMesh::MeshDB

RevocapMesh::MeshDB はメッシュ処理のためのクラス。コンテナ機能は MeshData を継承している。
MeshData に加え、

* 幾何処理
* 検索処理
* 近傍処理
* 要素間関係
* メッシュ結合・変換
* データ検索・補間

を行うことができる。

Ruby 言語から使うためには、拡張ライブラリ RevocapMesh.so を Ruby のロードパスにおいて
（インストールスクリプトを実行した場合は標準ライブラリディレクトリにコピーされているので問題ない）、

 require 'RevocapMesh'
 mesh = RevocapMesh::MeshDB.new

とすると、RevocapMesh::MeshDB のオブジェクトのインスタンス mesh が生成される。mesh オブジェクトに対して
以下で説明するメソッド群を呼び出すことが出来る。

==メソッド一覧

下記の一覧に加えて、MeshData のメソッド一覧も参照すること。

===クラスメソッド

((<MeshDB.new>))
((<getElementTypeMap>))

===メッシュ管理

((<copyMesh>))
((<setDefaultNodeContainerType>))
((<getDefaultNodeContainerType>))
((<setDefaultElementContainerType>))
((<getDefaultElementContainerType>))
((<setDefaultDataContainerType>))
((<getDefaultDataContainerType>))
((<setDefaultContainerType>))

===節点管理

((<getMaxNodeId>))
((<getMinNodeId>))
((<getNode>))
((<getNodePoint>))
((<deleteNode>))
((<eachNode>))
((<eachNodeWithId>))
((<eachNodeXYZ>))
((<eachNodeIdOfBody>))
((<eachNodeIdInRegion>))
((<nodeIdDefragmentation>))
((<replaceNodeId>))
((<convertAllNodes>))
((<translateAllNodes>))
((<scaleAllNodes>))
((<deleteUselessNodes>))
((<clearNodes>))

===要素管理

((<deleteElement>))
((<moveElement>))
((<getBodyId>))
((<getMaxElementId>))
((<cloneBody>))
((<cloneReverseBody>))
((<cloneBodyWithNode>))
((<cloneReverseBodyWithNode>))
((<getBoundingBox>))
((<updateBoundingBox>))
((<replaceNodeIdOfElement>))
((<replaceNodeIdOfBody>))
((<eachElement>))
((<eachElementWithType>))
((<eachElementWithId>))
((<eachElementIdInRegion>))
((<getEdgeCountOfBody>))
((<getNodeCountOfBody>))

===幾何処理

((<faceToElement>))
((<elementToFace>))
((<triangulation>))
((<getDistance>))
((<getDistanceToElement>))
((<getEdgeVector>))
((<getNormalVector>))
((<getNormalVectorFace>))
((<getAreaVectorOfSurface>))
((<getAreaVectorOfFaceGroup>))
((<getCornerNodeIdOfSurface>))
((<getCornerNodeIdOfFaceGroup>))
((<getElementVolume>))
((<getElementArea>))
((<getVolume>))
((<getArea>))
((<getBoundaryCount>))
((<getAspectRatio>))
((<getElementCoordinates>))
((<getPhysicalCoordinates>))
((<shapeFunction>))
((<reverseElement>))
((<reverseBody>))

===検索処理

((<generateNodeSearchCache>))
((<generateNodeSearchCacheOfBody>))
((<generateNodeSearchCacheOfData>))
((<appendNodeSearchCache>))
((<clearNodeSearchCache>))
((<getNearestNode>))
((<getNodesInRegion>))
((<generateElementSearchCache>))
((<clearElementSearchCache>))
((<searchElement>))
((<searchElementWithNormal>))
((<searchElementInData>))

===近傍処理

((<clearNeighborsInfo>))
((<generateNeighborsInfo>))
((<getNeighborNodes>))
((<getSurroundingElements>))
((<getSurroundingCount>))
((<getNeighborElements>))
((<getAdjacentFace>))

===要素間関係

((<getFaces>))
((<isFace>))
((<getCommonElement>))
((<getElementRelation>))

===メッシュ結合・変換

((<mergeBody>))
((<convertToLinearBody>))
((<importBody>))
((<importBodyWithNodeMatching>))
((<importAllBody>))

=== データ管理

((<getDataNameArray>))
((<replaceIdOfData>))
((<convertData>))
((<convertBodyToData>))
((<getElementCountOfData>))
((<getNodeCountOfData>))
((<eachNodeIdOfData>))
((<getBoundingBoxOfData>))
((<getUniqueDataName>))

===データ検索・補間

((<getNearestValue>))
((<getInterpolatedValue>))
((<getInterpolatedMultiValues>))
((<getInterpolatedValueInData>))
((<getInterpolatedMultiValuesInData>))
((<getMinMaxValue>))
((<getMinMaxValueWithId>))

===その他

((<calcMeshProperty>))

=end

MeshData で定義しているものについては、ここで定義するのは不要。
ただしドキュメントとの整合性を取るために、記述している。
MeshData で定義したメソッドをオーバーライド・オーバーロードしているものについては
こちらで再定義する必要あり。例：addNode

命名規則メモ
createXXX : コンテナ系の新規作成
generateXXX : すでにコンテナに入っているデータから別のデータの生成
---------------------------------------------------------------*/
%{
#include <string>
#include "MeshDB/kmbElement.h"
#include "MeshDB/kmbTriangle.h"
#include "MeshDB/kmbTriangle2.h"
#include "MeshDB/kmbQuad.h"
#include "MeshDB/kmbQuad2.h"
#include "MeshDB/kmbSegment.h"
#include "MeshDB/kmbFace.h"
#include "MeshDB/kmbMeshDB.h"
#include "MeshDB/kmbElementContainer.h"
#include "MeshDB/kmbElementRelation.h"
#include "MeshDB/kmbDataBindings.h"
#include "MeshDB/kmbPhysicalValue.h"
#include "MeshDB/kmbMatching.h"
#include "Geometry/kmbGeometry3D.h"
#include "Geometry/kmbRegion.h"
#include "Geometry/kmbGeometry2D.h"
#include "Geometry/kmbCircle.h"
#include "Geometry/kmbLine.h"
#include "Geometry/kmbPlane.h"
#include "Geometry/kmbFramedPlane.h"
#include "Geometry/kmbSphere.h"
#include "Geometry/kmbBoxRegion.h"
#include "Geometry/kmbBoundingBox.h"
#include "Geometry/kmbRegion.h"
#include "MeshDBRuby/kmbMeshDB_SWIG.h"
#include "MeshDBRuby/kmbPhysicalValue_SWIG.h"
%}


%include "../Geometry/kmbCircle.h"
%include "../Geometry/kmbLine.h"
%include "../Geometry/kmbPlane.h"
%include "../Geometry/kmbSphere.h"

namespace kmb{

class MeshDB : public MeshData
{
public:

%extend{
	static VALUE getElementTypeMap(void){
		VALUE typemap = rb_hash_new();
		rb_hash_aset( typemap, rb_str_new2("SEGMENT"), INT2NUM( kmb::SEGMENT ) );
		rb_hash_aset( typemap, rb_str_new2("SEGMENT2"), INT2NUM( kmb::SEGMENT2 ) );
		rb_hash_aset( typemap, rb_str_new2("TRIANGLE"), INT2NUM( kmb::TRIANGLE ) );
		rb_hash_aset( typemap, rb_str_new2("TRIANGLE2"), INT2NUM( kmb::TRIANGLE2 ) );
		rb_hash_aset( typemap, rb_str_new2("QUAD"), INT2NUM( kmb::QUAD ) );
		rb_hash_aset( typemap, rb_str_new2("QUAD2"), INT2NUM( kmb::QUAD2 ) );
		rb_hash_aset( typemap, rb_str_new2("TETRAHEDRON"), INT2NUM( kmb::TETRAHEDRON ) );
		rb_hash_aset( typemap, rb_str_new2("TETRAHEDRON2"), INT2NUM( kmb::TETRAHEDRON2 ) );
		rb_hash_aset( typemap, rb_str_new2("WEDGE"), INT2NUM( kmb::WEDGE ) );
		rb_hash_aset( typemap, rb_str_new2("WEDGE2"), INT2NUM( kmb::WEDGE2 ) );
		rb_hash_aset( typemap, rb_str_new2("PYRAMID"), INT2NUM( kmb::PYRAMID ) );
		rb_hash_aset( typemap, rb_str_new2("PYRAMID2"), INT2NUM( kmb::PYRAMID2 ) );
		rb_hash_aset( typemap, rb_str_new2("HEXAHEDRON"), INT2NUM( kmb::HEXAHEDRON ) );
		rb_hash_aset( typemap, rb_str_new2("HEXAHEDRON2"), INT2NUM( kmb::HEXAHEDRON2 ) );
		return typemap;
	}
}

/**--------------------------------------------------------------------
=begin
== クラスメソッド
--- MeshDB.new()
	MeshDB クラスのコンストラクタである。MeshDB オブジェクトのインスタンスを生成する。
--- getElementTypeMap()
	RevocapMesh で扱うことができる要素の種類とその識別のための整数定数の Hash を返す。
	例えば
	 typeMap = MeshDB.getElementTypeMap
	 typeMap["TETRAHEDRON"]
	で得られる整数値は MeshDB クラスの addElement などで要素の種類を識別するのに
	用いられる。
	それぞれの要素の詳細については
	((<要素ライブラリ|URL:Element.html>))
	を参照のこと。
=end
---------------------------------------------------------------------*/
	MeshDB(void);
	virtual ~MeshDB(void);

/**--------------------------------------------------------------------
=begin
==メッシュ管理
===メッシュ複製
--- copyMesh(other)
	以下で説明するメソッド eachNodeWithId および eachElement が定義されている他のメッシュ other
	（MeshDBでなくてもよい）からメッシュをコピーして作る。
	例えば他のメッシャーからメッシュデータを MeshDB のオブジェクトにコピーする時などに使う。
=end

	VALUE selfDB = SWIG_Ruby_NewPointerObj(self, SWIGTYPE_p_kmb__MeshDB, 0);

	は SWIG が生成するコードのメソッドを流用した。
	MeshDB* から VALUE への変換を行う。

	rb_iterate( _eachNodeWithId, mesh, (VALUE(*)(...))meshDB_addNodeWithId , selfDB );

	は mesh の eachNode を呼び出して、そのブロックの中で
	meshDB_addNodeWithId を実行する。
	_eachNodeWithId と meshDB_addNodeWithId は Callback 関数。
	meshdb.i で定義すると、SWIG が名前を変えてしまうので、
	kmbMeshDB_SWIG.h に定義してある。

----------------------------------------------------------------------*/
%extend{
	VALUE copyMesh(VALUE mesh){
		VALUE selfDB = SWIG_Ruby_NewPointerObj(self, SWIGTYPE_p_kmb__MeshDB, 0);
		unsigned int nodeCount = NUM2INT( rb_funcall(mesh, rb_intern("getNodeCount"), 0 ) );
		self->beginNode( nodeCount );
		rb_iterate( _eachNodeWithId, mesh, (VALUE(*)(...))meshDB_addNodeWithId , selfDB );
		self->endNode();
		int bodyCount = NUM2INT( rb_funcall(mesh, rb_intern("getBodyCount"), 0 ) );
		for(int i=0;i<bodyCount;++i){
			int elementCount = NUM2INT( rb_funcall(mesh, rb_intern("getElementCount"), 1, INT2NUM(i) ) );
			self->beginElement( elementCount );
			VALUE mesh_with_i = rb_ary_new2(2);
			rb_ary_store(mesh_with_i,0,mesh);
			rb_ary_store(mesh_with_i,1,INT2NUM(i));
			rb_iterate( _eachElement, mesh_with_i, (VALUE(*)(...))meshDB_addElement , selfDB );
			self->endElement();
		}
		return Qnil;
	}
}

/**---------------------------------------------------------------------
=begin
--- setDefaultNodeContainerType(nodeContainerType)
	デフォルトの節点コンテナの種類を指定する。
	RevocapMesh で使用可能なコンテナタイプ：
	 - stl::map<id,Point3D*>
	 - stl::vector<Point3D*>
	 - double_array
	 - double_marray
---	getDefaultNodeContainerType()
	デフォルトの節点コンテナの種類を取得する。
--- setDefaultElementContainerType(elementContainerType)
	デフォルトの要素コンテナの種類を指定する。
---	getDefaultElementContainerType()
	デフォルトの要素コンテナの種類を取得する。
--- setDefaultDataContainerType(dataContainerType)
	デフォルトのデータコンテナの種類を指定する。
---	getDefaultDataContainerType()
	デフォルトのデータコンテナの種類を取得する。
--- setDefaultContainerType(containerType)
	節点、要素、データコンテナを目的に応じて適切に設定する
	 - "Post"
	 - "Pre"
	 - "Refiner"
	に対応
=end
------------------------------------------------------------------------*/
	void setDefaultNodeContainerType(const char* nodeContainerType);
	const char* getDefaultNodeContainerType(void);
	void setDefaultElementContainerType(const char* elementContainerType);
	const char* getDefaultElementContainerType(void);
	void setDefaultDataContainerType(const char* dataContainerType);
	const char* getDefaultDataContainerType(void);
	void setDefaultContainerType(const char* containerType);


/**----------------------------------------------------------------------
=begin
==節点管理（MeshDB の拡張機能）

MeshData で定義されているメソッドは除く

=end
--------------------------------------------------------------------------*/

/**------------------------------------------------------------------------
=begin
--- getMaxNodeId()
--- getMinNodeId()
	MeshDBに登録されている節点の最大節点Idと最小節点Idを返す。
=end
--------------------------------------------------------------------------*/
	kmb::nodeIdType getMaxNodeId(void);
	kmb::nodeIdType getMinNodeId(void);
/**------------------------------------------------------------------------
=begin
--- getNode(nodeId)
	節点Id が nodeId の節点の座標を [x,y,z] 形式の Array で返す。
	節点コンテナが２次元の場合は [x,y] 形式の Array で返す。
=end
--------------------------------------------------------------------------*/
%extend{
	VALUE getNode(kmb::nodeIdType nodeID){
		VALUE ary = rb_ary_new();
		kmb::Node node;
		if( self->getNode(nodeID,node) ){
			rb_ary_store(ary,0,rb_float_new(node.x()));
			rb_ary_store(ary,1,rb_float_new(node.y()));
			rb_ary_store(ary,2,rb_float_new(node.z()));
			return ary;
		}else if( self->getNodeDim() == 2 ){
			const kmb::Point2DContainer* point2Ds = self->getNode2Ds();
			kmb::Point2D point;
			if( point2Ds && point2Ds->getPoint( nodeID, point ) ){
				rb_ary_store(ary,0,rb_float_new(point.x()));
				rb_ary_store(ary,1,rb_float_new(point.y()));
				return ary;
			}else{
				return Qnil;
			}
		}else{
			return Qnil;
		}
	}
}
/**------------------------------------------------------------------------
=begin
--- getNodePoint(nodeId)
	節点ID が nodeId の頂点の座標を Point3D で返す。
	（注）この Point3D の値を変えても元のメッシュの節点座標は変わりません。
=end
--------------------------------------------------------------------------*/
%extend{
	kmb::Point3D* getNodePoint(kmb::nodeIdType nodeID){
		kmb::Point3D* point = new kmb::Point3D();
		if( self->getNode(nodeID,*point) ){
			return point;
		}else{
			delete point;
			return NULL;
		}
	}
}
/**------------------------------------------------------------------------
=begin
--- deleteNode(nodeId)
	節点を削除する。もともとその nodeId の節点がなければ何もしない。
=end
--------------------------------------------------------------------------*/
	bool deleteNode(kmb::nodeIdType id);
/**------------------------------------------------------------------------
=begin
--- eachNode
	節点のイテレータである。

	 mesh.eachNode{ |node|
	   x = node[0]
	   y = node[1]
	   z = node[2]
	 }

	のように使う。

--- eachNodeWithId
	節点と節点IDのイテレータである。

	 mesh.eachNodeWithId{ |node,id|
	   x = node[0]
	   y = node[1]
	   z = node[2]
	 }

	のように使う。

--- eachNodeXYZ
	節点の XYZ 座標を与えるイテレータである。

	 mesh.eachNodeXYZ{ |x,y,z|
	   sq = x*x + y*y + z*z
	 }

	のように使う。
=end
--------------------------------------------------------------------------*/
%extend{
	VALUE eachNode(void){
		if( self->getNodes() != NULL ){
			double x,y,z;
			kmb::Point3DContainer::iterator	nIter = self->getNodes()->begin();
			kmb::Point3DContainer::iterator	endIter = self->getNodes()->end();
			while( nIter != endIter )
			{
				if( nIter.getXYZ(x,y,z) ){
					VALUE ary = rb_ary_new();
					rb_ary_store(ary,0,rb_float_new(x));
					rb_ary_store(ary,1,rb_float_new(y));
					rb_ary_store(ary,2,rb_float_new(z));
					rb_yield(ary);
				}
				++nIter;
			}
		}else if( self->getNode2Ds() != NULL ){
			double x,y;
			kmb::Point2DContainer::const_iterator	nIter = self->getNode2Ds()->begin();
			kmb::Point2DContainer::const_iterator	endIter = self->getNode2Ds()->end();
			while( nIter != endIter )
			{
				if( nIter.getXY(x,y) ){
					VALUE ary = rb_ary_new();
					rb_ary_store(ary,0,rb_float_new(x));
					rb_ary_store(ary,1,rb_float_new(y));
					rb_yield(ary);
				}
				++nIter;
			}
		}
		return Qnil;
	}
	VALUE eachNodeWithId(void){
		if( self->getNodes() != NULL ){
			double x,y,z;
			kmb::Point3DContainer::iterator	nIter = self->getNodes()->begin();
			kmb::Point3DContainer::iterator	endIter = self->getNodes()->end();
			while( nIter != endIter )
			{
				const kmb::nodeIdType id = nIter.getId();
				if( nIter.getXYZ(x,y,z) ){
					VALUE ary = rb_ary_new();
					rb_ary_store(ary,0,rb_float_new(x));
					rb_ary_store(ary,1,rb_float_new(y));
					rb_ary_store(ary,2,rb_float_new(z));
					rb_yield_values(2,ary,INT2FIX(id));
				}
				++nIter;
			}
		}else if( self->getNode2Ds() != NULL ){
			double x,y;
			kmb::Point2DContainer::const_iterator	nIter = self->getNode2Ds()->begin();
			kmb::Point2DContainer::const_iterator	endIter = self->getNode2Ds()->end();
			while( nIter != endIter )
			{
				const kmb::nodeIdType id = nIter.getId();
				if( nIter.getXY(x,y) ){
					VALUE ary = rb_ary_new();
		 			rb_ary_store(ary,0,rb_float_new(x));
					rb_ary_store(ary,1,rb_float_new(y));
					rb_yield_values(2,ary,INT2FIX(id));
				}
				++nIter;
			}
		}
		return Qnil;
	}
	VALUE eachNodeXYZ(void){
		if( self->getNodes() == NULL )
			return Qnil;
		double x,y,z;
		kmb::Point3DContainer::iterator	nIter = self->getNodes()->begin();
		kmb::Point3DContainer::iterator	endIter = self->getNodes()->end();
		while( nIter != endIter )
		{
			if( nIter.getXYZ(x,y,z) ){
				rb_yield_values(3,rb_float_new(x),rb_float_new(y),rb_float_new(z));
			}
			++nIter;
		}
		return Qnil;
	}
}
/**--------------------------------------------------------------------------
=begin
--- eachNodeIdIOfBody(bodyId)
	bodyId に含まれる要素が参照する節点Idのイテレータである。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE eachNodeIdOfBody(kmb::bodyIdType bodyID){
		std::set<kmb::nodeIdType> nodeSet;
		kmb::Body* body = self->getBodyPtr( bodyID );
		if( body ){
			body->getNodesOfBody(nodeSet);
		}
		std::set<kmb::nodeIdType>::iterator iIter = nodeSet.begin();
		while( iIter != nodeSet.end() )
		{
			rb_yield( INT2NUM( (*iIter) ) );
			++iIter;
		}
		return Qnil;
	}
}
/**--------------------------------------------------------------------------
=begin
--- eachNodeIdInRegion(region)
	領域オブジェクトに含まれる節点Idのイテレータである。
	直方体の Region インスタンスは
	region = RevocapMesh::BoxRegion.new(x0,y0,z0,x1,y1,z1) のように作ることができる。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE eachNodeIdInRegion(kmb::Region* region){
		std::set<kmb::nodeIdType> nodeSet;
		self->getNodesInRegion(region,nodeSet);
		std::set<kmb::nodeIdType>::iterator iIter = nodeSet.begin();
		while( iIter != nodeSet.end() )
		{
			rb_yield( INT2NUM( (*iIter) ) );
			++iIter;
		}
		return Qnil;
	}
}
/**--------------------------------------------------------------------------
=begin
--- nodeIdDefragmentation(initId=0)
	先頭の節点Idを initid で与えて節点Idを連続化する。
	参照している要素や物理量があれば自動的に変換する。
=end
---------------------------------------------------------------------------*/
		void nodeIdDefragmentation(nodeIdType initId=0);
/**--------------------------------------------------------------------------
=begin
--- replaceNodeId(oldId,newId)
	頂点Idを置き換える。
	参照している要素や物理量があれば自動的に変換する。
=end
---------------------------------------------------------------------------*/
		bool replaceNodeId(kmb::nodeIdType oldId, kmb::nodeIdType newId);
/**--------------------------------------------------------------------------
=begin
--- convertAllNodes(matrix)
	Matrix4x4 の matrix を与えて、すべての節点を座標変換する。
	たとえば y-up の座標系を z-up に変えるには
	 convertAllNodes(
		RevocapMesh::Matrix4x4.new( 1.0, 0.0, 0.0, 0.0,
									0.0, 0.0, -1.0, 0.0,
									0.0, 1.0, 0.0, 0.0,
									0.0, 0.0, 0.0, 1.0 ) )
	とすればよい。
--- translateAllNodes( x, y, z )
	すべての節点を平行移動する。
	 convertAllNodes(
		RevocapMesh::Matrix4x4.new( 1.0, 0.0, 0.0, x,
									0.0, 1.0, 0.0, y,
									0.0, 0.0, 1.0, z,
									0.0, 0.0, 0.0, 1.0 ) )
	と等価
--- scaleAllNodes( r )
	すべての節点の座標をスケール変換する。
	 convertAllNodes(
		RevocapMesh::Matrix4x4.new( r,   0.0, 0.0, 0.0,
									0.0, r,   0.0, 0.0,
									0.0, 0.0, r,   0.0,
									0.0, 0.0, 0.0, 1.0 ) )
	と等価
=end
---------------------------------------------------------------------------*/
		void	convertAllNodes( kmb::Matrix4x4& mat );
		void	translateAllNodes( double x,double y,double z );
		void	scaleAllNodes( double r );
/**--------------------------------------------------------------------------
=begin
--- deleteUselessNodes()
	要素に使われていない頂点をすべて削除する。
	ただし関連する物理量に関してはチェックしていない。
=end
---------------------------------------------------------------------------*/
		int		deleteUselessNodes(void);
/**--------------------------------------------------------------------------
=begin
--- clearNodes()
	頂点をすべて削除する。ただし関連する要素や物理量に関しては
	削除しない。
=end
---------------------------------------------------------------------------*/
		void	clearNodes(void);


/**--------------------------------------------------------------------
=begin
==要素管理（MeshDBの 拡張機能）

MeshData で定義されているメソッドは除く

RevocapMesh での材料属性データの管理方法

ソルバによってやり方は異なるため、実際のプリポストに実装する場合は
ソルバの仕様に合わせる。

(1) RevocapMesh の Data に { name => value } の Hash を Value として登録して、
    要素グループの番号を Id に追加する（FrontSTR の場合）
(2) 外部に材料属性値のデータベースを作り、RevocapMesh には
    { 要素グループId => 外部データベースにおける材料属性値のId }
    を Data として登録する（FrontFlow/red の場合）
=end
-----------------------------------------------------------------------*/

/**-------------------------------------------------------------------------
=begin
--- deleteElement(bodyId,elementId)
	bodyId の Body に含まれる要素Idが elementId の要素を削除する。
	削除できた場合はその要素Idを返し、失敗したときは-1を返す。
=end
----------------------------------------------------------------------------*/
	kmb::elementIdType deleteElement(kmb::bodyIdType bodyId,kmb::elementIdType elementId);

/**-------------------------------------------------------------------------
=begin
--- moveElement(bodyId0,bodyId1,elementId)
	bodyId0 の Body に含まれる要素Id elementId の要素を
	bodyId1 の Body に移動する。
=end
----------------------------------------------------------------------------*/
	bool moveElement(kmb::bodyIdType bodyId0,kmb::bodyIdType bodyId1,kmb::elementIdType elementId);
/**-------------------------------------------------------------------------
=begin
--- getBodyId(elementId)
	与えられた要素がどの Body に含まれるかを返す。
=end
---------------------------------------------------------------------------*/
	kmb::bodyIdType getBodyId(kmb::elementIdType elementID);

/**-------------------------------------------------------------------------
=begin
--- getMaxElementId()
	使われている要素IDの最大値を返す。
=end
---------------------------------------------------------------------------*/
	kmb::elementIdType	getMaxElementId();

/**-------------------------------------------------------------------------
=begin
--- cloneBody(bodyId)
--- cloneReverseBody(bodyId)
--- cloneBodyWithNode(bodyId)
--- cloneReverseBodyWithNode(bodyId)
    bodyId の Body に含まれる要素のすべてを複製して
    その要素すべてからなる複製された Body のIdを返す。
=end
---------------------------------------------------------------------------*/
	kmb::bodyIdType cloneBody(kmb::bodyIdType bodyId);
	kmb::bodyIdType cloneReverseBody(kmb::bodyIdType bodyId);
	kmb::bodyIdType cloneBodyWithNode(kmb::bodyIdType bodyId);
	kmb::bodyIdType cloneReverseBodyWithNode(kmb::bodyIdType bodyId);

/**---------------------------------------------------------------------
=begin
--- getBoundingBox(bodyId)
	Body の BoundingBox を BoundingBox クラスのインスタンスで返す。
	2次元の場合は maxZ minZ はともに 0。

--- getBoundingBox()
	Mesh に登録されている節点全体の BoundingBox を BoundingBox クラスのインスタンスで返す。
	2次元の場合は maxZ minZ はともに 0。

--- updateBoundingBox(bodyId)
	mergeBody moveElement などを実行したあとで、BoundingBox を更新する

=end
------------------------------------------------------------------------*/
	void updateBoundingBox(bodyIdType bodyId);
	const kmb::BoundingBox getBoundingBox(void) const;
	const kmb::BoundingBox getBoundingBox(kmb::bodyIdType bodyId) const;

/**---------------------------------------------------------------------------
=begin
--- duplicateNodeIdOfBody(bodyId,coupleName=nil)

--- replaceNodeIdOfElement(bodyId,elementId,oldNodeId,newNodeId)
	要素Id elementId の要素の節点配列の oldNodeId を
	newNodeId に置き換える。

--- replaceNodeIdOfBody(bodyId,oldNodeId,newNodeId)
	bodyId の Body のすべての要素に対して節点配列の oldNodeId を
	newNodeId に置き換える。

--- replaceNodeIdOfBody(bodyId,nodeMapper)
	bodyId の Body のすべての要素に対して
	節点配列の nodeMapper に従って置き換える

--- replaceNodeIdOfBody(bodyId,coupleName)
=end
-----------------------------------------------------------------------------*/
	int duplicateNodeIdOfBody(kmb::bodyIdType bodyId,const char* coupleName=NULL);
	int replaceNodeIdOfBody(kmb::bodyIdType bodyId,const char* coupleName);

	bool replaceNodeIdOfElement(kmb::bodyIdType bodyId,kmb::elementIdType elementId,kmb::nodeIdType oldNodeId,kmb::nodeIdType newNodeId);
	int replaceNodeIdOfBody(bodyIdType bodyId,kmb::nodeIdType oldNodeId,kmb::nodeIdType newNodeId);
%extend{
	VALUE replaceNodeIdOfBody(bodyIdType bodyId,VALUE hash){
		Check_Type(hash, T_HASH);
		VALUE ary = rb_ary_new();

		rb_iterate( rb_each, hash, (VALUE(*)(...))appendKeys, ary );
		const int len = RARRAY_LEN(ary);
		std::map<kmb::nodeIdType,kmb::nodeIdType> nodeMapper;
		for(int i=0;i<len;++i){
			VALUE key = rb_ary_entry(ary,i);
			Check_Type(key,T_FIXNUM);
			VALUE item = rb_hash_aref( hash, key );
			Check_Type(item,T_FIXNUM);
			nodeMapper.insert( std::pair<kmb::nodeIdType,kmb::nodeIdType>(FIX2INT(key),FIX2INT(item)) );
		}
		return self->replaceNodeIdOfBody(bodyId,nodeMapper);
	}
}
/**---------------------------------------------------------------------------
=begin
--- eachElement(bodyID)
	bodyID の Body の要素のイテレータを与える。
	イテレータの内部のブロックでは、要素が getElement と同じ形式で得られる。

--- eachElementWithId(bodyID)
	bodyID の Body の要素と要素IDのイテレータを与える。

--- eachElementWithType(bodyID)
	bodyID の Body の要素の節点配列と要素タイプの組で出力するイテレータを与える。

=end
-----------------------------------------------------------------------------*/
%extend{
	VALUE eachElementWithType(kmb::bodyIdType bodyID){
		kmb::ElementContainer* body = self->getBodyPtr(bodyID);
		if( body ){
			kmb::ElementContainer::iterator eIter = body->begin();
			while( eIter != body->end() )
			{
				VALUE ary = rb_ary_new();
				const int len = eIter.getNodeCount();
				for(int i=0; i<len; ++i){
					rb_ary_store(ary,i,INT2FIX(eIter.getCellId(i)));
				}
				rb_yield_values(2,ary,rb_str_new2(eIter.getTypeString().c_str()));
				++eIter;
			}
		}
		return Qnil;
	}
}
/**--------------------------------------------------------------------------
=begin
--- eachElementIdInRegion(bodyId,region)
	全ての頂点が領域オブジェクトに含まれる要素Idのイテレータである。
	直方体の Region インスタンスは
	region = RevocapMesh::BoxRegion.new(x0,y0,z0,x1,y1,z1) のように作ることができる。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE eachElementIdInRegion(kmb::bodyIdType bodyId,kmb::Region* region){
		kmb::ElementContainer* body = self->getBodyPtr(bodyId);
		if( body ){
			bool flag=true;
			kmb::Point3D pt;
			kmb::ElementContainer::iterator eIter = body->begin();
			while( eIter != body->end() )
			{
				flag = true;
				const int len = eIter.getVertexCount();
				for(int i=0; i<len; ++i){
					if( !self->getNode( eIter[i], pt ) || region->intersect(pt) == kmb::Region::OUTSIDE ){
						flag = false;
						break;
					}
				}
				if( flag ){
					rb_yield( INT2NUM( eIter.getId() ) );
				}
				++eIter;
			}
		}
		return Qnil;
	}
}
/**---------------------------------------------------------------------------
=begin
--- getEdgeCountOfBody(bodyID)
	2次元 Body に含まれる辺の個数を返す。Euler 標数の計算に使う。

--- getNodeCountOfBody(bodyID)
	Body に含まれる頂点の個数を返す。Euler 標数の計算に使う。

=end
-----------------------------------------------------------------------------*/
	int getEdgeCountOfBody(kmb::bodyIdType bodyID);
	int getNodeCountOfBody(kmb::bodyIdType bodyID);


/**-----------------------------------------------------------------------
=begin
==幾何処理
--- faceToElement( [elementID,localID] )
	要素 Id と局所面 Id から境界要素を取得する。
	例えば elementID で四面体の要素ID、localID で 0 を与えた場合は、
	四面体の４つの境界三角形のうちの 0 番目のものが getElement で得られるのと
	同じ形式で得られる。ここで得られた要素は MeshDB には登録されていない。
	従って elementID は持っていない。
--- elementToFace( elementType, ary, bodyId )
	"TRIANGLE",[0,1,2] 形式で与えて bodyId の Body から、
	faceToElement した時にこの要素になるような face を求める。
	NeighborsInfo が作成されている時には、それを用いて高速化する。
=end
--------------------------------------------------------------------------*/
%extend{
	VALUE faceToElement(VALUE ary){
		Check_Type(ary, T_ARRAY);
		int elementID = NUM2INT( rb_ary_entry(ary, 0) );
		int localID = NUM2INT( rb_ary_entry(ary, 1) );
		kmb::bodyIdType bodyId = self->getBodyId( elementID );
		kmb::ElementContainer* body = self->getBodyPtr(bodyId);
		if( body==NULL )
			return Qnil;
		kmb::Face f( elementID, localID );
		kmb::Element* element = f.createElement( body );
		if( element==NULL )
			return Qnil;
		VALUE a = rb_ary_new();
		rb_ary_store(a,0,rb_str_new2(element->getTypeString().c_str()));
		const int len = element->getNodeCount();
		for(int i=0; i<len; ++i){
			rb_ary_store(a,i+1,INT2FIX(element->getCellId(i)));
		}
		delete element;
		return a;
	}
	VALUE elementToFace(VALUE eType,VALUE ary,kmb::bodyIdType bodyId=kmb::Body::nullBodyId){
		kmb::elementType type;
		if( TYPE(eType) == T_STRING ){
			type = kmb::ElementBase::getType(StringValuePtr(eType));
		}else{
			type = static_cast<kmb::elementType>(FIX2INT(eType));
		}
		Check_Type(ary, T_ARRAY);
		const int len = kmb::Element::getNodeCount(type);
		kmb::nodeIdType* a = NULL;
		a = new kmb::nodeIdType[len];
		for(int i=0; i<len; ++i){
			a[i] = FIX2INT(rb_ary_entry(ary,i));
		}
		VALUE face = Qnil;
		kmb::Element* element = kmb::Element::create(type,a);
		if( element != NULL ){
			kmb::Face f;
			if( self->elementToFace( *element, f, bodyId ) ){
				face = rb_ary_new();
				rb_ary_store(face,0,INT2FIX(f.getElementId()));
				rb_ary_store(face,1,INT2FIX(f.getLocalFaceId()));
			}
			delete element;
		}else{
			delete[] a;
		}
		return face;
	}
}
/**-----------------------------------------------------------------------
=begin
--- triangulation(bodyId)
	要素グループ bodyId に含まれる三角形以外、四面体以外の要素を分割して
	三角形または四面体にする。
=end
--------------------------------------------------------------------------*/
	void triangulation(kmb::bodyIdType bodyId);
/**-----------------------------------------------------------------------
=begin
--- getDistance(n0,n1)
	i0 番目の節点と i1 番目の節点の距離を返す。
=end
--------------------------------------------------------------------------*/
	double getDistance(kmb::nodeIdType n0, kmb::nodeIdType n1) const;
/**-----------------------------------------------------------------------
=begin
--- getDistanceToElement(x,y,z,elementId,bodyId=-1)
	(x,y,z) から elementId の要素への距離を返す。
=end
--------------------------------------------------------------------------*/
	double getDistanceToElement(double x,double y,double z,kmb::elementIdType elementId0,kmb::bodyIdType bodyId=kmb::Body::nullBodyId) const;
/**-----------------------------------------------------------------------
=begin
--- getEdgeVector(i0,i1)
	i0 番目の頂点から i1 番目の頂点へのベクトルを返す。
=end
--------------------------------------------------------------------------*/
	Vector3D getEdgeVector(kmb::nodeIdType i0,kmb::nodeIdType i1) const;
/**-------------------------------------------------------------------------
=begin
--- getNormalVector(elementId,bodyId=-1)
	平面要素ID elementID の要素の法線ベクトルを
	Vector3D 形式で返す。
	平面要素じゃないときはゼロベクトルを返す。

	 normal = mesh.getNormalVector(elementId)
	 puts normal.x
	 puts normal.y
	 puts normal.z

	のように使う

--- getNormalVectorFace(face,bodyId=-1)
--- getAreaVectorOfSurface(bodyId)
--- getAreaVectorOfFaceGroup(facegroup)
--- getCornerNodeIdOfSurface(bodyId,dir)
--- getCornerNodeIdOfFaceGroup(faceGroup,dir)

=end
---------------------------------------------------------------------------*/
	Vector3D getNormalVector(kmb::elementIdType elementId,kmb::bodyIdType bodyId=kmb::Body::nullBodyId);
%extend{
	Vector3D getNormalVectorFace(VALUE ary,kmb::bodyIdType bodyId=kmb::Body::nullBodyId){
		Check_Type(ary, T_ARRAY);
		int elementId = NUM2INT( rb_ary_entry(ary, 0) );
		int localId = NUM2INT( rb_ary_entry(ary, 1) );
		kmb::Face f( elementId, localId );
		return self->getNormalVector(f,bodyId);
	}
}
	kmb::Vector3D getAreaVectorOfSurface(kmb::bodyIdType bodyId) const;
	kmb::Vector3D getAreaVectorOfFaceGroup(const char* faceGroup) const;

	kmb::nodeIdType getCornerNodeIdOfSurface(kmb::bodyIdType bodyId,kmb::Vector3D dir) const;
	kmb::nodeIdType getCornerNodeIdOfFaceGroup(const char* faceGroup,kmb::Vector3D dir) const;


/**---------------------------------------------------------------------------
=begin
--- getElementVolume(elementId,bodyId=-1)
	elementId の要素の体積を求める。
	負の向きになっていたら体積も負になる。
	bodyId は省略可能だが省略すると少し遅くなる。
--- getElementArea(elementId,bodyId=-1)
	elementId の2次元要素の面積を求める。
	bodyId は省略可能だが省略すると少し遅くなる。
--- getVolume(bodyId)
	bodyId の領域の体積を求める。
--- getArea(bodyId)
	bodyId の領域の面積を求める。
=end
-----------------------------------------------------------------------------*/
	double getElementArea(elementIdType elementID,kmb::bodyIdType bodyID=kmb::Body::nullBodyId) const;
	double getElementVolume(elementIdType elementID,kmb::bodyIdType bodyID=kmb::Body::nullBodyId) const;
	double getArea(kmb::bodyIdType bodyId) const;
	double getVolume(kmb::bodyIdType bodyId) const;
/**----------------------------------------------------------------------------
=begin
--- getBoundaryCount(elementId,bodyId=-1)
	要素グループ bodyId の elementId の要素の境界の個数を返す。
	境界とは立体要素なら周りを囲む平面要素、平面要素なら周りを囲む線分要素のことを言う。
	近傍の個数は必ずこの数以下である。
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getBoundaryCount(kmb::elementIdType elementId,kmb::bodyIdType bodyId=kmb::Body::nullBodyId){
		kmb::ElementContainer::iterator element = self->findElement(elementId,bodyId);
		return INT2FIX(element.getBoundaryCount());
	}
}
/**-------------------------------------------------------------------------
=begin
--- getAspectRatio(elementId,bodyId=-1)
	4面体のアスペクト比を返す。
	アスペクト比とは、外接球の半径 / (内接球の半径*3) で定義されるもので、
	正4面体で最小値 1 を取る。値が 1 に近いほど形状が正四面体に近いとされる。
	4面体以外のものについては 0 を返す。
	bodyId を省略すると少し遅くなる。
=end
---------------------------------------------------------------------------*/

	double getAspectRatio(elementIdType elementID,kmb::bodyIdType bodyID=kmb::Body::nullBodyId);

/**-------------------------------------------------------------------------
=begin
--- getNaturalCoordinates(elementId,x,y,z,bodyId=-1)
	物理座標系を要素内自然座標系に変換する
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getNaturalCoordinates(kmb::elementIdType elementId,double x,double y,double z,kmb::bodyIdType bodyId=kmb::Body::nullBodyId)
	{
		VALUE ary = rb_ary_new();
		double values[3] = {0.0, 0.0, 0.0};
		int len = self->getNaturalCoordinates(bodyId,elementId,x,y,z,values);
		for(int i=0;i<len;++i){
			rb_ary_store(ary,i,rb_float_new(values[i]));
		}
		return ary;
	}
}
/**-------------------------------------------------------------------------
=begin
--- getPhysicalCoordinates(elementId,s,t,u,bodyId=-1)
	要素内自然座標から物理座標系へ
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getPhysicalCoordinates(kmb::elementIdType elementId,double s,double t,double u,kmb::bodyIdType bodyId=kmb::Body::nullBodyId)
	{
		VALUE ary = rb_ary_new();
		kmb::Point3D target;
		if( self->getPhysicalCoordinates(bodyId,elementId,s,t,u,target) ){
			for(int i=0;i<3;++i){
				rb_ary_store(ary,i,rb_float_new(target[i]));
			}
		}
		return ary;
	}
}
/**-------------------------------------------------------------------------
=begin
--- shapeFunction(elementType,naturalCoords)
	形状関数の値を取得する
=end
---------------------------------------------------------------------------*/
%extend{
	static VALUE shapeFunction(const char* etype,VALUE coords)
	{
		kmb::elementType elemType = kmb::ElementBase::getType(etype);
		double values[20];
		double naturalCoords[3] = {0.0, 0.0, 0.0};
		const int len = RARRAY_LEN(coords);
		for(int i=0;i<len;++i){
			naturalCoords[i] = NUM2DBL( rb_ary_entry(coords,i) );
		}
		kmb::MeshDB::shapeFunction( elemType, naturalCoords, values );
		const int nodeCount = kmb::Element::getNodeCount(elemType);
		VALUE ary = rb_ary_new();
		for(int i = 0;i<nodeCount;++i){
			rb_ary_store(ary,i,rb_float_new(values[i]));
		}
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
--- reverseElement(elementId,bodyId=-1)
--- reverseBody(bodyId)
	要素または領域すべての要素について、
	逆向きにする。
=end
-----------------------------------------------------------------------------*/
	bool reverseElement(kmb::elementIdType elementId,kmb::bodyIdType bodyId=kmb::Body::nullBodyId);
	void reverseBody(kmb::bodyIdType bodyId);


/**-----------------------------------------------------------------------
=begin
==検索処理

MeshDB では Octree 法を用いた高速検索が可能である。Octree のキャッシュを生成する
generateNodeSearchCache を実行した後で getNearestNode(x,y,z) を実行すると最も近い
節点の節点Idとその節点までの距離のペアを返す

--- generateNodeSearchCache
	登録されている節点すべてについて検索用のキャッシュを生成する
--- generateNodeSearchCacheOfBody( bodyId )
	bodyId の Body に含まれている節点すべてについて検索用のキャッシュを生成する
--- generateNodeSearchCacheOfData( name )
	name の Data に含まれている節点すべてについて検索用のキャッシュを生成する
--- appendNodeSearchCache( nodeId )
	検索用のキャッシュに nodeId の節点を追加する
--- clearNodeSearchCache
	節点検索用のキャッシュをクリアする
--- getNearestNode(x,y,z)
	(x,y,z) に一番近い節点を検索する。[nearestNodeId,distance]
	の形式で返す。キャッシュが生成されていないときには、
	全点検索法で検索する。
--- getNodesInRegion(region)
	直方体を与えて、そこに含まれる節点を検索する。

	 getNodesInRegion(RevocapMesh::BoxRegion.new(0.0,0.0,0.0,1.0,1.0,1.0))

	のように使う
--- generateElementSearchCache(bodyId,cacheType="octree")
	bodyId に登録されている要素を要素検索用のキャッシュに登録する
	cacheType は octree または bucket
--- clearElementSearchCache()
	要素検索用のキャッシュをクリアする
--- searchElement(bodyId,x,y,z,tolerance=0.0)
	bodyId の中で (x,y,z) が含まれている（またはもっとも近くにある）要素を検索する。
	tolerance は距離の閾値を与える。solid 要素の時 tolerance = 0.0 とすると、内部にあることが保証される。
#	solid 要素の場合、tolerance を与えると、内部外部判定の時の形状関数の値に余裕を持たせる
	戻り値は 要素Id 見つからなければ nil
--- searchElementInData(name,x,y,z,tolerance=0.0)
	Data の中で (x,y,z) が含まれている要素を検索する。
	tolerance を与えると、内部外部判定の時の形状関数の値に余裕を持たせる
	戻り値は 要素ID 見つからなければ nil
=end
-------------------------------------------------------------------------*/
	size_t generateElementSearchCache( kmb::bodyIdType bodyId, const char* cacheType="octree" );
	void clearElementSearchCache( void );
	size_t generateNodeSearchCache(void);
	size_t generateNodeSearchCacheOfBody( kmb::bodyIdType bodyId );
	size_t generateNodeSearchCacheOfData( const char* name, const char* stype=NULL );
	void appendNodeSearchCache(kmb::nodeIdType nodeId);
	void clearNodeSearchCache(void);
%extend{
	VALUE searchElement(kmb::bodyIdType bodyId,double x,double y,double z,double tolerance=0.0){
		kmb::elementIdType findElementId = self->searchElement(bodyId,x,y,z,tolerance);
		if( findElementId != kmb::Element::nullElementId ){
			return INT2NUM(findElementId);
		}else{
			return Qnil;
		}
	}











	VALUE searchElementInData(const char* name,double x,double y,double z,double tolerance=0.0){
		kmb::elementIdType findElementId = self->searchElementInData(name,x,y,z,tolerance);
		if( findElementId != kmb::Element::nullElementId ){
			return INT2NUM(findElementId);
		}else{
			return Qnil;
		}
	}
	VALUE getNearestNode(double x,double y,double z){
		VALUE ary = rb_ary_new();
		kmb::nodeIdType nodeId = kmb::nullNodeId;
		double dist = self->getNearestNode(x,y,z,nodeId);
		rb_ary_store(ary,0,INT2NUM(nodeId));
		rb_ary_store(ary,1,rb_float_new(dist));
		return ary;
	}
	VALUE getNodesInRegion(kmb::BoxRegion &region){
		VALUE ary = rb_ary_new();
		std::set<kmb::nodeIdType> selectedNodes;
		self->getNodesInRegion(&region,selectedNodes);
		std::set<kmb::nodeIdType>::iterator nIter = selectedNodes.begin();
		while( nIter != selectedNodes.end() )
		{
			kmb::nodeIdType nodeId = *nIter;
			rb_ary_push(ary,INT2NUM(nodeId));
			++nIter;
		}
		return ary;
	}
	VALUE getNodesInRegion(kmb::Sphere &region){
		VALUE ary = rb_ary_new();
		std::set<kmb::nodeIdType> selectedNodes;
		self->getNodesInRegion(&region,selectedNodes);
		std::set<kmb::nodeIdType>::iterator nIter = selectedNodes.begin();
		while( nIter != selectedNodes.end() )
		{
			kmb::nodeIdType nodeId = *nIter;
			rb_ary_push(ary,INT2NUM(nodeId));
			++nIter;
		}
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
--- getNearestNode(id)
	節点 ID を与えて、自分以外の最も近い節点 ID と距離の組の Array を返す。

--- getNearNodes(x,y,z,tolerance)
	座標値と tolerance を与えて、その距離以下の節点の ID の Array を返す。

--- getNearNodes(id,tolerance)
	節点 ID と tolerance を与えて、その距離以下の自分以外の節点の ID の Array を返す。

=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getNearestNode(kmb::nodeIdType nodeID){
		kmb::nodeIdType nearestId = kmb::nullNodeId;
		double dist = self->getNearestNode(nodeID,nearestId);
		VALUE ary = rb_ary_new();
		rb_ary_store(ary, 0, INT2FIX(nearestId));
		rb_ary_store(ary, 1, rb_float_new(dist));
		return ary;
	}
	VALUE getNearNodes(double x,double y,double z,double tolerance){
		VALUE ary = rb_ary_new();
		if( self->getNodes() == NULL )
			return ary;
		kmb::Point3D node;
		kmb::Point3DContainer::iterator nIter = self->getNodes()->begin();
		kmb::Point3DContainer::iterator endIter = self->getNodes()->end();
		while( nIter != endIter )
		{
			if( nIter.getPoint(node) && node.distance(x,y,z) < tolerance ){
				rb_ary_push(ary, INT2NUM(nIter.getId()) );
			}
			++nIter;
		}
		return ary;
	}
	VALUE getNearNodes(kmb::nodeIdType id,double tolerance){
		VALUE ary = rb_ary_new();
		if( self->getNodes() == NULL )
			return ary;
		kmb::Node target;
		if( self->getNode(id,target) )
			return ary;
		kmb::Point3D node;
		kmb::Point3DContainer::iterator nIter = self->getNodes()->begin();
		kmb::Point3DContainer::iterator endIter = self->getNodes()->end();
		while( nIter != endIter )
		{
			if( id != nIter.getId() && nIter.getPoint(node) && node.distance(target) < tolerance ){
				rb_ary_push(ary, INT2NUM(nIter.getId()) );
			}
			++nIter;
		}
		return ary;
	}
}

/**-----------------------------------------------------------------------
=begin
==近傍処理

MeshDBの近傍処理では
* 節点に対し、その節点を含む要素を取得
* 節点に対し、その節点と接続している節点の取得
* 要素に対し、その要素に接している要素の取得
を行うことができる。これらは要素の種類によらず、ソリッドメッシュ、サーフェスメッシュ
にかかわらず実行することができる。

また、この処理を行うための近傍情報の生成と、それで利用したメモリの
解放を行うことができるため、必要のない時には無駄なメモリは使わない。

応用処理として
* 要素に対して接している要素を取得することで、外に顕になっている面を持つ要素の検索
* 節点の周りの要素を取得し、その法線ベクトルの平均を求める
などを容易に実装することができる。

--- clearNeighborsInfo()
	キャッシュに記憶した近傍情報をクリアする。

=end
-------------------------------------------------------------------------*/
%extend{
	void clearNeighborsInfo(void){
		self->neighborInfo.clear();
	}
}

/**------------------------------------------------------------------------
=begin
--- generateNeighborsInfo(bodyId)
	近傍計算に必要な情報を bodyId のボディに含まれる要素から
	生成してキャッシュに保存する。
	このメソッドを実行後に

	* getNeighborNodes(nodeId)
	* getSurroundingElements(nodeId)
	* getSurroundingCount(nodeId)
	* getNeighborElements(elementId)
	* getAdjacentFace(face)

	が有効になる
=end
--------------------------------------------------------------------------*/
%extend{
	void generateNeighborsInfo(kmb::bodyIdType bodyId){
		kmb::ElementContainer* body = self->getBodyPtr( bodyId );
		if( body != NULL ){
			self->neighborInfo.appendCoboundary(body);
		}
	}
}

/**-------------------------------------------------------------------------
=begin
--- getNeighborNodes(nodeId)
	nodeId の節点番号の節点に接している節点番号の
	配列を Array で返す。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getNeighborNodes(kmb::nodeIdType nodeId){
		VALUE ary = rb_ary_new();
		std::set<kmb::nodeIdType> neighbor;
		self->neighborInfo.getNodeNeighbor(nodeId,neighbor,self);
		int j=0;
		std::set<kmb::nodeIdType>::iterator nIter = neighbor.begin();
		while( nIter != neighbor.end() ){
			rb_ary_store(ary,j,INT2FIX(*nIter));
			++nIter;
			++j;
		}
		return ary;
	}
}

/**------------------------------------------------------------------------
=begin
--- getSurroundingElements(nodeId)
	nodeId の節点番号の節点を含む要素Idの
	配列を Array で返す。
=end
--------------------------------------------------------------------------*/
%extend{
	VALUE getSurroundingElements(kmb::nodeIdType nodeId){
		VALUE ary = rb_ary_new();
		kmb::NodeNeighbor::iterator nIter = self->neighborInfo.beginIteratorAt(nodeId);
		kmb::NodeNeighbor::iterator end = self->neighborInfo.endIteratorAt(nodeId);
		while( nIter != end ){
			rb_ary_push(ary,INT2FIX(nIter->second));
			++nIter;
		}
		return ary;
	}
}

/**------------------------------------------------------------------------
=begin
--- getSurroundingCount(nodeId)
	nodeId の節点番号の節点を含む要素の個数を返す。
=end
--------------------------------------------------------------------------*/
%extend{
	int getSurroundingCount(kmb::nodeIdType nodeId){
		return static_cast<int>( self->neighborInfo.getElementCountAroundNode(nodeId) );
	}
}

/**-------------------------------------------------------------------------
=begin
--- getNeighborElements(elementId)
	要素番号 elementId の要素が
	接する要素番号の配列を Array で返す。
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE getNeighborElements(kmb::elementIdType elementId){
		VALUE ary = rb_ary_new();
		kmb::elementIdType neighbor[6];
		self->neighborInfo.getNeighborElements(elementId,neighbor,self);
		kmb::ElementContainer::iterator element = self->findElement( elementId );
		const int boundaryNum = element.getBoundaryCount();
		for(int i=0;i<boundaryNum;++i){
			if( neighbor[i] != kmb::Element::nullElementId ){
				rb_ary_push(ary,INT2FIX( neighbor[i] ));
			}
		}
		return ary;
	}
}

/**-------------------------------------------------------------------------
=begin
--- getAdjacentFace(face)
	Face (要素番号と内部面番号の組) を与えて、その要素の内部面番号で接する Face を返す。
	例えば要素番号 100 の四面体の内部面番号 1 の面を与えた時、
	その面と要素番号 200 の四面体の内部面番号 2 が接している時に [200,2] を返す。
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE getAdjacentFace(VALUE face){
		Check_Type(face, T_ARRAY);
		int elementId = NUM2INT( rb_ary_entry(face, 0) );
		int localId = NUM2INT( rb_ary_entry(face, 1) );
		kmb::Face f(elementId,localId);
		kmb::Face adjf;
		if( self->neighborInfo.getAdjacentFace(f,adjf,self) ){
			VALUE a = rb_ary_new();
			rb_ary_store(a,0,INT2FIX(adjf.getElementId()));
			rb_ary_store(a,1,INT2FIX(adjf.getLocalFaceId()));
			return a;
		}
		return Qnil;
	}
}


/**------------------------------------------------------------------------
=begin
==要素間関係
--- getFaces(elementID,bodyID=-1)
	要素ID elementID の要素の面を構成する頂点配列を2次元配列で返す。
	例えば elementID = 0 の要素が
	 ["TETRAHEDRON",100,101,102,103]
	だとすると、getFaces(0) の結果は
	 [ [101,102,103], [100,103,102], [100,101,103], [100,102,101] ]
	となる
=end
--------------------------------------------------------------------------*/
%extend{
	VALUE getFaces(kmb::elementIdType elementID,kmb::bodyIdType bodyID=kmb::Body::nullBodyId){
		VALUE ary = rb_ary_new();
		kmb::ElementContainer::iterator element = self->findElement(elementID,bodyID);
		if( !element.isFinished() && element.getDimension() == 3){
			const int faceNum = element.getBoundaryCount();
			for(int i=0;i<faceNum;++i){
				VALUE face = rb_ary_new();
				const int len = element.getBoundaryNodeCount(i);
				for(int j=0;j<len;++j){
					int node = element.getBoundaryCellId(i,j);
					rb_ary_store(face,j,INT2FIX(node));
				}
				rb_ary_store(ary,i,face);
			}
		}else if( !element.isFinished() && element.getDimension() == 2){
			VALUE face = rb_ary_new();
			const int len = element.getNodeCount();
			for(int i=0; i<len; ++i){
				rb_ary_store(face,i,INT2FIX(element.getCellId(i)));
			}
			rb_ary_store(ary,0,face);
		}
		return ary;
	}
}
/**---------------------------------------------------------------------------
=begin
--- isFace(elementId,ary,bodyId=-1)
	立体要素 elementID が頂点配列 ary を表面として持つかどうか。
	等しい向きの表面があれば1、
	逆向きのの表面があれば-1、
	表面が無ければ0を返す。
=end
-----------------------------------------------------------------------------*/
%extend{
	int isFace(kmb::elementIdType elementId,VALUE ary,kmb::bodyIdType bodyId=kmb::Body::nullBodyId){
		Check_Type(ary, T_ARRAY);
		int result = 0;

		kmb::ElementContainer::iterator elem = self->findElement(elementId,bodyId);
		if( elem.isFinished() || elem.getDimension() != 3 ){
			return result;
		}

		int length = RARRAY_LEN(ary);
		int index;
		switch(length){
			case 3:
			case 6:
				result = elem.isFace(
					elem.indexOf( FIX2INT(rb_ary_entry(ary,0)) ),
					elem.indexOf( FIX2INT(rb_ary_entry(ary,1)) ),
					elem.indexOf( FIX2INT(rb_ary_entry(ary,2)) ),index);
				break;
			case 4:
			case 8:
				result = elem.isFace(
					elem.indexOf( FIX2INT(rb_ary_entry(ary,0)) ),
					elem.indexOf( FIX2INT(rb_ary_entry(ary,1)) ),
					elem.indexOf( FIX2INT(rb_ary_entry(ary,2)) ),
					elem.indexOf( FIX2INT(rb_ary_entry(ary,3)) ),index);
				break;
			default:
				break;
		}
		return result;
	}
}
/**----------------------------------------------------------------------------
=begin
--- getCommonElement(e0,e1)
	同じボディに含まれる要素ID e0 と e1 が共有している要素があれば、
	それを getElement と同じ形式で返す。
	その要素は MeshDB に登録されているとは限らない。
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE getCommonElement(kmb::elementIdType e0,kmb::elementIdType e1){
		VALUE ary = rb_ary_new();
		kmb::bodyIdType b0 = self->getBodyId(e0);
		kmb::bodyIdType b1 = self->getBodyId(e1);
		if( b0==b1 ){
			kmb::ElementContainer* body = self->getBodyPtr( b0 );
			if( body ){
				kmb::nodeIdType cell[8];
				kmb::elementType etype = kmb::ElementRelation::common( body->find(e0), body->find(e1), cell );
				if( etype != kmb::UNKNOWNTYPE ){
					rb_ary_store(ary,0,rb_str_new2(kmb::ElementBase::getTypeString(etype).c_str()));
					const int len = kmb::Element::getNodeCount( etype );
					for(int i=0;i<len;++i){
						rb_ary_store(ary,i+1,INT2FIX(cell[i]));
					}
				}
			}
		}
		return ary;
	}
}
/**---------------------------------------------------------------------------
=begin
--- getElementRelation(e0,e1)
	要素ID e0 と要素ID e1 の関係を文字列で返す。
=end
-----------------------------------------------------------------------------*/
%extend{
	VALUE getElementRelation(kmb::elementIdType e0, kmb::elementIdType e1){
		kmb::bodyIdType b0 = self->getBodyId(e0);
		kmb::bodyIdType b1 = self->getBodyId(e1);
		kmb::ElementRelation::relationType result;
		kmb::ElementContainer::iterator element0 = self->findElement(e0,b0);
		kmb::ElementContainer::iterator element1 = self->findElement(e1,b1);
		if( element0.isFinished() || element1.isFinished() ){
			result = kmb::ElementRelation::UNKNOWNRELATION;
		}else{
			int index=-1;
			int otherIndex=-1;
			result = kmb::ElementRelation::getRelation( element0, index, element1, otherIndex );
		}
		return rb_str_new2(kmb::ElementRelation::getRelationTypeString(result).c_str());
	}
}

/**-----------------------------------------------------------------------
=begin
==メッシュ結合・変換
--- mergeBody(bodyIds)
	複数の Body をまとめて一つの Body にする。
	もとの Body からは要素を削除する。
	まとめた新しい Body の BodyId を返す。

--- mergeBody(appendId,bodyId,moveFlag=true)
	appendId の Body を bodyId の Body に結合する。
	moveFlag = true のときは、appendId の Body から要素を削除する。
	moveFlag = false のときは、appendId の要素は残して、要素 Id を新たにつけなおして bodyId の Body に登録する
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE mergeBody(VALUE bodyIds){
		Check_Type(bodyIds, T_ARRAY);
		const int len = RARRAY_LEN(bodyIds);
		std::set< kmb::bodyIdType > bodies;
		for( int i=0; i<len; ++i){
			VALUE id = rb_ary_entry( bodyIds, i );
			bodies.insert( static_cast<kmb::bodyIdType>( NUM2INT(id) ) );
		}
		return INT2NUM(self->mergeBody( bodies ));
	}
}
	kmb::bodyIdType mergeBody( kmb::bodyIdType appendBodyId, kmb::bodyIdType bodyId, bool moveFlag=true);
/**-----------------------------------------------------------------------
=begin
--- convertToLinearBody(bodyId)
	Body をすべて１次要素に変換する。
=end
---------------------------------------------------------------------------*/
	kmb::bodyIdType convertToLinearBody( kmb::bodyIdType bodyId );

/**-------------------------------------------------------------------------
=begin
--- importBody(mesh,bodyId,nodeMapper=nil,ctype="SLAVE")
	別の Mesh の Body を追加する。必要な節点も同時に追加し、各要素の節点配列は
	自動的に書き換える。
	nodeMapper に文字列を与えた時には、引数の節点の対応を NodeVariable モードの
	Data として記憶する。すでに記憶されている Data がある時は、
	それを更新し、要素を追加するときもその節点の対応を用いる。
	ctype の引数で節点の対応を記憶するインスタンスを指定する。
	MASTER で self、SLAVE で mesh、BOTH で両方、None では nodeMapper を
	与えていても記憶しない。

--- importBodyWithNodeMatching(mesh,bodyId,tolerance,nodeMapper=nil)
	別の Mesh の Body を追加する。必要な節点は tolerance 以下にすでに節点がないかを調べて、
	存在すればそれを使い、なければ追加する。各要素の節点配列は自動的に書き換える。
	nodeMapper に文字列を与えた時には、引数の mesh に節点の対応を NodeVariable モードの
	Data として記憶する。すでに記憶されている Data がある時は、
	それを更新し、要素を追加するときもその節点の対応を用いる。

--- importAllBody(mesh)
	mesh のすべての Body を追加する。
=end
---------------------------------------------------------------------------*/
%extend{
	kmb::bodyIdType importBody(kmb::MeshDB& otherMesh,kmb::bodyIdType bodyId,const char* nodeMapper=NULL,const char* ctype="SLAVE"){
		if( strcmp(ctype,"SLAVE") == 0 ){
			return self->importBody(otherMesh,bodyId,nodeMapper,kmb::SLAVE);
		}else if( strcmp(ctype,"MASTER") == 0){
			return self->importBody(otherMesh,bodyId,nodeMapper,kmb::MASTER);
		}else if( strcmp(ctype,"BOTH") == 0){
			return self->importBody(otherMesh,bodyId,nodeMapper,kmb::BOTH);
		}else if( strcmp(ctype,"NONE") == 0){
			return self->importBody(otherMesh,bodyId,nodeMapper,kmb::NONE);
		}else{
			return self->importBody(otherMesh,bodyId,nodeMapper);
		}
	}
}
	kmb::bodyIdType importBodyWithNodeMatching(kmb::MeshDB& otherMesh, kmb::bodyIdType bodyId, double tolerance, const char* coupleName=NULL);
	size_t importAllBody(kmb::MeshData& otherMesh);


/**---------------------------------------------------------------------
=begin
--- getDataNameArray
--- getDataNameArray(specType)
	Mesh データに含まれる境界条件、材料属性などのプリ情報における MeshSpec の名前、
	およびポスト情報の計算結果のノードにおける値を保持した名前の配列を返す。
=end
---------------------------------------------------------------------*/
%extend{
	VALUE getDataNameArray(void){
		const std::multimap< std::string, kmb::DataBindings* > mapper = self->getDataBindingsMap();
		VALUE ary = rb_ary_new();
		std::multimap< std::string, kmb::DataBindings*>::const_iterator
			iter = mapper.begin();
		while( iter != mapper.end() )
		{
			std::string name = iter->first;
			rb_ary_push(ary, rb_str_new2( name.c_str() ));
			++iter;
		}
		return ary;
	}
	VALUE getDataNameArray(const char* specType){
		const std::multimap< std::string, kmb::DataBindings* > mapper = self->getDataBindingsMap();
		VALUE ary = rb_ary_new();
		std::multimap< std::string, kmb::DataBindings*>::const_iterator
			iter = mapper.begin();
		while( iter != mapper.end() )
		{
			kmb::DataBindings* binding = iter->second;
			if( binding != NULL &&
				binding->getSpecType() == specType )
			{
				std::string name = iter->first;
				rb_ary_push(ary, rb_str_new2( name.c_str() ));
			}
			++iter;
		}
		return ary;
	}
}
/**-------------------------------------------------------------------------
=begin
--- replaceIdOfData(name,oldID,newID,stype=nil)
	name で定義された Data の Id を取り替える
=end
---------------------------------------------------------------------------*/
	bool replaceIdOfData(const char* name,kmb::idType oldID,kmb::idType newID,const char* stype=NULL);
/**-------------------------------------------------------------------------
=begin
--- convertData(org,conv,orgstype=nil,convstype=nil)
	org のデータを conv に変換する。
	conv のデータはあらかじめ作成しておくこと。
	GROUP 系のデータについては、ValueType が一致する時に値もコピーする。
	対応済み：
	org : NodeGroup => conv : FaceGroup
	 面上のすべての節点が org データに含まれている場合に conv に登録する
	org : NodeGroup => conv : ElementGroup
	 要素のすべての節点が org データに含まれている場合に conv に登録する
	org : FaceGroup => conv : NodeGroup
	 面上のすべての節点を conv に登録する
	org : ElementGroup => conv : NodeGroup
	 要素上のすべての節点を conv に登録する
=end
---------------------------------------------------------------------------*/
	int convertData(const char* org, const char* conv, const char* orgstype=NULL,const char* convstype=NULL);
	int convertBodyToData(kmb::bodyIdType bodyId, const char* name,const char* stype=NULL);
/**-------------------------------------------------------------------------
=begin
--- getElementCountOfData(name)
	name のグループに含まれる要素数を出力する
=end
---------------------------------------------------------------------------*/
	int getElementCountOfData(const char* name,const char* stype=NULL);
/**-------------------------------------------------------------------------
=begin
--- getNodeCountOfData(name)
	name のグループに含まれる節点数を出力する
=end
---------------------------------------------------------------------------*/
	int getNodeCountOfData(const char* name,const char* stype=NULL);
/**-------------------------------------------------------------------------
=begin
--- eachNodeIdOfData(name,stype=nil)
	name のグループに含まれる節点のIdを取り出す iterator
	NodeVariable または NodeGroup の場合はそのものの iterator
	ElementVariable または ElementGroup の場合は要素に含まれる節点の iterator
	FaceVariable または FaceGroup の場合は FACE に含まれる節点の iterator
	BodyVariable または BodyGroup の場合は BODY に含まれる節点の iterator
=end
---------------------------------------------------------------------------*/
%extend{
	void eachNodeIdOfData(const char* name,const char* stype=NULL){
		std::set<kmb::nodeIdType> nodeSet;
		self->getNodeSetFromDataBindings(nodeSet,name,stype);
		std::set<kmb::nodeIdType>::iterator nIter = nodeSet.begin();
		while( nIter != nodeSet.end() ){
			rb_yield( INT2NUM( (*nIter) ) );
			++nIter;
		}
	}
}
/**--------------------------------------------------------------------------
=begin
--- getBoundingBoxOfData(name,stype)
	物理値またはプリ処理の名前を与えて、登録されている節点や要素の
	BoundingBox を返す。
=end
---------------------------------------------------------------------------*/
	const kmb::BoundingBox getBoundingBoxOfData(const char* key,const char* stype=NULL) const;
/**--------------------------------------------------------------------------
=begin
--- getUniqueDataName(prefix,num=0)
	prefixnum （必要なら num の値を増やして）形式の使われていない名前を取得する。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getUniqueDataName(const char* prefix,int num=0){
		std::string name = self->getUniqueDataName(prefix,num);
		return rb_str_new2(name.c_str());
	}
}
/**--------------------------------------------------------------------------
=begin
==データ検索・補間
--- getNearestValue(name,value,stype=nil)
	物理値を与えて、name グループに登録されているものの中から最も近い値を持つ ID を返す。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getNearestValue(const char* name,VALUE v,const char* stype=NULL){
		kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
		if( data == NULL ){
			return Qnil;
		}
		bool bface = false;
		switch(data->getBindingMode()){
			case kmb::DataBindings::NodeVariable:
			case kmb::DataBindings::ElementVariable:
			case kmb::DataBindings::BodyVariable:
				bface = false;
				break;
			case kmb::DataBindings::FaceVariable:
				bface = true;
				break;
			default:
				return Qnil;
		}
		switch(data->getValueType()){
			case kmb::PhysicalValue::Integer:
			{
				Check_Type(v, T_FIXNUM);
				if( bface ){
					kmb::Face f;
					self->getNearestValue(name,new kmb::IntegerValue(FIX2INT(v)),f);
					VALUE idPair = rb_ary_new();
					rb_ary_store(idPair,0,INT2FIX(f.getElementId()));
					rb_ary_store(idPair,1,INT2FIX(f.getLocalFaceId()));
					return idPair;
				}else{
					kmb::idType id;
					self->getNearestValue(name,new kmb::IntegerValue(FIX2INT(v)),id);
					return INT2NUM(id);
				}
			}
			case kmb::PhysicalValue::Scalar:
			{
				Check_Type(v, T_FLOAT);
				if( bface ){
					kmb::Face f;
					self->getNearestValue(name,new kmb::ScalarValue(NUM2DBL(v)),f);
					VALUE idPair = rb_ary_new();
					rb_ary_store(idPair,0,INT2FIX(f.getElementId()));
					rb_ary_store(idPair,1,INT2FIX(f.getLocalFaceId()));
					return idPair;
				}else{
					kmb::idType id;
					self->getNearestValue(name,new kmb::ScalarValue(NUM2DBL(v)),id);
					return INT2NUM(id);
				}
			}
			case kmb::PhysicalValue::Vector3:
			{
				Check_Type(v, T_ARRAY);
				double d[3];
				d[0] = NUM2DBL(rb_ary_entry(v,0));
				d[1] = NUM2DBL(rb_ary_entry(v,1));
				d[2] = NUM2DBL(rb_ary_entry(v,2));
				if( bface ){
					kmb::Face f;
					self->getNearestValue(name,new kmb::Vector3Value(d),f);
					VALUE idPair = rb_ary_new();
					rb_ary_store(idPair,0,INT2FIX(f.getElementId()));
					rb_ary_store(idPair,1,INT2FIX(f.getLocalFaceId()));
					return idPair;
				}else{
					kmb::idType id;
					self->getNearestValue(name,new kmb::Vector3Value(d),id);
					return INT2NUM(id);
				}
			}
			default:
				return Qnil;
		}
		return Qnil;
	}
}
/**--------------------------------------------------------------------------
=begin
--- getInterpolatedValue(name,x,y,z,tolerance=0.0,stype=nil)
--- getInterpolatedMultiValues(x,y,z,tolerance=0.0)
	物理値の名前を与えて、補間された値を返す。
	tolerance は形状関数の値で内部にあるかどうかを判定する閾値を与える。
	連成面での物理量を取り出す場合など以外は 0.0 で構わない。
	Multi の場合は setTargetData で与えられた複数の物理量について補間値を求める。

--- getInterpolatedValueInData(name,target,x,y,z,tolerance=0.0,stype=nil)
--- getInterpolatedMultiValuesInData(names,target,x,y,z,tolerance=0.0,stype=nil)
	物理値の名前 name を与えて、補間された値を返す。
	補間される要素は target で与えられたグループのみから検索する。
	tolerance は形状関数の値で内部にあるかどうかを判定する閾値を与える。
	連成面での物理量を取り出す場合など以外は 0.0 でかまいません。
	Multi の場合は setTargetData で与えられた複数の物理量について補間値を求める。

	連成面の組 (A,B) が与えられた時に、面 A に B から物理量を補間して与える場合に、
	target に B を指定する。B は ElementGroup または FaceGroup の Data である
	必要がある。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getInterpolatedValue(const char* name,double x,double y,double z,double tolerance=0.0,const char* stype=NULL){
		double v[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		int res = self->getInterpolatedValue(name,x,y,z,v,tolerance,stype);
		if(res == 1){
			return rb_float_new(v[0]);
		}else if(res > 1){
			VALUE values = rb_ary_new();
			for(int i=0;i<res;++i){
				rb_ary_push( values, rb_float_new(v[i]) );
			}
			return values;
		}
		return Qnil;
	}
	VALUE getInterpolatedMultiValues(double x,double y,double z,double tolerance=0.0){
		int len = self->getTargetDataDim();
		if( len <= 0 ){
			return Qnil;
		}
		double* v = new double[len];

		int res = self->getInterpolatedValues(x,y,z,v,tolerance);
		VALUE values = rb_ary_new();
		for(int i=0;i<res;++i){
			rb_ary_push( values, rb_float_new(v[i]) );
		}
		delete[] v;

		return values;
	}
	VALUE getInterpolatedValueInData(const char* name,const char* target,double x,double y,double z,double tolerance=0.0,const char* stype=NULL){
		double v[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		int res = self->getInterpolatedValueInData(name,target,x,y,z,v,tolerance,stype);
		if(res == 1){
			return rb_float_new(v[0]);
		}else if(res > 1){
			VALUE values = rb_ary_new();
			for(int i=0;i<res;++i){
				rb_ary_push( values, rb_float_new(v[i]) );
			}
			return values;
		}
		return Qnil;
	}
	VALUE getInterpolatedMultiValuesInData(const char* target,double x,double y,double z,double tolerance=0.0,const char* stype=NULL){
		int len = self->getTargetDataDim();
		if( len <= 0 ){
			return Qnil;
		}
		double* v = new double[len];

		int res = self->getInterpolatedValuesInData(target,x,y,z,v,tolerance);
		if( res <= 0 ){
			delete[] v;
			return Qnil;
		}

		VALUE values = rb_ary_new();
		for(int i=0;i<res;++i){
			rb_ary_push( values, rb_float_new(v[i]) );
		}
		delete[] v;

		return values;
	}
}

/**--------------------------------------------------------------------------
=begin
--- getMinMaxValue(name,stype)
	物理値の名前を与えて、登録されている値の最小最大を返す。
	ベクトル値については、ここでは成分ごとに最小最大を求める。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getMinMaxValue(const char* name,const char* stype=NULL){
		const kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
		if( data && data->getValueType() == kmb::PhysicalValue::Scalar ){
			kmb::BoundingBox1D bbox;
			self->getMinMaxValue(data,bbox);
			VALUE ary = rb_ary_new();
			rb_ary_push(ary,rb_float_new(bbox.getMin()));
			rb_ary_push(ary,rb_float_new(bbox.getMax()));
			return ary;
		}else if( data && data->getValueType() == kmb::PhysicalValue::Vector3 ){
			kmb::BoundingBox bbox;
			self->getMinMaxValue(data,bbox);
			VALUE ary = rb_ary_new();
			VALUE aryMin = rb_ary_new();
			VALUE aryMax = rb_ary_new();
			rb_ary_push(aryMin,rb_float_new(bbox.minX()));
			rb_ary_push(aryMin,rb_float_new(bbox.minY()));
			rb_ary_push(aryMin,rb_float_new(bbox.minZ()));
			rb_ary_push(ary,aryMin);
			rb_ary_push(aryMax,rb_float_new(bbox.maxX()));
			rb_ary_push(aryMax,rb_float_new(bbox.maxY()));
			rb_ary_push(aryMax,rb_float_new(bbox.maxZ()));
			rb_ary_push(ary,aryMax);
			return ary;
		}else{
			return Qnil;
		}
	}
}

/**--------------------------------------------------------------------------
=begin
--- getMinMaxValueWithId(name,stype=nil,comp=-1)
	データの最大最小値を求める。
	ベクトル値については、絶対値の最小最大を求めることに注意する。
	戻り値は [minVal, minId, maxVal, maxId] の形式
=end
----------------------------------------------------------------------------*/

%extend{
	VALUE getMinMaxValueWithId(const char *name, const char* stype=NULL, int comp=-1){
		kmb::MinMaxWithId<kmb::idType> minmax;
		const kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
		if( data && self->getMinMaxValueWithId(data,minmax,comp) ){
			VALUE ary = rb_ary_new();
			rb_ary_store(ary,0,rb_float_new(minmax.getMin()));
			rb_ary_store(ary,1,INT2FIX(minmax.getMinId()));
			rb_ary_store(ary,2,rb_float_new(minmax.getMax()));
			rb_ary_store(ary,3,INT2FIX(minmax.getMaxId()));
			return ary;
		}else{
			return Qnil;
		}
	}
}

/**--------------------------------------------------------------------------
=begin
==その他
--- calcMeshProperty(name,bodyId=-1)
	メッシュの性質などを計算して物理量に登録する。
	 name mode value
	 "NormalVectorOnNode" NodeVariable Vector3 節点の周りの要素の法線ベクトルの平均値
	 "AspectRatio" ElementVariable Scalar
	 "ElementVolume" ElementVariable Scalar
	 "MaxEdgeLength" ElementVariable Scalar
	 "AverageEdgeLength" ElementVariable Scalar
	 "EdgeLengthRatio" ElementVariable Scalar
	 "MinAngle" ElementVariable Scalar
	 "MinJacobian" ElementVariable Scalar
=end
---------------------------------------------------------------------------*/
	bool calcMeshProperty(const char* name,kmb::bodyIdType bodyId=-1);

/*------------------------------------------------------------------------------
test $ ruby -e "require 'RevocapMesh'; RevocapMesh::MeshDB::test();"
-------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
%extend{
	static VALUE test(void){
		return Qnil;
	}
}
-------------------------------------------------------------------------------*/
/*---------------------------- end of test ------------------------------------*/
};
}
