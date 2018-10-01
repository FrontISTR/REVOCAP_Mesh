/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_Refiner version 1.1                          #
# Program Name : rcapRefiner                                           #
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
#ifndef REVOCAP_REFINER_HEADER
#define REVOCAP_REFINER_HEADER

/* �v���|�X�g�ł���`���Ă��邽�߁A�d�����Ȃ��悤�� */
#ifndef REVOCAP_SIZE_DEF
#define REVOCAP_SIZE_DEF

/* for size_t */
#include <stddef.h>

#ifdef _MSC_VER
 #if (_MSC_VER >= 1300 )
  #ifndef HAVE_INT8_T
   typedef signed __int8 int8_t;
  #endif
  #ifndef HAVE_UINT8_T
   typedef unsigned __int8 uint8_t;
  #endif
  #ifndef HAVE_INT16_T
   typedef signed __int16 int16_t;
  #endif
  #ifndef HAVE_UINT16_T
   typedef unsigned __int16 uint16_t;
  #endif
  #ifndef HAVE_INT32_T
   typedef signed __int32 int32_t;
  #endif
  #ifndef HAVE_UINT32_T
   typedef unsigned __int32 uint32_t;
  #endif
  #ifndef HAVE_INT64_T
   typedef signed __int64 int64_t;
  #endif
  #ifndef HAVE_UINT64_T
   typedef unsigned __int64 uint64_t;
  #endif
 #else
  #ifndef HAVE_INT8_T
   typedef signed char int8_t;
  #endif
  #ifndef HAVE_UINT8_T
   typedef unsigned char uint8_t;
  #endif
  #ifndef HAVE_INT16_T
   typedef short int16_t;
  #endif
  #ifndef HAVE_UINT16_T
   typedef unsigned short uint16_t;
  #endif
  #ifndef HAVE_INT32_T
   typedef long int32_t;
  #endif
  #ifndef HAVE_UINT32_T
   typedef unsigned long uint32_t;
  #endif
  #ifndef HAVE_INT64_T
   typedef long long int64_t;
  #endif
  #ifndef HAVE_UINT64_T
   typedef unsigned long long uint64_t;
  #endif
 #endif
#else
/* C99 ���瓱������Ă��� */
#include <stdint.h>
#endif

typedef float float32_t;  /* REAL*4 */
typedef double float64_t; /* REAL*8 */

#endif

/* �v�f�̌^��� int8_t */
#define RCAP_UNKNOWNTYPE  -1
#define RCAP_SEGMENT      0
#define RCAP_SEGMENT2     1
#define RCAP_TRIANGLE     2
#define RCAP_TRIANGLE2    3
#define RCAP_QUAD         4
#define RCAP_QUAD2        5
#define RCAP_TETRAHEDRON  6
#define RCAP_TETRAHEDRON2 7
#define RCAP_WEDGE        8
#define RCAP_WEDGE2       9
#define RCAP_PYRAMID      10
#define RCAP_PYRAMID2     11
#define RCAP_HEXAHEDRON   12
#define RCAP_HEXAHEDRON2  13
/* 14 �͗v�f�Ɠ_�̋��E�Ƃ��ē����ŗ\�� */
#define RCAP_VERTEX       15

const int RCAP_TETRAFACES[12] =
{
	1, 2, 3,  /* 0 �Ԗڂ̖� */
	0, 3, 2,  /* 1 �Ԗڂ̖� */
	0, 1, 3,  /* 2 �Ԗڂ̖� */
	0, 2, 1   /* 3 �Ԗڂ̖� */
};

const int RCAP_HEXAFACES[24] =
{
	3, 2, 1, 0,  /* 0 �Ԗڂ̖� */
	4, 5, 6, 7,  /* 1 �Ԗڂ̖� */
	1, 5, 4, 0,  /* 2 �Ԗڂ̖� */
	1, 2, 6, 5,  /* 3 �Ԗڂ̖� */
	3, 7, 6, 2,  /* 4 �Ԗڂ̖� */
	4, 7, 3, 0   /* 5 �Ԗڂ̖� */
};

const int RCAP_PYRAFACES[20] =
{
	0, 1, 2,-1,  /* 0 �Ԗڂ̖� */
	0, 2, 3,-1,  /* 1 �Ԗڂ̖� */
	0, 3, 4,-1,  /* 2 �Ԗڂ̖� */
	0, 4, 1,-1,  /* 3 �Ԗڂ̖� */
	4, 3, 2, 1   /* 4 �Ԗڂ̖� */
};

const int RCAP_WEDGEFACES[20] =
{
	0, 2, 1,-1,  /* 0 �Ԗڂ̖� */
	3, 4, 5,-1,  /* 1 �Ԗڂ̖� */
	0, 1, 4, 3,  /* 2 �Ԗڂ̖� */
	1, 2, 5, 4,  /* 3 �Ԗڂ̖� */
	2, 0, 3, 5   /* 4 �Ԗڂ̖� */
};


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Version �������W���o�͂ɏo�͂���B
 */
void rcapGetVersion( void );

/**
 * @brief Refiner �����������A�ߓ_�ԍ��Ɨv�f�ԍ��̃I�t�Z�b�g�l��^����B
 * @param[in] nodeOffset �Ăяo�����̐ߓ_�ԍ��̏����l�iC����Ȃ�0�AFortran����Ȃ�1�Ȃǁj
 * @param[in] elementOffset �Ăяo�����̐ߓ_�ԍ��̏����l�iC����Ȃ�0�AFortran����Ȃ�1�Ȃǁj
 * @note �����̃I�t�Z�b�g�l�����������l��ߓ_�ԍ���v�f�ԍ��ŗ^�����ꍇ�͖����ƂȂ�B
 */
void rcapInitRefiner( int32_t nodeOffset, int32_t elementOffset );

/**
 * @brief Refiner �������ŕێ����Ă��钆�Ԑߓ_�f�[�^�̃L���b�V�����N���A����B
 * @note �ו��𕡐���s���ꍇ�ɁA��U�����ŕێ����Ă���f�[�^���N���A���邽�߂Ɏg���B
 * �Ⴆ�Α�P�i�K�̍ו����I����āA��Q�i�K�̍ו����n�߂�O�ɌĂԁB
 * �����ŃN���A�����f�[�^�͒��Ԑߓ_�̃f�[�^�݂̂ł���A�ߓ_�̍��W�l�A
 * �ו����ɍX�V����ߓ_�O���[�v�̏�񓙂̓N���A����Ȃ��̂Œ��ӂ���B
 */
void rcapClearRefiner( void );

/**
 * @brief Refiner �̏I���������s���B������Ăяo������͂��ׂĂ̏����������ɂȂ�B
 * @note Refiner �ɓo�^�����ߓ_�A�v�f�A���E�����Ȃǂ̃��������������O��
 * Refiner �̏I���������s���Ă��������BRefiner �̏I���������s���O��
 * ���������������ƁA�s���ȃA�h���X�ɃA�N�Z�X����\��������܂��B
 */
void rcapTermRefiner( void );

