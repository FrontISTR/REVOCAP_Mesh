/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : NurbsSurface3D                                     #
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
=RevocapShape::RnfShapeIO

Rnf 形式の形状ファイルの読み込み

==メソッド一覧

((<RnfShapeIO.new>))

((<loadFromFile>))

==メソッド

=end
------------------------------------------------------------------------------*/

%{
#include "Shape/kmbNurbsSurface3D.h"
#include "Shape/kmbRnfShapeIO.h"
%}

namespace kmb{

class RnfShapeIO
{
public:
/**--------------------------------------------------------------------------
=begin
--- RnfShapeIO.new
    コンストラクタ
=end
------------------------------------------------------------------------------*/
	RnfShapeIO(void);
	virtual ~RnfShapeIO(void);
/**--------------------------------------------------------------------------
=begin
--- loadFromFile( filename )
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE loadFromFile( const char* filename ){
		VALUE ary = rb_ary_new();
		std::vector< kmb::Surface3D* > surfaces;
		self->loadFromFile( filename, surfaces );
		for( std::vector< kmb::Surface3D* >::iterator sIter = surfaces.begin();
			sIter != surfaces.end(); ++sIter )
		{
			kmb::NurbsSurface3D* nurbs = reinterpret_cast< kmb::NurbsSurface3D* >( *sIter );
			if( nurbs != NULL ){
				VALUE obj = SWIG_NewPointerObj(nurbs, SWIGTYPE_p_kmb__NurbsSurface3D, SWIG_POINTER_OWN | 0);
				rb_ary_push(ary,obj);
			}
		}
		return ary;
	}
}

	int appendSurfaceToFile(const char* filename, kmb::NurbsSurface3D* surface);
};

}
