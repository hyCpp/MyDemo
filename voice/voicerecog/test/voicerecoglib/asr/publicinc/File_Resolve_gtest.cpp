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

#if defined(_WIN32) || defined(_WIN32_WCE)
#include "stdafx.h"
#endif

#include <iostream>
#include <sstream>
#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Engine.h"
#include "File_Resolve_gtest.h"
#include "Record_Log.h"
#include "Common_String_Define.h"

C_File_Resolve_gtest::C_File_Resolve_gtest()
{

}

C_File_Resolve_gtest::~C_File_Resolve_gtest()
{

}

int
C_File_Resolve_gtest::Resolve_Wave_Info_Data
(
    const std::string& str_file_name,
    std::list<C_Test_Wave_Info_gtest>& list_wave_info
)
{
    std::ifstream ifstream_File;
    ifstream_File.open(str_file_name.c_str());
    if (!ifstream_File.is_open()) {
        MACRO_Log_Error
        return -1;
    }

    char c_directory_flag = 0;

#if defined(_WIN32) || defined(_WIN32_WCE)
    c_directory_flag = '\\';
#else
    c_directory_flag = '/';
#endif


    std::string string_path = std::string(str_file_name, 0, str_file_name.find_last_of(c_directory_flag) + 1);

    // BOM

    char i_BOM;

    ifstream_File >> i_BOM >> i_BOM >> i_BOM;

    std::string string_Line;

    Get_string_Line_Fixed(ifstream_File, string_Line);

    // Parse lines.

    bool b_Eof;
    std::string string_Key;
    std::string string_Value;
    C_Test_Wave_Info_gtest temp_wave_test_config;

    list_wave_info.clear();
    while (true) {
        int nRetValue = Info_Parse_Line(
                            ifstream_File,
                            string_Key,
                            string_Value,
                            b_Eof
                        );

        if (nRetValue != 0) {
            return nRetValue;
        }

        if (b_Eof) {
            break;
        }

        if (string_Key == "TEST_ITEM_CONFIG") {
            nRetValue = Info_Parse_Test_Item_Config(ifstream_File, temp_wave_test_config);
        }
        else {
            continue;
        }

        if (nRetValue != 0) {
            return nRetValue;
        }

        temp_wave_test_config.string_wave_file_name = string_path + temp_wave_test_config.string_wave_file_name;
        list_wave_info.push_back(temp_wave_test_config);
    }

    ifstream_File.close();

    return 0;
}

int
C_File_Resolve_gtest::Resolve_Info_Data
(
    const std::string& str_file_name,
    const std::string& string_item_type,
    const std::string& string_context_type,
    std::set<std::string>& set_item_value
)
{
    std::ifstream ifstream_File;
    ifstream_File.open(str_file_name.c_str());
    if (!ifstream_File.is_open()) {
        MACRO_Log_Error
        return -1;
    }

    // BOM

    char i_BOM;

    ifstream_File >> i_BOM >> i_BOM >> i_BOM;

    std::string string_Line;

    Get_string_Line_Fixed(ifstream_File, string_Line);

    // Parse lines.

    std::string string_Key;
    std::string string_Value;
    std::string string_type;
    std::string string_item_value;
    bool b_Eof;

    string_type = string_context_type;
    set_item_value.clear();
    while (true) {
        int nRetValue = Info_Parse_Line(
                            ifstream_File,
                            string_Key,
                            string_Value,
                            b_Eof
                        );

        if (nRetValue != 0) {
            return nRetValue;
        }

        if (b_Eof) {
            break;
        }

        if (string_Key == "DIALOG") {
            nRetValue = Info_Parse_Dialog(ifstream_File, string_item_value);
        }
        else if (string_Key == "CONTEXT") {
            nRetValue = Info_Parse_Context(ifstream_File, string_type, string_item_value);
        }
        else if (string_Key == "TAG") {
            nRetValue = Info_Parse_Tag(ifstream_File, string_item_value);
        }
        else if (string_Key == "RULE") {
            nRetValue = Info_Parse_Rule(ifstream_File, string_item_value);
        }
        else {
            continue;
        }

        if (nRetValue != 0) {
            return nRetValue;
        }

        if ((string_item_type == string_Key)
            && (!string_item_value.empty())) {
            set_item_value.insert(string_item_value);
        }

    }

    ifstream_File.close();

    return 0;
}

#if 0
int
C_File_Resolve_gtest::Resovle_Fuzzy_Compile_Data
(
    const std::string& str_file_name,
    std::vector<boost::shared_ptr<N_Vr::N_Asr::C_Term> >& vector_spo_Term_Column,
    list<vector<boost::shared_ptr<N_Vr::N_Asr::C_Term> > >& list_vector_spo_Term_Column_Data
)
{
    std::ifstream ifstream_File;
    ifstream_File.open(str_file_name.c_str());
    if (!ifstream_File.is_open()) {
        MACRO_Log_Error
        return -1;
    }

    vector_spo_Term_Column.clear();
    spC_Term column_artist(new N_Vr::N_Asr::C_Term);
    column_artist->m_string_Orthography = "artist";
    column_artist->m_i_User_Data_Lo = FUZZYMATCHCTXCPL_COLUMN_SPEAKABLE | FUZZYMATCHCTXCPL_COLUMN_ACTIVATABLE | FUZZYMATCHCTXCPL_COLUMN_RESULTACCUMULATE;
    vector_spo_Term_Column.push_back(column_artist);

    spC_Term column_artist_id(new N_Vr::N_Asr::C_Term);
    column_artist_id->m_string_Orthography = "artist_id";
    column_artist_id->m_i_User_Data_Lo = FUZZYMATCHCTXCPL_COLUMN_INTEGER | FUZZYMATCHCTXCPL_COLUMN_ACTIVATABLE | FUZZYMATCHCTXCPL_COLUMN_RESULTACCUMULATE;
    vector_spo_Term_Column.push_back(column_artist_id);

    spC_Term column_album(new N_Vr::N_Asr::C_Term);
    column_album->m_string_Orthography = "album";
    column_album->m_i_User_Data_Lo = FUZZYMATCHCTXCPL_COLUMN_SPEAKABLE | FUZZYMATCHCTXCPL_COLUMN_ACTIVATABLE | FUZZYMATCHCTXCPL_COLUMN_RESULTACCUMULATE;
    vector_spo_Term_Column.push_back(column_album);

    spC_Term column_album_id(new N_Vr::N_Asr::C_Term);
    column_album_id->m_string_Orthography = "album_id";
    column_album_id->m_i_User_Data_Lo = FUZZYMATCHCTXCPL_COLUMN_INTEGER | FUZZYMATCHCTXCPL_COLUMN_ACTIVATABLE | FUZZYMATCHCTXCPL_COLUMN_RESULTACCUMULATE;
    vector_spo_Term_Column.push_back(column_album_id);

    spC_Term column_song(new N_Vr::N_Asr::C_Term);
    column_song->m_string_Orthography = "song";
    column_song->m_i_User_Data_Lo = FUZZYMATCHCTXCPL_COLUMN_SPEAKABLE | FUZZYMATCHCTXCPL_COLUMN_ACTIVATABLE | FUZZYMATCHCTXCPL_COLUMN_RESULTACCUMULATE;
    vector_spo_Term_Column.push_back(column_song);

    spC_Term column_song_id(new N_Vr::N_Asr::C_Term);
    column_song_id->m_string_Orthography = "song_id";
    column_song_id->m_i_User_Data_Lo = FUZZYMATCHCTXCPL_COLUMN_INTEGER | FUZZYMATCHCTXCPL_COLUMN_ACTIVATABLE | FUZZYMATCHCTXCPL_COLUMN_RESULTACCUMULATE;
    vector_spo_Term_Column.push_back(column_song_id);

    // BOM

    char i_BOM;

    ifstream_File >> i_BOM >> i_BOM >> i_BOM;

    std::string string_Line;

    Get_string_Line_Fixed(ifstream_File, string_Line);

    std::string string_column_artist;
    std::string string_column_artist_id;
    std::string string_column_album;
    std::string string_column_album_id;
    std::string string_column_song;
    std::string string_column_song_id;
    bool b_Eof;

    list_vector_spo_Term_Column_Data.clear();
    while (true) {
        int nRetValue = Info_Parse_Line(
                            ifstream_File,
                            string_column_artist,
                            string_column_artist_id,
                            string_column_album,
                            string_column_album_id,
                            string_column_song,
                            string_column_song_id,
                            b_Eof
                        );

        if (nRetValue != 0) {
            return nRetValue;
        }

        if (b_Eof) {
            break;
        }

        vector<boost::shared_ptr<N_Vr::N_Asr::C_Term> > vector_Entry;

        spC_Term column_artist(new N_Vr::N_Asr::C_Term);
        column_artist->m_string_Orthography = string_column_artist;
        vector_Entry.push_back(column_artist);

        spC_Term column_artist_id(new N_Vr::N_Asr::C_Term);
        column_artist_id->m_i_User_Data_Lo = atoi(string_column_artist_id.c_str());
        vector_Entry.push_back(column_artist_id);

        spC_Term column_album(new N_Vr::N_Asr::C_Term);
        column_album->m_string_Orthography = string_column_album;
        vector_Entry.push_back(column_album);

        spC_Term column_album_id(new N_Vr::N_Asr::C_Term);
        column_album_id->m_i_User_Data_Lo = atoi(string_column_album_id.c_str());
        vector_Entry.push_back(column_album_id);

        spC_Term column_song(new N_Vr::N_Asr::C_Term);
        column_song->m_string_Orthography = string_column_song;
        vector_Entry.push_back(column_song);

        spC_Term column_song_id(new N_Vr::N_Asr::C_Term);
        column_song_id->m_i_User_Data_Lo = atoi(string_column_song_id.c_str());
        vector_Entry.push_back(column_song_id);

        list_vector_spo_Term_Column_Data.push_back(vector_Entry);

    }

    return 0;
}

#endif

int
C_File_Resolve_gtest::Info_Parse_Rule
(
    std::ifstream & rifstream_File,
    std::string& string_item_value
)
{
    return 0;
}

int
C_File_Resolve_gtest::Info_Parse_Tag
(
    std::ifstream & rifstream_File,
    std::string& string_item_value
)
{
    // Parse lines.

    std::string string_Key;
    std::string string_Value;
    bool b_Eof;

    while (true) {
        int nRetValue = Info_Parse_Line(
                            rifstream_File,
                            string_Key,
                            string_Value,
                            b_Eof
                        );

        if (nRetValue != 0) {
            MACRO_Log_Error
            return -1;
        }

        if (b_Eof) {
            MACRO_Log_Error
            return -1;
        }


        if (string_Key == "TAG_END") {
            break;
        }
        else {
            continue;
        }
    }

    return 0;
}



void
C_File_Resolve_gtest::Resovle_Context
(
    const pugi::xml_node& node,
    C_List_Context_Item_gtest& context_item
)
{
    std::string string_slot_item_name;
    std::string string_slot_oper;
    pugi::xml_node slot_node;
    pugi::xml_node context_node;

    context_node = node;
    context_item.m_list_context_add_item.clear();
    context_item.m_list_context_del_item.clear();
    context_item.m_string_list_context_name = context_node.attribute(NODE_ITEM_NAME).as_string();

    slot_node = context_node.first_child();
    while (!slot_node.empty()) {
        int n_slot_id = slot_node.attribute(NODE_ITEM_ID).as_int();
        string_slot_item_name = slot_node.attribute(NODE_ITEM_NAME).as_string();
        string_slot_oper = slot_node.attribute(NODE_ITEM_OPER).as_string();

        spC_Term item(new N_Vr::N_Asr::C_Term);
        item->m_i_User_Data_Lo = n_slot_id;
        item->m_string_Orthography = string_slot_item_name;

        if (NODE_ITEM_OPER_VALUE_ADD == string_slot_oper) {
            context_item.m_list_context_add_item.push_back(item);
        }
        else {
            context_item.m_list_context_del_item.push_back(item);
        }

        slot_node = slot_node.next_sibling();
    }
}



