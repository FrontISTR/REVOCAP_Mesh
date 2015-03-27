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

==�T�v

���f���̍ו����s�����߂̃N���X�B
REVOCAP_Refiner �̋@�\�̈ꕔ�� PrePost ���痘�p�ł���B

==���\�b�h�ꗗ

((<MeshRefiner.new>))

==���\�b�h

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
	���b�V���v�f�ו����W���[�� MeshRefiner �̃C���X�^���X�𐶐�����B
=end
---------------------------------------------------------------------------*/
	MeshRefiner(void);
	virtual ~MeshRefiner(void);
/**--------------------------------------------------------------------------
=begin
--- clear
	MeshRefiner �̓������ԃf�[�^���N���A����
=end
---------------------------------------------------------------------------*/
	void clear(void);
/**--------------------------------------------------------------------------
=begin
--- setMesh(mesh)
	���b�V���v�f�ו��̑ΏۂƂȂ郁�b�V����o�^����
=end
---------------------------------------------------------------------------*/
	void setMesh(kmb::MeshData* mesh);
/**--------------------------------------------------------------------------
=begin
--- setSecondNodesByData(name,stype=nil)
	����2���v�f�ƂȂ��Ă���ߓ_�� ElementGroup ����o�^����
=end
---------------------------------------------------------------------------*/
	void setSecondNodesByData(const char* name,const char* stype=NULL);
/**--------------------------------------------------------------------------
=begin
--- setSecondNodesByBody(bodyId)
	����2���v�f�ƂȂ��Ă���ߓ_�� Body ����o�^����
=end
---------------------------------------------------------------------------*/
	void setSecondNodesByBody(kmb::bodyIdType bodyId);
/**--------------------------------------------------------------------------
=begin
--- convertToSecondBody(bodyId)
	Body �Ɋ܂܂��v�f��2���v�f�ɕϊ�����B
=end
---------------------------------------------------------------------------*/
	kmb::bodyIdType convertToSecondBody(kmb::bodyIdType bodyId);
/**--------------------------------------------------------------------------
=begin
--- refineBody(bodyId)
	Body �Ɋ܂܂��v�f���ו�����B
=end
---------------------------------------------------------------------------*/
	kmb::bodyIdType refineBody(kmb::bodyIdType orgBodyId);
/**--------------------------------------------------------------------------
=begin
--- getOriginal(middleNodeId)
	���Ԑߓ_��^���āA���ꂪ�ǂ̗v�f�i�Ӂj���琶�����ꂽ����Ԃ�
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
	�ӂ܂��͗v�f��^���āA�������琶�����ꂽ���Ԑߓ_��Ԃ�
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
	refineBody ����Ƃ��ɓ����ɍX�V����f�[�^��o�^����
=end
---------------------------------------------------------------------------*/
	void appendData(const char* dataname,const char* stype=NULL);
/**--------------------------------------------------------------------------
=begin
--- clearData()
	refineBody ����Ƃ��ɓ����ɍX�V����f�[�^���N���A����
=end
---------------------------------------------------------------------------*/
	void clearData(void);
};

}
