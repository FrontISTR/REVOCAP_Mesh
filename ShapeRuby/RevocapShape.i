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

==�T�v

RevocapShape �Ƃ� RevocapMesh �� OpenCASCADE �̃C���^�[�t�F�C�X
����ь`�󏈗����L�q�������W���[��

==���W���[���萔

((<VERSION>))
((<COPYRIGHT>))

== ���W���[���萔
�ȉ��̒萔�� RevocapShape ���W���[���Œ�`����Ă���B
--- VERSION
	�o�[�W�������̕�����
--- COPYRIGHT
	���쌠���̕�����
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

// swig �̐��䕶
#ifdef OPENCASCADE

// cxx �t�@�C���ɂ��̂܂܏o�͂����
%{
#define OPENCASCADE 1
#include "Shape/kmbCADFileIO.h"
#include "Shape/kmbShapeData.h"
#include "Shape/kmbPatchGenerator.h"
%}

/**-------------------------------------------------------------
=begin
=RevocapShape::ShapeData

==�T�v

CAD �f�[�^���Ǘ����邽�߂̃N���X�B
�����Ƀp�����g���b�N�ȋȖʃf�[�^�� Brep �\����ێ�����B

==���\�b�h�ꗗ

((<ShapeData.new>))
((<convertToBezier>))
((<fixShape>))
((<dropSmallEdge>))
((<isClosed>))
((<isValid>))
((<saveToRNF>))

==���\�b�h

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
    OpenCASCADE �̊֐����g���āA���Ă��邩�𔻒肷��
=end
---------------------------------------------------------------------------*/
	bool isClosed(void) const;
/**--------------------------------------------------------------------------
=begin
--- isValid()
    OpenCASCADE �̊֐����g���āA�Ó����ǂ����𔻒肷��
=end
---------------------------------------------------------------------------*/
	bool isValid(void) const;
/**--------------------------------------------------------------------------
=begin
--- saveToRNF(filename,append=false)
    RNF �`���ŋȖʏ����o�͂���
=end
---------------------------------------------------------------------------*/
	int saveToRNF(const char* filename,bool append=false) const;
};

}

// swig �̐��䕶
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
