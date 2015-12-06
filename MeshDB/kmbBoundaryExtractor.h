/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : BoundaryExtractor                                       #
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
#pragma once

#include <map>
#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElement.h"
#include "MeshDB/kmbElementRelation.h"

namespace kmb{

class MeshData;
class Face;
class ElementContainer;
class DataBindings;

class BoundaryExtractor
{
protected:
	kmb::MeshData* mesh;
	// ���Ԃ��Ă���v�f���������ǂ���
	bool reverseMode;
	// �ߓ_���܂ނ悤�Ȗʂ̑Ή��e�[�u��
	std::multimap< kmb::nodeIdType, kmb::Face > facemap;
	// ���ׂĂ� Face �ɂ��� facemap �ɓo�^����
	// elements == NULL �̂Ƃ��͊֌W�͓o�^�ς݂� mesh �̂��ׂĂ̗v�f�𒲂ׂ�
	void appendElement(kmb::elementIdType elementId,kmb::ElementBase &element,const kmb::ElementContainer* elements=NULL);
	// facemap �ɓo�^����
	void appendFaceMap(kmb::elementIdType elementId,kmb::ElementBase &element,kmb::idType faceId,const kmb::ElementContainer* elements=NULL);
	// facemap �ɓo�^����Ă�����폜����
	// �����ł̈��� reverse �̓����o�ϐ��� reverseMode �Ƃ͕ʁBreverseMode = false ��O��Ƃ���B
	// reverse = false �Ȃ�΁A���������ɓo�^����Ă�����̂��폜�i�o�^�ς݂̗v�f�ō폜����ꍇ�j
	// reverse = true �Ȃ�΁A�t�����ɓo�^����Ă�����̂��폜�i�ڂ��Ă���v�f�ō폜����ꍇ�j
	void eraseFaceMap(kmb::ElementBase &element,kmb::idType faceId,bool reverse=false,const kmb::ElementContainer* elements=NULL);
	// �|�C���^�ŌĂ΂��
	void appendElementContainer(const kmb::ElementContainer* body);
	// �����Ăяo���p���[�`��
	kmb::ElementRelation::relationType getFaceRelation(kmb::ElementBase &e0,kmb::idType f0,kmb::ElementBase &e1,kmb::idType f1) const;
public:
	BoundaryExtractor(void);
	virtual ~BoundaryExtractor(void);
	void setMesh(kmb::MeshData* mesh);
	void appendBody(kmb::bodyIdType bodyId);
	// ���E�̒T���Ώۂ��������g�݂̂Ƃ���
	void appendUnitBody(const kmb::ElementContainer* body);
	// �v�f�O���[�v�Ȃ�A���̗v�f�̖ʂ�o�^
	// �ʃO���[�v�Ȃ�A���̖ʂ����̂܂� facemap �ɓo�^
	void appendData(const char* name);
	void appendData(const kmb::DataBindings* data);
	void eraseData(const char* name,bool reverse=false);
	// ���E�𒊏o���āAmesh �ɓo�^���AbodyId ��Ԃ�
	// MeshData �� bindingsStack �ɑ������ꂽ�f�[�^���o�^����Ă�����A��������E�ɂ��o�^����
	// bodyId �ɑ����� Face �� facemap ������o���Ē��o
	kmb::bodyIdType getBoundary(kmb::bodyIdType bodyId) const;
	// facemap �Ɋ܂܂�� Face �����ׂĎ��o���Ē��o
	kmb::bodyIdType getBoundary(void) const;
	// ���� BoundaryExtractor �ɓo�^����Ă��� Face �����o���Ē��o
	kmb::bodyIdType getBoundary(kmb::BoundaryExtractor& other) const;

	// ���E�����炩���ߊm�ۂ����R���e�i�Ɋi�[���� (mesh == NULL �̂Ƃ��ł��g����)
	size_t getBoundary(const kmb::ElementContainer* parent,kmb::ElementContainer* boundary) const;
	// ���E�����炩���ߊm�ۂ����ߓ_�O���[�v�Ɋi�[���� (mesh == NULL �̂Ƃ��ł��g����)
	// ���E����v�f�łȂ��A�ߓ_�O���[�v�łق����Ƃ��Ɏg��
	size_t getBoundaryNodeGroup(const kmb::ElementContainer* parent,kmb::DataBindings* nodeGroup) const;
	// ���E�𐬕��ɕ������ĕԂ�
	int getBoundaryComponents(kmb::bodyIdType bodyId,kmb::bodyIdType* &boundaryIds) const;
	// �o�^���Ă��鋫�E��񂩂� bodyId �Ɋ܂܂����̂� FaceGroup �Ƃ���
	// name �����Ď��o���B���̂Ƃ��Astype = "Brep" �ɂ���B
	bool getBoundaryFace(kmb::bodyIdType bodyId,const char* name) const;
	// �o�^���Ă��鋫�E��񂩂� Element �ƈ�v���� Face �����o��
	bool getBoundaryFace(const kmb::ElementBase& elem,kmb::Face &f) const;

	// ���łɓo�^����Ă��鋫�E�ʂƐڂ���悤�Ȗʂ𒊏o
	// bodyId = 0 �� bodyId = 1 �̃C���^�[�t�F�C�X�ʂ����o���ɂ�
	// appendBody(0) ���Ă��� getInterfaceBoundary(1) ����
	// �i�[�����̂� bodyId = 1 �̖�
	kmb::bodyIdType getInterfaceBoundary(kmb::bodyIdType bodyId) const;
	bool getInterfaceBoundaryFace(kmb::bodyIdType bodyId,const char* name) const;

	// boundary ���Ȃ���� = closed ���ǂ����̔���
	// ����̂��߂ɂ͂��炩���� appendBody ���Ă������Ƃ��K�v
	// ���E�ʂ� body �ɑ����Ă�����̂������邩���`�F�b�N����
	bool isClosed(kmb::bodyIdType bodyId) const;
	// �[�ɋ��E�ʂ̏W�����󂩂ǂ������`�F�b�N����
	bool isClosed(void) const;
	// ������Ԃɖ߂�
	void clear(void);
	void eraseData(const kmb::DataBindings* data,bool reverse);
	void setReverseMode(bool mode);
	bool getReverseMode(void) const;
};

}
