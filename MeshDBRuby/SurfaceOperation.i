/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : SurfaceOperation                                   #
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
=RevocapMesh::SurfaceOperation

RevocapMesh::MeshDB �Ɋi�[���ꂽ�\�ʃ��b�V���̏������s���N���X�B
SurfaceOperaion �N���X�ł́A���ʗv�f�i�O�p�`�܂��͎l�p�`�j�ŗ^����ꂽ�ꍇ�A
���̗v�f�Ɩʔԍ��̑g�ŗ^����ꂽ�ꍇ�A���ꂼ��ɑ΂��āA
�p�x��^���Ă��̊p�x�ȏ�ɋȂ����Ă���\�ʂ𕪊�����@�\����������Ă���B

==���\�b�h�ꗗ

((<SurfaceOperation.new>))
((<setMesh>))
((<divideSurface>))
((<divideSurfaceWithRidge>))
((<divideFaceGroup>))
((<divideFaceGroupWithRidge>))
((<decomposeSurface>))
((<decomposeFaceGroup>))
((<faceGroup2Surface>))
((<surface2FaceGroup>))

=end
------------------------------------------------------------------------------*/

%{
#include "MeshDB/kmbSurfaceOperation.h"
%}


#pragma once

#include <map>
#include <vector>
#include <string>
#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElement.h"

namespace kmb{

class SurfaceOperation
{
public:
/**--------------------------------------------------------------------------
=begin
--- SurfaceOperation.new
	�\�ʃ��b�V�������N���X SurfaceOperation �̃C���X�^���X�𐶐�����B
=end
------------------------------------------------------------------------------*/
	SurfaceOperation(void);
	virtual ~SurfaceOperation(void);
/**--------------------------------------------------------------------------
=begin
--- setMesh(mesh)
	�ΏۂƂȂ� MeshDB ��^����B
=end
------------------------------------------------------------------------------*/
	void setMesh(kmb::MeshDB* mesh);
/**--------------------------------------------------------------------------
=begin
--- setPermitReverse(f)
	�\�ʂ���ʃO���[�v�𒊏o����Ƃ��ɁA�t�������������ۂ�
	�K��l�� true �Ȃ̂ŋ����Ȃ��Ƃ��� false �ɂ���
=end
------------------------------------------------------------------------------*/
	void setPermitReverse(bool f);
/**--------------------------------------------------------------------------
=begin
--- divideSurface(bodyId,angle)
--- divideSurfaceWithRidge(bodyId,angle)
	���ʗv�f����Ȃ� bodyId �̃{�f�B�ɂ��āA�p�x angle ��臒l�Ƃ���
	��������B
	divideSurface �͕��������v�f�O���[�v�� Id �� Array �ɓ���ĕԂ��B
	divideSurfaceWithRidge �͕��������{�f�B Id �ƗŐ��̃{�f�B Id �� Hash �ɓ���ĕԂ��B
	���Ƃ� bodyId �ɂ͗Ő���������Ȃ�v�f�O���[�v������B
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE divideSurface(kmb::bodyIdType bodyId,double angle){
		std::vector<kmb::bodyIdType> surfIds;
		self->divideSurface(bodyId,angle,surfIds);
		VALUE ary = rb_ary_new();
		int i=0;
		for(std::vector<kmb::bodyIdType>::iterator p = surfIds.begin();
			p != surfIds.end(); ++p)
		{
			kmb::bodyIdType surfId = (*p);
			rb_ary_store(ary,i,INT2FIX(surfId));
			++i;
		}
		return ary;
	}

	VALUE divideSurfaceWithRidge(kmb::bodyIdType bodyId,double angle){
		std::map<kmb::bodyIdType,kmb::bodyIdType> surfIds;
		self->divideSurfaceWithRidge(bodyId,angle,surfIds);
		VALUE hash = rb_hash_new();
		for(std::map<kmb::bodyIdType,kmb::bodyIdType>::iterator p = surfIds.begin();
			p != surfIds.end(); ++p)
		{
			rb_hash_aset( hash,INT2FIX(p->first),INT2FIX(p->second) );
		}
		return hash;
	}
}
/**-------------------------------------------------------------------------
=begin
--- divideFaceGroup(name,angle)
--- divideFaceGroupWithRidge(name,angle)
	Face ����Ȃ� Data �� angle ��臒l�Ƃ��ĕ�������B
	divideFaceGroup �͕������� Face �ɁA���Ƃ� name �� _N �iN�͐����j��t���������O�����āA
	�����ʂ̖��O��Array �Ɋi�[���ĕԂ��B
	divideFaceGroupWithRidge �͕������� Face �� Data ���� key �Ƃ��āA
	�Ő� Body �� BodyId �� value �Ƃ��A���Ƃ� Face �ɑ΂��Ă͑S�̗̂Ő� Body ��
	BodyId �� value �Ƃ��� Hash ��Ԃ��B

=end
---------------------------------------------------------------------------*/
%extend{
	VALUE divideFaceGroup(const char* name,double angle){
		std::vector<std::string> faceNames;
		self->divideFaceGroup(name,angle,faceNames);
		VALUE ary = rb_ary_new();
		int i=0;
		std::vector<std::string>::iterator sIter = faceNames.begin();
		while( sIter != faceNames.end() )
		{
			std::string facename = (*sIter);
			rb_ary_store(ary,i,rb_str_new2(facename.c_str()));
			++i;
			++sIter;
		}
		return ary;
	}
	VALUE divideFaceGroupWithRidge(const char* name,double angle){
		std::map<std::string,kmb::bodyIdType> faceNames;
		kmb::bodyIdType bodyId = self->divideFaceGroupWithRidge(name,angle,faceNames);
		VALUE hash = rb_hash_new();
		rb_hash_aset( hash, rb_str_new2(name), INT2FIX(bodyId) );
		std::map<std::string,kmb::bodyIdType>::iterator sIter = faceNames.begin();
		while( sIter != faceNames.end() )
		{
			rb_hash_aset( hash, rb_str_new2((sIter->first).c_str()) ,INT2FIX(sIter->second) );
			++sIter;
		}
		return hash;
	}
}
/**-------------------------------------------------------------------------
=begin
--- decomposeSurface(bodyId)
	�A�������ɕ�����B
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE decomposeSurface(kmb::bodyIdType bodyId){
		std::set<kmb::bodyIdType> compIds;
		self->decomposeSurface(bodyId,compIds);
		VALUE ary = rb_ary_new();
		std::set<kmb::bodyIdType>::iterator bIter = compIds.begin();
		while( bIter != compIds.end() )
		{
			kmb::bodyIdType compId = (*bIter);
			rb_ary_push(ary,INT2FIX(compId));
			++bIter;
		}
		return ary;
	}
}
/**-------------------------------------------------------------------------
=begin
--- decomposeFaceGroup(faceGroup)
	�A�������ɕ�����B
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE decomposeFaceGroup(const char* faceGroup){
		std::vector<std::string> faceNames;
		self->decomposeFaceGroup(faceGroup,faceNames);
		VALUE ary = rb_ary_new();
		std::vector<std::string>::iterator sIter = faceNames.begin();
		while( sIter != faceNames.end() )
		{
			std::string facename = (*sIter);
			rb_ary_push(ary,rb_str_new2(facename.c_str()));
			++sIter;
		}
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
--- faceGroup2Surface(name)
	FaceGroup ���� surface �𐶐�����
--- surface2FaceGroup(bodyId,parentBodyId,name);
	parentBodyId �� Body �̋��E�ɂȂ��Ă��� bodyId �� Body ���^����ꂽ���ɁA
	name �� FaceGroup �𐶐�����B
=end
-----------------------------------------------------------------------------*/
	kmb::bodyIdType faceGroup2Surface(const char* name);
%extend{
	bool surface2FaceGroup(kmb::bodyIdType bodyId,VALUE parent,const char* name){
		if( TYPE(parent) == T_STRING ){
			return self->surface2FaceGroup(bodyId,StringValuePtr(parent),name);
		}else if( TYPE(parent) == T_FIXNUM ){
			return self->surface2FaceGroup(bodyId,FIX2INT(parent),name);
		}
		return false;
	}
}
	void setSurface(kmb::MeshDB* mesh,kmb::bodyIdType bodyId);
	enum vertexType{
		Unknown,
		CONVEX,     // ���_�̎���̗v�f�̊p�x�̘a�� 360 �x��菬��������
		CONCAVE,    // ���_�̎���̗v�f�̊p�x�̘a�� 360 �x��菬��������
		FLAT,       // ���_�̎���̗v�f�̊p�x�̘a�� 360 �x
		HYPERBOLIC, // ���_�̎���̗v�f�̊p�x�̘a�� 360 �x���傫��
	};
	kmb::SurfaceOperation::vertexType getVertexType(kmb::nodeIdType nodeId);
};

}

/**--------------------------------------------------------------------------
=begin
==�g�p��
=== ���ʗv�f�ŗ^����ꂽ�\�ʃ��b�V���𕪊�������@

�����̂̂悤�ȃ��b�V������\�ʃ��b�V���̒��o���s���ƁA�����̂̕\�ʑS�̂�
1�̃{�f�B�Ƃ��ē����邪�A�����̂�6�ʂɕ������ċ��E������ݒ肷��ꍇ�Ȃǂ�
�s���B

��F
TetMesh �`���̎l�ʑ̃��b�V������\�ʃ��b�V���𒊏o���ĕ�������
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
  surfId = bext.getBoundary(0)
  surfOp = RevocapMesh::SurfaceOperation.new
  surfOp.setMesh(mesh)
  divSurfs = surfOp.divideSurface(surfId,60)

60�x��臒l�Ƃ��� surfId �ŕ\�����\�ʃ��b�V���𕪊�����BdivSurfs ��
�������ꂽ�\�ʃ��b�V���� BodyId ���i�[����Ă���B

��F
TetMesh �`���̎l�ʑ̃��b�V������\�ʃ��b�V���𒊏o���ĕ������Ă��̋��E�����b�V�����擾����
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
  surfId = bext.getBoundary(0)
  surfOp = RevocapMesh::SurfaceOperation.new
  surfOp.setMesh(mesh)
  divSurfs = surfOp.divideSurfaceWithRidge(surfId,60)

60�x��臒l�Ƃ��� surfId �ŕ\�����\�ʃ��b�V���𕪊�����BdivSurfs ��
�������ꂽ�\�ʃ��b�V���Ƃ��̋��E�̐����b�V���� BodyId �̑g�� Hash �Ƃ��Ċi�[����Ă���B

  divSurfs.each{ |k,v|
    # k = �������ꂽ�\�ʃ��b�V���� BodyId
    # v = BodyId k �̕\�ʃ��b�V���̋��E�̐����b�V��
  }
  # surId = ���E�ƂȂ��������b�V���S��

��F
TetMesh �`���̎l�ʑ̃��b�V������v�f�Ɩʔԍ��̑g�ŕ\�ʂ𒊏o���ĕ�������B

  require 'RevocapMesh'
  require 'MeshIO/TetMeshIO'

  mesh = RevocapMesh::MeshDB.new
  tetmesh = TetMeshIO.new
  tripatch = TripatchIO.new
  tetmesh.loadFromFile('test.msh',mesh)
  bext = RevocapMesh::BoundaryExtractor.new
  bext.setMesh(mesh)
  bext.appendBody(0)
  bext.getBoundaryFace(0,"wall")
  surfOp = RevocapMesh::SurfaceOperation.new
  surfOp.setMesh(mesh)
  divSurfs = surfOp.divideFaceGroup("wall",60)
  divSurfs.each{ |name|
    mesh.eachId(name){ |face|
      # �������ꂽ FaceGroup ���[�h�� Data Name �ɂ�����
      # face[0] ���\�ʂ̗v�f�ԍ�
      # face[1] ���\�ʂ̖ʔԍ�
    }
  }

��F
TetMesh �`���̎l�ʑ̃��b�V������v�f�Ɩʔԍ��̑g�ŕ\�ʂ𒊏o���ĕ������A���E�����b�V�����擾����B

  require 'RevocapMesh'
  require 'MeshIO/TetMeshIO'

  mesh = RevocapMesh::MeshDB.new
  tetmesh = TetMeshIO.new
  tripatch = TripatchIO.new
  tetmesh.loadFromFile('test.msh',mesh)
  bext = RevocapMesh::BoundaryExtractor.new
  bext.setMesh(mesh)
  bext.appendBody(0)
  bext.getBoundaryFace(0,"wall")
  surfOp = RevocapMesh::SurfaceOperation.new
  surfOp.setMesh(mesh)
  divSurfs = surfOp.divideFaceGroupWithRidge("wall",60)
  divSurfs.each{ |name,ridge|
    mesh.eachId(name){ |face|
      # �������ꂽ FaceGroup ���[�h�� Data Name �ɂ�����
      # face[0] ���\�ʂ̗v�f�ԍ�
      # face[1] ���\�ʂ̖ʔԍ�
    }
    # ridge = name �� FaceGroup �̋��E�����b�V��
  }

=end
------------------------------------------------------------------------------*/
