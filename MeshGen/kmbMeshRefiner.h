/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MeshRefiner                                             #
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
//
// �v�f�𕪊��܂��͎������グ��
//

#pragma once

#include "MeshDB/kmbElement.h"
#include <vector>
#include <string>

namespace kmb{

class MeshData;
class DataBindings;
class ElementContainer;
class MiddleNodeManager;

class MeshRefiner
{
public:
	enum interpolationRule{
		MAX,
		MIN,
		MIDDLE
	};
protected:
	struct DataPair{
		std::string name;
		std::string stype;
		const kmb::DataBindings* originalData;
		kmb::DataBindings* refinedData;
	};
	kmb::MeshData* mesh;
	kmb::MiddleNodeManager* middleMan;
	bool middleManDeletable;
	// dataPairs ��
	// refine �O : { name, stype, orgData, NULL }
	// refine �� : { name, stype, orgData, refinedData }
	// �ƂȂ�
	std::vector< kmb::MeshRefiner::DataPair > dataPairs;

	void setSecondNodes( kmb::ElementBase& element );
	interpolationRule interpRule;
	// 2���v�f�̌`��֐��̕␳�����邩
	bool secondFitting;
public:
	MeshRefiner(void);
	virtual ~MeshRefiner(void);
	void clear(void);
	void setMesh(kmb::MeshData* mesh);
	void setMiddleNodeManager(kmb::MiddleNodeManager* manager);
	// ���ɂQ���v�f�ɂȂ��Ă��� ElementGroup �ɂ��� MiddleNodeManager ���X�V����
	void setSecondNodesByData(const char* name,const char* stype=NULL);
	// ���ɂQ���v�f�ɂȂ��Ă��� Body �ɂ��� MiddleNodeManager ���X�V����
	void setSecondNodesByBody(kmb::bodyIdType bodyId);
	// �Q���v�f�ɕϊ�����
	kmb::bodyIdType convertToSecondBody(kmb::bodyIdType bodyId);
	// �ו���̗v�f�̌�
	size_t getRefinedElementCount(const kmb::ElementContainer* orgBody) const;
	// �v�f���ו�����
	// offsetElementId ��^���āArefine �̑O��ł͈قȂ� elementId ��^���Ă���
	kmb::bodyIdType refineBody(kmb::bodyIdType orgBodyId);
	// �v�f���ו�����
	// targetBodyId �̕ύX�� bodyId ��t�^���邽�߂ɂ��̃��\�b�h�̊O�ł��
	// refinedBody == NULL �̎��͍ו������v�f��o�^�����ɋ��E�����������X�V����
	size_t refineBody(const kmb::ElementContainer* orgBody, kmb::ElementContainer* refinedBody);
	// �v�f���ו��������ɒǉ����ꂽ�ߓ_�ɂ��āA���̐ߓ_��Ԃ�
	kmb::elementType getOriginal(kmb::nodeIdType middleNodeId, kmb::nodeIdType* originalNodes) const;
	// �ӂ܂��͗v�f��^���āA���̒��_���^�����Ă���΁A�����Ԃ�
	kmb::nodeIdType getMiddle(kmb::ElementBase &element) const;
	// refineBody ����Ƃ��ɓ����ɍX�V����f�[�^�̓o�^
	void appendData(const char* dataname,const char* stype=NULL);
	size_t getDataCount(void) const;
	// dataPairs �̃N���A
	void clearData(void);
	// commit ���邱�ƂŁArefineElement �̎��̍X�V�Ώۂ� refine ��� data �ɕύX����
	// commit �O : { name, stype, orgData, refinedData }
	// commit �� : { name, stype, refinedData, NULL }
	// �܂��Amesh �� name stpe �œo�^����Ă���f�[�^�� refinedData �ɒu��������
	// ���̃��\�b�h�����s�O : mesh �ɓo�^����Ă���f�[�^ = orgData
	// ���̃��\�b�h�����s�� : mesh �ɓo�^����Ă���f�[�^ = refinedData
	// �ƂȂ�
	void commitData(void);
	void setInterpolationMode(const char* mode);
	const char* getInterpolationMode(void) const;
	void setSecondFitting(bool flag);
	bool setSecondFitting(void) const;
protected:
	// body �ɒǉ�����
	// refineBody �� NULL �̂Ƃ��́A�ߓ_�O���[�v�̍X�V�������s��
	void refineSegment( kmb::elementIdType elementId, const kmb::ElementBase &segment, kmb::ElementContainer* refinedBody );
	void refineSegment2( kmb::elementIdType elementId, const kmb::ElementBase &segment2, kmb::ElementContainer* refinedBody );
	void refineTriangle( kmb::elementIdType elementId, const kmb::ElementBase &triangle, kmb::ElementContainer* refinedBody );
	void refineTriangle2( kmb::elementIdType elementId, const kmb::ElementBase &triangle2, kmb::ElementContainer* refinedBody );
	void refineQuad( kmb::elementIdType elementId, const kmb::ElementBase &quad, kmb::ElementContainer* refinedBody );
	void refineQuad2( kmb::elementIdType elementId, const kmb::ElementBase &quad2, kmb::ElementContainer* refinedBody );
	void refineTetrahedron( kmb::elementIdType elementId, const kmb::ElementBase &tetra, kmb::ElementContainer* refinedBody );
	void refineTetrahedron2( kmb::elementIdType elementId, const kmb::ElementBase &tetra2, kmb::ElementContainer* refinedBody );
	void refineHexahedron( kmb::elementIdType elementId, const kmb::ElementBase &hexa, kmb::ElementContainer* refinedBody );
	void refineHexahedron2( kmb::elementIdType elementId, const kmb::ElementBase &hexa2, kmb::ElementContainer* refinedBody );
	void refineWedge( kmb::elementIdType elementId, const kmb::ElementBase &wedge, kmb::ElementContainer* refinedBody );
	void refineWedge2( kmb::elementIdType elementId, const kmb::ElementBase &wedge2, kmb::ElementContainer* refinedBody );
	void refinePyramid( kmb::elementIdType elementId, const kmb::ElementBase &pyramid, kmb::ElementContainer* refinedBody );
	void refinePyramid2( kmb::elementIdType elementId, const kmb::ElementBase &pyramid2, kmb::ElementContainer* refinedBody );

	bool nodeGroupUpdate( kmb::nodeIdType n0, kmb::nodeIdType n1, kmb::nodeIdType n01, const kmb::DataBindings* originalData, kmb::DataBindings* refinedData);
	bool nodeGroupUpdate( const kmb::ElementBase &elem, kmb::nodeIdType centerId, const kmb::DataBindings* originalData, kmb::DataBindings* refinedData);
	bool nodeGroupUpdate( const kmb::ElementBase &elem, int faceIndex, kmb::nodeIdType centerId, const kmb::DataBindings* originalData, kmb::DataBindings* refinedData);

	bool nodeVariableUpdate( kmb::nodeIdType n0, kmb::nodeIdType n1, kmb::nodeIdType n01, const kmb::DataBindings* originalData, kmb::DataBindings* refinedData);
	bool nodeVariableUpdate( const kmb::ElementBase &elem, kmb::nodeIdType centerId, const kmb::DataBindings* originalData, kmb::DataBindings* refinedData);
	bool nodeVariableUpdate( const kmb::ElementBase &elem, int faceIndex, kmb::nodeIdType centerId, const kmb::DataBindings* originalData, kmb::DataBindings* refinedData);
};

}
