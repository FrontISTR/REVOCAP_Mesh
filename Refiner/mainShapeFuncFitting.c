/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_Refiner version 1.1                          #
# Sample Program ShapeFuncFitting                                      #
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
 * �`��֐��ɂ��l�ʑ�2���v�f�̍ו��e�X�g�p�v���O����
 *
 * Sample Program for refinement by shape functions.
 * This model consists of one element of second degree tetrahedron.
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
	/* �����ߓ_ */
	size_t nodeCount = 10;
	float64_t coords[30] = {
		0.0, 0.0, 0.0, // 0
		1.0, 0.0, 0.0, // 1
		0.0, 1.0, 0.0, // 2
		0.0, 0.0, 1.0, // 3
		0.5, 0.5,-0.1, // 4
		0.0, 0.5,-0.1, // 5
		0.5, 0.0,-0.1, // 6
		0.0, 0.0, 0.5, // 7
		0.5, 0.0, 0.5, // 8
		0.0, 0.5, 0.5  // 9
	};
	/* �ו���̐ߓ_ */
	size_t refineNodeCount = 0;
	float64_t* resultCoords = NULL;
	/* �ו��O�̗v�f */
	int8_t etype = RCAP_TETRAHEDRON2;
	size_t elementCount = 1;
	int32_t tetras[10] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	};
	/* �ו���̗v�f */
	size_t refineElementCount = 0;
	int32_t* refineTetras = NULL;

	/* �J�E���^ */
	int32_t i;

	/* �������F�ߓ_�ԍ��A�v�f�ԍ��̃I�t�Z�b�g�l��^���� */
	rcapInitRefiner( nodeOffset, elementOffset );

	/* �`��֐��ɂ�钆�Ԑߓ_��L���ɂ��� */
	rcapSetSecondFitting(1);

	printf("REVOCAP_Refiner sample program : Shape Function Refine\n");
	printf("----- Original Model -----\n");
	printf("---\n");
	/* ���W�l�� Refiner �ɗ^���� */
	rcapSetNode64( nodeCount, coords, NULL, NULL );
	/* �ו��O�̐ߓ_�� */
	nodeCount = rcapGetNodeCount();
	assert( nodeCount == 10 );
	printf("node:\n");
	printf("  size: %zu\n", nodeCount );
	printf("  coordinate:\n");
	for(i=0;(size_t)i<nodeCount;++i){
		printf("  - [%d, %f, %f, %f]\n", i+nodeOffset, coords[3*i], coords[3*i+1], coords[3*i+2] );
	}

	/* �ו��O�̗v�f�� */
	assert( elementCount == 1 );
	printf("element:\n");
	printf("  - size: %zu\n", elementCount );
	printf("    connectivity:\n");
	for(i=0;(size_t)i<elementCount;++i){
		printf("      - [%d, TETRAHEDRON2, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d]\n", i+elementOffset,
			tetras[10*i],   tetras[10*i+1], tetras[10*i+2], tetras[10*i+3], tetras[10*i+4],
			tetras[10*i+5], tetras[10*i+6], tetras[10*i+7], tetras[10*i+8], tetras[10*i+9]
			);
	}

	printf("----- Refined Model -----\n");
	printf("---\n");

	/* �v�f�̍ו� */
	refineElementCount = rcapGetRefineElementCount( elementCount, etype );
	refineTetras = (int32_t*)calloc( 10*refineElementCount, sizeof(int32_t) );
	elementCount = rcapRefineElement( elementCount, etype, tetras, refineTetras);
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
		printf("      - [%d, TETRAHEDRON2, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d]\n", i+elementOffset,
			refineTetras[10*i],   refineTetras[10*i+1], refineTetras[10*i+2], refineTetras[10*i+3], refineTetras[10*i+4],
			refineTetras[10*i+5], refineTetras[10*i+6], refineTetras[10*i+7], refineTetras[10*i+8], refineTetras[10*i+9]
			);
	}
	free( refineTetras );

	rcapTermRefiner();
	return 0;
}

#endif