/**
 * @brief �`��␳�ɗp���� CAD �t�@�C�����w�肷��B\n
 * �ו��O�̃��b�V���͂��� CAD �t�@�C�����琶������Ă�����̂Ƃ���B
 * ���̊֐����Ă�� CAD �t�@�C�����w�肵�Ȃ������ꍇ�́A�`��␳�͍s��Ȃ��B
 * ���b�V���������̐ߓ_�� globalID �� CAD �̌`��f�[�^�̑Ή����^�����Ă���Ƃ���B
 * �̈敪����̃��b�V���ɑ΂��čו����s���ꍇ�́AsetPartitionFilename �Ȃǂ�
 * globalID �� localID �̑Ή��t����^����K�v������B
 *
 * @remark ���̊֐��ŗ^���� CAD �t�@�C���ɂ́A�`��f�[�^�����ł͂Ȃ��A
 * �������b�V���̐ߓ_�� CAD �̖ʂ̊Ԃ̑Ή��֌W���L�q����Ă��邽�߁A
 * �������b�V�����قȂ�ꍇ�ɂ́ACAD �t�@�C�����u�������Ȃ���΂Ȃ�Ȃ��B
 * CAD �t�@�C���̐ߓ_�ԍ��� 0 �n�܂�̔ԍ���p���Ă���B
 * rcapSetNode32 rcapSetNode64 �܂��� rcapSetPartitionFilename �ŗ^����ꍇ�ɂ�
 * rcapInitRefiner �ŗ^���� nodeOffset ���������̂Œ��ӂ���B
 *
 * @remark �ו��O�̎��O�␳���s���ꍇ�� rcapSetNode32/64 �Őߓ_���W��^����O��
 * ���̊֐����ĂԕK�v������܂��B
 * ���̊֐����ɌĂ񂾏ꍇ���ו����̌`��K���͍s���܂����A�ו��O�̎��O�␳�����܂���B
 *
 * @param[in] filename �t�@�C����
 */
void rcapSetCADFilename( const char* filename );
/**
 * @brief �`��␳�ɗp���� CAD �t�@�C���Ǝ��O�␳�����o�͂���B
 * globalID �� localID ��^������ŏo�͂���␳���́AlocalID �ƋȖʂƂ̑Ή��ɂȂ�B
 *
 * @remark ���̃t�@�C���ŏo�͂����ߓ_�ԍ��� 0 �n�܂�ł���B
 *
 * @remark ���̊֐��̓f�o�b�O�p�r�܂��́A�̈敪��������ɋǏ��I�ȍו����s���ꍇ�ɗp����B
 *
 * @param[in] filename �t�@�C����
 */
void rcapWriteFittingFile( const char* filename );

/**
 * @brief ���Ԑߓ_�̐�����2���v�f�̌`��֐����g�����ǂ�����ݒ肷��B
 * @param[in] flag ���̎��ɗL���ɂ��A��̎��ɖ����ɂ��܂�
 *
 */
void rcapSetSecondFitting( int32_t flag );

/**
 * @brief ���Ԑߓ_�� Laplacian Smoothing ���邩�ǂ�����ݒ肷��B
 * @param[in] flag ���̎��ɗL���ɂ��A��̎��ɖ����ɂ��܂�
 *
 */
void rcapSetSmoothing( int32_t flag );

/**
 * @brief �ߓ_�� globalID �� localID �̑Ή��֌W���L�q�����t�@�C�����w�肷��B\n
 * �w�肵�Ȃ��ꍇ�́AglobalID �� localID �͋�ʂ��Ȃ��B
 * �t�@�C���ł͂Ȃ��A�ߓ_���W��o�^����Ƃ��� globalID �� localID �̊֌W��^���邱�Ƃ��ł���B
 * @param[in] filename �t�@�C����
 * @note �t�@�C���t�H�[�}�b�g�͊v�V�v���W�F�N�g�Ń\���o�ƃJ�v���̊Ԃŗp�������̂ɏ�����B
 */
void rcapSetPartitionFilename( const char* filename );

/**
 * @brief �ߓ_���W�� Refiner �ɗ^����
 * @param[in] num �^����ߓ_�̌�
 * @param[in] coords �ߓ_�̍��W�Adouble �^�̔z��� (x,y,z) �̏��Ԃ� 3*num ���ׂ�����
 * @param[in] localIds �̈敪�������̈���̋Ǐ��ߓ_ID �̔z��
 * @param[in] globalIds ���ߓ_ID �̔z��
 *
 * @note �����l���珇�� num ����ł���ꍇ�� localIds = NULL �ł悢�B
 * Global ID ���t�@�C������^����ꍇ�� globalIds = NULL �ɂ���B
 * setPartitionFilename �ŗ̈敪���̑��ߓ_�ƋǏ��ߓ_�̑Ή����^�����Ă��鎞�ɂ́A
 * globalIds ��^���Ȃ��Ă��悢�B���̊֐��ŗ^�����ꍇ�ɂ́AsetPartitionFilename �ŗ^�����Ή����㏑������B
 * setPartitionFilename �ŗ^���Ȃ��ꍇ�ɁA���̊֐��ł� globalIds ��^���Ȃ��ꍇ�́AlocalId �����̂܂܎g���B
 *
 * @li setNode64( num, coords, NULL, NULL ) => global �� local �͓����� nodeOffset ���珇�� num ��
 * @li setNode64( num, coords, globalIds, NULL ) => local �� nodeOffset ���珇�� num �Aglobal �͈����ŗ^����
 * @li setNode64( num, coords, NULL, localIds ) => global �� local �͓����� local �͗^����
 * @li setNode64( num, coords, globalIds, localIds ) => global �� local �����ꂼ��^����
 *
 * @note CAD�t�@�C����^�����Ɍ`��␳�����Ȃ��ꍇ�́A�Ǐ��ߓ_�ԍ��Ƒ��ߓ_�ԍ��̑Ή��͕K�v�Ȃ��B
 * @note Fortran ����Ăԏꍇ�ɂ� NULL �A�h���X�̑���ɍŏ��̒l�� nodeOffset �����������z���^����B
 */
void rcapSetNode64( size_t num, float64_t* coords, int32_t* globalIds, int32_t* localIds );

/**
 * @brief �ߓ_���W�� Refiner �ɗ^����
 * @param[in] num �^����ߓ_�̌�
 * @param[in] coords �ߓ_�̍��W�Afloat �^�̔z��� (x,y,z) �̏��Ԃ� 3*num ���ׂ�����
 * @param[in] localIds �̈敪�������̈���̋Ǐ��ߓ_ID �̔z��
 * @param[in] globalIds ���ߓ_ID �̔z��
 *
 * @note �ڍׂ� rcapSetNode64 �ɏ�����
 */
void rcapSetNode32( size_t num, float32_t* coords, int32_t* globalIds, int32_t* localIds );

/**
 * @brief ���� Refiner ���ێ����Ă���ߓ_�̌���Ԃ��B�ו������瑝����B
 * @return �ߓ_�̌�
 */
size_t rcapGetNodeCount( void );

/**
 * @brief Refiner ���Ǘ����Ă���ߓ_���W���擾����
 * @param[in] num �ߓ_�̌�
 * @param[in] localIds �̈敪�������̈���̋Ǐ��ߓ_ID �̔z��
 * @param[out] coords �ߓ_�̍��W�A(x,y,z) �̏��Ԃ� 3*num ���ׂ�����
 *
 * @note RefineElement �̖߂�l�� num �Ƃ��āAresultNodeArray �� localIds �ɓ�����
 * coords �ō��W�l���擾�ł���B\n
 * localIds = {1,5,8} �̏ꍇ
 * coords = {x1,y1,z1,x5,y5,z5,x8,y8,z8} �ƂȂ�B\n
 * localIds �̐ߓ_�ԍ��͎����I�� nodeOffset �ł��炵�� Refiner �ɖ₢���킹��
 */
void rcapGetNode64( size_t num, int32_t* localIds, float64_t* coords );
/**
 * @brief Refiner ���Ǘ����Ă���ߓ_���W���擾����
 * @param[in] num �ߓ_�̌�
 * @param[in] localIds �̈敪�������̈���̋Ǐ��ߓ_ID �̔z��
 * @param[out] coords �ߓ_�̍��W�A(x,y,z) �̏��Ԃ� 3*num ���ׂ�����
 *
 * @note rcapGetNode64 �� 32bit ��
 */
void rcapGetNode32( size_t num, int32_t* localIds, float32_t* coords );

