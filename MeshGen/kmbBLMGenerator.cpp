/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : BLMGenerator                                            #
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
#include "MeshGen/kmbBLMGenerator.h"

#include "Geometry/kmbGeometry3D.h"
#include "MeshDB/kmbMeshData.h"
#include "MeshDB/kmbQuad.h"
#include "MeshDB/kmbNodeNeighborInfo.h"
#include "MeshDB/kmbNodeNeighborFaceInfo.h"
#include "MeshDB/kmbElementContainer.h"
#include "MeshDB/kmbElementContainerMap.h"
#include "MeshDB/kmbElementEvaluator.h"
#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElement.h"
#include "MeshDB/kmbBoundaryExtractor.h"

#include <set>
#include <map>
#include <iostream>

kmb::BLMGenerator::BLMGenerator(void)
: layerNum(0)
, layerThick(NULL)
, mesh(NULL)
, layerNodes()
, nodeMapper()
{
}

kmb::BLMGenerator::BLMGenerator(MeshData* mesh)
{
	this->mesh = mesh;
	layerNum = 1;
	layerThick = new double[1];
	layerThick[0] = 1.0;
}

kmb::BLMGenerator::BLMGenerator(MeshData* mesh,int layerNum,double* layerThick)
{
	this->mesh = mesh;
	this->layerNum = layerNum;
	this->layerThick = layerThick;
}

kmb::BLMGenerator::~BLMGenerator(void)
{
	clearLayerNodes();
	if(layerThick){
		delete[] layerThick;
		this->layerThick = NULL;
	}
}

void
kmb::BLMGenerator::clearLayerNodes()
{
	nodeMapper.clear();
	std::map< std::pair<kmb::nodeIdType, kmb::nodeIdType>, kmb::nodeIdType*>::iterator layerIter = this->layerNodes.begin();
	while( layerIter != this->layerNodes.end() ){
		kmb::nodeIdType* a = layerIter->second;
		if( a ){
			delete[] a;
			a = NULL;
		}
		++layerIter;
	}
	layerNodes.clear();
}

void
kmb::BLMGenerator::setLayer(int layerNum,double* thicks)
{
	this->layerNum = layerNum;
	if( this->layerThick ){
		delete[] this->layerThick;
		this->layerThick = NULL;
	}
	this->layerThick = new double[this->layerNum];
	for(int i=0;i<this->layerNum;++i){
		this->layerThick[i] = thicks[i];
	}
}

void
kmb::BLMGenerator::setMesh(kmb::MeshData* mesh)
{
	this->mesh = mesh;
}

kmb::nodeIdType
kmb::BLMGenerator::getDuplicatedNodeId(kmb::nodeIdType nodeId)
{
	std::map< kmb::nodeIdType, kmb::nodeIdType >::const_iterator nIter = nodeMapper.find(nodeId);
	if( nIter != nodeMapper.end() ){
		return nIter->second;
	}else{
		kmb::Point3D pt;
		mesh->getNode(nodeId,pt);
		kmb::nodeIdType duplicatedId = mesh->addNode(pt.x(),pt.y(),pt.z());
		nodeMapper.insert( std::pair<kmb::nodeIdType,kmb::nodeIdType>(nodeId,duplicatedId) );
		return duplicatedId;
	}
}

kmb::bodyIdType
kmb::BLMGenerator::extrude(kmb::bodyIdType &boundaryId)
{
	// ����
	kmb::ElementContainer* boundBody = this->mesh->getBodyPtr(boundaryId);
	if( boundBody == NULL ){
		return kmb::Body::nullBodyId;
	}
	kmb::ElementEvaluator evaluator( this->mesh->getNodes() );

	// boundBody �Ɋ܂܂��ߓ_���Q�d��
	// �V���� boundaryId �ɐV�ߓ_����Ȃ�ʂ�ǉ�
	kmb::nodeIdType nodeTable[8];
	boundaryId = mesh->beginElement( boundBody->getCount() );
	kmb::ElementContainer::iterator bIter = boundBody->begin();
	while( !bIter.isFinished() ){
		int num = bIter.getNodeCount();
		for(int i=0;i<num;++i){
			nodeTable[i] = getDuplicatedNodeId( bIter[i] );
		}
		mesh->addElement( bIter.getType(), nodeTable );
		++bIter;
	}
	mesh->endElement();

	// �ߖT���̍쐬
	kmb::NodeNeighborInfo neighborInfo;
	neighborInfo.appendCoboundary( boundBody );

	// �@�����v�Z���� => �I���W�i���\��
	// �_�𓮂���     => ���������\��
	// ���̓_�̎���̗v�f�𒲂ׂ�
	// ���̖@�������Ń_�~�[�ߓ_���O���ɂ��炷
	// ���̐ߓ_�͂��̂܂܂̏ꏊ�ɂ����Ă���
	kmb::Node node;
	std::map<kmb::nodeIdType,kmb::nodeIdType>::iterator nIter = nodeMapper.begin();
	while( nIter != nodeMapper.end() )
	{
		kmb::nodeIdType nodeId = nIter->first;
		kmb::nodeIdType dupnodeId = nIter->second;
		// ���ӗv�f�����o���āA�@���x�N�g�������߂�
		kmb::Vector3D normVect(0.0,0.0,0.0);
		kmb::Vector3D v;
		kmb::NodeNeighbor::iterator neiIter = neighborInfo.beginIteratorAt( nodeId );
		kmb::NodeNeighbor::iterator neiEnd = neighborInfo.endIteratorAt( nodeId );
		while( neiIter != neiEnd )
		{
			kmb::elementIdType eId = neiIter->second;
			kmb::ElementContainer::iterator element = boundBody->find( eId );
			if( !element.isFinished() &&
				evaluator.getNormalVector( element, v ) )
			{
				normVect += v;
			}
			++neiIter;
		}
		normVect.normalize();
		this->mesh->getNode(nodeId,node);
		// �����̃_�~�[���_�̍X�V
		// �@���x�N�g���ɓ�����
		kmb::Point3D point = node + normVect.scalar(layerThick[layerNum-1]);
		mesh->updateNode( point.x(), point.y(), point.z(), dupnodeId );
		++nIter;
	}
	neighborInfo.clear();

	// �I���W�i���\�ʂƕ��������_�̏�񂩂�w���b�V���̍쐬
	kmb::bodyIdType layerId = this->mesh->beginElement( boundBody->getCount() * this->layerNum );
	{
		kmb::nodeIdType oldNodeId[8];
		kmb::ElementContainer::iterator bIter = boundBody->begin();
		while( !bIter.isFinished() ){
			int len = bIter.getNodeCount();
			for(int i=0;i<len;++i){
				oldNodeId[i] = bIter[i];
			}
			appendLayerElements( bIter.getType(), oldNodeId, false );
			++bIter;
		}
	}
	this->mesh->endElement();

	// ���������
	clearLayerNodes();
	return layerId;
}

kmb::bodyIdType
kmb::BLMGenerator::intrude(kmb::bodyIdType parentId,kmb::bodyIdType &boundaryId)
{
	// (1) body �̐ߓ_�̎���̋ߖT���A���E���
	// (2) ���� body �̐ߓ_���d��
	// (3) �e�v�f�Ō��̐ߓ_���܂ނ��̂�V�ߓ_�ɒu��������
	// (4) �ߓ_�̖@���x�N�g�������߂āA���̐ߓ_���O���ɓ�����
	// (5) ���̐ߓ_�ƐV�ߓ_�̊Ԃ�v�f�Ŗ��߂�

	// ����
	kmb::ElementContainer* boundBody = mesh->getBodyPtr( boundaryId );
	if( boundBody == NULL ){
		return false;
	}
	kmb::ElementContainer* parentBody = mesh->getBodyPtr( parentId );
	if( parentBody == NULL ){
		return false;
	}
	kmb::ElementEvaluator evaluator( this->mesh->getNodes() );

	// ���ߓ_����Ȃ�_�~�[�ʂ�ǉ�
	// ���̖ʂ̐ߓ_�ԍ���V�������̂ɒu��������
	kmb::nodeIdType orgNodeTable[8];
	kmb::nodeIdType nodeTable[8];
	kmb::bodyIdType outerBoundaryId = mesh->beginElement( boundBody->getCount() );
	kmb::ElementContainer::iterator bIter = boundBody->begin();
	while( !bIter.isFinished() ){
		int num = bIter.getNodeCount();
		for(int i=0;i<num;++i){
			orgNodeTable[i] = bIter[i];
			nodeTable[i] = getDuplicatedNodeId( bIter[i] );
		}
		mesh->addElement( bIter.getType(), orgNodeTable );
		for(int i=0;i<num;++i){
			bIter.setCellId(i,nodeTable[i]);
		}
		++bIter;
	}
	mesh->endElement();
	// boundBody => ���̋��E�ʂŐߓ_�ԍ���V�����ԍ��ɒu��������

	// �ʂ̐e�v�f���ׂĂɂ��Ĕԍ��̕t���ւ�
	kmb::ElementContainer::iterator eIter = parentBody->begin();
	while( !eIter.isFinished() ){
		int len = eIter.getNodeCount();
		for(int i=0;i<len;++i){
			std::map< kmb::nodeIdType, kmb::nodeIdType >::iterator nIter = nodeMapper.find( eIter[i] );
			if( nIter != nodeMapper.end() ){
				eIter.setCellId( i, nIter->second );
			}
		}
		++eIter;
	}
	// parentBody => �e�̈�Őߓ_�ԍ���V�����ԍ��ɒu��������

	// ���������\�ʃO���[�v
	kmb::ElementContainer* outerBoundaryBody = mesh->getBodyPtr( outerBoundaryId );
	if( outerBoundaryBody == NULL ){
		return false;
	}
	// outerBoundBody => ���̋��E�ʂŌ��̐ߓ_�ԍ��̂܂�

	// �ߓ_�̋ߖT�֌W�̎擾
	// boundBody �̐ߓ_��u�������Ă���
	kmb::NodeNeighborInfo neighborInfo;
	neighborInfo.appendCoboundary( boundBody );

	// �@�����v�Z���� => ���������ߓ_   �����̖�   boundBody
	// �_�𓮂���     => �I���W�i���ߓ_ �O���̖ʁ@ outerBoundaryBody
	// ���������_�̎���̗v�f�𒲂ׂ�
	// ���̖@�������Ō��̐ߓ_���O���ɂ��炷
	// ���������ߓ_�͂��̂܂܂̏ꏊ�ɂ����Ă���
	kmb::Node node;
	std::map<kmb::nodeIdType,kmb::nodeIdType>::iterator nIter = nodeMapper.begin();
	while( nIter != nodeMapper.end() )
	{
		kmb::nodeIdType nodeId = nIter->first;
		kmb::nodeIdType dupnodeId = nIter->second;
		// �����_�̎��ӗv�f�����o���āA�@���x�N�g�������߂�
		kmb::Vector3D normVect(0.0,0.0,0.0);
		kmb::Vector3D v;
		kmb::NodeNeighbor::iterator neiIter = neighborInfo.beginIteratorAt( dupnodeId );
		kmb::NodeNeighbor::iterator neiEnd = neighborInfo.endIteratorAt( dupnodeId );
		while( neiIter != neiEnd )
		{
			kmb::elementIdType eId = neiIter->second;
			kmb::ElementContainer::iterator element = boundBody->find( eId );
			if( !element.isFinished() &&
				evaluator.getNormalVector( element, v ) )
			{
				normVect += v;
			}
			++neiIter;
		}
		normVect.normalize();
		this->mesh->getNode(nodeId,node);
		// ���_�̍X�V
		// �@���x�N�g���̕����ɓ�����
		kmb::Point3D point = node + normVect.scalar(layerThick[layerNum-1]);
		mesh->updateNode( point.x(), point.y(), point.z(), nodeId );
		++nIter;
	}
	neighborInfo.clear();

	// �V�����\�ʂƃI���W�i���ߓ_�̏�񂩂�w���b�V���̍쐬
	kmb::bodyIdType layerId = this->mesh->beginElement( boundBody->getCount() * this->layerNum );
	{
		kmb::ElementContainer::iterator bIter = outerBoundaryBody->begin();
		while( !bIter.isFinished() ){
			int len = bIter.getNodeCount();
			for(int i=0;i<len;++i){
				orgNodeTable[i] = bIter[i];
			}
			appendLayerElements( bIter.getType(), orgNodeTable, true );
			++bIter;
		}
	}
	this->mesh->endElement();

	// ���������
	clearLayerNodes();
	// �V�����\�ʁi�ߓ_�ԍ��̓I���W�i���j��Ԃ�
	boundaryId = outerBoundaryId;
	return layerId;
}

