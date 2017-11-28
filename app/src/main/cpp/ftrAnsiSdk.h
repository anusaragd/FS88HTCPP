/*

Copyright (c) Futronic Technology Company Ltd. All rights reserved.

Abstract:

Definitions and prototypes for the Futronic ANSI SDK.

*/

#ifndef __FUTRONIC_ANSI_SDK_H__
#define __FUTRONIC_ANSI_SDK_H__

#include "ftrScanAPI.h"

#if defined(__WIN32__)
#pragma pack(push, 1)
#endif

/* Impression types */
#define FTR_ANSISDK_IMPTYPE_LP                  0x00    /* Live-scan plain */
#define FTR_ANSISDK_IMPTYPE_NP                  0x02    /* Nonlive-scan plain */

/* Position type codes */
#define FTR_ANSISDK_FINGPOS_UK                  0x00    /* Unknown finger */
#define FTR_ANSISDK_FINGPOS_RT                  0x01    /* Right thumb */
#define FTR_ANSISDK_FINGPOS_RI                  0x02    /* Right index finger */
#define FTR_ANSISDK_FINGPOS_RM                  0x03    /* Right middle finger */
#define FTR_ANSISDK_FINGPOS_RR                  0x04    /* Right ring finger */
#define FTR_ANSISDK_FINGPOS_RL                  0x05    /* Right little finger */
#define FTR_ANSISDK_FINGPOS_LT                  0x06    /* Left thumb */
#define FTR_ANSISDK_FINGPOS_LI                  0x07    /* Left index finger */
#define FTR_ANSISDK_FINGPOS_LM                  0x08    /* Left middle finger */
#define FTR_ANSISDK_FINGPOS_LR                  0x09    /* Left ring finger */
#define FTR_ANSISDK_FINGPOS_LL                  0x0A    /* Left little finger */

/* Finger quality values */
#define FTR_ANSISDK_QUAL_POOR                   20      /* NFIQ value 5 */
#define FTR_ANSISDK_QUAL_FAIR                   40      /* NFIQ value 4 */
#define FTR_ANSISDK_QUAL_GOOD                   60      /* NFIQ value 3 */
#define FTR_ANSISDK_QUAL_VGOOD                  80      /* NFIQ value 2 */
#define FTR_ANSISDK_QUAL_EXCELLENT              100     /* NFIQ value 1 */

#define FTR_ANSISDK_ERROR_BASE                  0x30000000
#define FTR_ANSISDK_ERROR_CODE( x )             (FTR_ANSISDK_ERROR_BASE  | (x))

#define FTR_ANSISDK_ERROR_NO_ERROR              0
#define FTR_ANSISDK_ERROR_IMAGE_SIZE_NOT_SUP    FTR_ANSISDK_ERROR_CODE( 1 )
#define FTR_ANSISDK_ERROR_EXTRACTION_UNSPEC     FTR_ANSISDK_ERROR_CODE( 2 )
#define FTR_ANSISDK_ERROR_EXTRACTION_BAD_IMP    FTR_ANSISDK_ERROR_CODE( 3 )
#define FTR_ANSISDK_ERROR_MATCH_NULL            FTR_ANSISDK_ERROR_CODE( 4 )
#define FTR_ANSISDK_ERROR_MATCH_PARSE_PROBE     FTR_ANSISDK_ERROR_CODE( 5 )
#define FTR_ANSISDK_ERROR_MATCH_PARSE_GALLERY   FTR_ANSISDK_ERROR_CODE( 6 )
#define FTR_ANSISDK_ERROR_MORE_DATA             FTR_ANSISDK_ERROR_CODE( 7 )

#define FTR_ANSISDK_MATCH_SCORE_LOW             37      /* FAR = 1% */
#define FTR_ANSISDK_MATCH_SCORE_LOW_MEDIUM      65      /* FAR = 0.1% */
#define FTR_ANSISDK_MATCH_SCORE_MEDIUM          93      /* FAR = 0.01% */
#define FTR_ANSISDK_MATCH_SCORE_HIGH_MEDIUM     121     /* FAR = 0.001% */
#define FTR_ANSISDK_MATCH_SCORE_HIGH            146     /* FAR = 0.0001% */
#define FTR_ANSISDK_MATCH_SCORE_VERY_HIGH       189     /* FAR = 0 */

#if defined(__WIN32__)
#pragma pack(pop)
#endif

#if defined( __cplusplus )
extern "C" { /* assume C declarations for C++ */
#endif

FTR_API_PREFIX FTR_BOOL FTR_API ftrAnsiSdkCaptureImage( FTRHANDLE ftrHandle, FTR_PVOID pBuffer );

int FTR_API ftrAnsiSdkGetMaxTemplateSize();
FTR_API_PREFIX FTR_BOOL FTR_API ftrAnsiSdkCreateTemplate( FTRHANDLE ftrHandle, FTR_BYTE byFingerPosition, FTR_PVOID pOutImageBuffer, FTR_PVOID pOutTemplate, int * pnOutTemplateSize );
FTR_API_PREFIX FTR_BOOL FTR_API ftrAnsiSdkCreateTemplateFromBuffer( FTRHANDLE ftrHandle, FTR_BYTE byFingerPosition, FTR_PVOID pImageBuffer, int nWidth, int nHeight, FTR_PVOID pOutTemplate, int * pnOutTemplateSize );

FTR_API_PREFIX FTR_BOOL FTR_API ftrAnsiSdkVerifyTemplate( FTRHANDLE ftrHandle, FTR_BYTE byFingerPosition, FTR_PVOID pInTemplate, FTR_PVOID pOutImageBuffer, float * pfOutResult );
FTR_API_PREFIX FTR_BOOL FTR_API ftrAnsiSdkMatchTemplates( FTR_PVOID pProbeTemplate, FTR_PVOID pGaleryTemplate, float * pfOutResult );

FTR_API_PREFIX FTR_BOOL FTR_API ftrAnsiSdkConvertAnsiTemplateToIso( FTR_PVOID pTemplateANSI, FTR_PVOID pTemplateIso, int * pnInOutTemplateSize );

#ifdef __cplusplus
}
#endif

#endif /* __FUTRONIC_ANSI_SDK_H__ */
