/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : ElementContainerMixedArray                              #
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
/**
 * ������̗v�f��ߓ_�z��Ɋi�[����
 *
 * nodeTable  �v�f�̐ߓ_�ԍ������ɕ��ׂ��z��
 * etypeTable �v�f�̎�ނ���ׂ��z��ichar �Ŋm�ۂ��Ă��邱�Ƃɒ��Ӂj
 * indexTable �v�f�̋�؂�̔z��̓Y��������ׂ��z��
 */
#pragma once
#include "MeshDB/kmbElementContainer.h"
#include "MeshDB/kmbElement.h"

namespace kmb{

class ElementContainerMixedArray : public ElementContainerDirectAccessable
{
public:
	static const char* CONTAINER_TYPE;
protected:
	size_t size; // �v�f�̔z��̑傫��
	kmb::nodeIdType *nodeTable; // �ߓ_�z��
	char *etypeTable; // �v�f�̎�ނ̔z��
	size_t *indexTable; // �v�f�̋�؂�� index �̔z��
	// i �Ԗڂ̗v�f��
	// �v�f�̎�� etype = etypeTable[i] 
	// �z��̋�؂� j = indexTable[i]
	// len = kmb::Element::getNodeCount( etype )
	// nodeTable[j] ... nodeTable[j+len-1]
	bool nodeTableDeletable;  // �ߓ_�z������̃N���X�� Delete ���Ă��悢���ǂ���
	bool etypeTableDeletable;  // �v�f��ޔz������̃N���X�� Delete ���Ă��悢���ǂ���
	bool indexTableDeletable;  // �v�f��؂�z������̃N���X�� Delete ���Ă��悢���ǂ���
	kmb::nodeIdType nodeOffset; // �ߓ_�ԍ������炵�ēo�^����ꍇ
	size_t index; // ���ɓo�^����v�f��ޔz��̓Y�����i�o�^�ς݂̌��Ƃ͈Ⴄ�j
	size_t nindex; // ���ɓo�^����ߓ_�z��̓Y�����i�o�^�ς݂̌��Ƃ͈Ⴄ�j
	size_t count; // ���ɓo�^�ς݂̌�
public:
	ElementContainerMixedArray(size_t typeCounter[kmb::ELEMENT_TYPE_NUM]);
	// �]�T�������� eSize * MAX_NODE_COUNT �ŏ���������
	ElementContainerMixedArray(size_t eSize);
	// writable = true �͊m�ۂ����z�񂾂��n���āA�v�f���i�[����̂͂��̃N���X�ōs��
	// writable = false �͂��łɗv�f���i�[�����z���n���āA���b�p�Ƃ��Ă��̃N���X���g��
	ElementContainerMixedArray(size_t eSize,char* etype, kmb::nodeIdType *nodeTable, bool writable=false, kmb::nodeIdType offset=0 );
	// �v�f��ޔz�񂪗^�����Ă���Ƃ�
	ElementContainerMixedArray(size_t eSize,char *etype,kmb::nodeIdType offset=0);
	virtual ~ElementContainerMixedArray(void);
	virtual kmb::elementIdType addElement(kmb::elementType etype, const kmb::nodeIdType *nodes);
	virtual kmb::elementIdType addElement(kmb::elementType etype, const kmb::nodeIdType *nodes, const kmb::elementIdType id);
	virtual bool getElement(kmb::elementIdType id,kmb::elementType &etype,kmb::nodeIdType *nodes) const;
	virtual bool deleteElement(const kmb::elementIdType id);
	virtual bool includeElement(const kmb::elementIdType id) const;
	virtual kmb::elementIdType getMaxId(void) const;
	virtual kmb::elementIdType getMinId(void) const;
	virtual size_t getCount(void) const;
	virtual void initialize(size_t eSize=0);
	void initialize(size_t typeCounter[kmb::ELEMENT_TYPE_NUM]);
	virtual void clear(void);
	virtual const char* getContainerType(void) const;

	class _iteratorMA : public ElementContainer::_iterator
	{
		friend class ElementContainerMixedArray;
	public:
		virtual kmb::elementIdType getId(void) const;
		virtual bool getElement(kmb::elementType &etype,kmb::nodeIdType *nodes) const;
		virtual kmb::elementType getType(void) const;
		virtual kmb::nodeIdType getCellId(int cellIndex) const;
		virtual bool setCellId(int cellIndex, kmb::nodeIdType nodeId);
		virtual kmb::nodeIdType operator[](const int i) const;
		virtual ElementContainer::_iterator* operator++(void);
		virtual ElementContainer::_iterator* operator++(int n);
		virtual ElementContainer::_iterator* clone(void);
	private:
		_iteratorMA(void);
		virtual ~_iteratorMA(void);
		size_t index;
		const ElementContainerMixedArray* elementContainer;
	};

	virtual iterator begin(void);
	virtual const_iterator begin(void) const;
	virtual iterator find(kmb::elementIdType id);
	virtual const_iterator find(kmb::elementIdType id) const;

	size_t getNodeTableSize(void) const;
	kmb::elementType getElementType(kmb::elementIdType id) const;
	// direct access
	// nodeOffset �͍l������Ȃ��̂Œ���
	// elementOffset �͍l�������
	// �v�f��ޔz�񂪎��O�ɗ^�����Ă��鎞�������������삷��
	kmb::nodeIdType operator()(kmb::elementIdType elementId,kmb::idType localId) const;
	kmb::nodeIdType& operator()(kmb::elementIdType elementId,kmb::idType localId);
	void commit(kmb::elementIdType elementId);
private:
	static size_t getRequiredNodeTableSize(const size_t typeCounter[kmb::ELEMENT_TYPE_NUM]);
	static size_t getRequiredElementSize(const size_t typeCounter[kmb::ELEMENT_TYPE_NUM]);
};

}