kmb::bodyIdType
kmb::BLMGenerator::intrudeB(kmb::bodyIdType parentId,kmb::bodyIdType &boundaryId)
{
	// (1) body �̐ߓ_�̎���̋ߖT���A���E���
	// (2) ���� body �̐ߓ_���d��
	// (3) �e�v�f�Ō��̐ߓ_���܂ނ��̂�V�ߓ_�ɒu��������
	// (4) �ߓ_�̖@���x�N�g�������߂āA�V�����ߓ_������ɓ�����
	// (5) ���̐ߓ_�ƐV�ߓ_�̊Ԃ�v�f�Ŗ��߂�

	// ����
	kmb::ElementContainer* boundBody = mesh->getBodyPtr( boundaryId );
	if( boundBody == NULL ){
		return false;
	}
	kmb::ElementContainer* parentBody = mesh->getBodyPtr( parentId );
	if( parentBody == NULL ){
		return false;
	}
	kmb::ElementEvaluator evaluator( this->mesh->getNodes() );

	// ���ߓ_����Ȃ�_�~�[�ʂ�ǉ�
	// ���̖ʂ̐ߓ_�ԍ���V�������̂ɒu��������
	kmb::nodeIdType orgNodeTable[8];
	kmb::nodeIdType nodeTable[8];
	kmb::bodyIdType outerBoundaryId = mesh->beginElement( boundBody->getCount() );
	kmb::ElementContainer::iterator bIter = boundBody->begin();
	while( !bIter.isFinished() ){
		int num = bIter.getNodeCount();
		for(int i=0;i<num;++i){
			orgNodeTable[i] = bIter[i];
			nodeTable[i] = getDuplicatedNodeId( bIter[i] );
		}
		mesh->addElement( bIter.getType(), orgNodeTable );
		for(int i=0;i<num;++i){
			bIter.setCellId(i,nodeTable[i]);
		}
		++bIter;
	}
	mesh->endElement();

	// �ʂ̐e�v�f���ׂĂɂ��Ĕԍ��̕t���ւ�
	kmb::ElementContainer::iterator eIter = parentBody->begin();
	while( !eIter.isFinished() ){
		int len = eIter.getNodeCount();
		for(int i=0;i<len;++i){
			std::map< kmb::nodeIdType, kmb::nodeIdType >::iterator nIter = nodeMapper.find( eIter[i] );
			if( nIter != nodeMapper.end() ){
				eIter.setCellId( i, nIter->second );
			}
		}
		++eIter;
	}

	// ���������\�ʃO���[�v
	kmb::ElementContainer* outerBoundaryBody = mesh->getBodyPtr( outerBoundaryId );
	if( outerBoundaryBody == NULL ){
		return false;
	}

	// �ߓ_�̋ߖT�֌W�̎擾
	// boundBody �̐ߓ_��u�������Ă���
	kmb::NodeNeighborInfo neighborInfo;
	neighborInfo.appendCoboundary( outerBoundaryBody );

	// �_�𓮂���     => ���������ߓ_   ������ boundBody
	// �@�����v�Z���� => �I���W�i���ߓ_ �O���� outerBoundaryBody
	// ���������_�̎���̗v�f�𒲂ׂ�
	// ���̖@�������Ō��̐ߓ_���O���ɂ��炷
	// ���������ߓ_�͂��̂܂܂̏ꏊ�ɂ����Ă���
	kmb::Node node;
	std::map<kmb::nodeIdType,kmb::nodeIdType>::iterator nIter = nodeMapper.begin();
	while( nIter != nodeMapper.end() )
	{
		kmb::nodeIdType nodeId = nIter->first;
		kmb::nodeIdType dupnodeId = nIter->second;
		// �����_�̎��ӗv�f�����o���āA�@���x�N�g�������߂�
		kmb::Vector3D normVect(0.0,0.0,0.0);
		kmb::Vector3D v;
		kmb::NodeNeighbor::iterator neiIter = neighborInfo.beginIteratorAt( nodeId );
		kmb::NodeNeighbor::iterator neiEnd = neighborInfo.endIteratorAt( nodeId );
		while( neiIter != neiEnd )
		{
			kmb::elementIdType eId = neiIter->second;
			kmb::ElementContainer::iterator element = outerBoundaryBody->find( eId );
			if( !element.isFinished() &&
				evaluator.getNormalVector( element, v ) )
			{
				normVect += v;
			}
			++neiIter;
		}
		normVect.normalize();
		this->mesh->getNode(dupnodeId,node);
		// ���_�̍X�V
		// �@���x�N�g���̋t�����ɓ�����
		kmb::Point3D point = node - normVect.scalar(layerThick[layerNum-1]);
		mesh->updateNode( point.x(), point.y(), point.z(), dupnodeId );
		++nIter;
	}
	neighborInfo.clear();

	// �V�����\�ʂƃI���W�i���ߓ_�̏�񂩂�w���b�V���̍쐬
	kmb::bodyIdType layerId = this->mesh->beginElement( boundBody->getCount() * this->layerNum );
	{
		kmb::ElementContainer::iterator bIter = outerBoundaryBody->begin();
		while( !bIter.isFinished() ){
			int len = bIter.getNodeCount();
			for(int i=0;i<len;++i){
				orgNodeTable[i] = bIter[i];
			}
			appendLayerElements( bIter.getType(), orgNodeTable, true );
			++bIter;
		}
	}
	this->mesh->endElement();

	// ���������
	clearLayerNodes();
	// �V�����\�ʁi�ߓ_�ԍ��̓I���W�i���j��Ԃ�
	boundaryId = outerBoundaryId;
	return layerId;
}

kmb::bodyIdType
kmb::BLMGenerator::extrudeFromData(const char* faceGroup)
{
	// (0) ����
	kmb::DataBindings* faceData = mesh->getDataBindingsPtr(faceGroup);
	if( faceData == NULL || faceData->getBindingMode() != kmb::DataBindings::FaceGroup ){
		return kmb::Body::nullBodyId;
	}
	kmb::ElementContainer* parentBody = mesh->getBodyPtr( faceData->getTargetBodyId() );
	if( parentBody == NULL ){
		return kmb::Body::nullBodyId;
	}

	// �]���֐��̏���
	kmb::ElementEvaluator evaluator( this->mesh->getNodes() );

	// �ߖT���̍쐬
	kmb::NodeNeighborFaceInfo neighborInfo;
	neighborInfo.appendCoboundary( faceData, parentBody );

	// FaceGroup ��̐ߓ_���d��
	kmb::DataBindings::iterator fIter = faceData->begin();
	while( !fIter.isFinished() ){
		kmb::Face f;
		if( fIter.getFace(f) ){
			int localId = static_cast<int>( f.getLocalFaceId() );
			kmb::ElementContainer::iterator eIter = parentBody->find( f.getElementId() );
			if( !eIter.isFinished() ){
				int bnum = eIter.getBoundaryNodeCount(localId);
				for(int i=0;i<bnum;++i){
					getDuplicatedNodeId( eIter.getBoundaryCellId(localId,i) );
				}
			}
		}
		++fIter;
	}

	// ���̓_�̎���̗v�f�𒲂ׂ�
	// ���̖@�������Ń_�~�[�ߓ_�����炷
	kmb::Node node;
	std::map<kmb::nodeIdType,kmb::nodeIdType>::iterator nIter = nodeMapper.begin();
	while( nIter != nodeMapper.end() )
	{
		kmb::nodeIdType nodeId = nIter->first;
		kmb::nodeIdType dupnodeId = nIter->second;
		// ���ӗv�f�����o���āA�@���x�N�g�������߂�
		kmb::Vector3D normVect(0.0,0.0,0.0);
		kmb::Vector3D v;
		kmb::NodeNeighborFace::iterator nfIter = neighborInfo.beginIteratorAt( nodeId );
		kmb::NodeNeighborFace::iterator nfEnd = neighborInfo.endIteratorAt( nodeId );
		while( nfIter != nfEnd )
		{
			kmb::Face f = nfIter->second;
			kmb::ElementContainer::iterator element = parentBody->find( f.getElementId() );
			if( !element.isFinished() &&
				evaluator.getNormalVectorOfFace( element, f.getLocalFaceId(), v ) )
			{
				normVect += v;
			}
			++nfIter;
		}
		normVect.normalize();
		this->mesh->getNode(nodeId,node);
		// �����̃_�~�[���_�̍X�V
		// �@���x�N�g���Ƌt�̕����ɓ�����
		kmb::Point3D point = node + normVect.scalar(layerThick[layerNum-1]);
		mesh->updateNode( point.x(), point.y(), point.z(), dupnodeId );
		++nIter;
	}
	neighborInfo.clear();

	// �ʂ��ׂĂɂ��ď���
	kmb::bodyIdType layerId = this->mesh->beginElement( faceData->getIdCount() * this->layerNum );
	{
		kmb::DataBindings* faceData2 = mesh->createDataBindings(faceGroup,
			kmb::DataBindings::FaceGroup,kmb::PhysicalValue::None,"tmp",faceData->getTargetBodyId());
		kmb::nodeIdType oldNodeId[4];
		kmb::DataBindings::iterator fIter = faceData->begin();
		while( !fIter.isFinished() ){
			kmb::Face f;
			if( fIter.getFace(f) ){
				kmb::ElementContainer::iterator elem = mesh->findElement( f.getElementId() );
				if( elem.isFinished() ){
					continue;
				}
				kmb::idType localId = f.getLocalFaceId();
				int len = elem.getBoundaryNodeCount(localId);
				for(int i=0;i<len;++i){
					oldNodeId[i] = elem.getBoundaryCellId(localId,i);
				}
				kmb::Face outerFace = appendLayerElements( elem.getBoundaryType(localId), oldNodeId, false );
				faceData2->addId( outerFace );
			}
			++fIter;
		}
		faceData->clear();
		kmb::DataBindings::iterator dIter = faceData2->begin();
		while( !dIter.isFinished() ){
			kmb::Face f;
			if( dIter.getFace( f ) ){
				faceData->addId( f );
			}
			++dIter;
		}
		mesh->deleteData(faceGroup,"tmp");
	}
	this->mesh->endElement();

	faceData->setTargetBodyId( layerId );
	clearLayerNodes();
	return layerId;
}

kmb::bodyIdType
kmb::BLMGenerator::intrudeFromData(const char* faceGroup)
{
	// ����
	kmb::DataBindings* faceData = mesh->getDataBindingsPtr(faceGroup);
	if( faceData == NULL || faceData->getBindingMode() != kmb::DataBindings::FaceGroup ){
		return kmb::Body::nullBodyId;
	}
	kmb::ElementContainer* parentBody = mesh->getBodyPtr( faceData->getTargetBodyId() );
	if( parentBody == NULL ){
		return kmb::Body::nullBodyId;
	}

	// FaceGroup ��̐ߓ_���d��
	// �I���W�i���ߓ_�� Boundary Body ���쐬
	kmb::ElementContainerMap boundaryBody;
	kmb::DataBindings::iterator fIter = faceData->begin();
	kmb::nodeIdType nodes[8];
	while( !fIter.isFinished() ){
		kmb::Face f;
		if( fIter.getFace(f) ){
			int localId = static_cast<int>( f.getLocalFaceId() );
			kmb::ElementContainer::iterator eIter = parentBody->find( f.getElementId() );
			if( !eIter.isFinished() ){
				int bnum = eIter.getBoundaryNodeCount(localId);
				for(int i=0;i<bnum;++i){
					nodes[i] = eIter.getBoundaryCellId(localId,i);
					getDuplicatedNodeId( nodes[i] );
				}
				boundaryBody.addElement( eIter.getBoundaryType(localId), nodes );
			}
		}
		++fIter;
	}

	// �ʂ̐e�v�f���ׂĂɂ��Ĕԍ��̕t���ւ�
	kmb::ElementContainer::iterator eIter = parentBody->begin();
	while( !eIter.isFinished() ){
		int len = eIter.getNodeCount();
		for(int i=0;i<len;++i){
			std::map< kmb::nodeIdType, kmb::nodeIdType >::iterator nIter = nodeMapper.find( eIter[i] );
			if( nIter != nodeMapper.end() ){
				eIter.setCellId( i, nIter->second );
			}
		}
		++eIter;
	}

	// �]���֐��̏���
	kmb::ElementEvaluator evaluator( this->mesh->getNodes() );

	// �ߖT���̍쐬
	kmb::NodeNeighborFaceInfo neighborInfo;
	neighborInfo.appendCoboundary( faceData, parentBody );

	// �����ߓ_�̎���̗v�f�𒲂ׂ�
	// ���̖@�������Ō��̐ߓ_�����炷
	kmb::Node node;
	std::map<kmb::nodeIdType,kmb::nodeIdType>::iterator nIter = nodeMapper.begin();
	while( nIter != nodeMapper.end() )
	{
		kmb::nodeIdType nodeId = nIter->first;
		kmb::nodeIdType dupnodeId = nIter->second;
		// ���ӗv�f�����o���āA�@���x�N�g�������߂�
		kmb::Vector3D normVect(0.0,0.0,0.0);
		kmb::Vector3D v;
		kmb::NodeNeighborFace::iterator nfIter = neighborInfo.beginIteratorAt( dupnodeId );
		kmb::NodeNeighborFace::iterator nfEnd = neighborInfo.endIteratorAt( dupnodeId );
		while( nfIter != nfEnd )
		{
			kmb::Face f = nfIter->second;
			kmb::ElementContainer::iterator element = parentBody->find( f.getElementId() );
			if( !element.isFinished() &&
				evaluator.getNormalVectorOfFace( element, f.getLocalFaceId(), v ) )
			{
				normVect += v;
			}
			++nfIter;
		}
		normVect.normalize();
		this->mesh->getNode(nodeId,node);
		// �����̃_�~�[���_�̍X�V
		// �@���x�N�g���Ƌt�̕����ɓ�����
		kmb::Point3D point = node + normVect.scalar(layerThick[layerNum-1]);
		mesh->updateNode( point.x(), point.y(), point.z(), nodeId );
		++nIter;
	}
	neighborInfo.clear();

	// �ʂ��ׂĂɂ��ď���
	kmb::bodyIdType layerId = this->mesh->beginElement( faceData->getIdCount() * this->layerNum );
	{
		faceData->clear();
		kmb::nodeIdType orgNodeTable[8];
		kmb::ElementContainer::iterator eIter = boundaryBody.begin();
		while( !eIter.isFinished() ){
			int len = eIter.getNodeCount();
			for(int i=0;i<len;++i){
				orgNodeTable[i] = eIter[i];
			}
			kmb::Face outerFace = appendLayerElements( eIter.getType(), orgNodeTable, true );
			faceData->addId( outerFace );
			++eIter;
		}
	}
	this->mesh->endElement();
	faceData->setTargetBodyId( layerId );

	// ���������
	clearLayerNodes();
	return layerId;
}

