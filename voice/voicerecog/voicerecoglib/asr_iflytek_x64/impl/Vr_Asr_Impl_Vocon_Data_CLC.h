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
 * @file Voice_Asr_Impl_Data_CLC.h
 * @brief C_Data_CLC class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Data_CLC_h__
#define __Vr_Asr_Impl_Data_CLC_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

class C_Engine_Concrete;

/**
 * @brief The C_Data_CLC class
 *
 * C_Data_CLC class
 */

class C_Data_CLC
{
  public:

    C_Engine_Concrete & m_ro_Engine_Concrete;

    ~C_Data_CLC(); // [F]Destructor

    explicit
    C_Data_CLC // [F]Constructor of Default
    (
        C_Engine_Concrete & ro_Engine_Concrete,
        string const & rstring_Id_CLC,
        string const & rstring_Path_File_CLC
    );

    void
    M_Fn_Set_Path_File_Id_Trans_Lookup
    (
        string const & rstring_Path_File_Id_Trans_Lookup
    )
    {
        m_string_Path_File_Id_Trans_Lookup = rstring_Path_File_Id_Trans_Lookup;
    }

    E_Result
    M_Fn_Init();

    E_Result
    M_Fn_Fina();

    E_Result
    M_Fn_Init_Id_Trans_Lookup(string const& ro_string_Id_Trans_Version);

    E_Result
    M_Fn_Fina_Id_Trans_Lookup();

    E_Result
    M_Fn_Id_Trans_Version_Update
    (
        string const& ro_string_Id_Trans_Version,
        int & b_Id_Trans_Version_Supported
    );

    E_Result
    M_Fn_Domain_Category_Set
    (
        string const & string_Domain_Category,
        string& ro_string_Domain_Category_Prev
    );

    LH_OBJECT &
    M_Fn_Get_ro_LH_OBJECT_CLC()
    {
        return m_o_LH_OBJECT_CLC;
    }

    LH_OBJECT &
    M_Fn_Get_ro_LH_OBJECT_CLC_LEX()
    {
        return m_o_LH_OBJECT_CLC_LEX;
    }

    LH_OBJECT &
    M_Fn_Get_ro_LH_OBJECT_CLC_Id_Trans()
    {
        return m_o_LH_OBJECT_CLC_Id_Trans;
    }

    int
    M_Fn_Get_b_Id_Trans_Lookup()
    {
        return m_b_Id_Trans_Lookup;
    }

    int
    M_Fn_Get_b_Id_Trans_Supported()
    {
        return !m_string_Path_File_Id_Trans_Lookup.empty();
    }

  PROTECTED:

    C_Data_CLC // [F]Constructor: Copy
    (
        C_Data_CLC const & // [I]
    );

    C_Data_CLC & // [R]
    operator = // [F]Operator: =
    (
        C_Data_CLC const & // [I]
    );

    int m_b_Id_Trans_Lookup;
    string m_string_Id_CLC;
    string m_string_Path_File_CLC;
    string m_string_Path_File_Id_Trans_Lookup;
    string m_string_Domain_Category_Curr;

    LH_OBJECT m_o_LH_OBJECT_CLC; // Handle to the CLC.
    LH_OBJECT m_o_LH_OBJECT_CLC_LEX; // Handle to the LEX CLC.
    LH_OBJECT m_o_LH_OBJECT_CLC_Cache; // Handle to the Cache CLC.
    LH_OBJECT m_o_LH_OBJECT_CLC_Id_Trans; // Handle to the CLC for gracenote.
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */