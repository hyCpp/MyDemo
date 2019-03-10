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


#include "VR_InteractionManager.h"
#include "VR_Log.h"
#ifndef VR_ALEXAOBSERVER_H
#    include "VR_AlexaObserver.h"
#endif

static const std::string ALEXA_WELCOME_MESSAGE =
    "                  #    #     #  #####      #####  ######  #    #              \n"
    "                 # #   #     # #     #    #     # #     # #   #               \n"
    "                #   #  #     # #          #       #     # #  #                \n"
    "               #     # #     #  #####      #####  #     # ###                 \n"
    "               #######  #   #        #          # #     # #  #                \n"
    "               #     #   # #   #     #    #     # #     # #   #               \n"
    "               #     #    #     #####      #####  ######  #    #              \n"
    "                                                                              \n"
    "       #####                                           #                      \n"
    "      #     #   ##   #    # #####  #      ######      # #   #####  #####      \n"
    "      #        #  #  ##  ## #    # #      #          #   #  #    # #    #     \n"
    "       #####  #    # # ## # #    # #      #####     #     # #    # #    #     \n"
    "            # ###### #    # #####  #      #         ####### #####  #####      \n"
    "      #     # #    # #    # #      #      #         #     # #      #          \n"
    "       #####  #    # #    # #      ###### ######    #     # #      #          \n";

using namespace alexaClientSDK::avsCommon::sdkInterfaces;

VR_InteractionManager::VR_InteractionManager(
    std::shared_ptr<defaultClient::DefaultClient> client,
    std::shared_ptr<VR_PortAudioMicWrapper> micWrapper,
    std::shared_ptr<VR_UIManager> userInterface,
    std::shared_ptr<VR_AlexaObserver> observer,
    capabilityAgents::aip::AudioProvider holdToTalkAudioProvider,
    capabilityAgents::aip::AudioProvider tapToTalkAudioProvider,
    capabilityAgents::aip::AudioProvider wakeWordAudioProvider) :
        RequiresShutdown{"VR_InteractionManager"},
        m_client{client},
        m_micWrapper{micWrapper},
        m_userInterface{userInterface},
        m_holdToTalkAudioProvider{holdToTalkAudioProvider},
        m_tapToTalkAudioProvider{tapToTalkAudioProvider},
        m_wakeWordAudioProvider{wakeWordAudioProvider},
        m_isHoldOccurring{false},
        m_isTapOccurring{false},
        m_isMicOn{true},
        m_isAudioFileOccurring{false} {
    m_micWrapper->startStreamingMicrophoneData();
    auto guiRenderer = std::make_shared<VR_GuiRenderer>(observer);
    m_observer = observer;
    m_client->addTemplateRuntimeObserver(guiRenderer);
};

void VR_InteractionManager::begin() {
    VR_LOGD_FUNC();
    VR_LOGI("%s", ALEXA_WELCOME_MESSAGE.c_str());
}

void VR_InteractionManager::help() {
    VR_LOGD_FUNC();
}

void VR_InteractionManager::settings() {
    VR_LOGD_FUNC();
}

void VR_InteractionManager::locale() {
    VR_LOGD_FUNC();
}

void VR_InteractionManager::errorValue() {
    VR_LOGD_FUNC();
}

void VR_InteractionManager::changeSetting(const std::string& key, const std::string& value) {
    VR_LOGD_FUNC();
    m_executor.submit([this, key, value]() { m_client->changeSetting(key, value); });
}

void VR_InteractionManager::startFileAudio(const std::string& path)
{
    VR_LOGD_FUNC();

    if (!m_isAudioFileOccurring) {
       // stopForegroundActivity();
        VR_LOGI("stopForegroundActivity");
        if (m_client->notifyOfTapToTalk(m_tapToTalkAudioProvider).get()) {
            m_isAudioFileOccurring = true;
            microphoneToggle();
            m_micWrapper->startStreamingData(path);
        }
        else {
            VR_LOGI("notifyOfHoldToTalkStart return false");
            NotifyCloseSuggestedSession();

        }
    }
    else {
        VR_LOGI("m_isAudioFileOccurring is true");
        NotifyCloseSuggestedSession();
    }
}

void VR_InteractionManager::stopFileAudio()
{
    VR_LOGD_FUNC();
    if (m_isAudioFileOccurring) {
        m_isAudioFileOccurring = false;
        // m_client->notifyOfTapToTalkEnd();
        // m_micWrapper->stopStreamingData();
        microphoneToggle();
    }
    else {
        VR_LOGI("stopFileAudio is false");
    }
}

void VR_InteractionManager::microphoneToggle() {
    VR_LOGD_FUNC();
    m_executor.submit([this]() {
        if (!m_wakeWordAudioProvider) {
            return;
        }
        VR_LOGI("m_isMicOn : %d", m_isMicOn);
        if (m_isMicOn) {
            m_isMicOn = false;
            m_micWrapper->stopStreamingMicrophoneData();
            // m_userInterface->microphoneOff();
        } else {
            m_isMicOn = true;
            m_micWrapper->startStreamingMicrophoneData();
            // m_userInterface->microphoneOn();
        }
    });
}

void VR_InteractionManager::holdToggled() {
    VR_LOGD_FUNC();
    m_executor.submit([this]() {
        if (!m_isMicOn) {
            return;
        }
        if (!m_isHoldOccurring) {
            if (m_client->notifyOfHoldToTalkStart(m_holdToTalkAudioProvider).get()) {
                m_isHoldOccurring = true;
            }
        } else {
            m_isHoldOccurring = false;
            m_client->notifyOfHoldToTalkEnd();
        }
    });
}

void VR_InteractionManager::tap() {
    VR_LOGD_FUNC();
    m_executor.submit([this]() {
        if (!m_isMicOn) {
            return;
        }
        if (!m_isTapOccurring) {
            if (m_client->notifyOfTapToTalk(m_tapToTalkAudioProvider).get()) {
                m_isTapOccurring = true;
            }
        } else {
            m_isTapOccurring = false;
            m_client->notifyOfTapToTalkEnd();
        }

    });
}

void VR_InteractionManager::stopForegroundActivity() {
    VR_LOGD_FUNC();
    m_executor.submit([this]() { m_client->stopForegroundActivity(); });
}

void VR_InteractionManager::playbackPlay() {
    VR_LOGD_FUNC();
    m_executor.submit([this]() { m_client->getPlaybackControllerInterface().playButtonPressed(); });
}

void VR_InteractionManager::playbackPause() {
    VR_LOGD_FUNC();
    m_executor.submit([this]() { m_client->getPlaybackControllerInterface().pauseButtonPressed(); });
}

void VR_InteractionManager::playbackNext() {
    VR_LOGD_FUNC();
    m_executor.submit([this]() { m_client->getPlaybackControllerInterface().nextButtonPressed(); });
}

void VR_InteractionManager::playbackPrevious() {
    VR_LOGD_FUNC();
    m_executor.submit([this]() { m_client->getPlaybackControllerInterface().previousButtonPressed(); });
}

void VR_InteractionManager::speakerControl() {
    VR_LOGD_FUNC();
}

void VR_InteractionManager::volumeControl() {
    VR_LOGD_FUNC();
}

void VR_InteractionManager::adjustVolume(avsCommon::sdkInterfaces::SpeakerInterface::Type type, int8_t delta) {
    VR_LOGD_FUNC();
    m_executor.submit([this, type, delta]() {
        /*
         * Group the unmute action as part of the same affordance that caused the volume change, so we don't
         * send another event. This isn't a requirement by AVS.
         */
        std::future<bool> unmuteFuture = m_client->getSpeakerManager()->setMute(type, false, true);
        if (!unmuteFuture.valid()) {
            return;
        }
        unmuteFuture.get();

        std::future<bool> future = m_client->getSpeakerManager()->adjustVolume(type, delta);
        if (!future.valid()) {
            return;
        }
        future.get();
    });
}

void VR_InteractionManager::setMute(avsCommon::sdkInterfaces::SpeakerInterface::Type type, bool mute) {
    VR_LOGD_FUNC();
    m_executor.submit([this, type, mute]() {
        std::future<bool> future = m_client->getSpeakerManager()->setMute(type, mute);
        future.get();
    });
}

void VR_InteractionManager::onDialogUXStateChanged(DialogUXState state) {
    VR_LOGD_FUNC();

    // reset tap-to-talk state
    if (DialogUXState::IDLE == state) {
        m_isTapOccurring = false;
    }
}

void VR_InteractionManager::doShutdown() {
    VR_LOGD_FUNC();
    m_client.reset();
}

void VR_InteractionManager::talkEnd() {
    VR_LOGD_FUNC();
    m_executor.submit([this]() {
        if (!m_isMicOn) {
            return;
        }

        m_client->notifyOfTapToTalkEnd();
    });
}

void VR_InteractionManager::NotifyCloseSuggestedSession()
{
    VR_LOGD_FUNC();

    stopFileAudio();

    if (NULL == m_observer) {
        VR_LOGI("m_observer is null");
        return;
    }

    m_observer->NotifyMessageToEngine("<event name=\"closeSuggestSession\"/>");
}

/* EOF */
