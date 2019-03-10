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
 * @file Vr_Asr_Def.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Def_h__
#define __Vr_Asr_Def_h__

#include <string>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_array.hpp>

#ifdef __GNUC__
# define ASR_API __attribute__((visibility("default")))
#else
# define ASR_API
#endif

#if defined(_WIN32) || defined(_WIN32_WCE)

#include <set>
#include <map>
#include <list>
#include <vector>

#define N_STD_WIN std
using N_STD_WIN::set;
using N_STD_WIN::map;
using N_STD_WIN::list;
using N_STD_WIN::vector;
#undef N_STD_WIN

#else

#include "MEM_set.h"
#include "MEM_map.h"
#include "MEM_list.h"
#include "MEM_vector.h"

/**
 * @brief The set class
 *
 * set class
 */
template <typename C_T>
class set : public VoiceSet<C_T>::type
{
};

/**
 * @brief The map class
 *
 * map class
 */
template<typename C_K, typename C_V>
class map : public VoiceMap<C_K, C_V>::type
{
};

/**
 * @brief The list class
 *
 * list class
 */
template <typename C_T>
class list : public VoiceList<C_T>::type
{
};

/**
 * @brief The vector class
 *
 * vector class
 */
template <typename C_T>
class vector : public VoiceVector<C_T>::type
{
};

#endif