int
C_File_Resolve_gtest::Resovle_List_Context_Xml
(
    const std::string& string_file_name,
    std::map<std::string, C_List_Context_Item_gtest>& list_context_and_item
)
{
    // std::string string_node_name;
    pugi::xml_node node;
    pugi::xml_parse_result xml_result;
    pugi::xml_document list_context_xml;
    C_List_Context_Item_gtest context_item;
    xml_result = list_context_xml.load_file(string_file_name.c_str());
    if (!xml_result) {
        MACRO_Log_Error
        return -1;
    }

    node = list_context_xml.first_child();
    while (!node.empty()) {
        std::string string_node_name = node.name();
        if (NODE_CONTEXT != string_node_name) {
            MACRO_Log_Error
            return -1;
        }

        Resovle_Context(node, context_item);
        list_context_and_item[context_item.m_string_list_context_name] = context_item;

        node = node.next_sibling();
    }

    return 0;
}

int
C_File_Resolve_gtest::Resovle_Voicerecog_Result_Xml
(
    const std::string& string_voicerecog_resutl_xml,
    bool b_actual_result,
    std::vector<std::map<std::string, C_Result_Node_gtest> >& vector_result
)
{
    std::string result_label;
    std::string string_node_name;
    pugi::xml_node node;
    pugi::xml_parse_result xml_result;
    pugi::xml_document voicerecog_result_xml;

    xml_result = voicerecog_result_xml.load_string(string_voicerecog_resutl_xml.c_str());
    if (!xml_result) {
        MACRO_Log_Error
        return -1;
    }

    node = voicerecog_result_xml.first_child();
    if (node.empty()) {
        MACRO_Log_Error
        return -1;
    }

    if (b_actual_result) {
        result_label = NODE_RESULT;
    }
    else {
        result_label = NODE_EXPECT_RESULT;
    }

    string_node_name = node.name();
    if (string_node_name != result_label) {
        MACRO_Log_Error
        return -1;
    }

    vector_result.clear();
    node = node.first_child();
    while (!node.empty()) {
        // string_node_name = node.name();

        int int_ret_value = Resovle_Voicerecog_Result_Node(node, vector_result);
        if (0 != int_ret_value) {
            MACRO_Log_Error
            return -1;
        }

        node = node.next_sibling();
    }


    return 0;
}

int
C_File_Resolve_gtest::Resovle_Voicerecog_Result_Node
(
    const pugi::xml_node& node,
    std::vector<std::map<std::string, C_Result_Node_gtest> >& vector_result
)
{
    std::string string_name;

    pugi::xml_node slot_node;
    pugi::xml_node intent_node;
    pugi::xml_node hypothesis_node;

    C_Result_Node_gtest temp_result_node;
    std::map<std::string, C_Result_Node_gtest> map_result_node;

    // hypothesis node
    hypothesis_node = node;
    if (hypothesis_node.empty()) {
        MACRO_Log_Error
        return -1;
    }

    string_name = hypothesis_node.attribute(NODE_ITEM_NAME).as_string();
    if (string_name != NODE_ITEM_NAME_VALUE_HYPOTHESIS) {
        MACRO_Log_Error
        return -1;
    }
    temp_result_node.string_value = hypothesis_node.attribute(NODE_ITEM_VALUE).as_string();
    temp_result_node.int_confidence = hypothesis_node.attribute(NODE_ITEM_CONFIDENCE).as_int();

    map_result_node[string_name] = temp_result_node;

    // intent node
    intent_node = hypothesis_node.first_child();
    if (intent_node.empty()) {
        MACRO_Log_Error
        return -1;
    }

    string_name = intent_node.attribute(NODE_ITEM_NAME).as_string();
    if (NODE_ITEM_NAME_VALUE_INTENT != string_name) {
        MACRO_Log_Error
        return -1;
    }

    temp_result_node.string_value = intent_node.attribute(NODE_ITEM_VALUE).as_string();
    temp_result_node.int_confidence = intent_node.attribute(NODE_ITEM_CONFIDENCE).as_int();

    map_result_node[string_name] = temp_result_node;

    // slot node
    slot_node = intent_node.first_child();
    while (!slot_node.empty()) {
        string_name = slot_node.attribute(NODE_ITEM_NAME).as_string();

        temp_result_node.string_value = slot_node.attribute(NODE_ITEM_VALUE).as_string();
        temp_result_node.int_confidence = slot_node.attribute(NODE_ITEM_CONFIDENCE).as_int();

        map_result_node[string_name] = temp_result_node;

        slot_node = slot_node.next_sibling();
    }

    vector_result.push_back(map_result_node);

    return 0;
}


