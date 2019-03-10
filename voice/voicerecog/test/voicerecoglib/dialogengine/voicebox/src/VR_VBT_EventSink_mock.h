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
 * @file VR_VBT_VECIString_mock.h
 * @brief Mock class for IVECIString.
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_VBT_EVENTSINK_MOCK_H
#define VR_VBT_EVENTSINK_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "VR_VoiceBoxXmlParser.h"
#include "VR_VoiceBoxEventSink.h"

/**
* MockVR_VBT_EventSink
*
* The class is a mock class for VR_VoiceBoxEventSink.
*/
class MockVR_VBT_EventSink : public VR_VoiceBoxEventSink
{
public:
    MockVR_VBT_EventSink(
        IVECIEngineClient& client,
        IVECIEngineCommand& engineCommand,
        VR_VoiceBoxEngineCallback& engineCallback
    )
    : VR_VoiceBoxEventSink(client, engineCommand, engineCallback)
    {
    }

    bool Initialize()
    {
        return true;
    }

    void UnInitialize()
    {
        return;
    }

    // Mock methods
    MOCK_METHOD1(ReplyQueryInfo, bool(VR_VoiceBoxXmlParser& parser));
    MOCK_METHOD1(ProcessAgentMessage, bool(IVECIParsedMessage *pcMsg));

    MOCK_METHOD2(QueryAgentSupport, HRESULT(VBT_CSTR szAgent, VBT_BOOL *pbSupported));
    MOCK_METHOD2(OnAgentResultMessage, HRESULT(IVECITransaction *pTrans,
                                               IVECIParsedMessage *pResultMsg));
    MOCK_METHOD1(OnAgentDriverMessage, HRESULT(IVECIParsedMessage *pDriverMsg));
};

#endif  // VR_VBT_EVENTSINK_MOCK_H
/* EOF */
