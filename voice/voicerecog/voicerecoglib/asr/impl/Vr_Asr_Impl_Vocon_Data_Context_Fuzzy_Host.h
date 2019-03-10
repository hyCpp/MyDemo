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
 * @file Voice_Asr_Impl_Data_Context_Fuzzy_Host.h
 * @brief C_Data_Context_Fuzzy_Host class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Data_Context_Fuzzy_Host_h__
#define __Vr_Asr_Impl_Data_Context_Fuzzy_Host_h__

#include "Vr_Asr_Impl_Vocon_Data_Context.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

#ifdef MACRO_Feature_Reserved

/**
 * @brief The C_Data_Context class
 *
 * C_Data_Context class
 */

class C_Data_Context_Fuzzy_Host : public C_Data_Context
{
  public:

    virtual
    ~C_Data_Context_Fuzzy_Host(); // [F]Destructor

    explicit
    C_Data_Context_Fuzzy_Host // [F]Constructor of Default
    (
        C_Data_Party & ro_Data_Party,
        string const & rstring_Id_Context,
        string const & rstring_Type_Context,
        string const & rstring_Data_File_Context,
        list<pair<string, string> > & rlist_pair_string_Id_Slot_string_Id_Context
    );

    virtual
    E_Result
    M_Fn_Init();

    virtual
    E_Result
    M_Fn_Fina();

    virtual
    E_Result
    M_Fn_Load
    (
        list<shared_ptr<C_Request_Activate> > & rlist_spo_Request_Activate
    );

    virtual
    E_Result
    M_Fn_Unload();

    virtual
    E_Result
    M_Fn_Slot_Guest_Load();

    virtual
    E_Result
    M_Fn_Slot_Guest_Unload();

  PROTECTED:

    C_Data_Context_Fuzzy_Host // [F]Constructor: Copy
    (
        C_Data_Context_Fuzzy_Host const & // [I]
    );

    C_Data_Context_Fuzzy_Host & // [R]
    operator = // [F]Operator: =
    (
        C_Data_Context_Fuzzy_Host const & // [I]
    );
};

#endif

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */