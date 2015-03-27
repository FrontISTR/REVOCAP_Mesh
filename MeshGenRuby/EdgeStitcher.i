/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : EdgeStithcer                                       #
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
=RevocapMeshGen::EdgeStitchcer

==�T�v

RevocapMeshGen::EdgeStitcher �N���X��
RevocapMesh::MeshData �Ɋi�[���ꂽ��������Ȃ�Ӄf�[�^�ɂ��āA
�����D�����킹��O�p�`�܂��͎l�p�`�p�b�`�𐶐�����B

==���\�b�h�ꗗ

((<EdgeStitcher.new>))
((<setMesh>))
((<setEdges>>)
((<setEdgesWithinDistance>))
((<setEdgesWithInitNodes>))
((<stitchEdgesByTriangles>))
((<stitchEdgesByQuads>))
((<stitchByQuadsWithNodeCount>))
((<getNormalCheck>))
((<setNormalCheck>))

==���\�b�h

=end
------------------------------------------------------------------------------*/
%{
#include "MeshGen/kmbEdgeStitcher.h"
%}

namespace kmb{

class MeshData;

class EdgeStitcher
{
public:
/**--------------------------------------------------------------------------
=begin
--- EdgeStitcher.new
	�ӖD�����킹�����N���X EdgeStitcher �̃C���X�^���X�𐶐�����B
=end
---------------------------------------------------------------------------*/
	EdgeStitcher(void);
	virtual ~EdgeStitcher(void);

/**--------------------------------------------------------------------------
=begin
--- setMesh(mesh)
	EdgeStitcher ���ΏۂƂ��郁�b�V����ݒ肷��B
=end
---------------------------------------------------------------------------*/
	void setMesh(kmb::MeshData* mesh);
/**--------------------------------------------------------------------------
=begin
--- setEdges( forward, backward )
	EdgeStitcher ���D�����킹��ΏۂƂȂ�Ӄf�[�^��ݒ肷��B
	forward �� backward �ł��ꂼ��� MeshData �ɂ����� bodyId ��^����B
	�D�����킹�̃p�b�`���쐬����ۂ� forward �͏������Abackward �͋t�����ɑ�������B
	���ꂼ��̕Ӄf�[�^�ɂ��Ď����I�ɐ擪�ƏI�[�𒲂ׂ�B
	�Е������[�v�̏ꍇ�́A��������̊J�n�_�Ɉ�ԋ߂��_���J�n�_�ɂ���B
	���������[�v�̏ꍇ�́Aedge0 �̍ŏ��̗v�f�̐ߓ_���J�n�_�ɂ��āAedge1 �̊J�n�_��
	edge0 �̊J�n�_����ł��߂��_�Ƃ���B

--- setEdgesWithinDistance( forward, backward, distance )
	EdgeStitcher ���D�����킹��ΏۂƂȂ�Ӄf�[�^��ݒ肷��B
	forward �� backward �Ɋ܂܂�邷�ׂĂ̐����ł͂Ȃ��A
	�����̗��[�̒��_����ΏۂƂȂ�ӂɊ܂܂��܂ł̋������Ƃ��� distance �ȉ���
	����������D�����킹��ΏۂƂ���B

---	setEdgesWithInitNodes( forward, forwardInit, backward, backwardInit )
	EdgeStitcher ���D�����킹��ΏۂƂȂ�Ӄf�[�^�ƖD�����킹�̊J�n�_��ݒ肷��B
	forwardInit �� forward ���\���Ӄf�[�^�ɂ�����ߓ_�ԍ��A
	backwardInit �� backward ���\���Ӄf�[�^�ɂ�����ߓ_�ԍ���^���A
	�����̐ߓ_����D�����킹�������J�n����B

=end
---------------------------------------------------------------------------*/
	bool setEdges(const char* forward,const char* backward);
	bool setEdges( kmb::bodyIdType forward, kmb::bodyIdType backward );
	bool setEdgesWithinDistance(kmb::bodyIdType forward,kmb::bodyIdType backward,double distance);
	bool setEdgesWithInitNodes(kmb::bodyIdType forward,kmb::nodeIdType forwardInit,kmb::bodyIdType backward,kmb::nodeIdType backwardInit);

/**--------------------------------------------------------------------------
=begin
--- stitchEdgesByTriangles()
	�O�p�`�ŖD�����킹����������B���������O�p�`�p�b�`�� MeshData �Ɋi�[���Ă��� bodyId ��Ԃ��B
	�[�_�ɂ��ǂ���ꍇ�������́A�J�n�_�ɖ߂�ꍇ�ɏI������B
	���荇�킹��O�p�`�̌����́A�������O�p�`�̋��E�ƂȂ�悤�ɂ���
	�i�O�p�`�̌����Ɛ����̌����͋t�j�B

--- stitchEdgesByQuads()
	�l�p�`�ŖD�����킹����������B���������l�p�`�p�b�`�� MeshData �Ɋi�[���Ă��� bodyId ��Ԃ��B
	�[�_�ɂ��ǂ���ꍇ�������́A�J�n�_�ɖ߂�ꍇ�ɏI������B
	�ӂ̌��̋��̃p���e�B���قȂ鎞�́A�Ō�̎l�p�`�����߂��Ȃ��ꍇ������B
	���荇�킹��l�p�`�̌����́A�������l�p�`�̋��E�ƂȂ�悤�ɂ���
	�i�l�p�`�̌����Ɛ����̌����͋t�j�B
=end
---------------------------------------------------------------------------*/
	kmb::bodyIdType stitchEdgesByTriangles(void);
	kmb::bodyIdType stitchEdgesByQuads(void);
	kmb::bodyIdType stitchByQuadsWithNodeCount(int fCount,int bCount);

/**--------------------------------------------------------------------------
=begin
--- getNormalCheck()
	�D�����킹��ӂ̑I�ѕ��Ŗ@���x�N�g�����l�����邩�ǂ����𒲂ׂ�B

--- setNormalCheck(check)
	�D�����킹��ӂ̑I�ѕ��Ŗ@���x�N�g�����l�����邩�ǂ����ݒ肷��B
	�O������ԓ��̕��ʏ�ɕӃf�[�^������ꍇ�� check = true �Ƃ���ƁA
	��������p�b�`�f�[�^�̌��������낤�B�@���x�N�g�����l�����Ȃ��ꍇ�́A
	�p�b�`�f�[�^�𐶐�����ӂ̑I�ѕ��͎O�p�`�̌`��Ŕ��f���邽�߁A
	�ꍇ�ɂ���Ă͕��ʓ��Ő܂ꂽ�p�b�`�f�[�^�����������ꍇ������B
=end
---------------------------------------------------------------------------*/
	bool getNormalCheck(void);
	void setNormalCheck(bool check);
};

}