/**
 * @brief initId ����A������ num �̐ߓ_���W���擾����
 * @param[in] num �ߓ_�̌�
 * @param[in] initId �擾����擪�̐ߓ_�ԍ�
 * @param[out] coords �ߓ_�̍��W�A(x,y,z) �̏��Ԃ� 3*num ���ׂ�����
 * @note coords �� initId �Ԗڂ���������
 */
void rcapGetNodeSeq64( size_t num, size_t initId, float64_t* coords );
/**
 * @brief rcapGetNodeSeq64 �� 32bit ��
 */
void rcapGetNodeSeq32( size_t num, size_t initId, float32_t* coords );

/**
 * @brief �v�f�����ꂼ��ӂ̂Q�������čו�����
 * @param[in] num �v�f�̌�
 * @param[in] etype ���͗v�f�̌^
 * @param[in] nodeArray ���͗v�f�̐ߓ_�z��ietype�^�̗v�f��num���ׂ����́j
 * @param[out] resultNodeArray �ו����ʂ̗v�f�̐ߓ_�z��
 * @return �ו��������ʂ̗v�f�̌�
 *
 * @note nodeArray �̓��[�J���ߓ_ID�ŋL�q�������̂�^���Ă��������B
 *
 * @note resultNodeArray �� NULL �܂��� -1 �Ƃ��ČĂяo���ƁA�ו������ꍇ�̌��������v�Z���ĕԂ��B
 * �Ⴆ�΂S�ʑ̂Ȃ�Γ��͗v�f�̌��̂W�{��Ԃ��B\n
 *
 * @code
 * etype = TETRAHEDRON;
 * num = 5;
 * nodeArray = new int32_t[4*5];
 * resultNodeArray = new int32_t[4*5*8];
 *
 * refineElement( &num, &etype, nodeArray, resultNodeArray )
 * @endcode
 *
 * @note �����̗v�f�̌^���������Ă���ꍇ�́A�����^�̗v�f���܂Ƃ߂Ă��̊֐���v�f�̌^���Ƃɕ�����ĂԂ��A
 * �ߓ_�z��ɂǂ̌^�̗v�f�����Ԃɓ����Ă��邩�̔z����쐬���� refineElementMulti �֐����ĂԁB\n
 *
 * @code
 * etype = TETRAHEDRON;
 * num = 5;
 * nodeArrayTetra = new int32_t[4*5];
 * resultNodeArrayTetra = new int32_t[4*5*8];
 *
 * refineElement( &num, &etype, nodeArrayTetra, resultNodeArrayTetra )
 * etype = HEXAHEDRON;
 * num = 5;
 * nodeArrayHex = new int32_t[8*5];
 * resultNodeArrayHex = new int32_t[8*5*8];
 *
 * refineElement( &num, &etype, nodeArrayHex, resultNodeArrayHex )
 * @endcode
 *
 * @note �v�f���ו�������ɁA���̗v�f�̖ʂ�ӂ��O�p�`�v�f������v�f�Ƃ݂Ȃ��Ă��̊֐����Ăяo���ƁA
 * �v�f���ו��������ʂ�ʂ�ӂɐ����������ʂ�Ԃ��B�t�ɗv�f�̖ʂ�ӂ̍ו����Ăяo������ŁA
 * �v�f�̍ו����s���ƁA��ɍו������ʂ�ӂɂ��Ă͂��̐ߓ_�ԍ����g���B
 *
 * @note rcapClearRefiner ���ĂԂ܂ł� nodeArray resultNodeArray ��������Ȃ��ł��������B
 *
 * @note �l�ʑ̗v�f�̍ו���̂W�̗v�f�̌���鏇��
 *
 *  - (1)�l�ʑ̂�1�Ԗڂ̐ߓ_��������v�f
 *  - (2)�l�ʑ̂�2�Ԗڂ̐ߓ_��������v�f
 *  - (3)�l�ʑ̂�3�Ԗڂ̐ߓ_��������v�f
 *  - (4)�l�ʑ̂�4�Ԗڂ̐ߓ_��������v�f
 *  - (5)�l�ʑ̂�1�Ԗڂ̖ʂ�������v�f
 *  - (6)�l�ʑ̂�2�Ԗڂ̖ʂ�������v�f
 *  - (7)�l�ʑ̂�3�Ԗڂ̖ʂ�������v�f
 *  - (8)�l�ʑ̂�4�Ԗڂ̖ʂ�������v�f
 *
 * �O����4�̖ʔԍ��͍ו��O�̗v�f�Ɠ�������
 * �㔼��4�̖ʔԍ���0�Ԗڂ��ו��O�̗v�f�̊O���̌���
 *
 * @note �Z�ʑ̗v�f�̍ו���̂W�̗v�f�̌���鏇��
 *  - (1)�Z�ʑ̂�1�Ԗڂ̐ߓ_��������v�f
 *  - (2)�Z�ʑ̂�2�Ԗڂ̐ߓ_��������v�f
 *  - (3)�Z�ʑ̂�3�Ԗڂ̐ߓ_��������v�f
 *  - (4)�Z�ʑ̂�4�Ԗڂ̐ߓ_��������v�f
 *  - (5)�Z�ʑ̂�5�Ԗڂ̐ߓ_��������v�f
 *  - (6)�Z�ʑ̂�6�Ԗڂ̐ߓ_��������v�f
 *  - (7)�Z�ʑ̂�7�Ԗڂ̐ߓ_��������v�f
 *  - (8)�Z�ʑ̂�8�Ԗڂ̐ߓ_��������v�f
 *
 * �ʔԍ��͍ו��O�̗v�f�Ɠ�������
 *
 * @note �O�p���v�f�̍ו����8�̗v�f�̌���鏇��
 *
 *  - (1)�O�p����1�Ԗڂ̐ߓ_��������v�f
 *  - (2)�O�p����2�Ԗڂ̐ߓ_��������v�f
 *  - (3)�O�p����3�Ԗڂ̐ߓ_��������v�f
 *  - (4)�O�p����4�Ԗڂ̐ߓ_��������v�f
 *  - (5)�O�p����5�Ԗڂ̐ߓ_��������v�f
 *  - (6)�O�p����6�Ԗڂ̐ߓ_��������v�f
 *  - (7)�O�p����0,1,2�Ԗڂ̐ߓ_�ɂ��O�p�`�ɐڂ���v�f
 *  - (8)�O�p����3,4,5�Ԗڂ̐ߓ_�ɂ��O�p�`�ɐڂ���v�f
 *
 * �͂��߂�6�̖ʔԍ��͍ו��O�̗v�f�Ɠ�������
 * �Ō��2�̖ʔԍ���0�Ԗڂ��ו��O�̗v�f�̊O���̌���
 *
 * @note �l�p���v�f�̍ו����10�̗v�f�̌���鏇��
 *
 *  - (1)�l�p����1�Ԗڂ̐ߓ_��������v�f
 *  - (2)�l�p����2�Ԗڂ̐ߓ_��������v�f
 *  - (3)�l�p����3�Ԗڂ̐ߓ_��������v�f
 *  - (4)�l�p����4�Ԗڂ̐ߓ_��������v�f
 *  - (5)�l�p����5�Ԗڂ̐ߓ_��������v�f
 *  - (6)�l�p����0,1,2�Ԗڂ̐ߓ_�ɂ��O�p�`�ɐڂ���l�ʑ̗v�f
 *  - (7)�l�p����0,2,3�Ԗڂ̐ߓ_�ɂ��O�p�`�ɐڂ���l�ʑ̗v�f
 *  - (8)�l�p����0,3,4�Ԗڂ̐ߓ_�ɂ��O�p�`�ɐڂ���l�ʑ̗v�f
 *  - (9)�l�p����0,4,1�Ԗڂ̐ߓ_�ɂ��O�p�`�ɐڂ���l�ʑ̗v�f
 *  - (10)�l�p���̒��ɖ��ߍ��܂�Ă���v�f
 *
 * �͂��߂�5�̖ʔԍ��͍ו��O�̗v�f�Ɠ�������
 * ����4�̖ʔԍ��͍ו��O�̎l�p���̖ʔԍ��ƁA�l�ʑ̗v�f�̊O�Ɍ����Ă���ʔԍ�����v�������
 * �Ō��1�͑S�Ă̖ʂ����ߍ��܂�Ă���
 */
