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
#include "Vr_Asr_Impl_Fn_Get_string_Path.h"
#include "Vr_Asr_Impl_Fn_Get_string_Line_Fixed.h"
#include "Vr_Asr_Impl_Kaldi_Engine_Concrete.h"
#include "Vr_Asr_Impl_Kaldi_Mgr_Rec.h"
#include "Vr_Asr_Impl_Kaldi_Fn_Callback_Rec_Event.h"
#include "Vr_Asr_Impl_Kaldi_Fn_Callback_Rec_Advance.h"
#include "Vr_Asr_Impl_Engine_Agent.h"
#include "Vr_Asr_Impl_Event_Phase_Impl.h"
#include "Vr_Asr_Impl_Event_Result_Impl.h"
#include "Vr_Asr_Impl_Event_Notify_Impl.h"
#include "Vr_Asr_Impl_Event_Notify_Impl_Signal_Level.h"
#include "Vr_Asr_Impl_Event_Init_Notify_Impl.h"
#include "Vr_Asr_Impl_Fn_Dir_File.h"


namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Kaldi
{

C_Engine_Concrete::~C_Engine_Concrete()
{
    MACRO_Log_Function
}

C_Engine_Concrete::C_Engine_Concrete
(
    C_Engine_Agent & ro_Engine_Agent,
    string const & rstring_Id_Language,
    boost::function<void(C_Event_Init_Notify const &)>& ro_function_On_Event_Init_Notify
)
: m_ro_Engine_Agent(ro_Engine_Agent)
, m_string_Id_Language(rstring_Id_Language)
, m_function_On_Event_Init_Notify(ro_function_On_Event_Init_Notify)
, m_scpo_Mgr_Rec(MACRO_New C_Mgr_Rec(*this))
, m_b_Init(false)
, m_b_Error(false)
, m_o_Log_File()
, m_o_HKALDI(NULL)
, m_string_Model_Model()
, m_string_Model_HCLG()
, m_string_Model_Word_Syms()
, m_string_Model_Silence_Phones()
, m_string_Model_Lad_Mat()
, m_b_Log_File(false)
{
    MACRO_Log_Function
}


E_Result
C_Engine_Concrete::M_Fn_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    IF (m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }
    m_b_Init = true;

    IF (m_b_Error) {
        MACRO_Log_Error
        return E_Result_Error;
    }

/*
    e_Result = _M_Fn_Data_Init();
    IF (E_Result_Succeeded != e_Result) {
        m_b_Error = true;
        MACRO_Log_Error
        return E_Result_Error;
    }
*/
    e_Result = _M_Fn_Mgr_Init();
    IF (E_Result_Succeeded != e_Result) {
        m_b_Error = true;
        MACRO_Log_Error
        return E_Result_Error;
    }
/*
    e_Result = _M_Fn_Kaldi_Init();
    IF (E_Result_Succeeded != e_Result) {
        m_b_Error = true;
        MACRO_Log_Error
        return E_Result_Error;
    }
*/
    m_b_Error = false;

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Fina()
{
    MACRO_Log_Function

    E_Result e_Result;

    m_b_Init = false;

    m_b_Error = true;
/*
    e_Result = _M_Fn_Kaldi_Fina();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
    }
*/
    e_Result = _M_Fn_Mgr_Fina();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
    }
/*
    e_Result = _M_Fn_Data_Fina();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
    }
*/
    m_b_Error = false;

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Data_Context_List_Update
(
    C_Request_Context_List_Update & ro_Request_Context_List_Update
)
{
    MACRO_Log_Function

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Rec_Start
(
    C_Request_Recognize & ro_Request_Recognize
)
{
    MACRO_Log_Function

    E_Result e_Result;

    IF (!m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (m_b_Error) {
        MACRO_Log_Error

        e_Result = M_Fn_Fina();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        e_Result = M_Fn_Init();
        IF (e_Result != E_Result_Succeeded) {
            MACRO_Log_Error
            return e_Result;
        }

        IF (m_b_Error) {
            MACRO_Log_Error
            return E_Result_Error;
        }
    }

    return m_scpo_Mgr_Rec->M_Fn_Start(ro_Request_Recognize);

}

E_Result
C_Engine_Concrete::M_Fn_Rec_Cancel()
{
    MACRO_Log_Function

    IF (!m_b_Init) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (m_b_Error) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return m_scpo_Mgr_Rec->M_Fn_Cancel();
}

E_Result
C_Engine_Concrete::M_Fn_Party_Load
(
    string const & rstring_Id_Party
)
{
    MACRO_Log_Function

    MACRO_Log_Error

    return E_Result_Succeeded;
}


E_Result
C_Engine_Concrete::M_Fn_Party_Unload
(
    string const & rstring_Id_Party
)
{
    MACRO_Log_Function

    MACRO_Log_Error

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::M_Fn_Param_Set
(
    C_Request_Param_Set & ro_Request_Param_Set
)
{
    MACRO_Log_Function

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Mgr_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    e_Result = m_scpo_Mgr_Rec->M_Fn_Init();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Mgr_Fina()
{
    MACRO_Log_Function

    E_Result e_Result;

    e_Result = m_scpo_Mgr_Rec->M_Fn_Fina();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}



/*
E_Result
C_Engine_Concrete::_M_Fn_Kaldi_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    int i_ret = 0;

    KALDI_EVENT_INTERFACE o_KALDI_EVENT_INTERFACE;
    o_KALDI_EVENT_INTERFACE.pfevent = Fn_Callback_Rec_Event;
    o_KALDI_EVENT_INTERFACE.pfadvance = Fn_Callback_Rec_Advance;

    i_ret = G_o_Kaldi().M_Fn_Kaldi_Create(&o_KALDI_EVENT_INTERFACE, this, &m_o_HKALDI);
    IF (0 != i_ret) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    i_ret = G_o_Kaldi().M_Fn_Kaldi_Model_Load(m_o_HKALDI, E_Kaldi_Mode_Type_Silence_Phones, m_string_Model_Silence_Phones);
    IF (0 != i_ret) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (!Fn_File_Access(m_string_Model_Model)) {
        Fn_Log(m_string_Model_Model + "is not exist");
        MACRO_Log_Error
        return E_Result_Error;
    }
    i_ret = G_o_Kaldi().M_Fn_Kaldi_Model_Load(m_o_HKALDI, E_Kaldi_Mode_Type_Mode, m_string_Model_Model);
    IF (0 != i_ret) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (!Fn_File_Access(m_string_Model_HCLG)) {
        Fn_Log(m_string_Model_HCLG + "is not exist");
        MACRO_Log_Error
        return E_Result_Error;
    }
    i_ret = G_o_Kaldi().M_Fn_Kaldi_Model_Load(m_o_HKALDI, E_Kaldi_Mode_Type_HCLG, m_string_Model_HCLG);
    IF (0 != i_ret) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (!Fn_File_Access(m_string_Model_Word_Syms)) {
        Fn_Log(m_string_Model_Word_Syms + "is not exist");
        MACRO_Log_Error
        return E_Result_Error;
    }
    i_ret = G_o_Kaldi().M_Fn_Kaldi_Model_Load(m_o_HKALDI, E_Kaldi_Mode_Type_Word_Syms, m_string_Model_Word_Syms);
    IF (0 != i_ret) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    IF (!Fn_File_Access(m_string_Model_Lad_Mat)) {
        Fn_Log(m_string_Model_Lad_Mat + "is not exist");
        MACRO_Log_Error
        return E_Result_Error;
    }
    i_ret = G_o_Kaldi().M_Fn_Kaldi_Model_Load(m_o_HKALDI, E_Kaldi_Mode_Type_Lad_Mat, m_string_Model_Lad_Mat);
    IF (0 != i_ret) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Kaldi_Fina()
{
    MACRO_Log_Function

    int i_ret = G_o_Kaldi().M_Fn_Kaldi_Destory(m_o_HKALDI);
    IF (0 != i_ret) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    return E_Result_Succeeded;
}
*/

E_Result
C_Engine_Concrete::_M_Fn_Data_Init()
{
    E_Result e_Result;

    // Mode file
    e_Result = _M_Fn_Info_Parse_File();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    // log enable
    m_o_Log_File.M_Fn_Enable_Log(m_b_Log_File);
    m_o_Log_File.M_Fn_Init(G_o_Get_Path().M_Fn_Get_string_Path_Usr() + "Log.txt");

    return E_Result_Succeeded;
}

E_Result
C_Engine_Concrete::_M_Fn_Data_Fina()
{
    m_string_Model_Model = "";
    m_string_Model_HCLG = "";
    m_string_Model_Word_Syms = "";
    m_string_Model_Silence_Phones = "";
    m_string_Model_Lad_Mat = "";

    m_o_Log_File.M_Fn_Fina();
    m_b_Log_File = false;

    return E_Result_Succeeded;
}


E_Result
C_Engine_Concrete::_M_Fn_Info_Parse_File()
{
    MACRO_Log_Function

    E_Result e_Result;

    std::ifstream ifstream_File;
    string string_Path_File_Info = G_o_Get_Path().M_Fn_Get_string_Path_VData() + "Info.dat";
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

        IF (string_Key == "MODEL") {
            e_Result = _M_Fn_Info_Parse_Model(ifstream_File);
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
C_Engine_Concrete::_M_Fn_Info_Parse_Line
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

        IF (rifstream_File.rdstate() & std::ios::failbit) {
            MACRO_Log_Error
            return E_Result_Error;
        }
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
C_Engine_Concrete::_M_Fn_Info_Parse_Model
(
    ifstream & rifstream_File
)
{
    E_Result e_Result;

    string string_Id_Dialog;
    size_t i_find_pos = string::npos;
    int i_Id_Context = 0;
    string string_Id_Context;

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

        IF (string_Key == "Model_Model") {
            m_string_Model_Model = G_o_Get_Path().M_Fn_Get_string_Path_VData() + string_Value;
        }
        ELSE
        IF (string_Key == "Model_HCLG") {
            m_string_Model_HCLG = G_o_Get_Path().M_Fn_Get_string_Path_VData() + string_Value;
        }
        ELSE
        IF (string_Key == "Model_Word_Syms") {
            m_string_Model_Word_Syms = G_o_Get_Path().M_Fn_Get_string_Path_VData() + string_Value;
        }
        ELSE
        IF (string_Key == "Model_Silence_Phones") {
            m_string_Model_Silence_Phones = string_Value;
        }
        ELSE
        IF (string_Key == "Model_Lad_Mat") {
            m_string_Model_Lad_Mat = G_o_Get_Path().M_Fn_Get_string_Path_VData() + string_Value;
        }
        ELSE
        IF (string_Key == "MODEL_END") {
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

} // name N_Kaldi

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