kmb::bodyIdType
kmb::BLMGenerator::intrudeBFromData(const char* faceGroup)
{
	// ����
	kmb::DataBindings* faceData = mesh->getDataBindingsPtr(faceGroup);
	if( faceData == NULL || faceData->getBindingMode() != kmb::DataBindings::FaceGroup ){
		return kmb::Body::nullBodyId;
	}
	kmb::ElementContainer* parentBody = mesh->getBodyPtr( faceData->getTargetBodyId() );
	if( parentBody == NULL ){
		return kmb::Body::nullBodyId;
	}

	// FaceGroup ��̐ߓ_���d��
	// �I���W�i���ߓ_�� Boundary Body ���쐬
	kmb::ElementContainerMap outerBoundaryBody;
	kmb::DataBindings::iterator fIter = faceData->begin();
	kmb::nodeIdType nodes[8];
	while( !fIter.isFinished() ){
		kmb::Face f;
		if( fIter.getFace(f) ){
			int localId = static_cast<int>( f.getLocalFaceId() );
			kmb::ElementContainer::iterator eIter = parentBody->find( f.getElementId() );
			if( !eIter.isFinished() ){
				int bnum = eIter.getBoundaryNodeCount(localId);
				for(int i=0;i<bnum;++i){
					nodes[i] = eIter.getBoundaryCellId(localId,i);
					getDuplicatedNodeId( nodes[i] );
				}
				outerBoundaryBody.addElement( eIter.getBoundaryType(localId), nodes );
			}
		}
		++fIter;
	}

	// �ʂ̐e�v�f���ׂĂɂ��Ĕԍ��̕t���ւ�
	kmb::ElementContainer::iterator eIter = parentBody->begin();
	while( !eIter.isFinished() ){
		int len = eIter.getNodeCount();
		for(int i=0;i<len;++i){
			std::map< kmb::nodeIdType, kmb::nodeIdType >::iterator nIter = nodeMapper.find( eIter[i] );
			if( nIter != nodeMapper.end() ){
				eIter.setCellId( i, nIter->second );
			}
		}
		++eIter;
	}

	// �]���֐��̏���
	kmb::ElementEvaluator evaluator( this->mesh->getNodes() );

	// �ߖT���̍쐬
	kmb::NodeNeighborInfo neighborInfo;
	neighborInfo.appendCoboundary( &outerBoundaryBody );

	// �����ߓ_�̎���̗v�f�𒲂ׂ�
	// ���̖@�������Ō��̐ߓ_�����炷
	kmb::Node node;
	std::map<kmb::nodeIdType,kmb::nodeIdType>::iterator nIter = nodeMapper.begin();
	while( nIter != nodeMapper.end() )
	{
		kmb::nodeIdType dupnodeId = nIter->second;
		// ���ӗv�f�����o���āA�@���x�N�g�������߂�
		kmb::Vector3D normVect(0.0,0.0,0.0);
		kmb::Vector3D v;
		kmb::NodeNeighbor::iterator neiIter = neighborInfo.beginIteratorAt( dupnodeId );
		kmb::NodeNeighbor::iterator neiEnd = neighborInfo.endIteratorAt( dupnodeId );
		while( neiIter != neiEnd )
		{
			kmb::elementIdType eId = neiIter->second;
			kmb::ElementContainer::iterator element = outerBoundaryBody.find( eId );
			if( !element.isFinished() &&
				evaluator.getNormalVector( element, v ) )
			{
				normVect += v;
			}
			++neiIter;
		}
		normVect.normalize();
		this->mesh->getNode(dupnodeId,node);
		// �����̃_�~�[���_�̍X�V
		// �@���x�N�g���Ƌt�̕����ɓ�����
		kmb::Point3D point = node - normVect.scalar(layerThick[layerNum-1]);
		mesh->updateNode( point.x(), point.y(), point.z(), dupnodeId );
		++nIter;
	}
	neighborInfo.clear();

	// �ʂ��ׂĂɂ��ď���
	kmb::bodyIdType layerId = this->mesh->beginElement( faceData->getIdCount() * this->layerNum );
	{
		faceData->clear();
		kmb::nodeIdType orgNodeTable[8];
		kmb::ElementContainer::iterator eIter = outerBoundaryBody.begin();
		while( !eIter.isFinished() ){
			int len = eIter.getNodeCount();
			for(int i=0;i<len;++i){
				orgNodeTable[i] = eIter[i];
			}
			kmb::Face outerFace = appendLayerElements( eIter.getType(), orgNodeTable, true );
			faceData->addId( outerFace );
			++eIter;
		}
	}
	this->mesh->endElement();
	faceData->setTargetBodyId( layerId );

	// ���������
	clearLayerNodes();
	return layerId;
}

bool
kmb::BLMGenerator::generateInnerFromData(const char* faceGroup,kmb::bodyIdType &layerId)
{
	// (0) ����
	// (1) faceGroup �̐ߓ_�̎���̋ߖT���A���E���
	// (2) ���� faceGroup �̓����ߓ_���d��
	// (3) �ߓ_�̖@���x�N�g�������߂āA��d�������V�ߓ_������ɓ�����
	// (4) ���̐ߓ_�ƐV�ߓ_�̊Ԃ�v�f�Ŗ��߂�
	// (5) �e�v�f�Ō��̐ߓ_���܂ނ��̂�V�ߓ_�ɒu��������

	// (0)
	kmb::DataBindings* faceData = mesh->getDataBindingsPtr(faceGroup);
	if( faceData == NULL || faceData->getBindingMode() != kmb::DataBindings::FaceGroup ){
		return false;
	}
	kmb::ElementContainer* parentBody = mesh->getBodyPtr( faceData->getTargetBodyId() );
	if( parentBody == NULL ){
		return false;
	}
	kmb::ElementEvaluator evaluator( this->mesh->getNodes() );
	// facegroup ��v�f������
	kmb::ElementContainer* body = new kmb::ElementContainerMap();
	mesh->faceGroupToBody( faceGroup, body );

	// (1)
	kmb::NodeNeighborFaceInfo neighborInfo;
	neighborInfo.appendCoboundary( faceData, parentBody );
	kmb::BoundaryExtractor bext;
	bext.appendUnitBody( body );
	kmb::DataBindings* boundNG = mesh->createDataBindings("BoundNG",kmb::DataBindings::NodeGroup,kmb::PhysicalValue::None,"tmp");
	bext.getBoundaryNodeGroup( body, boundNG );

	// (2)
	kmb::DataBindings::iterator fIter = faceData->begin();
	while( !fIter.isFinished() ){
		kmb::Face f;
		if( fIter.getFace(f) ){
			int localId = static_cast<int>( f.getLocalFaceId() );
			kmb::ElementContainer::iterator eIter = parentBody->find( f.getElementId() );
			if( !eIter.isFinished() ){
				int bnum = eIter.getBoundaryNodeCount(localId);
				for(int i=0;i<bnum;++i){
					kmb::nodeIdType nodeId = eIter.getBoundaryCellId(localId,i);
					if( !boundNG->hasId( nodeId ) ){
						getDuplicatedNodeId( eIter.getBoundaryCellId(localId,i) );
					}
				}
			}
		}
		++fIter;
	}

	// (3)
	// ���̓_�̎���̗v�f�𒲂ׂ�
	// ���̖@�������Ń_�~�[�ߓ_�����炷
	kmb::Node node;
	std::map<kmb::nodeIdType,kmb::nodeIdType>::iterator nIter = nodeMapper.begin();
	while( nIter != nodeMapper.end() )
	{
		kmb::nodeIdType nodeId = nIter->first;
		kmb::nodeIdType dupnodeId = nIter->second;
		// ���ӗv�f�����o���āA�@���x�N�g�������߂�
		kmb::Vector3D normVect(0.0,0.0,0.0);
		kmb::Vector3D v;
		kmb::NodeNeighborFace::iterator nfIter = neighborInfo.beginIteratorAt( nodeId );
		kmb::NodeNeighborFace::iterator nfEnd = neighborInfo.endIteratorAt( nodeId );
		while( nfIter != nfEnd )
		{
			kmb::Face f = nfIter->second;
			kmb::ElementContainer::iterator element = parentBody->find( f.getElementId() );
			if( !element.isFinished() &&
				evaluator.getNormalVectorOfFace( element, f.getLocalFaceId(), v ) )
			{
				normVect += v;
			}
			++nfIter;
		}
		normVect.normalize();
		this->mesh->getNode(nodeId,node);
		// �����̃_�~�[���_�̍X�V
		// �@���x�N�g���Ƌt�̕����ɓ�����
		kmb::Point3D point = node - normVect.scalar(layerThick[layerNum-1]);
		mesh->updateNode( point.x(), point.y(), point.z(), dupnodeId );
		++nIter;
	}
	neighborInfo.clear();

	// (4)
	// �ʂ��ׂĂɂ��ď���
	layerId = this->mesh->beginElement( faceData->getIdCount() * this->layerNum );
	{
		kmb::DataBindings* faceData2 = mesh->createDataBindings(faceGroup,
			kmb::DataBindings::FaceGroup,kmb::PhysicalValue::None,"tmp",faceData->getTargetBodyId());
		kmb::nodeIdType oldNodeId[4];
		kmb::DataBindings::iterator fIter = faceData->begin();
		while( !fIter.isFinished() ){
			kmb::Face f;
			if( fIter.getFace(f) ){
				kmb::ElementContainer::iterator elem = mesh->findElement( f.getElementId() );
				if( elem.isFinished() ){
					continue;
				}
				kmb::idType localId = f.getLocalFaceId();
				int len = elem.getBoundaryNodeCount(localId);
				for(int i=0;i<len;++i){
					oldNodeId[i] = elem.getBoundaryCellId(localId,i);
				}
				kmb::Face outerFace = appendLayerElements( elem.getBoundaryType(localId), oldNodeId );
				faceData2->addId( outerFace );
			}
			++fIter;
		}
		faceData->clear();
		kmb::DataBindings::iterator dIter = faceData2->begin();
		while( !dIter.isFinished() ){
			kmb::Face f;
			if( dIter.getFace( f ) ){
				faceData->addId( f );
			}
			++dIter;
		}
		mesh->deleteData(faceGroup,"tmp");
	}
	this->mesh->endElement();

	// (5)
	// �ʂ̐e�v�f���ׂĂɂ��Ĕԍ��̕t���ւ�
	kmb::ElementContainer::iterator eIter = parentBody->begin();
	while( !eIter.isFinished() ){
		int len = eIter.getNodeCount();
		for(int i=0;i<len;++i){
			std::map< kmb::nodeIdType, kmb::nodeIdType >::iterator nIter = nodeMapper.find( eIter[i] );
			if( nIter != nodeMapper.end() ){
				eIter.setCellId( i, nIter->second );
			}
		}
		++eIter;
	}

	mesh->clearData("BoundNG","tmp");
	clearLayerNodes();
	delete body;

	return true;
}

