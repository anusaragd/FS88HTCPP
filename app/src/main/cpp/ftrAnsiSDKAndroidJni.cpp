#include "ftrAnsiSDKAndroidJni.h"
#include "ftrAnsiSdk.h"
#include <string.h>
#include <stdio.h>

jobject m_glbIoCtxRef = NULL;

FTRHANDLE GetDeviceHandle(JNIEnv *env, jobject obj)
{
    /* Get a reference to obj¡¦s class */
    jclass cls = env->GetObjectClass(obj);
    jfieldID handle = NULL; 
    handle = env->GetFieldID( cls, "m_hDevice", "J" );
    if( handle == NULL )
        return NULL;
    FTRHANDLE hDevice = reinterpret_cast<FTRHANDLE>(env->GetLongField( obj, handle ));
    return hDevice;
}

FTR_BOOL SetDeviceHandle(JNIEnv *env, jobject obj, FTRHANDLE hDevice)
{
    /* Get a reference to obj¡¦s class */
    jclass cls = env->GetObjectClass(obj);
    jfieldID handle = NULL; 
    handle = env->GetFieldID( cls, "m_hDevice", "J" );
    env->SetLongField( obj, handle, reinterpret_cast<jlong>(hDevice) );
    return TRUE;
}

void SetErrorCode(JNIEnv *env, jobject obj, jint errCode)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID nErrCode = NULL;
    nErrCode = env->GetFieldID( cls, "m_ErrorCode", "I" );
    if( nErrCode != NULL )
    {
        env->SetIntField( obj, nErrCode, errCode );
    }
}

jboolean JNICALL Java_com_futronictech_AnsiSDKLib_OpenDevice(JNIEnv *env, jobject obj, jint instance)
{
    FTRHANDLE hDevice;
    hDevice = ftrScanOpenDeviceOnInterface(instance);
    if( hDevice == NULL )
    {
        SetErrorCode(env, obj, (jint)ftrScanGetLastError());
        return JNI_FALSE;
    }

    if( SetDeviceHandle( env, obj, hDevice ) )
    {
        return JNI_TRUE;
    }

    return JNI_FALSE;
}

jboolean JNICALL Java_com_futronictech_AnsiSDKLib_OpenDeviceCtx(JNIEnv *env, jobject obj, jobject io_ctx)
{
    FTRHANDLE hDevice;

    m_glbIoCtxRef = env->NewGlobalRef(io_ctx);
    if( m_glbIoCtxRef == NULL )
    	return JNI_FALSE;

	ANDROID_CTX ctx;
	ctx.io_ctx = m_glbIoCtxRef;
	ctx.zero_field = 0;
	ctx.ctx_version = FTR_ANDROID_CTX_VERSION_2;
	ctx.ctx_flags = FTR_ANDROID_CTX_FLAG_GLB_DONE;

	hDevice = ftrScanOpenDeviceWithIoContext( &ctx );
	if( hDevice == NULL )
	{
		SetErrorCode(env, obj, (jint)ftrScanGetLastError());
		return JNI_FALSE;
	}
	if( SetDeviceHandle( env, obj, hDevice ) )
	{
		return JNI_TRUE;
	}

    return JNI_FALSE;
}

jboolean JNICALL Java_com_futronictech_AnsiSDKLib_CloseDevice(JNIEnv *env, jobject obj)
{
    FTRHANDLE hDevice = GetDeviceHandle(env, obj);
    if( hDevice != NULL )
    {
        ftrScanCloseDevice(hDevice);
        SetDeviceHandle( env, obj, NULL );
    }
    if( m_glbIoCtxRef != NULL )
    {
    	//env->DeleteGlobalRef(m_glbIoCtxRef);
		m_glbIoCtxRef  = NULL;
    }

    return JNI_TRUE;
}

jboolean JNICALL Java_com_futronictech_AnsiSDKLib_FillImageSize(JNIEnv *env, jobject obj)
{
    FTRHANDLE hDevice = GetDeviceHandle(env, obj);
    if( hDevice == NULL )
    {
        SetErrorCode(env, obj, FTR_ERROR_INVALID_PARAMETER);
        return JNI_FALSE;
    }

    FTRSCAN_IMAGE_SIZE ImageSize;
    if( !ftrScanGetImageSize( hDevice, &ImageSize ) )
    {
        SetErrorCode(env, obj, (jint)ftrScanGetLastError());
        return JNI_FALSE;
    }

    /* Get a reference to obj¡¦s class */
    jclass cls = env->GetObjectClass(obj);
    jfieldID fidw = NULL; 
    jfieldID fidh = NULL;
    fidw = env->GetFieldID( cls, "m_ImageWidth", "I" );
    if( fidw == NULL )
        return JNI_FALSE;
    env->SetIntField( obj, fidw, ImageSize.nWidth );
    //
    fidh = env->GetFieldID( cls, "m_ImageHeight", "I" );
    if( fidh == NULL )
    {
        return JNI_FALSE;
    }

    env->SetIntField( obj, fidh, ImageSize.nHeight );
    
    return JNI_TRUE;
}

jboolean JNICALL Java_com_futronictech_AnsiSDKLib_IsFingerPresent(JNIEnv *env, jobject obj)
{
    FTRHANDLE hDevice = GetDeviceHandle(env, obj);
    if( hDevice == NULL )
    {
        SetErrorCode(env, obj, FTR_ERROR_INVALID_PARAMETER);
        return JNI_FALSE;
    }

    if( ftrScanIsFingerPresent(hDevice, NULL) )
    {
        return JNI_TRUE;
    }

    SetErrorCode(env, obj, (jint)ftrScanGetLastError());
    return JNI_FALSE;
}

jboolean JNICALL Java_com_futronictech_AnsiSDKLib_CaptureImage(JNIEnv *env, jobject obj, jbyteArray pImage)
{
    FTRHANDLE hDevice = GetDeviceHandle(env, obj);
    if( hDevice == NULL )
    {
        SetErrorCode(env, obj, FTR_ERROR_INVALID_PARAMETER);
        return JNI_FALSE;
    }

    jboolean res = JNI_TRUE;

    jboolean isCopy;
    jbyte *pJData = env->GetByteArrayElements( pImage, &isCopy );
    if(pJData)
    {
        if( !ftrAnsiSdkCaptureImage( hDevice, pJData ) )
        {
            SetErrorCode(env, obj, (jint)ftrScanGetLastError());
            res = JNI_FALSE;
        }

        env->ReleaseByteArrayElements( pImage, pJData, 0 );
    }
    else
    {
        SetErrorCode(env, obj, (jint)FTR_ERROR_NOT_ENOUGH_MEMORY);
        res = JNI_FALSE;
    }

    return res;
}

jboolean JNICALL Java_com_futronictech_AnsiSDKLib_CreateTemplate
    (JNIEnv *env, jobject obj, jint finger, jbyteArray pImage, jbyteArray pTemplate, jintArray pTemplateSize)
{
    FTRHANDLE hDevice = GetDeviceHandle(env, obj);
    if( hDevice == NULL )
    {
        SetErrorCode(env, obj, FTR_ERROR_INVALID_PARAMETER);
        return JNI_FALSE;
    }

    jboolean res = JNI_TRUE;

    jboolean isCopy;
    jbyte *img_data = env->GetByteArrayElements( pImage, &isCopy );
    jbyte *template_data = env->GetByteArrayElements( pTemplate, &isCopy );
    jint  *template_size = env->GetIntArrayElements( pTemplateSize, &isCopy );

    if( img_data && template_data && template_size )
    {
        if( !ftrAnsiSdkCreateTemplate( hDevice, finger, img_data, template_data, template_size) )
        {
            SetErrorCode(env, obj, (jint)ftrScanGetLastError());
            res = JNI_FALSE;
        }
    }
    else
    {
        SetErrorCode(env, obj, (jint)FTR_ERROR_NOT_ENOUGH_MEMORY);
        res = JNI_FALSE;
    }

    if( img_data ) env->ReleaseByteArrayElements( pImage, img_data, 0 );
    if( template_data ) env->ReleaseByteArrayElements( pTemplate, template_data, 0 );
    if( template_size )env->ReleaseIntArrayElements( pTemplateSize, template_size, 0 );

    return res;
}

jboolean JNICALL Java_com_futronictech_AnsiSDKLib_VerifyTemplate
    (JNIEnv *env, jobject obj, jint finger, jbyteArray pTemplate, jbyteArray pImage, jfloatArray pResult)
{
    FTRHANDLE hDevice = GetDeviceHandle(env, obj);
    if( hDevice == NULL )
    {
        SetErrorCode(env, obj, FTR_ERROR_INVALID_PARAMETER);
        return JNI_FALSE;
    }

    jboolean res = JNI_TRUE;

    jboolean isCopy;
    jbyte *img_data = env->GetByteArrayElements( pImage, &isCopy );
    jbyte *template_data = env->GetByteArrayElements( pTemplate, &isCopy );
    jfloat *result = env->GetFloatArrayElements( pResult, &isCopy );

    int template_size = 0;

    if(img_data && template_data && result)
    {
        if( !ftrAnsiSdkVerifyTemplate( hDevice, finger, template_data, img_data, result) )
        {
            SetErrorCode(env, obj, (jint)ftrScanGetLastError());
            res = JNI_FALSE;
        }
    }
    else
    {
        SetErrorCode(env, obj, (jint)FTR_ERROR_NOT_ENOUGH_MEMORY);
        res = JNI_FALSE;
    }

    if( img_data ) env->ReleaseByteArrayElements( pImage, img_data, 0 );
    if( template_data ) env->ReleaseByteArrayElements( pTemplate, template_data, 0 );
    if( result )env->ReleaseFloatArrayElements( pResult, result, 0 );

    return res;
}

jboolean JNICALL Java_com_futronictech_AnsiSDKLib_MatchTemplates
    (JNIEnv *env, jobject obj, jbyteArray pProbeTemplate, jbyteArray pGaleryTemplate,jfloatArray pResult)
{
    jboolean res = JNI_TRUE;
    
    jboolean isCopy;
    jbyte *probe = env->GetByteArrayElements( pProbeTemplate, &isCopy );
    jbyte *galery = env->GetByteArrayElements( pGaleryTemplate, &isCopy );
    jfloat *result = env->GetFloatArrayElements( pResult, &isCopy );

    int template_size = 0;

    if(probe && galery && result)
    {
        if( !ftrAnsiSdkMatchTemplates( probe, galery, result) )
        {
            SetErrorCode(env, obj, (jint)ftrScanGetLastError());
            res = JNI_FALSE;
        }
    }
    else
    {
        SetErrorCode(env, obj, (jint)FTR_ERROR_NOT_ENOUGH_MEMORY);
        res = JNI_FALSE;
    }

    if( probe ) env->ReleaseByteArrayElements( pProbeTemplate, probe, 0 );
    if( galery )env->ReleaseByteArrayElements( pGaleryTemplate, galery, 0 );
    if( result )env->ReleaseFloatArrayElements( pResult, result, 0 );

    return res;
}

jboolean JNICALL Java_com_futronictech_AnsiSDKLib_ConvertAnsiTemplateToIso
    (JNIEnv *env, jobject obj, jbyteArray pAnsiTmpl, jbyteArray pIsoTmpl,jintArray pIsoTmplSize)
{
    jboolean res = JNI_TRUE;

    jboolean isCopy;
    jbyte *ansi_tmpl = env->GetByteArrayElements( pAnsiTmpl, &isCopy );
    jbyte *iso_tmpl = env->GetByteArrayElements( pIsoTmpl, &isCopy );
    jint *iso_size = env->GetIntArrayElements( pIsoTmplSize, &isCopy );

    int template_size = 0;

    if(ansi_tmpl && iso_tmpl && iso_size)
    {
        if( !ftrAnsiSdkConvertAnsiTemplateToIso( ansi_tmpl, iso_tmpl, iso_size) )
        {
            SetErrorCode(env, obj, (jint)ftrScanGetLastError());
            res = JNI_FALSE;
        }
    }
    else
    {
        SetErrorCode(env, obj, (jint)FTR_ERROR_NOT_ENOUGH_MEMORY);
        res = JNI_FALSE;
    }

    if( ansi_tmpl ) env->ReleaseByteArrayElements( pAnsiTmpl, ansi_tmpl, 0 );
    if( iso_tmpl )env->ReleaseByteArrayElements( pIsoTmpl, iso_tmpl, 0 );
    if( iso_size )env->ReleaseIntArrayElements( pIsoTmplSize, iso_size, 0 );

    return res;
}

jint JNICALL Java_com_futronictech_AnsiSDKLib_GetMaxTemplateSize()
{
    return ftrAnsiSdkGetMaxTemplateSize();
}

