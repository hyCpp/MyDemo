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
 * @file Voice_Asr_EventNotify.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Event_Notify_h__
#define __Vr_Asr_Event_Notify_h__

#include "Vr_Asr_Def.h"

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Event_Notify class
 *
 * C_Event_Notify class
 */
class ASR_API C_Event_Notify
{
  public:

    enum E_Event_Notify_Type
    {
        E_Event_Notify_Type_None,
        E_Event_Notify_Type_Bad_SNR,
        E_Event_Notify_Type_Overload,
        E_Event_Notify_Type_Too_Quiet,
        E_Event_Notify_Type_Too_Noisy,
        E_Event_Notify_Type_No_Signal,
        E_Event_Notify_Type_No_Leading_Silence,
        E_Event_Notify_Type_Poor_Mic,
        E_Event_Notify_Type_Abnormal_Unknown,
        E_Event_Notify_Type_Signal_Level,
        E_Event_Notify_Type_Vocon_SSE_Not_Converged,
        E_Event_Notify_Type_Vocon_SSE_Converged,
        E_Event_Notify_Type_Too_Short,
        E_Event_Notify_Type_Not_Trained,
        E_Event_Notify_Type_Voicetag_Alike,
        E_Event_Notify_Type_Voicetag_Not_Alike,
        E_Event_Notify_Type_Local_Dictionary_Upload_Error,
        E_Event_Notify_Type_Cloud_Dictionary_Upload_Error,
        E_Event_Notify_Type_Audio_Decode_Error,
        E_Event_Notify_Type_Network_Connect,
        E_Event_Notify_Type_Network_Disconnect,
        E_Event_Notify_Type_Context_List_File_Save_Failed,
        E_Event_Notify_Type_Context_List_File_Save_Finish,
        E_Event_Notify_Type_Context_List_File_Size,
        E_Event_Notify_Type_Context_List_File_Delete_Failed,
        E_Event_Notify_Type_Transcription_Id_Support,
        E_Event_Notify_Type_Transcription_Id_Not_Support,
    };

    virtual
    ~C_Event_Notify(); // Destructor

    virtual
    E_Event_Notify_Type // [R] Result Notify Type.
    Get_Event_Notify_Type() const // [F] Get result.
    {
        return E_Event_Notify_Type_None;
    }

    virtual
    int // [R] Result int Signal Levels
    Get_Signal_Level_Energy() const // [F] Get signal levels
    {
        return 0;
    }

    virtual
    int // [R] Result int Signal Levels
    Get_Signal_Level_SNR() const // [F] Get signal levels
    {
        return 0;
    }

    virtual
    unsigned int // [r] result unsigned int context file size
    Get_Context_File_Size() const // [f] get context file size
    {
        return 0;
    }

  protected:

    explicit
    C_Event_Notify(); // Constructor of Default

    C_Event_Notify // Constructor: Copy
    (
        C_Event_Notify const & // [I]
    );

    C_Event_Notify & // [R]
    operator = // Operator: =
    (
        C_Event_Notify const & // [I]
    );
};

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