size_t rcapRefineElement( size_t num, int8_t etype, int32_t* nodeArray, int32_t* resultNodeArray );
/**
 * @brief �ו������v�f�̌����v�Z����i���ۂɂ͍ו��͂��Ȃ��j
 * @param[in] num �v�f�̌�
 * @param[in] etype ���͗v�f�̌^
 * @return �ו��������ʂ̗v�f�̌�
 */
size_t rcapGetRefineElementCount( size_t num, int8_t etype );

/**
 * @brief �����̎�ނ̌^�����݂��Ă��郂�f������x�ɍו�����
 * @param[in] num �v�f�̌�
 * @param[in] etypeArray ���͗v�f�̌^�̔z��
 * @param[in] nodeArray ���͗v�f�̐ߓ_�z��
 * @param[in,out] refinedNum �ו����ʂ̗v�f�̌�
 * @param[out] resultEtypeArray �ו����ʂ̗v�f�̌^�̔z��
 * @param[out] resultNodeArray �ו����ʂ̗v�f�̐ߓ_�z��
 * @return �ו��������ʂ��i�[����̂�[�K�v��/�g�p����]�ߓ_�z��̑傫��
 *
 * @note nodeArray �̓��[�J���ߓ_ID�ŋL�q�������̂�^���Ă��������B
 * @note rcapClearRefiner ���ĂԂ܂ł� etypeArray, nodeArray, resultETypeArray, resultNodeArray ��������Ȃ��ł��������B
 *
 * @note ���̊֐��͎��ۂɍו�����@�\�ƁA�ߓ_�z��̑傫���𒲂ׂ邾���̋@�\�Ƃ����˂Ă��܂��B
 * resultNodeArray �� NULL �܂��� -1 �Ƃ��ČĂяo���ƁA
 * �ו��������ʂ��i�[����̂ɕK�v�Ȑߓ_�z��̑傫����Ԃ��A
 * refinedNum �ɍו���̗v�f�̌��������܂��B
 * resultEtypeArray �͎��ۂɍו�����@�\�ƁA�ߓ_�z��̑傫���𒲂ׂ邾���̋@�\��
 * �؂�ւ��̂��߂̏��Ƃ��Ă͗��p���܂���B
 * ���̃��W���[���𗘗p����ꍇ�ɂ͂��̖߂�l�̑傫����
 * �z�� resultNodeArray ���Ăяo��������������Ɋm�ۂ��A
 * �ו����邽�߂ɂ��̊֐����ēx�Ăяo���K�v������܂��B
 *
 * @note ���ۂɍו����s���Ƃ��ɂ́A�z�� resultEtypeArray �� resultNodeArray ����������Ɋm�ۂ��A
 * ���ꂼ�ꏉ�������Ă��炱�̊֐����Ăяo���܂��B
 * ���̂Ƃ��ArefinedNum �� resultEtypeArray / resultNodeArray ��
 * �i�[�\�ȗv�f�̌���^���܂��B�^����ꂽ refinedNum ����
 * �ו����ꂽ�v�f�̌����傫���ꍇ�́A�����������͊i�[���܂���B
 * �߂�l�͍ו��̌��ʂ��i�[����̂Ɏg�p�����ߓ_�z��̑傫����Ԃ��܂��B
 *
 * @note �ו���̗v�f�̌^�������̏ꍇ�́AresultNodeArray ����������Ɋm�ۂ��āA
 * resultEtypeArray �� NULL ��^���Ă��ו��̎��s�͉\�ł��B
 *
 * @note
 * result = rcapRefineElementMulti(num,etypeArray,nodeArray,[refinedNum],NULL,NULL)
 * result = rcapRefineElementMulti(num,etypeArray,nodeArray,[refinedNum],[-1],[-1])
 * refinedNum �ɍו��œ�����v�f�̌��������Aresult �ɍו����ʂ��i�[����̂ɕK�v�Ȑߓ_�z��̑傫����Ԃ�
 * ���ۂɂ͍ו��͍s��Ȃ��B
 *
 * @note
 * result = rcapRefineElementMulti(num,etypeArray,nodeArray,[refinedNum],resultEtypeArray,resultNodeArray)
 * ���ۂɍו����s���A�ו����ʂ� resultEtypeArray �� resultNodeArray �Ɋi�[����B
 * refinedNum �ŗ^����ꂽ�傫���̗v�f���i�[����z�񂪊m�ۂ���Ă��邱�Ƃ����肵�Ă���B
 *
 * @note �v�f�̌^���Ƃɐߓ_�z����܂Ƃ߂�� rcapRefineElement �œ����̂��Ƃ����s�ł��܂��B
 *
 * @remark *** �߂�l�̎d�l�� 2010/2/9 �o�[�W��������ύX���Ă��܂� ***
 */
size_t rcapRefineElementMulti( size_t num, int8_t* etypeArray, int32_t* nodeArray, size_t* refinedNum, int8_t* resultEtypeArray, int32_t* resultNodeArray );
/**
 * @brief �����̎�ނ̌^�����݂��Ă��郂�f�����ו������Ƃ��̗v�f�̌����v�Z����i���ۂɂ͍ו����Ȃ��j
 * @param[in] num �v�f�̌�
 * @param[in] etypeArray ���͗v�f�̌^�̔z��
 * @param[out] refinedNum �ו����ʂ̗v�f�̌�
 * @return �ו��������ʂ��i�[����̂ɕK�v�Ȑߓ_�z��̑傫��
 */
size_t rcapGetRefineElementMultiCount( size_t num, int8_t* etypeArray, size_t* refinedNum );

/**
 * @brief
 * rcapRefineElement �ɂ��ו����ꂽ�f�[�^�i�ߓ_�O���[�v�A�v�f�O���[�v�A�ʃO���[�v�j���R�~�b�g����B
 * ���Ȃ킿�A�ȉ��� rcapGet[Node|Element|Face]Group[Count] ���\�b�h�̑Ώۂ��ו��O�̃f�[�^����
 * �ו���̃f�[�^�ɕς���B�ו��O�̃f�[�^�͍폜�����B
 * ���̊֐������s��� rcapRefineElement ���ēx���s�����ꍇ�A�X�V�����f�[�^��
 * �ו���̃f�[�^�ɂȂ�B
 * �܂��ArcapRefineElement �ōו���̗v�f�ɕt�^�����v�f�ԍ��� elementOffset �l�Ƀ��Z�b�g�����B
 * @remark
 * rcapRefineElement �𕡐���Ăԏꍇ�́A������Ă񂾌�Ɉ�x���� rcapCommit �����s����B
 * �Q�i�K�̍ו����s���ꍇ�́A�P�i�K�ڂ̍ו� rcapRefineElement ���Ă񂾌�A
 * �Q�i�K�ڂ� rcapRefineElement ���ĂԑO�Ɏ��s����B
 * @remark
 * rcapAppendBoundaryNodeGroup�ArcapAppendBoundaryNodeVariableXXX �Œǉ����ꂽ���E�����ɂ��Ă�
 * ���̊֐����Ă΂ꂽ���_�œo�^����Ă���v�f�̋��E�ʂ𒊏o���āA���̖ʂŋ��E�������X�V����B
 * ��K�͕��G���f���̂����̏������X�V����ꍇ�́A���E�ʂ̒��o�Ɏ��Ԃƃ�������v����ꍇ������B
 */
