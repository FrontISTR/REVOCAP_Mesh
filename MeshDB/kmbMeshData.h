/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MeshData                                                #
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
#pragma once

#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElement.h"
#include "MeshDB/kmbPhysicalValue.h"
#include "MeshDB/kmbDataBindings.h"
#include "MeshDB/kmbElementContainer.h"
#include "MeshDB/kmbElementOctree.h"

#include "Geometry/kmbBoundingBox.h"
#include "Geometry/kmbPoint3DContainer.h"
#include "Geometry/kmbPoint2DContainer.h"
#include "Geometry/kmbOctree.h"
#include "Common/kmbCalculator.h"

#include <vector>
#include <string>

namespace kmb{

class Matrix4x4;

typedef std::multimap< std::string, kmb::DataBindings* > datamap;

class MeshData
{
protected:
	kmb::Point3DContainer* node3Ds;
	std::vector< kmb::ElementContainer* > bodies;
	kmb::ElementContainer* currentBody;
	std::multimap< std::string, kmb::DataBindings* > bindings;
	// �ꎞ�I�ȃf�[�^�ۑ��p
	std::vector< kmb::DataBindings* > bindingsStack;
	int bindingsStackDim;
	std::string defaultSpecType;
	// the element ids are not duplicated even if they are not contained in different bodies
	elementIdType maxElementId;
	// coordinate system
	kmb::Matrix4x4* coordMatrix;
public:
	MeshData(void);
	virtual ~MeshData(void);
	// mesh �� new ������Ԃɖ߂�
	virtual void clearModel(void);
	// ���f���S�̂̓o�^���J�n
	// ���N���X�ł͓��ɉ������Ȃ�
	// �h���N���X�ł͖ړI�ɉ����ĊO���̏��������[�`�����R�[���o�b�N����Ȃ�
	virtual void beginModel(void);
	// ���f���S�̂̓o�^�̏I��
	// ���N���X�ł͓��ɉ������Ȃ�
	// �h���N���X�ł͖ړI�ɉ����ĊO���̏��������[�`�����R�[���o�b�N����Ȃ�
	virtual void endModel(void);
	//-------------- ���W�n ---------------------//
	// �R�s�[���Ďg��
	void setCoordinate( const kmb::Matrix4x4& matrix );
	void translateCoordinate( double x, double y, double z );
	kmb::Matrix4x4* getCoordMatrix(void);
	const kmb::Matrix4x4* getCoordMatrix(void) const;
	//-------------- �ߓ_�Ǘ� -------------------//
	int getNodeDim(void) const;
	// �ߓ_�̓o�^�J�n
	// �R���e�i���w�肷�邱�Ƃ��ł���
	virtual void beginNode(size_t size=0,const char* containerType=NULL);
	virtual void beginNode(size_t size,kmb::Point3DContainer* point3Ds);
	// �ߓ_�̓o�^
	virtual kmb::nodeIdType addNode(double x,double y,double z);
	// �ߓ_�̓o�^�iId�t���j
	virtual kmb::nodeIdType addNodeWithId(double x,double y,double z, kmb::nodeIdType id);
	// �ߓ_�̓o�^�I��
	// ���N���X�ł͉������Ȃ�
	// �h���N���X�̃R�[���o�b�N�Ŏg��
	virtual void endNode(void);
	// �ߓ_�ԍ��̍ő�ŏ�
	// ���݂��Ȃ��Ƃ��� max < min
	// ���ɔԍ����w�肹���ɒǉ������Ƃ��ɂ� max + 1
	kmb::nodeIdType getMaxNodeId(void) const;
	kmb::nodeIdType getMinNodeId(void) const;
	// �ߓ_�R���e�i�̎擾
	// each ���\�b�h���̓R���e�i���擾���Ă���
	const kmb::Point3DContainer* getNodes(void) const;
	kmb::Point3DContainer* getNodes(void);
	virtual const kmb::Point2DContainer* getNode2Ds(void) const{ return NULL; };
	kmb::Point3DContainer* replaceNodes(kmb::Point3DContainer* nodes);
	// �ߓ_��
	virtual size_t getNodeCount(void) const;
	// �ߓ_�𒼐� Point3D �^�Ŏ擾
	virtual bool getNode(nodeIdType i,kmb::Point3D &node) const;
	bool getNodeXYZ(nodeIdType i,double &x,double &y,double &z) const;
	// endNode ������Őߓ_�̒ǉ�
	// ���s������ kmb::nullNodeId ��Ԃ�
	virtual kmb::nodeIdType insertNode(double x,double y,double z);
	// endNode ������Őߓ_�̒ǉ� (id �t��)
	// ���s������ kmb::nullNodeId ��Ԃ�
	virtual kmb::nodeIdType insertNodeWithId(double x,double y,double z, kmb::nodeIdType id);
	// ���ɓo�^�ς݂̐ߓ_�̍��W���X�V����
	virtual bool updateNode(double x,double y,double z, kmb::nodeIdType id);
	// �R���e�i�^�C�v
	const char* getNodeContainerType(void) const;
	virtual const kmb::BoundingBox getBoundingBox(void) const;
	//-------------- �v�f�Ǘ� -------------------//
	// �v�f�̓o�^�J�n
	// set suitable container type
	// �ȍ~ endElement ���Ă΂��܂� currentBody �ɓo�^�����
	virtual kmb::bodyIdType beginElement(size_t size,kmb::ElementContainer* container);
	virtual kmb::bodyIdType beginElement(size_t size=0,const char* containerType=NULL);
	// �v�f�̒ǉ�
	virtual kmb::elementIdType addElement(kmb::elementType type,kmb::nodeIdType *ary);
	// �v�f�̒ǉ��iId�t���j
	virtual kmb::elementIdType addElementWithId(kmb::elementType type,kmb::nodeIdType *ary,kmb::elementIdType elementId);
	// �v�f�̓o�^�I��
	virtual void endElement(void);
	// max element id means that larger id is not used
	kmb::elementIdType getMaxElementId(void) const;
	// beginElement endElement �̌�ŗv�f�̒ǉ�
	virtual kmb::elementIdType insertElement(kmb::bodyIdType bodyID,kmb::elementType type,kmb::nodeIdType *ary);
	virtual kmb::elementIdType insertElementWithId(kmb::bodyIdType bodyID,kmb::elementType type,kmb::nodeIdType *ary,kmb::elementIdType elementId);
	// �o�^�ς݂̗v�f�̈ړ�
	virtual bool moveElement(kmb::bodyIdType bodyId0,kmb::bodyIdType bodyId1,kmb::elementIdType elementId);
	// �v�f�O���[�v�̌��擾
	kmb::bodyIdType getBodyCount(void) const;
	// �v�f�ԍ�����v�f�O���[�v�ԍ��̎擾
	bodyIdType getBodyId(elementIdType elementId) const;
	// �v�f�O���[�v�R���e�i�̒ǉ��E�u��
	// offset �t���O�͒ǉ�����R���e�i�� offset �������I�ɏ���������
	virtual kmb::bodyIdType appendBody( kmb::Body* body, bool offset=true );
	virtual kmb::bodyIdType replaceBody( kmb::bodyIdType bodyId, kmb::Body* body, bool offset=true );
	// MeshData �̊O�� ElementContainer ���X�V�����Ƃ���
	// MeshData ���������X�V���邽��
	// beginElement endElement �ŉ��� Body ������� insertElement �����ꍇ�́A�Ō�ɂ��̃��\�b�h���Ăяo���čX�V���Ă���
	virtual bool updateBody( kmb::bodyIdType bodyId );
	// �v�f�O���[�v�̒��g����ɂ��邾���ŁAbodyId �͏����Ȃ�
	virtual void clearBody(bodyIdType bodyId);
	// ���ׂĂ̗v�f�O���[�v����������
	virtual void removeAllBodies(void);
	// �v�f�O���[�v�̃R���e�i�擾
	kmb::Body* getBodyPtr(bodyIdType bodyId);
	const kmb::Body* getBodyPtr(bodyIdType bodyId) const;
	// �v�f�O���[�v�Ɋ܂܂��v�f�̌��̎擾
	size_t getElementCount(kmb::bodyIdType bodyId) const;
	size_t getElementCount(void) const;
	size_t getElementCountOfDim(int dim) const;
	// �v�f�O���[�v�̎����i�\�ʃ��b�V���Ȃ�2�A�̐σ��b�V���Ȃ�3�j
	int getDimension(kmb::bodyIdType bodyId) const;
	// �v�f�O���[�v�̍ő原���i�\�ʃ��b�V���Ȃ�2�A�̐σ��b�V���Ȃ�3�j
	int getDimension(void) const;
	// �v�f�O���[�v�̎����i2���v�f�Ȃ�2�j
	int getDegree(kmb::bodyIdType bodyId) const;
	// �v�f�O���[�v�Ɋ܂܂��v�f�̃^�C�v����ӂ��ǂ���
	bool isUniqueElementType(kmb::bodyIdType bodyId,kmb::elementType etype) const;
	// �v�f�̎擾
	kmb::ElementContainer::iterator findElement(elementIdType elementID,bodyIdType bodyID=kmb::Body::nullBodyId);
	kmb::ElementContainer::const_iterator findElement(elementIdType elementID,bodyIdType bodyID=kmb::Body::nullBodyId) const;
	// �v�f�O���[�v��
	virtual const char* getBodyName(bodyIdType bodyId) const;
	virtual void setBodyName(bodyIdType bodyId,const char* name);
	kmb::bodyIdType getBodyIdByName(const char* name) const;
	// �R���e�i�^�C�v
	const char* getElementContainerType(bodyIdType bodyId) const;
	// �O�����b�V������v�f�O���[�v�̒ǉ�
	kmb::bodyIdType importBody(const kmb::MeshData& otherMesh,kmb::bodyIdType bodyId);
	//---------------------------------------------//
	//-------------- �����ʊǗ� -------------------//
	//---------------------------------------------//
	virtual const std::multimap< std::string, kmb::DataBindings* >& getDataBindingsMap(void) const;
	kmb::datamap::iterator beginDataIterator();
	kmb::datamap::const_iterator beginDataIterator() const;
	kmb::datamap::iterator endDataIterator();
	kmb::datamap::const_iterator endDataIterator() const;
	// �����ʂ̐����i�h���N���X�Ŏ����j
	virtual kmb::DataBindings* createDataBindings(
		const char* name,
		kmb::DataBindings::bindingMode bmode,
		kmb::PhysicalValue::valueType vtype,
		const char* stype="",
		kmb::bodyIdType targetBodyId=kmb::Body::nullBodyId);
	// �����ʃf�[�^�̎擾
	kmb::DataBindings* getDataBindingsPtr(const char* name,const char* stype=NULL);
	const kmb::DataBindings* getDataBindingsPtr(const char* name,const char* stype=NULL) const;
	// �����ʃf�[�^�̐ݒ�
	bool setDataBindingsPtr(const char* name,kmb::DataBindings* data,const char* stype=NULL);
	// �o�^����Ă��镨���ʂ̎�ނ̌�
	virtual size_t getDataCount(const char* stype=NULL) const;
	bool hasData(const char* name,const char* stype=NULL) const;
	bool renameData(const char* oldname,const char* newname,const char* stype=NULL);
	bool deleteData(const char* name,const char* stype=NULL);
	bool clearData(const char* name,const char* stype=NULL);
	// name �ɓo�^����Ă���f�[�^�̒u������
	// �g��Ȃ��Ȃ��� olddata �͂��̃��\�b�h�̊O�� delete ���邱��
	// olddata �� NULL �ł����Ă͂����Ȃ����Anewdata �� NULL �̏ꍇ����
	bool replaceData(const kmb::DataBindings* olddata, kmb::DataBindings* newdata, const char* name,const char* stype=NULL);
	// id �̌�
	size_t getIdCount(const char* name,const char* stype=NULL) const;
	// stype ���ȗ������Ƃ��ɉ����g���������߂Ă���
	void setDefaultSpecType(const char* specType);
	// �o�^����Ă��镨���ʂ̐ݒ胂�[�h��Ԃ�
	kmb::DataBindings::bindingMode getDataMode(const char* name, const char* stype=NULL) const;
	// �o�^����Ă��镨���ʂ̒l�̌^��Ԃ�
	kmb::PhysicalValue::valueType getDataValueType(const char* name, const char* stype=NULL) const;
	// �o�^����Ă��镨���ʂ̎��ʎq(SpecType)��Ԃ�
	std::string getDataSpecType(const char* name) const;
	// �o�^����Ă��镨���ʂ̎��ʎq(SpecType)��ύX����
	bool setDataSpecType(const char* name,const char* oldstype,const char* newstype);
	// �R���e�i�^�C�v
	const char* getDataContainerType(const char* name,const char* stype=NULL) const;
	// Id �̓o�^
	virtual void addId(const char* name,kmb::idType id,const char* stype=NULL);
	virtual void addId(const char* name,kmb::Face f,const char* stype=NULL);
	// id ���o�^����Ă��邩�ǂ���
	// �����ʂɒl��o�^
	virtual void setPhysicalValue(const char* name,kmb::PhysicalValue* val,const char* stype=NULL);
	virtual void setPhysicalValueAtId(const char* name,kmb::idType id,kmb::PhysicalValue* val,const char* stype=NULL);
	virtual void setPhysicalValueAtId(const char* name,kmb::Face f,kmb::PhysicalValue* val,const char* stype=NULL);
	// �����ʂ��擾
	virtual const kmb::PhysicalValue* getPhysicalValue(const char* name,const char* stype=NULL) const;
	virtual const kmb::PhysicalValue* getPhysicalValueAtId(const char* name,kmb::idType id,const char* stype=NULL) const;
	virtual const kmb::PhysicalValue* getPhysicalValueAtId(const char* name,kmb::Face f,const char* stype=NULL) const;
	virtual bool getPhysicalValueAtId(const char* name,kmb::idType id,double *val,const char* stype=NULL) const;
	virtual bool getPhysicalValueAtId(const char* name,kmb::Face f,double *val,const char* stype=NULL) const;
	// ���������o�^
	void appendTargetData(const char* name, const char* stype=NULL);
	void appendTargetDataPtr(kmb::DataBindings* data);
	void clearTargetData(void);
	int getTargetDataNum(void) const;
	int getTargetDataDim(void) const;
	virtual void setMultiPhysicalValues(kmb::idType id, std::vector< kmb::PhysicalValue* > &values);
	virtual void setMultiPhysicalValues(kmb::idType id, double* values);
	virtual void setMultiPhysicalValues(kmb::idType id, long* values);
	virtual void setMultiPhysicalValues(double* values);
	virtual void getMultiPhysicalValues(kmb::idType id, double* values) const;
	// auto name generation such as "prefix_0" "prefix_1"
	std::string getUniqueDataName(std::string prefix,int num=0);
	// �o�^����Ă���f�[�^�� ElementGroup / ElementVariable ������΁A
	// orgElementId �̃f�[�^�� elementId �������p��
	void deriveTargetData(kmb::elementIdType elementId,kmb::elementIdType orgElementId);

	// �ϊ��n�F�f�[�^����{�f�B�𐶐����ēo�^
	kmb::bodyIdType faceGroupToBody(const char* faceG,const char* stype=NULL);
	// �ϊ��n�F�f�[�^����{�f�B�𐶐����ēo�^�A�l�͗v�f�l�ɓo�^
	kmb::bodyIdType faceVariableToBody(const char* faceV,const char* elemV,const char* stype=NULL);
	// �ϊ��n�F�f�[�^����w�肳�ꂽ�{�f�B�ɒǉ��i�{�f�B�̒ǉ��͂��Ȃ��j
	size_t faceGroupToBody(const char* faceG,kmb::ElementContainer* body, const char* stype=NULL);
	size_t faceVariableToBody(const char* faceV,kmb::ElementContainer* body,const char* stype=NULL);
	// get node ids of binding data
	void getNodeSetFromDataBindings(std::set<kmb::nodeIdType>&nodeSet,const char* name,const char* stype=NULL) const;
	void getBoundingBoxOfData(kmb::BoundingBox &bbox,const kmb::DataBindings* data) const;
	// ���炩���߃f�[�^�͍���Ă�������
	int convertData(const char* org, const char* conv, const char* orgstype=NULL,const char* convstype=NULL);
	int convertData(const kmb::DataBindings* orgData, kmb::DataBindings* convData);
	// �����Z subtrahend �Ɋ܂܂�� Id ���@minuend ����̂���
	// FaceGroup NodeGroup ElementGroup �ɑΉ�
	int subtractData(const char* subt, const char* minu, const char* subtstype=NULL,const char* minustype=NULL);
	// Body �Ɋ܂܂��ߓ_�����ׂĐߓ_�O���[�v�ɂ���
	// Body �� FaceGroup ElementGroup �ɕϊ�����
	int convertBodyToData(kmb::bodyIdType bodyId, const char* name,const char* stype=NULL);

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4100)
#endif

#ifdef __INTEL_COMPILER
#pragma warning(push)
#pragma warning(disable:869)
#endif

