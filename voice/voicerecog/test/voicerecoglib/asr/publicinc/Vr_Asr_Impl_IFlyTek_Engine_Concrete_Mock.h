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
 * @file Voice_Asr_Impl_Engine_Concrete_Mock.h
 * @brief C_Engine_Concrete_Mock class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_IFlyTek_Engine_Concrete_Mock_h__
#define __Vr_Asr_Impl_IFlyTek_Engine_Concrete_Mock_h__

#include "Vr_Asr_Impl_IFlyTek_Engine_Concrete.h"

namespace N_Vr
{

namespace N_Asr
{

class C_Network_Interface;

namespace N_Impl
{

class C_Engine_Agent;

namespace N_IFlyTek
{
/**
 * @brief The C_Engine_Concrete_Mock class
 *
 * C_Engine_Concrete_Mock class
 */

class C_Engine_Concrete_Mock : public C_Engine_Concrete
{
  public:
    C_Engine_Concrete_Mock
    (
        C_Engine_Agent & ro_Engine_Agent,
        string const & rstring_Id_Language,
        C_Network_Interface & ro_Network_Interface
    ) : C_Engine_Concrete(ro_Engine_Agent, rstring_Id_Language, ro_Network_Interface)
    {
    }

    MOCK_METHOD0(M_Fn_Init, E_Result());
    MOCK_METHOD0(M_Fn_Fina, E_Result());
    MOCK_METHOD1(M_Fn_Party_Load, E_Result(string const & rstring_Id_Party));
    MOCK_METHOD1(M_Fn_Party_Unload, E_Result(string const & rstring_Id_Party));
    MOCK_METHOD1(M_Fn_Param_Set, E_Result(C_Request_Param_Set & ro_Request_Param_Set));
    MOCK_METHOD1(M_Fn_Data_Context_List_Update, E_Result(C_Request_Context_List_Update & ro_Request_Context_List_Update));

    MOCK_METHOD1(M_Fn_Rec_Start, E_Result(C_Request_Recognize & ro_Request_Recognize));
    MOCK_METHOD0(M_Fn_Rec_Cancel, E_Result());
    MOCK_METHOD3(M_Fn_Append_Audio_Data, E_Result(short* pi_Audio_Buffer, unsigned int i_Audio_Frames, unsigned int& i_Write_Audio_Frames));
    MOCK_METHOD3(M_Fn_On_Callback_Msg, E_Result(unsigned int i_Msg, unsigned int i_wParam, const void* pv_lParam));


    MOCK_METHOD1(_M_Fn_Proc_On_Result, void(const string& string_Result));
    MOCK_METHOD0(_M_Fn_Start_Audio, E_Result());
    MOCK_METHOD0(_M_Fn_Stop_Audio, E_Result());
    MOCK_METHOD0(_M_Fn_Mgr_Init, E_Result());
    MOCK_METHOD0(_M_Fn_Mgr_Fina, E_Result());
    MOCK_METHOD0(_M_Fn_IFlyTek_Data_Init, E_Result());
    MOCK_METHOD0(_M_Fn_IFlyTek_Data_Fina, E_Result());
    MOCK_METHOD0(_M_Fn_IFlyTek_Init, E_Result());
    MOCK_METHOD0(_M_Fn_IFlyTek_Fina, E_Result());
    MOCK_METHOD0(_M_Fn_On_Rec_End, void());

    MOCK_METHOD2(_M_Fn_Get_IFlyTek_Mode, E_Result(
        const string& string_mode,
        ISS_SR_MODE& o_ISS_SR_MODE
    ));

    MOCK_METHOD2(_M_Fn_Get_IFlyTek_Sence, E_Result(
        const string& string_sence,
        ISS_SR_SCENE& o_ISS_SR_SCENE
    ));

    MOCK_METHOD2(_M_Fn_Get_IFlyTek_Language, E_Result(
        const string& string_language,
        ISS_SR_ACOUS_LANG& o_ISS_SR_ACOUS_LANG
    ));

    MOCK_METHOD0(_M_Fn_Info_Parse_File, E_Result());

    MOCK_METHOD4(_M_Fn_Info_Parse_Line, E_Result
    (
        ifstream & rifstream_File,
        string & rstring_Key,
        string & rstring_Value,
        bool & rb_Eof
    ));

    MOCK_METHOD1(_M_Fn_Info_Parse_Dialog, E_Result(
        ifstream & rifstream_File
    ));

    MOCK_METHOD2(_M_Fn_Format_Json_Dictionary, E_Result(
        C_Request_Context_List_Update & ro_Request_Context_List_Update,
        string & ro_string_Json_Dictionary
    ));

    MOCK_METHOD2(_M_Fn_Format_Json_Local_Cmd, E_Result(
        list<shared_ptr<C_Term> > & ro_list_spo_Term_Local_Cmd,
        string & ro_string_Json_Local_Cmd
    ));

    MOCK_METHOD3(_M_Fn_Get_Dic_Name_and_Src, void(
        string const & ro_string_Id_Conext,
        string & ro_string_Dic_Name,
        string & ro_string_Dic_Src
    ));

    MOCK_METHOD3(_M_Fn_Get_Json_Content, void(
        list<shared_ptr<C_Term> > & ro_list_spo_Term,
        rapidjson::GenericValue<rapidjson::UTF8<> > & ro_Dic_Content,
        rapidjson::GenericDocument<rapidjson::UTF8<> >::AllocatorType & ro_Allocator
    ));

};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
