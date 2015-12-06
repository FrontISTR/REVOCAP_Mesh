/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : ElementEvaluator                                        #
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
// �v�f�̕i���]���̂��߂̊֐��Q
// http://www.jama.or.jp/cgi-bin/pdq/download_pdq.cgi �Ȃǂ��Q�l�ɂ���
//
// �Ǐ��I�ɕ]���ł�����݂̂̂�Ώۂɂ���
// ���I�ɕ]�����ׂ����̂ɂ��Ă͈���Ȃ�
//

#pragma once

#include "MeshDB/kmbElement.h"
#include "MeshDB/kmbFace.h"
#include "MeshDB/kmbElementContainer.h"
#include "Geometry/kmbGeometry3D.h"

namespace kmb{

class Point3DContainer;
class Point2DContainer;
class BoundingBox;
class SquareMatrix;

class ElementEvaluator
{
public:
	ElementEvaluator(const kmb::Point3DContainer* points);
	ElementEvaluator(const kmb::Point2DContainer* point2Ds);
	virtual ~ElementEvaluator(void);
	// �O�ڋ��̔��a�Ɠ��ڋ��̔��a�̔�
	double getAspectRatio(const kmb::ElementBase &eIter) const;
	double getAspectRatio(kmb::nodeIdType n0,kmb::nodeIdType n1,kmb::nodeIdType n2,kmb::nodeIdType n3) const;
	// ���ڋ��̔��a
	double getInscribedRadius(kmb::nodeIdType n0,kmb::nodeIdType n1,kmb::nodeIdType n2,kmb::nodeIdType n3) const;
	// �O�ڋ��̔��a
	double getCircumRadius(kmb::nodeIdType n0,kmb::nodeIdType n1,kmb::nodeIdType n2,kmb::nodeIdType n3) const;
	// �Œ��ӂ̒����ƍŒZ�ӂ̒����̔�
	double getEdgeLengthRatio(const kmb::ElementBase &eIter) const;
	double getMaxEdgeLength(const kmb::ElementBase &eIter) const;
	double getAverageEdgeLength(const kmb::ElementBase &eIter) const;
	// �O�p�`�̍ŏ��p�x�i�ʂ̂����O�p�`�����𒲂ׂ�j
	double getMinAngleTri(const kmb::ElementBase &eIter) const;
	// �ŏ��p�x
	double getMinAngle(const kmb::ElementBase &eIter) const;
	// �ő�p�x
	double getMaxAngle(const kmb::ElementBase &eIter) const;
	// �̐�
	double getVolume(const kmb::ElementBase &eIter) const;
	double getVolume(kmb::nodeIdType n0,kmb::nodeIdType n1,kmb::nodeIdType n2,kmb::nodeIdType n3) const;
	// �ʐ�
	double getArea(const kmb::ElementBase &eIter) const;
	double getArea(const kmb::ElementBase &eIter,kmb::idType localFaceId) const;
	// ����
	double getLength(const kmb::ElementBase &eIter) const;
	// �p�x
	double getAngle(const kmb::ElementBase &eIter,kmb::nodeIdType nodeId) const;
	double getAngleByIndex(const kmb::ElementBase &eIter,int index) const;
	double getCosByIndex(const kmb::ElementBase &eIter,int index) const;
	// n0 �̐��
	// n0->n3, n1->n3, n2->n3 �𐳋K�����Ă���O�d�ς����
	double getSharpness(kmb::nodeIdType n0,kmb::nodeIdType n1,kmb::nodeIdType n2,kmb::nodeIdType n3) const;
	// Bounding Box
	bool getBoundingBox(const kmb::ElementBase &element,kmb::BoundingBox &bbox) const;
	double getBoundingBoxRadius(const kmb::ElementBase &element) const;
	// double getAngleBetweenFaces(const kmb::ElementBase &eIter,int index,int index) const;
	// �@���x�N�g��
	bool getNormalVector(const kmb::ElementBase &element, kmb::Vector3D &vect) const;
	bool getNormalVector(const Face &face, const kmb::ElementContainer* elements, kmb::Vector3D &vect) const;
	bool getNormalVectorOfFace(const kmb::ElementBase &element, int index, kmb::Vector3D &vect) const;
	// �v�f�����R���W�n�̎擾
	bool getNaturalCoordinates(const kmb::ElementBase &element,const double x,const double y,const double z,double* retvals) const;
	bool getNaturalCoordinatesOfFace(const kmb::ElementBase &element,int index,const double x,const double y,const double z,double* retvals) const;
	double checkShapeFunctionDomain(const kmb::ElementBase &element,double s,double t,double u);
	// �v�f�̌`��֐��ŕ]�������Ƃ��� (x,y,z) �̌W��
	// (x,y,z) = \sum_i weights[i] * element[i].(x,y,z)
	// �߂�l�� checkShapeFunctionDomain �Ɠ���
	double getWeightElement(const kmb::ElementBase &element,const double x,const double y,const double z,double* weights) const;
	double getWeightElementFace(const kmb::ElementBase &element,int index,const double x,const double y,const double z,double* weights) const;
	// �v�f�����R���W�n���畨�����W�̎擾
	bool getPhysicalCoordinates(const kmb::ElementBase &element,const double s,const double t,const double u,kmb::Point3D &target) const;
	// �v�f�̖ʂ�1�_ (x,y,z) ����Ȃ�S�ʑ̂̑̐ς̍ŏ��l
	// ���̒l�����Ȃ�΁A�_ (x,y,z) �͗v�f�̓����ɂ���Ƃ��Ă悢
	double getMinInnerVolume(const kmb::ElementBase &element,const double x,const double y,const double z) const;
	// �v�f��1�_ (x,y,z) �̋���
	// 2�����v�f�̎��̂�
	double getDistanceSq(const kmb::ElementBase &element,const double x,const double y,const double z) const;
	// �v�f�̖ʂ�1�_ (x,y,z) �̋����̍ŏ��l
	double getDistanceSqOnBoundary(const kmb::ElementBase &element,const double x,const double y,const double z) const;
	// �l�p�`�̑Ίp���̒����̔�
	// �l�p�`�����ꕽ�ʂɂ��邩�ǂ���
	// ���ʗv�f�̂Ȃ��p�x
	double getAngleBetweenElements(const kmb::ElementBase &element0,const kmb::ElementBase &element1) const;
	// �ʂ̂Ȃ��p�� cos = - �@���x�N�g���̓���
	// ���̒l�����̎����ʂƖʂ��s�p�ɂȂ��Ă��邱�Ƃ��Ӗ�����
	double getCosBetweenElements(const kmb::ElementBase &element0,const kmb::ElementBase &element1) const;
	// �l�p�`�ɉ��_�����݂��邩�ǂ���
	// ���݂����炻�̓_�� index ��Ԃ��A���݂��Ȃ���� -1 ��Ԃ�
	int getConcaveInQuad(const kmb::ElementBase &quad) const;
	// ���R�r�A���̗v�f���̍ő�ŏ�
	// QUAD or HEXAHEDRON �̏ꍇ�� distorsion element ��T������
	bool getMinMaxJacobian(const kmb::ElementBase &element, double &min, double &max) const;
	// �d���ߓ_�ԍ��̌�
	// �S�Ă̑g�ݍ��킹�̔�r�̂��������ɂȂ������̂̌�
	int getDuplicationNodeIdCount(const kmb::ElementBase &element) const;

	// �v�f�����}�g���N�X
	// �ߓ_�̌��傫���̃}�g���N�X�ɑ���i�C���f�b�N�X�͐ߓ_�ԍ��ł͂Ȃ����Ƃɒ��Ӂj
	bool getStiffMatrix(const kmb::ElementBase &element,kmb::SquareMatrix &stiff) const;
protected:
	const kmb::Point3DContainer* points;
	const kmb::Point2DContainer* point2Ds;
private:
	double getMaxAngleCos(const kmb::ElementBase &eIter) const;
	double getMinAngleCos(const kmb::ElementBase &eIter) const;
};

}
