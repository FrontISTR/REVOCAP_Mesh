/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : MeshSmoother                                       #
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
/*------------------------------------------------------------------------------
=begin
=RevocapMeshGen::MeshSmoother

==�T�v

RevocapMeshGen::MeshSmoother �N���X��
RevocapMesh::MeshData �Ɋi�[���ꂽ���b�V���ɂ��� Laplacian smoothing ��
�s�����̂ł���B

�g�����F
 smoother = RevocapMeshGen::MeshSmoother.new(mesh)
 smoother.appendBody(bodyId)
 smoother.init                     # smoothing �p�̈ꎞ�ۑ��p�̗̈�m��
 smoother.smoothingNode(nodeId)    # �K�v�ɉ����� nodeId �ɂ��ČJ��Ԃ�
 smoother.commit                   # smoothing �������ʂ�ߓ_���W�ɔ��f����

==���\�b�h�ꗗ

((<MeshSmoother.new>))
((<appendBody>))
((<init>))
((<commit>))
((<smoothingNode>))
((<smoothingAllNode>))
((<smoothingNodeGroup>))


=end
------------------------------------------------------------------------------*/
%{
#include "MeshGen/kmbMeshSmoother.h"
%}

namespace kmb{

class MeshData;

class MeshSmoother
{
public:
/**--------------------------------------------------------------------------
=begin
--- MeshSmoother.new(mesh)
	���b�V���~�������W���[�� RevocapMeshGen::MeshSmoother �̃C���X�^���X�𐶐�����B
	���b�V���Ɖ~�����̑Ώۂ� mesh �ŗ^����B
=end
---------------------------------------------------------------------------*/
	MeshSmoother(kmb::MeshData* mesh);
	virtual ~MeshSmoother(void);
	size_t appendBody(kmb::bodyIdType bodyId);
/**--------------------------------------------------------------------------
=begin
--- init
	���b�V���~�������W���[���̏�����
=end
---------------------------------------------------------------------------*/
	void init(void);
/**--------------------------------------------------------------------------
=begin
--- commit
	���b�V���~�����̌��ʂ�ߓ_�z��ɔ��f����
=end
---------------------------------------------------------------------------*/
	void commit(void);
/**--------------------------------------------------------------------------
=begin
--- smoothingNode(nodeId)
	�ߓ_�ԍ� nodeId �̓_���~��������B
	���̃��\�b�h�����s���������ł͐ߓ_�z��ɂ͔��f���Ȃ��B
=end
---------------------------------------------------------------------------*/
	void smoothingNode(kmb::nodeIdType nodeId);
	void smoothingAllNode(void);
	void smoothingNodeGroup(const char* name,const char* stype=NULL);
};

}
