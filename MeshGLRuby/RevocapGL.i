/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Module Name : RevocapGL                                         #
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
= RevocapGL

RevocapGL とは RevocapMesh のデータ構造に対応して、
RevocapVisual における FXRuby での OpenGL の描画を高速化する目的で作られた
モジュールである。

==モジュール定数

((<VERSION>))
((<COPYRIGHT>))

== モジュール定数
以下の定数は RevocapGL モジュールで定義されている。
--- VERSION
	バージョン情報の文字列
--- COPYRIGHT
	著作権情報の文字列

=end
---------------------------------------------------------------*/
%module RevocapGL

#ifndef VERSION
namespace kmb{
#define VERSION "RevocapGL version 1.6.03 (2013/3/8)"
#define COPYRIGHT "IIS, The University of Tokyo CISS, Large Scale Assembly, Structural Correspondence, Multi Dynamics Simulator"
}
#endif

%{
#ifdef _MSC_VER
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#endif
%}

#ifdef REVOCAP_SUPPORT_GLEW
%{
#define REVOCAP_SUPPORT_GLEW 1
#include "MeshGL/kmbFBORendering.h"
%}
#endif

%{
#include "MeshGL/kmbMeshGL.h"
#include "MeshGL/kmbColorMap.h"
#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbMeshData.h"
#include "Geometry/kmbIdTypes.h"
%}

%include "../MeshDB/kmbTypes.h"
%include "../Geometry/kmbIdTypes.h"
%include "../Common/kmbCommon.h"


%include "MeshGL.i"
%include "ColorMap.i"
%include "FBORendering.i"
