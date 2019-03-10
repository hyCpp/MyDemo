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
#include "Vr_Asr_Impl_Vocon_Data_CLC.h"
#include "Vr_Asr_Impl_Vocon_Engine_Concrete.h"
#include "Vr_Asr_Impl_Event_Init_Notify_Impl.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Data_CLC::~C_Data_CLC()
{
    MACRO_Log_Function
}

C_Data_CLC::C_Data_CLC
(
    C_Engine_Concrete & ro_Engine_Concrete,
    string const & rstring_Id_CLC,
    string const & rstring_Path_File_CLC
)
: m_ro_Engine_Concrete(ro_Engine_Concrete)
, m_b_Id_Trans_Lookup(false)
, m_string_Id_CLC(rstring_Id_CLC)
, m_string_Path_File_CLC(rstring_Path_File_CLC)
, m_string_Path_File_Id_Trans_Lookup()
, m_string_Domain_Category_Curr()
, m_o_LH_OBJECT_CLC(G_o_Vocon().M_Fn_LH_GetNullObj())
, m_o_LH_OBJECT_CLC_LEX(G_o_Vocon().M_Fn_LH_GetNullObj())
, m_o_LH_OBJECT_CLC_Cache(G_o_Vocon().M_Fn_LH_GetNullObj())
, m_o_LH_OBJECT_CLC_Id_Trans(G_o_Vocon().M_Fn_LH_GetNullObj())
{
    MACRO_Log_Function
}

