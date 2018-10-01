/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_Refiner version 0.4                          #
# Program Name : rcapRefinerIO                                         #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2010/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/
#ifndef REVOCAP_REFINERIO_HEADER
#define REVOCAP_REFINERIO_HEADER

#include "rcapRefiner.h"

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
#ifdef FORTRAN90
int32_t rcaploadgffile_( const char* gffile, const char* bounfile );
int32_t rcaploadhecfile_( const char* hecfile );
int32_t rcapsavegffile_( const char* gffile, const char* bounfile );
int32_t rcapsavehecfile_( const char* hecfile );
int32_t rcapsavernffile_( const char* rnffile );
void rcaprefineffbmodel_(void);
#endif

#ifdef FORTRAN77
#endif

#ifdef G95
#endif

#ifdef INTEL_FORTRAN
#endif

#ifdef PG_FORTRAN
#endif

#ifdef __cplusplus
}
#endif

#endif
