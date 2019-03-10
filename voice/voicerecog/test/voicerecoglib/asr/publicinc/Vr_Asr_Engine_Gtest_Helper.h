/**
 * Copyright @ 2013 - 2015 Suntec Software(Shanghai) Co., Ltd.
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
 * @file Vr_Asr_Engine_Gtest_Helper.h
 * @brief Interface file of Asr_Engine_Test_Helper.
 *
 * This file includes the interface of Asr_Engine_Test_Helper.
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef _VR_ASR_ENGINE_GTEST_HELPER_H_
#define _VR_ASR_ENGINE_GTEST_HELPER_H_

#include "File_Resolve.h"
#include "C_Engine_Test.h"

/**
* Asr_Engine_Test_Helper
*
* The class is just for Asr_Engine_Test_Helper.
*/
class Asr_Engine_Test_Helper
{
public:

    Asr_Engine_Test_Helper() {}
    virtual ~Asr_Engine_Test_Helper() {}

#ifndef MACRO_Dynamic_Debug

    bool Prepare_AudioIn();
    bool Prepare_Data();
    void Recognize_Loop(N_Vr::N_Asr::E_Type_Engine e_Type_Engine);

private:

    Asr_Engine_Test_Helper(const Asr_Engine_Test_Helper&);
    Asr_Engine_Test_Helper& operator=(const Asr_Engine_Test_Helper&);

    boost::shared_ptr<N_Vr::N_Asr::C_Audio_In> m_o_Audio_In;
    std::set<std::string> m_set_dialog_id;
    std::map<std::string, C_List_Context_Item> m_piar_list_context;

    C_Engine_Test m_o_Engine_Test;
    scoped_ptr<N_Vr::N_Asr::C_Engine> m_scpo_Engine;

    void _Mock_Recognize_Engine(N_Vr::N_Asr::E_Type_Engine e_Type_Engine, int i_param_index, int i_loop_index);
    void _Context_List_Update();
    void _Request_Recognize(string string_Id_Dialog = "topmenu_idle", bool b_Barge_In = false);
    void _Do_One_Test(bool b_use_real_engine, N_Vr::N_Asr::E_Type_Engine e_Type_Engine, N_Vr::N_Asr::C_Request_Factory& o_Request_Factory, int i_param_index, int i_loop_index);

    void
    M_Fn_Log
    (
        string const & string_Log
    );

    void
    M_Fn_Vocon_Test
    (
        N_Vr::N_Asr::E_Type_Engine e_Type_Engine
    );

    void
    M_Fn_Vocon_Test_Once
    (
        N_Vr::N_Asr::E_Type_Engine e_Type_Engine
    );

    void
    M_Fn_Vocon_Test_Etc();

    void
    M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Data_Context();

    void
    M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Mgr_Rec();

    void
    M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Data_Party();

    void
    M_Fn_Vocon_Test_Fn_Get_ro_LH_AUDIOIN_INTERFACE();

    void
    M_Fn_Vocon_Test_Fn_Get_ro_LH_HEAP_INTERFACE();

    void
    M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Data_CLC();

    void
    M_Fn_Vocon_Test_Vr_Asr_Impl_Vocon_Fn_Callback_LH_Error();
#endif
};

#endif // _VR_ASR_ENGINE_GTEST_HELPER_H_
/* EOF */
