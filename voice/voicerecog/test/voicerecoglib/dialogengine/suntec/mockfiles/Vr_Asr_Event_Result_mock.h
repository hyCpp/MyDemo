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
 * @file Vr_Asr_Event_Result_mock.h
 * @brief Declaration file of Vr_Asr_Event_Result_mock.
 *
 * This file includes the declaration of Vr_Asr_Event_Result_mock.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ASREVENTRESULT_MOCK_H
#define VR_ASREVENTRESULT_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Vr_Asr_Event_Result.h"


/**
* Vr_Asr_Event_Result_mock
*
* The class is a mock class for Vr_Asr_Event_Result_mock.
*/
class C_Event_Result_mock : public N_Vr::N_Asr::C_Event_Result
{
public:
    MOCK_CONST_METHOD0(Get_XML, boost::shared_ptr<std::string const>());
    MOCK_CONST_METHOD0(Get_Json, boost::shared_ptr<std::string const>());
    MOCK_CONST_METHOD0(Get_Buffer_Userword_Transcription, N_Vr::N_Asr::C_Buffer());
    MOCK_CONST_METHOD0(Get_Buffer_Userword_Audio, N_Vr::N_Asr::C_Buffer());

};

#endif
/* EOF */
