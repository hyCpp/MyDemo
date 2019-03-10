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
 * @file VR_Settings_mock.h
 * @brief inteface for interpeter or asr engine to perfer command
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_AUDIOINSOURCE_MOCK_H
#define VR_AUDIOINSOURCE_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VR_AudioInSource.h"

/**
* VR_Settings_mock
*
* The class is a mock class for VR_Settings.
*/
class VR_AudioInSource_mock : public VR_AudioInSource
{
public:
    MOCK_METHOD2(M_Fn_Get_Data_Type_Audio_SSE, void(void ** ppvoid_Data_Type_Audio_SSE, size_t * pi_Size_Data_Type_Audio_SSE));
    MOCK_METHOD0(M_Fn_Start, int());
    MOCK_METHOD0(M_Fn_Stop, int());
    MOCK_METHOD1(M_Fn_Data_Available_Check, int(bool * pb_Data_Available));

    MOCK_METHOD2(M_Fn_Data_Fetch, int(void ** ppvoid_Buffer, size_t * pi_Size));
    MOCK_METHOD1(M_Fn_Data_Return, int(void * const & rpvoid_Buffer));

    MOCK_METHOD2(OnAudioInData, VOID(VOID * pData, INT len));
    MOCK_METHOD3(OnAudioInCustom, VOID(int type, VOID* data, INT len));
    MOCK_METHOD0(OnAudioInStopped, VOID());

    MOCK_CONST_METHOD0(IsStarted, bool());
    MOCK_METHOD0(Prepare, int());
    MOCK_METHOD0(UnPrepare, int());

    MOCK_METHOD0(enableVAD, void());
    MOCK_METHOD0(marktVadOffset, void());
    MOCK_METHOD1(setCachePcm, void(BOOL on));
    MOCK_METHOD1(setAudioMode, void(int mode));
};

#endif
/* EOF */


