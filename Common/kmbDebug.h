/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Common Definitions for Debug                                         #
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
#pragma once

/* DEBUG ���b�Z�[�W */
/*
REVOCAP_Debug ��ʓI�ȃf�o�b�O�p���b�Z�[�W
REVOCAP_Debug_1 �ڍׂȃf�o�b�O�p���b�Z�[�W�P
REVOCAP_Debug_2 �ڍׂȃf�o�b�O�p���b�Z�[�W�Q�i�t�@�C���o�͂̒��ł��g���Ă悢�j
REVOCAP_Debug_3 �ڍׂȃf�o�b�O�p���b�Z�[�W�R�i���[�v�̒��ł��g���Ă悢�j
REVOCAP_Debug_X �ڍׂȃf�o�b�O�p���b�Z�[�W�w�i�K���o�́j

�f�o�b�O���ɋǏ��I�ɏo�͂������ꍇ��

#define _DEBUG_ 3
#include "Common/kmbDebug.h"

... �����ł� _DEBUG_ = 3 ���L��
... �����ŌĂяo�����֐����ł� REVOCAP_Debug() ���L���ɂȂ�

#undef _DEBUG_
#include "Common/kmbDebug.h"

�ȂǂƋL�q���Ă�������
*/

#include <cstdio>

#undef REVOCAP_Debug
#undef REVOCAP_Debug_1
#undef REVOCAP_Debug_2
#undef REVOCAP_Debug_3
#undef REVOCAP_Debug_X

#if defined( _MSC_VER ) && ( _MSC_VER < 1400 ) || defined( DEBUG_DISABLE )
	#define REVOCAP_Debug(format)
	#define REVOCAP_Debug_1(format)
	#define REVOCAP_Debug_2(format)
	#define REVOCAP_Debug_3(format)
	#define REVOCAP_Debug_X(format)
#else
	/* _DEBUG �}�N���Ɋ֌W�Ȃ��o�� */
	#define REVOCAP_Debug_X(fmt, ...) fprintf(stderr,"%s, line %d: "fmt,__FILE__,__LINE__, ##__VA_ARGS__)

	#if defined _DEBUG || _DEBUG_
	#include <cstdio>
	#define REVOCAP_Debug(fmt, ...) fprintf(stderr,"%s, line %d: "fmt,__FILE__,__LINE__, ##__VA_ARGS__)
	#else
	#define REVOCAP_Debug(format, ...)
	#endif

	#if ( _DEBUG >= 1 ) || ( _DEBUG_ >= 1 )
	#define REVOCAP_Debug_1(fmt, ...) fprintf(stderr,"%s, line %d: "fmt,__FILE__,__LINE__, ##__VA_ARGS__)
	#else
	#define REVOCAP_Debug_1(format, ...)
	#endif

	#if ( _DEBUG >= 2 ) || ( _DEBUG_ >= 2 )
	#define REVOCAP_Debug_2(fmt, ...) fprintf(stderr,"%s, line %d: "fmt,__FILE__,__LINE__, ##__VA_ARGS__)
	#else
	#define REVOCAP_Debug_2(format, ...)
	#endif

	#if ( _DEBUG >= 3 ) || ( _DEBUG_ >= 3 )
	#define REVOCAP_Debug_3(fmt, ...) fprintf(stderr,"%s, line %d: "fmt,__FILE__,__LINE__, ##__VA_ARGS__)
	#else
	#define REVOCAP_Debug_3(format, ...)
	#endif
#endif
