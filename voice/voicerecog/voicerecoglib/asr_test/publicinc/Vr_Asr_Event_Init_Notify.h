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
 * @file Vr_Asr_Event_Init_Notify.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Event_Init_Notify_h__
#define __Vr_Asr_Event_Init_Notify_h__

#include "Vr_Asr_Def.h"

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Event_Init_Notify class
 *
 * C_Event_Init_Notify class
 */
class ASR_API C_Event_Init_Notify
{
  public:

    enum E_Event_Init_Type
    {
        E_Event_Init_Notify_Type_None,
        E_Event_Init_Notify_Type_Context_Not_Exist,
        E_Event_Init_Notify_Type_Context_Invalid,
        E_Event_Init_Notify_Type_Map_Resource_Invalid,
        E_Event_Init_Notify_Type_Gracenote_Id_Supported,
    };

    virtual
    ~C_Event_Init_Notify(); // Destructor

    virtual
    E_Event_Init_Type // [R] Result Init Msg Type.
    Get_Event_Init_Notify_Type() const = 0; // [F] Get Init Msg Type.

    virtual
    std::string // [R] Result string param 
    Get_Event_Init_Notify_Param_String() const = 0; // [F] Get param string

  protected:

    explicit
    C_Event_Init_Notify(); // Constructor of Default

    C_Event_Init_Notify // Constructor: Copy
    (
        C_Event_Init_Notify const & // [I]
    );

    C_Event_Init_Notify & // [R]
    operator = // Operator: =
    (
        C_Event_Init_Notify const & // [I]
    );
};

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