int
C_File_Resolve_gtest::Wave_Info_Parse_Line
(
    std::ifstream & rifstream_File,
    std::string & string_party_id,
    std::string & string_dialog_id,
    std::string & string_wave_file_name,
    bool & rb_Eof
)
{
    string_party_id.clear();
    string_dialog_id.clear();
    string_wave_file_name.clear();
    rb_Eof = false;

    // Get line.

    std::string string_Line;

    Get_string_Line_Fixed(rifstream_File, string_Line);

    if (!rifstream_File) {
        // Check stream state: badbit

        if (rifstream_File.rdstate() & std::ios::badbit) {
            MACRO_Log_Error
            return -1;
        }

        // Check StreamFile: eofbit

        if (rifstream_File.rdstate() & std::ios::eofbit) {
            rb_Eof = true;
            return 0;
        }

        // Check StreamFile: failbit

        if (rifstream_File.rdstate() & std::ios::failbit) {
            MACRO_Log_Error
            return -1;
        }
    }

    // Remove trailing comment.

    std::string::size_type i_Pos_Char_Dollar = string_Line.find('$');
    if (i_Pos_Char_Dollar != std::string::npos) {
        string_Line = string_Line.substr(0, i_Pos_Char_Dollar);
    }

    // Get string.

    std::stringstream stringstream_Line;
    stringstream_Line.clear();
    stringstream_Line.str(string_Line);

    // Get party id.

    if (!stringstream_Line) {
        return 0;
    }
    stringstream_Line >> string_party_id;

    // Get dialog id.

    if (!stringstream_Line) {
        return 0;
    }
    std::getline(stringstream_Line, string_dialog_id);
    string_dialog_id = Get_string_Without_Blank(string_dialog_id);

    stringstream_Line.clear();
    stringstream_Line.str(string_dialog_id);
    if (!stringstream_Line) {
        return 0;
    }
    stringstream_Line >> string_dialog_id;


    // Get file name.

    if (!stringstream_Line) {
        return 0;
    }
    std::getline(stringstream_Line, string_wave_file_name);
    string_wave_file_name = Get_string_Without_Blank(string_wave_file_name);


    return 0;
}


int
C_File_Resolve_gtest::Info_Parse_Line
(
    std::ifstream & rifstream_File,
    std::string & rstring_Key,
    std::string & rstring_Value,
    bool & rb_Eof
)
{
    // Init return value.

    rstring_Key.clear();
    rstring_Value.clear();
    rb_Eof = false;

    // Get line.

    std::string string_Line;

    Get_string_Line_Fixed(rifstream_File, string_Line);

    if (!rifstream_File) {
        // Check stream state: badbit

        if (rifstream_File.rdstate() & std::ios::badbit) {
            return -1;
        }

        // Check StreamFile: eofbit

        if (rifstream_File.rdstate() & std::ios::eofbit) {
            rb_Eof = true;
            return 0;
        }

        // Check StreamFile: failbit

        if (rifstream_File.rdstate() & std::ios::failbit) {
            return -1;
        }
    }

    // Remove trailing comment.

    std::string::size_type i_Pos_Char_Dollar = string_Line.find('$');
    if (i_Pos_Char_Dollar != std::string::npos) {
        string_Line = string_Line.substr(0, i_Pos_Char_Dollar);
    }

    // Get string.

    std::stringstream stringstream_Line;
    stringstream_Line.clear();
    stringstream_Line.str(string_Line);

    // Get key.

    if (!stringstream_Line) {
        return 0;
    }
    stringstream_Line >> rstring_Key;

    // Get value.

    if (!stringstream_Line) {
        return 0;
    }
    std::getline(stringstream_Line, rstring_Value);
    rstring_Value = Get_string_Without_Blank(rstring_Value);

    return 0;
}

