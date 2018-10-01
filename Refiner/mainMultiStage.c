/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_Refiner version 1.1                          #
# Sample Program MultiStage                                            #
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
 * �Q�i�K���s�T���v���v���O����
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
	/*
	 * �g�����̗�
	 * ���߂�5�͍ו�����O�̐ߓ_���W
	 */
	float64_t coords[15] = {
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0
	};
	/* �ו���̍��W�F�K�v�ɉ����� calloc ���� */
	float64_t* resultCoords = NULL;
	/* �l�ʑ̂̐ߓ_�z��F���͂�2�� */
	int32_t tetras[8] = {
		0, 1, 2, 3,
		1, 2, 3, 4
	};
	/* �ו���̎l�ʑ̂̐ߓ_�z��F�o�͂�2*8=16��*/
	int32_t* refineTetras = NULL;
	/* �Q�i�K�ו���̎l�ʑ̂̐ߓ_�z��F�o�͂�2*8*8=128��*/
	int32_t* refine2Tetras = NULL;
	/* �ו�����v�f�̌^(�萔�l) */
	int8_t etype = RCAP_TETRAHEDRON;
	/* ���b�V���̐ߓ_�z��Ɍ����ߓ_�ԍ��̍ŏ��̒l */
	/* C ����ĂԂƂ��� 0 fortran ����Ăԏꍇ�� 1 */
	int32_t nodeOffset = 0;
	int32_t elementOffset = 0;
	/* �����ߓ_�̌� */
	size_t nodeCount = 5;
	/* �����v�f�̌� */
	size_t elementCount = 2;
	/* �ו���̗v�f�̌� */
	size_t refineElementCount = 0;
	/* �ו���̐ߓ_�̌� */
	size_t refineNodeCount = 0;
	/* �v�f�̍ו��Ɠ����ɍX�V����ߓ_�O���[�v */
	int32_t ng0[3] = {0,1,4};
	size_t ngCount = 3;
	int32_t* result_ng0 = NULL;
	/* �v�f�̍ו��Ɠ����ɍX�V����ʃO���[�v */
	/* �v�f�ԍ��Ɨv�f���ʔԍ��̏��Ɍ��݂ɕ��ׂ� */
	int32_t fg0[4] = {0,0,1,1};  /* [1,2,3] [1,4,3] */
	size_t fgCount = 2;
	int32_t* result_fg0 = NULL;

	/* ���[�v�̃J�E���^ */
	int32_t i = 0;
	int32_t j = 0;

	/* �ߓ_�ԍ��̃I�t�Z�b�g�l��^���� */
	rcapInitRefiner( nodeOffset, elementOffset );

	printf("REVOCAP_Refiner sample program : Multi Stage Refine\n");
	printf("----- Original Model -----\n");
	printf("---\n");
	/*
	 * globalId �ƍ��W�l�� Refiner �ɋ�����
	 * localIds �� NULL ����������� coords �� nodeOffset ���珇�Ԃɕ���ł�����̂Ɖ��߂���
	 */
	rcapSetNode64( nodeCount, coords, NULL, NULL );
	/* �ו��O�̐ߓ_�� */
	nodeCount = rcapGetNodeCount();
	assert( nodeCount == 5 );
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
		printf("      - [%d, TETRAHEDRON, %d, %d, %d, %d]\n", i+elementOffset,
			tetras[4*i], tetras[4*i+1], tetras[4*i+2], tetras[4*i+3] );
	}

	/* �ߓ_�O���[�v�̓o�^ */
	rcapAppendNodeGroup("innovate",ngCount,ng0);
	ngCount = rcapGetNodeGroupCount("innovate");
	assert( ngCount == 3 );
	printf("data:\n");
	printf("  - name: ng0\n");
	printf("    mode: NODEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %zu\n",ngCount);
	printf("    id:\n");
	for(i=0;(size_t)i<ngCount;++i){
		printf("    - %d\n", ng0[i]);
	}

	/* �ʃO���[�v�̓o�^ */
	rcapAppendFaceGroup("revolute",fgCount,fg0);
	fgCount = rcapGetFaceGroupCount("revolute");
	assert( fgCount == 2 );
	printf("  - name: revolute\n");
	printf("    mode: FACEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %zu\n",fgCount);
	printf("    id:\n");
	for(i=0;(size_t)i<fgCount;++i){
		printf("    - [%d, %d]\n", fg0[2*i], fg0[2*i+1]);
	}

	/*---------------------- REFINE STEP 1 -----------------------------------------*/

	/* �v�f�̍ו� */
	refineElementCount = rcapGetRefineElementCount( elementCount, etype );
	assert( refineElementCount == 16 );
	refineTetras = (int32_t*)calloc( 4*refineElementCount, sizeof(int32_t) );
	refineElementCount = rcapRefineElement( elementCount, etype, tetras, refineTetras );
	assert( refineElementCount == 16 );
	rcapCommit();

	printf("----- Refined Model 1 -----\n");
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

	/* �ו���̗v�f */
	printf("element:\n");
	printf("  - size: %zu\n", refineElementCount );
	printf("    connectivity:\n");
	for(i=0;(size_t)i<refineElementCount;++i){
		printf("      - [%d, TETRAHEDRON, %d, %d, %d, %d]\n", i+elementOffset,
			refineTetras[4*i], refineTetras[4*i+1], refineTetras[4*i+2], refineTetras[4*i+3] );
	}

	/* �ו���̐ߓ_�O���[�v�̍X�V */
	ngCount = rcapGetNodeGroupCount("innovate");
	assert( ngCount > 0 );
	result_ng0 = (int32_t*)calloc( ngCount, sizeof(int32_t) );
	rcapGetNodeGroup("innovate",ngCount,result_ng0);
	printf("data:\n");
	printf("  - name: innovate\n");
	printf("    mode: NODEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %d\n",ngCount);
	printf("    id:\n");
	for(j=0;(size_t)j<ngCount;++j){
		printf("    - %d\n", result_ng0[j]);
	}
	free( result_ng0 );
	result_ng0 = NULL;

	/* �ו���̖ʃO���[�v�̍X�V */
	fgCount = rcapGetFaceGroupCount("revolute");
	assert( fgCount > 0 );
	result_fg0 = (int32_t*)calloc( fgCount*2, sizeof(int32_t) );
	rcapGetFaceGroup("revolute",fgCount,result_fg0);
	assert( fgCount == 8 );
	printf("  - name: revolute\n");
	printf("    mode: FACEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %zu\n",fgCount);
	printf("    id:\n");
	for(i=0;(size_t)i<fgCount;++i){
		printf("    - [%d, %d]\n", result_fg0[2*i], result_fg0[2*i+1]);
	}
	free( result_fg0 );
	result_fg0 = NULL;

	free( resultCoords );
	resultCoords = NULL;

	/* ��Q�i�̍ו��̑O�ɃL���b�V�����N���A */
	rcapClearRefiner();

	/*---------------------- REFINE STEP 2 -----------------------------------------*/

	/* �v�f�̍ו� */
	elementCount = refineElementCount;
	refineElementCount = rcapGetRefineElementCount( elementCount, etype );
	refine2Tetras = (int32_t*)calloc( 4*refineElementCount, sizeof(int32_t) );
	refineElementCount = rcapRefineElement( elementCount, etype, refineTetras, refine2Tetras );
	rcapCommit();

	printf("----- Refined Model 2 -----\n");
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

	/* �ו���̗v�f */
	printf("element:\n");
	printf("  - size: %zu\n", refineElementCount );
	printf("    connectivity:\n");
	for(i=0;(size_t)i<refineElementCount;++i){
		printf("      - [%d, TETRAHEDRON, %d, %d, %d, %d]\n", i+elementOffset,
			refine2Tetras[4*i], refine2Tetras[4*i+1], refine2Tetras[4*i+2], refine2Tetras[4*i+3] );
	}

	/* �ו���̐ߓ_�O���[�v�̍X�V */
	ngCount = rcapGetNodeGroupCount("innovate");
	assert( ngCount > 0 );
	result_ng0 = (int32_t*)calloc( ngCount, sizeof(int32_t) );
	rcapGetNodeGroup("innovate",ngCount,result_ng0);
	printf("data:\n");
	printf("  - name: innovate\n");
	printf("    mode: NODEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %d\n",ngCount);
	printf("    id:\n");
	for(j=0;(size_t)j<ngCount;++j){
		printf("    - %d\n", result_ng0[j]);
	}
	free( result_ng0 );
	result_ng0 = NULL;

	/* �ו���̖ʃO���[�v�̍X�V */
	fgCount = rcapGetFaceGroupCount("revolute");
	assert( fgCount > 0 );
	result_fg0 = (int32_t*)calloc( fgCount*2, sizeof(int32_t) );
	rcapGetFaceGroup("revolute",fgCount,result_fg0);
	assert( fgCount == 8 );
	printf("  - name: revolute\n");
	printf("    mode: FACEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %zu\n",fgCount);
	printf("    id:\n");
	for(i=0;(size_t)i<fgCount;++i){
		printf("    - [%d, %d]\n", result_fg0[2*i], result_fg0[2*i+1]);
	}
	free( result_fg0 );
	result_fg0 = NULL;

	free( resultCoords );
	resultCoords = NULL;

	free( refineTetras );
	refineTetras = NULL;
	free( refine2Tetras );
	refine2Tetras = NULL;

	rcapTermRefiner();
	return 0;
}

#endif
