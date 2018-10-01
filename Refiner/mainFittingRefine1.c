/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_Refiner version 1.1                          #
# Sample Program FittingRefine                                         #
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
 * �`��t�@�C���� column.rnf (z=0�œ�̋Ȗʂ𒣂荇�킹)
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
	int32_t nodeOffset = 1;
	int32_t elementOffset = 1;
	float64_t coords[36] = {
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		0.0,-1.0, 0.0,
		0.0, 0.0,-1.0,
		1.0, 1.0, 0.0,
		1.0, 0.0, 1.0,
		1.0,-1.0, 0.0,
		1.0, 0.0,-1.0,
		2.0, 1.0, 0.0,
		2.0, 0.0, 1.0,
		2.0,-1.0, 0.0,
		2.0, 0.0,-1.0,
	};
	/*
	 * CAD �t�@�C���ɋL�q���Ă���O���[�o���ߓ_�ԍ��Ƃ̑Ή�
	 * nodeOffset = 1 ���������l�� CAD �t�@�C���ɋL�q���Ă��邱�Ƃɒ���
	 */
	int32_t globalIds[12] = {
		101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112
	};
	int32_t localIds[12] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
	};
	size_t refineNodeCount = 0;
	float64_t* resultCoords = NULL;
	int32_t hexas[16] = {
		1, 2, 3, 4, 5, 6, 7, 8,
		5, 6, 7, 8, 9, 10, 11, 12,
	};
	/* �ו���̗v�f�̐ߓ_�z�� */
	int32_t* refineHexas = NULL;
	int8_t etype = RCAP_HEXAHEDRON;
	/* �����ߓ_�̌� */
	size_t nodeCount = 12;
	/* �����v�f�̌� */
	size_t elementCount = 2;
	/* �ו���̗v�f�̌� */
	size_t refineElementCount = 0;

	/* ���E�����i�ߓ_�O���[�v�j */
	int32_t ng0[4] = {1,4,5,8};
	int32_t* result_ng0 = NULL;
	size_t ng0Count = 4;

	/* �J�E���^ */
	int32_t i,j;

	/* �ߓ_�ԍ��̃I�t�Z�b�g�l��^���� */
	rcapInitRefiner( nodeOffset, elementOffset );
	rcapSetCADFilename( "data/column/column.rnf" );

	printf("REVOCAP_Refiner sample program : Fitting Refine1\n");
	printf("----- Original Model -----\n");
	printf("---\n");
	/* ���W�l�� Refiner �ɗ^���� */
	/* �ߓ_�z��ŗ^����Ǐ��ߓ_�ԍ��ƁACAD �t�@�C���ɋL�q���Ă�����ߓ_�ԍ��Ƃ̑Ή����^���� */
	rcapSetNode64( nodeCount, coords, globalIds, localIds );
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
	assert( elementCount == 2 );
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

	printf("----- Refined Model -----\n");
	printf("---\n");

	/* �v�f�̍ו� */
	refineElementCount = rcapGetRefineElementCount( elementCount, etype );
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
	for(j=0;(size_t)j<refineNodeCount;++j){
		printf("  - [%d, %f, %f, %f]\n", j+nodeOffset, resultCoords[3*j], resultCoords[3*j+1], resultCoords[3*j+2] );
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

	free( refineHexas );
	rcapTermRefiner();
	return 0;
}

#endif
