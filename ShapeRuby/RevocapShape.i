/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Module Name : RevocapShape                                      #
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

%module RevocapShape

/**--------------------------------------------------------------------
=begin
= RevocapShape

==概要

RevocapShape とは RevocapMesh と OpenCASCADE のインターフェイス
および形状処理を記述したモジュール

==モジュール定数

((<VERSION>))
((<COPYRIGHT>))

== モジュール定数
以下の定数は RevocapShape モジュールで定義されている。
--- VERSION
	バージョン情報の文字列
--- COPYRIGHT
	著作権情報の文字列
=end
---------------------------------------------------------------*/
namespace kmb{
#define VERSION "RevocapShape version 1.6.01 (2013/3/8)"
#define COPYRIGHT "IIS, The University of Tokyo CISS, Large Scale Assembly, Structural Correspondence, Multi Dynamics Simulator"
}



%{
#ifdef _MSC_VER
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#endif
%}

// swig の制御文
#ifdef OPENCASCADE

// cxx ファイルにそのまま出力される
%{
#define OPENCASCADE 1
#include "Shape/kmbCADFileIO.h"
#include "Shape/kmbShapeData.h"
#include "Shape/kmbPatchGenerator.h"
%}

/**-------------------------------------------------------------
=begin
=RevocapShape::ShapeData

==概要

CAD データを管理するためのクラス。
内部にパラメトリックな曲面データと Brep 構造を保持する。

==メソッド一覧

((<ShapeData.new>))
((<convertToBezier>))
((<fixShape>))
((<dropSmallEdge>))
((<isClosed>))
((<isValid>))
((<saveToRNF>))

==メソッド

=end
---------------------------------------------------------------------*/

%include "../Shape/kmbPatchGenerator.h"
%include "../Shape/kmbCADFileIO.h"

namespace kmb{

class ShapeData
{
public:
/**--------------------------------------------------------------------------
=begin
--- ShapeData.new
=end
---------------------------------------------------------------------------*/
	ShapeData(void);
	virtual ~ShapeData(void);
/**--------------------------------------------------------------------------
=begin
--- convertToBezier
=end
---------------------------------------------------------------------------*/
	void convertToBezier(void);
/**--------------------------------------------------------------------------
=begin
--- fixShape(precision,tolerance)
--- dropSmallEdge(precision,tolerance)
=end
---------------------------------------------------------------------------*/
	void fixShape(double precision,double tolerance);
	void dropSmallEdge(double precision,double tolerance);
/**--------------------------------------------------------------------------
=begin
--- isClosed()
    OpenCASCADE の関数を使って、閉じているかを判定する
=end
---------------------------------------------------------------------------*/
	bool isClosed(void) const;
/**--------------------------------------------------------------------------
=begin
--- isValid()
    OpenCASCADE の関数を使って、妥当かどうかを判定する
=end
---------------------------------------------------------------------------*/
	bool isValid(void) const;
/**--------------------------------------------------------------------------
=begin
--- saveToRNF(filename,append=false)
    RNF 形式で曲面情報を出力する
=end
---------------------------------------------------------------------------*/
	int saveToRNF(const char* filename,bool append=false) const;
};

}

// swig の制御文
#endif

%{
#include "Shape/kmbBSpline.h"
#include "Shape/kmbSurface3D.h"
%}

namespace kmb{

class Surface3D;

}

%include "../MeshDB/kmbTypes.h"
%include "../Geometry/kmbIdTypes.h"

%include "../Shape/kmbBSpline.h"
%include "BezierCurve2D.i"
%include "BSplineCurve2D.i"
%include "NurbsCurve2D.i"
%include "BezierSurface3D.i"
%include "BSplineSurface3D.i"
%include "NurbsSurface3D.i"
%include "RnfShapeIO.i"
