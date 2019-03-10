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

#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Impl_Fn_string.h"
#include "Vr_Asr_Impl_Fn_Get_string_Line_Fixed.h"
#include "Vr_Asr_Impl_Fn_Get_string_Path.h"
#include "Vr_Asr_Impl_Fn_Dir_File.h"
#include "Vr_Asr_Impl_Vocon_Data_Node_Result.h"
#include "Vr_Asr_Impl_Vocon_Data_Tag.h"
#include "Vr_Asr_Impl_Vocon_Data_Rule.h"
#include "Vr_Asr_Impl_Vocon_Data_Context_List.h"
#include "Vr_Asr_Impl_Vocon_Data_Context_Grammar.h"
#include "Vr_Asr_Impl_Vocon_Data_Context_Field.h"
#include "Vr_Asr_Impl_Vocon_Data_Context_Spelling.h"
#include "Vr_Asr_Impl_Vocon_Data_Context_Userword.h"
#include "Vr_Asr_Impl_Vocon_Data_Context_Fuzzy_Host.h"
#include "Vr_Asr_Impl_Vocon_Data_Context_Fuzzy.h"
#include "Vr_Asr_Impl_Vocon_Data_Context.h"
#include "Vr_Asr_Impl_Vocon_Data_Context_In_Dialog.h"
#include "Vr_Asr_Impl_Vocon_Data_Dialog.h"
#include "Vr_Asr_Impl_Vocon_Data_Party.h"
#include "Vr_Asr_Impl_Vocon_Engine_Concrete.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Data_Party::~C_Data_Party()
{
    MACRO_Log_Function
}

C_Data_Party::C_Data_Party
(
    C_Engine_Concrete & ro_Engine_Concrete,
    string const & rstring_Id_Party
)
: m_ro_Engine_Concrete(ro_Engine_Concrete)
, m_b_Log_File(false)
, m_string_Id_Party(rstring_Id_Party)
, m_string_Path_Data_Party_Folder()
, m_string_Path_Folder_Dynamic_Context()
, m_vector_spo_Data_Tag()
, m_map_string_spo_Data_Rule()
, m_map_string_spo_Data_Context()
, m_map_string_spo_Data_Dialog()
{
    MACRO_Log_Function

    m_vector_spo_Data_Tag.reserve(64);
}

