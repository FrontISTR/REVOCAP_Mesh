/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : MeshData                                           #
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
/**--------------------------------------------------------------------
=begin
= RevocapMesh::MeshData

RevocapMesh::MeshData クラスはメッシュデータについてのコンテナ機能に関するものだけを実装した基底クラスである。
ＩＯスクリプトはこのクラスで実装されているメソッドのみを用いて実装するようにする。
RevocapMesh::MeshDB はこのクラスを継承している。
他のメッシュコンテナクラスについても、このクラスで定義されてメソッドを実装すると、ＩＯスクリプトを利用することができる。

==メソッド一覧

===クラスメソッド

((<MeshData.new>))

===メッシュ管理

((<beginModel>))
((<endModel>))
((<clearModel>))

===座標系

((<setCoordinate>))
((<translateCoordinate>))

===節点管理

((<beginNode>))
((<addNode>))
((<addNodeWithId>))
((<endNode>))
((<getNodeCount>))
((<getNode>))
((<eachNode>))
((<eachNodeWithId>))
((<insertNode>))
((<insertNodeWithId>))
((<updateNode>))
((<getNodeContainerType>))

===要素管理

((<beginElement>))
((<addElement>))
((<addElementWithId>))
((<endElement>))
((<insertElement>))
((<insertElementWithId>))
((<getElementCount>))
((<getElementCountByType>))
((<getDimension>))
((<getDegree>))
((<isUniqueElementType>))
((<getBodyName>))
((<setBodyName>))
((<getBodyIdByName>))
((<getElementContainerType>))
((<getElement>))
((<eachElement>))
((<eachElementWithId>))
((<getBodyCount>))
((<clearBody>))

===データ管理

((<createData>))
((<getDataCount>))
((<hasData>))
((<renameData>))
((<deleteData>))
((<clearData>))
((<getDataMode>))
((<getDataValueType>))
((<getDataSpecType>))
((<setDataSpecType>))
((<getDataTargetBodyId>))
((<getDataContainerType>))
((<eachDataName>))
((<getIdCount>))
((<setDefaultSpecType>))
((<getValueAtId>))
((<setValueAtId>))
((<getValue>))
((<setValue>))
((<addId>))
((<hasId>))
((<deleteId>))
((<setTargetData>))
((<setMultiValuesAtId>))
((<eachId>))
((<eachIdWithValue>))
((<faceGroupToBody>))

=end
---------------------------------------------------------------*/

