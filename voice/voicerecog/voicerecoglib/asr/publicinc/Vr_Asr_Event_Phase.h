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
 * @file Voice_Asr_EventPhase.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Event_Phase_h__
#define __Vr_Asr_Event_Phase_h__
#include "Vr_Asr_Def.h"
namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Event_Phase class
 *
 * C_Event_Phase class
 */
class ASR_API C_Event_Phase
{
  public:

    enum E_Event_Phase_Type
    {
        E_Event_Phase_Type_None,
        E_Event_Phase_Type_Proc_Begin,
        E_Event_Phase_Type_Proc_End,
        E_Event_Phase_Type_Proc_Fail,
        E_Event_Phase_Type_Listen_Begin,
        E_Event_Phase_Type_Listen_End,
        E_Event_Phase_Type_Speech_Begin_Fx,
        E_Event_Phase_Type_Speech_End_Fx,
        E_Event_Phase_Type_Speech_End_Rec,
        E_Event_Phase_Type_Response_Timeout,
        E_Event_Phase_Type_Speech_Timeout,
        E_Event_Phase_Type_Context_OK,
        E_Event_Phase_Type_Context_Error,
    };

    virtual
    ~C_Event_Phase(); // Destructor

    virtual
    E_Event_Phase_Type // [R] Result string XML.
    Get_Event_Phase_Type() const = 0; // [F] Get result.

  protected:

    explicit
    C_Event_Phase(); // Constructor of Default

    C_Event_Phase // Constructor: Copy
    (
        C_Event_Phase const & // [I]
    );

    C_Event_Phase & // [R]
    operator = // Operator: =
    (
        C_Event_Phase const & // [I]
    );
};

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
