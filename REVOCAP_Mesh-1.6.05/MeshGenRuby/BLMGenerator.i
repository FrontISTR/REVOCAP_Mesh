/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : BLMGenerator                                       #
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
=RevocapMeshGen::BLMGenerator

==�T�v

RevocapMesh �̃��b�V���Ǘ��N���X MeshDB �Ɋi�[���ꂽ�\�ʃ��b�V������
���E�w���b�V���𐶐����邽�߂̃N���X�ł���B

�g�p��F
 blm = BLMGenerator.new
 blm.setLayer([0.1,0.2,0.3])
 blmid = blm.generate(mesh,0)

==���\�b�h�ꗗ

((<BLMGenerator.new>))
((<setLayer>))
((<extrude>))
((<intrude>))

==���\�b�h

=end
------------------------------------------------------------------------------*/
%{
#include "MeshGen/kmbBLMGenerator.h"
%}

namespace kmb{

class MeshData;

class BLMGenerator
{
public:
/**--------------------------------------------------------------------------
=begin
--- BLMGenerator.new
	���E�w�������W���[���N���X BLMGenerator �̃C���X�^���X�𐶐�����B
=end
---------------------------------------------------------------------------*/
	BLMGenerator(void);
	virtual ~BLMGenerator(void);
/**--------------------------------------------------------------------------
=begin
--- setLayer(layers)
	layers �Ő������鋫�E�w�̌����i�ώZ�������́j��z��ŗ^����B
	��Fblm.setLayer([0.1,0.2,0.3])
	���̏ꍇ�́A�\�ʃ��b�V������ 0.1 0.2 0.3 �̍����̂Ƃ���ɋ��E�w��
	�^���悤�Ƃ��邽�߁A�����͂��ׂ� 0.1 �ƂȂ�B
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE setLayer(VALUE layers){
		Check_Type(layers, T_ARRAY);
		int layerNum = RARRAY_LEN(layers);
		double* layerThick = NULL;
		layerThick = new double[layerNum];
		for(int i=0;i<layerNum;++i){
			layerThick[i] = NUM2DBL(rb_ary_entry(layers,i));
		}
		self->setLayer(layerNum,layerThick);
		delete layerThick;
		return Qnil;
	}
}
/**--------------------------------------------------------------------------
=begin
--- extrude(mesh,bodyId)
	mesh �� bodyId �ŗ^������\�ʃ��b�V���ɑ΂��ċ��E�w���b�V���𐶐�����B
	�߂�l�͋��E�w�̊O���\�ʃ��b�V����BodyId�Ƌ��E�w���b�V����BodyId��v�f�Ɏ���
	Array �l�ł���B
	��Fblm.generate( mesh,0 ) => [1,2]
	���̂Ƃ� BodyId 1 �͊O���\�ʃ��b�V���̗v�f�O���[�vId�� BodyId 2 ���O�p���v�f�ɂ�鋫�E�w�̗v�f�O���[�vId�ł���B
--- intrude(mesh,parentId,bodyId)
	mesh �� bodyId �ŗ^������\�ʃ��b�V���ɑ΂��ċ��E�w���b�V���𐶐�����B
	�߂�l�͋��E�w�̊O���\�ʃ��b�V����BodyId�Ƌ��E�w���b�V����BodyId��v�f�Ɏ���
	Array �l�ł���B
	��Fblm.generate( mesh,0 ) => [1,2]
	���̂Ƃ� BodyId 1 �͊O���\�ʃ��b�V���̗v�f�O���[�vId�� BodyId 2 ���O�p���v�f�ɂ�鋫�E�w�̗v�f�O���[�vId�ł���B
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE extrude(MeshData* mesh,kmb::bodyIdType bodyId){
		self->setMesh(mesh);
		kmb::bodyIdType layerId;
		kmb::bodyIdType boundaryId = bodyId;
		layerId = self->extrude(boundaryId);
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,INT2FIX(boundaryId));
		rb_ary_store(ary,1,INT2FIX(layerId));
		return ary;
	}
	VALUE intrude(MeshData* mesh,kmb::bodyIdType parentId,kmb::bodyIdType bodyId){
		self->setMesh(mesh);
		kmb::bodyIdType layerId;
		kmb::bodyIdType boundaryId = bodyId;
		layerId = self->intrude(parentId,boundaryId);
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,INT2FIX(boundaryId));
		rb_ary_store(ary,1,INT2FIX(layerId));
		return ary;
	}
}

};

}
