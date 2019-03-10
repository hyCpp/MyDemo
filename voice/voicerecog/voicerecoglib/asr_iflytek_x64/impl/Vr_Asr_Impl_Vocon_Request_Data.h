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
 * @file Voice_Asr_Impl_Request_Data.h
 * @brief C_Request_Data class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Request_Data_h__
#define __Vr_Asr_Impl_Request_Data_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

class C_Data_Party;

/**
 * @brief The C_Request_Data class
 *
 * C_Request_Data class
 */

class C_Request_Data
{
  public:

    enum E_Type
    {
        E_Type_None,
        E_Type_Context_List_Update_Begin,
        E_Type_Context_List_Update_Clear,
        E_Type_Context_List_Update_Delete,
        E_Type_Context_List_Update_Add,
        E_Type_Context_List_Update_Commit,
        E_Type_Context_Fuzzy_Compile_Begin,
        E_Type_Context_Fuzzy_Compile_Add_Column,
        E_Type_Context_Fuzzy_Compile_Add_Entry,
        E_Type_Context_Fuzzy_Compile_Run,
        E_Type_Context_List_File_Save,
        E_Type_Context_List_File_Delete,
        E_Type_Transcription_Id_Version_Update,
    };

    virtual
    ~C_Request_Data() // [F]Destructor
    {
    }

    explicit
    C_Request_Data() // [F]Constructor of Default
    {
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_None;
    }

    virtual
    string
    M_Fn_Get_string_Id_Party()
    {
        return string();
    }

    virtual
    string
    M_Fn_Get_string_Id_Context()
    {
        return string();
    }

    virtual
    string
    M_Fn_Get_string_Id_Category()
    {
        return string();
    }

    virtual
    C_Request_Context_List_Update *
    M_Fn_Get_po_Request_Context_List_Update()
    {
        RETURN_NULL;
    }

    virtual
    C_Term *
    M_Fn_Get_po_Term()
    {
        RETURN_NULL;
    }

#if 0

    virtual
    C_Request_Context_Fuzzy_Compile *
    M_Fn_Get_po_Request_Context_Fuzzy_Compile()
    {
        RETURN_NULL;
    }

#endif

    virtual
    vector<shared_ptr<C_Term> >*
    M_Fn_Get_Vector_po_Term()
    {
        RETURN_NULL;
    }

  PROTECTED:

    C_Request_Data // [F]Constructor: Copy
    (
        C_Request_Data const & // [I]
    );

    C_Request_Data & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data const & // [I]
    );
};

/**
 * @brief The C_Request_Data_Context_List_Update_Begin class
 *
 * C_Request_Data_Context_List_Update_Begin class
 */

class C_Request_Data_Context_List_Update_Begin : public C_Request_Data
{
  public:

    virtual
    ~C_Request_Data_Context_List_Update_Begin() // [F]Destructor
    {
    }

    explicit
    C_Request_Data_Context_List_Update_Begin // [F]Constructor of Default
    (
        C_Request_Context_List_Update * po_Request_Context_List_Update
    )
    : m_po_Request_Context_List_Update(po_Request_Context_List_Update)
    {
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_Context_List_Update_Begin;
    }

    virtual
    string
    M_Fn_Get_string_Id_Party()
    {
        return m_po_Request_Context_List_Update->m_string_Id_Party;
    }

    virtual
    string
    M_Fn_Get_string_Id_Context()
    {
        return m_po_Request_Context_List_Update->m_string_Id_Context;
    }

    virtual
    string
    M_Fn_Get_string_Id_Category()
    {
        return m_po_Request_Context_List_Update->m_string_Id_Category;
    }

    virtual
    C_Request_Context_List_Update *
    M_Fn_Get_po_Request_Context_List_Update()
    {
        return m_po_Request_Context_List_Update;
    }

  PROTECTED:

    C_Request_Data_Context_List_Update_Begin // [F]Constructor: Copy
    (
        C_Request_Data_Context_List_Update_Begin const & // [I]
    );

    C_Request_Data_Context_List_Update_Begin & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data_Context_List_Update_Begin const & // [I]
    );

    C_Request_Context_List_Update * m_po_Request_Context_List_Update;
};

/**
 * @brief The C_Request_Data_Context_List_Update_Clear class
 *
 * C_Request_Data_Context_List_Update_Clear class
 */

class C_Request_Data_Context_List_Update_Clear : public C_Request_Data
{
  public:

    virtual
    ~C_Request_Data_Context_List_Update_Clear() // [F]Destructor
    {
    }

    explicit
    C_Request_Data_Context_List_Update_Clear() // [F]Constructor of Default
    {
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_Context_List_Update_Clear;
    }

  PROTECTED:

    C_Request_Data_Context_List_Update_Clear // [F]Constructor: Copy
    (
        C_Request_Data_Context_List_Update_Clear const & // [I]
    );

    C_Request_Data_Context_List_Update_Clear & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data_Context_List_Update_Clear const & // [I]
    );
};

/**
 * @brief The C_Request_Data_Context_List_Update_Delete class
 *
 * C_Request_Data_Context_List_Update_Delete class
 */

class C_Request_Data_Context_List_Update_Delete : public C_Request_Data
{
  public:

    virtual
    ~C_Request_Data_Context_List_Update_Delete() // [F]Destructor
    {
    }

    explicit
    C_Request_Data_Context_List_Update_Delete // [F]Constructor of Default
    (
        shared_ptr<C_Term> & rspo_Term
    )
    : m_spo_Term(rspo_Term)
    {
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_Context_List_Update_Delete;
    }

    virtual
    C_Term *
    M_Fn_Get_po_Term()
    {
        return m_spo_Term.get();
    }

  PROTECTED:

    C_Request_Data_Context_List_Update_Delete // [F]Constructor: Copy
    (
        C_Request_Data_Context_List_Update_Delete const & // [I]
    );

    C_Request_Data_Context_List_Update_Delete & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data_Context_List_Update_Delete const & // [I]
    );

    shared_ptr<C_Term> m_spo_Term;
};

/**
 * @brief The C_Request_Data_Context_List_Update_Add class
 *
 * C_Request_Data_Context_List_Update_Add class
 */

class C_Request_Data_Context_List_Update_Add : public C_Request_Data
{
  public:

    virtual
    ~C_Request_Data_Context_List_Update_Add() // [F]Destructor
    {
    }

    explicit
    C_Request_Data_Context_List_Update_Add // [F]Constructor of Default
    (
        shared_ptr<C_Term> & rspo_Term,
        C_Request_Context_List_Update * po_Request_Context_List_Update
    )
    : m_spo_Term(rspo_Term)
    , m_po_Request_Context_List_Update(po_Request_Context_List_Update)
    {
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_Context_List_Update_Add;
    }

    virtual
    C_Term *
    M_Fn_Get_po_Term()
    {
        return m_spo_Term.get();
    }

    virtual
    C_Request_Context_List_Update *
    M_Fn_Get_po_Request_Context_List_Update()
    {
        return m_po_Request_Context_List_Update;
    }

  PROTECTED:

    C_Request_Data_Context_List_Update_Add // [F]Constructor: Copy
    (
        C_Request_Data_Context_List_Update_Add const & // [I]
    );

    C_Request_Data_Context_List_Update_Add & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data_Context_List_Update_Add const & // [I]
    );

    shared_ptr<C_Term> m_spo_Term;
    C_Request_Context_List_Update * m_po_Request_Context_List_Update;
};

/**
 * @brief The C_Request_Data_Context_List_Update_Commit class
 *
 * C_Request_Data_Context_List_Update_Commit class
 */

class C_Request_Data_Context_List_Update_Commit : public C_Request_Data
{
  public:

    virtual
    ~C_Request_Data_Context_List_Update_Commit() // [F]Destructor
    {
    }

