/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : BSplineSurface3D                                   #
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
=RevocapShape::BSplineSurface3D

==���\�b�h�ꗗ

==���\�b�h

=end
------------------------------------------------------------------------------*/

%{
#include "Shape/kmbBSplineSurface3D.h"
%}

namespace kmb{

class BSplineSurface3D
{
public:
/**--------------------------------------------------------------------------
=begin
--- BSplineSurface3D.new
=end
------------------------------------------------------------------------------*/
	BSplineSurface3D(void);
	virtual ~BSplineSurface3D(void);
/**--------------------------------------------------------------------------
=begin
--- clear
    �f�[�^���N���A����
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
--- setOrder(uOrder,vOrder)
    order ��ݒ肷��
=end
------------------------------------------------------------------------------*/
	bool setOrder(unsigned int uOrder,unsigned int vOrder);
/**--------------------------------------------------------------------------
=begin
--- getOrder
    order �� [uOrder,vOrder] �̌`���Ŏ擾����
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getOrder(void) const{
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
--- getKnotCount
    knot �̌��� [u�����̌�,v�����̌�] �̌`���Ŏ擾����
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getKnotCount(void) const{
		unsigned int uCount = 0;
		unsigned int vCount = 0;
		self->getKnotCount( uCount, vCount );
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,INT2NUM(uCount));
		rb_ary_store(ary,1,INT2NUM(vCount));
		return ary;
	}
}

/**--------------------------------------------------------------------------
=begin
--- appendUKnot(k)
    u ������ knot ��ǉ�����
--- appendVKnot(k)
    v ������ knot ��ǉ�����
=end
------------------------------------------------------------------------------*/
	void appendUKnot(double k);
	void appendVKnot(double k);

/**--------------------------------------------------------------------------
=begin
--- eachUKnot
    u ������ knot �̃C�e���[�^
--- eachVKnot
    v ������ knot �̃C�e���[�^
--- getUKnotArray
--- getVKnotArray
=end
------------------------------------------------------------------------------*/
%extend{
	void eachUKnot(void){
		unsigned int uNum, vNum;
		self->getKnotCount(uNum,vNum);
		for(unsigned int i=0;i<uNum;++i){
			rb_yield( rb_float_new( self->getUKnot(i) ) );
		}
	}
	void eachVKnot(void){
		unsigned int uNum, vNum;
		self->getKnotCount(uNum,vNum);
		for(unsigned int i=0;i<vNum;++i){
			rb_yield( rb_float_new( self->getVKnot(i) ) );
		}
	}
	VALUE getUKnotArray(void){
		VALUE ary = rb_ary_new();
		unsigned int uNum, vNum;
		self->getKnotCount(uNum,vNum);
		for(unsigned int i=0;i<uNum;++i){
			rb_ary_store(ary,i,rb_float_new( self->getUKnot(i) ) );
		}
		return ary;
	}
	VALUE getVKnotArray(void){
		VALUE ary = rb_ary_new();
		unsigned int uNum, vNum;
		self->getKnotCount(uNum,vNum);
		for(unsigned int i=0;i<vNum;++i){
			rb_ary_store(ary,i,rb_float_new( self->getVKnot(i) ) );
		}
		return ary;
	}
}

/**--------------------------------------------------------------------------
=begin
--- getCtrlPtCount
    ����_�̌���Ԃ��B
    (u ������ knot �̌� - u ������ order) �~ (v ������ knot �̌� - v ������ order) �ƈ�v����B
--- appendCtrlPt(x,y,z)
    ����_��ǉ�����
--- eachCtrlPt
    ����_�̃C�e���[�^
=end
------------------------------------------------------------------------------*/
	int getCtrlPtCount(void) const;
	void appendCtrlPt(double x,double y,double z);

%extend{
	void eachCtrlPt(void){
		int num = self->getCtrlPtCount();
		kmb::Point3D pt;
		for(int i=0;i<num;++i){
			self->getCtrlPt(i,pt);
			VALUE ary = rb_ary_new();
			rb_ary_store(ary,0,rb_float_new( pt.x() ));
			rb_ary_store(ary,1,rb_float_new( pt.y() ));
			rb_ary_store(ary,2,rb_float_new( pt.z() ));
			rb_yield( ary );
		}
	}
}

/**--------------------------------------------------------------------------
=begin
--- valid
    �p�����[�^�̑Ó����𔻒f����B
    order �� knot �Ɛ���_�̌��̊֌W�������Ă���̂��̃`�F�b�N������B
=end
------------------------------------------------------------------------------*/
	bool valid(void) const;

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
    �Ȗʏ�̓_�̍��W��Ԃ�
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getPoint( double u, double v ) const{
		kmb::Point3D pt;
		if( self->getPoint( u, v, pt ) ){
			VALUE ary = rb_ary_new();
			rb_ary_store(ary,0,rb_float_new(pt.x()));
			rb_ary_store(ary,1,rb_float_new(pt.y()));
			rb_ary_store(ary,2,rb_float_new(pt.z()));
			return ary;
		}else{
			return Qnil;
		}
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
	VALUE getNearest( double x, double y,double z,double u=0.5,double v=0.5) const{
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
