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
 * @file Voice_Asr_Impl_Vocon.h
 * @brief C_Vocon class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_IFlytek_h__
#define __Vr_Asr_Impl_IFlytek_h__

#ifdef _IFLYTEK

#include "iss_sr.h"
#include "iss_auth.h"
#include "iss_errors.h"
#include "iss_types.h"

#endif

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

/**
 * @brief The C_Vocon class
 *
 * C_IFlytek class
 */

class C_IFlyTek
{
  public:

    virtual
    ~C_IFlyTek(); // [F]Destructor

    explicit
    C_IFlyTek(); // [F]Constructor of Default

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
        return ISSSRCreate(phISSSRObj, eAcousLang, szResourceDir, 
            pfnOnMsgProc, pUsrArg, pfnConnectFunc, 
            pfnDisconnectFunc, ppfnOnStatusChanged);
    }

    virtual
    ISSErrID
    M_Fn_ISSSRDestroy(
        HISSSR hISSSRObj
    )
    {
        return ISSSRDestroy(hISSSRObj);
    }

    virtual
    ISSErrID
    M_Fn_ISSSRSetParam(
        HISSSR hISSSRObj,
        const char* szParam,
        const char* szParamValue
    )
    {
        return ISSSRSetParam(hISSSRObj, szParam, szParamValue);
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
        return ISSSRStart(hISSSRObj, mScene, mMode, szCmd, szCustom);
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
        return ISSSRAppendAudioData(hISSSRObj, pSamples, nNumberOfToWrite, pNumberOfWritten);
    }
    
    virtual
    ISSErrID
    M_Fn_ISSSREndAudioData(
        HISSSR hISSSRObj
    )
    {
        return ISSSREndAudioData(hISSSRObj);
    }

    virtual
    ISSErrID
    M_Fn_ISSSRStop(
        HISSSR hISSSRObj
    )
    {
        return ISSSRStop(hISSSRObj);
    }

    virtual
    ISSErrID
    M_Fn_ISSSRUpLoadDict(
        HISSSR hISSSRObj,
        const char *szList
    )
    {
        return ISSSRUpLoadDict(hISSSRObj, szList);
    }

    virtual
    ISSErrID
    M_Fn_ISSSRUnLoadResource(
        HISSSR hISSSRObj,
        ISS_SR_RESOURCE_TYPE dataType
    )
    {
        return ISSSRUnLoadResource(hISSSRObj, dataType);
    }

    virtual
    ISSErrID
    M_Fn_ISSSRLoadResource(
        HISSSR hISSSRObj,
        ISS_SR_RESOURCE_TYPE dataType
    )
    {
        return ISSSRLoadResource(hISSSRObj, dataType);
    }

    virtual
    ISSErrID 
    M_Fn_ISSSetMachineCode(
        const char* szMachineCode
    )
    {
        return ISSSetMachineCode(szMachineCode);
    }

    virtual
    ISSErrID 
    M_Fn_ISSSRMspLogin(
        HISSSR hISSSRObj
    )
    {
        return ISSSRMspLogin(hISSSRObj);
    }

    virtual
    ISSErrID 
    M_Fn_ISSSRSetGlobalCfg(
        const char* szGlobalCfg
    )
    {
        return ISSSRSetGlobalCfg(szGlobalCfg);
    }

    virtual
    ISSErrID 
    M_Fn_ISSSRAccOff(
        HISSSR hISSSRObj
    )
    {
        return ISSSRAccOff(hISSSRObj);
    }

  protected:

    C_IFlyTek // [F]Constructor: Copy
    (
        C_IFlyTek const & // [I]
    );

    C_IFlyTek & // [R]
    operator = // [F]Operator: =
    (
        C_IFlyTek const & // [I]
    );

};

extern scoped_ptr<C_IFlyTek> g_scp_o_IFlyTek;

inline
C_IFlyTek &
G_o_IFlyTek()
{
    return *g_scp_o_IFlyTek;
}

} // name N_IFlytek

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */