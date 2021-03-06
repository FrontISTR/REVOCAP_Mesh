/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : ElementContainer                                        #
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

#include "MeshDB/kmbElement.h"
#include "Geometry/kmbBoundingBox.h"
#include "Geometry/kmbPoint3DContainer.h"
#include "Geometry/kmbFramedPlane.h"
#include <set>
#include <map>
#include <string>

#include "MeshDB/kmbTypes.h"

namespace kmb{

class ElementContainer
{
protected:
	size_t typeCounter[kmb::ELEMENT_TYPE_NUM];
	kmb::BoundingBox boundingBox;
	kmb::elementIdType offsetId;
	std::string bodyName;
public:
	ElementContainer(void);
	virtual ~ElementContainer(void);
	// nodes は再利用可能なポインタとする
	// コンテナ側からは必ずコピーして使うこと
	virtual kmb::elementIdType addElement(kmb::elementType etype, const kmb::nodeIdType *nodes) = 0;
	virtual kmb::elementIdType addElement(kmb::elementType etype, const kmb::nodeIdType *nodes, const kmb::elementIdType id) = 0;
	virtual bool getElement(kmb::elementIdType id,kmb::elementType &etype,kmb::nodeIdType *nodes) const = 0;
	/// deleteElement は pointer を delete する
	virtual bool deleteElement(const kmb::elementIdType id) = 0;
	virtual bool includeElement(const kmb::elementIdType id) const = 0;
	virtual kmb::elementIdType getMaxId(void) const = 0;
	virtual kmb::elementIdType getMinId(void) const = 0;
	virtual size_t getCount(void) const = 0;
	virtual void initialize(size_t size=0) = 0;

	// 拡張クラスで定義する場合は親クラスの clear を呼ぶこと
	virtual void clear(void);

	// 拡張コンテナでより良い実装があれば override する
	// addElement したら、element のメモリ管理は ElementContainer 側で行うため、
	// 呼び出し側での管理は不要
	virtual kmb::elementIdType addElement(const kmb::Element* element);
	virtual kmb::elementIdType addElement(const kmb::Element* element,const kmb::elementIdType id);
	virtual void updateBoundingBox(const kmb::Point3DContainer* points);
	/// eraseElement は pointer を delete しない
	virtual kmb::Element* eraseElement(kmb::elementIdType id);

	virtual const char* getContainerType(void) const = 0;

