/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : PatchModifier                                      #
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
=RevocapMeshGen::PatchModifier

==�T�v

�\�ʃp�b�`�̏C�����s�����߂̃N���X�B

==���\�b�h�ꗗ

((<PatchModifier.new>))


==���\�b�h

=end
------------------------------------------------------------------------------*/
%{
#include "MeshGen/kmbPatchModifier.h"
%}

namespace kmb{

class MeshData;

class PatchModifier
{
public:
/**--------------------------------------------------------------------------
=begin
--- PatchModifier.new(mesh,bodyId)
    mesh �� bodyId ��^���āA�\�ʃp�b�`�̏������s���B
=end
---------------------------------------------------------------------------*/
	PatchModifier(kmb::MeshData* mesh,kmb::bodyIdType bodyId);
	virtual ~PatchModifier(void);
/**--------------------------------------------------------------------------
=begin
--- triangleEdgeSwap(triangle0,triangle1)
	�O�p�` triangle0 �� triangle1 �ɂ��āA����炪�ڂ��Ă��鎞��
	�ڂ��Ă���ӂƑΊp������������B
	[0,1,3] + [1,2,3] => [0,1,2] + [2,3,0]
	�����ł������̌��ʂ� true / false �ŕԂ��B
=end
-----------------------------------------------------------------------------*/
	bool triangleEdgeSwap(kmb::elementIdType tri0, kmb::elementIdType tri1 );
/**--------------------------------------------------------------------------
=begin
--- edgeContract(nodeId0,nodeId1)
	nodeId1 �� nodeId0 �ɒׂ�
=end
-----------------------------------------------------------------------------*/
	bool edgeContract(kmb::nodeIdType node0, kmb::nodeIdType node1);
/**--------------------------------------------------------------------------
=begin
--- quadEdgeSwap(quad0, quad1, orientation=true)
	�l�p�` quad0 �� quad1 �ɂ��āA����炪�ڂ��Ă��鎞�ɁA�ӂ���������B
	�ӂ̌����̎d���͂Q�ʂ肠�邽�߁Aorientation �ŋ�ʂ���B
	[0,1,2,3] + [0,3,4,5] => [1,2,3,4] + [0,1,4,5]  (orientation=true)
                          => [0,1,2,5] + [2,3,4,5]  (orientation=false)
=end
-----------------------------------------------------------------------------*/
	bool quadEdgeSwap(kmb::elementIdType quad0, kmb::elementIdType quad1, bool orientation=true);
	kmb::elementIdType quadTrisect(kmb::elementIdType quad0, kmb::elementIdType quad1, bool orientation=true );
/**--------------------------------------------------------------------------
=begin
--- uniformOrientation(elemId=-1)
    �p�b�`�̖ʂ̌��������낦��B
    elemId �Ŋ�ƂȂ�O�p�`�� Id ��^����B
    �߂�l�͗��Ԃ����v�f�̌��B
=end
-----------------------------------------------------------------------------*/
	int uniformOrientation(kmb::elementIdType elementId=kmb::Element::nullElementId);


/**--------------------------------------------------------------------------
=begin
--- getAngleByIndex(elementId,index)
=end
-----------------------------------------------------------------------------*/
	double getAngleByIndex(kmb::elementIdType elementId,int index) const;
/**--------------------------------------------------------------------------
=begin
--- getNeighborElements(elementId)
--- getNeighborElement(face)
=end
-----------------------------------------------------------------------------*/
%extend{
	VALUE getNeighborElements(kmb::elementIdType elementId) const{
		VALUE ary = rb_ary_new();
		kmb::elementIdType neighbor[6];
		int res = self->getNeighborElements(elementId,neighbor);
		for(int i=0;i<res;++i){
			rb_ary_push(ary,INT2FIX( neighbor[i] ));
		}
		return ary;
	}
	kmb::elementIdType getNeighborElement(VALUE face) const{
		if( TYPE(face) == T_ARRAY ){
			kmb::elementIdType eId = FIX2INT(rb_ary_entry(face,0));
			kmb::idType lId = FIX2INT(rb_ary_entry(face,1));
			kmb::Face f(eId,lId);
			return self->getNeighborElement(f);
		}else{
			return kmb::Element::nullElementId;
		}
	}
}
};

}