    explicit
    C_Request_Data_Context_List_Update_Commit // [F]Constructor of Default
    (
        shared_ptr<C_Request_Context_List_Update> spo_Request_Context_List_Update
    )
    : m_spo_Request_Context_List_Update(spo_Request_Context_List_Update)
    {
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_Context_List_Update_Commit;
    }

    virtual
    C_Request_Context_List_Update *
    M_Fn_Get_po_Request_Context_List_Update()
    {
        return m_spo_Request_Context_List_Update.get();
    }

  PROTECTED:

    C_Request_Data_Context_List_Update_Commit // [F]Constructor: Copy
    (
        C_Request_Data_Context_List_Update_Commit const & // [I]
    );

    C_Request_Data_Context_List_Update_Commit & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data_Context_List_Update_Commit const & // [I]
    );

    shared_ptr<C_Request_Context_List_Update> m_spo_Request_Context_List_Update;
};

/**
* @brief The C_Request_Data_Context_List_File_Save class
*
* C_Request_Data_Context_List_File_Save class
*/

class C_Request_Data_Context_List_File_Save: public C_Request_Data
{
  public:

    virtual
    ~C_Request_Data_Context_List_File_Save() // [F]Destructor
    {
    }

    explicit
    C_Request_Data_Context_List_File_Save // [F]Constructor of Default
    (
        shared_ptr<C_Request_Context_List_Update> spo_Request_Context_List_Update
    )
    : m_spo_Request_Context_List_Update(spo_Request_Context_List_Update)
    {
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_Context_List_File_Save;
    }

    virtual
    string
    M_Fn_Get_string_Id_Party()
    {
        return m_spo_Request_Context_List_Update->m_string_Id_Party;
    }

    virtual
    string
    M_Fn_Get_string_Id_Context()
    {
        return m_spo_Request_Context_List_Update->m_string_Id_Context;
    }

    virtual
    string
    M_Fn_Get_string_Id_Category()
    {
        return m_spo_Request_Context_List_Update->m_string_Id_Category;
    }

    virtual
    C_Request_Context_List_Update *
    M_Fn_Get_po_Request_Context_List_Update()
    {
        return m_spo_Request_Context_List_Update.get();
    }

  PROTECTED:

    C_Request_Data_Context_List_File_Save // [F]Constructor: Copy
    (
        C_Request_Data_Context_List_File_Save const & // [I]
    );

    C_Request_Data_Context_List_File_Save & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data_Context_List_File_Save const & // [I]
    );

    shared_ptr<C_Request_Context_List_Update> m_spo_Request_Context_List_Update;
};

/**
* @brief The C_Request_Data_Context_List_File_Delete class
*
* C_Request_Data_Context_List_File_Delete class
*/

class C_Request_Data_Context_List_File_Delete: public C_Request_Data
{
  public:

    virtual
    ~C_Request_Data_Context_List_File_Delete() // [F]Destructor
    {
    }

    explicit
    C_Request_Data_Context_List_File_Delete // [F]Constructor of Default
    (
        shared_ptr<C_Request_Context_List_Update> spo_Request_Context_List_Update
    )
    : m_spo_Request_Context_List_Update(spo_Request_Context_List_Update)
    {
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_Context_List_File_Delete;
    }

    virtual
    string
    M_Fn_Get_string_Id_Party()
    {
        return m_spo_Request_Context_List_Update->m_string_Id_Party;
    }

    virtual
    string
    M_Fn_Get_string_Id_Context()
    {
        return m_spo_Request_Context_List_Update->m_string_Id_Context;
    }

    virtual
    string
    M_Fn_Get_string_Id_Category()
    {
        return m_spo_Request_Context_List_Update->m_string_Id_Category;
    }

    virtual
    C_Request_Context_List_Update *
    M_Fn_Get_po_Request_Context_List_Update()
    {
        return m_spo_Request_Context_List_Update.get();
    }

  PROTECTED:

    C_Request_Data_Context_List_File_Delete // [F]Constructor: Copy
    (
        C_Request_Data_Context_List_File_Delete const & // [I]
    );

    C_Request_Data_Context_List_File_Delete & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data_Context_List_File_Delete const & // [I]
    );

    shared_ptr<C_Request_Context_List_Update> m_spo_Request_Context_List_Update;
};

/**
 * @brief The C_Request_Data_Transcription_Id_Version_Update class
 *
 * C_Request_Data_Transcription_Id_Version_Update class
 */

class C_Request_Data_Transcription_Id_Version_Update : public C_Request_Data
{
  public:

    virtual
    ~C_Request_Data_Transcription_Id_Version_Update() // [F]Destructor
    {
    }

    explicit
    C_Request_Data_Transcription_Id_Version_Update // [F]Constructor of Default
    (
        shared_ptr<C_Request_Context_List_Update> spo_Request_Context_List_Update
    )
    : m_spo_Request_Context_List_Update(spo_Request_Context_List_Update)
    {
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_Transcription_Id_Version_Update;
    }

    virtual
    C_Request_Context_List_Update *
    M_Fn_Get_po_Request_Context_List_Update()
    {
        return m_spo_Request_Context_List_Update.get();
    }

  PROTECTED:

    C_Request_Data_Transcription_Id_Version_Update // [F]Constructor: Copy
    (
        C_Request_Data_Transcription_Id_Version_Update const & // [I]
    );

    C_Request_Data_Transcription_Id_Version_Update & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data_Transcription_Id_Version_Update const & // [I]
    );

    shared_ptr<C_Request_Context_List_Update> m_spo_Request_Context_List_Update;
};

#if 0

/**
 * @brief The C_Request_Data_Context_Fuzzy_Compile_Begin class
 *
 * C_Request_Data_Context_Fuzzy_Compile_Begin class
 */

class C_Request_Data_Context_Fuzzy_Compile_Begin : public C_Request_Data
{
  public:

    virtual
    ~C_Request_Data_Context_Fuzzy_Compile_Begin() // [F]Destructor
    {
    }

    explicit
    C_Request_Data_Context_Fuzzy_Compile_Begin // [F]Constructor of Default
    (
        C_Request_Context_Fuzzy_Compile * po_Request_Context_Fuzzy_Compile
    )
    : m_po_Request_Context_Fuzzy_Compile(po_Request_Context_Fuzzy_Compile)
    {
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_Context_Fuzzy_Compile_Begin;
    }

    virtual
    string
    M_Fn_Get_string_Id_Party()
    {
        return m_po_Request_Context_Fuzzy_Compile->m_string_Id_Party;
    }

    virtual
    string
    M_Fn_Get_string_Id_Context()
    {
        return m_po_Request_Context_Fuzzy_Compile->m_string_Id_Context;
    }

    virtual
    C_Request_Context_Fuzzy_Compile *
    M_Fn_Get_po_Request_Context_Fuzzy_Compile()
    {
        return m_po_Request_Context_Fuzzy_Compile;
    }

  PROTECTED:

    C_Request_Data_Context_Fuzzy_Compile_Begin // [F]Constructor: Copy
    (
        C_Request_Data_Context_Fuzzy_Compile_Begin const & // [I]
    );

    C_Request_Data_Context_Fuzzy_Compile_Begin & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data_Context_Fuzzy_Compile_Begin const & // [I]
    );

    C_Request_Context_Fuzzy_Compile * m_po_Request_Context_Fuzzy_Compile;
};

/**
 * @brief The C_Request_Data_Context_Fuzzy_Compile_Add_Column class
 *
 * C_Request_Data_Context_Fuzzy_Compile_Add_Column class
 */

class C_Request_Data_Context_Fuzzy_Compile_Add_Column : public C_Request_Data
{
  public:

    virtual
    ~C_Request_Data_Context_Fuzzy_Compile_Add_Column() // [F]Destructor
    {
    }

    explicit
    C_Request_Data_Context_Fuzzy_Compile_Add_Column // [F]Constructor of Default
    (
        vector<shared_ptr<C_Term> > & ro_vector_spo_Term
    )
    {
        m_vector_spo_Term.assign(
            ro_vector_spo_Term.begin(),
            ro_vector_spo_Term.end());
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_Context_Fuzzy_Compile_Add_Column;
    }

