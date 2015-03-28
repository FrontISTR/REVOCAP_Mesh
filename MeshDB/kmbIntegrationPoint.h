/*----------------------------------------------------------------------
#                                                                      #
# Software Name : Advance/REVOCAP version 3.2                          #
# Class Name : IntegrationPoint                                        #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2013/08/22     #
#                                                                      #
----------------------------------------------------------------------*/
#pragma once

#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElement.h"

/**
 * �ϕ��_�ł̕����ʂ��Ǘ����邽�߂̗v�f�ԍ��Ɛϕ��_�̃C���f�b�N�X�̑g
 */

namespace kmb{

class IntegrationPoint
{
public:
	kmb::elementIdType elementId;
	kmb::idType integrationIndex;
	IntegrationPoint(kmb::elementIdType _elementId=kmb::Element::nullElementId, kmb::idType _integrationIndex=kmb::nullId);
	~IntegrationPoint(void);

	bool operator<(const IntegrationPoint &other)const{
		return (elementId < other.elementId) ||
			(elementId==other.elementId && integrationIndex < other.integrationIndex);
	}
	bool operator==(const IntegrationPoint &other)const{
		return (elementId == other.elementId) && (integrationIndex == other.integrationIndex);
	}
	bool operator!=(const IntegrationPoint &other)const{
		return (elementId != other.elementId) || (integrationIndex != other.integrationIndex);
	}
	IntegrationPoint& operator=(const IntegrationPoint &other){
		elementId = other.elementId;
		integrationIndex = other.integrationIndex;
		return *this;
	}
};

}
