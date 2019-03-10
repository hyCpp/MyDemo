/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

/**
 * @file Voice_Asr_Impl_Vocon_Mock.h
 * @brief C_Vocon_Mock class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_IFlytek_IFlytek_Mock_h__
#define __Vr_Asr_Impl_IFlytek_IFlytek_Mock_h__

#include "Win_Linux_Interface.h"
#include "Vr_Asr_Impl_IFlytek_IFlytek.h"
#include "Vr_Asr_Impl_IFlytek_Callback_Mock.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

/**
 * @brief The C_Vocon_Mock class
 *
 * C_Vocon_Mock class
 */

class C_IFlyTek_Mock : public C_IFlyTek
{
public:

    enum E_Param
    {
        E_Param_Begin = 0,
        E_Param_ISSSRCreate_ERROR = 0,
        E_Param_ISSSRDestroy_ERROR,
        E_Param_ISSSRSetParam_ERROR,
        E_Param_ISSSRStart_ERROR,
        E_Param_ISSSRAppendAudioData_ERROR,
        E_Param_ISSSREndAudioData_ERROR,
        E_Param_ISSSRStop_ERROR,
        E_Param_ISSSRUpLoadDict_ERROR,
        E_Param_ISSSRLoadResource_ERROR,
        E_Param_ISSSRUnLoadResource_ERROR,
        E_Param_ISSSetMachineCode_ERROR,
        E_Param_ISSSRMspLogin_ERROR,
        E_Param_ISSSRSetGlobalCfg_ERROR,
        E_Param_ISSSRAccOff_ERROR,
        E_Param_ISS_SR_MSG_InitStatus_Fail,
        E_Param_ISS_SR_MSG_UpLoadDictToLocalStatus_Success,
        E_Param_ISS_SR_MSG_UpLoadDictToLocalStatus_Fail,
        E_Param_ISS_SR_MSG_UpLoadDictToCloudStatus_Success,
        E_Param_ISS_SR_MSG_VolumeLevel_Success,
        E_Param_ISS_SR_MSG_VolumeLevel_Fail,
        E_Param_ISS_SR_MSG_ResponseTimeout_Success,
        E_Param_ISS_SR_MSG_SpeechStart_Success,
        E_Param_ISS_SR_MSG_SpeechTimeOut_Success,
        E_Param_ISS_SR_MSG_SpeechEnd_Success,
        E_Param_ISS_SR_MSG_Error_Success,
        E_Param_ISS_SR_MSG_PreResult_Success,
        E_Param_ISS_SR_MSG_Result_Success,
        E_Param_ISS_SR_MSG_LoadBigSrResStatus_Success,
        E_Param_ISS_SR_MSG_LoadBigSrResStatus_Fail,
        E_Param_ISS_SR_MSG_ErrorDecodingAudio_Success,
        E_Param_ISS_SR_MSG_UserCmd_Resourcebroken_Success,
        E_Param_ISS_SR_MSG_Map_Resourcebroken_Success,
        E_Param_ISS_SR_MSG_WaitingForCloudResult,
        E_Param_ISS_SR_MSG_TooLoud_Success,
        E_Param_ISS_SR_MSG_TooLow_Success,
        E_Param_ISS_SR_MSG_Noisy_Success,
        E_Param_ISS_SR_MSG_Undefine,
        E_Param_End,
        E_Param_Size = E_Param_End - E_Param_Begin,
    };

    vector<int> vector_b_Param;
    Proc_OnMsgProc p_CallbakcMsgProc;
    void* p_UserData;
    C_Mutex m_o_Mutex_IFlyTek_Mock;
    scoped_ptr<C_IFlyTek_Callback_Mock> m_scpo_IFlyTek_Callback_Mock;

    virtual
    ~C_IFlyTek_Mock(); // [F]Destructor

    explicit
    C_IFlyTek_Mock(); // [F]Constructor of Default

    int
    M_Fn_Init();

    void
    M_Fn_Fina();

    void
    M_Fn_Callback_Msg(int i_type)
    {
        switch (i_type) {
        case 0: { // rec
            if (vector_b_Param[E_Param_ISS_SR_MSG_VolumeLevel_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_VolumeLevel, 10, NULL);
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_VolumeLevel_Fail]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_VolumeLevel, 1000, NULL);
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_ResponseTimeout_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_SpeechStart_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_SpeechStart, ISS_SUCCESS, NULL);
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_SpeechTimeOut_Success]) {
                Sleep_WinLinux(200);
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_SpeechTimeOut, ISS_SUCCESS, NULL);
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_SpeechEnd_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_SpeechEnd, ISS_SUCCESS, NULL);
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_Error_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_Error, ISS_SUCCESS, "error");
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_PreResult_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_PreResult, ISS_SUCCESS, "result");
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_Result_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_Result, ISS_SUCCESS, "result");
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_LoadBigSrResStatus_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_LoadBigSrResStatus, ISS_SUCCESS, NULL);
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_LoadBigSrResStatus_Fail]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_LoadBigSrResStatus, ISS_ERROR_FAIL, NULL);
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_ErrorDecodingAudio_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ErrorDecodingAudio, ISS_SUCCESS, NULL);
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_UserCmd_Resourcebroken_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_UserCmdResourceBroken, ISS_SUCCESS, "broken");
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_Map_Resourcebroken_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_MapResourceBroken, ISS_SUCCESS, "broken");
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_WaitingForCloudResult]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_WaitingForCloudResult, ISS_SUCCESS, "wait cloud result");
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_TooLoud_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_TooLoud, ISS_SUCCESS, NULL);
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_TooLow_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_TooLow, ISS_SUCCESS, NULL);
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_Noisy_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_Noisy, ISS_SUCCESS, NULL);
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }
            else  if (vector_b_Param[E_Param_ISS_SR_MSG_Undefine]) {
                p_CallbakcMsgProc(p_UserData, 300, ISS_SUCCESS, NULL);
            }
            else {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_ResponseTimeout, ISS_SUCCESS, NULL);
            }

            break;
        }
        case 1: {  // for list context

            if (vector_b_Param[E_Param_ISS_SR_MSG_UpLoadDictToLocalStatus_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_UpLoadDictToLocalStatus, ISS_SUCCESS, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_UpLoadDictToLocalStatus_Fail]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_UpLoadDictToLocalStatus, ISS_ERROR_FAIL, NULL);
            }
            else if (vector_b_Param[E_Param_ISS_SR_MSG_UpLoadDictToCloudStatus_Success]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_UpLoadDictToCloudStatus, ISS_SUCCESS, NULL);
            }
            else {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_UpLoadDictToCloudStatus, ISS_ERROR_FAIL, NULL);
            }

            break;
        }
        case 2: {
            if (vector_b_Param[E_Param_ISS_SR_MSG_InitStatus_Fail]) {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_InitStatus, ISS_ERROR_FAIL, NULL);
            }
            else {
                p_CallbakcMsgProc(p_UserData, ISS_SR_MSG_InitStatus, ISS_SUCCESS, NULL);
            }

            break;
        }
        default: {

            MACRO_Log_Error
        }
        }

        return;
    }

    virtual
    ISSErrID
    M_Fn_ISSSRCreate(
        HISSSR*           phISSSRObj,
        ISS_SR_ACOUS_LANG eAcousLang,
        const char*       szResourceDir,
        Proc_OnMsgProc     pfnOnMsgProc,
        void*              pUsrArg,
        ConnectFunc        pfnConnectFunc,
        DisconnectFunc     pfnDisconnectFunc,
        OnStatusChangedFunc*   ppfnOnStatusChanged
    )
    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_IFlyTek_Mock);
        
        if (vector_b_Param[E_Param_ISSSRCreate_ERROR]) {
            return ISS_ERROR_FAIL;
        }
        p_UserData = pUsrArg;
        p_CallbakcMsgProc = pfnOnMsgProc;

        if (m_scpo_IFlyTek_Callback_Mock) {
            m_scpo_IFlyTek_Callback_Mock->M_Fn_Callback_Msg(2);
        }

        return ISS_SUCCESS;
    }

    virtual
    ISSErrID
    M_Fn_ISSSRDestroy(
        HISSSR hISSSRObj
    )
    {
        if (vector_b_Param[E_Param_ISSSRDestroy_ERROR]) {
            return ISS_ERROR_FAIL;
        }

        return ISS_SUCCESS;
    }

    virtual
    ISSErrID
    M_Fn_ISSSRSetParam(
        HISSSR hISSSRObj,
        const char* szParam,
        const char* szParamValue
    )
    {
        if (vector_b_Param[E_Param_ISSSRSetParam_ERROR]) {
            return ISS_ERROR_FAIL;
        }

        return ISS_SUCCESS;
    }

    virtual
    ISSErrID
    M_Fn_ISSSRStart(
        HISSSR       hISSSRObj,
        ISS_SR_SCENE mScene,
        ISS_SR_MODE  mMode,
        const char*  szCmd,
        const char*  szCustom
    )
    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_IFlyTek_Mock);

        if (vector_b_Param[E_Param_ISSSRStart_ERROR]) {
            return ISS_ERROR_FAIL;
        }

        if (m_scpo_IFlyTek_Callback_Mock) {
            m_scpo_IFlyTek_Callback_Mock->M_Fn_Callback_Msg(0);
        }

        return ISS_SUCCESS;
    }

    virtual
    ISSErrID
    M_Fn_ISSSRAppendAudioData(
        HISSSR          hISSSRObj,
        const short*    pSamples,
        unsigned int    nNumberOfToWrite,
        unsigned int*   pNumberOfWritten
    )
    {
        if (vector_b_Param[E_Param_ISSSRAppendAudioData_ERROR]) {
            return ISS_ERROR_FAIL;
        }

        return ISS_SUCCESS;
    }

    virtual
    ISSErrID
    M_Fn_ISSSREndAudioData(
        HISSSR hISSSRObj
    )
    {
        if (vector_b_Param[E_Param_ISSSREndAudioData_ERROR]) {
            return ISS_ERROR_FAIL;
        }

        return ISS_SUCCESS;
    }

    virtual
    ISSErrID
    M_Fn_ISSSRStop(
        HISSSR hISSSRObj
    )
    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_IFlyTek_Mock);
        if (vector_b_Param[E_Param_ISSSRStop_ERROR]) {
            return ISS_ERROR_FAIL;
        }

        if (m_scpo_IFlyTek_Callback_Mock) {
            m_scpo_IFlyTek_Callback_Mock->M_Fn_Callback_Msg(3);
        }

        return ISS_SUCCESS;
    }

    virtual
    ISSErrID
    M_Fn_ISSSRUpLoadDict(
        HISSSR hISSSRObj,
        const char *szList
    )
    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_IFlyTek_Mock);
        if (vector_b_Param[E_Param_ISSSRUpLoadDict_ERROR]) {
            return ISS_ERROR_FAIL;
        }

        if (m_scpo_IFlyTek_Callback_Mock) {
            m_scpo_IFlyTek_Callback_Mock->M_Fn_Callback_Msg(1);
        }

        return ISS_SUCCESS;
    }

    virtual
    ISSErrID
    M_Fn_ISSSRUnLoadResource(
        HISSSR hISSSRObj,
        ISS_SR_RESOURCE_TYPE dataType
    )
    {
        if (vector_b_Param[E_Param_ISSSRUnLoadResource_ERROR]) {
            return ISS_ERROR_FAIL;
        }

        return ISS_SUCCESS;
    }

    virtual
    ISSErrID
    M_Fn_ISSSRLoadResource(
        HISSSR hISSSRObj,
        ISS_SR_RESOURCE_TYPE dataType
    )
    {
        if (vector_b_Param[E_Param_ISSSRLoadResource_ERROR]) {
            return ISS_ERROR_FAIL;
        }

        return ISS_SUCCESS;
    }

    virtual
    ISSErrID
    M_Fn_ISSSetMachineCode(
        const char* szMachineCode
    )
    {
        if (vector_b_Param[E_Param_ISSSetMachineCode_ERROR]) {
            return ISS_ERROR_FAIL;
        }

        return ISS_SUCCESS;
    }

    virtual
    ISSErrID 
    M_Fn_ISSSRMspLogin(
        HISSSR hISSSRObj
    )
    {
        if (vector_b_Param[E_Param_ISSSRMspLogin_ERROR]) {
            return ISS_ERROR_FAIL;
        }
        
        return ISS_SUCCESS;
    }

    virtual
    ISSErrID 
    M_Fn_ISSSRSetGlobalCfg(
        const char* szGlobalCfg
    )
    {
        if (vector_b_Param[E_Param_ISSSRSetGlobalCfg_ERROR]) {
            return ISS_ERROR_FAIL;
        }
        
        return ISS_SUCCESS;
    }

    virtual
    ISSErrID 
    M_Fn_ISSSRAccOff(
        HISSSR hISSSRObj
    )
    {
        if (vector_b_Param[E_Param_ISSSRAccOff_ERROR]) {
            return ISS_ERROR_FAIL;
        }

        return ISS_SUCCESS;
    }

protected:

    C_IFlyTek_Mock // [F]Constructor: Copy
    (
        C_IFlyTek_Mock const & // [I]
    );

    C_IFlyTek_Mock & // [R]
    operator = // [F]Operator: =
    (
        C_IFlyTek_Mock const & // [I]
    );
};

extern C_IFlyTek_Mock g_o_IFlyTek_Mock;

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
