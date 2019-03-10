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
 * @file Voice_Asr_Impl_Mgr_Rec.h
 * @brief C_Mgr_Rec class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Mgr_Rec_h__
#define __Vr_Asr_Impl_Mgr_Rec_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

class C_Engine_Agent;

namespace N_Vocon
{

class C_Result_Hypothesis;
class C_Data_Node_Result;
class C_Data_Party;
class C_Thread_Rec;
class C_Engine_Concrete;
class C_Result_Item;

/**
 * @brief The C_Mgr_Rec class
 *
 * C_Mgr_Rec class
 */

class C_Mgr_Rec
{
  public:

    enum E_State_Rec
    {
        E_State_Rec_Run_Req,
        E_State_Rec_Run_Ack,
        E_State_Rec_Idle_Req,
        E_State_Rec_Idle_Ack,
    };

    C_Engine_Concrete & m_ro_Engine_Concrete;

    ~C_Mgr_Rec(); // [F]Destructor

    explicit
    C_Mgr_Rec // [F]Constructor of Default
    (
        C_Engine_Concrete & ro_Engine_Concrete
    );

    E_Result
    M_Fn_Init();

    E_Result
    M_Fn_Fina();

    E_Result
    M_Fn_Start
    (
        C_Request_Recognize & ro_Request_Recognize
    );

    E_Result
    M_Fn_Cancel();

    void
    M_Fn_On_Thread_Notified();

    E_Result
    M_Fn_On_Callback_Advance
    (
        LH_BOOL * po_LH_BOOL_Continue
    );

    E_Result
    M_Fn_On_Callback_Event
    (
        unsigned long i_Type,
        LH_TIME o_LH_TIME
    );

  PROTECTED:

    E_Result
    _M_Fn_Thread_Init();

    E_Result
    _M_Fn_Thread_Fina();

    E_Result
    _M_Fn_Proc();

    E_Result
    _M_Fn_Proc_Rec();

    E_Result
    _M_Fn_Proc_On_Result();

    E_Result
    _M_Fn_Proc_On_Result_Userword
    (
        LH_OBJECT & ro_LH_OBJECT_Result
    );

    E_Result
    _M_Fn_Proc_On_Result_Userword_Check
    (
        LH_OBJECT & ro_LH_OBJECT_Result,
        bool & rb_Voicetag_Alike
    );

    E_Result
    _M_Fn_Proc_On_Result_Userword_Deal();

    E_Result
    _M_Fn_Proc_On_Result_Userword_Transcription_Audio
    (
        LH_OBJECT & ro_LH_OBJECT_Result
    );

    E_Result
    _M_Fn_Proc_On_Result_Json_NBest
    (
        LH_OBJECT & ro_LH_OBJECT_Result
    );

    E_Result
    _M_Fn_Proc_On_Result_Json_NBest_Hypothesis
    (
        LH_OBJECT o_LH_OBJECT_Cursor,
        size_t i_Hypothesis_Index
    );

    E_Result
    _M_Fn_Proc_On_Result_Json_NBest_Array
    (
        LH_OBJECT & ro_LH_OBJECT_Cursor,
        shared_ptr<C_Result_Hypothesis>& rspo_Result_Hypothesis,
        list<C_Result_Item *>& ro_list_po_Result_Item_Stack,
        string& ro_string_Indent,
        unsigned int& ri_Item_Index
    );

    E_Result
    _M_Fn_Proc_On_Result_Json_NBest_Object
    (
        LH_OBJECT & ro_LH_OBJECT_Cursor,
        shared_ptr<C_Result_Hypothesis>& rspo_Result_Hypothesis,
        list<C_Result_Item *>& ro_list_po_Result_Item_Stack,
        string& ro_string_Indent,
        unsigned int& ri_Item_Index,
        int& rb_Tag
    );

    E_Result
    _M_Fn_Proc_On_Result_NBest_Hypothesis_XML
    (
        C_Result_Hypothesis & ro_Result_Hypothesis,
        pugi::xml_node & o_RootNode
    );

    E_Result
    _M_Fn_Proc_On_Result_NBest_Hypothesis_XML_Nodes
    (
        C_Result_Hypothesis & ro_Result_Hypothesis,
        list<shared_ptr<C_Data_Node_Result> > & rlist_spo_Data_Node_Result,
        pugi::xml_node & o_RootNode
    );
    
    string
    _M_Fn_Get_string_Eval_Result
    (
        string const& ro_string_Express
    );

    string
    _M_Fn_Calculate_Expression
    (
        string const& ro_string_Express
    );

    E_Result
    _M_Fn_Set_Obj_Param
    (
        LH_OBJECT& ro_LH_OBJECT,
        map<string, pair<unsigned long, long> >& ro_map_Param,
        map<string, pair<unsigned long, long> >& ro_map_Origin_Param
    );

    E_Result
    _M_Fn_Set_Fx_And_Rec_Param
    (
        LH_OBJECT& ro_LH_OBJECT_Fx,
        LH_OBJECT& ro_LH_OBJECT_Rec,
        map<string, pair<unsigned long, long> >& ro_map_Fx_Param,
        map<string, pair<unsigned long, long> >& ro_map_Origin_Fx_Param,
        map<string, pair<unsigned long, long> >& ro_map_Rec_Param,
        map<string, pair<unsigned long, long> >& ro_map_Origin_Rec_Param
    );

    C_Mgr_Rec // [F]Constructor: Copy
    (
        C_Mgr_Rec const & // [I]
    );

    C_Mgr_Rec & // [R]
    operator = // [F]Operator: =
    (
        C_Mgr_Rec const & // [I]
    );

    C_Data_Party * m_po_Data_Party_Using;

    shared_ptr<string> m_spstring_Result_XML;

    C_Buffer m_buffer_Userword_Transcription;
    C_Buffer m_buffer_Userword_Audio;

    C_Mutex m_o_Mutex_State;

    E_State_Rec m_e_State_Rec;

    scoped_ptr<C_Request_Recognize> m_scpo_Request_Recognize;

    scoped_ptr<C_Thread_Rec> m_scpo_Thread_Rec;

    vector<shared_ptr<C_Result_Hypothesis> > m_vector_spo_Result_Hypothesis;

    int m_b_VOD_BOS;
    int m_b_VOD_TSF;
    int m_b_VOD_TSR;
    unsigned int m_i_Rec_Cnt;

    int m_b_UserWord_Accepted;
    LH_OBJECT m_o_LH_OBJECT_UserWord_Result;

    C_Time m_o_Time_VOD_BOS;
    C_Time m_o_Time_VOD_TSF;
    C_Time m_o_Time_VOD_TSR;
    C_Time m_o_Time_Result;
    C_Time m_o_Time_Begin;
    C_Time m_o_Time_End;

    int m_i_Audio_level;
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */