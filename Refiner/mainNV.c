/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_Refiner version 1.1                          #
# Sample Program NodeVariable                                          #
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
/*
 *
 * �T���v�����s�ၕ�e�X�g�p�v���O����
 * BoundaryNodeGroup �� BoundaryNodeVariable �̋����`�F�b�N�p
 *
 */

#ifdef _CONSOLE

#include "rcapRefiner.h"
#include "rcapRefinerMacros.h"
#include <stdio.h>
#include <stdlib.h>  /* for calloc, free */
#include <assert.h>
#include <string.h>

int main(void)
{
	/* �Z�ʑ̂��T�����������̂�2���ׂ� */
	float64_t coords[36] = {
		0.0, 0.0, -1.0,
		1.0, 0.0, -1.0,
		1.0, 1.0, -1.0,
		0.0, 1.0, -1.0,
		0.0, 0.0,  0.0,
		1.0, 0.0,  0.0,
		1.0, 1.0,  0.0,
		0.0, 1.0,  0.0,
		0.0, 0.0,  1.0,
		1.0, 0.0,  1.0,
		1.0, 1.0,  1.0,
		0.0, 1.0,  1.0,
	};
	/* �ו���̍��W�F�K�v�ɉ����� calloc ���� */
	float64_t* resultCoords = NULL;
	int32_t tetras[40] = {
		1, 2, 4, 5,
		2, 3, 4, 7,
		4, 5, 7, 8,
		2, 4, 5, 7,
		6, 2, 5, 7,
		5, 6, 7,10,
		7,10,11,12,
		5, 9,10,12,
		5, 7, 8,12,
		5,10, 7,12
	};
	/* �ו���̎l�ʑ̂̐ߓ_�z��F�o�͂�10*8=80�� */
	int32_t* refineTetras = NULL;
	/* �ו�����v�f�̌^(�萔�l) */
	int8_t etype = RCAP_TETRAHEDRON;
	int32_t nodeOffset = 1;
	int32_t elementOffset = 1;
	/* �����ߓ_�̌� */
	size_t nodeCount = 12;
	/* �����v�f�̌� */
	size_t elementCount = 10;
	/* �ו���̗v�f�̌� */
	size_t refineElementCount = 0;
	/* �ו���̐ߓ_�̌� */
	size_t refineNodeCount = 0;

	/* ���̐����̎擾 */
	int32_t seg[2] = {-1,-1};
	int8_t orgtype = RCAP_UNKNOWNTYPE;

	/* ���E���� */
	int32_t ng0[4] = {1,2,5,6};
	int32_t* result_ng0 = NULL;
	size_t ng0Count = 4;
	int32_t bng0[3] = {5,6,7};
	int32_t* result_bng0 = NULL;
	size_t bng0Count = 3;
	int32_t bnv0[3] = {5,6,7};  /* �ߓ_ */
	int32_t bnv1[3] = {1,2,3};  /* �ߓ_��̒l */
	int32_t* result_bnv0 = NULL;
	int32_t* result_bnv1 = NULL;
	size_t bnv0Count = 3;
	int32_t i = 0;
	int32_t j = 0;
	int32_t middle = -1;
	int32_t flag = 0;
	char mode[32];
	/* �ߓ_�ԍ��̃I�t�Z�b�g�l��^���� */
	rcapInitRefiner( nodeOffset, elementOffset );
	/* ���_�� Variable �͏���������^���� */
	rcapSetInterpolateMode( "MIN" );
	rcapGetInterpolateMode( mode );
	assert( strncmp( "MIN", mode, 3 )==0 );

	printf("REVOCAP_Refiner sample program : Boundary Node Variable Refine\n");
	printf("----- Original Model -----\n");
	printf("---\n");
	/* ���W�l�� Refiner �ɋ����� */
	rcapSetNode64( nodeCount, coords, NULL, NULL );
	/* �ו��O�̐ߓ_�� */
	nodeCount = rcapGetNodeCount();
	assert( nodeCount == 12 );
	printf("node:\n");
	printf("  size: %zu\n", nodeCount );
	printf("  coordinate:\n");
	for(i=0;(size_t)i<nodeCount;++i){
		printf("  - [%d, %f, %f, %f]\n", i+nodeOffset, coords[3*i], coords[3*i+1], coords[3*i+2] );
	}

	/* �ו��O�̗v�f�� */
	assert( elementCount == 10 );
	printf("element:\n");
	printf("  - size: %zu\n", elementCount );
	printf("    connectivity:\n");
	for(i=0;(size_t)i<elementCount;++i){
		printf("      - [%d, TETRAHEDRON, %d, %d, %d, %d]\n", i+elementOffset,
			tetras[4*i], tetras[4*i+1], tetras[4*i+2], tetras[4*i+3] );
	}

	/* �ߓ_�O���[�v�̓o�^ */
	rcapAppendNodeGroup("ng0",ng0Count,ng0);
	ng0Count = rcapGetNodeGroupCount("ng0");
	assert( ng0Count == 4 );
	printf("data:\n");
	printf("  - name: ng0\n");
	printf("    mode: NODEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %zu\n",ng0Count);
	printf("    id:\n");
	for(i=0;(size_t)i<ng0Count;++i){
		printf("    - %d\n", ng0[i]);
	}

	/* ���E�ߓ_�O���[�v�̓o�^ */
	rcapAppendBNodeGroup("bng0",bng0Count,bng0);
	bng0Count = rcapGetBNodeGroupCount("bng0");
	assert( bng0Count == 3 );
	printf("  - name: bng0\n");
	printf("    mode: NODEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %zu\n",bng0Count);
	printf("    id:\n");
	for(i=0;(size_t)i<ng0Count;++i){
		printf("    - %d\n", bng0[i]);
	}

	/* ���E�ߓ_�ϐ��̓o�^ */
	rcapAppendBNodeVarInt("bnv0",bnv0Count,bnv0,bnv1);
	bnv0Count = rcapGetBNodeVarIntCount("bnv0");
	assert( bnv0Count == 3 );
	printf("  - name: bnv0\n");
	printf("    mode: NODEVARIABLE\n");
	printf("    vtype: INTEGER\n");
	printf("    size: %zu\n",bnv0Count);
	printf("    value:\n");
	for(i=0;(size_t)i<bnv0Count;++i){
		printf("    - [%d, %d]\n", bnv0[i], bnv1[i]);
	}

	/*---------------------- REFINE -----------------------------------------*/

	/* �v�f�̍ו� */
	refineElementCount = rcapGetRefineElementCount( elementCount, etype );
	refineTetras = (int32_t*)calloc( 4*refineElementCount, sizeof(int32_t) );
	elementCount = rcapRefineElement( elementCount, etype, tetras, refineTetras );
	rcapCommit();

	printf("----- Refined Model -----\n");
	printf("---\n");

	/* �ו���̐ߓ_ */
	refineNodeCount = rcapGetNodeCount();
	resultCoords = (float64_t*)calloc( 3*refineNodeCount, sizeof(float64_t) );
	rcapGetNodeSeq64( refineNodeCount, nodeOffset, resultCoords );
	printf("node:\n");
	printf("  size: %zu\n", refineNodeCount );
	printf("  coordinate:\n");
	for(i=0;(size_t)i<refineNodeCount;++i){
		printf("  - [%d, %f, %f, %f]\n", i+nodeOffset, resultCoords[3*i], resultCoords[3*i+1], resultCoords[3*i+2] );
	}
	free( resultCoords );

	/* �ו���̗v�f */
	printf("element:\n");
	printf("  - size: %zu\n", refineElementCount );
	printf("    connectivity:\n");
	for(i=0;(size_t)i<refineElementCount;++i){
		printf("      - [%d, TETRAHEDRON, %d, %d, %d, %d]\n", i+elementOffset,
			refineTetras[4*i], refineTetras[4*i+1], refineTetras[4*i+2], refineTetras[4*i+3] );
	}

	/* �ו���̐ߓ_�O���[�v�̍X�V */
	ng0Count = rcapGetNodeGroupCount("ng0");
	assert( ng0Count > 0 );
	result_ng0 = (int32_t*)calloc( ng0Count, sizeof(int32_t) );
	rcapGetNodeGroup("ng0",ng0Count,result_ng0);
	printf("data:\n");
	printf("  - name: ng0\n");
	printf("    mode: NODEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %d\n",ng0Count);
	printf("    id:\n");
	for(i=0;(size_t)i<ng0Count;++i){
		printf("    - %d\n", result_ng0[i]);
	}
	free( result_ng0 );
	result_ng0 = NULL;

	/* ���E�ߓ_�O���[�v�̓o�^ */
	bng0Count = rcapGetBNodeGroupCount("bng0");
	result_bng0 = (int32_t*)calloc( bng0Count, sizeof(int32_t) );
	rcapGetBNodeGroup("bng0",bng0Count,result_bng0);
	printf("  - name: bng0\n");
	printf("    mode: NODEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %d\n",bng0Count);
	printf("    id:\n");
	for(i=0;(size_t)i<bng0Count;++i){
		printf("    - %d\n", result_bng0[i]);
	}
	free( result_bng0 );

	/* ���E�ߓ_�ϐ��̓o�^ */
	bnv0Count = rcapGetBNodeVarIntCount("bnv0");
	result_bnv0 = (int32_t*)calloc( bnv0Count, sizeof(int32_t) );
	result_bnv1 = (int32_t*)calloc( bnv0Count, sizeof(int32_t) );
	rcapGetBNodeVarInt("bnv0",bnv0Count,result_bnv0,result_bnv1);
	printf("  - name: bnv0\n");
	printf("    mode: NODEVARIABLE\n");
	printf("    vtype: INTEGER\n");
	printf("    size: %zu\n",bnv0Count);
	printf("    value:\n");
	for(i=0;(size_t)i<bnv0Count;++i){
		printf("    - [%d, %d]\n", result_bnv0[i], result_bnv1[i]);
	}

	/* �����Ń`�F�b�N�I */
	for(i=0;(size_t)i<bnv0Count;++i){
		orgtype = rcapGetOriginal( result_bnv0[i], seg );
		if( orgtype == RCAP_SEGMENT ){
			middle = rcapGetMiddle( orgtype, seg );
			assert( middle == result_bnv0[i] );
			/* ���������ɂȂ��Ă��邱�Ƃ��m���߂� */
			flag = 0;
			for(j=0;(size_t)j<bnv0Count;++j){
				if( bnv0[j] == seg[0] ){
					assert( bnv1[j] >= result_bnv1[i] );
					if( bnv1[j] == result_bnv1[i] ){
						flag = 1;
					}
				}
				if( bnv0[j] == seg[1] ){
					assert( bnv1[j] >= result_bnv1[i] );
					if( bnv1[j] == result_bnv1[i] ){
						flag = 1;
					}
				}
			}
			assert( flag == 1 );
		}
	}
	free( result_bnv0 );
	free( result_bnv1 );
	free( refineTetras );
	rcapTermRefiner();
	return 0;
}

#endif