bool
kmb::BLMGenerator::generateInner(kmb::bodyIdType bodyId,kmb::bodyIdType parentId,kmb::bodyIdType &layerId)
{
	// (0) ����
	// (1) body �̐ߓ_�̎���̋ߖT���A���E���
	// (2) ���� body �̓����ߓ_���d��
	// (3) �ߓ_�̖@���x�N�g�������߂āA��d�������V�ߓ_������ɓ�����
	// (4) ���̐ߓ_�ƐV�ߓ_�̊Ԃ�v�f�Ŗ��߂�
	// (5) �e�v�f�Ō��̐ߓ_���܂ނ��̂�V�ߓ_�ɒu��������

	// (0)
	kmb::ElementContainer* boundBody = mesh->getBodyPtr( bodyId );
	if( boundBody == NULL ){
		return false;
	}
	kmb::ElementContainer* parentBody = mesh->getBodyPtr( parentId );
	if( parentBody == NULL ){
		return false;
	}
	kmb::ElementEvaluator evaluator( this->mesh->getNodes() );

	// (1)
	kmb::NodeNeighborInfo neighborInfo;
	neighborInfo.appendCoboundary( boundBody );
	kmb::BoundaryExtractor bext;
	bext.appendUnitBody( boundBody );
	kmb::DataBindings* boundNG = mesh->createDataBindings("BoundNG",kmb::DataBindings::NodeGroup,kmb::PhysicalValue::None,"tmp");
	bext.getBoundaryNodeGroup( boundBody, boundNG );

	// (2)
	kmb::ElementContainer::iterator bIter = boundBody->begin();
	while( !bIter.isFinished() ){
		int num = bIter.getNodeCount();
		for(int i=0;i<num;++i){
			getDuplicatedNodeId( bIter[i] );
		}
		++bIter;
	}

	// (3)
	// ���̓_�̎���̗v�f�𒲂ׂ�
	// ���̖@��������2�d�������_�~�[�ߓ_������ɂ��炷
	kmb::Node node;
	std::map<kmb::nodeIdType,kmb::nodeIdType>::iterator nIter = nodeMapper.begin();
	while( nIter != nodeMapper.end() )
	{
		kmb::nodeIdType nodeId = nIter->first;
		kmb::nodeIdType dupnodeId = nIter->second;
		// ���ӗv�f�����o���āA�@���x�N�g�������߂�
		kmb::Vector3D normVect(0.0,0.0,0.0);
		kmb::Vector3D v;
		kmb::NodeNeighbor::iterator neiIter = neighborInfo.beginIteratorAt( nodeId );
		kmb::NodeNeighbor::iterator neiEnd = neighborInfo.endIteratorAt( nodeId );
		while( neiIter != neiEnd )
		{
			kmb::elementIdType eId = neiIter->second;
			kmb::ElementContainer::iterator element = boundBody->find( eId );
			if( !element.isFinished() &&
				evaluator.getNormalVector( element, v ) )
			{
				normVect += v;
			}
			++neiIter;
		}
		normVect.normalize();
		this->mesh->getNode(nodeId,node);
		// �����̃_�~�[���_�̍X�V
		// �@���x�N�g���Ƌt�̕����ɓ�����
		kmb::Point3D point = node - normVect.scalar(layerThick[layerNum-1]);
		mesh->updateNode( point.x(), point.y(), point.z(), dupnodeId );
		++nIter;
	}
	neighborInfo.clear();

	// (4)
	// �ʂ��ׂĂɂ��ď���
	layerId = this->mesh->beginElement( boundBody->getCount() * this->layerNum );
	{
		kmb::nodeIdType oldNodeId[8];
		kmb::ElementContainer::iterator bIter = boundBody->begin();
		while( !bIter.isFinished() ){
			int len = bIter.getNodeCount();
			for(int i=0;i<len;++i){
				oldNodeId[i] = bIter[i];
			}
			appendLayerElements( bIter.getType(), oldNodeId );
			++bIter;
		}
	}
	this->mesh->endElement();

	// (5)
	// �ʂ̐e�v�f���ׂĂɂ��Ĕԍ��̕t���ւ�
	kmb::ElementContainer::iterator eIter = parentBody->begin();
	while( !eIter.isFinished() ){
		int len = eIter.getNodeCount();
		for(int i=0;i<len;++i){
			std::map< kmb::nodeIdType, kmb::nodeIdType >::iterator nIter = nodeMapper.find( eIter[i] );
			if( nIter != nodeMapper.end() ){
				eIter.setCellId( i, nIter->second );
			}
		}
		++eIter;
	}

	mesh->clearData("BoundNG","tmp");
	clearLayerNodes();

	return true;
}

