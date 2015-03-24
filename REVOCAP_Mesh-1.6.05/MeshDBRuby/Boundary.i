/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : BoundaryExtractor                                  #
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
=RevocapMesh::BoundaryExtractor

RevocapMesh::MeshDB �Ɋi�[���ꂽ���b�V���ɑ΂��āA���̋��E�ʂ𒊏o����
���߂̏������s���N���X�ł���B

RevocapMesh �ł́A��̓R�[�h�̗v���ɉ����āA�{�����[�����b�V������\�ʃ��b�V����
���o���@�Ƃ��āA�\�ʂ𕽖ʗv�f�i�O�p�`�܂��͎l�p�`�j�ŗ^������@�ƁA
���̗v�f�Ɩʔԍ��̑g�ŗ^����Q�ʂ�̂������\�ł���B
SurfaceOperation �N���X��p����ƁA���ꂼ��ɑ΂��Ċp�x�ŕ������邱�Ƃ��\�ł���B

==���\�b�h�ꗗ

((<BoundaryExtractor.new>))
((<setMesh>))
((<setReverseMode>))
((<getReverseMode>))
((<appendBody>))
((<appendData>))
((<eraseData>))
((<getBoundary>))
((<getBoundaryFace>))
((<getBoundaryComponents>))
((<clear>))

=end
------------------------------------------------------------------------------*/

%{
#include "MeshDB/kmbBoundaryExtractor.h"
%}

namespace kmb{

class BoundaryExtractor
{
public:
/**--------------------------------------------------------------------------
=begin
--- BoundaryExtractor.new
	���E���o�N���X BoundaryExtractor �̃C���X�^���X�𐶐�����B
=end
------------------------------------------------------------------------------*/
	BoundaryExtractor(void);
	virtual ~BoundaryExtractor(void);
/**--------------------------------------------------------------------------
=begin
--- setMesh(mesh)
	�ΏۂƂȂ� MeshDB ��^����B
=end
------------------------------------------------------------------------------*/
	void setMesh(kmb::MeshData* mesh);
/**--------------------------------------------------------------------------
=begin
--- setReverseMode(mode)
    ���E���o�̎��ɗ��Ԃ��Ă���v�f�������ꍇ�� true ��ݒ肷��B
--- getReverseMode
    ���E���o�̎��ɗ��Ԃ��Ă���v�f���������������o�͂���B
=end
------------------------------------------------------------------------------*/
	void setReverseMode(bool mode);
	bool getReverseMode(void) const;
/**--------------------------------------------------------------------------
=begin
--- appendBody(bodyId)
	���E���o���s�� Body ��ǉ�����B
=end
------------------------------------------------------------------------------*/
	void appendBody(kmb::bodyIdType bodyId);
/**--------------------------------------------------------------------------
=begin
--- appendData(name)
	���E���o���s�� ElementGroup ��ǉ�����B
=end
------------------------------------------------------------------------------*/
	void appendData(const char* name);
/**--------------------------------------------------------------------------
=begin
--- eraseData(name)
	���E���珜������ʂ��w�肷�� ElementGroup / FaceGroup ��^����B
	reverse = true �̎��́A�t�����̖ʂ��o�^����Ă���Ώ�������B
=end
------------------------------------------------------------------------------*/
	void eraseData(const char* name,bool reverse=false);
/**--------------------------------------------------------------------------
=begin
--- getBoundary(bodyId)
--- getBoundary()
	���E�𒊏o���� MeshDB �Ɋi�[���A���� BodyId ��Ԃ��B
	��F���郁�b�V���̋��E�����߂�ꍇ
	 ex = BoundaryExtractor.new
	 ex.setMesh(mesh)
	 ex.appendBody(bodyId)
	 boundaryId = ex.getBoundary(bodyId)
	��F������ Body �ɂ��ċ��E�ʒ��o����ꍇ
	 ex = BoundaryExtractor.new
	 ex.setMesh(mesh)
	 ex.appendBody(bodyId0)
	 ex.appendBody(bodyId1)
	 boundaryId0 = ex.getBoundary(bodyId0)
	 boundaryId1 = ex.getBoundary(bodyId1)
	���̂Ƃ��́AbodyId0 �� bodyId1 �ŕ\�����Q���b�V���̋��E�ʂ�
	boundaryId0 boundaryId1 �ɂ͊܂܂Ȃ��B
	getBoundary �̈������ȗ������ꍇ�͂��ׂĂ� Body �Ɋ܂܂��v�f�ɂ��ċ��E�ʂ𒊏o����B
--- getBoundaryComponents(bodyId)
	���E�𒊏o���ĘA�������ɕ����� MeshDB �Ɋi�[���A���� BodyId �� Array ��Ԃ��B
=end
------------------------------------------------------------------------------*/
	kmb::bodyIdType getBoundary(kmb::bodyIdType bodyId);
	kmb::bodyIdType getBoundary(void);
%extend{
	VALUE getBoundaryComponents(kmb::bodyIdType bodyId){
		kmb::bodyIdType* boundIds = NULL;
		int num = self->getBoundaryComponents(bodyId,boundIds);
		VALUE ary = rb_ary_new();
		for(int i=0;i<num;++i){
			rb_ary_push(ary,INT2NUM(boundIds[i]));
		}
		if( boundIds ){
			delete[] boundIds;
		}
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
--- getBoundaryFace(bodyId,name)
	���E�� name �ƌ������O�� Data �� Face �̏W���Ƃ��Ē��o����B
	���[�h�� FaceGroup �l�̌^�� None �ɂȂ�B
=end
------------------------------------------------------------------------------*/
	bool getBoundaryFace(kmb::bodyIdType bodyId,const char* name);
/**--------------------------------------------------------------------------
=begin
--- isClosed(bodyId)
	boundary ���Ȃ���� = closed ���ǂ����̔���
	����̂��߂ɂ͂��炩���� appendBody ���Ă������Ƃ��K�v
=end
------------------------------------------------------------------------------*/
	bool isClosed(kmb::bodyIdType bodyId);

/**--------------------------------------------------------------------------
=begin
--- clear
	MeshDB ����ы��E���o�̑ΏۂƂȂ� Body �̏�����������B
=end
------------------------------------------------------------------------------*/
	void clear(void);
};

}

/**--------------------------------------------------------------------------
=begin
==�g�p��
=== �O�p�`�ŕ\�ʂ𒊏o������@

appendBody ���\�b�h�ŕ\�ʂ𒊏o����ΏۂƂȂ� Body ��^���āAgetBoudary ���\�b�h��
���E�ʂ𐶐������� BodyID �����o���B

��F
TetMesh �`���̎l�ʑ̃��b�V�� test.msh ����\�ʃ��b�V���𒊏o���āATripatch �`���� patch �t�@�C�����o�͂���

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
  surfID = bext.getBoundary(0)
  tripatch.bodyID = surfID
  tripatch.saveToFile('test.pch',mesh)

=== �l�ʑ̗v�f�Ɩʔԍ��̑g�ŕ\�ʂ𒊏o������@

appendBody ���\�b�h�ŕ\�ʂ𒊏o����ΏۂƂȂ� Body ��^���āAgetBoudaryFace ���\�b�h��
������������Ƃ��ė^���A�v�f�Ɩʔԍ��̑g�̏W���� FaceGroup ���[�h�� Data �Ő�������B

��F
TetMesh �`���̎l�ʑ̃��b�V������v�f�Ɩʔԍ��̑g�ŕ\�ʂ𒊏o����B

  require 'RevocapMesh'
  require 'MeshIO/TetMeshIO'

  mesh = RevocapMesh::MeshDB.new
  tetmesh = TetMeshIO.new
  tripatch = TripatchIO.new
  tetmesh.loadFromFile('test.msh',mesh)
  bext = RevocapMesh::BoundaryExtractor.new
  bext.setMesh(mesh)
  bext.appendBody(0)
  surfID = bext.getBoundaryFace(0,'wall')
  mesh.extractBoundaryFace(0,"wall")
  mesh.eachId("wall"){ |face|
    # face[0] ���\�ʂ̗v�f�ԍ�
    # face[1] ���\�ʂ̖ʔԍ�
  }

=end
------------------------------------------------------------------------------*/
