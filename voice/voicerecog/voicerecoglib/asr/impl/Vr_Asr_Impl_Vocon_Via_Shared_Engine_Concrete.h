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
 * @file Voice_Asr_Impl_Vocon_Via_Engine_Concrete.h
 * @brief C_Engine_Concrete class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Vocon_Via_Engine_Concrete_h__
#define __Vr_Asr_Impl_Vocon_Via_Engine_Concrete_h__

#include "Vr_Asr_Impl_Engine_Base.h"
#include "Vr_Asr_Impl_Vocon_Engine_Concrete.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

class C_Engine_Agent;

namespace N_Vocon
{

class C_Heap;
class C_Data_Party;
class C_Mgr_Data;
class C_Mgr_Rec;

namespace N_Via_Shared
{

/**
 * @brief The C_Engine_Concrete class
 *
 * C_Engine_Concrete class
 */

class C_Engine_Concrete : public N_Vocon::C_Engine_Concrete
{
  public:

    LH_COMPONENT * m_po_LH_COMPONENT_Asr; // Handle to the ASR component.

    LH_OBJECT * m_po_LH_OBJECT_Mod; // Handle to the acoustic mode object.

    ~C_Engine_Concrete(); // [F]Destructor

    explicit
    C_Engine_Concrete // [F]Constructor of Default
    (
        C_Engine_Agent & ro_Engine_Agent,
        string const & rstring_Id_Language,
        void * po_Vocon_LH_COMPONENT_Asr,
        void * po_Vocon_LH_OBJECT_Mod_Ac,
        boost::function<void(C_Event_Init_Notify const &)>& ro_function_On_Event_Init_Notify
    );

    virtual
    LH_COMPONENT &
    M_Fn_Get_ro_LH_COMPONENT_Asr();

    virtual
    LH_OBJECT &
    M_Fn_Get_ro_LH_OBJECT_Mod();

  PROTECTED:

    virtual
    E_Result
    _M_Fn_Base_Init();

    virtual
    E_Result
    _M_Fn_Base_Fina();

    virtual
    E_Result
    _M_Fn_Asr_Init();

    virtual
    E_Result
    _M_Fn_Asr_Fina();

    virtual
    E_Result
    _M_Fn_Asr_Mod_Init();

    virtual
    E_Result
    _M_Fn_Asr_Mod_Fina();

    virtual
    E_Result
    _M_Fn_Pron_Init();

    virtual
    E_Result
    _M_Fn_Pron_Fina();

    C_Engine_Concrete // [F]Constructor: Copy
    (
        C_Engine_Concrete const & // [I]
    );

    C_Engine_Concrete & // [R]
    operator = // [F]Operator: =
    (
        C_Engine_Concrete const & // [I]
    );
};

} // name N_Via_Shared

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */