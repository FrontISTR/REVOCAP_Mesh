/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : refiner                                                 #
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
=RevocapMeshGen::MeshRefiner

==概要

モデルの細分を行うためのクラス。
REVOCAP_Refiner の機能の一部を PrePost から利用できる。

==メソッド一覧

((<MeshRefiner.new>))

==メソッド

=end
------------------------------------------------------------------------------*/
%{
#include "MeshDB/kmbElement.h"
#include "MeshGen/kmbMeshRefiner.h"
%}

namespace kmb{

class MeshData;
class DataBindings;

class MeshRefiner
{
public:
/**--------------------------------------------------------------------------
=begin
--- MeshRefiner.new
	メッシュ要素細分モジュール MeshRefiner のインスタンスを生成する。
=end
---------------------------------------------------------------------------*/
	MeshRefiner(void);
	virtual ~MeshRefiner(void);
/**--------------------------------------------------------------------------
=begin
--- clear
	MeshRefiner の内部中間データをクリアする
=end
---------------------------------------------------------------------------*/
	void clear(void);
/**--------------------------------------------------------------------------
=begin
--- setMesh(mesh)
	メッシュ要素細分の対象となるメッシュを登録する
=end
---------------------------------------------------------------------------*/
	void setMesh(kmb::MeshData* mesh);
/**--------------------------------------------------------------------------
=begin
--- setSecondNodesByData(name,stype=nil)
	既に2次要素となっている節点を ElementGroup から登録する
=end
---------------------------------------------------------------------------*/
	void setSecondNodesByData(const char* name,const char* stype=NULL);
/**--------------------------------------------------------------------------
=begin
--- setSecondNodesByBody(bodyId)
	既に2次要素となっている節点を Body から登録する
=end
---------------------------------------------------------------------------*/
	void setSecondNodesByBody(kmb::bodyIdType bodyId);
/**--------------------------------------------------------------------------
=begin
--- convertToSecondBody(bodyId)
	Body に含まれる要素を2次要素に変換する。
=end
---------------------------------------------------------------------------*/
	kmb::bodyIdType convertToSecondBody(kmb::bodyIdType bodyId);
/**--------------------------------------------------------------------------
=begin
--- refineBody(bodyId)
	Body に含まれる要素を細分する。
=end
---------------------------------------------------------------------------*/
	kmb::bodyIdType refineBody(kmb::bodyIdType orgBodyId);
/**--------------------------------------------------------------------------
=begin
--- getOriginal(middleNodeId)
	中間節点を与えて、それがどの要素（辺）から生成されたかを返す
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getOriginal(kmb::nodeIdType middleNodeId){
		kmb::nodeIdType originalNodes[20];
		kmb::elementType etype = self->getOriginal(middleNodeId,originalNodes);
		if( etype == kmb::UNKNOWNTYPE ){
			return Qnil;
		}
		int num = kmb::Element::getNodeCount(etype);
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,rb_str_new2(kmb::Element::getTypeString(etype).c_str()));
		for(int i=0;i<num;++i){
			rb_ary_push(ary,INT2FIX(originalNodes[i]));
		}
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
--- getMiddle(etype,nodes)
	辺または要素を与えて、そこから生成された中間節点を返す
=end
---------------------------------------------------------------------------*/
%extend{
	kmb::nodeIdType getMiddle(const char* etype, VALUE ary ){
		kmb::elementType elemType = kmb::ElementBase::getType( etype );
		kmb::Element* elem = kmb::Element::create( elemType );
		int num = kmb::Element::getNodeCount(elemType);
		for(int i=0;i<num;++i){
			elem->setCellId( i, FIX2INT( rb_ary_entry(ary,i) ) );
		}
		kmb::nodeIdType middle = self->getMiddle( *elem );
		delete elem;
		return middle;
	}
}

/**--------------------------------------------------------------------------
=begin
--- appendData(name,stype=nil)
	refineBody するときに同時に更新するデータを登録する
=end
---------------------------------------------------------------------------*/
	void appendData(const char* dataname,const char* stype=NULL);
/**--------------------------------------------------------------------------
=begin
--- clearData()
	refineBody するときに同時に更新するデータをクリアする
=end
---------------------------------------------------------------------------*/
	void clearData(void);
};

}