void rcapCommit(void);

/**
 * @brief �ו��Ɠ����ɍX�V����ߓ_�O���[�v��o�^
 * @param[in] dataname �ߓ_�O���[�v�̎��ʎq
 * @param[in] num �ߓ_�O���[�v�̐ߓ_��
 * @param[in] nodeArray �ߓ_�O���[�v�̐ߓ_
 * @note refineElement ���Ăяo���O�ɂ��̊֐��Őߓ_�O���[�v��o�^���Ă����ƁArefineElement ���Ăяo�������ɁA
 * �ו�����v�f�̕ӂ̗��[���ߓ_�O���[�v�Ɋ܂܂��Ȃ�΁A�ו��ɂ���Đ������ꂽ�ӂ̒��_�̐ߓ_��
 * �ߓ_�O���[�v�ɒǉ�����B
 * ���ʎq�͕�����ł��BFortran ����Ăяo���ꍇ�́A������̍Ō�Ƀk��������ǉ����Ă��������B
 * @code
 * int32_t cl[2] = {1,2};
 * appendNodeGroup("CL",2,cl);
 * refineElement( num, etype, nodeArray, resultNodeArray );
 * rcapCommit();
 * size_t s = getNodeGroupCount("CL");
 * int32_t* resultcl = new int_32[s];
 * getNodeGroup("CL",s,resultcl);
 * @endcode
 */
void rcapAppendNodeGroup( const char dataname[80], size_t num, int32_t* nodeArray );

/**
 * @brief Refiner �ɓo�^����Ă���ߓ_�O���[�v�̐ߓ_�̌���Ԃ�
 * @param[in] dataname �ߓ_�O���[�v�̎��ʎq
 * @return �ߓ_�O���[�v�̐ߓ_�̌�
 * @note refineElement ���ĂԑO�́AappendNodeGroup �œo�^���������̂��̂�Ԃ��B
 */
size_t rcapGetNodeGroupCount( const char dataname[80] );

/**
 * @brief Refiner �ɓo�^����Ă���ߓ_�O���[�v��Ԃ�
 * @param[in] dataname �ߓ_�O���[�v�̎��ʎq
 * @param[in] num �擾����ߓ_�O���[�v�̌�
 * @param[out] nodeArray �ߓ_�O���[�v�̐ߓ_
 * @note ���� num �� getNodeGroupCount �Ŏ擾�����l��^���āAnodeArray �͂��炩���� num ��
 * �傫���̔z��ł��炩���� allocate ���Ă�����̂Ƃ���B
 */
void rcapGetNodeGroup( const char dataname[80], size_t num, int32_t* nodeArray );

/**
 * @brief BoundaryNodeGroup �Ƃ́A���E�ʏ�ɂ݂̂���ߓ_�O���[�v�̂��ƁB���̊֐��œo�^����B
 * @param[in] dataname ���E�ߓ_�O���[�v�̎��ʎq
 * @param[in] num �擾���鋫�E�ߓ_�O���[�v�̌�
 * @param[out] nodeArray ���E�ߓ_�O���[�v�̐ߓ_
 * @note rcapGetBoundaryNodeGroup ����Ƃ��́A���炩���� nodeArray �� allocate ���Ă������ƁB
 * @note refineElement ������Ƃ��ɁA���̃^�C�v�̋��E���������鎞�́A�\�ʒ��o���s���A�\�ʂ̎O�p�`�܂��͎l�p�`��
 * �g���ċ��E�������X�V����B
 */
void rcapAppendBNodeGroup( const char dataname[80], size_t num, int32_t* nodeArray );

/**
 * @brief Refiner �ɓo�^����Ă��鋫�E�ߓ_�O���[�v�̐ߓ_�̌���Ԃ�
 * @param[in] dataname �ߓ_�O���[�v�̎��ʎq
 * @return �ߓ_�O���[�v�̐ߓ_�̌�
 * @note refineElement ���ĂԑO�́AappendBNodeGroup �œo�^���������̂��̂�Ԃ��B
 */
size_t rcapGetBNodeGroupCount( const char dataname[80] );

/**
 * @brief Refiner �ɓo�^����Ă��鋫�E�ߓ_�O���[�v��Ԃ�
 * @param[in] dataname ���E�ߓ_�O���[�v�̎��ʎq
 * @param[in] num �擾���鋫�E�ߓ_�O���[�v�̌�
 * @param[out] nodeArray ���E�ߓ_�O���[�v�̐ߓ_
 * @note ���� num �� getBNodeGroupCount �Ŏ擾�����l��^���āAnodeArray �͂��炩���� num ��
 * �傫���̔z��ł��炩���� allocate ���Ă�����̂Ƃ���B
 */
void rcapGetBNodeGroup( const char dataname[80], size_t num, int32_t* nodeArray );

/**
 * @brief BoundaryNodeVariableInt �Ƃ́A���E�ʏ�ɂ݂̂���ߓ_��̐����l�ϐ��̂���
 * @param[in] dataname ���E�ߓ_�ϐ��̎��ʎq
 * @param[in] num �擾���鋫�E�ߓ_�ϐ��́i�ߓ_�́j��
 * @param[out] nodeArray ���E�ߓ_�ϐ��̐ߓ_
 * @param[out] nodeVars ���E�ߓ_�ϐ��̒l�inodeArray�Ɠ����傫���j
 * @note rcapGetBoundaryNodeVariableInt ����Ƃ��́A���炩���� nodeArray �� nodeVars �� allocate ���Ă������ƁB
 * @note refineElement ������Ƃ��ɁA���̃^�C�v�̋��E���������鎞�́A�\�ʒ��o���s���A�\�ʂ̎O�p�`�܂��͎l�p�`��
 * �g���ċ��E�������X�V����B
 * @note �ו��Ő����钆�Ԑߓ_�ɂ��āA
 * ���Ƃ̐ߓ_�̂Ȃ��ŕϐ����^�����Ă��Ȃ��ߓ_������΁A���Ԑߓ_�ɂ͕ϐ��͗^���Ȃ��B
 * ���Ƃ̐ߓ_�ɕϐ����^�����Ă��āA�ϐ��̒l�����ׂē��������́A���Ԑߓ_�ɂ��̓������l��^����B
 * ���Ƃ̐ߓ_�ɕϐ����^�����Ă��āA�ϐ��̒l���قȂ鎞�́A���Ԑߓ_�ɍł��������l��^����B
 */
void rcapAppendBNodeVarInt( const char dataname[80], size_t num, int32_t* nodeArray, int32_t* nodeVars );

/**
 * @brief Refiner �ɓo�^����Ă��鐮���l���E�ߓ_�ϐ��̐ߓ_�̌���Ԃ�
 * @param[in] dataname ���E�ߓ_�ϐ��̎��ʎq
 * @return ���E�ߓ_�ϐ��̐ߓ_�̌�
 * @note refineElement ���ĂԑO�́AappendBNodeVarInt �œo�^���������̂��̂�Ԃ��B
 */
size_t rcapGetBNodeVarIntCount( const char dataname[80] );

/**
 * @brief Refiner �ɓo�^����Ă��鐮���l���E�ߓ_�ϐ���Ԃ�
 * @param[in] dataname ���E�ߓ_�ϐ��̎��ʎq
 * @param[in] num �擾���鋫�E�ߓ_�ϐ��̌�
 * @param[out] nodeArray ���E�ߓ_�ϐ��̐ߓ_
 * @param[out] nodeVars ���E�ߓ_�ϐ��̒l
 * @note ���� num �� getBNodeVarIntCount �Ŏ擾�����l��^���āAnodeArray, nodeVars �͂��炩���� num ��
 * �傫���̔z��ł��炩���� allocate ���Ă�����̂Ƃ���B
 */
void rcapGetBNodeVarInt( const char dataname[80], size_t num, int32_t* nodeArray, int32_t* nodeVars );

/**
 * @brief ElementGroup �Ƃ́A�v�f�ԍ��̏W���̂��ƁB
 * @param[in] dataname �v�f�O���[�v�̎��ʎq
 * @param[in] num �擾����v�f�O���[�v�̌�
 * @param[out] elementArray �v�f�O���[�v�̔z��
 * @note rcapGetElementGroup ����Ƃ��́A���炩���� elementArray �� allocate ���Ă������ƁB
 */
void rcapAppendElementGroup( const char dataname[80], size_t num, int32_t* elementArray );

/**
 * @brief Refiner �ɓo�^����Ă���v�f�O���[�v�̗v�f�̌���Ԃ�
 * @param[in] dataname �v�f�O���[�v�̎��ʎq
 * @return �v�f�O���[�v�̗v�f�̌�
 * @note refineElement ���ĂԑO�́AappendElementGroup �œo�^���������̂��̂�Ԃ��B
 */
size_t rcapGetElementGroupCount( const char dataname[80] );

/**
 * @brief Refiner �ɓo�^����Ă���v�f�O���[�v��Ԃ�
 * @param[in] dataname �v�f�O���[�v�̎��ʎq
 * @param[in] num �擾����v�f�O���[�v�̌�
 * @param[out] elementArray �v�f�O���[�v�̐ߓ_
 * @note ���� num �� getElementGroupCount �Ŏ擾�����l��^���āAelementArray �͂��炩���� num ��
 * �傫���̔z��ł��炩���� allocate ���Ă�����̂Ƃ���B
 */
void rcapGetElementGroup( const char dataname[80], size_t num, int32_t* elementArray );

/**
 * @brief FaceGroup �Ƃ́A�v�f�ԍ��A�v�f���ʔԍ��̑g�̂��ƁB�A���ʂ��ו�����ꍇ�Ȃǂɗp����B
 * @param[in] dataname �ʃO���[�v�̎��ʎq
 * @param[in] num �擾����ʃO���[�v�̌��i�v�f�ԍ��Ɩʔԍ��̑g�̌��j
 * @param[out] faceArray �ʃO���[�v�̗v�f�ԍ��A�ʔԍ��̑g�̔z��B
 * @note faceArray �͗v�f�ԍ��Ɩʔԍ������݂ɕ��ׂ� 2*num �̔z��ɂȂ�B
 */
void rcapAppendFaceGroup( const char dataname[80], size_t num, int32_t* faceArray );

/**
 * @brief Refiner �ɓo�^����Ă���ʃO���[�v�̌���Ԃ�
 * @param[in] dataname �ʃO���[�v�̎��ʎq
 * @return �ʃO���[�v�̖ʂ̌�
 * @note refineElement ���ĂԑO�́AappendFaceGroup �œo�^���������̂��̂�Ԃ��B
 */
size_t rcapGetFaceGroupCount( const char dataname[80] );

/**
 * @brief Refiner �ɓo�^����Ă���ʃO���[�v��Ԃ�
 * @param[in] dataname �ʃO���[�v�̎��ʎq
 * @param[in] num �擾����ʃO���[�v�̌�
 * @param[out] faceArray �ʃO���[�v�̖�
 * @note ���� num �� getFaceGroupCount �Ŏ擾�����l��^���āAfaceArray �͂��炩���� num ��
 * �傫���̔z��ł��炩���� allocate ���Ă�����̂Ƃ���B
 */
void rcapGetFaceGroup( const char dataname[80], size_t num, int32_t* faceArray );

/**
 * @brief NodeVariable ��o�^�����Ƃ��ɁA���Ԑߓ_�ɗ^����l�̌��ߕ���I�����܂��B
 * ���݂� "MIN" "MAX" "MIDDLE" ��3��ނɑΉ����Ă��܂��B
 * MIN �͒��Ԑߓ_�𐶐�����̂ɗp�����ߓ_��̒l�̍ŏ��l��^���܂��B
 * MAX �͒��Ԑߓ_�𐶐�����̂ɗp�����ߓ_��̒l�̍ő�l��^���܂��B
 * MIDDLE �͒��Ԑߓ_�𐶐�����̂ɗp�����ߓ_��̒l�̕��ϒl��^���܂��B
 */
void rcapSetInterpolateMode( const char mode[32] );

/**
 * @brief NodeVariable ��o�^�����Ƃ��ɁA���Ԑߓ_�ɗ^����l�̌��ߕ���Ԃ��܂��B
 * �߂�l�� "MIN" "MAX" "MIDDLE" �Ƃ���������̂����ꂩ�ł��B
 */
void rcapGetInterpolateMode( char mode[32] );

/**
 * @brief ���Ԑߓ_����A����𐶐�����̂Ɏg�����ӁA�ʁA�v�f�̐ߓ_�z���Ԃ�
 * @param[in] localNodeId �ו����ꂽ���Ԑߓ_�̋Ǐ��ߓ_�ԍ�
 * @param[out] originalNodeArray localNodeId �𐶐�����̂Ɏg�����v�f�̐ߓ_�z���Ԃ��܂�
 * @return �߂�l�͗v�f�̌^
 * @note ��F�ӂ̒��_�̏ꍇ�͐ߓ_�z��ɂQ�l�������� RCAP_SEGMENT ��Ԃ��BlocalNodeId �ɍו��Ő������ꂽ�_
 * �łȂ��_��^�����ꍇ�́A���������� -1 = RCAP_UNKNOWNTYPE ��Ԃ��܂��B
 *       ��F�l�p�`�̒��S�̏ꍇ�́A�ߓ_�z��ɂS�l�������� RCAP_QUAD ��Ԃ��B
 *
 * @note ���̊֐����Ăԏꍇ�� rcapRefineElement �ŗ^�����ו��O�̗v�f�̐ߓ_�z�� nodeArray
 * ����� rcapRefineElementMulti �ŗ^�����ו��O�̗v�f�̗v�f�^�C�v�z�� etypeArray �� �ߓ_�z�� nodeArray
 * �̃�������������Ȃ��ł��������B�ʂ̒l�ŏ㏑�����邱�Ƃ���߂Ă��������B
 * �i���̊֐��̓����ł��̐ߓ_�z����Q�Ƃ��Ă��邽�߁j
 *
 */
int8_t rcapGetOriginal( int32_t localNodeId, int32_t* originalNodeArray );

/**
 * @brief �ӁA�ʁA�v�f��^���āA���ꂩ����ꂽ���Ԑߓ_��߂�l�ŕԂ�
 * @return ���Ԑߓ_�̐ߓ_�ԍ�
 *
 * ���ɍו����s��ꂽ��ł�
 *
 * \code
 * middle = rcapGetMiddle( RCAP_SEGMENT, nodeArray );
 * \endcode
 *
 * ��
 *
 * \code
 * rcapRefineElement( 1, RCAP_SEGMENT, nodeArray, resultArray );
 * middle = resultArray[2];
 * \endcode
 *
 * �͓����ł��B�������A�ו����s���Ă��Ȃ��ꍇ�i���Ԑߓ_�����݂��Ȃ��ꍇ�j��
 * rcapGetMiddle �͖����Ȑߓ_�ԍ�(-1)��Ԃ��܂����A
 * rcapRefineElement �͍ו������Ă��炻�̌��ʂ�Ԃ��܂��B
 */
int32_t rcapGetMiddle( int8_t etype, int32_t* originalNodeArray );

/**
 * @brief �v�f�̕i���Ɋւ�������o�͂��܂��B
 * @param[in] name �i���𑪒肷��w�W
 * @param[in] filename �t�@�C�����i�w�肵�Ȃ��ꍇ�͕W���G���[�ɏo�͂��܂��j
 */
void rcapQualityReport( const char name[80], const char* filename );

/**
 * @brief �ו���̎��R���W�̕ϊ�
 * ������
 */


/**
 * @brief �f�o�b�O�p�t�@�C�����o�̓��[�`��
 * @return �ǂݍ��񂾁A�܂��͏������񂾗v�f�̌�
 *
 * File ���� File �ւ̍ו���
 * rcapInitRefiner(0,0);
 * rcapLoadGFFile( "MESH", "BOUN" );
 * rcapRefineFFbModel();
 * rcapSaveGFFile( "REFINEMESH", "REFINEBOUN" );
 * rcapTermRefiner();
 *
 * @remark �����ŏo�͂���f�[�^�� rcapRefinerDoc.mesh �Ɋi�[���ꂽ����
 * �Ȃ̂ŁA�ו���̃��b�V�����o�͂ł���킯�ł͂Ȃ��B
 */
int32_t rcapLoadGFFile( const char* gffile, const char* bounfile );

/**
 * @brief FFbModel�ו��i�f�o�b�O�p�j
 */
void rcapRefineFFbModel();

/**
 * @brief GF�t�@�C���֏������݁i�f�o�b�O�p�j
 */
int32_t rcapSaveGFFile( const char* gffile, const char* bounfile );

/**
 * @brief HEC�t�@�C���̓ǂݍ��݁i�f�o�b�O�p�j
 */
int32_t rcapLoadHECFile( const char* hecfile );

/**
 * @brief HEC�t�@�C���֏������݁i�f�o�b�O�p�j
 */
int32_t rcapSaveHECFile( const char* hecfile );

/**
 * @brief RNF�t�@�C���֏������݁i�f�o�b�O�p�j
 */
int32_t rcapSaveRNFFile( const char* rnffile );

/* rcapxxx_  ���ׂď����� */
/* gfortran, intel fortran, pgi fortran �͂���ł悢 */
#if defined FORTRAN90 || defined FORTRAN_CALL_C_DOWNCASE_
void rcapgetversion_( void );
void rcapinitrefiner_( int32_t* nodeOffset, int32_t* elementOffset );
void rcapclearrefiner_( void );
void rcaptermrefiner_( void );
void rcapsetcadfilename_( const char* filename );
void rcapsetsecondfitting_( int32_t* flag );
void rcapsetsmoothing_( int32_t* flag );
void rcapsetpartitionfilename_( const char* filename );

void rcapsetnode64_( int32_t* num, float64_t* coords, int32_t* globalIds, int32_t* localIds );
void rcapsetnode32_( int32_t* num, float32_t* coords, int32_t* globalIds, int32_t* localIds );
int32_t rcapgetnodecount_( void );
void rcapgetnode64_( int32_t* num, int32_t* localIds, float64_t* coords );
void rcapgetnode32_( int32_t* num, int32_t* localIds, float32_t* coords );
void rcapgetnodeseq64_( int32_t* num, int32_t* initId, float64_t* coords );
void rcapgetnodeseq32_( int32_t* num, int32_t* initId, float32_t* coords );

int32_t rcapgetrefineelementcount_( int32_t* num, int8_t* etype );
int32_t rcaprefineelement_( int32_t* num, int8_t* etype, int32_t* nodeArray, int32_t* resultNodeArray );
int32_t rcapgetrefineelementmulticount_( int32_t* num, int8_t* etypeArray, int32_t* refinedNum );
int32_t rcaprefineelementmulti_( int32_t* num, int8_t* etypeArray, int32_t* nodeArray, int32_t* refinedNum, int8_t* resultEtypeArray, int32_t* resultNodeArray );
void rcapcommit_( void );

void rcapappendnodegroup_( const char dataname[80], int32_t* num, int32_t* nodeArray );
int32_t rcapgetnodegroupcount_( const char dataname[80] );
void rcapgetnodegroup_( const char dataname[80], int32_t* num, int32_t* nodeArray );

void rcapappendbnodegroup_( const char dataname[80], int32_t* num, int32_t* nodeArray );
int32_t rcapgetbnodegroupcount_( const char dataname[80] );
void rcapgetbnodegroup_( const char dataname[80], int32_t* num, int32_t* nodeArray );

void rcapappendbnodevarint_( const char dataname[80], int32_t* num, int32_t* nodeArray, int32_t* nodeVars );
int32_t rcapgetbnodevarintcount_( const char dataname[80] );
void rcapgetbnodevarint_( const char dataname[80], int32_t* num, int32_t* nodeArray, int32_t* nodeVars  );

void rcapappendelementgroup_( const char dataname[80], int32_t* num, int32_t* elementArray );
int32_t rcapgetelementgroupcount_( const char dataname[80] );
void rcapgetelementgroup_( const char dataname[80], int32_t* num, int32_t* elementArray );

void rcapappendfacegroup_( const char dataname[80], int32_t* num, int32_t* faceArray );
int32_t rcapgetfacegroupcount_( const char dataname[80] );
void rcapgetfacegroup_( const char dataname[80], int32_t* num, int32_t* faceArray );

int8_t rcapgetoriginal_( int32_t* localNodeId, int32_t* originalNodeArray );
int32_t rcapgetmiddle_( int8_t *etype, int32_t* originalNodeArray );

void rcapsetinterpolatemode_( const char mode[32] );
void rcapgetinterpolatemode_( char mode[32] );

void rcapqualityreport_( const char mode[80], const char* filename );

int32_t rcaploadgffile_( const char* gffile, const char* bounfile );
int32_t rcaploadhecfile_( const char* hecfile );
int32_t rcapsavegffile_( const char* gffile, const char* bounfile );
int32_t rcapsavehecfile_( const char* hecfile );
int32_t rcapsavernffile_( const char* rnffile );
#endif

#if defined FORTRAN_CALL_C_DOWNCASE__
#define rcapgetversion_ rcapgetversion__
#define rcapinitrefiner_ rcapinitrefiner__
#define rcapclearrefiner_ rcapclearrefiner__
#define rcaptermrefiner_ rcaptermrefiner__
#define rcapsetcadfilename_ rcapsetcadfilename__
#define rcapsetsecondfitting_ rcapsetsecondfitting__
#define rcapsetsmoothing_ rcapsetsmoothing__
#define rcapsetpartitionfilename_ rcapsetpartitionfilename__
#define rcapsetnode64_ rcapsetnode64__
#define rcapsetnode32_ rcapsetnode32__
#define rcapgetnodecount_ rcapgetnodecount__
#define rcapgetnode64_ rcapgetnode64__
#define rcapgetnode32_ rcapgetnode32__
#define rcapgetnodeseq64_ rcapgetnodeseq64__
#define rcapgetnodeseq32_ rcapgetnodeseq32__
#define rcapgetrefineelementcount_ rcapgetrefineelementcount__
#define rcaprefineelement_ rcaprefineelement__
#define rcapgetrefineelementmulticount_ rcapgetrefineelementmulticount__
#define rcaprefineelementmulti_ rcaprefineelementmulti__
#define rcapcommit_ rcapcommit__
#define rcapappendnodegroup_ rcapappendnodegroup__
#define rcapgetnodegroupcount_ rcapgetnodegroupcount__
#define rcapgetnodegroup_ rcapgetnodegroup__
#define rcapappendbnodegroup_ rcapappendbnodegroup__
#define rcapgetbnodegroupcount_ rcapgetbnodegroupcount__
#define rcapgetbnodegroup_ rcapgetbnodegroup__
#define rcapappendbnodevarint_ rcapappendbnodevarint__
#define rcapgetbnodevarintcount_ rcapgetbnodevarintcount__
#define rcapgetbnodevarint_ rcapgetbnodevarint__
#define rcapappendelementgroup_ rcapappendelementgroup__
#define rcapgetelementgroupcount_ rcapgetelementgroupcount__
#define rcapgetelementgroup_ rcapgetelementgroup__
#define rcapappendfacegroup_ rcapappendfacegroup__
#define rcapgetfacegroupcount_ rcapgetfacegroupcount__
#define rcapgetfacegroup_ rcapgetfacegroup__
#define rcapgetoriginal_ rcapgetoriginal__
#define rcapgetmiddle_ rcapgetmiddle__
#define rcapsetinterpolatemode_ rcapsetinterpolatemode__
#define rcapgetinterpolatemode_ rcapgetinterpolatemode__
#define rcapqualityreport_ rcapqualityreport__
#define rcaploadgffile_ rcaploadgffile__
#define rcaploadhecfile_ rcaploadhecfile__
#define rcapsavegffile_ rcapsavegffile__
#define rcapsavehecfile_ rcapsavehecfile__
#define rcapsavernffile_ rcapsavernffile__
#endif