	// non virtual container methods
	kmb::elementIdType getOffsetId(void) const;
	void setOffsetId(kmb::elementIdType id);
	size_t getCountByType(kmb::elementType elementType) const;
	void getCountOfDim(int count[3]) const;
	int getDimension(void) const;
	int getDegree(void) const;
	bool isUniqueDim(int dim) const;
	bool isUniqueType(kmb::elementType type) const;
	void getNodesOfBody(const kmb::Point3DContainer* points,kmb::Point3DContainer* result) const;
	void getNodesOfBody(std::set<kmb::nodeIdType> &result) const;
	void getMaxPoint(kmb::Point3D& pt) const;
	void getMinPoint(kmb::Point3D& pt) const;
	const kmb::BoundingBox& getBoundingBox(void) const;
	void setBoundingBox(const kmb::Point3D& max,const kmb::Point3D& min);
	// 節点番号の置き換え
	// 置き換えた要素の個数を返す
	int replaceNodeId(kmb::nodeIdType oldNodeId,kmb::nodeIdType newNodeId);
	int replaceNodeId(std::map<kmb::nodeIdType,kmb::nodeIdType>& nodeMapper);
public:
	class _iterator
	{
	public:
		virtual ~_iterator(){};
		// offset する前の Id
		virtual kmb::elementIdType getId(void) const = 0;
		virtual kmb::Element* getElement(void){ return NULL; };
		virtual const kmb::Element* getElement(void) const{ return NULL; };
		virtual bool getElement(kmb::elementType &etype,kmb::nodeIdType *nodes) const = 0;
		virtual kmb::elementType getType(void) const = 0;
		virtual kmb::nodeIdType getNodeId(int index) const = 0;
		virtual bool setNodeId(int cellIndex, kmb::nodeIdType nodeId) = 0;
		virtual kmb::nodeIdType operator[](const int i) const = 0;
		virtual _iterator* operator++(void) = 0;
		virtual _iterator* operator++(int n) = 0;
		virtual _iterator* clone(void) = 0; // 代入演算子の定義に必要
	};

public:
	class const_iterator;
	class iterator : public kmb::ElementBase
	{
		friend class ElementContainer;
		friend class ElementContainer::const_iterator;
	private:
		_iterator* _iter;
		kmb::elementIdType offsetId;
	public:
		iterator(_iterator* _it=NULL,kmb::elementIdType id=0)
			: _iter(_it)
			, offsetId(id){}
		iterator( const iterator &other );
		virtual ~iterator(void);
		kmb::elementIdType getId(void) const;
		kmb::Element* getElement(void) const;
		bool getElement(kmb::elementType &etype,kmb::nodeIdType *nodes) const;
		bool getElement(kmb::Element &element) const;
		kmb::elementType getType(void) const;
		kmb::nodeIdType getNodeId(int cellIndex) const;
		bool setNodeId(int cellIndex, kmb::nodeIdType nodeId);
		virtual kmb::nodeIdType operator[](const int i) const;
		// 代入
		iterator& operator=(const iterator& other);
		iterator& operator++(void);  // 前置 ++
		iterator  operator++(int n); // 後置 ++
		bool operator==(const iterator &other) const;
		bool operator!=(const iterator &other) const;
		// 終了判定 == end() でもいいけど、こっちの方が速い
		bool isFinished(void){	return (_iter==NULL);	}
	};
	class const_iterator : public kmb::ElementBase
	{
		friend class ElementContainer;
	private:
		_iterator* _iter;
		kmb::elementIdType offsetId;
	public:
		const_iterator(_iterator* _it=NULL,kmb::elementIdType id=0)
			: _iter(_it)
			, offsetId(id){}
		const_iterator( const const_iterator &other );
		const_iterator( const iterator &other );
		virtual ~const_iterator(void);
		kmb::elementIdType getId(void) const;
		const kmb::Element* getElement(void) const;
		bool getElement(kmb::elementType &etype,kmb::nodeIdType *nodes) const;
		bool getElement(kmb::Element &element) const;
		kmb::elementType getType(void) const;
		kmb::nodeIdType getNodeId(int cellIndex) const;
		bool setNodeId(int cellIndex, kmb::nodeIdType nodeId);
		virtual kmb::nodeIdType operator[](const int i) const;
		// 代入
		const_iterator& operator=(const const_iterator& other);
		const_iterator& operator=(const iterator& other);
		const_iterator& operator++(void);  // 前置 ++
		const_iterator  operator++(int n); // 後置 ++
		bool operator==(const const_iterator &other) const;
		bool operator!=(const const_iterator &other) const;
		// 終了判定 == end() でもいいけど、こっちの方が速い
		bool isFinished(void){	return (_iter==NULL);	}
	};
private:
	static const iterator endIterator; // NULL 値
	static const const_iterator endConstIterator; // NULL 値
public:
	virtual iterator begin(void) = 0;
	virtual const_iterator begin(void) const = 0;
	virtual iterator find(kmb::elementIdType id) = 0;
	virtual const_iterator find(kmb::elementIdType id) const = 0;
	// 値を返すと返すたびにコンストラクタとデストラクタが呼ばれるよ
	// 参照返しにする
	const iterator& end(void){
		return ElementContainer::endIterator;
	};
	const const_iterator& end(void) const{
		return ElementContainer::endConstIterator;
	};
	static const iterator& getEndIterator(void){
		return ElementContainer::endIterator;
	};
	static const const_iterator& getEndConstIterator(void){
		return ElementContainer::endConstIterator;
	};
public:
	void setBodyName(const char* name);
	const char* getBodyName(void) const;
	void setName(const char* name);
	const char* getName(void) const;
public:
	static const kmb::bodyIdType nullBodyId;
	static bool copy(const kmb::ElementContainer* org, kmb::ElementContainer* elements);
	static kmb::ElementContainer* createContainer( const char* containerType, size_t size );
};

// direct access
class ElementContainerDirectAccessable : public ElementContainer{
public:
	virtual kmb::elementType getElementType(kmb::elementIdType elementId) const = 0;
	virtual kmb::nodeIdType operator()(kmb::elementIdType elementId,kmb::idType localId) const = 0;
	virtual kmb::nodeIdType& operator()(kmb::elementIdType elementId,kmb::idType localId) = 0;
	// elementId までの値の設定を行って、iterator で参照できるようにする
	// 注：size は elementId + 1 である
	virtual void commit(kmb::elementIdType elementId) = 0;
};


typedef ElementContainer Body;

}