int
C_File_Resolve_gtest::Info_Parse_Line
(
    std::ifstream & rifstream_File,
    std::string & rstring_one,
    std::string & rstring_two,
    std::string & rstring_three,
    std::string & rstring_four,
    std::string & rstring_five,
    std::string & rstring_six,
    bool & rb_Eof
)
{
    // Init return value.

    rstring_one.clear();
    rstring_two.clear();
    rstring_three.clear();
    rb_Eof = false;

    char* p_string = NULL;

    // Get line.

    std::string string_Line;

    Get_string_Line_Fixed(rifstream_File, string_Line);

    if (!rifstream_File) {
        // Check stream state: badbit

        if (rifstream_File.rdstate() & std::ios::badbit) {
            return -1;
        }

        // Check StreamFile: eofbit

        if (rifstream_File.rdstate() & std::ios::eofbit) {
            rb_Eof = true;
            return 0;
        }

        // Check StreamFile: failbit

        if (rifstream_File.rdstate() & std::ios::failbit) {
            return -1;
        }
    }

    p_string = std::strtok(const_cast<char*>(string_Line.c_str()), "\t");
    if (NULL != p_string) {
        rstring_one = std::string(p_string);
    }

    p_string = std::strtok(NULL, "\t");
    if (NULL != p_string) {
        rstring_two = std::string(p_string);
    }

    p_string = std::strtok(NULL, "\t");
    if (NULL != p_string) {
        rstring_three = std::string(p_string);
    }

    p_string = std::strtok(NULL, "\t");
    if (NULL != p_string) {
        rstring_four = std::string(p_string);
    }

    p_string = std::strtok(NULL, "\t");
    if (NULL != p_string) {
        rstring_five = std::string(p_string);
    }

    p_string = std::strtok(NULL, "\t");
    if (NULL != p_string) {
        rstring_six = std::string(p_string);
    }

    return 0;
}


int
C_File_Resolve_gtest::Info_Parse_Test_Item_Config
(
    std::ifstream & rifstream_File,
    C_Test_Wave_Info_gtest& wave_test_config
)
{
    // Parse lines.

    std::string string_Key;
    std::string string_Value;
    std::string string_expect_result_xml;
    bool b_Eof;

    while (true) {
        int nRetValue = Info_Parse_Line(
                            rifstream_File,
                            string_Key,
                            string_Value,
                            b_Eof
                        );

        if (nRetValue != 0) {
            return nRetValue;
        }

        if (b_Eof) {
            return -1;
        }

        if (string_Key == "PARTY_ID") {
            wave_test_config.string_party_id = string_Value;
        }
        else if (string_Key == "DIALOG_ID") {
            wave_test_config.string_dialog_id = string_Value;
        }
        else if (string_Key == "WAVE_FILE_NAME") {
            wave_test_config.string_wave_file_name = string_Value;
        }
        else if (string_Key == "EXPECT_RESULT_BEG") {
            nRetValue = Info_Parse_Test_Item_Config_Expect_Result_Xml(
                            rifstream_File,
                            string_expect_result_xml
                        );
            if (0 != nRetValue) {
                MACRO_Log_Error
                return -1;
            }

            wave_test_config.string_expect_result_xml = string_expect_result_xml;
        }
        else if (string_Key == "TEST_ITEM_CONFIG_END") {
            break;
        }
        else {
            continue;
        }
    }

    if (wave_test_config.string_party_id.empty()
        || wave_test_config.string_dialog_id.empty()
        || wave_test_config.string_wave_file_name.empty()
        || wave_test_config.string_expect_result_xml.empty()) {
        MACRO_Log_Error
        return -1;
    }

    return 0;
}

