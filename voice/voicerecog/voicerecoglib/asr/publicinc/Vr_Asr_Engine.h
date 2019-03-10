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
 * @file Voice_Asr_EngineNuance.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Engine_h__
#define __Vr_Asr_Engine_h__

#include "Vr_Asr_Def.h"
#include "Vr_Asr_Audio_In.h"
#include "Vr_Asr_Request_Factory.h"
#include "Vr_Asr_Request_Param_Set.h"
#include "Vr_Asr_Request_Context_List_Update.h"
#include "Vr_Asr_Request_Context_Fuzzy_Compile.h"
#include "Vr_Asr_Request_Activate.h"
#include "Vr_Asr_Request_Recognize.h"
#include "Vr_Asr_Request_Language_Change.h"

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Engine class
 *
 * C_Engine class
 */
class ASR_API C_Engine
{

  public:

    static
    C_Engine * // [R] Created instance.
    Factory
    (
        C_Request_Factory & ro_Request_Factory
    ); // [F] Factory.

    static
    C_Engine * // [R] Created instance.
    Factory
    (
        std::string const & rstring_Id_Language // [I] Language id.
    ); // [F] Factory.

    static
    E_Result // [R] True for success, false for failed.
    Clear_User_Data();    // [F] Clear user data.

    virtual
    E_Result // [R] True for success, false for failed.
    Language_Change // [F] Load language.
    (
        std::string const & rstring_Id_Language // [I] Language id.
    ) = 0;
    
    virtual
    E_Result // [R] True for success, false for failed.
    Language_Change // [F] Load language.
    (
        C_Request_Language_Change const & ro_Request_Language_Change
    ) = 0;

    virtual
    E_Result // [R] True for success, false for failed.
    Party_Load // [F] Load information.
    (
        std::string const & rstring_Id_Party // [I] Party id.
    ) = 0;

    virtual
    E_Result // [R] True for success, false for failed.
    Party_Unload // [F] Load information.
    (
        std::string const & rstring_Id_Party // [I] Party id.
    ) = 0;

    virtual
    E_Result // [R] True for success, false for failed.
    Param_Set // [F] Set Param.
    (
        C_Request_Param_Set & ro_Request_Param_Set // [I] Request.
    ) = 0;

    virtual
    E_Result // [R] True for success, false for failed.
    Context_List_Update_Sync // [F] Update List Context.
    (
        C_Request_Context_List_Update & ro_Request_Context_List_Update // [I] Request.
    ) = 0;

    virtual
    E_Result // [R] True for success, false for failed.
    Context_List_Update // [F] Update List Context.
    (
        C_Request_Context_List_Update & ro_Request_Context_List_Update // [I] Request.
    ) = 0;

#if 0
    virtual
    E_Result // [R] True for success, false for failed.
    Context_Fuzzy_Compile // [F] Update List Context.
    (
        C_Request_Context_Fuzzy_Compile & ro_Request_Context_Fuzzy_Compile // [I] Request.
    ) = 0;
#endif

    virtual
    E_Result // [R] True for success, false for failed.
    Recognize_Start_Sync // [F] Start VR.
    (
        C_Request_Recognize & ro_Request_Recognize // [I] Request.
    ) = 0;

    virtual
    E_Result // [R] True for success, false for failed.
    Recognize_Start // [F] Start VR.
    (
        C_Request_Recognize & ro_Request_Recognize // [I] Request.
    ) = 0;

    virtual
    E_Result // [R] True for success, false for failed.
    Recognize_Cancel() = 0; // [F] Cancel VR.

    virtual
    E_Result
    Get_Transcriptions
    (
        C_Term& ro_Term
    ) = 0;

    virtual
    ~C_Engine(); // Destructor

  protected:

    explicit
    C_Engine(); // Constructor of Default

    C_Engine // Constructor: Copy
    (
        C_Engine const & // [I]
    );

    C_Engine & // [R]
    operator = // Operator: =
    (
        C_Engine const & // [I]
    );
};

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