	// (x,y,z) ���狗�� tolerance �ȉ��ōł��߂��v�f��T��
	// solid �v�f�̎� tolerance = 0.0 �Ƃ���ƁA(x,y,z) ���܂ޓ_��T��
	virtual kmb::elementIdType searchElement(kmb::bodyIdType bodyId,double x,double y,double z,double tolerance=0.0) const{ return kmb::Element::nullElementId; };

	// 0����ԁF�ł��߂��ߓ_�ł̒l�����o��
	// NodeVariable �̂ݑΉ�
	virtual int getValueOnNearestNode(const char* name,double x,double y,double z,double *values,const char* stype=NULL) const { return -1; };
	// ��Ԓl���v�Z����
	virtual int getInterpolatedValueInBody(const char* name,kmb::bodyIdType bodyId,double x,double y,double z,double *values,double tolerance=0.0,const char* stype=NULL) const { return -1; };
	virtual int getInterpolatedValue(const char* name,double x,double y,double z,double *values,double tolerance=0.0,const char* stype=NULL) const { return -1; };
	virtual int getInterpolatedValueInData(const char* name,const char* target,double x,double y,double z,double *values,double tolerance=0.0,const char* stype=NULL) const{ return -1; };
	// TargetData �ŃL���b�V�����Ă���f�[�^�ɂ��ĕ�Ԓl�����߂�
	// �߂�l�� values �ɓ��ꂽ���l�̎���
	// ��Ԃł��Ȃ������� 0 ���A���̑��̃G���[�� -1 ��Ԃ�
	virtual int getInterpolatedValuesInBody(kmb::bodyIdType bodyId,double x,double y,double z,double *values) const { return -1; };
	virtual int getInterpolatedValues(double x,double y,double z,double *values) const { return -1; };
	virtual int getInterpolatedValuesInData(const char* target,double x,double y,double z,double *values,double tolerance=0.0) const{ return -1;};

#if defined _MSC_VER || defined __INTEL_COMPILER
#pragma warning(pop)
#endif

};

}