int
C_File_Resolve_gtest::Info_Parse_Test_Item_Config_Expect_Result_Xml
(
    std::ifstream & rifstream_File,
    std::string& string_expect_result_xml
)
{
    // Parse lines.

    std::string string_Line;

    string_expect_result_xml.clear();
    while (true) {
        Get_string_Line_Fixed(rifstream_File, string_Line);
        string_Line = Get_string_Without_Blank(string_Line);

        if (string_Line == "EXPECT_RESULT_END") {
            break;
        }
        else {
            string_expect_result_xml += string_Line;
        }
    }

    // get xml format
    pugi::xml_parse_result xml_result;
    pugi::xml_document expect_result_xml;
    xml_result = expect_result_xml.load_string(string_expect_result_xml.c_str());
    if (!xml_result) {
        MACRO_Log_Error
        return -1;
    }

    std::stringstream sstringXml;
    expect_result_xml.save(sstringXml, "    ");
    string_expect_result_xml = sstringXml.str();

    return 0;
}


int
C_File_Resolve_gtest::Info_Parse_Dialog
(
    std::ifstream & rifstream_File,
    std::string& string_item_value
)
{
    std::string string_Id_Dialog;

    // Parse lines.

    std::string string_Key;
    std::string string_Value;
    bool b_Eof;

    while (true) {
        int nRetValue = Info_Parse_Line(
                            rifstream_File,
                            string_Key,
                            string_Value,
                            b_Eof
                        );

        if (nRetValue != 0) {
            return nRetValue;
        }

        if (b_Eof) {
            MACRO_Log_Error
            return -1;
        }

        if (string_Key == "PRIMARY_ID_DIALOG") {
            string_Id_Dialog = string_Value;
        }
        else if (string_Key == "DIALOG_END") {
            break;
        }
        else {
            continue;
        }
    }

    if (string_Id_Dialog.empty()) {
        MACRO_Log_Error
        return -1;
    }

    string_item_value = string_Id_Dialog;

    return 0;
}

int
C_File_Resolve_gtest::Info_Parse_Context
(
    std::ifstream & rifstream_File,
    std::string& string_context_type,
    std::string& string_item_value
)
{
    std::string string_Id_Context;
    std::string string_Type_Context;

    // Parse lines.

    std::string string_Key;
    std::string string_Value;
    bool b_Eof;

    string_item_value = "";
    while (true) {
        int nRetValue = Info_Parse_Line(
                            rifstream_File,
                            string_Key,
                            string_Value,
                            b_Eof
                        );

        if (nRetValue != 0) {
            return nRetValue;
        }

        if (b_Eof) {
            MACRO_Log_Error
            return -1;
        }

        if (string_Key == "PRIMARY_ID_CONTEXT") {
            string_Id_Context = string_Value;
        }
        else if (string_Key == "TYPE_CONTEXT") {
            string_Type_Context = string_Value;
        }
        else if (string_Key == "CONTEXT_END") {
            break;
        }
        else {
            continue;
        }
    }

    if (string_Id_Context.empty()
        || string_Type_Context.empty()) {
        MACRO_Log_Error
        return -1;
    }

    if (string_context_type == string_Type_Context) {
        string_item_value = string_Id_Context;
    }

    return 0;
}




std::string
C_File_Resolve_gtest::Get_string_Without_Blank
(
    std::string const & rstring_I
)
{
    std::string rstring_O(rstring_I);

    if (rstring_O.empty()) {
        return rstring_O;
    }

    rstring_O.erase(0, rstring_O.find_first_not_of(" \t\r\n"));
    rstring_O.erase(rstring_O.find_last_not_of(" \t\r\n") + 1);

    return rstring_O;
}

void
C_File_Resolve_gtest::Get_string_Line_Fixed
(
    std::ifstream & rifstream_In,
    std::string & rstring_Out
)
{
    std::getline(rifstream_In, rstring_Out);

    while (!rstring_Out.empty()) {
        char i_Char = rstring_Out[rstring_Out.size() - 1];

        if ((i_Char != 0xd)
            && (i_Char != 0xa)) {
            break;
        }

        rstring_Out = rstring_Out.substr(0, rstring_Out.size() - 1);
    }
}


