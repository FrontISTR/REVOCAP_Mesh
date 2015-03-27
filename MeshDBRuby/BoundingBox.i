/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : BoundingBox                                        #
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
/**--------------------------------------------------------------------
=begin
= RevocapMesh::BoundingBox

==�T�v

�R�����̃o�E���f�B���O�{�b�N�X��^����N���X�B

==���\�b�h�ꗗ

((<BoundingBox.new>))
((<minX>))
((<minY>))
((<minZ>))
((<maxX>))
((<maxY>))
((<maxZ>))
((<centerX>))
((<centerY>))
((<centerZ>))
((<rangeX>))
((<rangeY>))
((<rangeZ>))
((<diameter>))
((<getVolume>))
((<update>))
((<intersect>))
((<intersectVolume>))
((<distanceSq>))
((<expand>))
((<to_a>))

=end
----------------------------------------------------------------------*/
%{
#include "Geometry/kmbRegion.h"
#include "Geometry/kmbBoxRegion.h"
#include "Geometry/kmbBoundingBox.h"
%}

namespace kmb{

class BoundingBox : public BoxRegion
{
public:
/**--------------------------------------------------------------------------
=begin
--- BoundingBox.new
=end
---------------------------------------------------------------------------*/
	BoundingBox(void);
	virtual ~BoundingBox(void);
/**--------------------------------------------------------------------------
=begin
--- minX
--- minY
--- minZ
    x ���W�Ay ���W�Az ���W�̍ŏ��l��^����
=end
---------------------------------------------------------------------------*/
	double minX(void) const;
	double minY(void) const;
	double minZ(void) const;
/**--------------------------------------------------------------------------
=begin
--- maxX
--- maxY
--- maxZ
    x ���W�Ay ���W�Az ���W�̍ő�l��^����
=end
---------------------------------------------------------------------------*/
	double maxX(void) const;
	double maxY(void) const;
	double maxZ(void) const;
/**--------------------------------------------------------------------------
=begin
--- centerX
--- centerY
--- centerZ
    x ���W�Ay ���W�Az ���W�̒��S�l��^����
=end
---------------------------------------------------------------------------*/
	double centerX(void) const;
	double centerY(void) const;
	double centerZ(void) const;
/**--------------------------------------------------------------------------
=begin
--- rangeX
--- rangeY
--- rangeZ
    x ���W�Ay ���W�Az ���W�̕���^����
=end
---------------------------------------------------------------------------*/
	double rangeX(void) const;
	double rangeY(void) const;
	double rangeZ(void) const;
/**--------------------------------------------------------------------------
=begin
--- diameter
    �Ίp���̒�����^����
=end
---------------------------------------------------------------------------*/
	double diameter(void) const;
/**--------------------------------------------------------------------------
=begin
--- getVolume
    �̐ς�^����
=end
---------------------------------------------------------------------------*/
	double getVolume(void) const;
/**--------------------------------------------------------------------------
=begin
--- update(x,y,z)
    �X�V����
=end
---------------------------------------------------------------------------*/
	void update(const double x, const double y,const double z);
	void update(const BoundingBox& box);
/**--------------------------------------------------------------------------
=begin
--- intersect(box)
    ��������
=end
---------------------------------------------------------------------------*/
	bool intersect(const kmb::BoundingBox& box) const;
/**--------------------------------------------------------------------------
=begin
--- intersectVolume(box)
    �����̐�
=end
---------------------------------------------------------------------------*/
	double intersectVolume(const kmb::BoundingBox& box) const;
/**--------------------------------------------------------------------------
=begin
--- distanceSq(box)
    ������2��
    �ő�_���m�̋�����2��ƍŏ��_���m�̋�����2��̘a
    �������Ă��Ă�0�Ƃ͌���Ȃ��̂Œ��ӂ���
=end
---------------------------------------------------------------------------*/
	double distanceSq(const kmb::BoundingBox& box) const;
/**--------------------------------------------------------------------------
=begin
--- expand(r)
    �g�傷��
=end
---------------------------------------------------------------------------*/
	void expand(double ratio);
/**--------------------------------------------------------------------------
=begin
--- to_a
    �z��ɕϊ�����
    �`���� [[minX, minY, minZ], [maxX, maxY, maxZ]]
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE to_a(void) const{
		VALUE minmax = rb_ary_new();
		VALUE min = rb_ary_new();
		VALUE max = rb_ary_new();
		rb_ary_store(min,0,rb_float_new(self->minX()));
		rb_ary_store(min,1,rb_float_new(self->minY()));
		rb_ary_store(min,2,rb_float_new(self->minZ()));
		rb_ary_store(max,0,rb_float_new(self->maxX()));
		rb_ary_store(max,1,rb_float_new(self->maxY()));
		rb_ary_store(max,2,rb_float_new(self->maxZ()));
		rb_ary_store(minmax,0,min);
		rb_ary_store(minmax,1,max);
		return minmax;
	}
}
};

/**--------------------------------------------------------------------
=begin
= RevocapMesh::BoundingBox2D

==�T�v

�Q�����ł̃o�E���f�B���O�{�b�N�X��^����N���X�B

==���\�b�h�ꗗ

((<BoundingBox2D.new>))
((<minX>))
((<minY>))
((<maxX>))
((<maxY>))
((<centerX>))
((<centerY>))
((<rangeX>))
((<rangeY>))
((<diameter>))
((<getArea>))
((<intersect>))
((<intersectArea>))
((<update>))
((<distanceSq>))
((<expand>))
((<to_a>))

=end
----------------------------------------------------------------------*/
class BoundingBox2D
{
public:
	BoundingBox2D(void);
	virtual ~BoundingBox2D(void);
	double minX(void) const;
	double minY(void) const;
	double maxX(void) const;
	double maxY(void) const;
	double centerX(void) const;
	double centerY(void) const;
	double rangeX(void) const;
	double rangeY(void) const;
	double diameter(void) const;
	double getArea(void) const;
	bool intersect(const kmb::BoundingBox2D& box) const;
	double intersectArea(const kmb::BoundingBox2D& box) const;
	void update(const double x, const double y);
	double distanceSq(const kmb::BoundingBox2D& box) const;
	void expand(double r);
/**--------------------------------------------------------------------------
=begin
--- to_a
    �z��ɕϊ�����
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE to_a(void) const{
		VALUE minmax = rb_ary_new();
		VALUE min = rb_ary_new();
		VALUE max = rb_ary_new();
		rb_ary_store(min,0,rb_float_new(self->minX()));
		rb_ary_store(min,1,rb_float_new(self->minY()));
		rb_ary_store(max,0,rb_float_new(self->maxX()));
		rb_ary_store(max,1,rb_float_new(self->maxY()));
		rb_ary_store(minmax,0,min);
		rb_ary_store(minmax,1,max);
		return minmax;
	}
}
};

}
