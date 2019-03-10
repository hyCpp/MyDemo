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
 * @file Voice_Asr_Impl_Engine_Agent.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Engine_Agent_h__
#define __Vr_Asr_Impl_Engine_Agent_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

class C_Engine_Base;

/**
 * @brief The C_Engine_Agent class
 *
 * C_Engine_Agent class
 */
class C_Engine_Agent : public C_Engine
{
  public:

    E_Type_Engine m_e_Type_Engine;

    set<string> m_set_string_Id_Party;

    boost::function<void(C_Event_Init_Notify const &)> m_function_On_Event_Init_Notify;

    scoped_ptr<C_Engine_Base> m_scpo_Engine_Base;

    // For Vocon Via Shared.

    void * m_po_Vocon_LH_COMPONENT_Asr;
    void * m_po_Vocon_LH_OBJECT_Mod_Ac;

    // For iFlyTek
    std::string m_string_Center_Vr_Url;

    virtual
    ~C_Engine_Agent(); // [F]Destructor

    explicit
    C_Engine_Agent(); // [F]Constructor of Default

    virtual
    E_Result // [R] True for success, false for failed.
    Language_Change // [F] Set language.
    (
        string const & rstring_Id_Language // [I] Language id.
    );

    virtual
    E_Result // [R] True for success, false for failed.
    Language_Change // [F] Change Language
    (
        C_Request_Language_Change const & ro_Request_Language_Change
    );

    virtual
    E_Result // [R] True for success, false for failed.
    Party_Load // [F] Load information.
    (
        string const & rstring_Id_Party // [I] Party id.
    );

    virtual
    E_Result // [R] True for success, false for failed.
    Party_Unload // [F] Load information.
    (
        string const & rstring_Id_Party // [I] Party id.
    );

    virtual
    E_Result // [R] True for success, false for failed.
    Param_Set // [F] Set Param.
    (
        C_Request_Param_Set & ro_Request_Param_Set // [I] Request.
    );

    virtual
    E_Result // [R] True for success, false for failed.
    Context_List_Update_Sync // [F] Update List Context.
    (
        C_Request_Context_List_Update & ro_Request_Context_List_Update // [I] Request.
    );

    virtual
    E_Result // [R] True for success, false for failed.
    Context_List_Update // [F] Update List Context.
    (
        C_Request_Context_List_Update & ro_Request_Context_List_Update // [I] Request.
    );

#if 0
    virtual
    E_Result // [R] True for success, false for failed.
    Context_Fuzzy_Compile // [F] Update List Context.
    (
        C_Request_Context_Fuzzy_Compile & ro_Request_Context_Fuzzy_Compile // [I] Request.
    );
#endif

    virtual
    E_Result // [R] True for success, false for failed.
    Recognize_Start_Sync // [F] Start VR.
    (
        C_Request_Recognize & ro_Request_Recognize // [I] Request.
    );

    virtual
    E_Result // [R] True for success, false for failed.
    Recognize_Start // [F] Start VR.
    (
        C_Request_Recognize & ro_Request_Recognize // [I] Request.
    );

    virtual
    E_Result // [R] True for success, false for failed.
    Recognize_Cancel(); // [F] Cancel VR.

    virtual
    E_Result
    Get_Transcriptions
    (
        C_Term& ro_Term
    );

  PROTECTED:

    E_Result
    _M_Fn_Language_Init
    (
        string const & rstring_Id_Language
    );

    E_Result
    _M_Fn_Language_Fina();

    void
    _M_Fn_On_Event_Phase
    (
        C_Event_Phase const & ro_Event_Phase
    );

    E_Result
    _M_Fn_Clear_User_Data();

    C_Engine_Agent // [F]Constructor: Copy
    (
        C_Engine_Agent const & // [I]
    );

    C_Engine_Agent & // [R]
    operator = // [F]Operator: =
    (
        C_Engine_Agent const & // [I]
    );

    C_Mutex m_o_Mutex_Interface;

    C_Semaphore m_o_Semaphore;

    boost::function<void(C_Event_Phase const &)> m_function_On_Event_Phase;
};

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
