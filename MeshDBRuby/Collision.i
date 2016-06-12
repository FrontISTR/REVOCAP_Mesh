/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : Collision                                          #
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
= RevocapMesh::Collision

==概要

要素間の干渉を判定する

=end
----------------------------------------------------------------------*/
%{
#include "MeshDB/kmbCollision.h"
%}

namespace kmb{

class MeshData;

class Collision
{
public:
/**--------------------------------------------------------------------------
=begin
--- Collision.new
=end
---------------------------------------------------------------------------*/
	Collision(kmb::MeshData* mesh);
	virtual ~Collision(void);
/**--------------------------------------------------------------------------
=begin
--- detectFaceGroup
--- detectSurfaceFaceGroup
--- detectSurfaceFaceGroupByNode
=end
---------------------------------------------------------------------------*/
	int detectFaceGroup(const char* fg0,const char* fg1,double thres,const char* result);
	int detectSurfaceFaceGroup(kmb::bodyIdType bodyId0,const char* fg1,double thres,const char* result);
	int detectSurfaceFaceGroupByNode(kmb::bodyIdType bodyId0,const char* fg1,int tCount,double thres,const char* result);
%extend{
/**--------------------------------------------------------------------------
=begin
--- distanceSegSeg
=end
---------------------------------------------------------------------------*/
	VALUE distanceSegSeg(VALUE seg0,VALUE seg1) const
	{
		Check_Type(seg0, T_ARRAY);
		Check_Type(seg1, T_ARRAY);
		const int len0 = RARRAY_LEN(seg0);
		const int len1 = RARRAY_LEN(seg1);
		if( len0 < 2 || len1 < 2 ){
			return Qnil;
		}
		double s=0.0,t=0.0;
		double d = self->distanceSqSegSeg(
			FIX2INT(rb_ary_entry(seg0,0)), FIX2INT(rb_ary_entry(seg0,1)),
			FIX2INT(rb_ary_entry(seg1,0)), FIX2INT(rb_ary_entry(seg1,1)),
			s,t);
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,rb_float_new( sqrt(d) ));
		rb_ary_store(ary,1,rb_float_new(s));
		rb_ary_store(ary,2,rb_float_new(t));
		return ary;
	}
/**--------------------------------------------------------------------------
=begin
--- distanceSegTri
=end
---------------------------------------------------------------------------*/
	double distanceSegTri(VALUE seg,VALUE tri) const
	{
		Check_Type(seg, T_ARRAY);
		Check_Type(tri, T_ARRAY);
		const int len0 = RARRAY_LEN(seg);
		const int len1 = RARRAY_LEN(tri);
		if( len0 < 2 || len1 < 3 ){
			return DBL_MAX;
		}
		kmb::Triangle t( FIX2INT(rb_ary_entry(tri,0)), FIX2INT(rb_ary_entry(tri,1)), FIX2INT(rb_ary_entry(tri,2)) );
		return self->distanceSegTri(
			FIX2INT(rb_ary_entry(seg,0)), FIX2INT(rb_ary_entry(seg,1)), t);
	}
/**--------------------------------------------------------------------------
=begin
--- distanceTriTri
=end
---------------------------------------------------------------------------*/
	double distanceTriTri(VALUE tri0,VALUE tri1) const
	{
		Check_Type(tri0, T_ARRAY);
		Check_Type(tri1, T_ARRAY);
		const int len0 = RARRAY_LEN(tri0);
		const int len1 = RARRAY_LEN(tri1);
		if( len0 < 3 || len1 < 3 ){
			return DBL_MAX;
		}
		kmb::Triangle t0( FIX2INT(rb_ary_entry(tri0,0)), FIX2INT(rb_ary_entry(tri0,1)), FIX2INT(rb_ary_entry(tri0,2)) );
		kmb::Triangle t1( FIX2INT(rb_ary_entry(tri1,0)), FIX2INT(rb_ary_entry(tri1,1)), FIX2INT(rb_ary_entry(tri1,2)) );
		return self->distanceTriTri( t0, t1 );
	}
/**--------------------------------------------------------------------------
=begin
--- detectTriTri
=end
---------------------------------------------------------------------------*/
	const char* detectTriTri(VALUE tri0,VALUE tri1) const
	{
		Check_Type(tri0, T_ARRAY);
		Check_Type(tri1, T_ARRAY);
		const int len0 = RARRAY_LEN(tri0);
		const int len1 = RARRAY_LEN(tri1);
		if( len0 < 3 || len1 < 3 ){
			return "UNKNOWN";
		}
		kmb::Triangle t0( FIX2INT(rb_ary_entry(tri0,0)), FIX2INT(rb_ary_entry(tri0,1)), FIX2INT(rb_ary_entry(tri0,2)) );
		kmb::Triangle t1( FIX2INT(rb_ary_entry(tri1,0)), FIX2INT(rb_ary_entry(tri1,1)), FIX2INT(rb_ary_entry(tri1,2)) );
		return kmb::Collision::collisionTypeString( self->detectTriTri( t0, t1 ) );
	}
/**--------------------------------------------------------------------------
=begin
--- distanceQuadQuad
=end
---------------------------------------------------------------------------*/
	double distanceQuadQuad(VALUE quad0,VALUE quad1) const
	{
		Check_Type(quad0, T_ARRAY);
		Check_Type(quad1, T_ARRAY);
		const int len0 = RARRAY_LEN(quad0);
		const int len1 = RARRAY_LEN(quad1);
		if( len0 < 4 || len1 < 4 ){
			return DBL_MAX;
		}
		kmb::Quad q0( FIX2INT(rb_ary_entry(quad0,0)), FIX2INT(rb_ary_entry(quad0,1)), FIX2INT(rb_ary_entry(quad0,2)), FIX2INT(rb_ary_entry(quad0,3)) );
		kmb::Quad q1( FIX2INT(rb_ary_entry(quad1,0)), FIX2INT(rb_ary_entry(quad1,1)), FIX2INT(rb_ary_entry(quad1,2)), FIX2INT(rb_ary_entry(quad1,3)) );
		return self->distanceQuadQuad( q0, q1 );
	}
/**--------------------------------------------------------------------------
=begin
--- getNearestFace
=end
---------------------------------------------------------------------------*/
	VALUE getNearestFace(VALUE quad0,const char* fg) const
	{
		Check_Type(quad0, T_ARRAY);
		const int len0 = RARRAY_LEN(quad0);
		if( len0 < 4 ){
			return Qnil;
		}
		int index = 0;
		kmb::Face f;
		double d = 0.0;
		if( len0 == 3 ){
			kmb::Triangle t0( FIX2INT(rb_ary_entry(quad0,0)), FIX2INT(rb_ary_entry(quad0,1)), FIX2INT(rb_ary_entry(quad0,2)) );
			d = self->getNearestFace(t0,fg,f,index);
		}else if( len0 == 4 ){
			kmb::Quad q0( FIX2INT(rb_ary_entry(quad0,0)), FIX2INT(rb_ary_entry(quad0,1)), FIX2INT(rb_ary_entry(quad0,2)), FIX2INT(rb_ary_entry(quad0,3)) );
			d = self->getNearestFace(q0,fg,f,index);
		}
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,rb_float_new(d));
		rb_ary_store(ary,1,toVALUE(f));
		rb_ary_store(ary,2,INT2FIX(index));
		return ary;
	}
}
};

}
