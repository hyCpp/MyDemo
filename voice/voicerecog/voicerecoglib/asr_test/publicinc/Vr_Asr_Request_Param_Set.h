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
 * @file Voice_Asr_Request_Param_Set.h
 * @brief C_Request_Param_Set class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Request_Param_Set_h__
#define __Vr_Asr_Request_Param_Set_h__

#include "Vr_Asr_Def.h"

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Request_Param_Set class
 *
 * C_Request_Param_Set class
 */

class ASR_API C_Request_Param_Set
{
  public:

    E_Param_Type m_e_Param_Type;
    int m_i_Value; // For Naunce: , For Iflytek: 0: UnLoadResource, 1: LoadResource
    std::string m_string_Value; // For IFlytek

    ~C_Request_Param_Set() // [F]Destructor
    {
    }

    explicit
    C_Request_Param_Set() // [F]Constructor of Default
    : m_e_Param_Type(E_Param_Type_None)
    , m_i_Value(0)
    , m_string_Value()
    {
    }

    // [F]Constructor: Copy
    //    = default

    // [F]Operator: =
    //    = default
};

} // name N_Asr

} // name N_Vr

#endif
/* EOF */