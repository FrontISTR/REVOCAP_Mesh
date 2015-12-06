/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : SurfaceOperation                                        #
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
//
// 2�����v�f����Ȃ�\�ʃ��b�V���i�p�b�`�j�ɑ΂���􉽏����E�ʑ�����
//

#pragma once

#include <map>
#include <vector>
#include <string>
#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElement.h"
#include "Common/kmbClassification.h"

namespace kmb{

class MeshData;
class Face;
class ElementContainer;
class NodeNeighborInfo;
class Point3DContainer;
class DataBindings;

class SurfaceOperation
{
protected:
	kmb::MeshData* mesh;
	kmb::ElementContainer* elements;
	kmb::NodeNeighborInfo* neighborInfo;
	kmb::Classification< kmb::elementIdType > dividedElementsInfo;
	std::set< std::pair<kmb::elementIdType, kmb::elementIdType> > edgesBetweenElements;
	kmb::Classification< kmb::Face > dividedFacesInfo;
	std::set< std::pair<kmb::Face,kmb::Face> > edgesBetweenFaces;
	// �\�ʂ���ʃO���[�v���č\������Ƃ��ɋt�����̕\�ʂ��������
	bool permitReverse;
public:
	SurfaceOperation(void);
	virtual ~SurfaceOperation(void);
	void setMesh(kmb::MeshData* mesh);
	void setPermitReverse(bool f);
	//
	// divide surface by threshold of angles
	// surfIDs = array of divided surface id
	// original body Id = all frames
	//
	// if domain of body id does not consist of 2-dim element, do nothing
	//
	// WithRidge
	// surfIDs[divided surface id] = ridge id
	//
	void divideSurface(bodyIdType bodyId,double angle,std::vector<kmb::bodyIdType> &surfIDs);
	void divideSurfaceWithRidge(bodyIdType bodyId,double angle,std::map<kmb::bodyIdType,kmb::bodyIdType> &surfIDs);	
	// 
	// divide face group by threshold of angles
	// the names of divided face groups are postfixed "1" such as "face_1". 
	//
	// divideFaceGroup �͕����̋��E�̗Ő��W���� Body �Ƃ��ĕۑ����āA���� bodyId ��Ԃ�
	// divideFaceGroupWithRidge �͕������� FaceGroup �Ƃ��̗Ő��� BodyId �̘A�z�z�������
	// key = FaceGroup �̖��O�Avalue = ������͂ޗŐ��W���� BodyId
	// FaceGroup �� stype �� "Brep" �ɂ���B
	// �߂�l�͑S�̗̂Ő��� BodyId
	kmb::bodyIdType divideFaceGroup(std::string name,double angle,std::vector<std::string> &subFaceGroups);
	kmb::bodyIdType divideFaceGroupWithRidge(std::string name,double angle,std::map<std::string,kmb::bodyIdType> &subFaceGroupMapper);
	// divide => ������蕪������i�p�x��臒l�ɂ���j
	// decompose => ���R�ɕ������
	// �A�������ɕ�������
	void decomposeSurface(kmb::bodyIdType bodyID, std::set< kmb::bodyIdType > &components);
	// FaceGroup ��A�������ɕ�������
	bool decomposeFaceGroup(std::string faceGroup, std::vector<std::string> &subFaceGroups);
	// �A���������Ƃɕ����鉺�������\�b�h
	// element id �͕ۑ�
	void decomposeByElementContainer(kmb::ElementContainer* elements,std::set< kmb::ElementContainer* > &components);
	void clear(void);

	void setSurface(kmb::MeshData* mesh,kmb::bodyIdType bodyId);
	enum vertexType{
		Unknown,
		CONVEX,     // ���_�̎���̗v�f�̊p�x�̘a�� 360 �x��菬��������
		CONCAVE,    // ���_�̎���̗v�f�̊p�x�̘a�� 360 �x��菬��������
		FLAT,       // ���_�̎���̗v�f�̊p�x�̘a�� 360 �x
		HYPERBOLIC  // ���_�̎���̗v�f�̊p�x�̘a�� 360 �x���傫��
	};
	// NodeNeighborInfo ���O����^����ꍇ
	// ElementContainer �� Element2D �݂̂���Ȃ邱�Ƃ̓m�[�`�F�b�N
	static vertexType getVertexType
		(kmb::nodeIdType nodeId,
		 kmb::MeshData* mesh,
		 kmb::ElementContainer* elements,
		 kmb::NodeNeighborInfo* neighborInfo);
	vertexType getVertexType(kmb::nodeIdType nodeId) const;

	kmb::bodyIdType faceGroup2Surface(const char* name);
	bool surface2FaceGroup(kmb::bodyIdType bodyId,kmb::bodyIdType parentBodyId,const char* name);
	// parentFaceGroup �ŗ^������ face �̂����AbodyId �ŗ^������ʂƋ��ʂ̂��̂� name ��
	// face group �ɓo�^����
	bool surface2FaceGroup(kmb::bodyIdType bodyId,const char* parentFaceGroup,const char* name);
protected:
	// append data to divideElementsInfo
	// return the number of divided groups
	size_t divideSurface(const kmb::ElementContainer* elements,const kmb::Point3DContainer* points,double angle);
	// ���������ʂ̎O�p�`�Ǝl�p�`�̌�
	// 2���v�f�������悤�ɐ�����
	bool getDividedSurfaceElementCount(const kmb::ElementContainer* orgSurf,kmb::elementIdType initElementId,size_t &triCount,size_t &quadCount) const;
	// get divided surface included an element with initElementId as id
	size_t getDividedSurface(const kmb::ElementContainer* orgSurf,kmb::elementIdType initElementId,kmb::ElementContainer* dividedSurf) const;
	size_t getTotalRidge(const kmb::ElementContainer* org,kmb::ElementContainer* ridges) const;
	size_t getSurfaceRidge(const kmb::ElementContainer* orgSurf,kmb::elementIdType initElementId,kmb::ElementContainer* ridges) const;
	// append data to divideFacesInfo
	// return the number of divided groups
	size_t divideFaceGroup(const kmb::DataBindings* data, const kmb::ElementContainer* elements, const kmb::Point3DContainer* points, double angle);
	size_t divideComponentsFaceGroup(const kmb::DataBindings* data, const kmb::ElementContainer* elements);
	// get divided face group included an initFace
	size_t getDividedFaceGroup(kmb::Face initFace,kmb::DataBindings* data) const;
	size_t getSurfaceRidge(const kmb::ElementContainer* elements,kmb::Face initFace,kmb::ElementContainer* ridges) const;
};

}
