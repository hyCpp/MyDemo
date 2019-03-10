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
 * @file Vr_Asr_Request_Language_Change.h
 * @brief C_Request_Language_Change class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Request_Language_Change_h__
#define __Vr_Asr_Request_Language_Change_h__


namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Request_Language_Change class
 *
 * C_Request_Language_Change class
 */

class C_Request_Language_Change
{
  public:

    std::string m_string_Id_Language;

    // For Vocon Via Shared.
    void * m_po_Vocon_LH_COMPONENT_Asr;
    void * m_po_Vocon_LH_OBJECT_Mod_Ac;

    // For iFlyTek
    boost::function<void(C_Event_Init_Notify const &)> m_function_On_Event_Init_Notify;

    ~C_Request_Language_Change() // [F]Destructor
    {
    }

    explicit
    C_Request_Language_Change() // [F]Constructor of Default
    : m_string_Id_Language()
    , m_po_Vocon_LH_COMPONENT_Asr()
    , m_po_Vocon_LH_OBJECT_Mod_Ac()
    , m_function_On_Event_Init_Notify()
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