E_Result
C_Data_CLC::M_Fn_Init()
{
    MACRO_Log_Function

    ST_ERROR o_ST_ERROR; // = ST_OK;
    LH_ERROR o_LH_ERROR; // = LH_OK;

    stringstream stringstream_Log;


    // CLC

    void * pvoid_Instance_Stream = NULL; // Pointer to the stream.

    LH_ISTREAM_INTERFACE o_LH_ISTREAM_INTERFACE; // The stream interface.

    LH_ISTREAM_PARAMS o_LH_ISTREAM_PARAMS;
    memset(&o_LH_ISTREAM_PARAMS, 0, sizeof(LH_ISTREAM_PARAMS));

    o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamReaderFromFile(
        m_string_Path_File_CLC.c_str(),
        &o_LH_ISTREAM_INTERFACE,
        &pvoid_Instance_Stream
    );

    IF (ST_OK != o_ST_ERROR) {
        Fn_Log("    Path: " + m_string_Path_File_CLC);
        Fn_Log_ST_ERROR(o_ST_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateMLCLC(
        m_ro_Engine_Concrete.M_Fn_Get_ro_LH_COMPONENT_Pron(),
        &o_LH_ISTREAM_INTERFACE,
        pvoid_Instance_Stream,
        &o_LH_ISTREAM_PARAMS,
        NULL,
        &m_o_LH_OBJECT_CLC
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Categories

    char ** ppi_Categories = NULL;
    size_t i_Num_Categorie;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CLCBorrowDomainCategories(
        m_o_LH_OBJECT_CLC,
        &ppi_Categories,
        &i_Num_Categorie
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    stringstream_Log.clear();
    stringstream_Log.str("");
    stringstream_Log << "    " << i_Num_Categorie << " categories found\n";
    Fn_Log(stringstream_Log.str());

    for (
        size_t i_Pos_Categorie = 0;
        i_Pos_Categorie < i_Num_Categorie;
        ++i_Pos_Categorie
    ) {
        string string_Category = ppi_Categories[i_Pos_Categorie];

        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "        " << i_Pos_Categorie << " "<< string_Category;
        Fn_Log(stringstream_Log.str());

        IF (m_string_Domain_Category_Curr.empty()) {

            m_string_Domain_Category_Curr = string_Category;
        }

        map<string, C_Data_CLC * >::iterator iterator_pair_string_po_Data_CLC_Find;

        iterator_pair_string_po_Data_CLC_Find = m_ro_Engine_Concrete.m_map_string_po_Data_CLC.find(
            string_Category
        );

        IF (iterator_pair_string_po_Data_CLC_Find == m_ro_Engine_Concrete.m_map_string_po_Data_CLC.end()) {

            m_ro_Engine_Concrete.m_map_string_po_Data_CLC.insert(
                std::pair<string, C_Data_CLC * >(
                    string_Category,
                    this
                )
            );
        }
    }

    // Languages

    char ** ppi_Sec_Languages = NULL;
    size_t i_Num_Sec_Language;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CLCFetchInputLanguages(
        m_o_LH_OBJECT_CLC,
        &ppi_Sec_Languages,
        &i_Num_Sec_Language);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    stringstream_Log.clear();
    stringstream_Log.str("");
    stringstream_Log << "    " << i_Num_Sec_Language << " Secondary Languages found\n";
    Fn_Log(stringstream_Log.str());

    for (
        size_t i_Pos_Sec_Language = 0;
        i_Pos_Sec_Language < i_Num_Sec_Language;
        ++i_Pos_Sec_Language
    ) {
        stringstream_Log.clear();
        stringstream_Log.str("");
        stringstream_Log << "        " << i_Pos_Sec_Language << " "<< ppi_Sec_Languages[i_Pos_Sec_Language];
        Fn_Log(stringstream_Log.str());
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CLCReturnInputLanguages(
        m_o_LH_OBJECT_CLC,
        ppi_Sec_Languages);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Category set

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CLCSetCurrentDomainCategory(
        m_o_LH_OBJECT_CLC,
        m_string_Domain_Category_Curr.c_str()
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Get the transcription specs from the ASR component.
    LH_TRANSCRIPTION_SPEC * po_LH_TRANSCRIPTION_SPEC = NULL;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_TransSpecFetchValue(
        m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Mod(),
        &po_LH_TRANSCRIPTION_SPEC);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Lexicon object.
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateLex(
        m_ro_Engine_Concrete.M_Fn_Get_ro_LH_COMPONENT_Pron(),
        po_LH_TRANSCRIPTION_SPEC,
        &m_o_LH_OBJECT_CLC_LEX);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_TransSpecReturnValue(
        m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Mod(),
        po_LH_TRANSCRIPTION_SPEC);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Connect Lexicon object with the CLC Object.

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_LexSetG2P(
        m_o_LH_OBJECT_CLC_LEX,
        m_o_LH_OBJECT_CLC,
        m_o_LH_OBJECT_CLC_Cache
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Checks if an object supports a particular interface. An object can support multiple interfaces.

    LH_BOOL o_LH_BOOL_Has_Interface = LH_FALSE;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjHasInterface(
        m_o_LH_OBJECT_CLC_LEX,
        LH_IID_ITRANSLOOKUP,
        &o_LH_BOOL_Has_Interface
    );

    IF (!o_LH_BOOL_Has_Interface) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_CLC::M_Fn_Fina()
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    IF (!G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_CLC_LEX)) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_LexClearG2P(m_o_LH_OBJECT_CLC_LEX);

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }

        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_CLC_LEX);

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }
    }

    M_Fn_Fina_Id_Trans_Lookup();

    IF (!G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_CLC)) {
        o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_CLC);

        IF (LH_OK != o_LH_ERROR) {
            Fn_Log_LH_ERROR(o_LH_ERROR);
            MACRO_Log_Error
            return E_Result_Failed;
        }
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_CLC::M_Fn_Init_Id_Trans_Lookup
(
    string const& ro_string_Id_Trans_Version
)
{
    MACRO_Log_Function

    ST_ERROR o_ST_ERROR; // = ST_OK;
    LH_ERROR o_LH_ERROR; // = LH_OK;

    void * pvoid_Instance_Stream = NULL; // Pointer to the stream.

    LH_ISTREAM_INTERFACE o_LH_ISTREAM_INTERFACE; // The stream interface.

    LH_ISTREAM_PARAMS o_LH_ISTREAM_PARAMS;

    // CLC Id Trans
    memset(&o_LH_ISTREAM_PARAMS, 0, sizeof(LH_ISTREAM_PARAMS));

    o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamReaderFromFile(
        m_string_Path_File_CLC.c_str(),
        &o_LH_ISTREAM_INTERFACE,
        &pvoid_Instance_Stream
    );
    IF (ST_OK != o_ST_ERROR) {
        Fn_Log("    Path: " + m_string_Path_File_CLC);
        Fn_Log_ST_ERROR(o_ST_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Get the transcription specs from the ASR component.
    LH_TRANSCRIPTION_SPEC * po_LH_TRANSCRIPTION_SPEC = NULL;
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_TransSpecFetchValue(
        m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Mod(),
        &po_LH_TRANSCRIPTION_SPEC);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CreateMLCLC(
        m_ro_Engine_Concrete.M_Fn_Get_ro_LH_COMPONENT_Pron(),
        &o_LH_ISTREAM_INTERFACE,
        pvoid_Instance_Stream,
        &o_LH_ISTREAM_PARAMS,
        po_LH_TRANSCRIPTION_SPEC,
        &m_o_LH_OBJECT_CLC_Id_Trans);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_TransSpecReturnValue(
        m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Mod(),
        po_LH_TRANSCRIPTION_SPEC);
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    // Load
    memset(&o_LH_ISTREAM_PARAMS, 0, sizeof(LH_ISTREAM_PARAMS));

    o_ST_ERROR = G_o_Vocon().M_Fn_ST_CreateStreamReaderFromFile(
        m_string_Path_File_Id_Trans_Lookup.c_str(),
        &o_LH_ISTREAM_INTERFACE,
        &pvoid_Instance_Stream
    );
    IF (ST_OK != o_ST_ERROR) {
        Fn_Log("    Path: " + m_string_Path_File_Id_Trans_Lookup);
        Fn_Log_ST_ERROR(o_ST_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    LH_IDLOOKUP_STATUS o_LH_IDLOOKUP_STATUS;
    memset(&o_LH_IDLOOKUP_STATUS, 0, sizeof(LH_IDLOOKUP_STATUS));
    Fn_Log("The id trans version is " + ro_string_Id_Trans_Version);
    o_LH_ERROR = G_o_Vocon().M_Fn_LH_IdTransLookupLoadData(
        m_o_LH_OBJECT_CLC_Id_Trans,
        &o_LH_ISTREAM_INTERFACE,
        pvoid_Instance_Stream,
        &o_LH_ISTREAM_PARAMS,
        ro_string_Id_Trans_Version.c_str(),
        &o_LH_IDLOOKUP_STATUS
    );
    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (!o_LH_IDLOOKUP_STATUS.loaded) {
        Fn_Log("    The id dictionary could not be loaded onto CLC.");
        MACRO_Log_Error
        return E_Result_Failed;
    }

    IF (o_LH_IDLOOKUP_STATUS.compatible) {
        Fn_Log("The id trans verson is compatible");
        m_b_Id_Trans_Lookup = true;
    }
    ELSE {
        Fn_Log("The id trans verson is not compatible");
        m_b_Id_Trans_Lookup = false;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_CLC::M_Fn_Fina_Id_Trans_Lookup()
{
    MACRO_Log_Function

    IF (G_o_Vocon().M_Fn_LH_ObjIsNull(m_o_LH_OBJECT_CLC_Id_Trans)) {

        return E_Result_Succeeded;
    }

    LH_ERROR o_LH_ERROR; // = LH_OK;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_IdTransLookupUnloadData(
        m_o_LH_OBJECT_CLC_Id_Trans
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_ObjClose(&m_o_LH_OBJECT_CLC_Id_Trans);

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    m_b_Id_Trans_Lookup = false;

    return E_Result_Succeeded;
}

E_Result
C_Data_CLC::M_Fn_Id_Trans_Version_Update
(
    string const& ro_string_Id_Trans_Version,
    int & b_Id_Trans_Version_Supported
)
{
    MACRO_Log_Function

    E_Result e_Result;

    b_Id_Trans_Version_Supported = false;

    e_Result = M_Fn_Fina_Id_Trans_Lookup();
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    e_Result = M_Fn_Init_Id_Trans_Lookup(ro_string_Id_Trans_Version);
    IF (E_Result_Succeeded != e_Result) {
        MACRO_Log_Error
        return E_Result_Failed;
    }

    b_Id_Trans_Version_Supported = m_b_Id_Trans_Lookup;

    return E_Result_Succeeded;
}


E_Result
C_Data_CLC::M_Fn_Domain_Category_Set
(
    string const & string_Domain_Category,
    string& ro_string_Domain_Category_Prev
)
{
    MACRO_Log_Function

    LH_ERROR o_LH_ERROR; // = LH_OK;

    Fn_Log("    CLC: " + m_string_Id_CLC);

    Fn_Log("    Domain Category Prev: " + m_string_Domain_Category_Curr);

    ro_string_Domain_Category_Prev = m_string_Domain_Category_Curr;

    IF (m_string_Domain_Category_Curr == string_Domain_Category) {

        return E_Result_Succeeded;
    }

    m_string_Domain_Category_Curr = string_Domain_Category;

    o_LH_ERROR = G_o_Vocon().M_Fn_LH_CLCSetCurrentDomainCategory(
        m_o_LH_OBJECT_CLC,
        m_string_Domain_Category_Curr.c_str()
    );

    IF (LH_OK != o_LH_ERROR) {
        Fn_Log_LH_ERROR(o_LH_ERROR);
        MACRO_Log_Error
        return E_Result_Failed;
    }

    Fn_Log("    Domain Category Curr: " + m_string_Domain_Category_Curr);

    return E_Result_Succeeded;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */