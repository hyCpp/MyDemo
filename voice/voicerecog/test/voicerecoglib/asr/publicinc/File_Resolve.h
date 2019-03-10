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
 * @file File_Resolve_gtest.h
 * @brief Declaration file of class C_List_Context_Item_gtest.
 *
 * This file includes the declaration of class C_List_Context_Item_gtest.
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __File_Resolve_h__
#define __File_Resolve_h__

#include <set>
#include <map>
#include <list>
#include <vector>
#include <fstream>
#include <string>

#include <pugixml.hpp>

#include <boost/shared_ptr.hpp>
#include "Vr_Asr_Term.h"


typedef boost::shared_ptr<N_Vr::N_Asr::C_Term> spC_Term;

/**
 * @brief The C_List_Context_Item class
 *
 * C_List_Context_Item class
 */
class C_List_Context_Item
{
// Attribute
public:
    std::string m_string_list_context_name;
    std::list<spC_Term> m_list_context_add_item;
    std::list<spC_Term> m_list_context_del_item;
};

/**
 * @brief The C_Result_Node class
 *
 * C_Result_Node class
 */
class C_Result_Node
{
// Attribute
public:
    int int_confidence;
    std::string string_value;
};

/**
 * @brief The C_Test_Wave_Info class
 *
 * C_Test_Wave_Info class
 */
class C_Test_Wave_Info
{
// Attribute
public:
    std::string string_party_id;
    std::string string_dialog_id;
    std::string string_wave_file_name;
    std::string string_expect_result_xml;
};

/**
 * @brief The C_File_Resolve class
 *
 * C_File_Resolve class
 */
class C_File_Resolve
{
public:
    C_File_Resolve();
    ~C_File_Resolve();

public:
    int
    Resolve_Info_Data
    (
        const std::string& str_file_name,
        const std::string& string_item_type,
        const std::string& string_context_type,
        std::set<std::string>& set_item_value
    );

    int
    Resolve_Wave_Info_Data
    (
        const std::string& str_file_name,
        std::list<C_Test_Wave_Info>& list_wave_info
    );


    int
    Resovle_List_Context_Xml
    (
        const std::string& string_file_name,
        std::map<std::string, C_List_Context_Item>& list_context_and_item
    );

    int
    Resovle_Voicerecog_Result_Xml
    (
        const std::string& string_voicerecog_resutl_xml,
        bool b_actual_result,
        std::vector<std::map<std::string, C_Result_Node> >& vector_result
    );

    int
    Resovle_List_Context_Txt
    (
        const std::string& string_file_name,
        std::map<std::string, C_List_Context_Item>& list_context_and_item
    );

    int
    Resovle_List_Context
    (
        const std::string& string_file_name,
        std::map<std::string, C_List_Context_Item>&  pair_list_context
    );

    int 
    Resovle_Fuzzy_Compile_Data
    (
        const std::string& str_file_name, 
        std::vector<boost::shared_ptr<N_Vr::N_Asr::C_Term> >& vector_spo_Term_Column,
        std::list<std::vector<boost::shared_ptr<N_Vr::N_Asr::C_Term> > >& list_vector_spo_Term_Column_Data
    );
    
private:
    int
    Read_List_Context_Txt
    (
        const std::string& string_file_name,
        std::vector<std::string>& vector_context_item
    );

    void
    Resovle_Context
    (
        const pugi::xml_node& node,
        C_List_Context_Item& context_item
    );

    int
    Resovle_Voicerecog_Result_Node
    (
        const pugi::xml_node& node,
        std::vector<std::map<std::string, C_Result_Node> >& vector_result
    );

    int
    Info_Parse_Line
    (
        std::ifstream & rifstream_File,
        std::string & rstring_Key,
        std::string & rstring_Value,
        bool & rb_Eof
    );

    int
    Info_Parse_Line
    (
        std::ifstream & rifstream_File,
        std::string & rstring_one,
        std::string & rstring_two,
        std::string & rstring_three,
        std::string & rstring_four,
        std::string & rstring_five,
        std::string & rstring_six,
        bool & rb_Eof
    );

    int
    Info_Parse_Test_Item_Config
    (
        std::ifstream & rifstream_File,
        C_Test_Wave_Info& wave_test_config
    );

    int
    Info_Parse_Test_Item_Config_Expect_Result_Xml
    (
        std::ifstream & rifstream_File,
        std::string& string_expect_result_xml
    );

    int
    Info_Parse_Dialog
    (
        std::ifstream & rifstream_File,
        std::string& string_item_value
    );

    int
    Info_Parse_Context
    (
        std::ifstream & rifstream_File,
        std::string& string_context_type,
        std::string& string_item_value
    );

    int
    Info_Parse_Tag
    (
        std::ifstream & rifstream_File,
        std::string& string_item_value
    );

    int
    Info_Parse_Rule
    (
        std::ifstream & rifstream_File,
        std::string& string_item_value
    );

    int
    Wave_Info_Parse_Line
    (
        std::ifstream & rifstream_File,
        std::string & string_party_id,
        std::string & string_dialog_id,
        std::string & string_wave_file_name,
        bool & rb_Eof
    );

    std::string
    Get_string_Without_Blank
    (
        std::string const & rstring_I
    );

    void
    Get_string_Line_Fixed
    (
        std::ifstream & rifstream_In,
        std::string & rstring_Out
    );
};

#endif // __File_Resolve_h__
/* EOF */
