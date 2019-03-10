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
 * @file Voice_Asr_Impl_Data_Party.h
 * @brief C_Data_Party class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Data_Party_h__
#define __Vr_Asr_Impl_Data_Party_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

class C_Data_Node_Result;
class C_Data_Tag;
class C_Data_Rule;
class C_Data_Context;
class C_Data_Context_In_Dialog;
class C_Data_Dialog;

class C_Engine_Concrete;

/**
 * @brief The C_Data_Party class
 *
 * C_Data_Party class
 */

class C_Data_Party
{
  public:

    C_Engine_Concrete & m_ro_Engine_Concrete;
    int m_b_Log_File;

    ~C_Data_Party(); // [F]Destructor

    explicit
    C_Data_Party // [F]Constructor of Default
    (
        C_Engine_Concrete & ro_Engine_Concrete,
        string const & rstring_Id_Party
    );

    E_Result
    M_Fn_Init();

    E_Result
    M_Fn_Fina();

    E_Result
    M_Fn_Dialog_Load
    (
        C_Request_Recognize & ro_Request_Recognize
    );

    E_Result
    M_Fn_Dialog_Unload
    (
        C_Request_Recognize & ro_Request_Recognize
    );

    C_Data_Tag *
    M_Fn_Find_po_Data_Tag
    (
        size_t i_Pos_Tag
    );

    C_Data_Rule *
    M_Fn_Find_po_Data_Rule
    (
        string const & rstring_Id_Rule
    );

    C_Data_Context *
    M_Fn_Find_po_Data_Context
    (
        string const & rstring_Id_Context
    );

    C_Data_Dialog *
    M_Fn_Find_po_Data_Dialog
    (
        string const & rstring_Id_Dialog
    );

    E_Result
    M_Fn_Get_Dialog_Fx_And_Rec_Param
    (
        C_Request_Recognize & ro_Request_Recognize,
        map<string, pair<unsigned long, long> >& ro_map_Fx_Param,
        map<string, pair<unsigned long, long> >& ro_map_Rec_Param
    );

  PROTECTED:

    E_Result
    _M_Fn_Info_Get_Path_Data_Party_Folder
    (
        string const & string_Path_PData_Folder,
        string const & string_Path_VData_Folder
    );

    E_Result
    _M_Fn_Info_Parse_File();

    E_Result
    _M_Fn_Info_Parse_Dialog
    (
        ifstream & rifstream_File // [I] File
    );

    E_Result
    _M_Fn_Info_Parse_Context_In_Dialog
    (
        ifstream & rifstream_File,
        C_Data_Context_In_Dialog & ro_Data_Foreign_Context
    );

    E_Result
    _M_Fn_Info_Parse_Context
    (
        ifstream & rifstream_File // [I] File
    );

    E_Result
    _M_Fn_Info_Parse_Slot_In_Context
    (
        ifstream & rifstream_File,
        pair<string, string> & rpair_string_Id_Slot_string_Id_Context
    );

    E_Result
    _M_Fn_Info_Parse_Rule
    (
        ifstream & rifstream_File // [I] File
    );

    E_Result
    _M_Fn_Info_Parse_Tag
    (
        ifstream & rifstream_File // [I] File
    );

    E_Result
    _M_Fn_Info_Parse_Node_Result
    (
        ifstream & rifstream_File, // [I] File
        C_Data_Node_Result & ro_Data_Node_Result
    );

    E_Result
    _M_Fn_Info_Parse_Line
    (
        ifstream & rifstream_File, // [I] File
        string & rstring_Key, // [O] Key
        string & rstring_Value, // [O] Value
        bool & rb_Eof // [O] End of file
    );

    E_Result
    _M_Fn_Info_Get_Version
    (
        string const & rstring_Path_File, // [I] File Path
        long & ri_Static_Version, // [O] Static Data Version
        long & ri_Navi_Version // [O] Navigation Data Version
    );

    E_Result
    _M_Fn_Info_Parse_Param_In_File
    (
        ifstream & rifstream_File,
        string& ro_string_End,
        map<string, pair<unsigned long, long> >& ro_map_Param
    );

    E_Result
    _M_Fn_Decode_Param_String
    (
        string& ro_string_Key,
        string& ro_string_Value,
        map<string, pair<unsigned long, long> >& ro_map_Param
    );

    C_Data_Party // [F]Constructor: Copy
    (
        C_Data_Party const & // [I]
    );

    C_Data_Party & // [R]
    operator = // [F]Operator: =
    (
        C_Data_Party const & // [I]
    );

    string m_string_Id_Party;
    string m_string_Path_Data_Party_Folder;
    string m_string_Path_Folder_Dynamic_Context;

    vector<shared_ptr<C_Data_Tag> > m_vector_spo_Data_Tag;
    map<string, shared_ptr<C_Data_Rule> > m_map_string_spo_Data_Rule;
    map<string, shared_ptr<C_Data_Context> > m_map_string_spo_Data_Context;
    map<string, shared_ptr<C_Data_Dialog> > m_map_string_spo_Data_Dialog;
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */