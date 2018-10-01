/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_Refiner version 1.1                          #
# Sample Program Hexahedron                                            #
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
 * �Z�ʑ̂̍ו��`�F�b�N�p
 * �ߓ_�O���[�v�Ƌ��E�ߓ_�O���[�v�̍ו��@�\
 *
 */

#ifdef _CONSOLE

#include "rcapRefiner.h"
#include "rcapRefinerMacros.h"
#include <stdio.h>
#include <stdlib.h>  /* for calloc, free */
#include <assert.h>

int main(void)
{
	/* �Z�ʑ̂�2���ׂ� */
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
	size_t refineNodeCount = 0;
	float64_t* resultCoords = NULL;
	int32_t hexas[16] = {
		1, 2, 3, 4, 5, 6, 7, 8,
		5, 6, 7, 8, 9,10,11,12
	};
	/* �ו���̘Z�ʑ̂̐ߓ_�z��F�o�͂�2*8=16�� */
	int32_t* refineHexas = NULL;
	/* �ו�����v�f�̌^(�萔�l) */
	int8_t etype = RCAP_HEXAHEDRON;
	int32_t nodeOffset = 1;
	int32_t elementOffset = 1;
	/* �����ߓ_�̌� */
	size_t nodeCount = 12;
	/* �����v�f�̌� */
	size_t elementCount = 2;
	/* �ו���̗v�f�̌� */
	size_t refineElementCount = 0;

	/* ���E�����i�ߓ_�O���[�v�j */
	int32_t ng0[4] = {1,2,5,6};
	int32_t* result_ng0 = NULL;
	size_t ng0Count = 4;
	/* ���E�����i���E�ߓ_�O���[�v�j */
	int32_t bng0[3] = {5,6,7};
	int32_t* result_bng0 = NULL;
	size_t bng0Count = 3;

	/* �J�E���^ */
	int32_t i;

	/* �ߓ_�ԍ��̃I�t�Z�b�g�l��^���� */
	rcapInitRefiner( nodeOffset, elementOffset );

	printf("REVOCAP_Refiner sample program : Hexa Refine\n");
	printf("----- Original Model -----\n");
	printf("---\n");
	/* ���W�l�� Refiner �ɗ^���� */
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
	printf("element:\n");
	printf("  - size: %zu\n", elementCount );
	printf("    connectivity:\n");
	for(i=0;(size_t)i<elementCount;++i){
		printf("      - [%d, HEXAHEDRON, %d, %d, %d, %d, %d, %d, %d, %d]\n", i+elementOffset,
			hexas[8*i], hexas[8*i+1], hexas[8*i+2], hexas[8*i+3],
			hexas[8*i+4], hexas[8*i+5], hexas[8*i+6], hexas[8*i+7]);
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

	rcapAppendBNodeGroup("bng0",bng0Count,bng0);
	bng0Count = rcapGetBNodeGroupCount("bng0");
	assert( bng0Count == 3 );
	printf("  - name: bng0\n");
	printf("    mode: NODEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %zu\n",ng0Count);
	printf("    id:\n");
	for(i=0;(size_t)i<bng0Count;++i){
		printf("    - %d\n", bng0[i]);
	}

	printf("----- Refined Model -----\n");
	printf("---\n");

	/* �v�f�̍ו� */
	refineElementCount = rcapGetRefineElementCount( elementCount, etype );
	refineHexas = (int32_t*)calloc( 8*refineElementCount, sizeof(int32_t) );
	elementCount = rcapRefineElement( elementCount, etype, hexas, refineHexas );
	rcapCommit();

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
		printf("      - [%d, HEXAHEDRON, %d, %d, %d, %d, %d, %d, %d, %d]\n", i+elementOffset,
			refineHexas[8*i], refineHexas[8*i+1], refineHexas[8*i+2], refineHexas[8*i+3],
			refineHexas[8*i+4], refineHexas[8*i+5], refineHexas[8*i+6], refineHexas[8*i+7] );
	}

	/* �ו���̐ߓ_�O���[�v�̍X�V */
	ng0Count = rcapGetNodeGroupCount("ng0");
	result_ng0 = (int32_t*)calloc( ng0Count, sizeof(int32_t) );
	rcapGetNodeGroup("ng0",ng0Count,result_ng0);
	printf("Refined Node Group : Count = %zu\n", ng0Count );
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

	free( refineHexas );

	rcapTermRefiner();
	return 0;
}

#endif
