/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : BezierSurface3D                                    #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
----------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
=begin
=RevocapShape::BezierSurface3D

==�T�v

�R������ԓ��̃x�W�G�Ȗʂ�����

==���\�b�h�ꗗ

==���\�b�h

=end
------------------------------------------------------------------------------*/

%{
#include "Shape/kmbBezierSurface3D.h"
%}

namespace kmb{

class BezierSurface3D
{
public:
/**--------------------------------------------------------------------------
=begin
--- BezierSurface3D.new
=end
------------------------------------------------------------------------------*/
	BezierSurface3D(void);
	virtual ~BezierSurface3D(void);

/**--------------------------------------------------------------------------
=begin
--- clear
    �f�[�^�̃N���A
=end
------------------------------------------------------------------------------*/
	void clear(void);

/**--------------------------------------------------------------------------
=begin
--- setSurfaceId(id)
    �\�ʃf�[�^�̎��ʎq��^����
--- getSurfaceId
    �\�ʃf�[�^�̎��ʎq���擾����
=end
------------------------------------------------------------------------------*/
	long getSurfaceId(void) const;
	void setSurfaceId(long id);

/**--------------------------------------------------------------------------
=begin
--- getOrder
    order ���擾����B [uOrder,vOrder] �̌`���B
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getOrder(void){
		unsigned int uOrder = 0;
		unsigned int vOrder = 0;
		self->getOrder( uOrder, vOrder );
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,INT2NUM(uOrder));
		rb_ary_store(ary,1,INT2NUM(vOrder));
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
--- setOrder(uOrder,vOrder)
    order��ݒ肷��
=end
------------------------------------------------------------------------------*/
	bool setOrder(unsigned int uOrder,unsigned int vOrder);

/**--------------------------------------------------------------------------
=begin
--- appendCtrlPt(x,y,z)
    ����_��ǉ�����
=end
------------------------------------------------------------------------------*/
	void appendCtrlPt(double x,double y,double z);

/**--------------------------------------------------------------------------
=begin
--- getBoundingBox
    BoundingBox ��Ԃ��B
=end
------------------------------------------------------------------------------*/
%extend{
	const kmb::BoundingBox getBoundingBox(void) const{
		kmb::BoundingBox bbox;
		self->getBoundingBox(bbox);
		return bbox;
	}
}

/**--------------------------------------------------------------------------
=begin
--- getPoint(u,v)
    �Ȗʏ�̓_�̍��W�����߂�B�߂�l�� [x,y,z] �̌`���B
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getPoint( double u, double v ){
		kmb::Point3D pt;
		if( u < 0 || 1 < v || v < 0 || 1 < v ){
			return Qnil;
		}
		self->getPoint( u, v, pt );
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,rb_float_new(pt.x()));
		rb_ary_store(ary,1,rb_float_new(pt.y()));
		rb_ary_store(ary,2,rb_float_new(pt.z()));
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
--- getNearest(x,y,z,u=0.5,v=0.5)
	�����@�ŋ��߂�B�������Ȃ������Ƃ��� nil ��Ԃ�
	u,v �͔����@(Newton-Raphson) �̏����l�B
	�����l����`��ɂȂ�������K���ɏ����l��T��
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getNearest( double x, double y,double z,double u=0.5,double v=0.5){
		double u0 = u;
		double v0 = v;
		kmb::Point3D pt(x,y,z);
		if( self->getNearest( pt, u0, v0 ) ){
			VALUE ary = rb_ary_new();
			rb_ary_store(ary,0,rb_float_new( u0 ));
			rb_ary_store(ary,1,rb_float_new( v0 ));
			return ary;
		}else{
			return Qnil;
		}
	}
}
};

}
