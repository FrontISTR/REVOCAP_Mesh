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

RevocapMesh::MeshDB �̓��b�V�������̂��߂̃N���X�B�R���e�i�@�\�� MeshData ���p�����Ă���B
MeshData �ɉ����A

* �􉽏���
* ��������
* �ߖT����
* �v�f�Ԋ֌W
* ���b�V�������E�ϊ�
* �f�[�^�����E���

���s�����Ƃ��ł���B

Ruby ���ꂩ��g�����߂ɂ́A�g�����C�u���� RevocapMesh.so �� Ruby �̃��[�h�p�X�ɂ�����
�i�C���X�g�[���X�N���v�g�����s�����ꍇ�͕W�����C�u�����f�B���N�g���ɃR�s�[����Ă���̂Ŗ��Ȃ��j�A

 require 'RevocapMesh'
 mesh = RevocapMesh::MeshDB.new

�Ƃ���ƁARevocapMesh::MeshDB �̃I�u�W�F�N�g�̃C���X�^���X mesh �����������Bmesh �I�u�W�F�N�g�ɑ΂���
�ȉ��Ő������郁�\�b�h�Q���Ăяo�����Ƃ��o����B

==���\�b�h�ꗗ

���L�̈ꗗ�ɉ����āAMeshData �̃��\�b�h�ꗗ���Q�Ƃ��邱�ƁB

===�N���X���\�b�h

((<MeshDB.new>))
((<getElementTypeMap>))

===���b�V���Ǘ�

((<copyMesh>))
((<setDefaultNodeContainerType>))
((<getDefaultNodeContainerType>))
((<setDefaultElementContainerType>))
((<getDefaultElementContainerType>))
((<setDefaultDataContainerType>))
((<getDefaultDataContainerType>))
((<setDefaultContainerType>))

===�ߓ_�Ǘ�

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

===�v�f�Ǘ�

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

===�􉽏���

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

===��������

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

===�ߖT����

((<clearNeighborsInfo>))
((<generateNeighborsInfo>))
((<getNeighborNodes>))
((<getSurroundingElements>))
((<getSurroundingCount>))
((<getNeighborElements>))
((<getAdjacentFace>))

===�v�f�Ԋ֌W

((<getFaces>))
((<isFace>))
((<getCommonElement>))
((<getElementRelation>))

===���b�V�������E�ϊ�

((<mergeBody>))
((<convertToLinearBody>))
((<importBody>))
((<importBodyWithNodeMatching>))
((<importAllBody>))

=== �f�[�^�Ǘ�

((<getDataNameArray>))
((<replaceIdOfData>))
((<convertData>))
((<convertBodyToData>))
((<getElementCountOfData>))
((<getNodeCountOfData>))
((<eachNodeIdOfData>))
((<getBoundingBoxOfData>))
((<getUniqueDataName>))

===�f�[�^�����E���

((<getNearestValue>))
((<getInterpolatedValue>))
((<getInterpolatedMultiValues>))
((<getInterpolatedValueInData>))
((<getInterpolatedMultiValuesInData>))
((<getMinMaxValue>))
((<getMinMaxValueWithId>))

===���̑�

((<calcMeshProperty>))

=end

MeshData �Œ�`���Ă�����̂ɂ��ẮA�����Œ�`����͕̂s�v�B
�������h�L�������g�Ƃ̐���������邽�߂ɁA�L�q���Ă���B
MeshData �Œ�`�������\�b�h���I�[�o�[���C�h�E�I�[�o�[���[�h���Ă�����̂ɂ��Ă�
������ōĒ�`����K�v����B��FaddNode

�����K������
createXXX : �R���e�i�n�̐V�K�쐬
generateXXX : ���łɃR���e�i�ɓ����Ă���f�[�^����ʂ̃f�[�^�̐���
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
== �N���X���\�b�h
--- MeshDB.new()
	MeshDB �N���X�̃R���X�g���N�^�ł���BMeshDB �I�u�W�F�N�g�̃C���X�^���X�𐶐�����B
--- getElementTypeMap()
	RevocapMesh �ň������Ƃ��ł���v�f�̎�ނƂ��̎��ʂ̂��߂̐����萔�� Hash ��Ԃ��B
	�Ⴆ��
	 typeMap = MeshDB.getElementTypeMap
	 typeMap["TETRAHEDRON"]
	�œ����鐮���l�� MeshDB �N���X�� addElement �Ȃǂŗv�f�̎�ނ����ʂ���̂�
	�p������B
	���ꂼ��̗v�f�̏ڍׂɂ��Ă�
	((<�v�f���C�u����|URL:Element.html>))
	���Q�Ƃ̂��ƁB
=end
---------------------------------------------------------------------*/
	MeshDB(void);
	virtual ~MeshDB(void);

/**--------------------------------------------------------------------
=begin
==���b�V���Ǘ�
===���b�V������
--- copyMesh(other)
	�ȉ��Ő������郁�\�b�h eachNodeWithId ����� eachElement ����`����Ă��鑼�̃��b�V�� other
	�iMeshDB�łȂ��Ă��悢�j���烁�b�V�����R�s�[���č��B
	�Ⴆ�Α��̃��b�V���[���烁�b�V���f�[�^�� MeshDB �̃I�u�W�F�N�g�ɃR�s�[���鎞�ȂǂɎg���B
=end

	VALUE selfDB = SWIG_Ruby_NewPointerObj(self, SWIGTYPE_p_kmb__MeshDB, 0);

	�� SWIG ����������R�[�h�̃��\�b�h�𗬗p�����B
	MeshDB* ���� VALUE �ւ̕ϊ����s���B

	rb_iterate( _eachNodeWithId, mesh, (VALUE(*)(...))meshDB_addNodeWithId , selfDB );

	�� mesh �� eachNode ���Ăяo���āA���̃u���b�N�̒���
	meshDB_addNodeWithId �����s����B
	_eachNodeWithId �� meshDB_addNodeWithId �� Callback �֐��B
	meshdb.i �Œ�`����ƁASWIG �����O��ς��Ă��܂��̂ŁA
	kmbMeshDB_SWIG.h �ɒ�`���Ă���B

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
	�f�t�H���g�̐ߓ_�R���e�i�̎�ނ��w�肷��B
	RevocapMesh �Ŏg�p�\�ȃR���e�i�^�C�v�F
	 - stl::map<id,Point3D*>
	 - stl::vector<Point3D*>
	 - double_array
	 - double_marray
---	getDefaultNodeContainerType()
	�f�t�H���g�̐ߓ_�R���e�i�̎�ނ��擾����B
--- setDefaultElementContainerType(elementContainerType)
	�f�t�H���g�̗v�f�R���e�i�̎�ނ��w�肷��B
---	getDefaultElementContainerType()
	�f�t�H���g�̗v�f�R���e�i�̎�ނ��擾����B
--- setDefaultDataContainerType(dataContainerType)
	�f�t�H���g�̃f�[�^�R���e�i�̎�ނ��w�肷��B
---	getDefaultDataContainerType()
	�f�t�H���g�̃f�[�^�R���e�i�̎�ނ��擾����B
--- setDefaultContainerType(containerType)
	�ߓ_�A�v�f�A�f�[�^�R���e�i��ړI�ɉ����ēK�؂ɐݒ肷��
	 - "Post"
	 - "Pre"
	 - "Refiner"
	�ɑΉ�
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
==�ߓ_�Ǘ��iMeshDB �̊g���@�\�j

MeshData �Œ�`����Ă��郁�\�b�h�͏���

=end
--------------------------------------------------------------------------*/

/**------------------------------------------------------------------------
=begin
--- getMaxNodeId()
--- getMinNodeId()
	MeshDB�ɓo�^����Ă���ߓ_�̍ő�ߓ_Id�ƍŏ��ߓ_Id��Ԃ��B
=end
--------------------------------------------------------------------------*/
	kmb::nodeIdType getMaxNodeId(void);
	kmb::nodeIdType getMinNodeId(void);
/**------------------------------------------------------------------------
=begin
--- getNode(nodeId)
	�ߓ_Id �� nodeId �̐ߓ_�̍��W�� [x,y,z] �`���� Array �ŕԂ��B
	�ߓ_�R���e�i���Q�����̏ꍇ�� [x,y] �`���� Array �ŕԂ��B
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
	�ߓ_ID �� nodeId �̒��_�̍��W�� Point3D �ŕԂ��B
	�i���j���� Point3D �̒l��ς��Ă����̃��b�V���̐ߓ_���W�͕ς��܂���B
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
	�ߓ_���폜����B���Ƃ��Ƃ��� nodeId �̐ߓ_���Ȃ���Ή������Ȃ��B
=end
--------------------------------------------------------------------------*/
	bool deleteNode(kmb::nodeIdType id);
/**------------------------------------------------------------------------
=begin
--- eachNode
	�ߓ_�̃C�e���[�^�ł���B

	 mesh.eachNode{ |node|
	   x = node[0]
	   y = node[1]
	   z = node[2]
	 }

	�̂悤�Ɏg���B

--- eachNodeWithId
	�ߓ_�Ɛߓ_ID�̃C�e���[�^�ł���B

	 mesh.eachNodeWithId{ |node,id|
	   x = node[0]
	   y = node[1]
	   z = node[2]
	 }

	�̂悤�Ɏg���B

--- eachNodeXYZ
	�ߓ_�� XYZ ���W��^����C�e���[�^�ł���B

	 mesh.eachNodeXYZ{ |x,y,z|
	   sq = x*x + y*y + z*z
	 }

	�̂悤�Ɏg���B
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
	bodyId �Ɋ܂܂��v�f���Q�Ƃ���ߓ_Id�̃C�e���[�^�ł���B
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
	�̈�I�u�W�F�N�g�Ɋ܂܂��ߓ_Id�̃C�e���[�^�ł���B
	�����̂� Region �C���X�^���X��
	region = RevocapMesh::BoxRegion.new(x0,y0,z0,x1,y1,z1) �̂悤�ɍ�邱�Ƃ��ł���B
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
	�擪�̐ߓ_Id�� initid �ŗ^���Đߓ_Id��A��������B
	�Q�Ƃ��Ă���v�f�╨���ʂ�����Ύ����I�ɕϊ�����B
=end
---------------------------------------------------------------------------*/
		void nodeIdDefragmentation(nodeIdType initId=0);
/**--------------------------------------------------------------------------
=begin
--- replaceNodeId(oldId,newId)
	���_Id��u��������B
	�Q�Ƃ��Ă���v�f�╨���ʂ�����Ύ����I�ɕϊ�����B
=end
---------------------------------------------------------------------------*/
		bool replaceNodeId(kmb::nodeIdType oldId, kmb::nodeIdType newId);
/**--------------------------------------------------------------------------
=begin
--- convertAllNodes(matrix)
	Matrix4x4 �� matrix ��^���āA���ׂĂ̐ߓ_�����W�ϊ�����B
	���Ƃ��� y-up �̍��W�n�� z-up �ɕς���ɂ�
	 convertAllNodes(
		RevocapMesh::Matrix4x4.new( 1.0, 0.0, 0.0, 0.0,
									0.0, 0.0, -1.0, 0.0,
									0.0, 1.0, 0.0, 0.0,
									0.0, 0.0, 0.0, 1.0 ) )
	�Ƃ���΂悢�B
--- translateAllNodes( x, y, z )
	���ׂĂ̐ߓ_�𕽍s�ړ�����B
	 convertAllNodes(
		RevocapMesh::Matrix4x4.new( 1.0, 0.0, 0.0, x,
									0.0, 1.0, 0.0, y,
									0.0, 0.0, 1.0, z,
									0.0, 0.0, 0.0, 1.0 ) )
	�Ɠ���
--- scaleAllNodes( r )
	���ׂĂ̐ߓ_�̍��W���X�P�[���ϊ�����B
	 convertAllNodes(
		RevocapMesh::Matrix4x4.new( r,   0.0, 0.0, 0.0,
									0.0, r,   0.0, 0.0,
									0.0, 0.0, r,   0.0,
									0.0, 0.0, 0.0, 1.0 ) )
	�Ɠ���
=end
---------------------------------------------------------------------------*/
		void	convertAllNodes( kmb::Matrix4x4& mat );
		void	translateAllNodes( double x,double y,double z );
		void	scaleAllNodes( double r );
/**--------------------------------------------------------------------------
=begin
--- deleteUselessNodes()
	�v�f�Ɏg���Ă��Ȃ����_�����ׂč폜����B
	�������֘A���镨���ʂɊւ��Ă̓`�F�b�N���Ă��Ȃ��B
=end
---------------------------------------------------------------------------*/
		int		deleteUselessNodes(void);
/**--------------------------------------------------------------------------
=begin
--- clearNodes()
	���_�����ׂč폜����B�������֘A����v�f�╨���ʂɊւ��Ă�
	�폜���Ȃ��B
=end
---------------------------------------------------------------------------*/
		void	clearNodes(void);


/**--------------------------------------------------------------------
=begin
==�v�f�Ǘ��iMeshDB�� �g���@�\�j

MeshData �Œ�`����Ă��郁�\�b�h�͏���

RevocapMesh �ł̍ޗ������f�[�^�̊Ǘ����@

�\���o�ɂ���Ă����͈قȂ邽�߁A���ۂ̃v���|�X�g�Ɏ�������ꍇ��
�\���o�̎d�l�ɍ��킹��B

(1) RevocapMesh �� Data �� { name => value } �� Hash �� Value �Ƃ��ēo�^���āA
    �v�f�O���[�v�̔ԍ��� Id �ɒǉ�����iFrontSTR �̏ꍇ�j
(2) �O���ɍޗ������l�̃f�[�^�x�[�X�����ARevocapMesh �ɂ�
    { �v�f�O���[�vId => �O���f�[�^�x�[�X�ɂ�����ޗ������l��Id }
    �� Data �Ƃ��ēo�^����iFrontFlow/red �̏ꍇ�j
=end
-----------------------------------------------------------------------*/

/**-------------------------------------------------------------------------
=begin
--- deleteElement(bodyId,elementId)
	bodyId �� Body �Ɋ܂܂��v�fId�� elementId �̗v�f���폜����B
	�폜�ł����ꍇ�͂��̗v�fId��Ԃ��A���s�����Ƃ���-1��Ԃ��B
=end
----------------------------------------------------------------------------*/
	kmb::elementIdType deleteElement(kmb::bodyIdType bodyId,kmb::elementIdType elementId);

/**-------------------------------------------------------------------------
=begin
--- moveElement(bodyId0,bodyId1,elementId)
	bodyId0 �� Body �Ɋ܂܂��v�fId elementId �̗v�f��
	bodyId1 �� Body �Ɉړ�����B
=end
----------------------------------------------------------------------------*/
	bool moveElement(kmb::bodyIdType bodyId0,kmb::bodyIdType bodyId1,kmb::elementIdType elementId);
/**-------------------------------------------------------------------------
=begin
--- getBodyId(elementId)
	�^����ꂽ�v�f���ǂ� Body �Ɋ܂܂�邩��Ԃ��B
=end
---------------------------------------------------------------------------*/
	kmb::bodyIdType getBodyId(kmb::elementIdType elementID);

/**-------------------------------------------------------------------------
=begin
--- getMaxElementId()
	�g���Ă���v�fID�̍ő�l��Ԃ��B
=end
---------------------------------------------------------------------------*/
	kmb::elementIdType	getMaxElementId();

/**-------------------------------------------------------------------------
=begin
--- cloneBody(bodyId)
--- cloneReverseBody(bodyId)
--- cloneBodyWithNode(bodyId)
--- cloneReverseBodyWithNode(bodyId)
    bodyId �� Body �Ɋ܂܂��v�f�̂��ׂĂ𕡐�����
    ���̗v�f���ׂĂ���Ȃ镡�����ꂽ Body ��Id��Ԃ��B
=end
---------------------------------------------------------------------------*/
	kmb::bodyIdType cloneBody(kmb::bodyIdType bodyId);
	kmb::bodyIdType cloneReverseBody(kmb::bodyIdType bodyId);
	kmb::bodyIdType cloneBodyWithNode(kmb::bodyIdType bodyId);
	kmb::bodyIdType cloneReverseBodyWithNode(kmb::bodyIdType bodyId);

/**---------------------------------------------------------------------
=begin
--- getBoundingBox(bodyId)
	Body �� BoundingBox �� BoundingBox �N���X�̃C���X�^���X�ŕԂ��B
	2�����̏ꍇ�� maxZ minZ �͂Ƃ��� 0�B

--- getBoundingBox()
	Mesh �ɓo�^����Ă���ߓ_�S�̂� BoundingBox �� BoundingBox �N���X�̃C���X�^���X�ŕԂ��B
	2�����̏ꍇ�� maxZ minZ �͂Ƃ��� 0�B

--- updateBoundingBox(bodyId)
	mergeBody moveElement �Ȃǂ����s�������ƂŁABoundingBox ���X�V����

=end
------------------------------------------------------------------------*/
	void updateBoundingBox(bodyIdType bodyId);
	const kmb::BoundingBox getBoundingBox(void) const;
	const kmb::BoundingBox getBoundingBox(kmb::bodyIdType bodyId) const;

/**---------------------------------------------------------------------------
=begin
--- duplicateNodeIdOfBody(bodyId,coupleName=nil)

--- replaceNodeIdOfElement(bodyId,elementId,oldNodeId,newNodeId)
	�v�fId elementId �̗v�f�̐ߓ_�z��� oldNodeId ��
	newNodeId �ɒu��������B

--- replaceNodeIdOfBody(bodyId,oldNodeId,newNodeId)
	bodyId �� Body �̂��ׂĂ̗v�f�ɑ΂��Đߓ_�z��� oldNodeId ��
	newNodeId �ɒu��������B

--- replaceNodeIdOfBody(bodyId,nodeMapper)
	bodyId �� Body �̂��ׂĂ̗v�f�ɑ΂���
	�ߓ_�z��� nodeMapper �ɏ]���Ēu��������

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
	bodyID �� Body �̗v�f�̃C�e���[�^��^����B
	�C�e���[�^�̓����̃u���b�N�ł́A�v�f�� getElement �Ɠ����`���œ�����B

--- eachElementWithId(bodyID)
	bodyID �� Body �̗v�f�Ɨv�fID�̃C�e���[�^��^����B

--- eachElementWithType(bodyID)
	bodyID �� Body �̗v�f�̐ߓ_�z��Ɨv�f�^�C�v�̑g�ŏo�͂���C�e���[�^��^����B

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
	�S�Ă̒��_���̈�I�u�W�F�N�g�Ɋ܂܂��v�fId�̃C�e���[�^�ł���B
	�����̂� Region �C���X�^���X��
	region = RevocapMesh::BoxRegion.new(x0,y0,z0,x1,y1,z1) �̂悤�ɍ�邱�Ƃ��ł���B
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
	2���� Body �Ɋ܂܂��ӂ̌���Ԃ��BEuler �W���̌v�Z�Ɏg���B

--- getNodeCountOfBody(bodyID)
	Body �Ɋ܂܂�钸�_�̌���Ԃ��BEuler �W���̌v�Z�Ɏg���B

=end
-----------------------------------------------------------------------------*/
	int getEdgeCountOfBody(kmb::bodyIdType bodyID);
	int getNodeCountOfBody(kmb::bodyIdType bodyID);


/**-----------------------------------------------------------------------
=begin
==�􉽏���
--- faceToElement( [elementID,localID] )
	�v�f Id �ƋǏ��� Id ���狫�E�v�f���擾����B
	�Ⴆ�� elementID �Ŏl�ʑ̗̂v�fID�AlocalID �� 0 ��^�����ꍇ�́A
	�l�ʑ̂̂S�̋��E�O�p�`�̂����� 0 �Ԗڂ̂��̂� getElement �œ�����̂�
	�����`���œ�����B�����œ���ꂽ�v�f�� MeshDB �ɂ͓o�^����Ă��Ȃ��B
	�]���� elementID �͎����Ă��Ȃ��B
--- elementToFace( elementType, ary, bodyId )
	"TRIANGLE",[0,1,2] �`���ŗ^���� bodyId �� Body ����A
	faceToElement �������ɂ��̗v�f�ɂȂ�悤�� face �����߂�B
	NeighborsInfo ���쐬����Ă��鎞�ɂ́A�����p���č���������B
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
	�v�f�O���[�v bodyId �Ɋ܂܂��O�p�`�ȊO�A�l�ʑ̈ȊO�̗v�f�𕪊�����
	�O�p�`�܂��͎l�ʑ̂ɂ���B
=end
--------------------------------------------------------------------------*/
	void triangulation(kmb::bodyIdType bodyId);
/**-----------------------------------------------------------------------
=begin
--- getDistance(n0,n1)
	i0 �Ԗڂ̐ߓ_�� i1 �Ԗڂ̐ߓ_�̋�����Ԃ��B
=end
--------------------------------------------------------------------------*/
	double getDistance(kmb::nodeIdType n0, kmb::nodeIdType n1) const;
/**-----------------------------------------------------------------------
=begin
--- getDistanceToElement(x,y,z,elementId,bodyId=-1)
	(x,y,z) ���� elementId �̗v�f�ւ̋�����Ԃ��B
=end
--------------------------------------------------------------------------*/
	double getDistanceToElement(double x,double y,double z,kmb::elementIdType elementId0,kmb::bodyIdType bodyId=kmb::Body::nullBodyId) const;
/**-----------------------------------------------------------------------
=begin
--- getEdgeVector(i0,i1)
	i0 �Ԗڂ̒��_���� i1 �Ԗڂ̒��_�ւ̃x�N�g����Ԃ��B
=end
--------------------------------------------------------------------------*/
	Vector3D getEdgeVector(kmb::nodeIdType i0,kmb::nodeIdType i1) const;
/**-------------------------------------------------------------------------
=begin
--- getNormalVector(elementId,bodyId=-1)
	���ʗv�fID elementID �̗v�f�̖@���x�N�g����
	Vector3D �`���ŕԂ��B
	���ʗv�f����Ȃ��Ƃ��̓[���x�N�g����Ԃ��B

	 normal = mesh.getNormalVector(elementId)
	 puts normal.x
	 puts normal.y
	 puts normal.z

	�̂悤�Ɏg��

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
	elementId �̗v�f�̑̐ς����߂�B
	���̌����ɂȂ��Ă�����̐ς����ɂȂ�B
	bodyId �͏ȗ��\�����ȗ�����Ə����x���Ȃ�B
--- getElementArea(elementId,bodyId=-1)
	elementId ��2�����v�f�̖ʐς����߂�B
	bodyId �͏ȗ��\�����ȗ�����Ə����x���Ȃ�B
--- getVolume(bodyId)
	bodyId �̗̈�̑̐ς����߂�B
--- getArea(bodyId)
	bodyId �̗̈�̖ʐς����߂�B
=end
-----------------------------------------------------------------------------*/
	double getElementArea(elementIdType elementID,kmb::bodyIdType bodyID=kmb::Body::nullBodyId) const;
	double getElementVolume(elementIdType elementID,kmb::bodyIdType bodyID=kmb::Body::nullBodyId) const;
	double getArea(kmb::bodyIdType bodyId) const;
	double getVolume(kmb::bodyIdType bodyId) const;
/**----------------------------------------------------------------------------
=begin
--- getBoundaryCount(elementId,bodyId=-1)
	�v�f�O���[�v bodyId �� elementId �̗v�f�̋��E�̌���Ԃ��B
	���E�Ƃ͗��̗v�f�Ȃ������͂ޕ��ʗv�f�A���ʗv�f�Ȃ������͂ސ����v�f�̂��Ƃ������B
	�ߖT�̌��͕K�����̐��ȉ��ł���B
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
	4�ʑ̂̃A�X�y�N�g���Ԃ��B
	�A�X�y�N�g��Ƃ́A�O�ڋ��̔��a / (���ڋ��̔��a*3) �Œ�`�������̂ŁA
	��4�ʑ̂ōŏ��l 1 �����B�l�� 1 �ɋ߂��قǌ`�󂪐��l�ʑ̂ɋ߂��Ƃ����B
	4�ʑ̈ȊO�̂��̂ɂ��Ă� 0 ��Ԃ��B
	bodyId ���ȗ�����Ə����x���Ȃ�B
=end
---------------------------------------------------------------------------*/

	double getAspectRatio(elementIdType elementID,kmb::bodyIdType bodyID=kmb::Body::nullBodyId);

/**-------------------------------------------------------------------------
=begin
--- getNaturalCoordinates(elementId,x,y,z,bodyId=-1)
	�������W�n��v�f�����R���W�n�ɕϊ�����
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
	�v�f�����R���W���畨�����W�n��
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
	�`��֐��̒l���擾����
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
	�v�f�܂��͗̈悷�ׂĂ̗v�f�ɂ��āA
	�t�����ɂ���B
=end
-----------------------------------------------------------------------------*/
	bool reverseElement(kmb::elementIdType elementId,kmb::bodyIdType bodyId=kmb::Body::nullBodyId);
	void reverseBody(kmb::bodyIdType bodyId);


/**-----------------------------------------------------------------------
=begin
==��������

MeshDB �ł� Octree �@��p���������������\�ł���BOctree �̃L���b�V���𐶐�����
generateNodeSearchCache �����s������� getNearestNode(x,y,z) �����s����ƍł��߂�
�ߓ_�̐ߓ_Id�Ƃ��̐ߓ_�܂ł̋����̃y�A��Ԃ�

--- generateNodeSearchCache
	�o�^����Ă���ߓ_���ׂĂɂ��Č����p�̃L���b�V���𐶐�����
--- generateNodeSearchCacheOfBody( bodyId )
	bodyId �� Body �Ɋ܂܂�Ă���ߓ_���ׂĂɂ��Č����p�̃L���b�V���𐶐�����
--- generateNodeSearchCacheOfData( name )
	name �� Data �Ɋ܂܂�Ă���ߓ_���ׂĂɂ��Č����p�̃L���b�V���𐶐�����
--- appendNodeSearchCache( nodeId )
	�����p�̃L���b�V���� nodeId �̐ߓ_��ǉ�����
--- clearNodeSearchCache
	�ߓ_�����p�̃L���b�V�����N���A����
--- getNearestNode(x,y,z)
	(x,y,z) �Ɉ�ԋ߂��ߓ_����������B[nearestNodeId,distance]
	�̌`���ŕԂ��B�L���b�V������������Ă��Ȃ��Ƃ��ɂ́A
	�S�_�����@�Ō�������B
--- getNodesInRegion(region)
	�����̂�^���āA�����Ɋ܂܂��ߓ_����������B

	 getNodesInRegion(RevocapMesh::BoxRegion.new(0.0,0.0,0.0,1.0,1.0,1.0))

	�̂悤�Ɏg��
--- generateElementSearchCache(bodyId,cacheType="octree")
	bodyId �ɓo�^����Ă���v�f��v�f�����p�̃L���b�V���ɓo�^����
	cacheType �� octree �܂��� bucket
--- clearElementSearchCache()
	�v�f�����p�̃L���b�V�����N���A����
--- searchElement(bodyId,x,y,z,tolerance=0.0)
	bodyId �̒��� (x,y,z) ���܂܂�Ă���i�܂��͂����Ƃ��߂��ɂ���j�v�f����������B
	tolerance �͋�����臒l��^����Bsolid �v�f�̎� tolerance = 0.0 �Ƃ���ƁA�����ɂ��邱�Ƃ��ۏ؂����B
#	solid �v�f�̏ꍇ�Atolerance ��^����ƁA�����O������̎��̌`��֐��̒l�ɗ]�T����������
	�߂�l�� �v�fId ������Ȃ���� nil
--- searchElementInData(name,x,y,z,tolerance=0.0)
	Data �̒��� (x,y,z) ���܂܂�Ă���v�f����������B
	tolerance ��^����ƁA�����O������̎��̌`��֐��̒l�ɗ]�T����������
	�߂�l�� �v�fID ������Ȃ���� nil
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
	�ߓ_ ID ��^���āA�����ȊO�̍ł��߂��ߓ_ ID �Ƌ����̑g�� Array ��Ԃ��B

--- getNearNodes(x,y,z,tolerance)
	���W�l�� tolerance ��^���āA���̋����ȉ��̐ߓ_�� ID �� Array ��Ԃ��B

--- getNearNodes(id,tolerance)
	�ߓ_ ID �� tolerance ��^���āA���̋����ȉ��̎����ȊO�̐ߓ_�� ID �� Array ��Ԃ��B

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
==�ߖT����

MeshDB�̋ߖT�����ł�
* �ߓ_�ɑ΂��A���̐ߓ_���܂ޗv�f���擾
* �ߓ_�ɑ΂��A���̐ߓ_�Ɛڑ����Ă���ߓ_�̎擾
* �v�f�ɑ΂��A���̗v�f�ɐڂ��Ă���v�f�̎擾
���s�����Ƃ��ł���B�����͗v�f�̎�ނɂ�炸�A�\���b�h���b�V���A�T�[�t�F�X���b�V��
�ɂ�����炸���s���邱�Ƃ��ł���B

�܂��A���̏������s�����߂̋ߖT���̐����ƁA����ŗ��p������������
������s�����Ƃ��ł��邽�߁A�K�v�̂Ȃ����ɂ͖��ʂȃ������͎g��Ȃ��B

���p�����Ƃ���
* �v�f�ɑ΂��Đڂ��Ă���v�f���擾���邱�ƂŁA�O�Ɍ��ɂȂ��Ă���ʂ����v�f�̌���
* �ߓ_�̎���̗v�f���擾���A���̖@���x�N�g���̕��ς����߂�
�Ȃǂ�e�ՂɎ������邱�Ƃ��ł���B

--- clearNeighborsInfo()
	�L���b�V���ɋL�������ߖT�����N���A����B

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
	�ߖT�v�Z�ɕK�v�ȏ��� bodyId �̃{�f�B�Ɋ܂܂��v�f����
	�������ăL���b�V���ɕۑ�����B
	���̃��\�b�h�����s���

	* getNeighborNodes(nodeId)
	* getSurroundingElements(nodeId)
	* getSurroundingCount(nodeId)
	* getNeighborElements(elementId)
	* getAdjacentFace(face)

	���L���ɂȂ�
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
	nodeId �̐ߓ_�ԍ��̐ߓ_�ɐڂ��Ă���ߓ_�ԍ���
	�z��� Array �ŕԂ��B
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
	nodeId �̐ߓ_�ԍ��̐ߓ_���܂ޗv�fId��
	�z��� Array �ŕԂ��B
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
	nodeId �̐ߓ_�ԍ��̐ߓ_���܂ޗv�f�̌���Ԃ��B
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
	�v�f�ԍ� elementId �̗v�f��
	�ڂ���v�f�ԍ��̔z��� Array �ŕԂ��B
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
	Face (�v�f�ԍ��Ɠ����ʔԍ��̑g) ��^���āA���̗v�f�̓����ʔԍ��Őڂ��� Face ��Ԃ��B
	�Ⴆ�Ηv�f�ԍ� 100 �̎l�ʑ̂̓����ʔԍ� 1 �̖ʂ�^�������A
	���̖ʂƗv�f�ԍ� 200 �̎l�ʑ̂̓����ʔԍ� 2 ���ڂ��Ă��鎞�� [200,2] ��Ԃ��B
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
==�v�f�Ԋ֌W
--- getFaces(elementID,bodyID=-1)
	�v�fID elementID �̗v�f�̖ʂ��\�����钸�_�z���2�����z��ŕԂ��B
	�Ⴆ�� elementID = 0 �̗v�f��
	 ["TETRAHEDRON",100,101,102,103]
	���Ƃ���ƁAgetFaces(0) �̌��ʂ�
	 [ [101,102,103], [100,103,102], [100,101,103], [100,102,101] ]
	�ƂȂ�
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
	���̗v�f elementID �����_�z�� ary ��\�ʂƂ��Ď����ǂ����B
	�����������̕\�ʂ������1�A
	�t�����̂̕\�ʂ������-1�A
	�\�ʂ��������0��Ԃ��B
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
	�����{�f�B�Ɋ܂܂��v�fID e0 �� e1 �����L���Ă���v�f������΁A
	����� getElement �Ɠ����`���ŕԂ��B
	���̗v�f�� MeshDB �ɓo�^����Ă���Ƃ͌���Ȃ��B
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
	�v�fID e0 �Ɨv�fID e1 �̊֌W�𕶎���ŕԂ��B
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
==���b�V�������E�ϊ�
--- mergeBody(bodyIds)
	������ Body ���܂Ƃ߂Ĉ�� Body �ɂ���B
	���Ƃ� Body ����͗v�f���폜����B
	�܂Ƃ߂��V���� Body �� BodyId ��Ԃ��B

--- mergeBody(appendId,bodyId,moveFlag=true)
	appendId �� Body �� bodyId �� Body �Ɍ�������B
	moveFlag = true �̂Ƃ��́AappendId �� Body ����v�f���폜����B
	moveFlag = false �̂Ƃ��́AappendId �̗v�f�͎c���āA�v�f Id ��V���ɂ��Ȃ����� bodyId �� Body �ɓo�^����
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
	Body �����ׂĂP���v�f�ɕϊ�����B
=end
---------------------------------------------------------------------------*/
	kmb::bodyIdType convertToLinearBody( kmb::bodyIdType bodyId );

/**-------------------------------------------------------------------------
=begin
--- importBody(mesh,bodyId,nodeMapper=nil,ctype="SLAVE")
	�ʂ� Mesh �� Body ��ǉ�����B�K�v�Ȑߓ_�������ɒǉ����A�e�v�f�̐ߓ_�z���
	�����I�ɏ���������B
	nodeMapper �ɕ������^�������ɂ́A�����̐ߓ_�̑Ή��� NodeVariable ���[�h��
	Data �Ƃ��ċL������B���łɋL������Ă��� Data �����鎞�́A
	������X�V���A�v�f��ǉ�����Ƃ������̐ߓ_�̑Ή���p����B
	ctype �̈����Őߓ_�̑Ή����L������C���X�^���X���w�肷��B
	MASTER �� self�ASLAVE �� mesh�ABOTH �ŗ����ANone �ł� nodeMapper ��
	�^���Ă��Ă��L�����Ȃ��B

--- importBodyWithNodeMatching(mesh,bodyId,tolerance,nodeMapper=nil)
	�ʂ� Mesh �� Body ��ǉ�����B�K�v�Ȑߓ_�� tolerance �ȉ��ɂ��łɐߓ_���Ȃ����𒲂ׂāA
	���݂���΂�����g���A�Ȃ���Βǉ�����B�e�v�f�̐ߓ_�z��͎����I�ɏ���������B
	nodeMapper �ɕ������^�������ɂ́A������ mesh �ɐߓ_�̑Ή��� NodeVariable ���[�h��
	Data �Ƃ��ċL������B���łɋL������Ă��� Data �����鎞�́A
	������X�V���A�v�f��ǉ�����Ƃ������̐ߓ_�̑Ή���p����B

--- importAllBody(mesh)
	mesh �̂��ׂĂ� Body ��ǉ�����B
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
	Mesh �f�[�^�Ɋ܂܂�鋫�E�����A�ޗ������Ȃǂ̃v�����ɂ����� MeshSpec �̖��O�A
	����у|�X�g���̌v�Z���ʂ̃m�[�h�ɂ�����l��ێ��������O�̔z���Ԃ��B
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
	name �Œ�`���ꂽ Data �� Id �����ւ���
=end
---------------------------------------------------------------------------*/
	bool replaceIdOfData(const char* name,kmb::idType oldID,kmb::idType newID,const char* stype=NULL);
/**-------------------------------------------------------------------------
=begin
--- convertData(org,conv,orgstype=nil,convstype=nil)
	org �̃f�[�^�� conv �ɕϊ�����B
	conv �̃f�[�^�͂��炩���ߍ쐬���Ă������ƁB
	GROUP �n�̃f�[�^�ɂ��ẮAValueType ����v���鎞�ɒl���R�s�[����B
	�Ή��ς݁F
	org : NodeGroup => conv : FaceGroup
	 �ʏ�̂��ׂĂ̐ߓ_�� org �f�[�^�Ɋ܂܂�Ă���ꍇ�� conv �ɓo�^����
	org : NodeGroup => conv : ElementGroup
	 �v�f�̂��ׂĂ̐ߓ_�� org �f�[�^�Ɋ܂܂�Ă���ꍇ�� conv �ɓo�^����
	org : FaceGroup => conv : NodeGroup
	 �ʏ�̂��ׂĂ̐ߓ_�� conv �ɓo�^����
	org : ElementGroup => conv : NodeGroup
	 �v�f��̂��ׂĂ̐ߓ_�� conv �ɓo�^����
=end
---------------------------------------------------------------------------*/
	int convertData(const char* org, const char* conv, const char* orgstype=NULL,const char* convstype=NULL);
	int convertBodyToData(kmb::bodyIdType bodyId, const char* name,const char* stype=NULL);
/**-------------------------------------------------------------------------
=begin
--- getElementCountOfData(name)
	name �̃O���[�v�Ɋ܂܂��v�f�����o�͂���
=end
---------------------------------------------------------------------------*/
	int getElementCountOfData(const char* name,const char* stype=NULL);
/**-------------------------------------------------------------------------
=begin
--- getNodeCountOfData(name)
	name �̃O���[�v�Ɋ܂܂��ߓ_�����o�͂���
=end
---------------------------------------------------------------------------*/
	int getNodeCountOfData(const char* name,const char* stype=NULL);
/**-------------------------------------------------------------------------
=begin
--- eachNodeIdOfData(name,stype=nil)
	name �̃O���[�v�Ɋ܂܂��ߓ_��Id�����o�� iterator
	NodeVariable �܂��� NodeGroup �̏ꍇ�͂��̂��̂� iterator
	ElementVariable �܂��� ElementGroup �̏ꍇ�͗v�f�Ɋ܂܂��ߓ_�� iterator
	FaceVariable �܂��� FaceGroup �̏ꍇ�� FACE �Ɋ܂܂��ߓ_�� iterator
	BodyVariable �܂��� BodyGroup �̏ꍇ�� BODY �Ɋ܂܂��ߓ_�� iterator
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
	�����l�܂��̓v�������̖��O��^���āA�o�^����Ă���ߓ_��v�f��
	BoundingBox ��Ԃ��B
=end
---------------------------------------------------------------------------*/
	const kmb::BoundingBox getBoundingBoxOfData(const char* key,const char* stype=NULL) const;
/**--------------------------------------------------------------------------
=begin
--- getUniqueDataName(prefix,num=0)
	prefixnum �i�K�v�Ȃ� num �̒l�𑝂₵�āj�`���̎g���Ă��Ȃ����O���擾����B
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
==�f�[�^�����E���
--- getNearestValue(name,value,stype=nil)
	�����l��^���āAname �O���[�v�ɓo�^����Ă�����̂̒�����ł��߂��l������ ID ��Ԃ��B
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
	�����l�̖��O��^���āA��Ԃ��ꂽ�l��Ԃ��B
	tolerance �͌`��֐��̒l�œ����ɂ��邩�ǂ����𔻒肷��臒l��^����B
	�A���ʂł̕����ʂ����o���ꍇ�ȂǈȊO�� 0.0 �ō\��Ȃ��B
	Multi �̏ꍇ�� setTargetData �ŗ^����ꂽ�����̕����ʂɂ��ĕ�Ԓl�����߂�B

--- getInterpolatedValueInData(name,target,x,y,z,tolerance=0.0,stype=nil)
--- getInterpolatedMultiValuesInData(names,target,x,y,z,tolerance=0.0,stype=nil)
	�����l�̖��O name ��^���āA��Ԃ��ꂽ�l��Ԃ��B
	��Ԃ����v�f�� target �ŗ^����ꂽ�O���[�v�݂̂��猟������B
	tolerance �͌`��֐��̒l�œ����ɂ��邩�ǂ����𔻒肷��臒l��^����B
	�A���ʂł̕����ʂ����o���ꍇ�ȂǈȊO�� 0.0 �ł��܂��܂���B
	Multi �̏ꍇ�� setTargetData �ŗ^����ꂽ�����̕����ʂɂ��ĕ�Ԓl�����߂�B

	�A���ʂ̑g (A,B) ���^����ꂽ���ɁA�� A �� B ���畨���ʂ��Ԃ��ė^����ꍇ�ɁA
	target �� B ���w�肷��BB �� ElementGroup �܂��� FaceGroup �� Data �ł���
	�K�v������B
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
	�����l�̖��O��^���āA�o�^����Ă���l�̍ŏ��ő��Ԃ��B
	�x�N�g���l�ɂ��ẮA�����ł͐������Ƃɍŏ��ő�����߂�B
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
	�f�[�^�̍ő�ŏ��l�����߂�B
	�x�N�g���l�ɂ��ẮA��Βl�̍ŏ��ő�����߂邱�Ƃɒ��ӂ���B
	�߂�l�� [minVal, minId, maxVal, maxId] �̌`��
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
==���̑�
--- calcMeshProperty(name,bodyId=-1)
	���b�V���̐����Ȃǂ��v�Z���ĕ����ʂɓo�^����B
	 name mode value
	 "NormalVectorOnNode" NodeVariable Vector3 �ߓ_�̎���̗v�f�̖@���x�N�g���̕��ϒl
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
