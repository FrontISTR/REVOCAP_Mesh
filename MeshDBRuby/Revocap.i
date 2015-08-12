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

���b�V���f�[�^��ėp�I�Ɉ������߂̃��W���[���B
�X�N���v�g���� Ruby �̂��߂̊g���N���X�Q�Ƃ��Ē񋟂����B
REVOCAP_PrePost �� FXRuby �Ŏ������ꂽ GUI �����痘�p�����B

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

// typedef �� %include ���Ȃ��� int �^�̃G�C���A�X����{�^�ɂȂ�Ȃ�
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

RevocapMesh �ɂ����āA���̃��W���[���萔����`����Ă���B

==���W���[���萔
((<VERSION>))
((<COPYRIGHT>))

=end

---------------------------------------------------------------*/

/**--------------------------------------------------------------------
=begin
== ���W���[���萔
�ȉ��̒萔�� RevocapMesh ���W���[���Œ�`����Ă���B
--- VERSION
	�o�[�W�������̕�����
--- COPYRIGHT
	���쌠���̕�����
=end
---------------------------------------------------------------*/
namespace kmb{
#define VERSION "RevocapMesh version 1.6.09 (2015/8/12)"
#define COPYRIGHT "IIS, The University of Tokyo CISS, Large Scale Assembly, Structural Correspondence, Multi Dynamics Simulator"
}

// ���Ԃ͏d�v�I
%include "BoundingBox.i"
%include "MeshData.i"
%include "MeshDB.i"
%include "Collision.i"
%include "Plane.i"
%include "Boundary.i"
%include "Matching.i"
%include "SurfaceOperation.i"
%include "MeshOperation.i"