namespace kmb{

class MeshData
{
public:
/**--------------------------------------------------------------------
=begin
== クラスメソッド
--- MeshData.new()
	MeshData クラスのコンストラクタである。MeshData オブジェクトのインスタンスを生成する。
=end
---------------------------------------------------------------------*/
	MeshData(void);
	virtual ~MeshData(void);

/**--------------------------------------------------------------------
=begin
==メッシュ管理

--- beginModel()
	メッシュの登録を開始する。
	beginNode addNode endNode beginElement addElement endElement
	を始める前に呼び出す。

--- endModel()
	メッシュの登録を終了する。
	* beginNode
	* addNode
	* endNode
	* beginElement
	* addElement
	* endElement
	などの節点・要素情報、および
	* createData
	* setValueAtId
	* setValue
	* addId
	などプリ・ポストのための情報を読み込んだ後に実行する。
	実装ではこのメソッドが呼ばれた時点で内部データの最適化等を行う。

=end
----------------------------------------------------------------------*/
	void beginModel(void);
	void endModel(void);

/**---------------------------------------------------------------------
=begin
--- clearModel()
	MeshDB のすべてのデータを消去し、初期化する。
=end
------------------------------------------------------------------------*/
	void clearModel(void);

/**---------------------------------------------------------------------
=begin
--- setCoordinate(mtrx)
--- translateCoordinate(x,y,z)
	座標系を指定する。

=end
------------------------------------------------------------------------*/
	void setCoordinate( const kmb::Matrix4x4& matrix );
	void translateCoordinate( double x, double y, double z );



/**----------------------------------------------------------------------
=begin
==節点管理

MeshData では3次元空間内の点の座標 (x,y,z) を節点として管理する。
MeshData では節点にはすべて節点Idが付与される。

--- beginNode(size=0,containerType=nil)
	size 個の節点の登録を開始する。
	id が連続でない場合は size には空き番号の個数も含めたものを指定する。
	例えば、配列での実装なら 0 から size-1 までの節点Idが登録可能になる。
	containerType は目的に応じて節点コンテナを変更する場合に用いる。
	デフォルトでは節点コンテナとして節点IDをキーとした連想配列を用いる。

	containerType の例
	* ２次元の節点の連想配列 "stl::map<id,Point2D*>"
	* コンテナに stl::map を使う（デフォルト） "stl::map<id,Point3D*>"
	* コンテナに stl::vector を使う "stl::vector<Point3D*>"
	* OpenGL で glDrawArray および glDrawElements を利用可能な３次元配列 "double_array"
=end
------------------------------------------------------------------------*/
	void beginNode(unsigned int size=0,const char* containerType=NULL);

/**----------------------------------------------------------------------
=begin
--- addNode(x,y,z)
--- addNodeWithId(x,y,z,id)
	addNode メソッドでは
	座標 (x,y,z) を与えて、
	その座標の節点を登録して付与された節点Idを返す。
	通常は現在登録されている節点の最大節点Id+1が付与される。
	addNodeWithId メソッドで節点Idをあらかじめ与えて登録することも出来る。
	この場合は既に登録されている場合は -1 を返す。
	２次元コンテナを利用している時には z 座標を無視する
=end
-------------------------------------------------------------------------*/
	kmb::nodeIdType addNode(double x,double y,double z);
	kmb::nodeIdType addNodeWithId(double x,double y,double z,kmb::nodeIdType id);

/**-----------------------------------------------------------------------
=begin
--- endNode()
	節点の登録を終了する。
=end
--------------------------------------------------------------------------*/
	void endNode(void);

/**----------------------------------------------------------------------
=begin
--- getNodeContainerType()
	節点コンテナの名前を返す。
=end
------------------------------------------------------------------------*/
	const char* getNodeContainerType(void);

/**------------------------------------------------------------------------
=begin
--- getNodeCount()
	MeshDBに登録されている節点の個数を返す。
	節点Idが不連続の時にはこの値と節点Idの最大値+1が一致しているとは限らない。
=end
--------------------------------------------------------------------------*/
	size_t getNodeCount(void);
/**------------------------------------------------------------------------
=begin
--- getNode(nodeId)
	節点Id が nodeId の節点の座標を [x,y,z] 形式の Array で返す。
=end
--------------------------------------------------------------------------*/
	%extend{
		VALUE getNode(kmb::nodeIdType nodeId){
			VALUE ary = rb_ary_new();
			kmb::Node node;
			if( self->getNode(nodeId,node) ){
				rb_ary_store(ary,0,rb_float_new(node.x()));
				rb_ary_store(ary,1,rb_float_new(node.y()));
				rb_ary_store(ary,2,rb_float_new(node.z()));
				return ary;
			}
			return Qnil;
		}
	}
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
=end

メモ：each 系のメソッドでは yield の前にイテレータを進めておく
--------------------------------------------------------------------------*/
%extend{
	VALUE eachNode(void) const{
		if( self->getNodes() != NULL ){
			double x,y,z;
			kmb::Point3DContainer::const_iterator nIter = self->getNodes()->begin();
			while( !nIter.isFinished() )
			{
				if( nIter.getXYZ(x,y,z) ){
					VALUE ary = rb_ary_new();
					rb_ary_store(ary,0,rb_float_new(x));
					rb_ary_store(ary,1,rb_float_new(y));
					rb_ary_store(ary,2,rb_float_new(z));
					++nIter;
					rb_yield(ary);
				}else{
					++nIter;
				}
			}
		}
		return Qnil;
	}
	VALUE eachNodeWithId(void) const{
		if( self->getNodes() != NULL ){
			double x,y,z;
			kmb::Point3DContainer::const_iterator nIter = self->getNodes()->begin();
			while( !nIter.isFinished() )
			{
				const kmb::nodeIdType id = nIter.getId();
				if( nIter.getXYZ(x,y,z) ){
					VALUE ary = rb_ary_new();
					rb_ary_store(ary,0,rb_float_new(x));
					rb_ary_store(ary,1,rb_float_new(y));
					rb_ary_store(ary,2,rb_float_new(z));
					++nIter;
					rb_yield_values(2,ary,INT2FIX(id));
				}else{
					++nIter;
				}
			}
		}
		return Qnil;
	}
}
/**----------------------------------------------------------------------
=begin
--- insertNode(x,y,z)
--- insertNodeWithId(x,y,z,id)
	insertNode メソッドでは endNode メソッドを読んだ後に
	座標 (x,y,z) を与えて、
	その座標の節点を登録して付与された節点IDを返す。
	通常は現在登録されている節点の最大節点ID+1が付与される。
	insertNodeWithId メソッドで節点IDをあらかじめ与えて登録することも出来る。
	この場合は既に登録されている場合は -1 を返す。
=end
-------------------------------------------------------------------------*/
	kmb::nodeIdType insertNode(double x,double y,double z);
	kmb::nodeIdType insertNodeWithId(double x,double y,double z,kmb::nodeIdType id);

/**----------------------------------------------------------------------
=begin
--- updateNode(x,y,z,id)
	すでに登録されている節点の座標を更新する
=end
-------------------------------------------------------------------------*/
	bool updateNode(double x,double y,double z, kmb::nodeIdType id);

/**--------------------------------------------------------------------
=begin
==要素管理

MeshData では要素は要素タイプと節点配列で管理する。
要素にはすべて要素Idが付与される。

MeshData で扱うことのできる要素タイプは以下のとおりである。
以下のメソッドで要素タイプを引数に与える場合はこれらをすべて大文字の文字列または getElementTypeMap で
得られる整数値を与える。それぞれの要素の節点配列のコネクティビティについては要素ライブラリの項で説明する。

* SEGMENT       (線分)
* SEGMENT2      (2次の線分)
* TRIANGLE      (三角形)
* TRIANGLE2     (2次の三角形)
* QUAD          (四角形)
* QUAD2         (2次の四角形)
* TETRAHEDRON   (四面体)
* TETRAHEDRON2  (2次の四面体)
* WEDGE         (三角柱(楔形、プリズム))
* WEDGE2        (2次の三角柱)
* PYRAMID       (四角柱(ピラミッド))
* PYRAMID2      (2次の四角柱)
* HEXAHEDRON    (六面体)
* HEXAHEDRON2   (2次の六面体)

要素の集まりは複数管理することができる。MeshDB では要素の集まり（要素グループ）のことを
Body と呼ぶ。以下の beginElement から endElement までの間に addElement で
登録された要素の集合が1つの Body になる。複数の Body を識別するために BodyID が
それぞれの Body に与えられている。BodyId は beginElement の戻り値で与えられる他、
または表面抽出や領域分割、境界層生成などで得られる。

--- beginElement(size=0)
	size 個の要素の登録を開始する。要素グループ(=Body)のIDを返す。
	id が連続でない場合は size には空き番号の個数も含めたものを指定する。
	containerType は目的に応じて要素コンテナを変更する場合に用いる。
	デフォルトでは要素コンテナとして節点IDをキーとした連想配列を用いる。

	containerType の例
	* コンテナに stl::map を使う（デフォルト） "stl::map"
	* ポインタの配列を使う "element_array"
	* OpenGL の glDrawElements で利用可能な三角形配列を使う "triangle_array"
=end
-----------------------------------------------------------------------*/
	kmb::bodyIdType beginElement(unsigned int size=0,const char* containerType=NULL);

/**---------------------------------------------------------------------
=begin
--- addElement(eType,ary)
--- addElementWithId(eType,ary,id)
	要素タイプが文字列 eType で、
	節点配列が Array 型の ary で与えられた要素を登録して
	付与された要素IDを返す。
	addElementWithId を用いれば要素 Id を指定して登録することができる。
	既にその Id が使われている場合は何もしない。
=end
------------------------------------------------------------------------*/
%extend{
	kmb::elementIdType addElement(VALUE eType,VALUE ary){
		kmb::elementType type = kmb::UNKNOWNTYPE;
		kmb::elementIdType elemId = kmb::Element::nullElementId;
		if( TYPE(eType) == T_STRING ){
			type = kmb::ElementBase::getType(StringValuePtr(eType));
		}else{
			type = static_cast<kmb::elementType>(FIX2INT(eType));
		}
		Check_Type(ary, T_ARRAY);
		const int len = kmb::Element::getNodeCount(type);
		if( len > 0 ){
			kmb::nodeIdType a[20];
			for(int i=0; i<len; ++i){
				a[i] = FIX2INT(rb_ary_entry(ary,i));
			}
			elemId = self->addElement(type,a);
		}
		return elemId;
	}
	kmb::elementIdType addElementWithId(VALUE eType,VALUE ary,VALUE id){
		kmb::elementType type = kmb::UNKNOWNTYPE;
		kmb::elementIdType elemId = kmb::Element::nullElementId;
		if( TYPE(eType) == T_STRING ){
			type = kmb::ElementBase::getType(StringValuePtr(eType));
		}else{
			type = static_cast<kmb::elementType>(FIX2INT(eType));
		}
		Check_Type(ary, T_ARRAY);
		const int len = kmb::Element::getNodeCount(type);
		if( len > 0 ){
			kmb::nodeIdType a[20];
			for(int i=0; i<len; ++i){
				a[i] = FIX2INT(rb_ary_entry(ary,i));
			}
			elemId = self->addElementWithId(type,a,FIX2INT(id));
		}
		return elemId;
	}
}

/**---------------------------------------------------------------------------
=begin
--- endElement(void)
    要素の登録を終了する。
=end
----------------------------------------------------------------------------*/
	void endElement(void);

/**--------------------------------------------------------------------------
=begin
--- insertElement(bodyId,eType,ary)
	endElement した後に bodyId の Body に
	addElement と同じ形式で要素を登録する。
=end
----------------------------------------------------------------------------*/
%extend{
	kmb::elementIdType insertElement(kmb::bodyIdType bodyId,VALUE eType,VALUE ary){
		kmb::elementType type = kmb::UNKNOWNTYPE;
		if( TYPE(eType) == T_STRING ){
			type = kmb::ElementBase::getType(StringValuePtr(eType));
		}else{
			type = static_cast<kmb::elementType>(FIX2INT(eType));
		}
		Check_Type(ary, T_ARRAY);
		const int len = kmb::Element::getNodeCount(type);
		if( len > 0 ){
			kmb::nodeIdType a[20];
			for(int i=0; i<len; ++i){
				a[i] = FIX2INT(rb_ary_entry(ary,i));
			}
			return self->insertElement(bodyId,type,a);
		}
		return kmb::Element::nullElementId;
	}
}

/**--------------------------------------------------------------------------
=begin
--- insertElementWithId(bodyId,eType,ary,id)
	endElement した後に bodyId の Body に
	addElement と同じ形式で要素を登録する。
=end
----------------------------------------------------------------------------*/
%extend{
	kmb::elementIdType insertElementWithId(kmb::bodyIdType bodyId,VALUE eType,VALUE ary,kmb::elementIdType elementId){
		kmb::elementType type = kmb::UNKNOWNTYPE;
		if( TYPE(eType) == T_STRING ){
			type = kmb::ElementBase::getType(StringValuePtr(eType));
		}else{
			type = static_cast<kmb::elementType>(FIX2INT(eType));
		}
		Check_Type(ary, T_ARRAY);
		const int len = kmb::Element::getNodeCount(type);
		if( len > 0 ){
			kmb::nodeIdType a[20];
			for(int i=0; i<len; ++i){
				a[i] = FIX2INT(rb_ary_entry(ary,i));
			}
			return self->insertElementWithId(bodyId,type,a,elementId);
		}
		return kmb::Element::nullElementId;
	}
}

/**--------------------------------------------------------------------------
=begin
--- getElementCount(bodyId)
	与えられた Body に含まれる要素の個数を返す。
--- getElementCount()
	すべての Body の要素の個数の合計を返す。
--- getElementCountByType(bodyId)
	与えられた Body に含まれる要素の個数を型別の Hash で返す。
=end
---------------------------------------------------------------------------*/
	int getElementCount(kmb::bodyIdType bodyId) const;
	int getElementCount(void) const;
%extend{
	VALUE getElementCountByType(kmb::bodyIdType bodyId){
		VALUE hash = rb_hash_new();
		kmb::ElementContainer* body = self->getBodyPtr( bodyId );
		if( body != NULL ){
			for(int i = 0; i < kmb::ELEMENT_TYPE_NUM; ++i)
			{
				kmb::elementType type = static_cast<kmb::elementType>(i);
				std::string	str = kmb::ElementBase::getTypeString(type);
				int count = static_cast<int>( body->getCountByType( type ) );
				if( count > 0 ){
					rb_hash_aset( hash, rb_str_new2( str.c_str() ), INT2NUM(count) );
				}
			}
		}
		return hash;
	}
}

/**--------------------------------------------------------------------------
=begin
--- getDimension(bodyId)
--- getDimension()
	要素グループの次元（表面メッシュなら2、体積メッシュなら3）
=end
---------------------------------------------------------------------------*/
	int getDimension(kmb::bodyIdType bodyId) const;
	int getDimension(void) const;

/**--------------------------------------------------------------------------
=begin
--- getDegree(bodyId)
	要素グループの次元（2次要素なら2）
=end
---------------------------------------------------------------------------*/
	int getDegree(kmb::bodyIdType bodyId) const;

/**--------------------------------------------------------------------------
=begin
--- isUniqueElementType(bodyId,etype)
	要素の型を表す文字列を入れて、要素グループがその要素の型のみから
	なるかどうかを判断する。
=end
---------------------------------------------------------------------------*/
%extend{
	bool isUniqueElementType(kmb::bodyIdType bodyId,const char* etype) const{
		return self->isUniqueElementType( bodyId, kmb::ElementBase::getType(etype) );
	}
}

/**--------------------------------------------------------------------------
=begin
--- getBodyName(bodyId)
	要素グループの名前の取得
--- setBodyName(bodyId,name)
	要素グループの名前の設定
--- getBodyIdByName(name)
	要素グループ名から id を求める
	なければ -a
=end
---------------------------------------------------------------------------*/
	const char* getBodyName(bodyIdType bodyId) const;
	void setBodyName(bodyIdType bodyId,const char* name);
	kmb::bodyIdType getBodyIdByName(const char* name) const;

/**--------------------------------------------------------------------------
=begin
--- getElementContainerType(bodyId)
	要素グループのコンテナタイプ名
=end
---------------------------------------------------------------------------*/
	const char* getElementContainerType(bodyIdType bodyId) const;

/**-------------------------------------------------------------------------
=begin
--- getElement(elementId,bodyId=-1)
	bodyId の Body に含まれる要素ID elementId
	の要素を取り出す。戻り値は Array で第一成分に
	型の文字列、第2成分以下に要素の頂点配列が追加されている。
	bodyId を与えなくても取り出すことはできるが、検索のため余分な時間がかかる。

	戻り値の例：
	["TETRAHEDRON",100,103,106,110]
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getElement(kmb::elementIdType elementId,kmb::bodyIdType bodyId=kmb::Body::nullBodyId){
		kmb::ElementContainer::const_iterator element = self->findElement(elementId,bodyId);
		if( !element.isFinished() ){
			VALUE ary = rb_ary_new();
			rb_ary_store(ary,0,rb_str_new2(element.getTypeString().c_str()));
			const int len = element.getNodeCount();
			for(int i=0; i<len; ++i){
				rb_ary_store(ary,i+1,INT2FIX(element.getCellId(i)));
			}
			return ary;
		}
		return Qnil;
	}
}

/**---------------------------------------------------------------------------
=begin
--- eachElement(bodyID)
	bodyID の Body の要素のイテレータを与える。
	イテレータの内部のブロックでは、要素が getElement と同じ形式で得られる。

--- eachElementWithId(bodyID)
	bodyID の Body の要素と要素IDのイテレータを与える。

=end
-----------------------------------------------------------------------------*/
%extend{
	VALUE eachElement(kmb::bodyIdType bodyID){
		const kmb::ElementContainer* body = self->getBodyPtr(bodyID);
		if( body ){
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( eIter != body->end() )
			{
				VALUE ary = rb_ary_new();
				rb_ary_store(ary,0,rb_str_new2(eIter.getTypeString().c_str()));
				const int len = eIter.getNodeCount();
				for(int i=0; i<len; ++i){
					rb_ary_store(ary,i+1,INT2FIX(eIter.getCellId(i)));
				}
				++eIter;
				rb_yield(ary);
			}
		}
		return Qnil;
	}
	VALUE eachElementWithId(kmb::bodyIdType bodyID){
		const kmb::ElementContainer* body = self->getBodyPtr(bodyID);
		if( body ){
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( eIter != body->end() )
			{
				VALUE ary = rb_ary_new();
				kmb::elementIdType elementID = eIter.getId();
				rb_ary_store(ary,0,rb_str_new2(eIter.getTypeString().c_str()));
				const int len = eIter.getNodeCount();
				for(int i=0; i<len; ++i){
					rb_ary_store(ary,i+1,INT2FIX(eIter.getCellId(i)));
				}
				++eIter;
				rb_yield_values(2,ary,INT2FIX(elementID));
			}
		}
		return Qnil;
	}
}

/**---------------------------------------------------------------------
=begin
--- getBodyCount()
	登録されている Body の個数を返す。
=end
------------------------------------------------------------------------*/
	kmb::bodyIdType getBodyCount(void) const;

/**-------------------------------------------------------------------------
=begin
--- clearBody(bodyId)
	bodyId の Body に含まれている要素をすべて削除する。
	要素を削除しても近傍要素のキャッシュまではクリアしない。
=end
---------------------------------------------------------------------------*/
	void clearBody(kmb::bodyIdType bodyId);


/**--------------------------------------------------------------------------
=begin
==データ管理

メッシュに付随するプリ処理またはポスト処理のためのデータの管理を行う。
データの種類はモードと型で分類される。
ソルバ－が識別するためのタグも指定できる。

モードは以下の文字列で与える
: "NODEGROUP"
  節点グループ（節点IDの集合を定義し、物理量は節点グループ全体に与える）
: "ELEMENTGROUP"
  要素グループ（要素IDの集合を定義し、物理量は要素グループ全体に与える）
: "FACEGROUP"
  要素の表面グループ（FaceIDの集合を定義し、物理量は要素表面グループ全体に与える）
: "BODYGROUP"
  ボディグループ（BodyIDの集合を定義し、物理量はボディグループ全体に与える）
: "NODEVARIABLE"
  節点上の物理量を定義する（節点IDと物理量のペアを登録する）
: "ELEMENTVARIABLE"
  要素上の物理量を定義する（要素IDと物理量のペアを登録する）
: "FACEVARIABLE"
  要素の面上に物理量を定義する（FaceIDと物理量のペアを登録する）
: "BODYVARIABLE"
  領域ごとに物理量を定義する（BodyIDと物理量のペアを登録する）
: "GLOBAL"
  グループに ID を登録せずに、グローバルな物理量を設定するためのモード
: "UNKNWON"
  上記以外のモード（論理的にこのモードになることは無い。デバッグ用。）

物理量の型は以下の文字列で与える
: "INTEGER"
  整数値
: "SCALAR"
  スカラー値
: "VECTOR2"
  2次元のベクトル値
: "VECTOR3"
  3次元のベクトル値
: "VECTOR4"
  4次元のベクトル値
: "POINT3VECTOR3"
  3次元の始点とベクトルの組 [[x,y,z],[vx,vy,vz]] の形式
: "TENSOR6"
  自由度6のテンソル値（3次元の対称テンソル）6次元の配列で与える
: "STRING"
  文字列
: "ARRAY"
  物理量の配列
: "HASH"
  文字列をキーとし、物理量を値とする連想配列
: "NONE"
  物理量なし

例えば BodyID = 1,2,3 のボディの全体に物理量 "pressure" のスカラー値 10.0 を与えるには

 mesh.createData("pressure","BODYGROUP","SCALAR")
 mesh.addId("pressure",1)
 mesh.addId("pressure",2)
 mesh.addId("pressure",3)
 mesh.setValue("pressure",10.0)

のようにする。

IDと物理量のペアを与える場合は、addId と setValue の代わりに
setValueAtId を使う。

 mesh.createData("displace","NODEVARIABLE","VECTOR3")
 mesh.setValueAtId("displace", 0, [0.0, 0.1, 0.2])
 mesh.setValueAtId("displace", 1, [0.2, 0.1, 0.1])
 mesh.setValueAtId("displace", 2, [0.4, 0.0, 0.0])
 mesh.setValueAtId("displace", 3, [0.6, 0.0, 0.1])

または、複数の物理量を同時に与えたい時には

 mesh.createData("displace","NODEVARIABLE","VECTOR3")
 mesh.createData("mises","NODEVARIABLE","SCALAR")
 mesh.setTargetData(["displace","mises"])
 mesh.setMultiValuesAtId(0, [0.0, 0.1, 0.2, 0.01])
 mesh.setMultiValuesAtId(1, [0.2, 0.1, 0.1, 0.02])
 mesh.setMultiValuesAtId(2, [0.4, 0.0, 0.0, 0.03])
 mesh.setMultiValuesAtId(3, [0.6, 0.0, 0.1, 0.04])

のようにする。

Globalに物理量を与える時は

 mesh.createData("absTemp","GLOBAL","SCALAR")
 mesh.setValue("absTemp",-273.15)

のようにする。

mode を GROUP 系で指定した時は setValueAtId は何もしない。
mode を VARIABLE 系で指定した時は addId および setValue は何もしない。
mode を Global に指定した時は addId および setValueAtId は何もしない。

FaceGroup と FaceVariable は要素 ID とその要素の FaceID の組に対して、
物理量を設定するためのモードである。
例えば "wall" という FaceGroup に格納されている FACE を取得するには
次のようにする。

 mesh.eachId( "wall" ){ |faceId|
   # faceId = [elementId,localId]  ex, [1001,2] のような形式
   element = mesh.faceToElement(faceId)
   # element = ['TRIANGLE',0,1,2] のような形式
 }

stype は解析コードが境界条件の分類をする場合などのために、
データに与えることのできる文字列のタグである。この文字列をキーとして
Name( stype ) をすると、与えられた stype に一致する
データの名前だけのイテレータが得られる。

 mesh.createData("Const_1","NODEGROUP","ARRAY","Constraint")
 mesh.createData("wall","FACEGROUP","SCALAR","Boundary")

などのようにして使う。

stype が異なる場合は、名前が重複していてもよい。

--- createData(name,bmode,vtype,stype,bodyId)
	モードが bmode で物理量の型が vtype で与えられるデータ name を生成する。
	stype bodyId は省略可能
	すでに存在している場合は何もしない

=end
---------------------------------------------------------------------------*/

%extend{
	VALUE createData(const char* name,const char* bmode,const char* vtype,const char* stype="",int bodyId=kmb::Body::nullBodyId){
		kmb::DataBindings::bindingMode
			m = kmb::DataBindings::string2bindingMode(bmode);
		kmb::PhysicalValue::valueType
			v = kmb::PhysicalValue::string2valueType(vtype);
		self->createDataBindings(name,m,v,stype,bodyId);
		return Qnil;
	}
}

/**--------------------------------------------------------------------------
=begin
--- getDataCount()
--- getDataCount(specType)
	登録されているデータの名前の個数を返す。specType を与えた時はその specType に登録されている
	データの名前の個数を返す。
=end
----------------------------------------------------------------------------*/
	size_t getDataCount(const char* stype=NULL);

/**--------------------------------------------------------------------------
=begin
--- hasData(name,stype=nil)
	データ name で spec type が stype のものがデータベースに登録されているかどうか検索する。
	stype=nil の場合は name だけで検索する。
=end
----------------------------------------------------------------------------*/
	bool hasData(const char* name,const char* stype=NULL);

/**--------------------------------------------------------------------------
=begin
--- renameData(oldname,newname,stype=nil)
	データ oldname を newname に名称変更する。
	oldname が登録されていて、newname が登録されていない時だけ実行される
	戻り値は true / false
=end
----------------------------------------------------------------------------*/
	bool renameData(const char* oldname,const char* newname,const char* stype=NULL);

/**--------------------------------------------------------------------------
=begin
--- deleteData(name,stype=nil)
	データ name をデータベースから削除する
=end
----------------------------------------------------------------------------*/
	bool deleteData(const char* name,const char* stype=NULL);

/**--------------------------------------------------------------------------
=begin
--- clearData(name,stype=nil)
	データ name のデータベースを空にする。
	データベースのコンテナを削除するわけではない。
=end
----------------------------------------------------------------------------*/
	bool clearData(const char* name,const char* stype=NULL);

/**--------------------------------------------------------------------------
=begin
--- getDataMode(name,stype=nil);
	データ name のモードを文字列で取得する
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE getDataMode(const char* name,const char* stype=NULL){
		std::string str = kmb::DataBindings::bindingMode2string( self->getDataMode( name, stype ) );
		return rb_str_new2( str.c_str() );
	}
}

/**--------------------------------------------------------------------------
=begin
--- getDataValueType(name,stype=nil)
	データ name の物理量の型を文字列で取得する
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE getDataValueType(const char* name,const char* stype=NULL){
		std::string str = kmb::PhysicalValue::valueType2string( self->getDataValueType( name, stype ) );
		return rb_str_new2( str.c_str() );
	}
}

/**--------------------------------------------------------------------------
=begin
--- getDataSpecType(name)
	データ name の物理量の種類を文字列で取得する
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE getDataSpecType(const char* name){
		std::string str = self->getDataSpecType( name );
		return rb_str_new2( str.c_str() );
	}
}

/**--------------------------------------------------------------------------
=begin
--- setDataSpecType(name,oldstype,newstype)
	Spec Type を設定する
=end
----------------------------------------------------------------------------*/
	bool setDataSpecType(const char* name,const char* oldstype,const char* newstype);

/**--------------------------------------------------------------------------
=begin
--- getDataContainerType(name,stype)
	コンテナタイプを取得する
=end
----------------------------------------------------------------------------*/
	const char* getDataContainerType(const char* name,const char* stype=NULL) const;

/**--------------------------------------------------------------------------
=begin
--- getDataTargetBodyId(name,stype=nil)
	データ name の対象とする BodyId を返す
=end
----------------------------------------------------------------------------*/
%extend{
	kmb::bodyIdType getDataTargetBodyId(const char* name,const char* stype=NULL){
		kmb::DataBindings* data = self->getDataBindingsPtr(name);
		if( data != NULL ){
			return data->getTargetBodyId();
		}else{
			return kmb::Body::nullBodyId;
		}
	}
}

/**------------------------------------------------------------------------
=begin
--- eachDataName
--- eachDataName(specType)
	登録されている Data (物理量) の名前を出力する iterator を与える。
=end
-------------------------------------------------------------------------*/
%extend{
	void eachDataName(void){
		const std::multimap< std::string, kmb::DataBindings* > mapper = self->getDataBindingsMap();
		std::multimap< std::string, kmb::DataBindings*>::const_iterator dIter = mapper.begin();
		while( dIter != mapper.end() ){
			std::string name = dIter->first;
			++dIter;
			rb_yield( rb_str_new2( name.c_str() ) );
		}
	}
	void eachDataName(const char* stype){
		const std::multimap< std::string, kmb::DataBindings* > mapper = self->getDataBindingsMap();
		std::multimap< std::string, kmb::DataBindings*>::const_iterator dIter = mapper.begin();
		while( dIter != mapper.end() ){
			kmb::DataBindings* binding = dIter->second;
			if( binding != NULL &&
				binding->getSpecType().compare(stype) == 0 )
			{
				std::string name = dIter->first;
				++dIter;
				rb_yield( rb_str_new2( name.c_str() ) );
			}else{
				++dIter;
			}
		}
	}
}

/**-------------------------------------------------------------------------
=begin
--- getIdCount(name,stype)
	name のグループに登録された id の個数を出力する
	NodeGroup / NodeVariable なら頂点の個数、
	ElementGroup / ElementVariable なら要素の個数
	BodyGroup / BodyVariable なら立体の個数
	FaceGroup / FaceVariable なら FACE の個数
	それ以外は 0 を返す
=end
---------------------------------------------------------------------------*/
	int getIdCount(const char* name,const char* stype=NULL);

/**--------------------------------------------------------------------------
=begin
--- setDefaultSpecType(specType)
	以下のメソッド群での specType の引数が nil でありかつ、
	このメソッドで default 値が設定されているときには、ここで与えられた specType の中から探す。
=end
----------------------------------------------------------------------------*/
	void setDefaultSpecType(const char* specType);

/**---------------------------------------------------------------------------
=begin
--- getValueAtId(name,id,stype=nil)
	グループ name の id に定義された物理量を取得する。XXXVARIABLE モードの時だけ有効。
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE getValueAtId(const char* name,VALUE id,const char* stype=NULL){
		kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
		if( data ){
			switch( data->getValueType() )
			{
			case kmb::PhysicalValue::Scalar:
				{
					double v = 0.0;
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						if( data->getPhysicalValue(kmb::Face(id0,id1),&v) ){
							return rb_float_new(v);
						}
					}else{
						if( data->getPhysicalValue(FIX2INT(id),&v) ){
							return rb_float_new(v);
						}
					}
					break;
				}
			case kmb::PhysicalValue::Integer:
				{
					long l;
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						if( data->getPhysicalValue(kmb::Face(id0,id1),&l) ){
							return INT2FIX(l);
						}
					}else{
						if( data->getPhysicalValue(FIX2INT(id),&l) ){
							return INT2FIX(l);
						}
					}
					break;
				}
			case kmb::PhysicalValue::Vector2:
				{
					double v[2] = {0.0, 0.0};
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						if( data->getPhysicalValue(kmb::Face(id0,id1),v) ){
							return toVALUE( v[0], v[1] );
						}
					}else{
						if( data->getPhysicalValue(FIX2INT(id),v) ){
							return toVALUE( v[0], v[1] );
						}
					}
					break;
				}
			case kmb::PhysicalValue::Vector3:
				{
					double v[3] = {0.0, 0.0, 0.0};
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						if( data->getPhysicalValue(kmb::Face(id0,id1),v) ){
							return toVALUE( v[0], v[1], v[2] );
						}
					}else{
						if( data->getPhysicalValue(FIX2INT(id),v) ){
							return toVALUE( v[0], v[1], v[2] );
						}
					}
					break;
				}
			case kmb::PhysicalValue::Vector4:
				{
					double v[4] = {0.0, 0.0, 0.0, 0.0};
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						if( data->getPhysicalValue(kmb::Face(id0,id1),v) ){
							return toVALUE( v[0], v[1], v[2], v[3] );
						}
					}else{
						if( data->getPhysicalValue(FIX2INT(id),v) ){
							return toVALUE( v[0], v[1], v[2], v[3] );
						}
					}
					break;
				}
			case kmb::PhysicalValue::Vector2withInt:
				{
					double v[2] = {0.0, 0.0};
					long ival = 0;
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						kmb::Face f(id0,id1);
						if( data->getPhysicalValue(f,v) && data->getPhysicalValue(f,&ival) ){
							return toVALUE( v[0], v[1], ival );
						}
					}else{
						kmb::idType i = FIX2INT(id);
						if( data->getPhysicalValue(i,v) && data->getPhysicalValue(i,&ival) ){
							return toVALUE( v[0], v[1], ival );
						}
					}
					break;
				}
			default:
				{
					kmb::PhysicalValue* val = NULL;
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						val = data->getPhysicalValue(kmb::Face(id0,id1));
					}else{
						val = data->getPhysicalValue(FIX2INT(id));
					}
					if( val ){
						return toVALUE( val );
					}
					break;
				}
			}
		}
		return Qnil;
	}
}

/**--------------------------------------------------------------------------
=begin
--- setValueAtId(name,id,v,stype=nil)
	グループ name の id に物理量 v を与える。XXXVARIABLE モードの時だけ有効。
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE setValueAtId(const char* name,VALUE id,VALUE v,const char* stype=NULL){
		kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
		kmb::PhysicalValue* val = createFromVALUE(v);
		if( val != NULL && data != NULL ){
			if( val->getType() == data->getValueType() ){
				if( TYPE(id) == T_ARRAY ){
					int id0 = NUM2INT(rb_ary_entry(id,0));
					int id1 = NUM2INT(rb_ary_entry(id,1));
					data->setPhysicalValue(kmb::Face(id0,id1),val);
				}else{
					data->setPhysicalValue(FIX2INT(id),val);
				}
			}else{
				delete val;
			}
		}
		return Qnil;
	}
}

/**---------------------------------------------------------------------------
=begin
--- getValue(name,stype=nil)
	グループ name 全体に定義された物理量を取得する。XXXGROUP モード、Global モードの時だけ有効。
=end
----------------------------------------------------------------------------*/
	%extend{
		VALUE getValue(const char* name,const char* stype=NULL){
			kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
			if( data != NULL ){
				kmb::PhysicalValue* val = data->getPhysicalValue();
				if( val != NULL ){
					return toVALUE( val );
				}
			}
			return Qnil;
		}
	}

/**-------------------------------------------------------------------------
=begin
--- setValue(name,v,stype=nil)
	グループ name 全体に物理量 v を定義する。XXXGROUP モードまたは Global モードの時だけ有効。
=end
---------------------------------------------------------------------------*/
	%extend{
		VALUE setValue(const char* name,VALUE v,const char* stype=NULL){
			kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
			if( data != NULL ){
				kmb::PhysicalValue* val =
					(data->getValueType() == kmb::PhysicalValue::Array) ?
					createArrayFromVALUE(v) : createFromVALUE(v);
				if( val != NULL ){
					if( val->getType() == data->getValueType() ){
						data->setPhysicalValue(val);
					}else{
						delete val;
					}
				}
			}
			return Qnil;
		}
	}

/**--------------------------------------------------------------------------
=begin
--- addId(name,id,stype=nil)
	name のデータに id を追加する。
=end
---------------------------------------------------------------------------*/
	%extend{
		VALUE addId(const char* name,VALUE id,const char* stype=NULL){
			kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
			if( data != NULL ){
				if( TYPE(id) == T_ARRAY ){
					int id0 = NUM2INT(rb_ary_entry(id,0));
					int id1 = NUM2INT(rb_ary_entry(id,1));
					data->addId(kmb::Face(id0,id1));
				}else{
					data->addId(FIX2INT(id));
				}
			}
			return Qnil;
		}
	}

/**--------------------------------------------------------------------------
=begin
--- hasId(name,id,stype=nil)
	name のデータが id を含むかどうかを返す
=end
---------------------------------------------------------------------------*/
	%extend{
		bool hasId(const char* name,VALUE id,const char* stype=NULL){
			kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
			if( data != NULL ){
				if( TYPE(id) == T_ARRAY ){
					int id0 = NUM2INT(rb_ary_entry(id,0));
					int id1 = NUM2INT(rb_ary_entry(id,1));
					return data->hasId(kmb::Face(id0,id1));
				}else{
					return data->hasId(FIX2INT(id));
				}
			}
			return false;
		}
	}

/**--------------------------------------------------------------------------
=begin
--- deleteId(name,id,stype=nil)
	name のデータから id を削除する
=end
---------------------------------------------------------------------------*/
	%extend{
		bool deleteId(const char* name,VALUE id,const char* stype=NULL){
			kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
			if( data != NULL ){
				if( TYPE(id) == T_ARRAY ){
					int id0 = NUM2INT(rb_ary_entry(id,0));
					int id1 = NUM2INT(rb_ary_entry(id,1));
					return data->deleteId(kmb::Face(id0,id1));
				}else{
					return data->deleteId(FIX2INT(id));
				}
			}
			return false;
		}
	}

/**--------------------------------------------------------------------------
=begin
--- setTargetData(names,stype=nil)
	setMultiValuesAtId で複数の物理量を一度に与えるための
	対象を設定する
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE setTargetData(VALUE names,const char* stype=NULL){
		Check_Type(names, T_ARRAY);
		const int len = RARRAY_LEN(names);
		self->clearTargetData();
		for(int i=0;i<len;++i)
		{
			VALUE item = rb_ary_entry(names,i);
			if(TYPE(item) == T_STRING)
			{
				self->appendTargetData( StringValueCStr(item), stype );
			}
		}
		return Qnil;
	}
}
/**--------------------------------------------------------------------------
=begin
--- setMultiValuesAtId(id,v)
	setTargetData で対象が与えられている時に
	グループ name の id に物理量の配列 v を与える。NodeVariable or ElementVariable モードの時だけ有効。
	v は setTargetData で登録された物理量に与える値を順に並べた 1 次元配列。
	例：displace, stress, mises が 3, 6, 1 次元で登録されていた場合は
	3 + 6 + 1 = 10 次元の 1 次元配列を与える。
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE setMultiValuesAtId(VALUE id,VALUE v){
		Check_Type(v, T_ARRAY);
		const int len = RARRAY_LEN(v);
		if( len > 0 ){
			VALUE first = rb_ary_entry(v,0);
			if( TYPE(first) == T_FIXNUM ){
				long* values = NULL;
				values = new long[len];
				for(int i=0;i<len;++i)
				{
					VALUE item = rb_ary_entry(v,i);
					values[i] = FIX2INT(item);
				}
				self->setMultiPhysicalValues( FIX2INT(id), values );
				delete[] values;
			}else if( TYPE(first) == T_FLOAT ){
				double* values = NULL;
				values = new double[len];
				for(int i=0;i<len;++i)
				{
					VALUE item = rb_ary_entry(v,i);
					values[i] = NUM2DBL(item);
				}
				self->setMultiPhysicalValues( FIX2INT(id), values );
				delete[] values;
			}
		}
		return Qnil;
	}
}
/**-------------------------------------------------------------------------
=begin
--- eachId(name,stype=nil)
	name グループの Id を出力する iterator
=end
---------------------------------------------------------------------------*/
%extend{
	void eachId(const char* name,const char* stype=NULL){
		kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
		if( data ){
			switch( data->getBindingMode() ){
				case kmb::DataBindings::NodeGroup:
				case kmb::DataBindings::ElementGroup:
				case kmb::DataBindings::BodyGroup:
				case kmb::DataBindings::NodeVariable:
				case kmb::DataBindings::ElementVariable:
				case kmb::DataBindings::BodyVariable:
				{
					kmb::DataBindings::iterator dIter = data->begin();
					kmb::DataBindings::iterator dEnd = data->end();
					while( dIter != dEnd ){
						rb_yield( INT2FIX( dIter.getId() ) );
						++dIter;
					}
					break;
				}
				case kmb::DataBindings::FaceGroup:
				case kmb::DataBindings::FaceVariable:
				{
					kmb::Face f;
					kmb::DataBindings::iterator dIter = data->begin();
					kmb::DataBindings::iterator dEnd = data->end();
					while( dIter != dEnd ){
						if( dIter.getFace( f ) ){
							rb_yield( toVALUE(f) );
						}
						++dIter;
					}
					break;
				}
				case kmb::DataBindings::Global:
				default:
					break;
			}
		}
	}
}

/**------------------------------------------------------------------------
=begin
--- eachIdWithValue(name,stype)
	name グループの Id と物理量の組を出力する iterator
	値の型が Scalar の時は value は値そのもの、
	Vector3 または Tensor6 の場合は、配列で与えられる。
	以下のようにして使う。

	 mesh.eachIdWithValue("weight"){ |id,val|
	  puts "id = #{id} x = #{val[0]} y = #{val[1]} z = #{val[2]}"
	 }

=end
-------------------------------------------------------------------------*/
%extend{
	void eachIdWithValue(const char* name,const char* stype=NULL){
		kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
		if( data ){
			switch( data->getBindingMode() ){
				case kmb::DataBindings::NodeVariable:
				case kmb::DataBindings::ElementVariable:
				case kmb::DataBindings::BodyVariable:
				{
					kmb::DataBindings::iterator dIter = data->begin();
					switch( data->getValueType() ){
						case kmb::PhysicalValue::Scalar:
						{
							double v = 0.0;
							while( !dIter.isFinished() ){
								if( dIter.getValue(&v) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), rb_float_new(v) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Integer:
						{
							long l = 0;
							while( !dIter.isFinished() ){
								if( dIter.getValue(&l) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), INT2FIX(l) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector2:
						{
							double v[2] = {0.0,0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector3:
						{
							double v[3] = {0.0,0.0,0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1],v[2]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector4:
						{
							double v[4] = {0.0,0.0,0.0,0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1],v[2],v[3]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Tensor6:
						{
							double v[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1],v[2],v[3],v[4],v[5]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector2withInt:
						{
							double v[2] = {0.0,0.0};
							long iVal = 0L;
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) && dIter.getValue(&iVal) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1],iVal) );
								}
								++dIter;
							}
							break;
						}
						default:
						{
							while( !dIter.isFinished() ){
								kmb::PhysicalValue* val = dIter.getValue();
								if( val ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE( val ) );
								}
								++dIter;
							}
							break;
						}
					}
				}
				break;
				case kmb::DataBindings::FaceVariable:
				{
					kmb::Face f;
					kmb::DataBindings::iterator dIter = data->begin();
					switch( data->getValueType() ){
						case kmb::PhysicalValue::Scalar:
						{
							double v=0.0;
							while( !dIter.isFinished() ){
								if( dIter.getValue(&v) && dIter.getFace( f ) ){
									rb_yield_values( 2, toVALUE( f ), rb_float_new(v) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Integer:
						{
							long l=0;
							while( !dIter.isFinished() ){
								if( dIter.getValue(&l) && dIter.getFace( f ) ){
									rb_yield_values( 2, toVALUE( f ), INT2FIX(l) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector2:
						{
							double v[2]={0.0,0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) && dIter.getFace( f ) ){
									rb_yield_values( 2, toVALUE( f ), toVALUE(v[0],v[1]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector3:
						{
							double v[3]={0.0,0.0,0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) && dIter.getFace( f ) ){
									rb_yield_values( 2, toVALUE( f ), toVALUE(v[0],v[1],v[2]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector4:
						{
							double v[4] = {0.0,0.0,0.0,0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1],v[2],v[3]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector2withInt:
						{
							double v[2] = {0.0,0.0};
							long iVal = 0L;
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) && dIter.getValue(&iVal) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1],iVal) );
								}
								++dIter;
							}
							break;
						}
						default:
						{
							while( !dIter.isFinished() ){
								kmb::PhysicalValue* val = dIter.getValue();
								if( val && dIter.getFace( f ) ){
									rb_yield_values( 2, toVALUE( f ) , toVALUE( val ) );
								}
								++dIter;
							}
							break;
						}
					}
				}
				break;
				default:
					break;
			}
		}
	}
}

/**--------------------------------------------------------------------------
=begin
--- faceGroupToBody(name,stype=nil)
	FaceGroup のデータを body に変換する。
=end
----------------------------------------------------------------------------*/
	kmb::bodyIdType faceGroupToBody(const char* name,const char* stype=NULL);
};

}
