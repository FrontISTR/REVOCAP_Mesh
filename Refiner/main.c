/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_Refiner version 1.1                          #
# Sample Program By C Language                                         #
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
 * �T���v�����s�ၕ�e�X�g�p�v���O����
 *
 * ����͂����Ƃ��P���ȏꍇ�Ƃ��āA2�̎l�ʑ̂���Ȃ郂�f����
 * �ו�����T���v���ł���B
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
	 * �Ǐ��I�ɂ� 1 ���珇�ɐߓ_�ԍ����t�^����Ă���Ɖ��߂���
	 */
	float64_t coords[15] = {
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0
	};
	/*
	 * �ו���̐ߓ_�ԍ����i�[����
	 */
	float64_t* resultCoords = NULL;
	/* �ߓ_�ԍ��� GlobalId */
	int32_t globalIds[5] = { 100, 101, 102, 103, 104 };
	/* ���b�V���̐ߓ_�z��Ɍ����ߓ_�ԍ��̍ŏ��̒l
	 * �ʏ� C ����ĂԂƂ��� 0 fortran ����Ăԏꍇ�� 1
	 * �����ł� C ���痘�p���Ă��邪�A������ 1 ���炵���ꍇ���l����
	 */
	int32_t nodeOffset = 1;
	int32_t elementOffset = 1;
	/* �l�ʑ̂̐ߓ_�z��F���͂�2�� */
	int32_t tetras[8] = {
		1, 2, 3, 4,
		2, 3, 4, 5
	};
	/* �ו���̎l�ʑ̂̐ߓ_�z��F�o�͂�2*8=16��*/
	int32_t* refineTetras = NULL;
	/* �ו�����v�f�̌^(�萔�l) */
	int8_t etype = RCAP_TETRAHEDRON;
	/* �����ߓ_�̌� */
	size_t nodeCount = 5;
	/* �����v�f�̌� */
	size_t elementCount = 2;
	/* �ו���̗v�f�̌� */
	size_t refineElementCount = 0;
	/* �ו���̐ߓ_�̌� */
	size_t refineNodeCount = 0;
	/* �v�f�̍ו��Ɠ����ɍX�V����ߓ_�O���[�v */
	int32_t ng0[3] = {1,2,3};
	size_t ngCount = 3;
	int32_t* result_ng0 = NULL;
	/* �v�f�̍ו��Ɠ����ɍX�V����ʃO���[�v */
	/* �v�f�ԍ��Ɨv�f���ʔԍ��̏��Ɍ��݂ɕ��ׂ� */
	int32_t fg0[4] = {1,3,2,1};   /* [1,2,3] �� [2,5,4] */
	size_t fgCount = 2;
	int32_t* result_fg0 = NULL;
	/* �v�f�̍ו��Ɠ����ɍX�V����v�f�O���[�v */
	int32_t eg0[1] = {2};
	size_t egCount = 1;
	int32_t* result_eg0 = NULL;
	/* �֐��̖߂�l�i�[�p */
	int32_t seg[2] = {-1,-1};
	int8_t orgtype = RCAP_UNKNOWNTYPE;
	int32_t middle = -1;
	int32_t flag = 0;
	/* ���[�v�̃J�E���^ */
	int32_t i = 0;
	int32_t j = 0;

	assert( sizeof(int8_t) == 1 );
	assert( sizeof(uint8_t) == 1 );
	assert( sizeof(int16_t) == 2 );
	assert( sizeof(uint16_t) == 2 );
	assert( sizeof(int32_t) == 4 );
	assert( sizeof(uint32_t) == 4 );
	assert( sizeof(float32_t) == 4 );
	assert( sizeof(float64_t) == 8 );

	/* �ߓ_�ԍ��̃I�t�Z�b�g�l��^���� */
	rcapInitRefiner( nodeOffset, elementOffset );

	printf("----- Original Model -----\n");
	printf("---\n");
	/*
	 * globalId �ƍ��W�l�� Refiner �ɋ�����
	 * localIds �� NULL ����������� coords �� nodeOffset ���珇�Ԃɕ���ł�����̂Ɖ��߂���
	 */
	rcapSetNode64( nodeCount, coords, globalIds, NULL );
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
	for(i=0;i<elementCount;++i){
		printf("      - [%d, TETRAHEDRON, %d, %d, %d, %d]\n", i+elementOffset,
			tetras[4*i], tetras[4*i+1], tetras[4*i+2], tetras[4*i+3] );
	}
	/* �ߓ_�O���[�v�̓o�^ */
	rcapAppendNodeGroup("innovate",ngCount,ng0);
	ngCount = rcapGetNodeGroupCount("innovate");
	assert( ngCount == 3 );
	printf("data:\n");
	printf("  - name: innovate\n");
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
	/* �v�f�O���[�v�̓o�^ */
	rcapAppendElementGroup("eg",egCount,eg0);
	egCount = rcapGetElementGroupCount("eg");
	assert( egCount == 1 );
	printf("  - name: eg\n");
	printf("    mode: ELEMENTGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %zu\n",egCount);
	printf("    id:\n");
	for(i=0;(size_t)i<egCount;++i){
		printf("    - %d\n", eg0[i]);
	}

	/* �v�f�̍ו� */
	refineElementCount = rcapGetRefineElementCount( elementCount, etype );
	assert( refineElementCount == 16 );
	refineTetras = (int32_t*)calloc( 4*refineElementCount, sizeof(int32_t) );
	refineElementCount = rcapRefineElement( elementCount, etype, tetras, refineTetras );
	assert( refineElementCount == 16 );
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
	for(j=0;(size_t)j<refineNodeCount;++j){
		printf("  - [%d, %f, %f, %f]\n", j+nodeOffset, resultCoords[3*j], resultCoords[3*j+1], resultCoords[3*j+2] );
		orgtype = rcapGetOriginal( j+nodeOffset, seg );
		if( orgtype == RCAP_SEGMENT ){
			assert( coords[3*(seg[0]-nodeOffset)] + coords[3*(seg[1]-nodeOffset)] == 2.0 * resultCoords[3*j] );
			assert( coords[3*(seg[0]-nodeOffset)+1] + coords[3*(seg[1]-nodeOffset)+1] == 2.0 * resultCoords[3*j+1] );
			assert( coords[3*(seg[0]-nodeOffset)+2] + coords[3*(seg[1]-nodeOffset)+2] == 2.0 * resultCoords[3*j+2] );
		}
	}

	/* ���Ԑߓ_�̃`�F�b�N */
	for(j=0;(size_t)j<4*refineElementCount;++j){
		orgtype = rcapGetOriginal( refineTetras[j], seg );
		if( orgtype == RCAP_SEGMENT ){
			middle = rcapGetMiddle( orgtype, seg );
			/* ���Ԑߓ_ => ���̗��[ => ���Ԑߓ_ ���������̂��������� */
			assert( middle == refineTetras[j] );
		}else{
			/* ���Ԑߓ_�ł͂Ȃ� => �����炠��ߓ_ */
			assert( refineTetras[j] <= (int32_t)nodeCount );
		}
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

	/* �ו���̐ߓ_�O���[�v�̃`�F�b�N */
	/* ���̐ߓ_�O���[�v���܂� */
	for(i=0;i<3;++i){
		flag = 0;
		for(j=0;(size_t)j<ngCount;++j){
			if( result_ng0[j] == ng0[i] ){
				flag = 1;
			}
		}
		assert( flag == 1 );
	}
	/* �ו���̐ߓ_�O���[�v�̊m�F */
	for(j=0;(size_t)j<ngCount;++j){
		printf("    - %d\n", result_ng0[j]);
		orgtype = rcapGetOriginal( result_ng0[j], seg );
		if( orgtype == RCAP_SEGMENT ){
			/* ���Ԑߓ_ => ���̗��[ */
			flag = 0;
			for(i=0;i<3;++i){
				if( seg[0] == ng0[i] ){
					flag = 1;
				}
			}
			assert( flag == 1 );
			flag = 0;
			for(i=0;i<3;++i){
				if( seg[1] == ng0[i] ){
					flag = 1;
				}
			}
			assert( flag == 1 );
		}else{
			/* ���Ԑߓ_�ł͂Ȃ� */
			flag = 0;
			for(i=0;i<3;++i){
				if( result_ng0[j] == ng0[i] ){
					flag = 1;
				}
			}
			assert( flag == 1 );
		}
	}
	free( result_ng0 );

	/* �ו���̖ʃO���[�v�̍X�V */
	fgCount = rcapGetFaceGroupCount("revolute");
	printf("  - name: revolute\n");
	printf("    mode: FACEGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %zu\n",fgCount);
	printf("    id:\n");
	if( fgCount > 0 ){
		result_fg0 = (int32_t*)calloc( fgCount*2, sizeof(int32_t) );
		rcapGetFaceGroup("revolute",fgCount,result_fg0);
		assert( fgCount == 8 );
		for(i=0;(size_t)i<fgCount;++i){
			printf("    - [%d, %d]\n", result_fg0[2*i], result_fg0[2*i+1]);
		}
		free( result_fg0 );
	}

	/* �ו���̗v�f�O���[�v�̍X�V */
	egCount = rcapGetElementGroupCount("eg");
	printf("  - name: eg\n");
	printf("    mode: ELEMENTGROUP\n");
	printf("    vtype: NONE\n");
	printf("    size: %zu\n",egCount);
	printf("    id:\n");
	if( egCount > 0 ){
		result_eg0 = (int32_t*)calloc( egCount, sizeof(int32_t) );
		rcapGetElementGroup("eg",egCount,result_eg0);
		assert( egCount == 8 );
		flag = 1;
		for(i=0;i<egCount;++i){
			printf("    - %d\n", result_eg0[i]);
			if( result_eg0[i] != 9+i ){
				flag = 0;
			}
		}
		assert( flag == 1 );
		free( result_eg0 );
	}

	free( resultCoords );
	free( refineTetras );

	rcapTermRefiner();
	return 0;
}

#endif
