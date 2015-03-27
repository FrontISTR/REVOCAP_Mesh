/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Common Definitions                                                   #
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

/* �萔�̒�` */
/* swig �ɋ�����ꍇ�� REVOCAP_SUPPORT_RUBY ���`���� */
/* PI NULL => swig �ɋ����Ȃ� */
#ifndef REVOCAP_SUPPORT_RUBY

#ifndef PI
 #define PI 3.1415926535897932384626
#endif

#ifndef NULL
 #define NULL 0
#endif

#endif

/* double �l�̔��U�`�F�b�N */
/* #include <cmath> ���K�v */

#if defined _MSC_VER
#define isfinite(x) _finite(x)
#else
#include <cmath>
#define isfinite(x) !std::isnan(x)
#endif

/* �T�C�Y�̒�` */

#ifndef REVOCAP_SIZE_DEF
#define REVOCAP_SIZE_DEF

/* int �̌^ => swig �ɋ����� */
#if defined _MSC_VER || defined REVOCAP_SUPPORT_RUBY || defined REVOCAP_SUPPORT_JAVA
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

#endif /* REVOCAP_SIZE_DEF */