    virtual
    vector<shared_ptr<C_Term> >*
    M_Fn_Get_Vector_po_Term()
    {
        return &m_vector_spo_Term;
    }

  PROTECTED:

    C_Request_Data_Context_Fuzzy_Compile_Add_Column // [F]Constructor: Copy
    (
        C_Request_Data_Context_Fuzzy_Compile_Add_Column const & // [I]
    );

    C_Request_Data_Context_Fuzzy_Compile_Add_Column & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data_Context_Fuzzy_Compile_Add_Column const & // [I]
    );

    vector<shared_ptr<C_Term> > m_vector_spo_Term;
};

/**
 * @brief The C_Request_Data_Context_Fuzzy_Compile_Add_Entry class
 *
 * C_Request_Data_Context_Fuzzy_Compile_Add_Entry class
 */

class C_Request_Data_Context_Fuzzy_Compile_Add_Entry : public C_Request_Data
{
  public:

    virtual
    ~C_Request_Data_Context_Fuzzy_Compile_Add_Entry() // [F]Destructor
    {
    }

    explicit
    C_Request_Data_Context_Fuzzy_Compile_Add_Entry // [F]Constructor of Default
    (
        vector<shared_ptr<C_Term> > & ro_vector_spo_Term
    )
    {
        m_vector_spo_Term.assign(
            ro_vector_spo_Term.begin(),
            ro_vector_spo_Term.end());
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_Context_Fuzzy_Compile_Add_Entry;
    }

    virtual
    vector<shared_ptr<C_Term> >*
    M_Fn_Get_Vector_po_Term()
    {
        return &m_vector_spo_Term;
    }

  PROTECTED:

    C_Request_Data_Context_Fuzzy_Compile_Add_Entry // [F]Constructor: Copy
    (
        C_Request_Data_Context_Fuzzy_Compile_Add_Entry const & // [I]
    );

    C_Request_Data_Context_Fuzzy_Compile_Add_Entry & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data_Context_Fuzzy_Compile_Add_Entry const & // [I]
    );

    vector<shared_ptr<C_Term> > m_vector_spo_Term;
};

/**
 * @brief The C_Request_Data_Context_Fuzzy_Compile_Run class
 *
 * C_Request_Data_Context_Fuzzy_Compile_Run class
 */

class C_Request_Data_Context_Fuzzy_Compile_Run : public C_Request_Data
{
  public:

    virtual
    ~C_Request_Data_Context_Fuzzy_Compile_Run() // [F]Destructor
    {
    }

    explicit
    C_Request_Data_Context_Fuzzy_Compile_Run // [F]Constructor of Default
    (
        shared_ptr<C_Request_Context_Fuzzy_Compile> spo_Request_Context_Fuzzy_Compile
    )
    : m_spo_Request_Context_Fuzzy_Compile(spo_Request_Context_Fuzzy_Compile)
    {
    }

    virtual
    E_Type
    M_Fn_Get_Type()
    {
        return E_Type_Context_Fuzzy_Compile_Run;
    }

    virtual
    C_Request_Context_Fuzzy_Compile *
    M_Fn_Get_po_Request_Context_Fuzzy_Compile()
    {
        return m_spo_Request_Context_Fuzzy_Compile.get();
    }

  PROTECTED:

    C_Request_Data_Context_Fuzzy_Compile_Run // [F]Constructor: Copy
    (
        C_Request_Data_Context_Fuzzy_Compile_Run const & // [I]
    );

    C_Request_Data_Context_Fuzzy_Compile_Run & // [R]
    operator = // [F]Operator: =
    (
        C_Request_Data_Context_Fuzzy_Compile_Run const & // [I]
    );

    shared_ptr<C_Request_Context_Fuzzy_Compile> m_spo_Request_Context_Fuzzy_Compile;
};

#endif

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */