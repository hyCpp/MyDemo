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

#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"

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
    int
    M_Fn_QISRBuildGrammar
    (
        const char* grammarType,
        const char* grammarContent,
        unsigned int grammarLength,
        const char* params,
        GrammarCallBack callback,
        void* userData
    )
    {
        return QISRBuildGrammar(grammarType, grammarContent, grammarLength, params, callback, userData);
    }

    virtual
    int
    M_Fn_QISRUpdateLexicon
    (
        const char *lexiconName,
        const char *lexiconContent,
        unsigned int lexiconLength,
        const char *params,
        LexiconCallBack callback,
        void *userData
    )
    {
        return QISRUpdateLexicon(lexiconName, lexiconContent, lexiconLength, params, callback, userData);
    }

    virtual
    const char*
    M_Fn_QISRSessionBegin
    (
        const char* grammarList,
        const char* params,
        int* errorCode
    )
    {
        return QISRSessionBegin(grammarList, params, errorCode);
    }

    virtual
    int
    M_Fn_QISRSessionEnd
    (
        const char* sessionID,
        const char* hints
    )
    {
        return QISRSessionEnd(sessionID, hints);
    }

    virtual
    int
    M_Fn_QISRAudioWrite
    (
        const char* sessionID,
        const void* waveData,
        unsigned int waveLen,
        int audioStatus,
        int *epStatus,
        int *recogStatus
    )
    {
        return QISRAudioWrite(sessionID, waveData, waveLen, audioStatus, epStatus, recogStatus);
    }

    virtual
    const char*
    M_Fn_QISRGetResult
    (
        const char* sessionID,
        int* rsltStatus,
        int waitTime,
        int *errorCode
    )
    {
        return QISRGetResult(sessionID, rsltStatus, waitTime, errorCode);
    }

    virtual
    int
    M_Fn_MSPLogin
    (
        const char* usr,
        const char* pwd,
        const char* params
    )
    {
        return MSPLogin(usr, pwd, params);
    }

    virtual
    int
    M_Fn_MSPLogout()
    {
        return MSPLogout();
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