kmb::Face
kmb::BLMGenerator::appendLayerElements( kmb::elementType etype, kmb::nodeIdType* oldNodeId, bool outer)
{
	kmb::Face outerFace;
	int len = kmb::Element::getNodeCount(etype);
	unsigned int flag = 0x00;
	kmb::nodeIdType nodeTable[8];
	kmb::nodeIdType newNodeId[4];
	kmb::elementIdType elemId;
	for(int i=0;i<len;++i){
		std::map< kmb::nodeIdType, kmb::nodeIdType >::iterator nIter = nodeMapper.find( oldNodeId[i] );
		if( nIter != nodeMapper.end() ){
			flag |= (0x01 << i);
			newNodeId[i] = nIter->second;
		}else{
			newNodeId[i] = oldNodeId[i];
		}
	}
	// �v�f�̒��_�̂����A�ǂ̐ߓ_���Q�d������Ă��邩�ŏꍇ����
	switch( etype ){
	case kmb::TRIANGLE:
		switch( flag ){
		case 0x07:
			// 3�_�S��
			if( outer ){
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = getLayerNodeId( oldNodeId[0], newNodeId[0], i );
					nodeTable[1] = getLayerNodeId( oldNodeId[1], newNodeId[1], i );
					nodeTable[2] = getLayerNodeId( oldNodeId[2], newNodeId[2], i );
					nodeTable[3] = getLayerNodeId( oldNodeId[0], newNodeId[0], i+1 );
					nodeTable[4] = getLayerNodeId( oldNodeId[1], newNodeId[1], i+1 );
					nodeTable[5] = getLayerNodeId( oldNodeId[2], newNodeId[2], i+1 );
					elemId = mesh->addElement(kmb::WEDGE,nodeTable);
					if( i == layerNum-1 ){
						outerFace.setId(elemId,1);
					}
				}
			}else{
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = getLayerNodeId( oldNodeId[0], newNodeId[0], i+1 );
					nodeTable[1] = getLayerNodeId( oldNodeId[1], newNodeId[1], i+1 );
					nodeTable[2] = getLayerNodeId( oldNodeId[2], newNodeId[2], i+1 );
					nodeTable[3] = getLayerNodeId( oldNodeId[0], newNodeId[0], i );
					nodeTable[4] = getLayerNodeId( oldNodeId[1], newNodeId[1], i );
					nodeTable[5] = getLayerNodeId( oldNodeId[2], newNodeId[2], i );
					elemId = mesh->addElement(kmb::WEDGE,nodeTable);
					if( i == 0 ){
						outerFace.setId(elemId,1);
					}
				}
			}
			break;
		case 0x03:
			// 2�_ 0 1
			if( outer ){
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = oldNodeId[2];
					nodeTable[1] = getLayerNodeId( oldNodeId[0], newNodeId[0], i );
					nodeTable[2] = getLayerNodeId( oldNodeId[0], newNodeId[0], i+1 );
					nodeTable[3] = getLayerNodeId( oldNodeId[1], newNodeId[1], i+1 );
					nodeTable[4] = getLayerNodeId( oldNodeId[1], newNodeId[1], i );
					elemId = mesh->addElement(kmb::PYRAMID,nodeTable);
					if( i == layerNum-1 ){
						outerFace.setId(elemId,1);
					}
				}
			}else{
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = oldNodeId[2];
					nodeTable[1] = getLayerNodeId( oldNodeId[0], newNodeId[0], i+1 );
					nodeTable[2] = getLayerNodeId( oldNodeId[0], newNodeId[0], i );
					nodeTable[3] = getLayerNodeId( oldNodeId[1], newNodeId[1], i );
					nodeTable[4] = getLayerNodeId( oldNodeId[1], newNodeId[1], i+1 );
					elemId = mesh->addElement(kmb::PYRAMID,nodeTable);
					if( i == 0 ){
						outerFace.setId(elemId,1);
					}
				}
			}
			break;
		case 0x05:
			// 2�_ 2 0
			if( outer ){
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = oldNodeId[1];
					nodeTable[1] = getLayerNodeId( oldNodeId[2], newNodeId[2], i );
					nodeTable[2] = getLayerNodeId( oldNodeId[2], newNodeId[2], i+1 );
					nodeTable[3] = getLayerNodeId( oldNodeId[0], newNodeId[0], i+1 );
					nodeTable[4] = getLayerNodeId( oldNodeId[0], newNodeId[0], i );
					elemId = mesh->addElement(kmb::PYRAMID,nodeTable);
					if( i == layerNum-1 ){
						outerFace.setId(elemId,1);
					}
				}
			}else{
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = oldNodeId[1];
					nodeTable[1] = getLayerNodeId( oldNodeId[2], newNodeId[2], i+1 );
					nodeTable[2] = getLayerNodeId( oldNodeId[2], newNodeId[2], i );
					nodeTable[3] = getLayerNodeId( oldNodeId[0], newNodeId[0], i );
					nodeTable[4] = getLayerNodeId( oldNodeId[0], newNodeId[0], i+1 );
					elemId = mesh->addElement(kmb::PYRAMID,nodeTable);
					if( i == 0 ){
						outerFace.setId(elemId,1);
					}
				}
			}
			break;
		case 0x06:
			// 2�_ 1 2
			if( outer ){
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = oldNodeId[0];
					nodeTable[1] = getLayerNodeId( oldNodeId[1], newNodeId[1], i );
					nodeTable[2] = getLayerNodeId( oldNodeId[1], newNodeId[1], i+1 );
					nodeTable[3] = getLayerNodeId( oldNodeId[2], newNodeId[2], i+1 );
					nodeTable[4] = getLayerNodeId( oldNodeId[2], newNodeId[2], i );
					elemId = mesh->addElement(kmb::PYRAMID,nodeTable);
					if( i == layerNum-1 ){
						outerFace.setId(elemId,1);
					}
				}
			}else{
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = oldNodeId[0];
					nodeTable[1] = getLayerNodeId( oldNodeId[1], newNodeId[1], i+1 );
					nodeTable[2] = getLayerNodeId( oldNodeId[1], newNodeId[1], i );
					nodeTable[3] = getLayerNodeId( oldNodeId[2], newNodeId[2], i );
					nodeTable[4] = getLayerNodeId( oldNodeId[2], newNodeId[2], i+1 );
					elemId = mesh->addElement(kmb::PYRAMID,nodeTable);
					if( i == 0 ){
						outerFace.setId(elemId,1);
					}
				}
			}
			break;
		case 0x01:
			// 1�_ 0
			if( outer ){
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = getLayerNodeId( oldNodeId[0], newNodeId[0], i );
					nodeTable[1] = oldNodeId[1];
					nodeTable[2] = oldNodeId[2];
					nodeTable[3] = getLayerNodeId( oldNodeId[0], newNodeId[0], i+1 );
					elemId = mesh->addElement(kmb::TETRAHEDRON,nodeTable);
					if( i == layerNum-1 ){
						outerFace.setId(elemId,0);
					}
				}
			}else{
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = getLayerNodeId( oldNodeId[0], newNodeId[0], i+1 );
					nodeTable[1] = oldNodeId[1];
					nodeTable[2] = oldNodeId[2];
					nodeTable[3] = getLayerNodeId( oldNodeId[0], newNodeId[0], i );
					elemId = mesh->addElement(kmb::TETRAHEDRON,nodeTable);
					if( i == 0 ){
						outerFace.setId(elemId,0);
					}
				}
			}
			break;
		case 0x02:
			// 1�_ 1
			if( outer ){
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = oldNodeId[0];
					nodeTable[1] = getLayerNodeId( oldNodeId[1], newNodeId[1], i );
					nodeTable[2] = oldNodeId[2];
					nodeTable[3] = getLayerNodeId( oldNodeId[1], newNodeId[1], i+1 );
					elemId = mesh->addElement(kmb::TETRAHEDRON,nodeTable);
					if( i == layerNum-1 ){
						outerFace.setId(elemId,1);
					}
				}
			}else{
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = oldNodeId[0];
					nodeTable[1] = getLayerNodeId( oldNodeId[1], newNodeId[1], i+1 );
					nodeTable[2] = oldNodeId[2];
					nodeTable[3] = getLayerNodeId( oldNodeId[1], newNodeId[1], i );
					elemId = mesh->addElement(kmb::TETRAHEDRON,nodeTable);
					if( i == 0 ){
						outerFace.setId(elemId,1);
					}
				}
			}
			break;
		case 0x04:
			// 1�_ 2
			if( outer ){
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = oldNodeId[0];
					nodeTable[1] = oldNodeId[1];
					nodeTable[2] = getLayerNodeId( oldNodeId[2], newNodeId[2], i );
					nodeTable[3] = getLayerNodeId( oldNodeId[2], newNodeId[2], i+1 );
					elemId = mesh->addElement(kmb::TETRAHEDRON,nodeTable);
					if( i == layerNum-1 ){
						outerFace.setId(elemId,2);
					}
				}
			}else{
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = oldNodeId[0];
					nodeTable[1] = oldNodeId[1];
					nodeTable[2] = getLayerNodeId( oldNodeId[2], newNodeId[2], i+1 );
					nodeTable[3] = getLayerNodeId( oldNodeId[2], newNodeId[2], i );
					elemId = mesh->addElement(kmb::TETRAHEDRON,nodeTable);
					if( i == 0 ){
						outerFace.setId(elemId,2);
					}
				}
			}
			break;
		default:
			break;
		}
		break;
	case kmb::QUAD:
		switch( flag ){
		case 0x0f:
			// 4�_�S��
			if( outer ){
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = getLayerNodeId( oldNodeId[0], newNodeId[0], i );
					nodeTable[1] = getLayerNodeId( oldNodeId[1], newNodeId[1], i );
					nodeTable[2] = getLayerNodeId( oldNodeId[2], newNodeId[2], i );
					nodeTable[3] = getLayerNodeId( oldNodeId[3], newNodeId[3], i );
					nodeTable[4] = getLayerNodeId( oldNodeId[0], newNodeId[0], i+1 );
					nodeTable[5] = getLayerNodeId( oldNodeId[1], newNodeId[1], i+1 );
					nodeTable[6] = getLayerNodeId( oldNodeId[2], newNodeId[2], i+1 );
					nodeTable[7] = getLayerNodeId( oldNodeId[3], newNodeId[3], i+1 );
					elemId = mesh->addElement(kmb::HEXAHEDRON,nodeTable);
					if( i == layerNum-1 ){
						outerFace.setId(elemId,1);
					}
				}
			}else{
				for(int i=0;i<layerNum;++i){
					nodeTable[0] = getLayerNodeId( oldNodeId[0], newNodeId[0], i+1 );
					nodeTable[1] = getLayerNodeId( oldNodeId[1], newNodeId[1], i+1 );
					nodeTable[2] = getLayerNodeId( oldNodeId[2], newNodeId[2], i+1 );
					nodeTable[3] = getLayerNodeId( oldNodeId[3], newNodeId[3], i+1 );
					nodeTable[4] = getLayerNodeId( oldNodeId[0], newNodeId[0], i );
					nodeTable[5] = getLayerNodeId( oldNodeId[1], newNodeId[1], i );
					nodeTable[6] = getLayerNodeId( oldNodeId[2], newNodeId[2], i );
					nodeTable[7] = getLayerNodeId( oldNodeId[3], newNodeId[3], i );
					elemId = mesh->addElement(kmb::HEXAHEDRON,nodeTable);
					if( i == 0 ){
						outerFace.setId(elemId,1);
					}
				}
			}
			break;
		case 0x01:
			// 1�_ 0
			break;
		case 0x02:
			// 1�_ 1
			break;
		case 0x04:
			// 1�_ 2
			break;
		case 0x08:
			// 1�_ 3
			break;
		case 0x03:
			// 2�_ 0 1
			break;
		case 0x05:
			// 2�_ 0 2
			break;
		case 0x09:
			// 2�_ 0 3
			break;
		case 0x06:
			// 2�_ 1 2
			break;
		case 0x0a:
			// 2�_ 1 3
			break;
		case 0x0c:
			// 2�_ 2 3
			break;
		case 0x07:
			// 3�_ 0 1 2
			break;
		case 0x0b:
			// 3�_ 0 1 3
			break;
		case 0x0d:
			// 3�_ 0 2 3
			break;
		case 0x0e:
			// 3�_ 1 2 3
			break;
		default:
			break;
		}
		break;
	default:
		std::cout << "error : unsupported layer element " << kmb::Element::getTypeString(etype) << " " << flag << std::endl;
	}
	return outerFace;
}

