/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_Refiner version 1.1                          #
# Sample Program SmoothingRefine                                       #
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
 * �`��␳�@�\�e�X�g�p�v���O�����T���v��
 * �Z�ʑ̂Q����Ȃ�i�q���~���Ō`��␳����
 * �`��␳��ɕi���␳����
 * �W���o�͂����H����� Revocap Neutral Format �`���̃t�@�C���ɂȂ�
 *
 * Sample Program for refinement with fitting to CAD surfaces.
 * This model consists two hexahedra with a column.
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
	int32_t nodeOffset = 0;
	int32_t elementOffset = 0;
	float64_t coords[72] = {
		0.000000,  0.70710678, -0.70710678,
		0.000000,  0.70710678,  0.70710678,
		0.000000, -0.70710678,  0.70710678,
		0.000000, -0.70710678, -0.70710678,
		1.000000,  0.70710678, -0.70710678,
		1.000000,  0.70710678,  0.70710678,
		1.000000, -0.70710678,  0.70710678,
		1.000000, -0.70710678, -0.70710678,
		2.000000,  0.70710678, -0.70710678,
		2.000000,  0.70710678,  0.70710678,
		2.000000, -0.70710678,  0.70710678,
		2.000000, -0.70710678, -0.70710678,

		0.000000,  0.30000000, -0.30000000,
		0.000000,  0.30000000,  0.30000000,
		0.000000, -0.30000000,  0.30000000,
		0.000000, -0.30000000, -0.30000000,
		1.000000,  0.30000000, -0.30000000,
		1.000000,  0.30000000,  0.30000000,
		1.000000, -0.30000000,  0.30000000,
		1.000000, -0.30000000, -0.30000000,
		2.000000,  0.30000000, -0.30000000,
		2.000000,  0.30000000,  0.30000000,
		2.000000, -0.30000000,  0.30000000,
		2.000000, -0.30000000, -0.30000000,
	};
	size_t refineNodeCount = 0;
	float64_t* resultCoords = NULL;
	int32_t hexas[80] = {
		0, 1, 13, 12, 4, 5, 17, 16,
		1, 2, 14, 13, 5, 6, 18, 17,
		2, 3, 15, 14, 6, 7, 19, 18,
		3, 0, 12, 15, 7, 4, 16, 19,

		4, 5, 17, 16, 8,   9, 21, 20,
		5, 6, 18, 17, 9,  10, 22, 21,
		6, 7, 19, 18, 10, 11, 23, 22,
		7, 4, 16, 19, 11,  8, 20, 23,

		12, 13, 14, 15, 16, 17, 18, 19,
		16, 17, 18, 19, 20, 21, 22, 23
	};
	/* �ו���̗v�f�̐ߓ_�z�� */
	int32_t* refineHexas = NULL;
	int32_t* refine2Hexas = NULL;
	int8_t etype = RCAP_HEXAHEDRON;
	/* �����ߓ_�̌� */
	size_t nodeCount = 24;
	/* �����v�f�̌� */
	size_t elementCount = 10;
	/* �ו���̗v�f�̌� */
	size_t refineElementCount = 0;

	/* ���E�����i�ߓ_�O���[�v�j */
	int32_t ng0[4] = {0,3,4,7};
	int32_t* result_ng0 = NULL;
	size_t ng0Count = 4;

	/* �J�E���^ */
	int32_t i;

	/* �ߓ_�ԍ��̃I�t�Z�b�g�l��^���� */
	rcapInitRefiner( nodeOffset, elementOffset );
	rcapSetCADFilename( "data/column2/column2.rnf" );
	rcapSetSmoothing(1);

	printf("REVOCAP_Refiner sample program : Fitting Smoothing Refine\n");
	printf("----- Original Model -----\n");
	printf("---\n");
	/* ���W�l�� Refiner �ɗ^���� */
	/* �ߓ_�z��ŗ^����Ǐ��ߓ_�ԍ��ƁACAD �t�@�C���ɋL�q���Ă�����ߓ_�ԍ��Ƃ̑Ή����^���� */
	rcapSetNode64( nodeCount, coords, NULL, NULL );
	/* �ו��O�̐ߓ_�� */
	nodeCount = rcapGetNodeCount();
	assert( nodeCount == 24 );
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
		printf("      - [%d, HEXAHEDRON, %d, %d, %d, %d, %d, %d, %d, %d]\n", i+elementOffset,
			hexas[8*i], hexas[8*i+1], hexas[8*i+2], hexas[8*i+3],
			hexas[8*i+4], hexas[8*i+5], hexas[8*i+6], hexas[8*i+7]);
	}
	/* �ߓ_�O���[�v�̓o�^ */
	printf("data:\n");
	printf("  - name: ng0\n");
	printf("    mode: NODEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %d\n",ng0Count);
	printf("    id:\n");
	rcapAppendNodeGroup("ng0",ng0Count,ng0);
	ng0Count = rcapGetNodeGroupCount("ng0");
	assert( ng0Count == 4 );
	for(i=0;(size_t)i<ng0Count;++i){
		printf("    - %d\n", ng0[i]);
	}

	/*---------------------- REFINE STEP 1 -----------------------------------------*/
	printf("----- Refined Model -----\n");
	printf("---\n");

	/* �v�f�̍ו� */
	refineElementCount = rcapRefineElement( elementCount, etype, hexas, NULL );
	refineHexas = (int32_t*)calloc( 8*refineElementCount, sizeof(int32_t) );
	elementCount = rcapRefineElement( elementCount, etype, hexas, refineHexas);
	rcapCommit();

	/* �ו���̐ߓ_ */
	refineNodeCount = rcapGetNodeCount();
	printf("node:\n");
	printf("  size: %zu\n", refineNodeCount );
	printf("  coordinate:\n");
	resultCoords = (float64_t*)calloc( 3*refineNodeCount, sizeof(float64_t) );
	rcapGetNodeSeq64( refineNodeCount, nodeOffset, resultCoords );
	for(i=0;(size_t)i<refineNodeCount;++i){
		printf("  - [%d, %f, %f, %f]\n", i+nodeOffset, resultCoords[3*i], resultCoords[3*i+1], resultCoords[3*i+2] );
	}
	free( resultCoords );
	resultCoords = NULL;

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
	printf("data:\n");
	printf("  - name: ng0\n");
	printf("    mode: NODEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %d\n",ng0Count);
	printf("    id:\n");
	rcapGetNodeGroup("ng0",ng0Count,result_ng0);
	for(i=0;(size_t)i<ng0Count;++i){
		printf("    - %d\n", result_ng0[i]);
	}
	free( result_ng0 );
	result_ng0 = NULL;

	/* ��Q�i�̍ו��̑O�ɃL���b�V�����N���A */
	rcapClearRefiner();

	/*---------------------- REFINE STEP 2 -----------------------------------------*/

	/* �v�f�̍ו� */
	elementCount = refineElementCount;
	refineElementCount = rcapRefineElement( elementCount, etype, refineHexas, NULL );
	refine2Hexas = (int32_t*)calloc( 8*refineElementCount, sizeof(int32_t) );
	refineElementCount = rcapRefineElement( elementCount, etype, refineHexas, refine2Hexas );
	rcapCommit();

	printf("----- Refined Model -----\n");
	printf("---\n");

	/* �ו���̐ߓ_ */
	refineNodeCount = rcapGetNodeCount();
	printf("node:\n");
	printf("  size: %zu\n", refineNodeCount );
	printf("  coordinate:\n");
	resultCoords = (float64_t*)calloc( 3*refineNodeCount, sizeof(float64_t) );
	rcapGetNodeSeq64( refineNodeCount, nodeOffset, resultCoords );
	for(i=0;(size_t)i<refineNodeCount;++i){
		printf("  - [%d, %f, %f, %f]\n", i+nodeOffset, resultCoords[3*i], resultCoords[3*i+1], resultCoords[3*i+2] );
	}
	free( resultCoords );
	resultCoords = NULL;

	/* �ו���̗v�f */
	printf("element:\n");
	printf("  - size: %zu\n", refineElementCount );
	printf("    connectivity:\n");
	for(i=0;(size_t)i<refineElementCount;++i){
		printf("      - [%d, HEXAHEDRON, %d, %d, %d, %d, %d, %d, %d, %d]\n", i+elementOffset,
			refine2Hexas[8*i], refine2Hexas[8*i+1], refine2Hexas[8*i+2], refine2Hexas[8*i+3],
			refine2Hexas[8*i+4], refine2Hexas[8*i+5], refine2Hexas[8*i+6], refine2Hexas[8*i+7] );
	}

	/* �ו���̐ߓ_�O���[�v�̍X�V */
	ng0Count = rcapGetNodeGroupCount("ng0");
	result_ng0 = (int32_t*)calloc( ng0Count, sizeof(int32_t) );
	printf("data:\n");
	printf("  - name: ng0\n");
	printf("    mode: NODEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %d\n",ng0Count);
	printf("    id:\n");
	rcapGetNodeGroup("ng0",ng0Count,result_ng0);
	for(i=0;(size_t)i<ng0Count;++i){
		printf("    - %d\n", result_ng0[i]);
	}

	free( result_ng0 );
	result_ng0 = NULL;

	free( refineHexas );
	refineHexas = NULL;

	free( refine2Hexas );
	refine2Hexas = NULL;

	rcapTermRefiner();
	return 0;
}

#endif