#if defined FORTRAN_CALL_C_UPCASE
#define rcapgetversion_ RCAPGETVERSION
#define rcapinitrefiner_ RCAPINITREFINER
#define rcapclearrefiner_ RCAPCLEARREFINER
#define rcaptermrefiner_ RCAPTERMREFINER
#define rcapsetcadfilename_ RCAPSETCADFILENAME
#define rcapsetsecondfitting_ RCAPSETSECONDFITTING
#define rcapsetsmoothing_ RCAPSETSMOOTHING
#define rcapsetpartitionfilename_ RCAPSETPARTITIONFILENAME
#define rcapsetnode64_ RCAPSETNODE64
#define rcapsetnode32_ RCAPSETNODE32
#define rcapgetnodecount_ RCAPGETNODECOUNT
#define rcapgetnode64_ RCAPGETNODE64
#define rcapgetnode32_ RCAPGETNODE32
#define rcapgetnodeseq64_ RCAPGETNODESEQ64
#define rcapgetnodeseq32_ RCAPGETNODESEQ32
#define rcapgetrefineelementcount_ RCAPGETREFINEELEMENTCOUNT
#define rcaprefineelement_ RCAPREFINEELEMENT
#define rcapgetrefineelementmulticount_ RCAPGETREFINEELEMENTMULTICOUNT
#define rcaprefineelementmulti_ RCAPREFINEELEMENTMULTI
#define rcapcommit_ RCAPCOMMIT
#define rcapappendnodegroup_ RCAPAPPENDNODEGROUP
#define rcapgetnodegroupcount_ RCAPGETNODEGROUPCOUNT
#define rcapgetnodegroup_ RCAPGETNODEGROUP
#define rcapappendbnodegroup_ RCAPAPPENDBNODEGROUP
#define rcapgetbnodegroupcount_ RCAPGETBNODEGROUPCOUNT
#define rcapgetbnodegroup_ RCAPGETBNODEGROUP
#define rcapappendbnodevarint_ RCAPAPPENDBNODEVARINT
#define rcapgetbnodevarintcount_ RCAPGETBNODEVARINTCOUNT
#define rcapgetbnodevarint_ RCAPGETBNODEVARINT
#define rcapappendelementgroup_ RCAPAPPENDELEMENTGROUP
#define rcapgetelementgroupcount_ RCAPGETELEMENTGROUPCOUNT
#define rcapgetelementgroup_ RCAPGETELEMENTGROUP
#define rcapappendfacegroup_ RCAPAPPENDFACEGROUP
#define rcapgetfacegroupcount_ RCAPGETFACEGROUPCOUNT
#define rcapgetfacegroup_ RCAPGETFACEGROUP
#define rcapgetoriginal_ RCAPGETORIGINAL
#define rcapgetmiddle_ RCAPGETMIDDLE
#define rcapsetinterpolatemode_ RCAPSETINTERPOLATEMODE
#define rcapgetinterpolatemode_ RCAPGETINTERPOLATEMODE
#define rcapqualityreport_ RCAPQUALITYREPORT
#define rcaploadgffile_ RCAPLOADGFFILE
#define rcaploadhecfile_ RCAPLOADHECFILE
#define rcapsavegffile_ RCAPSAVEGFFILE
#define rcapsavehecfile_ RCAPSAVEHECFILE
#define rcapsavernffile_ RCAPSAVERNFFILE
#endif

#if defined FORTRAN_CALL_C_UPCASE_
#define rcapgetversion_ RCAPGETVERSION_
#define rcapinitrefiner_ RCAPINITREFINER_
#define rcapclearrefiner_ RCAPCLEARREFINER_
#define rcaptermrefiner_ RCAPTERMREFINER_
#define rcapsetcadfilename_ RCAPSETCADFILENAME_
#define rcapsetsecondfitting_ RCAPSETSECONDFITTING_
#define rcapsetsmoothing_ RCAPSETSMOOTHING_
#define rcapsetpartitionfilename_ RCAPSETPARTITIONFILENAME_
#define rcapsetnode64_ RCAPSETNODE64_
#define rcapsetnode32_ RCAPSETNODE32_
#define rcapgetnodecount_ RCAPGETNODECOUNT_
#define rcapgetnode64_ RCAPGETNODE64_
#define rcapgetnode32_ RCAPGETNODE32_
#define rcapgetnodeseq64_ RCAPGETNODESEQ64_
#define rcapgetnodeseq32_ RCAPGETNODESEQ32_
#define rcapgetrefineelementcount_ RCAPGETREFINEELEMENTCOUNT_
#define rcaprefineelement_ RCAPREFINEELEMENT_
#define rcapgetrefineelementmulticount_ RCAPGETREFINEELEMENTMULTICOUNT_
#define rcaprefineelementmulti_ RCAPREFINEELEMENTMULTI_
#define rcapcommit_ RCAPCOMMIT_
#define rcapappendnodegroup_ RCAPAPPENDNODEGROUP_
#define rcapgetnodegroupcount_ RCAPGETNODEGROUPCOUNT_
#define rcapgetnodegroup_ RCAPGETNODEGROUP_
#define rcapappendbnodegroup_ RCAPAPPENDBNODEGROUP_
#define rcapgetbnodegroupcount_ RCAPGETBNODEGROUPCOUNT_
#define rcapgetbnodegroup_ RCAPGETBNODEGROUP_
#define rcapappendbnodevarint_ RCAPAPPENDBNODEVARINT_
#define rcapgetbnodevarintcount_ RCAPGETBNODEVARINTCOUNT_
#define rcapgetbnodevarint_ RCAPGETBNODEVARINT_
#define rcapappendelementgroup_ RCAPAPPENDELEMENTGROUP_
#define rcapgetelementgroupcount_ RCAPGETELEMENTGROUPCOUNT_
#define rcapgetelementgroup_ RCAPGETELEMENTGROUP_
#define rcapappendfacegroup_ RCAPAPPENDFACEGROUP_
#define rcapgetfacegroupcount_ RCAPGETFACEGROUPCOUNT_
#define rcapgetfacegroup_ RCAPGETFACEGROUP_
#define rcapgetoriginal_ RCAPGETORIGINAL_
#define rcapgetmiddle_ RCAPGETMIDDLE_
#define rcapsetinterpolatemode_ RCAPSETINTERPOLATEMODE_
#define rcapgetinterpolatemode_ RCAPGETINTERPOLATEMODE_
#define rcapqualityreport_ RCAPQUALITYREPORT_
#define rcaploadgffile_ RCAPLOADGFFILE_
#define rcaploadhecfile_ RCAPLOADHECFILE_
#define rcapsavegffile_ RCAPSAVEGFFILE_
#define rcapsavehecfile_ RCAPSAVEHECFILE_
#define rcapsavernffile_ RCAPSAVERNFFILE_
#endif

#ifdef __cplusplus
}
#endif

#endif