int
C_File_Resolve_gtest::Resovle_List_Context
(
    const std::string& string_file_name,
    std::map<std::string, C_List_Context_Item_gtest>&  pair_list_context
)
{
    std::string string_file_type;
    string_file_type = std::string(string_file_name, string_file_name.find_last_of('.') + 1);
    if (string_file_type == "xml") {
        return Resovle_List_Context_Xml(string_file_name, pair_list_context);
    }
    else if (string_file_type == "txt") {   // xxxx.txt
        return Resovle_List_Context_Txt(string_file_name, pair_list_context);
    }
    else {
        MACRO_Log_Error
        return -1;
    }

    return 0;
}


int
C_File_Resolve_gtest::Resovle_List_Context_Txt
(
    const std::string& string_file_name,
    std::map<std::string, C_List_Context_Item_gtest>& list_context_and_item
)
{
    std::string string_context_id;
    std::string string_context_oper;

    char a_i_Char_Delimiter[3] = "";

#if defined(_WIN32) || defined(_WIN32_WCE)
    a_i_Char_Delimiter[0] = '\\';
    a_i_Char_Delimiter[1] = '/';
    a_i_Char_Delimiter[2] = 0;
#else
    a_i_Char_Delimiter[0] = '/';
    a_i_Char_Delimiter[1] = 0;
    a_i_Char_Delimiter[2] = 0;

#endif


    string_context_id = std::string(string_file_name, string_file_name.find_last_of(a_i_Char_Delimiter) + 1);
    string_context_id = std::string(string_context_id, 0, string_context_id.find_last_of('.'));
    string_context_oper = std::string(string_context_id, string_context_id.find_last_of('_') + 1);
    string_context_id = std::string(string_context_id, 0, string_context_id.find_last_of('_'));
    if (string_context_id.empty() || string_context_oper.empty()) {
        MACRO_Log_Error
        return -1;
    }

    int nRetValue;
    std::vector<std::string> vecotr_context_item;
    nRetValue = Read_List_Context_Txt(string_file_name, vecotr_context_item);
    if (nRetValue != 0) {
        return nRetValue;
    }

    int int_slot_id = 0;
    std::list<spC_Term> list_Term;

    typedef std::vector<std::string>::const_iterator VecIter;
    VecIter context_end_iter = vecotr_context_item.end();
    for (
        VecIter context_iter =  vecotr_context_item.begin();
        context_iter != context_end_iter;
        ++context_iter
    ) {
        spC_Term item(new N_Vr::N_Asr::C_Term);
        item->m_i_User_Data_Lo = int_slot_id++;
        item->m_string_Orthography = *context_iter;

        list_Term.push_back(item);
    }

    C_List_Context_Item_gtest& context_item = list_context_and_item[string_context_id];
    context_item.m_string_list_context_name = string_context_id;
    if (NODE_ITEM_OPER_VALUE_ADD == string_context_oper) {
        context_item.m_list_context_add_item.assign(list_Term.begin(), list_Term.end());
    }
    else {
        context_item.m_list_context_del_item.assign(list_Term.begin(), list_Term.end());
    }

    return 0;
}

int
C_File_Resolve_gtest::Read_List_Context_Txt
(
    const std::string& string_file_name,
    std::vector<std::string>& vector_context_item
)
{
    std::ifstream ifstream_File;
    ifstream_File.open(string_file_name.c_str());
    if (!ifstream_File.is_open()) {
        MACRO_Log_Error
        return -1;
    }

    std::string string_Line;
    std::string string_context_item;

    for (;;) {
        Get_string_Line_Fixed(ifstream_File, string_Line);

        if (ifstream_File.rdstate() & std::ios::eofbit) {
            break;
        }

        string_context_item = Get_string_Without_Blank(string_Line);
        vector_context_item.push_back(string_context_item);
    }

    ifstream_File.close();

    return 0;
}

/* EOF */
