/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Module Name : RevocapMeshGen                                    #
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

=RevocapMeshGen モジュール

==モジュール定数

((<VERSION>))
((<COPYRIGHT>))

== モジュール定数
--- VERSION
	バージョン情報の文字列
--- COPYRIGHT
	著作権情報の文字列
=end
---------------------------------------------------------------*/
%module RevocapMeshGen  // create_makefile と同じ名前にする

#ifndef VERSION
namespace kmb{
#define VERSION "RevocapMeshGen version 1.6.01 (2013/3/8)"
#define COPYRIGHT "IIS, The University of Tokyo CISS, Large Scale Assembly, Structural Correspondence, Multi Dynamics Simulator"
}
#endif

%{
#ifdef _MSC_VER
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#endif
#include "MeshDB/kmbMeshData.h"
#include "MeshDB/kmbMeshDB.h"
%}

%include "../MeshDB/kmbTypes.h"
%include "../Geometry/kmbIdTypes.h"
%include "../Common/kmbCommon.h"
%include "MeshRefiner.i"
%include "BLMGenerator.i"
%include "PatchModifier.i"
%include "EdgeStitcher.i"
%include "MeshSmoother.i"