namespace N_Vr
{

namespace N_Asr
{

enum E_Result
{
    E_Result_Error       = -1, // Error.
    E_Result_Failed      =  0, // Failed.
    E_Result_Succeeded   =  1, // Succeeded.
};

enum E_Type_Engine
{
    E_Type_Engine_None = 0,
    E_Type_Engine_Vocon,
    E_Type_Engine_Vocon_Via_Shared,
    E_Type_Engine_iFlytek,
    E_Type_Engine_Kaldi,
};

enum E_Notify_Type
{
    E_Notify_Type_None = 0,
    E_Notify_Type_Param_Audio_Timeout,
    E_Notify_Type_Param_Fx_Timeout_Speech,
    E_Notify_Type_Param_Fx_Timeout_Silence_Leading,
    E_Notify_Type_Param_Fx_Timeout_Silence_Trailing,
    E_Notify_Type_Param_Fx_Sensitivity,
    E_Notify_Type_Param_Fx_Absolute_Threshold,
    E_Notify_Type_Param_Fx_Minspeech,
    E_Notify_Type_Param_Rec_Firstpass_Distapprox,
    E_Notify_Type_Param_Fx_Event_Timer_Interval_Millis,
    E_Notify_Type_Param_Longitude,
    E_Notify_Type_Param_Latitude,
    E_Notify_Type_Param_Local_City,
    E_Notify_Type_Param_Debug_Level,
    E_Notify_Type_Audio_Src,
    E_Notify_Type_Contact_Src,
    E_Notify_Type_Resource_Map,
    E_Notify_Type_Resource_Cmd,
    E_Notify_Type_Resource_Song,
    E_Notify_Type_Msg_Login,
    E_Notify_Type_Network_Change,
    E_Notify_Type_End,
};

/*
    Vocon Param:

    LH_FX_PARAM_START_ENABLE
    LH_FX_PARAM_SENSITIVITY
    LH_FX_PARAM_ABSOLUTE_THRESHOLD
    LH_FX_PARAM_MINSPEECH
    LH_FX_PARAM_FARTALK
    LH_FX_PARAM_AGC
    LH_FX_PARAM_TIMEOUT_LSILENCE
    LH_FX_PARAM_TIMEOUT_SPEECH
    LH_FX_PARAM_TSILENCE
    LH_FX_PARAM_FIX_WARPFACTOR
    LH_FX_PARAM_ADAPTATION
    LH_FX_PARAM_EVENT_TIMER
    LH_FX_PARAM_KNOWN_SPEAKER_CHANGES
    LH_FX_PARAM_ENABLEFETCHPCMAUDIO
    LH_FX_PARAM_AUTOMATIC_RESTART

    LH_REC_PARAM_AUTOMATIC_RESTART
    LH_REC_PARAM_EXTRAEVENT_ENABLE
    LH_REC_PARAM_GARBAGE
    LH_REC_PARAM_FIRSTPASS_DISTAPPROX
    LH_REC_PARAM_RESULTSTRATEGY
    LH_REC_PARAM_SPEAKER_ADAPTATION

    LH_CTX_PARAM_ACCURACY
    LH_CTX_PARAM_MAXNBEST
    LH_CTX_PARAM_MAXNBEST_SECONDPASS
    LH_CTX_PARAM_TSILENCE_FX
    LH_CTX_PARAM_TSILENCE
    LH_CTX_PARAM_INITBEAMWIDTH
    LH_CTX_PARAM_WORDPENALTY
    LH_CTX_PARAM_DECODING_STRATEGY
    LH_CTX_PARAM_IG_LOWCONF
    LH_CTX_PARAM_IG_HIGHCONF

    LH_AUDIOSOURCE_PARAM_TIMEOUT
*/

enum E_Param_Type
{
    E_Param_Type_None = 0,
    E_Param_Type_Fx_Sensitivity,
    E_Param_Type_Fx_Absolute_Threshold,
    E_Param_Type_Fx_Minspeech,
    E_Param_Type_Fx_Timeout_Silence_Leading,
    E_Param_Type_Fx_Timeout_Speech,
    E_Param_Type_Fx_Timeout_Silence_Trailing,
    E_Param_Type_Fx_Event_Timer_Interval_Millis,
    E_Param_Type_Rec_Garbage,
    E_Param_Type_Rec_Firstpass_Distapprox,
    E_Param_Type_Audio_Timeout,
    E_Param_Type_Longitude,
    E_Param_Type_Latitude,
    E_Param_Type_Local_City,
    E_Param_Type_Debug_Level,
    E_Param_Type_Audio_Src,
    E_Param_Type_Contact_Src,
    E_Param_Type_Resource_Map,
    E_Param_Type_Resource_Cmd,
    E_Param_Type_Resource_Song,
    E_Param_Type_Msg_Login,
    E_Param_Type_Network_Change,
    E_Param_Type_Center_Vr_Url,
    E_Param_Type_Acc_Off,
    E_Param_Type_End,
};

enum E_Data_Request_Type
{
    E_Data_Request_Type_None = 0,
    E_Data_Request_Type_Context_List_Update,
    E_Data_Request_Type_Context_List_File_Save,
    E_Data_Request_Type_Context_List_File_Delete,
    E_Data_Request_Type_Transcription_Id_Version_Update,
    E_Data_Request_Type_End,
};

enum E_Param_Value_Default
{
    E_Param_Value_Default_Fx_Sensitivity = 50,
    E_Param_Value_Default_Fx_Absolute_Threshold = -6000,
    E_Param_Value_Default_Fx_Minspeech = 60,
    E_Param_Value_Default_Fx_Timeout_Silence_Leading = 5500,
    E_Param_Value_Default_Fx_Timeout_Speech = 14000,
    E_Param_Value_Default_Fx_Timeout_Silence_Trailing = 700,
    E_Param_Value_Default_Fx_FuzzyCtx_TSilence = 800,
    E_Param_Value_Default_Fx_Event_Timer_Interval_Millis = 50,
    E_Param_Value_Default_Rec_Garbage = 50,
    E_Param_Value_Default_Rec_Firstpass_Distapprox = 7000,
    E_Param_Value_Default_Audio_Timeout = 14000,
    E_Param_Value_Default_TableCtx_Max_Collapsed_List_Size = 10,
    E_Param_Value_Default_TableCtx_Nbr_Uncollapsed_Entries = 3,
};

// nuance fuzzy context compile
#define  FUZZYMATCHCTXCPL_COLUMN_SPEAKABLE   0x01 
#define  FUZZYMATCHCTXCPL_COLUMN_ACTIVATABLE   0x02 
#define  FUZZYMATCHCTXCPL_COLUMN_INTEGER   0x04 
#define  FUZZYMATCHCTXCPL_COLUMN_ARRAY   0x08 
#define  FUZZYMATCHCTXCPL_COLUMN_RESULTACCUMULATE   0x10 
#define  FUZZYMATCHCTXCPL_COLUMN_INTEGER64   0x20 
#define  FUZZYMATCHCTXCPL_COLUMN_UNIQUEVALUES   0x40 

#if defined(_WIN32) || defined(_WIN32_WCE)

#else
// iflytek language
ASR_API extern std::string const g_string_Iss_Sr_Acous_Lang_Mandarin; // "mandarin"
ASR_API extern std::string const g_string_Iss_Sr_Acous_Lang_English; // "english"
ASR_API extern std::string const g_string_Iss_Sr_Acous_Lang_Cantonese; // "cantonese"
   
// iflytek mode
ASR_API extern std::string const g_string_Iss_Sr_Mode_Cloud; // "cloud"
ASR_API extern std::string const g_string_Iss_Sr_Mode_Local; // "local"
ASR_API extern std::string const g_string_Iss_Sr_Mode_Mix; // "mix"
#endif



} // name N_Asr

} // name N_Vr

#endif
/* EOF */