kmb::nodeIdType
kmb::BLMGenerator::getLayerNodeId(kmb::nodeIdType outerNodeId, kmb::nodeIdType innerNodeId, int layerIndex ) const
{
	if( this->mesh == NULL || 0 > layerIndex || layerIndex > this->layerNum ){
		return kmb::nullNodeId;

	}
	kmb::nodeIdType* nodes = NULL;
	// ����΂����Ԃ�
	if( this->layerNodes.find( std::pair<kmb::nodeIdType,kmb::nodeIdType>(innerNodeId,outerNodeId) ) != this->layerNodes.end() ){
		nodes = this->layerNodes[ std::pair<kmb::nodeIdType,kmb::nodeIdType>(innerNodeId,outerNodeId) ];
	}else{
	// �Ȃ���΍��
		kmb::Node inner,outer;
		if( !this->mesh->getNode(innerNodeId,inner)
			|| !this->mesh->getNode(outerNodeId,outer) ){
			return kmb::nullNodeId;
		}
		nodes = new kmb::nodeIdType[ this->layerNum-1 ];
		if( nodes != NULL ){
			kmb::Vector3D dict = outer - inner;
			dict.normalize();
			for( int i = 0 ; i < this->layerNum-1; ++i )
			{
				// ���Ԓ��_�̓o�^
				kmb::Point3D pt = inner + dict.scalar( layerThick[i] );
				nodes[i] = this->mesh->addNode(pt.x(),pt.y(),pt.z());
			}
			this->layerNodes.insert( std::pair< std::pair<kmb::nodeIdType,kmb::nodeIdType>, kmb::nodeIdType*>( std::pair<kmb::nodeIdType,kmb::nodeIdType>(innerNodeId,outerNodeId), nodes ) );
		}
	}
	if( nodes == NULL ){
		return kmb::nullNodeId;
	}
	if( layerIndex == this->layerNum ){
		return outerNodeId;
	}else if( layerIndex == 0 ){
		return innerNodeId;
	}else{
		return nodes[layerIndex-1];
	}
}