E_Result
C_Data_Party::M_Fn_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    string string_Path_Folder_Tmp = "Party_" + m_string_Id_Party;
    string_Path_Folder_Tmp += "/Language_" + m_ro_Engine_Concrete.M_Fn_Get_rstring_Id_Language();
    string_Path_Folder_Tmp += "/";

    m_string_Path_Folder_Dynamic_Context = G_o_Get_Path().M_Fn_Get_string_Path_Usr(); 
    m_string_Path_Folder_Dynamic_Context += string_Path_Folder_Tmp;
    m_string_Path_Folder_Dynamic_Context += "Dynamic_Context";
    
    IF (!Fn_File_Access(m_string_Path_Folder_Dynamic_Context)) {
        IF (0 != Fn_Directory_Create_Deep(m_string_Path_Folder_Dynamic_Context)) {
            MACRO_Log_Error
            return E_Result_Failed;
        }
    }

    m_string_Path_Folder_Dynamic_Context += '/';
    Fn_Log("    Party Dynamic Context:" + m_string_Path_Folder_Dynamic_Context);

    string string_Path_PData_Folder = G_o_Get_Path().M_Fn_Get_string_Path_PData() + string_Path_Folder_Tmp;
    string string_Path_VData_Folder = G_o_Get_Path().M_Fn_Get_string_Path_VData() + string_Path_Folder_Tmp;

    e_Result = _M_Fn_Info_Get_Path_Data_Party_Folder(
        string_Path_PData_Folder,
        string_Path_VData_Folder
    );

    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    m_b_Log_File = false;
    e_Result = _M_Fn_Info_Parse_File();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Enable_Log(m_b_Log_File);

    map<string, shared_ptr<C_Data_Context> >::iterator iterator_pair_string_spo_Data_Context_End = m_map_string_spo_Data_Context.end();

    for (
        map<string, shared_ptr<C_Data_Context> >::iterator iterator_pair_string_spo_Data_Context_Curr = m_map_string_spo_Data_Context.begin();
        iterator_pair_string_spo_Data_Context_Curr != iterator_pair_string_spo_Data_Context_End;
        ++iterator_pair_string_spo_Data_Context_Curr
    ) {
        shared_ptr<C_Data_Context> & rspo_Data_Context = iterator_pair_string_spo_Data_Context_Curr->second;
        IF (!rspo_Data_Context) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        e_Result = rspo_Data_Context->M_Fn_Init();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Party::M_Fn_Fina()
{
    MACRO_Log_Function

    E_Result e_Result;

    m_map_string_spo_Data_Dialog.clear();

    map<string, shared_ptr<C_Data_Context> >::iterator iterator_pair_string_spo_Data_Context_End = m_map_string_spo_Data_Context.end();

    for (
        map<string, shared_ptr<C_Data_Context> >::iterator iterator_pair_string_spo_Data_Context_Curr = m_map_string_spo_Data_Context.begin();
        iterator_pair_string_spo_Data_Context_Curr != iterator_pair_string_spo_Data_Context_End;
        ++iterator_pair_string_spo_Data_Context_Curr
    ) {
        shared_ptr<C_Data_Context> & rspo_Data_Context = iterator_pair_string_spo_Data_Context_Curr->second;
        IF (!rspo_Data_Context) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        e_Result = rspo_Data_Context->M_Fn_Fina();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    m_map_string_spo_Data_Context.clear();

    m_map_string_spo_Data_Rule.clear();

    m_vector_spo_Data_Tag.clear();

    return E_Result_Succeeded;
}

E_Result
C_Data_Party::M_Fn_Dialog_Load
(
    C_Request_Recognize & ro_Request_Recognize
)
{
    MACRO_Log_Function

    C_Data_Dialog * po_Data_Dialog = M_Fn_Find_po_Data_Dialog(ro_Request_Recognize.m_string_Id_Dialog);
    IF (!po_Data_Dialog) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return po_Data_Dialog->M_Fn_Load(ro_Request_Recognize);
}

E_Result
C_Data_Party::M_Fn_Dialog_Unload
(
    C_Request_Recognize & ro_Request_Recognize
)
{
    MACRO_Log_Function

    C_Data_Dialog * po_Data_Dialog = M_Fn_Find_po_Data_Dialog(ro_Request_Recognize.m_string_Id_Dialog);
    IF (!po_Data_Dialog) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return po_Data_Dialog->M_Fn_Unload();
}

C_Data_Tag *
C_Data_Party::M_Fn_Find_po_Data_Tag
(
    size_t i_Pos_Tag
)
{
    stringstream stringstream_Log;
    
    IF (i_Pos_Tag > m_vector_spo_Data_Tag.size()) {
        stringstream_Log << "    No Data Tag: " << i_Pos_Tag;
        Fn_Log(stringstream_Log.str());
        
        RETURN_NULL;
    }

    return m_vector_spo_Data_Tag[i_Pos_Tag].get();
}

C_Data_Rule *
C_Data_Party::M_Fn_Find_po_Data_Rule
(
    string const & rstring_Id_Rule
)
{
    stringstream stringstream_Log;
#if 1 // for PreCV milestone
    string string_Id_Rule_Tar_Tmp;
    const string string_Id_Rule_Cur_Tmp_01 = "grm_def_navi_address_house_01#rul_def_navi_address_house_num";
    const string string_Id_Rule_Cur_Tmp_03 = "grm_def_navi_address_house_03#rul_def_navi_address_house_num";

    IF ((string_Id_Rule_Cur_Tmp_01 == rstring_Id_Rule) 
        || (string_Id_Rule_Cur_Tmp_03 == rstring_Id_Rule)) {
        string_Id_Rule_Tar_Tmp = "grm_def_navi_address_house_num#rul_def_navi_address_house_num";
    }
    ELSE {
        string_Id_Rule_Tar_Tmp = rstring_Id_Rule;
    }
    

    map<string, shared_ptr<C_Data_Rule> >::iterator iterator_pair_string_spo_Data_Rule_Find;
    iterator_pair_string_spo_Data_Rule_Find = m_map_string_spo_Data_Rule.find(
        string_Id_Rule_Tar_Tmp
    );

#else // original code    
    map<string, shared_ptr<C_Data_Rule> >::iterator iterator_pair_string_spo_Data_Rule_Find;
    iterator_pair_string_spo_Data_Rule_Find = m_map_string_spo_Data_Rule.find(
        rstring_Id_Rule
    );
#endif    

    IF (iterator_pair_string_spo_Data_Rule_Find == m_map_string_spo_Data_Rule.end()) {
        stringstream_Log << "    No Data Rule: " << rstring_Id_Rule;
        Fn_Log(stringstream_Log.str());

        RETURN_NULL;
    }

    shared_ptr<C_Data_Rule> & rspo_Data_Rule = iterator_pair_string_spo_Data_Rule_Find->second;

    return rspo_Data_Rule.get();
}

C_Data_Context *
C_Data_Party::M_Fn_Find_po_Data_Context
(
    string const & rstring_Id_Context
)
{
    stringstream stringstream_Log;
    
    map<string, shared_ptr<C_Data_Context> >::iterator iterator_pair_string_spo_Data_Context_Find;
    iterator_pair_string_spo_Data_Context_Find = m_map_string_spo_Data_Context.find(
        rstring_Id_Context
    );

    IF (iterator_pair_string_spo_Data_Context_Find == m_map_string_spo_Data_Context.end()) {
        stringstream_Log << "    No Data Context: " << rstring_Id_Context;
        Fn_Log(stringstream_Log.str());
        
        MACRO_Log_Error
        RETURN_NULL;
    }

    shared_ptr<C_Data_Context> & rspo_Data_Context = iterator_pair_string_spo_Data_Context_Find->second;

    return rspo_Data_Context.get();
}

C_Data_Dialog *
C_Data_Party::M_Fn_Find_po_Data_Dialog
(
    string const & rstring_Id_Dialog
)
{
    stringstream stringstream_Log;
    
    map<string, shared_ptr<C_Data_Dialog> >::iterator iterator_pair_string_spo_Data_Dialog_Find;
    iterator_pair_string_spo_Data_Dialog_Find = m_map_string_spo_Data_Dialog.find(
        rstring_Id_Dialog
    );

    IF (iterator_pair_string_spo_Data_Dialog_Find == m_map_string_spo_Data_Dialog.end()) {
        stringstream_Log << "    No Data Dialog: " << rstring_Id_Dialog;
        Fn_Log(stringstream_Log.str());
        
        MACRO_Log_Error
        RETURN_NULL;
    }

    shared_ptr<C_Data_Dialog> & rspo_Data_Dialog = iterator_pair_string_spo_Data_Dialog_Find->second;

    return rspo_Data_Dialog.get();
}

E_Result
C_Data_Party::_M_Fn_Info_Get_Path_Data_Party_Folder
(
    string const & string_Path_PData_Folder,
    string const & string_Path_VData_Folder
)
{
    MACRO_Log_Function

    E_Result e_Result;

    string string_Path_PData_File_Info;
    string string_Path_VData_File_Info;
    int i_Has_PData_File_Info = 0;
    int i_Has_VData_File_Info = 0;

    string_Path_PData_File_Info = string_Path_PData_Folder + "Info.dat";
    string_Path_VData_File_Info = string_Path_VData_Folder + "Info.dat";

    i_Has_PData_File_Info = Fn_File_Access(string_Path_PData_File_Info);
    i_Has_VData_File_Info = Fn_File_Access(string_Path_VData_File_Info);

    IF (!i_Has_PData_File_Info && !i_Has_VData_File_Info) {
        // Info data is not exist in pdata folder and vdata folder. 

        Fn_Log("    The file does not exist in " + string_Path_PData_File_Info + "and" + string_Path_VData_File_Info);
        MACRO_Log_Error
        return E_Result_Failed;
    }
    ELSE
    IF (i_Has_PData_File_Info && i_Has_VData_File_Info) {
        // Info data exist in pdata folder and vdata folder. 

        // Get pdata info data version.

        long i_Static_Version_PData_File_Info = 0;
        long i_Navi_Version_PData_File_Info = 0;

        e_Result = _M_Fn_Info_Get_Version(
            string_Path_PData_File_Info, 
            i_Static_Version_PData_File_Info,
            i_Navi_Version_PData_File_Info
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        // Get vdata info data version.

        long i_Static_Version_VData_File_Info = 0;
        long i_Navi_Version_VData_File_Info = 0;

        e_Result = _M_Fn_Info_Get_Version(
            string_Path_VData_File_Info, 
            i_Static_Version_VData_File_Info,
            i_Navi_Version_VData_File_Info
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        // For pp release data

        IF (0 == i_Navi_Version_PData_File_Info) {

            IF (2015120401 == i_Static_Version_PData_File_Info) {
                i_Navi_Version_PData_File_Info = 2015121401;
            }
            ELSE
            IF (2015111501 == i_Static_Version_PData_File_Info) {

                IF ((i_Navi_Version_VData_File_Info == 2015111601)
                    || (i_Navi_Version_VData_File_Info == 2015112301)) {

                    i_Navi_Version_PData_File_Info = i_Navi_Version_VData_File_Info;
                }
            }
        }

        // Switch path.

        m_string_Path_Data_Party_Folder = string_Path_PData_Folder;

        IF ((i_Navi_Version_PData_File_Info == i_Navi_Version_VData_File_Info) 
            && (i_Static_Version_PData_File_Info < i_Static_Version_VData_File_Info)) {
            // Use the latest data version when navigation data version matched.

            m_string_Path_Data_Party_Folder = string_Path_VData_Folder;
        }
    }
    ELSE
    IF (i_Has_PData_File_Info && !i_Has_VData_File_Info) {
        // Only has pdata folder

        m_string_Path_Data_Party_Folder = string_Path_PData_Folder;
    }
    ELSE {
        // Only has vdata folder

        m_string_Path_Data_Party_Folder = string_Path_VData_Folder;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Party::_M_Fn_Info_Parse_File()
{
    MACRO_Log_Function

    E_Result e_Result;

    std::ifstream ifstream_File;
    string string_Path_File_Info;

    // Open file.

    string_Path_File_Info = m_string_Path_Data_Party_Folder + "Info.dat";
    Fn_Log("    Party:" + string_Path_File_Info);

    ifstream_File.open(string_Path_File_Info.c_str());
    IF (!ifstream_File.is_open()) {
        Fn_Log("    Failed to open file: "+ string_Path_File_Info);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // BOM

    char i_BOM;

    ifstream_File >> i_BOM >> i_BOM >> i_BOM;

    string string_Line;

    Fn_Get_string_Line_Fixed(ifstream_File, string_Line);

    // Parse lines.

    string string_Key;
    string string_Value;
    bool b_Eof;

    while (true) {
        e_Result = _M_Fn_Info_Parse_Line(
            ifstream_File,
            string_Key,
            string_Value,
            b_Eof
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        IF (b_Eof) {
            break;
        }

        IF (string_Key == "DIALOG") {
            e_Result = _M_Fn_Info_Parse_Dialog(ifstream_File);
        }
        ELSE
        IF (string_Key == "CONTEXT") {
            e_Result = _M_Fn_Info_Parse_Context(ifstream_File);
        }
        ELSE
        IF (string_Key == "RULE") {
            e_Result = _M_Fn_Info_Parse_Rule(ifstream_File);
        }
        ELSE
        IF (string_Key == "TAG") {
            e_Result = _M_Fn_Info_Parse_Tag(ifstream_File);
        }
        ELSE
        IF (string_Key == "LOG") {
            IF (string_Value == "enable") {
                Fn_Log("    Enable Log File");
                m_b_Log_File = true;
            }

            continue;
        }
        ELSE
        IF (string_Key.empty()) {
            continue;
        }
        ELSE {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Party::_M_Fn_Info_Parse_Dialog
(
    ifstream & rifstream_File
)
{
    E_Result e_Result;

    string string_Id_Dialog;
    list<shared_ptr<C_Data_Context_In_Dialog> > list_spo_Data_Context_In_Dialog;

    // Parse lines.

    string string_Key;
    string string_Value;
    bool b_Eof;

    while (true) {
        e_Result = _M_Fn_Info_Parse_Line(
            rifstream_File,
            string_Key,
            string_Value,
            b_Eof
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        IF (b_Eof) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (string_Key == "PRIMARY_ID_DIALOG") {
            string_Id_Dialog = string_Value;
        }
        ELSE
        IF (string_Key == "CONTEXT") {
            shared_ptr<C_Data_Context_In_Dialog> spo_Data_Context_In_Dialog(MACRO_New C_Data_Context_In_Dialog);

            e_Result = _M_Fn_Info_Parse_Context_In_Dialog(
                rifstream_File,
                *spo_Data_Context_In_Dialog
            );

            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                return e_Result;
            }
            
            list_spo_Data_Context_In_Dialog.push_back(spo_Data_Context_In_Dialog);
        }
        ELSE
        IF (string_Key == "DIALOG_END") {
            break;
        }
        ELSE
        IF (string_Key.empty()) {
            continue;
        }
        ELSE {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    IF (string_Id_Dialog.empty()) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (list_spo_Data_Context_In_Dialog.empty()) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    map<string, shared_ptr<C_Data_Dialog> >::iterator iterator_pair_string_spo_Data_Dialog_Find = m_map_string_spo_Data_Dialog.find(string_Id_Dialog);

    IF (iterator_pair_string_spo_Data_Dialog_Find != m_map_string_spo_Data_Dialog.end()) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    stringstream stringstream_Log;
    stringstream_Log << "    Dialog - (Id)" << string_Id_Dialog << " (Context)";
    list<shared_ptr<C_Data_Context_In_Dialog> >::iterator iterator_list_spo_Data_Context_In_Dialog_End = list_spo_Data_Context_In_Dialog.end();
    for (
        list<shared_ptr<C_Data_Context_In_Dialog> >::iterator iterator_list_spo_Data_Context_In_Dialog_Curr = list_spo_Data_Context_In_Dialog.begin();
        iterator_list_spo_Data_Context_In_Dialog_Curr != iterator_list_spo_Data_Context_In_Dialog_End;
        ++iterator_list_spo_Data_Context_In_Dialog_Curr
    ) {
        shared_ptr<C_Data_Context_In_Dialog> & rspo_Data_Context_In_Dialog = *iterator_list_spo_Data_Context_In_Dialog_Curr;
        IF (!rspo_Data_Context_In_Dialog) {
            MACRO_Log_Error;
            return E_Result_Error;
        }
        stringstream_Log << rspo_Data_Context_In_Dialog->m_string_Id_Context << ",";
    }
    Fn_Log(stringstream_Log.str());

    shared_ptr<C_Data_Dialog> spo_Data_Dialog(
        MACRO_New C_Data_Dialog(
            *this,
            string_Id_Dialog,
            list_spo_Data_Context_In_Dialog
        )
    );

    m_map_string_spo_Data_Dialog.insert(
        std::make_pair(
            string_Id_Dialog,
            spo_Data_Dialog
        )
    );

    return E_Result_Succeeded;
}

E_Result
C_Data_Party::_M_Fn_Info_Parse_Context_In_Dialog
(
    ifstream & rifstream_File,
    C_Data_Context_In_Dialog & ro_Data_Context_In_Dialog
)
{
    E_Result e_Result;

    string string_Key;
    string string_Value;
    bool b_Eof;

    list<std::string> list_string_Id_Rule_Deactivate;

    while (true) {
        e_Result = _M_Fn_Info_Parse_Line(
            rifstream_File,
            string_Key,
            string_Value,
            b_Eof
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        IF (b_Eof) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (string_Key == "FOREIGN_ID_CONTEXT") {
            ro_Data_Context_In_Dialog.m_string_Id_Context = string_Value;
        }
        ELSE
        IF (string_Key == "TYPE_MERGE_CONTEXT") {
            IF ("host" == string_Value) {
                ro_Data_Context_In_Dialog.m_b_Is_Host_For_Merge = true;
            }
            ELSE IF ("guest" == string_Value) {
                ro_Data_Context_In_Dialog.m_b_Is_Host_For_Merge = false;
            }
            ELSE {
                MACRO_Log_Error
                return E_Result_Error;
            }
        }
        ELSE
        IF (string_Key == "FOREIGN_ID_RULE_DEACTIVATE") {
            list_string_Id_Rule_Deactivate.push_back(string_Value);
        }
        ELSE
        IF (string_Key == "CONTEXT_END") {
            break;
        }
        ELSE
        IF (string_Key.empty()) {
            continue;
        }
        ELSE {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    IF (!list_string_Id_Rule_Deactivate.empty()) {

        ro_Data_Context_In_Dialog.m_spo_Request_Activate.reset(MACRO_New C_Request_Activate);
        ro_Data_Context_In_Dialog.m_spo_Request_Activate->m_string_Id_Context = ro_Data_Context_In_Dialog.m_string_Id_Context;
        ro_Data_Context_In_Dialog.m_spo_Request_Activate->m_list_string_Id_Rule.swap(list_string_Id_Rule_Deactivate);
    }

    IF (ro_Data_Context_In_Dialog.m_string_Id_Context.empty()) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Party::_M_Fn_Info_Parse_Context
(
    ifstream & rifstream_File
)
{
    E_Result e_Result;

    string string_Id_Context;
    string string_Type_Context;
    string string_File_Context;
    string string_Data_File_Context;
    string string_Path_Data_Folder_Context;
    list<pair<string, string> > list_pair_string_Id_Slot_string_Id_Context;

    string_Path_Data_Folder_Context = m_string_Path_Data_Party_Folder + "Context/";
    // Parse lines.

    string string_Key;
    string string_Value;
    bool b_Eof;

    while (true) {
        e_Result = _M_Fn_Info_Parse_Line(
            rifstream_File,
            string_Key,
            string_Value,
            b_Eof
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        IF (b_Eof) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (string_Key == "PRIMARY_ID_CONTEXT") {
            string_Id_Context = string_Value;
        }
        ELSE
        IF (string_Key == "TYPE_CONTEXT") {
            string_Type_Context = string_Value;
        }
        ELSE
        IF (string_Key == "FILE_CONTEXT") {
            string_File_Context = string_Value;
        }
        ELSE
        IF (string_Key == "SLOT") {
            pair<string, string> pair_string_Id_Slot_string_Id_Context;

            e_Result = _M_Fn_Info_Parse_Slot_In_Context(
                rifstream_File,
                pair_string_Id_Slot_string_Id_Context
            );

            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                return e_Result;
            }

            list_pair_string_Id_Slot_string_Id_Context.push_back(
                pair_string_Id_Slot_string_Id_Context
            );
        }
        ELSE
        IF (string_Key == "CONTEXT_END") {
            break;
        }
        ELSE
        IF (string_Key.empty()) {
            continue;
        }
        ELSE {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    IF (string_Id_Context.empty()) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (string_File_Context.empty()) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF ((string_Type_Context == "list")
        || (string_Type_Context == "fuzzy")) {
        string_Data_File_Context = m_string_Path_Folder_Dynamic_Context + string_File_Context; 
    }
    ELSE {
        string_Data_File_Context = string_Path_Data_Folder_Context + string_File_Context;    
    }

    map<string, shared_ptr<C_Data_Context> >::iterator iterator_pair_string_spo_Data_Context_Find = m_map_string_spo_Data_Context.find(string_Id_Context);

    IF (iterator_pair_string_spo_Data_Context_Find != m_map_string_spo_Data_Context.end()) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    shared_ptr<C_Data_Context> spo_Data_Context;

    IF (string_Type_Context == "none") {

    }
    ELSE
    IF (string_Type_Context == "list") {
        spo_Data_Context.reset(
            MACRO_New C_Data_Context_List(
                *this,
                string_Id_Context,
                string_Type_Context,
                string_Data_File_Context,
                list_pair_string_Id_Slot_string_Id_Context
            )
        );
    }
    ELSE
    IF (string_Type_Context == "grammar") {
        spo_Data_Context.reset(
            MACRO_New C_Data_Context_Grammar(
                *this,
                string_Id_Context,
                string_Type_Context,
                string_Data_File_Context,
                list_pair_string_Id_Slot_string_Id_Context
            )
        );
    }
    ELSE
    IF (string_Type_Context == "field") {
        spo_Data_Context.reset(
            MACRO_New C_Data_Context_Field(
                *this,
                string_Id_Context,
                string_Type_Context,
                string_Data_File_Context,
                list_pair_string_Id_Slot_string_Id_Context
            )
        );
    }
    ELSE
    IF (string_Type_Context == "spelling") {
        spo_Data_Context.reset(
            MACRO_New C_Data_Context_Spelling(
                *this,
                string_Id_Context,
                string_Type_Context,
                string_Data_File_Context,
                list_pair_string_Id_Slot_string_Id_Context
            )
        );
    }
    ELSE
    IF (string_Type_Context == "userword") {
        spo_Data_Context.reset(
            MACRO_New C_Data_Context_Userword(
                *this,
                string_Id_Context,
                string_Type_Context,
                string_Data_File_Context,
                list_pair_string_Id_Slot_string_Id_Context
            )
        );
    }
#if 0
    ELSE
    IF (string_Type_Context == "fuzzy_host") {

        spo_Data_Context.reset(
            MACRO_New C_Data_Context_Fuzzy_Host(
                *this,
                string_Id_Context,
                string_Type_Context,
                string_Data_File_Context,
                list_pair_string_Id_Slot_string_Id_Context
            )
        );
    }
    ELSE
    IF (string_Type_Context == "fuzzy") {
        spo_Data_Context.reset(
            MACRO_New C_Data_Context_Fuzzy(
                *this,
                string_Id_Context,
                string_Type_Context,
                string_Data_File_Context,
                list_pair_string_Id_Slot_string_Id_Context
            )
        );
    }
#endif
    ELSE {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_map_string_spo_Data_Context.insert(
        std::make_pair(
            string_Id_Context,
            spo_Data_Context
        )
    );

    stringstream stringstream_Log;
    stringstream_Log << "    Context - (Id)" << string_Id_Context << "; (Type)" + string_Type_Context;
    Fn_Log(stringstream_Log.str());

    return E_Result_Succeeded;
}

E_Result
C_Data_Party::_M_Fn_Info_Parse_Slot_In_Context
(
    ifstream & rifstream_File,
    pair<string, string> & rpair_string_Id_Slot_string_Id_Context
)
{
    E_Result e_Result;

    string string_Key;
    string string_Value;
    bool b_Eof;

    while (true) {
        e_Result = _M_Fn_Info_Parse_Line(
            rifstream_File,
            string_Key,
            string_Value,
            b_Eof
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        IF (b_Eof) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (string_Key == "FOREIGN_ID_RULE_SLOT") {
            rpair_string_Id_Slot_string_Id_Context.first = string_Value;
        }
        ELSE
        IF (string_Key == "FOREIGN_ID_CONTEXT") {
            rpair_string_Id_Slot_string_Id_Context.second = string_Value;
        }
        ELSE
        IF (string_Key == "SLOT_END") {
            break;
        }
        ELSE
        IF (string_Key.empty()) {
            continue;
        }
        ELSE {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Party::_M_Fn_Info_Parse_Rule
(
    ifstream & rifstream_File
)
{
    E_Result e_Result;

    shared_ptr<C_Data_Rule> spo_Data_Rule(MACRO_New C_Data_Rule(*this));

    // Parse lines.

    string string_Key;
    string string_Value;
    bool b_Eof;

    while (true) {
        e_Result = _M_Fn_Info_Parse_Line(
            rifstream_File,
            string_Key,
            string_Value,
            b_Eof
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        IF (b_Eof) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (string_Key == "NODE_RESULT") {
            shared_ptr<C_Data_Node_Result> spo_Data_Node_Result(MACRO_New C_Data_Node_Result);

            e_Result = _M_Fn_Info_Parse_Node_Result(
                rifstream_File,
                *spo_Data_Node_Result
            );

            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                return e_Result;
            }

            spo_Data_Rule->m_list_spo_Data_Node_Result.push_back(spo_Data_Node_Result);
        }
        ELSE
        IF (string_Key == "TYPE") {
            spo_Data_Rule->m_string_Type_Rule = string_Value;
        }
        ELSE
        IF (string_Key == "PRIMARY_ID_RULE") {
            spo_Data_Rule->m_string_Id_Rule = string_Value;
        }
        ELSE
        IF (string_Key == "RULE_END") {
            break;
        }
        ELSE
        IF (string_Key.empty()) {
            continue;
        }
        ELSE {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    IF (spo_Data_Rule->m_string_Id_Rule.empty()) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_map_string_spo_Data_Rule.insert(
        std::make_pair(
            spo_Data_Rule->m_string_Id_Rule,
            spo_Data_Rule
        )
    );

    stringstream stringstream_Log;
    stringstream_Log << "    Rule - (Id)" << spo_Data_Rule->m_string_Id_Rule;
    Fn_Log(stringstream_Log.str());

    return E_Result_Succeeded;
}

E_Result
C_Data_Party::_M_Fn_Info_Parse_Tag
(
    ifstream & rifstream_File
)
{
    E_Result e_Result;

    shared_ptr<C_Data_Tag> spo_Data_Tag(MACRO_New C_Data_Tag(*this));

    // Parse lines.

    string string_Key;
    string string_Value;
    bool b_Eof;

    while (true) {
        e_Result = _M_Fn_Info_Parse_Line(
            rifstream_File,
            string_Key,
            string_Value,
            b_Eof
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        IF (b_Eof) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (string_Key == "NODE_RESULT") {
            shared_ptr<C_Data_Node_Result> spo_Data_Node_Result(MACRO_New C_Data_Node_Result);

            e_Result = _M_Fn_Info_Parse_Node_Result(
                rifstream_File,
                *spo_Data_Node_Result
            );

            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                return e_Result;
            }

            spo_Data_Tag->m_list_spo_Data_Node_Result.push_back(spo_Data_Node_Result);
        }
        ELSE
        IF (string_Key == "PRIMARY_CODE_TAG") {
            stringstream stringstream_Value;
            stringstream_Value << string_Value;
            stringstream_Value >> spo_Data_Tag->m_i_Pos_Tag;
#ifdef MACRO_Dynamic_Debug
            IF (!stringstream_Value) {
                MACRO_Log_Error
                return E_Result_Error;
            }
#endif
        }
        ELSE
        IF (string_Key == "PRIMARY_ID_TAG") {
            spo_Data_Tag->m_string_Id_Tag = string_Value;
        }
        ELSE
        IF (string_Key == "TAG_END") {
            break;
        }
        ELSE
        IF (string_Key.empty()) {
            continue;
        }
        ELSE {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    IF (spo_Data_Tag->m_string_Id_Tag.empty()) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    while (spo_Data_Tag->m_i_Pos_Tag >= m_vector_spo_Data_Tag.size()) {
        m_vector_spo_Data_Tag.push_back(shared_ptr<C_Data_Tag>());
    }

    shared_ptr<C_Data_Tag> & rspo_Data_Tag = m_vector_spo_Data_Tag[spo_Data_Tag->m_i_Pos_Tag];
    IF ((bool)rspo_Data_Tag) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    rspo_Data_Tag = spo_Data_Tag;

    stringstream stringstream_Log;
    stringstream_Log << "    Tag - (Code)" << spo_Data_Tag->m_i_Pos_Tag << "; (Id)" + spo_Data_Tag->m_string_Id_Tag;
    Fn_Log(stringstream_Log.str());

    return E_Result_Succeeded;
}

E_Result
C_Data_Party::_M_Fn_Info_Parse_Node_Result
(
    ifstream & rifstream_File,
    C_Data_Node_Result & ro_Data_Node_Result
)
{
    E_Result e_Result;

    // Parse lines.

    string string_Key;
    string string_Value;
    bool b_Eof;

    while (true) {
        e_Result = _M_Fn_Info_Parse_Line(
            rifstream_File,
            string_Key,
            string_Value,
            b_Eof
        );

        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        IF (b_Eof) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        IF (string_Key == "NODE_RESULT") {
            shared_ptr<C_Data_Node_Result> spo_Data_Node_Result(MACRO_New C_Data_Node_Result);

            e_Result = _M_Fn_Info_Parse_Node_Result(
                rifstream_File,
                *spo_Data_Node_Result
            );

            IF (e_Result != E_Result_Succeeded) {
                MACRO_Log_Error
                return e_Result;
            }

            ro_Data_Node_Result.m_list_spo_Data_Node_Result.push_back(spo_Data_Node_Result);
        }
        ELSE
        IF (string_Key == "NAME") {
            ro_Data_Node_Result.m_string_Name = string_Value;
        }
        ELSE
        IF (string_Key == "TYPE_VALUE") {
            C_Data_Node_Result::E_Type_Value e_Type_Value;

            IF (string_Value == "none") {
                e_Type_Value = C_Data_Node_Result::E_Type_Value_None;
            }
            ELSE
            IF (string_Value == "defined") {
                e_Type_Value = C_Data_Node_Result::E_Type_Value_Defined;
            }
            ELSE
            IF (string_Value == "tag_name") {
                e_Type_Value = C_Data_Node_Result::E_Type_Value_Tag_Name;
            }
            ELSE
            IF (string_Value == "tag_user_id") {
                e_Type_Value = C_Data_Node_Result::E_Type_Value_Tag_User_Id;
            }
            ELSE
            IF (string_Value == "tag_orthography") {
                e_Type_Value = C_Data_Node_Result::E_Type_Value_Tag_Orthography;
            }
            ELSE
            IF (string_Value == "tag_eval") {
                e_Type_Value = C_Data_Node_Result::E_Type_Value_Tag_Eval;
            }
            ELSE
            IF (string_Value == "terminal_name") {
                e_Type_Value = C_Data_Node_Result::E_Type_Value_Terminal_Name;
            }
            ELSE
            IF (string_Value == "terminal_user_id") {
                e_Type_Value = C_Data_Node_Result::E_Type_Value_Terminal_User_Id;
            }
            ELSE
            IF (string_Value == "terminal_orthography") {
                e_Type_Value = C_Data_Node_Result::E_Type_Value_Terminal_Orthography;
            }
            ELSE {
                MACRO_Log_Error
                return e_Result;
            }

            ro_Data_Node_Result.m_e_Type_Value = e_Type_Value;
        }
        ELSE
        IF (string_Key == "TYPE_TERMINAL") {
            C_Data_Node_Result::E_Type_Terminal e_Type_Terminal;

            IF (string_Value == "none") {
                e_Type_Terminal = C_Data_Node_Result::E_Type_Terminal_None;
            }
            ELSE
            IF (string_Value == "by_pos") {
                e_Type_Terminal = C_Data_Node_Result::E_Type_Terminal_By_Pos;
            }
            ELSE
            IF (string_Value == "by_code") {
                e_Type_Terminal = C_Data_Node_Result::E_Type_Terminal_By_Code;
            }
            ELSE {
                MACRO_Log_Error
                return e_Result;
            }

            ro_Data_Node_Result.m_e_Type_Terminal = e_Type_Terminal;
        }
        ELSE
        IF (string_Key == "TYPE_TAG") {
            C_Data_Node_Result::E_Type_Tag e_Type_Tag;

            IF (string_Value == "none") {
                e_Type_Tag = C_Data_Node_Result::E_Type_Tag_None;
            }
            ELSE
            IF (string_Value == "by_pos") {
                e_Type_Tag = C_Data_Node_Result::E_Type_Tag_By_Pos;
            }
            ELSE
            IF (string_Value == "by_code") {
                e_Type_Tag = C_Data_Node_Result::E_Type_Tag_By_Code;
            }
            ELSE {
                MACRO_Log_Error
                return e_Result;
            }

            ro_Data_Node_Result.m_e_Type_Tag = e_Type_Tag;
        }
        ELSE
        IF (string_Key == "POS_TERMINAL") {
            stringstream stringstream_Value;
            stringstream_Value << string_Value;
            stringstream_Value >> ro_Data_Node_Result.m_i_Pos_Terminal;
#ifdef MACRO_Dynamic_Debug
            IF (!stringstream_Value) {
                MACRO_Log_Error
                return E_Result_Error;
            }
#endif
        }
        ELSE
        IF (string_Key == "POS_TAG") {
            stringstream stringstream_Value;
            stringstream_Value << string_Value;
            stringstream_Value >> ro_Data_Node_Result.m_i_Pos_Tag;
#ifdef MACRO_Dynamic_Debug
            IF (!stringstream_Value) {
                MACRO_Log_Error
                return E_Result_Error;
            }
#endif
        }
        ELSE
        IF (string_Key == "CODE_TERMINAL_MIN") {
            stringstream stringstream_Value;
            stringstream_Value << string_Value;
            stringstream_Value >> ro_Data_Node_Result.m_i_Code_Terminal_Min;
#ifdef MACRO_Dynamic_Debug
            IF (!stringstream_Value) {
                MACRO_Log_Error
                return E_Result_Error;
            }
#endif
        }
        ELSE
        IF (string_Key == "CODE_TERMINAL_MAX") {
            stringstream stringstream_Value;
            stringstream_Value << string_Value;
            stringstream_Value >> ro_Data_Node_Result.m_i_Code_Terminal_Max;
#ifdef MACRO_Dynamic_Debug
            IF (!stringstream_Value) {
                MACRO_Log_Error
                return E_Result_Error;
            }
#endif
        }
        ELSE
        IF (string_Key == "CODE_TAG_MIN") {
            stringstream stringstream_Value;
            stringstream_Value << string_Value;
            stringstream_Value >> ro_Data_Node_Result.m_i_Code_Tag_Min;
#ifdef MACRO_Dynamic_Debug
            IF (!stringstream_Value) {
                MACRO_Log_Error
                return E_Result_Error;
            }
#endif
        }
        ELSE
        IF (string_Key == "CODE_TAG_MAX") {
            stringstream stringstream_Value;
            stringstream_Value << string_Value;
            stringstream_Value >> ro_Data_Node_Result.m_i_Code_Tag_Max;
#ifdef MACRO_Dynamic_Debug
            IF (!stringstream_Value) {
                MACRO_Log_Error
                return E_Result_Error;
            }
#endif
        }
        ELSE
        IF (string_Key == "OPTIONAL") {
            IF (string_Value == "true") {
                ro_Data_Node_Result.m_b_Optional = true;
            }
        }
        ELSE
        IF (string_Key == "VALUE_DEFIND") {
            ro_Data_Node_Result.m_string_Value_Defined = string_Value;
        }
        ELSE
        IF (string_Key == "NODE_RESULT_END") {
            break;
        }
        ELSE
        IF (string_Key.empty()) {
            continue;
        }
        ELSE {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }
/*
    IF (ro_Data_Node_Result.m_string_Name.empty()) {
        MACRO_Log_Error
        return E_Result_Error;
    }
*/
    IF (ro_Data_Node_Result.m_i_Code_Tag_Min > ro_Data_Node_Result.m_i_Code_Tag_Max) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Party::_M_Fn_Info_Parse_Line
(
    ifstream & rifstream_File,
    string & rstring_Key,
    string & rstring_Value,
    bool & rb_Eof
)
{
    // Init return value.

    rstring_Key.clear();
    rstring_Value.clear();
    rb_Eof = false;

    // Get line.

    string string_Line;

    Fn_Get_string_Line_Fixed(rifstream_File, string_Line);

    IF (!rifstream_File) {
        // Check stream state: badbit

        IF (rifstream_File.rdstate() & std::ios::badbit) {
            MACRO_Log_Error
            return E_Result_Error;
        }

        // Check StreamFile: eofbit

        IF (rifstream_File.rdstate() & std::ios::eofbit) {
            rb_Eof = true;
            return E_Result_Succeeded;
        }

        // Check StreamFile: failbit
#ifdef MACRO_Dynamic_Debug
        IF (rifstream_File.rdstate() & std::ios::failbit) {
            MACRO_Log_Error
            return E_Result_Error;
        }
#endif
    }

    // Remove trailing comment.

    string::size_type i_Pos_Char_Dollar = string_Line.find('$');
    IF (i_Pos_Char_Dollar != string::npos) {
        string_Line = string_Line.substr(0, i_Pos_Char_Dollar);
    }

    // Get string.

    stringstream stringstream_Line;
    stringstream_Line.clear();
    stringstream_Line.str(string_Line);

    // Get key.

    IF (!stringstream_Line) {
        return E_Result_Succeeded;
    }
    stringstream_Line >> rstring_Key;

    // Get value.

    IF (!stringstream_Line) {
        return E_Result_Succeeded;
    }
    std::getline(stringstream_Line, rstring_Value);
    rstring_Value = Fn_Get_string_Without_Blank(rstring_Value);

    return E_Result_Succeeded;
}

E_Result
C_Data_Party::_M_Fn_Info_Get_Version
(
    string const & rstring_Path_File,
    long & ri_Static_Version,
    long & ri_Navi_Version
)
{
    ri_Static_Version = 0;
    ri_Navi_Version = 0;

    // Open file.

    std::ifstream ifstream_File;
    ifstream_File.open(rstring_Path_File.c_str());
    IF (!ifstream_File.is_open()) {
        Fn_Log("    Failed to open file: " + rstring_Path_File);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Parse version info.

    enum E_Type_Line_Number
    {
        E_Type_Line_Number_Start = 1,  // start line index
        E_Type_Line_Number_Version_Static,  // static version line index
        E_Type_Line_Number_Version_Navi,  // navigation version line index
        E_Type_Line_Number_End,  // end line index
    };

    for (
        size_t i_Line_Index = E_Type_Line_Number_Start;
        i_Line_Index != E_Type_Line_Number_End;
        ++i_Line_Index
    ) {
        // Get line.

        string string_Line;

        Fn_Get_string_Line_Fixed(ifstream_File, string_Line);

        IF (!ifstream_File) {
            // Check stream state: badbit

            IF (ifstream_File.rdstate() & std::ios::badbit) {
                MACRO_Log_Error
                return E_Result_Error;
            }

            // Check StreamFile: eofbit

            IF (ifstream_File.rdstate() & std::ios::eofbit) {
                return E_Result_Succeeded;
            }

            // Check StreamFile: failbit
#ifdef MACRO_Dynamic_Debug
            IF (ifstream_File.rdstate() & std::ios::failbit) {
                MACRO_Log_Error
                return E_Result_Error;
            }
#endif
        }

        switch (i_Line_Index) {
        case E_Type_Line_Number_Version_Static:
            {
                // Get static data version.

                string string_Static_Version_Flag("$VER:");
                string::size_type i_Pos_Static_Version = string_Line.find(string_Static_Version_Flag);
                IF (i_Pos_Static_Version != string::npos) {
                    string string_Static_Version = Fn_Get_string_Without_Blank(
                        string_Line.substr(i_Pos_Static_Version + string_Static_Version_Flag.length())
                    );

                    IF (!Fn_Convert_String_To_Integer(string_Static_Version, ri_Static_Version)) {
                        MACRO_Log_Error
                        return E_Result_Error;
                    }
                }

                break;
            }
        case E_Type_Line_Number_Version_Navi:
            {
                // Get navigation data version.

                string string_Navi_Version_Flag("$NAVIVER:");
                string::size_type i_Pos_Navi_Version = string_Line.find(string_Navi_Version_Flag);
                IF (i_Pos_Navi_Version != string::npos) {
                    string string_Navi_Version = Fn_Get_string_Without_Blank(
                        string_Line.substr(i_Pos_Navi_Version + string_Navi_Version_Flag.length())
                    );

                    IF (!Fn_Convert_String_To_Integer(string_Navi_Version, ri_Navi_Version)) {
                        MACRO_Log_Error
                        return E_Result_Error;
                    }
                }

                break;
            }
        default:
            {
                break;
            }
        }

    }

    return E_Result_Succeeded;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */