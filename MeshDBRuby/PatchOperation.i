/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : PatchOperation                                     #
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
=RevocapMesh::PatchOperation

RevocapMesh::PatchOperation �N���X��
RevocapMesh::MeshDB �Ɋi�[���ꂽ�\�ʃp�b�`�ɑ΂��āA�����Ԃ𖄂߂�A
���������낦��Ȃǂ̈ʑ������E�􉽏������s�����߂̃N���X�ł���B

�`��f�[�^ (IGES) ����p�b�`�f�[�^�𐶐�����ɂ́A���̃N���X�ł͂Ȃ�
RevocapPatch ���W���[�����g���B

==���\�b�h�ꗗ

((<PatchOperation.new>))
((<uniformOrientation>))
((<isClosed>))
((<subdivideByEdge>))
((<subdivideByEdgeWithPatch>))
((<divideByPlane>))

=end
------------------------------------------------------------------------------*/

%{
#include "MeshDB/kmbPatchOperation.h"
%}

namespace kmb{

class PatchOperation
{
public:
/**--------------------------------------------------------------------------
=begin
--- PatchOperation.new
	�p�b�`�����N���X PatchOperation �̃C���X�^���X�𐶐�����B
=end
---------------------------------------------------------------------------*/
	PatchOperation(void);
	virtual ~PatchOperation(void);
/**--------------------------------------------------------------------------
=begin
--- uniformOrientation(mesh,patch,elementId=-1)
	MeshDB �� bodyId �� patch �ŗ^������\�ʃp�b�`�ɑ΂��āA
	�v�f�̌������ꌳ������B
	�߂�l�͌�����ς����v�f�̌��B
	elementId �͊�ƂȂ�v�f�� Id ��^���A���̗v�f�Ɨאڂ��Ă���v�f�̌�����
	���낦�Ă����B�ȗ������ꍇ�͎����I�Ɍ��߂�B
	�\�ʃp�b�`���A�����Ă��Ȃ����ɂ� elementId �𖾎��I�ɗ^����K�v������B
=end
---------------------------------------------------------------------------*/
	int uniformOrientation(kmb::MeshDB* mesh,kmb::bodyIdType bodyId,kmb::elementIdType elementId=kmb::Element::nullElementId);
/**--------------------------------------------------------------------------
=begin
--- subdivideByEdge(mesh,edgeId,tolerance=Float::MAX,globalTolerance=Float::MAX)
	edgeId ��̐ߓ_�ő��̂P���� Body ��̐������ו�����B
	�߂�l�� edge �ƌ`��}�b�`���� mesh �� 1 ���� Body �� Id ��Ԃ��B
--- subdivideByEdgeWithPatch(mesh,edgeId,tolerance=Float::MAX,globalTolerance=Float::MAX)
	edgeId ��̐ߓ_�ő��̂P���� Body ��̐������ו�����B
	����ɂ��̂P���� Body �����E�Ɏ��悤�ȎO�p�`�ɂ��Ă� edgeId ��̐ߓ_�ōו�����B
	�߂�l�� edge �ƌ`��}�b�`���� mesh �� 1 ���� Body �� Id ��Ԃ��B
=end
-----------------------------------------------------------------------------*/
	kmb::bodyIdType subdivideByEdge(kmb::MeshDB* mesh,kmb::bodyIdType edgeId,double tolerance=DBL_MAX,double globalTolerance=DBL_MAX);
	kmb::bodyIdType subdivideByEdgeWithPatch(kmb::MeshDB* mesh,kmb::bodyIdType edgeId,double tolerance=DBL_MAX,double globalTolerance=DBL_MAX);
/**--------------------------------------------------------------------------
=begin
--- divideByPlane(mesh,bodyId,a,b,c,d,cap=false,duplicate=false)
--- divideByPlane(mesh,bodyId,plane,cap=false,duplilcate=false)
	�O�p�`����Ȃ� Body �� ax+by+cz+d=0 �̕��ʂŕ�������B
	plane �� RevocapMesh::FramedPlane.new(a,b,c,d) �Ő�������
	cap = true �̂Ƃ��͕��������ʂɂӂ�������B
	duplicate = true �̂Ƃ��́A���������ʂ̐ߓ_���d������B
	�����������̈�ƕ��̈�̎O�p�` BodyID �̑g��
	�ӂ�������̂ɐ����������̐^�U�l��3�g�݂�Ԃ��B
=end
-----------------------------------------------------------------------------*/
%extend{
	VALUE divideByPlane(kmb::MeshDB* mesh,kmb::bodyIdType bodyId,double a,double b,double c,double d,bool capFlag=false,bool duplicateFlag=false){
		kmb::FramedPlane plane(a,b,c,d);
		VALUE ary = rb_ary_new();
		kmb::bodyIdType positiveID;
		kmb::bodyIdType negativeID;
		bool capped = self->divideByPlane(mesh,bodyId,plane,positiveID,negativeID,capFlag,duplicateFlag);
		rb_ary_push(ary,INT2NUM(positiveID));
		rb_ary_push(ary,INT2NUM(negativeID));
		rb_ary_push(ary,(capped ? Qtrue : Qfalse));
		return ary;
	}
	VALUE divideByPlane(kmb::MeshDB* mesh,kmb::bodyIdType bodyId,kmb::FramedPlane &plane,bool capFlag=false,bool duplicateFlag=false){
		VALUE ary = rb_ary_new();
		kmb::bodyIdType positiveID;
		kmb::bodyIdType negativeID;
		bool capped = self->divideByPlane(mesh,bodyId,plane,positiveID,negativeID,capFlag,duplicateFlag);
		rb_ary_push(ary,INT2NUM(positiveID));
		rb_ary_push(ary,INT2NUM(negativeID));
		rb_ary_push(ary,(capped ? Qtrue : Qfalse));
		return ary;
	}
}
};

}
