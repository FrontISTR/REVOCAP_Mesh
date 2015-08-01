/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : PhysicalValue                                           #
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
/**
 * 物理量の処理に関する Ruby 拡張ライブラリのみで使われる関数群
 * meshdb.i に書いてしまうと SWIG が名前を変えてしまう
 * クラスのメソッドとして定義すると、Ruby 拡張ライブラリ版と通常版の sizeof の値が変わってしまうため
 */
#include <math.h>
#include <float.h>
#include "MeshDB/kmbPhysicalValue.h"

VALUE toVALUE(double x,double y)
{
	VALUE ary = rb_ary_new();
	rb_ary_store(ary,0,rb_float_new(x));
	rb_ary_store(ary,1,rb_float_new(y));
	return ary;
}

VALUE toVALUE(double x,double y,double z)
{
	VALUE ary = rb_ary_new();
	rb_ary_store(ary,0,rb_float_new(x));
	rb_ary_store(ary,1,rb_float_new(y));
	rb_ary_store(ary,2,rb_float_new(z));
	return ary;
}

VALUE toVALUE(double x,double y,double z,double w)
{
	VALUE ary = rb_ary_new();
	rb_ary_store(ary,0,rb_float_new(x));
	rb_ary_store(ary,1,rb_float_new(y));
	rb_ary_store(ary,2,rb_float_new(z));
	rb_ary_store(ary,3,rb_float_new(w));
	return ary;
}

VALUE toVALUE(double xx,double yy,double zz,double xy,double yz,double zx)
{
	VALUE ary = rb_ary_new();
	rb_ary_store(ary,0,rb_float_new(xx));
	rb_ary_store(ary,1,rb_float_new(yy));
	rb_ary_store(ary,2,rb_float_new(zz));
	rb_ary_store(ary,3,rb_float_new(xy));
	rb_ary_store(ary,4,rb_float_new(yz));
	rb_ary_store(ary,5,rb_float_new(zx));
	return ary;
}

VALUE toVALUE(double x,double y,double z,long ival)
{
	VALUE ary = rb_ary_new();
	rb_ary_store(ary,0,rb_float_new(x));
	rb_ary_store(ary,1,rb_float_new(y));
	rb_ary_store(ary,2,rb_float_new(z));
	rb_ary_store(ary,3,INT2FIX(ival));
	return ary;
}

VALUE toVALUE(double x,double y,double z,double w,long ival)
{
	VALUE ary = rb_ary_new();
	rb_ary_store(ary,0,rb_float_new(x));
	rb_ary_store(ary,1,rb_float_new(y));
	rb_ary_store(ary,2,rb_float_new(z));
	rb_ary_store(ary,3,rb_float_new(w));
	rb_ary_store(ary,4,INT2FIX(ival));
	return ary;
}

VALUE toVALUE(double x,double y,long ival)
{
	VALUE ary = rb_ary_new();
	rb_ary_store(ary,0,rb_float_new(x));
	rb_ary_store(ary,1,rb_float_new(y));
	rb_ary_store(ary,2,INT2FIX(ival));
	return ary;
}

VALUE toVALUE(int x,int y)
{
	VALUE ary = rb_ary_new();
	rb_ary_store(ary,0,INT2FIX(x));
	rb_ary_store(ary,1,INT2FIX(y));
	return ary;
}

VALUE toVALUE(kmb::Face f)
{
	VALUE ary = rb_ary_new();
	rb_ary_store(ary,0,INT2FIX(f.getElementId()));
	rb_ary_store(ary,1,INT2FIX(f.getLocalFaceId()));
	return ary;
}

VALUE appendKeys(VALUE block,VALUE ary)
{
	Check_Type(block,T_ARRAY);
	Check_Type(ary,T_ARRAY);
	VALUE key = rb_ary_entry(block,0);
	rb_ary_push(ary, key);
	return Qnil;
}

VALUE toVALUE(kmb::PhysicalValue* value)
{
	if( value ){
		switch( value->getType() )
		{
		case kmb::PhysicalValue::Boolean:
			return ( reinterpret_cast<kmb::BooleanValue*>(value)->getValue() ) ? Qtrue : Qfalse;
		case kmb::PhysicalValue::Integer:
			return INT2FIX( reinterpret_cast<kmb::IntegerValue*>(value)->getValue() );
		case kmb::PhysicalValue::Scalar:
			return rb_float_new( reinterpret_cast<kmb::ScalarValue*>(value)->getValue() );
		case kmb::PhysicalValue::Vector2:
			{
				VALUE ary = rb_ary_new();
				for(int i=0;i<2;++i){
					rb_ary_store(ary,i,rb_float_new( reinterpret_cast<kmb::Vector2Value*>(value)->getValue(i) ));
				}
				return ary;
			}
		case kmb::PhysicalValue::Vector3:
			{
				VALUE ary = rb_ary_new();
				for(int i=0;i<3;++i){
					rb_ary_store(ary,i,rb_float_new( reinterpret_cast<kmb::Vector3Value*>(value)->getValue(i) ));
				}
				return ary;
			}
		case kmb::PhysicalValue::Vector4:
			{
				VALUE ary = rb_ary_new();
				for(int i=0;i<4;++i){
					rb_ary_store(ary,i,rb_float_new( reinterpret_cast<kmb::Vector4Value*>(value)->getValue(i) ));
				}
				return ary;
			}
		case kmb::PhysicalValue::Tensor6:
			{
				VALUE ary = rb_ary_new();
				for(int i=0;i<6;++i){
					rb_ary_store(ary,i,rb_float_new( reinterpret_cast<kmb::Tensor6Value*>(value)->getValue(i) ));
				}
				return ary;
			}
		case kmb::PhysicalValue::Point3Vector3:
			{
				VALUE ary = rb_ary_new();
				VALUE p = rb_ary_new();
				VALUE v = rb_ary_new();
				for(int i=0;i<3;++i){
					rb_ary_store(p,i,rb_float_new( reinterpret_cast<kmb::Point3Vector3Value*>(value)->getPoint(i) ));
					rb_ary_store(v,i,rb_float_new( reinterpret_cast<kmb::Point3Vector3Value*>(value)->getValue(i) ));
				}
				rb_ary_store(ary,0,p);
				rb_ary_store(ary,1,v);
				return ary;
			}
		case kmb::PhysicalValue::Vector2withInt:
			{
				VALUE ary = rb_ary_new();
				for(int i=0;i<2;++i){
					rb_ary_store(ary,i,rb_float_new( reinterpret_cast<kmb::Vector2WithInt*>(value)->getValue(i) ));
				}
				rb_ary_store(ary,2,INT2FIX( reinterpret_cast<kmb::Vector2WithInt*>(value)->getIValue() ) );
				return ary;
			}
		case kmb::PhysicalValue::Vector3withInt:
			{
				VALUE ary = rb_ary_new();
				for(int i=0;i<3;++i){
					rb_ary_store(ary,i,rb_float_new( reinterpret_cast<kmb::Vector3WithInt*>(value)->getValue(i) ));
				}
				rb_ary_store(ary,3,INT2FIX( reinterpret_cast<kmb::Vector3WithInt*>(value)->getIValue() ) );
				return ary;
			}
		case kmb::PhysicalValue::Vector4withInt:
			{
				VALUE ary = rb_ary_new();
				for(int i=0;i<4;++i){
					rb_ary_store(ary,i,rb_float_new( reinterpret_cast<kmb::Vector4WithInt*>(value)->getValue(i) ));
				}
				rb_ary_store(ary,4,INT2FIX( reinterpret_cast<kmb::Vector4WithInt*>(value)->getIValue() ) );
				return ary;
			}
		case kmb::PhysicalValue::String:
			return rb_str_new2( reinterpret_cast< kmb::TextValue*>(value)->getValue().c_str() );
		case kmb::PhysicalValue::Array:
			{
				VALUE ary = rb_ary_new();
				const size_t len = reinterpret_cast< kmb::ArrayValue*>(value)->getSize();
				for(size_t i=0;i<len;++i){
					kmb::PhysicalValue* val = reinterpret_cast< kmb::ArrayValue*>(value)->getValue(static_cast<int>(i));
					rb_ary_store( ary, static_cast<long>(i), toVALUE(val) );
				}
				return ary;
			}
		case kmb::PhysicalValue::Hash:
			{
				VALUE hash = rb_hash_new();
				std::map< std::string, kmb::PhysicalValue* >::const_iterator vIter
					= reinterpret_cast< kmb::HashValue* >(value)->begin();
				std::map< std::string, kmb::PhysicalValue* >::const_iterator endIter
					= reinterpret_cast< kmb::HashValue* >(value)->end();
				while( vIter != endIter )
				{
					std::string str = vIter->first;
					kmb::PhysicalValue* val = vIter->second;
					if( val != NULL )
					{
						rb_hash_aset( hash, rb_str_new2( str.c_str() ), toVALUE(val) );
					}
					++vIter;
				}
				return hash;
			}
		case kmb::PhysicalValue::IntHash:
			{
				VALUE hash = rb_hash_new();
				std::map< int, kmb::PhysicalValue* >::const_iterator vIter
					= reinterpret_cast< kmb::IntHashValue* >(value)->begin();
				std::map< int, kmb::PhysicalValue* >::const_iterator endIter
					= reinterpret_cast< kmb::IntHashValue* >(value)->end();
				while( vIter != endIter )
				{
					int n = vIter->first;
					kmb::PhysicalValue* val = vIter->second;
					if( val != NULL )
					{
						rb_hash_aset( hash, INT2FIX(n), toVALUE(val) );
					}
					++vIter;
				}
				return hash;
			}
		default:
			break;
		}
	}
	return Qnil;
}

kmb::PhysicalValue* createFromVALUE(VALUE v)
{
	switch( TYPE(v) ){
		case T_TRUE:
			return new kmb::BooleanValue(true);
		case T_FALSE:
			return new kmb::BooleanValue(false);
		case T_FIXNUM:
			return new kmb::IntegerValue(FIX2INT(v));
		case T_FLOAT:
			return new kmb::ScalarValue(NUM2DBL(v));
		case T_STRING:
			return new kmb::TextValue(StringValuePtr(v));
		case T_ARRAY:
		{
			const int len = RARRAY_LEN(v);
			if( len == 0 ){
				return new kmb::ArrayValue();
			}else{
				VALUE first = rb_ary_entry(v,0);
				if( len == 3 && TYPE(first) == T_FLOAT && TYPE(rb_ary_entry(v,2)) == T_FLOAT){
					double d[3];
					d[0] = NUM2DBL(rb_ary_entry(v,0));
					d[1] = NUM2DBL(rb_ary_entry(v,1));
					d[2] = NUM2DBL(rb_ary_entry(v,2));
					return new kmb::Vector3Value(d);
				}else if( len == 2 && TYPE(first) == T_FLOAT && TYPE(rb_ary_entry(v,1)) == T_FLOAT ){
					double d[2];
					d[0] = NUM2DBL(rb_ary_entry(v,0));
					d[1] = NUM2DBL(rb_ary_entry(v,1));
					return new kmb::Vector2Value(d);
				}else if( len == 4 && TYPE(first) == T_FLOAT && TYPE(rb_ary_entry(v,1)) == T_FLOAT ){
					double d[4];
					d[0] = NUM2DBL(rb_ary_entry(v,0));
					d[1] = NUM2DBL(rb_ary_entry(v,1));
					d[2] = NUM2DBL(rb_ary_entry(v,2));
					d[3] = NUM2DBL(rb_ary_entry(v,3));
					return new kmb::Vector4Value(d);
				}else if( TYPE(first) == T_FLOAT && len == 6 ){
					double d[6];
					d[0] = NUM2DBL(rb_ary_entry(v,0));
					d[1] = NUM2DBL(rb_ary_entry(v,1));
					d[2] = NUM2DBL(rb_ary_entry(v,2));
					d[3] = NUM2DBL(rb_ary_entry(v,3));
					d[4] = NUM2DBL(rb_ary_entry(v,4));
					d[5] = NUM2DBL(rb_ary_entry(v,5));
					return new kmb::Tensor6Value(d);
				}else if( TYPE(first) == T_ARRAY && len == 2 ){
					VALUE point = rb_ary_entry(v,0);
					VALUE vec = rb_ary_entry(v,1);
					Check_Type(point, T_ARRAY);
					Check_Type(vec, T_ARRAY);
					double p[3];
					p[0] = NUM2DBL(rb_ary_entry(point,0));
					p[1] = NUM2DBL(rb_ary_entry(point,1));
					p[2] = NUM2DBL(rb_ary_entry(point,2));
					double d[3];
					d[0] = NUM2DBL(rb_ary_entry(vec,0));
					d[1] = NUM2DBL(rb_ary_entry(vec,1));
					d[2] = NUM2DBL(rb_ary_entry(vec,2));
					return new kmb::Point3Vector3Value(p,d);
				}else if( len == 3 && TYPE(first) == T_FLOAT && TYPE(rb_ary_entry(v,2)) == T_FIXNUM ){
					double d[2];
					long iVal;
					d[0] = NUM2DBL(rb_ary_entry(v,0));
					d[1] = NUM2DBL(rb_ary_entry(v,1));
					iVal = FIX2INT(rb_ary_entry(v,2));
					return new kmb::Vector2WithInt(d[0],d[1],iVal);
				}else if( len == 4 && TYPE(first) == T_FLOAT && TYPE(rb_ary_entry(v,3)) == T_FIXNUM ){
					double d[3];
					long iVal;
					d[0] = NUM2DBL(rb_ary_entry(v,0));
					d[1] = NUM2DBL(rb_ary_entry(v,1));
					d[2] = NUM2DBL(rb_ary_entry(v,2));
					iVal = FIX2INT(rb_ary_entry(v,3));
					return new kmb::Vector3WithInt(d[0],d[1],d[2],iVal);
				}else if( len == 5 && TYPE(first) == T_FLOAT && TYPE(rb_ary_entry(v,4)) == T_FIXNUM ){
					double d[4];
					long iVal;
					d[0] = NUM2DBL(rb_ary_entry(v,0));
					d[1] = NUM2DBL(rb_ary_entry(v,1));
					d[2] = NUM2DBL(rb_ary_entry(v,2));
					d[3] = NUM2DBL(rb_ary_entry(v,3));
					iVal = FIX2INT(rb_ary_entry(v,4));
					return new kmb::Vector4WithInt(d[0],d[1],d[2],d[3],iVal);
				}else{
					kmb::ArrayValue* ary = new kmb::ArrayValue();
					for(int i=0;i<len;++i){
						VALUE item = rb_ary_entry(v,i);
						kmb::PhysicalValue* pVal = createFromVALUE(item);
						if( pVal != NULL ){
							ary->appendValue( pVal );
						}
					}
					return ary;
				}
			}
		}
		case T_HASH:
		{
			VALUE ary = rb_ary_new();
			// これで ary に key がすべて入る
			rb_iterate( rb_each, v, (VALUE(*)(...))appendKeys, ary );
			const int len = RARRAY_LEN(ary);
			if( len > 0 ){
				switch( TYPE( rb_ary_entry(ary,0) ) )
				{
				case T_STRING:
					{
						kmb::HashValue* hash = new kmb::HashValue();
						for(int i=0;i<len;++i){
							VALUE key = rb_ary_entry(ary,i);
							Check_Type(key,T_STRING);
							VALUE item = rb_hash_aref( v, key );
							kmb::PhysicalValue* val = createFromVALUE( item );
							if( val != NULL ){
								hash->setValue( StringValueCStr(key), val );
							}
						}
						return hash;
					}
				case T_FIXNUM:
					{
						kmb::IntHashValue* hash = new kmb::IntHashValue();
						for(int i=0;i<len;++i){
							VALUE key = rb_ary_entry(ary,i);
							Check_Type(key,T_FIXNUM);
							VALUE item = rb_hash_aref( v, key );
							kmb::PhysicalValue* val = createFromVALUE( item );
							if( val != NULL ){
								hash->setValue( FIX2INT(key), val );
							}
						}
						return hash;
					}
				default:
					break;
				}
			}
		}
	}
	return NULL;
}

kmb::PhysicalValue* createArrayFromVALUE(VALUE v)
{
	switch( TYPE(v) ){
		case T_ARRAY:
		{
			const int len = RARRAY_LEN(v);
			kmb::ArrayValue* ary = new kmb::ArrayValue();
			for(int i=0;i<len;++i){
				VALUE item = rb_ary_entry(v,i);
				kmb::PhysicalValue* pVal = createFromVALUE(item);
				if( pVal != NULL ){
					ary->appendValue( pVal );
				}
			}
			return ary;
		}
		default:
			break;
	}
	return NULL;
}
