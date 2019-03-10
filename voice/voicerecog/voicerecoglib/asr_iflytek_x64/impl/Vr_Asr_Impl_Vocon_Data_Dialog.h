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
 * @file Voice_Asr_Impl_Data_Dialog.h
 * @brief C_Data_Dialog class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Data_Dialog_h__
#define __Vr_Asr_Impl_Data_Dialog_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

class C_Data_Party;
class C_Data_Context_In_Dialog;

/**
 * @brief The C_Data_Dialog class
 *
 * C_Data_Dialog class
 */

class C_Data_Dialog
{
  public:

    C_Data_Party & m_ro_Data_Party;

    ~C_Data_Dialog(); // [F]Destructor

    explicit
    C_Data_Dialog // [F]Constructor of Default
    (
        C_Data_Party & ro_Data_Party,
        string const & rstring_Id_Dialog,
        list<shared_ptr<C_Data_Context_In_Dialog> > & rlist_spo_Data_Context_In_Dialog,
        map<string, pair<unsigned long, long> >& m_map_Fx_Param,
        map<string, pair<unsigned long, long> >& m_map_Rec_Param
    );

    E_Result
    M_Fn_Load
    (
        C_Request_Recognize & ro_Request_Recognize
    );

    E_Result
    M_Fn_Unload();

    map<string, pair<unsigned long, long> >
    M_Fn_Get_Fx_Param();

    map<string, pair<unsigned long, long> >
    M_Fn_Get_Rec_Param();

  PROTECTED:

    C_Data_Dialog // [F]Constructor: Copy
    (
        C_Data_Dialog const & // [I]
    );

    C_Data_Dialog & // [R]
    operator = // [F]Operator: =
    (
        C_Data_Dialog const & // [I]
    );

    string m_string_Id_Dialog;

    list<shared_ptr<C_Data_Context_In_Dialog> > m_list_spo_Data_Context_In_Dialog;

    map<string, pair<unsigned long, long> > m_map_Fx_Param;
    map<string, pair<unsigned long, long> > m_map_Rec_Param;
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */