/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : Match                                              #
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
=RevocapMesh::Matching

RevocapMesh::MeshDB �Ɋi�[���ꂽ���b�V���ɑ΂���`��}�b�`���O�������s�����߂̃N���X�ł���

==���\�b�h�ꗗ

((<Matching.new>))
((<getDistanceEdgeToNode>))
((<getDistanceEdgeToEdge>))
((<getDistanceDataToData>))
((<getMatchingEdgePair>))
((<getSurfaceRelation>))
((<getFaceRelation>))
((<nodeMatchingOnBody>))
((<nodeMatchingBetweenBodies>))

=end
------------------------------------------------------------------------------*/

%{
#include "MeshDB/kmbMatching.h"
%}

namespace kmb{

class MeshDB;

class Matching
{
public:
/**--------------------------------------------------------------------------
=begin
--- Matching.new
	�`��}�b�`���O�����N���X Matching �̃C���X�^���X�𐶐�����B
=end
---------------------------------------------------------------------------*/
	Matching(void);
	virtual ~Matching(void);
/**--------------------------------------------------------------------------
=begin
--- getDistanceEdgeToNode( mesh, bodyId, nodeId )
	MeshDB mesh �� bodyId �ŗ^������1�����v�f�O���[�v�ɑ΂��āA
	�ߓ_ID nodeId �ŗ^������ߓ_�Ƃ̋�����Ԃ��B
	bodyId �ŗ^������v�f�O���[�v��1�����łȂ����AnodeId �ŕ\�킳���
	�ߓ_�����݂��Ȃ��Ƃ��Ȃǂ́A�������Ԃ��B
=end
---------------------------------------------------------------------------*/
	double getDistanceEdgeToNode( kmb::MeshDB* mesh, kmb::bodyIdType bodyId, kmb::nodeIdType nodeId );
/**--------------------------------------------------------------------------
=begin
--- getDistanceEdgeToEdge( mesh, bodyId0, bodyId1 )
	MeshDB mesh �� bodyId0, bodyId1 �ŗ^������1�����v�f�O���[�v�ɑ΂��āA
	�����̂Ȃ�������Ԃ��B
	�����ł�1�����v�f�O���[�v�Ԃ̋����̒�`��
	 1/2 * ( �ߓ_�Ɋւ��镽�� (bodyId0 �̐ߓ_�� bodyId1 �̋���) + �ߓ_�Ɋւ��镽�� (bodyId1 �̐ߓ_�� bodyId0 �̋���) )
	�ł���B
--- getDistanceEdgeToEdge( mesh0, bodyId0, mesh1, bodyId1 )
	MeshDB mesh0 �� bodyId0�AMeshDB mesh1 �� bodyId1 �ŗ^������1�����v�f�O���[�v�ɑ΂��āA
	�����̂Ȃ�������Ԃ��B
	�����ł�1�����v�f�O���[�v�Ԃ̋����̒�`��
	 1/2 * ( �ߓ_�Ɋւ��镽�� (bodyId0 �̐ߓ_�� bodyId1 �̋���) + �ߓ_�Ɋւ��镽�� (bodyId1 �̐ߓ_�� bodyId0 �̋���) )
	�ł���B
=end
---------------------------------------------------------------------------*/
	double getDistanceEdgeToEdge( kmb::MeshDB* mesh, kmb::bodyIdType edgeId0, kmb::bodyIdType edgeId1 );
	double getDistanceEdgeToEdge( kmb::MeshDB* mesh0, kmb::bodyIdType edgeId0, kmb::MeshDB* mesh1, kmb::bodyIdType edgeId1 );
/**-----------------------------------------------------------------------
=begin
--- getDistanceDataToData(mesh,key0,key1)
	key0 �� key1 �ŗ^������ Data �� BoundingBox �̊Ԃ̋��������߂�B
=end
--------------------------------------------------------------------------*/
	double getDistanceDataToData(kmb::MeshDB* mesh,const char* key0,const char* key1);

/**--------------------------------------------------------------------------
=begin
--- getMatchingEdgePair( mesh0, mesh1, tolerance )
	MeshDB mesh0 �� mesh1 �Ɋ܂܂��1�����v�f�O���[�v�Ԃ̋����� tolerance �ȉ��ɂȂ�悤��
	�g��T�����āA����� bodyId �� Array �� Array �ŕԂ��B
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getMatchingEdgePair( kmb::MeshDB& mesh0, kmb::MeshDB &mesh1, double tolerance ){
		VALUE ary = rb_ary_new();
		kmb::bodyIdType bodyCount0 = mesh0.getBodyCount();
		kmb::bodyIdType bodyCount1 = mesh1.getBodyCount();
		for(kmb::bodyIdType bodyId0 = 0; bodyId0 < bodyCount0; ++bodyId0){
			for(kmb::bodyIdType bodyId1 = 0; bodyId1 < bodyCount1; ++bodyId1){
				if( self->getDistanceEdgeToEdge( &mesh0, bodyId0, &mesh1, bodyId1 ) < tolerance ){
					VALUE item = rb_ary_new();
					rb_ary_push( item, INT2NUM(bodyId0) );
					rb_ary_push( item, INT2NUM(bodyId1) );
					rb_ary_push( ary, item );
				}
			}
		}
		return ary;
	}
}
/**-----------------------------------------------------------------------
=begin
--- getSurfaceRelation( mesh, bodyID0, bodyID1 )
	���ʗv�f�݂̂���Ȃ��̃{�f�B�ɑ΂��āA��v���Ă����� 1
	�t�����Ȃ� -1 ����ȊO�Ȃ� 0 ��Ԃ��B
	�`��ł͂Ȃ��A�O�p�`�̈ʑ����画�ʂ���B
=end
--------------------------------------------------------------------------*/
%extend{
	int getSurfaceRelation( kmb::MeshDB* mesh, kmb::bodyIdType surfId0, kmb::bodyIdType surfId1 ){
		kmb::ElementRelation::relationType rel =
			self->getSurfaceRelation( mesh, surfId0, surfId1 );
		switch(rel){
			case kmb::ElementRelation::EQUAL:
				return 1;
			case kmb::ElementRelation::REVERSE:
				return -1;
			default:
				return 0;
		}
	}
}
/**-----------------------------------------------------------------------
=begin
--- getFaceRelation( mesh, face0, face1 )
	FaceGroup �̓�� Data �ɑ΂��ĕ��ʂƂ��Ĉ�v���Ă����� 1
	�t�����Ȃ� -1 ����ȊO�Ȃ� 0 ��Ԃ��B
=end
--------------------------------------------------------------------------*/
%extend{
	int getFaceRelation( kmb::MeshDB* mesh, const char* faceId0, const char* faceId1 ){
		kmb::ElementRelation::relationType rel =
			self->getFaceRelation( mesh, faceId0, faceId1 );
		switch(rel){
			case kmb::ElementRelation::EQUAL:
				return 1;
			case kmb::ElementRelation::REVERSE:
				return -1;
			default:
				return 0;
		}
	}
}
/**-----------------------------------------------------------------------
=begin
--- nodeMatchingOnBody(mesh0, bodyId0, mesh1, tolerance, coupleName)
	mesh0 �� bodyId0 ��̐ߓ_�� mesh1 �̂��ׂĂ̐ߓ_�̒�����
	������ tolerance �ȉ��̂��̂�T���āAmesh0 �� coupleName �� Data �ɓo�^����B
	 mesh0.setPhysicalValueAtId( coupleName, mesh0 �� nodeId, mesh1 �� nodeId )
	coupleName �� Data �� NodeVariable Integer �l�Ŋ��ɑ��݂���ꍇ�͂��� Data �ɒǉ�����B
	���̃��[�h�⑼�̒l�� Data �����݂���ꍇ�͉������Ȃ��B
	�߂�l�͓o�^�����l�̌��B
=end
--------------------------------------------------------------------------*/
	int nodeMatchingOnBody(kmb::MeshDB* mesh0, kmb::bodyIdType bodyId0, kmb::MeshDB* mesh1, double tolerance, const char* coupleName);
/**-----------------------------------------------------------------------
=begin
--- nodeMatchingBetweenBodies(mesh0, bodyId0, mesh1, bodyId1, tolerance, coupleName)
	mesh0 �� bodyId0 ��̐ߓ_�� mesh1 �� bodyId1 ��̐ߓ_�̒�����
	������ tolerance �ȉ��̂��̂�T���āAmesh0 �� coupleName �� Data �ɓo�^����B
	 mesh0.setPhysicalValueAtId( coupleName, mesh0 �� nodeId, mesh1 �� nodeId )
	coupleName �� Data �� NodeVariable Integer �l�Ŋ��ɑ��݂���ꍇ�͂��� Data �ɒǉ�����B
	���̃��[�h�⑼�̒l�� Data �����݂���ꍇ�͉������Ȃ��B
	�߂�l�͓o�^�����l�̌��B
=end
--------------------------------------------------------------------------*/
	int nodeMatchingBetweenBodies(kmb::MeshDB* mesh0, kmb::bodyIdType bodyId0, kmb::MeshDB* mesh1, kmb::bodyIdType bodyId1, double tolerance, const char* coupleName);

};

}

%{
#include "MeshDB/kmbSurfaceMatching.h"
%}

namespace kmb{

class MeshData;

class SurfaceMatching
{
public:
	SurfaceMatching(void);
	virtual ~SurfaceMatching(void);
/**-----------------------------------------------------------------------
=begin
--- setMesh(mesh,insNodes=nil)
	mesh �ɓo�^���Ă���ʃO���[�v�ƕ\�ʗv�f�Ń}�b�`���O���s���B
	insNodes �̓_�~�[�v�f��ǉ������Ƃ��ɒǉ������ߓ_���o���Ă������߂̃f�[�^���B
	insNodes �� NodeGroup ���Ȃ���Ύ����I�ɍ��B
=end
--------------------------------------------------------------------------*/
	void setMesh(kmb::MeshData* mesh,const char* insNodes=NULL);
	void setPair(kmb::bodyIdType bodyId,const char* faceGroup);
	size_t constructDummyElements(void);
%extend{
	VALUE getMatchingElementId(VALUE v){
		VALUE ary = rb_ary_new();
		int index = -1;
		kmb::elementIdType elemId = self->getMatchingElementId( faceFromVALUE(v), index );
		if(elemId != kmb::Element::nullElementId){
			rb_ary_store( ary, 0, INT2FIX(elemId) );
			rb_ary_store( ary, 1, INT2FIX(index) );
			return ary;
		}else{
			return Qnil;
		}
	}
}

};

}
