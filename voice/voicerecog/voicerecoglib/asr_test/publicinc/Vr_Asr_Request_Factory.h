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
 * @file Voice_Asr_Request_Factory.h
 * @brief C_Request_Factory class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Request_Factory_h__
#define __Vr_Asr_Request_Factory_h__

#include "Vr_Asr_Event_Init_Notify.h"

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Request_Factory class
 *
 * C_Request_Factory class
 */

class C_Request_Factory
{
  public:

    E_Type_Engine m_e_Type_Engine;

    std::string m_string_Id_Language;

    boost::function<void(C_Event_Init_Notify const &)> m_function_On_Event_Init_Notify;


    // For Vocon Via Shared.

    void * m_po_Vocon_LH_COMPONENT_Asr;
    void * m_po_Vocon_LH_OBJECT_Mod_Ac;

    // For iFlyTek
    std::string m_string_Center_Vr_Url;

    ~C_Request_Factory() // [F]Destructor
    {
    }

    explicit
    C_Request_Factory() // [F]Constructor of Default
    : m_e_Type_Engine(E_Type_Engine_None)
    , m_string_Id_Language()
    , m_function_On_Event_Init_Notify()
    , m_po_Vocon_LH_COMPONENT_Asr(NULL)
    , m_po_Vocon_LH_OBJECT_Mod_Ac(NULL)
    , m_string_Center_Vr_Url()
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