/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Module Name : RevocapMesh                                       #
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

/**-------------------------------------------------------------
=begin
= RevocapMesh

メッシュデータを汎用的に扱うためのモジュール。
スクリプト言語 Ruby のための拡張クラス群として提供される。
REVOCAP_PrePost の FXRuby で実装された GUI 部から利用される。

=end

---------------------------------------------------------------------*/

%module RevocapMesh

%{
#ifdef _MSC_VER
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#endif
%}

#define REVOCAP_SUPPORT_RUBY

%{
#include "MeshDB/kmbElement.h"
#include "MeshDB/kmbElement.h"
#include "MeshDB/kmbElementContainer.h"
#include "MeshDB/kmbElementContainerMap.h"
#include "MeshDB/kmbElementContainerArray.h"
#include "MeshDB/kmbElementContainerTriangleArray.h"
#include "MeshDB/kmbMeshData.h"
#include "Geometry/kmbGeometry2D.h"
#include "Geometry/kmbGeometry3D.h"
#include "Geometry/kmbGeometry4D.h"
#include "Geometry/kmbPoint3DContainer.h"
#include "MeshDBRuby/kmbMeshDB_SWIG.h"
#include "MeshDBRuby/kmbPhysicalValue_SWIG.h"
%}

// typedef は %include しないと int 型のエイリアスが基本型にならない
%include "../MeshDB/kmbTypes.h"
%include "../Common/kmbCommon.h"
%include "../Geometry/kmbIdTypes.h"
%include "../Geometry/kmbGeometry4D.h"
%include "../Geometry/kmbGeometry3D.h"
%include "../Geometry/kmbGeometry2D.h"
%include "../Geometry/kmbRegion.h"
%include "../Geometry/kmbBoxRegion.h"

/**--------------------------------------------------------------------
=begin

RevocapMesh において、次のモジュール定数が定義されている。

==モジュール定数
((<VERSION>))
((<COPYRIGHT>))

=end

---------------------------------------------------------------*/

/**--------------------------------------------------------------------
=begin
== モジュール定数
以下の定数は RevocapMesh モジュールで定義されている。
--- VERSION
	バージョン情報の文字列
--- COPYRIGHT
	著作権情報の文字列
=end
---------------------------------------------------------------*/
namespace kmb{
#define VERSION "RevocapMesh version 1.6.09 (2015/8/12)"
#define COPYRIGHT "IIS, The University of Tokyo CISS, Large Scale Assembly, Structural Correspondence, Multi Dynamics Simulator"
}

// 順番は重要！
%include "BoundingBox.i"
%include "MeshData.i"
%include "MeshDB.i"
%include "Collision.i"
%include "Plane.i"
%include "Boundary.i"
%include "Matching.i"
%include "SurfaceOperation.i"
%include "MeshOperation.i"
