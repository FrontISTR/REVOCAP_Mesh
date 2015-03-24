/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : PatchGenerator                                          #
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

#ifdef OPENCASCADE

namespace kmb{

class MeshData;
class ShapeData;

class PatchGenerator
{
private:
	double deflection;
	double incremental;
	double tolerance;
	double modelDiameter;
public:
	PatchGenerator(void);
	virtual ~PatchGenerator(void);
	/*
	 * mesh �ɂ͐������� CAD �ʂ� HashCode �� BodyVariable �� Data �� FaceHashCode �ŗ^����
	 */
	bool execute(kmb::ShapeData& shape,kmb::MeshData& mesh);
	/*
	 * �O�p�`�����鎞�̕ӂ̒����̍ŏ��l��^����
	 * BRepTools::Triangulation
	 *
	 * �ŏ��ɓǂݍ��� shape �̑傫���̑��Βl�Ƃ��邱�Ƃɒ���
	 *
	 */
	void setDeflection(double d);
	double getDeflection(void) const;
	/*
	 * BRepMesh::Mesh
	 */
	void setIncremental(double d);
	double getIncremental(void) const;
	/*
	 * ���_�𓯈ꎋ����臒l
	 */
	void setTolerance(double d);
	double getTolerance(void) const;
};

}

#endif
