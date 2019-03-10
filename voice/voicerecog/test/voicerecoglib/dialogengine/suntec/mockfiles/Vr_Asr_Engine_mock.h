/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
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
 * @file Vr_Asr_Event_Notify_mock.h
 * @brief Declaration file of Vr_Asr_Event_Notify_mock.
 *
 * This file includes the declaration of Vr_Asr_Event_Notify_mock.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ASRENGINE_MOCK_H
#define VR_ASRENGINE_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Vr_Asr_Engine.h"


/**
* Vr_Asr_Event_Notify_mock
*
* The class is a mock class for Vr_Asr_Event_Notify_mock.
*/
class C_Engine_mock : public N_Vr::N_Asr::C_Engine
{
public:
    N_Vr::N_Asr::E_Result Language_Change(std::string const & rstring_Id_Language)
    {
        return N_Vr::N_Asr::E_Result::E_Result_Succeeded;
    }

    N_Vr::N_Asr::E_Result Language_Change(N_Vr::N_Asr::C_Request_Language_Change const & ro_Request_Language_Change)
    {
        return N_Vr::N_Asr::E_Result::E_Result_Succeeded;
    }

    MOCK_METHOD1(Party_Load, N_Vr::N_Asr::E_Result(std::string const & rstring_Id_Party));
    MOCK_METHOD1(Party_Unload, N_Vr::N_Asr::E_Result(std::string const & rstring_Id_Party));
    MOCK_METHOD1(Param_Set, N_Vr::N_Asr::E_Result(N_Vr::N_Asr::C_Request_Param_Set & ro_Request_Param_Set));
    MOCK_METHOD1(Context_List_Update_Sync, N_Vr::N_Asr::E_Result(N_Vr::N_Asr::C_Request_Context_List_Update & ro_Request_Context_List_Update));
    MOCK_METHOD1(Context_List_Update, N_Vr::N_Asr::E_Result(N_Vr::N_Asr::C_Request_Context_List_Update & ro_Request_Context_List_Update));
    MOCK_METHOD1(Recognize_Start_Sync, N_Vr::N_Asr::E_Result(N_Vr::N_Asr::C_Request_Recognize & ro_Request_Recognize));
    MOCK_METHOD1(Recognize_Start, N_Vr::N_Asr::E_Result(N_Vr::N_Asr::C_Request_Recognize & ro_Request_Recognize));
    MOCK_METHOD0(Recognize_Cancel, N_Vr::N_Asr::E_Result());
};


#endif
/* EOF */
