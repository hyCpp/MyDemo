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

/* Suntec Header */
#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#ifndef VR_ALEXAOBSERVER_H
#    include "VR_AlexaObserver.h"
#endif

#ifndef VR_GOOGLEOBSERVER_H
#    include "VR_GoogleObserver.h"
#endif

#ifndef VR_KEYWORDOBSERVER_H
#    include "VR_KeywordObserver.h"
#endif

#ifndef VR_CONFIGUREIF_H
#    include "VR_ConfigureIF.h"
#endif

VR_KeywordObserver::VR_KeywordObserver(
    std::shared_ptr<defaultClient::DefaultClient> client,
    capabilityAgents::aip::AudioProvider audioProvider,
    std::shared_ptr<VR_AlexaObserver> alexaObserver,
    std::shared_ptr<VR_GoogleObserver> googleObserver):
        m_client{client},
        m_audioProvider{audioProvider},
        m_alexaObserver{alexaObserver},
        m_googleObserver{googleObserver} {
}

void VR_KeywordObserver::onKeyWordDetected(
    std::shared_ptr<avsCommon::avs::AudioInputStream> stream,
    std::string keyword,
    avsCommon::avs::AudioInputStream::Index beginIndex,
    avsCommon::avs::AudioInputStream::Index endIndex)
{
    VR_LOGD_FUNC();

    VR_LOGI("keyword is: %s", keyword.c_str());

    if (("ALEXA" == keyword) || ("alexa" == keyword)) {
        if (m_alexaObserver) {
            if (!VR_ConfigureIF::Instance()->setEngineCurrentStatus(VR_Engine_Alexa_Activtity)) {
                VR_LOGD("engine status set failed");
                return;
            }
            m_alexaObserver->onKeyWordDetected();
        }
        else {
            VR_LOGI("m_alexaObserver is null");
        }

        // notify Alexa
        if (endIndex != avsCommon::sdkInterfaces::KeyWordObserverInterface::UNSPECIFIED_INDEX &&
            beginIndex == avsCommon::sdkInterfaces::KeyWordObserverInterface::UNSPECIFIED_INDEX) {
            if (m_client) {
                m_client->notifyOfTapToTalk(m_audioProvider, endIndex);
            }
        } else if (
            endIndex != avsCommon::sdkInterfaces::KeyWordObserverInterface::UNSPECIFIED_INDEX &&
            beginIndex != avsCommon::sdkInterfaces::KeyWordObserverInterface::UNSPECIFIED_INDEX) {
                if (m_client) {
                    m_client->notifyOfWakeWord(m_audioProvider, beginIndex, endIndex, keyword);
                }
        }

    }
    else if (("HELLO GOOGLE" == keyword) || ("hello google" == keyword)) {
        if (m_googleObserver) {
            m_googleObserver->onKeyWordDetected("google");
        }
        else {
            VR_LOGI("m_googleObserver for google is null");
        }
    }
    else if (("HELLO BIDU" == keyword) || ("hello bidu" == keyword)) {
        if (m_googleObserver) {
            m_googleObserver->onKeyWordDetected("baidu");
        }
        else {
            VR_LOGI("m_googleObserver for baidu is null");
        }
    }
    else {
        VR_LOGI("Unknown Keyword !");
    }
}

/* EOF */
