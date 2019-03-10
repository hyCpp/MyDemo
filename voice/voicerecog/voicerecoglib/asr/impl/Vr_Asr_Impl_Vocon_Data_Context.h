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
 * @file Voice_Asr_Impl_Data_Context.h
 * @brief C_Data_Context class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Data_Context_h__
#define __Vr_Asr_Impl_Data_Context_h__

namespace N_Vr
{

namespace N_Asr
{

class C_Term;

namespace N_Impl
{

namespace N_Vocon
{

class C_Data_Party;
class C_Data_CLC;

/**
 * @brief The C_Data_Context class
 *
 * C_Data_Context class
 */

class C_Data_Context
{
  public:

    C_Data_Party & m_ro_Data_Party;

    LH_OBJECT m_o_LH_OBJECT_Context;

    virtual
    ~C_Data_Context(); // [F]Destructor

    explicit
    C_Data_Context // [F]Constructor of Default
    (
        C_Data_Party & ro_Data_Party,
        string const & rstring_Id_Context,
        string const & rstring_Type_Context,
        string const & rstring_Data_File_Context,
        list<pair<string, string> > & rlist_pair_string_Id_Slot_string_Id_Context
    );

    virtual
    int
    M_Fn_Is_Ready();

    virtual
    E_Result
    M_Fn_Init();

    virtual
    E_Result
    M_Fn_Fina();

    virtual
    E_Result
    M_Fn_Load
    (
        list<shared_ptr<C_Request_Activate> > & rlist_spo_Request_Activate
    );

    virtual
    E_Result
    M_Fn_Unload();

    virtual
    E_Result
    M_Fn_Slot_Guest_Load();

    virtual
    E_Result
    M_Fn_Slot_Guest_Unload();

    E_Result
    M_Fn_Merge_Host_Merge
    (
        C_Data_Context * po_Data_Context_Merge_Guest
    );

    E_Result
    M_Fn_Merge_Host_Clear();

    E_Result
    M_Fn_Rec_Add();

    E_Result
    M_Fn_Rec_Remove();

    E_Result
    M_Fn_List_Update_Begin();

    E_Result
    M_Fn_List_Update_Clear();

    E_Result
    M_Fn_List_Update_Delete(const C_Term& o_Term);

    E_Result
    M_Fn_List_Update_Add
    (
        const C_Term& o_Term,
        C_Data_CLC& ro_Data_CLC
    );

    E_Result
    M_Fn_List_Update_Commit();

    E_Result
    M_Fn_File_Save(int b_File_Write, unsigned int& ri_File_Size);

    E_Result
    M_Fn_File_Delete();

#if 0

    E_Result
    M_Fn_Fuzzy_Compile_Begin();

    E_Result
    M_Fn_Fuzzy_Compile_Add_Column
    (
        vector<shared_ptr<C_Term> > const & ro_vector_spo_Term
    );

    E_Result
    M_Fn_Fuzzy_Compile_Add_Entry
    (
        vector<shared_ptr<C_Term> > const & ro_vector_spo_Term
    );

    E_Result
    M_Fn_Fuzzy_Compile_Run();

    E_Result
    M_Fn_On_Callback_Fuzzy_Compile_Error
    (
        string const& ro_string_Error_Msg
    );

#endif

  PROTECTED:

    E_Result
    _M_Fn_Obj_Create_From_File();

    E_Result
    _M_Fn_Obj_Create_List();

    E_Result
    _M_Fn_Obj_Create_Userword();

    E_Result
    _M_Fn_Obj_Destroy();

    E_Result
    _M_Fn_Obj_Slot_Host_Add();

    E_Result
    _M_Fn_Obj_Slot_Host_Clear();

    E_Result
    _M_Fn_Obj_Activate_Grammar
    (
        list<shared_ptr<C_Request_Activate> > & rlist_spo_Request_Activate
    );

    E_Result
    _M_Fn_Get_Obj_Activatable_Rules
    (
        set<string>& ro_set_string_Activatable_Rules
    );

    E_Result
    _M_Fn_Obj_Activate_Field
    (
        list<shared_ptr<C_Request_Activate> > & rlist_spo_Request_Activate
    );

    E_Result
    _M_Fn_Obj_Activate_Fuzzy
    (
        list<shared_ptr<C_Request_Activate> > & rlist_spo_Request_Activate
    );

    E_Result
    _M_Fn_List_Update_Add_Transcriptions
    (
        const string& ro_string_Orthography,
        unsigned int i_User_Id_Hi,
        unsigned int i_User_Id_Lo,
        LH_TRANSCRIPTION * p_LH_TRANSCRIPTIONs,
        size_t i_LH_TRANSCRIPTIONs_count
    );

    E_Result
    _M_Fn_List_Update_Add_Userword_Transcriptions(const C_Term& o_Term);

    E_Result
    _M_Fn_List_Update_Add_Gracenote_And_Text_Transcriptions
    (
        const C_Term& o_Term,
        C_Data_CLC& ro_Data_CLC
    );

    C_Data_Context // [F]Constructor: Copy
    (
        C_Data_Context const & // [I]
    );

    C_Data_Context & // [R]
    operator = // [F]Operator: =
    (
        C_Data_Context const & // [I]
    );

    string m_string_Id_Context;
    string m_string_Type_Context;
    string m_string_Data_File_Context;
    string m_string_File_Context_Fuzzy_Tmp;
    list<pair<string, string> > m_list_pair_string_Id_Slot_string_Id_Context;

    int m_b_Obj_Created;
    int m_b_Obj_Merged;
    int m_b_Obj_Rec_Added;
    int m_b_Obj_List_Updating;
    int m_b_Obj_List_Save_File;
    int m_b_Slot_Added;

    vector<unsigned int> m_vector_i_Column_Id;
    vector<unsigned int> m_vector_i_Column_Attribute;
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
