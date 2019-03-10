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


#ifndef VR_SPEECHCOMMANDOBSERVER_H
#    include "VR_SpeechCommandObserver.h"
#endif

#ifndef VR_CONTROLLERBASE_H
#    include "VR_ControllerBase.h"
#endif

#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

namespace SphinxS {

VR_SpeechCommandObserver::VR_SpeechCommandObserver(VR_AsrEngineIF* pListener)
:m_pListener(pListener)
{

}

void VR_SpeechCommandObserver::onBeginofSpeechDetected()
{
    // printf("##### Test sphinx Begin speech detected!\n");
    VR_LOGD_FUNC();
    // if (m_controller) {
    //    m_controller->PostMessage("LISTENING", -1);
    // }
}

void VR_SpeechCommandObserver::onEndofSpeechDetected(const std::string & cPath)
{
    // printf("##### Test sphinx PCM path: %s\n", cPath.c_str());
    VR_LOGD_FUNC();
    if (m_pListener) {
        m_pListener->postRequest(cPath);
    }
}

void VR_SpeechCommandObserver::onKeyWordSpeechDetected(const std::string & keyword)
{
    // printf("##### Test sphinx speech detector Keyword result: %s\n", keyword.c_str());
    VR_LOGD_FUNC();
    if (m_pListener) {
        m_pListener->PostMessage("keyWordDetected", -1);
    }

}

}  // namespace SphinxS
