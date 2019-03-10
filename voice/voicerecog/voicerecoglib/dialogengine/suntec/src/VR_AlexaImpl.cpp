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
#include "VR_AlexaImpl.h"
#include "VR_GoogleImpl.h"
#include "VR_Log.h"
#include "VR_UIManager.h"
#include "VR_ConnectionObserver.h"
#include "VR_PortAudioMicWrapper.h"
#include "VR_KeywordObserver.h"
#include "VR_InteractionManager.h"
#include "VR_ConsolePrinter.h"
//#include "VR_DummyMediaPlayer.h"
#include "VR_AlexaObserver.h"
#include "VR_GoogleObserver.h"
#include "VR_ControllerBase.h"

#include <Alerts/Storage/SQLiteAlertStorage.h>
#include <Settings/SQLiteSettingStorage.h>
#include <AuthDelegate/AuthDelegate.h>
#include <AVSCommon/AVS/Initialization/AlexaClientSDKInit.h>
#include <AVSCommon/Utils/Configuration/ConfigurationNode.h>
#include <AVSCommon/Utils/LibcurlUtils/HTTPContentFetcherFactory.h>
#include <Audio/AudioFactory.h>
// #include <Nuance/NuanceKeyWordDetector.h>
#include <Sphinx/SphinxKeyWordDetector.h>
#include <DefaultClient/DefaultClient.h>
#include <AVSCommon/Utils/Logger/LoggerSinkManager.h>

#ifndef CXX_GOOASSPUBLIC_H
#    include "GooAssPublic.h"
#endif

#ifndef VR_AUDIODATAWRITER_H
#    include "VR_AudioDataWriter.h"
#endif

#include <algorithm>
#include <cctype>
#include <fstream>

 using namespace alexaClientSDK;

/// The sample rate of microphone audio data.
static const unsigned int SAMPLE_RATE_HZ = 16000;

/// The number of audio channels.
static const unsigned int NUM_CHANNELS = 1;

/// The size of each word within the stream.
static const size_t WORD_SIZE = 2;

/// The maximum number of readers of the stream.
static const size_t MAX_READERS = 10;

/// The amount of audio data to keep in the ring buffer.
static const std::chrono::seconds AMpathToInputFolderOUNT_OF_AUDIO_DATA_IN_BUFFER = std::chrono::seconds(15);

/// The amount of audio data to keep in the ring buffer.
static const std::chrono::seconds AMOUNT_OF_AUDIO_DATA_IN_BUFFER = std::chrono::seconds(15);

/// The size of the ring buffer.
static const size_t BUFFER_SIZE_IN_SAMPLES = (SAMPLE_RATE_HZ)*AMOUNT_OF_AUDIO_DATA_IN_BUFFER.count();

/// Key for the root node value containing configuration values for SampleApp.
static const std::string SAMPLE_APP_CONFIG_KEY("sampleApp");

/// Key for the endpoint value under the @c SAMPLE_APP_CONFIG_KEY configuration node.
static const std::string ENDPOINT_KEY("endpoint");

/// Default AVS endpoint to connect to.
static const std::string DEFAULT_ENDPOINT("https://avs-alexa-na.amazon.com");

/// A set of all log levels.
static const std::set<alexaClientSDK::avsCommon::utils::logger::Level> allLevels = {
    alexaClientSDK::avsCommon::utils::logger::Level::DEBUG9,
    alexaClientSDK::avsCommon::utils::logger::Level::DEBUG8,
    alexaClientSDK::avsCommon::utils::logger::Level::DEBUG7,
    alexaClientSDK::avsCommon::utils::logger::Level::DEBUG6,
    alexaClientSDK::avsCommon::utils::logger::Level::DEBUG5,
    alexaClientSDK::avsCommon::utils::logger::Level::DEBUG4,
    alexaClientSDK::avsCommon::utils::logger::Level::DEBUG3,
    alexaClientSDK::avsCommon::utils::logger::Level::DEBUG2,
    alexaClientSDK::avsCommon::utils::logger::Level::DEBUG1,
    alexaClientSDK::avsCommon::utils::logger::Level::DEBUG0,
    alexaClientSDK::avsCommon::utils::logger::Level::INFO,
    alexaClientSDK::avsCommon::utils::logger::Level::WARN,
    alexaClientSDK::avsCommon::utils::logger::Level::ERROR,
    alexaClientSDK::avsCommon::utils::logger::Level::CRITICAL,
    alexaClientSDK::avsCommon::utils::logger::Level::NONE};

/**
 * Gets a log level consumable by the SDK based on the user input string for log level.
 *
 * @param userInputLogLevel The string to be parsed into a log level.
 * @return The log level. This will default to NONE if the input string is not properly parsable.
 */
static alexaClientSDK::avsCommon::utils::logger::Level getLogLevelFromUserInput(std::string userInputLogLevel) {
    std::transform(userInputLogLevel.begin(), userInputLogLevel.end(), userInputLogLevel.begin(), ::toupper);
    return alexaClientSDK::avsCommon::utils::logger::convertNameToLevel(userInputLogLevel);
}

// VR_AlexaImpl
VR_AlexaImpl::VR_AlexaImpl()
: m_controller(nullptr)
, m_micWrapper(nullptr)
, m_audioMicrophone(nullptr)
, m_audioDataWriter(nullptr)
{

}


bool
VR_AlexaImpl::Initialize(VR_ControllerBase* controller, std::shared_ptr<VR_GoogleObserver> googleObserver, alexaClientSDK::sampleApp::AudioMicrophone* audioMicrophone)
{
    VR_LOGD_FUNC();

    const std::string pathToConfig("/data/avs_data/AlexaClientSDKConfig.json");
    const std::string pathToInputFolder("/data/avs_data/inputs");
    const std::string logLevel("DEBUG9");

    m_controller = controller;
    m_alexaObserver = std::make_shared<VR_AlexaObserver>(m_controller);
    m_googleObserver = googleObserver;
    m_audioMicrophone = audioMicrophone;

    /*
     * Set up the SDK logging system to write to the SampleApp's ConsolePrinter.  Also adjust the logging level
     * if requested.
     */
    std::shared_ptr<alexaClientSDK::avsCommon::utils::logger::Logger> consolePrinter =
        std::make_shared<VR_ConsolePrinter>();

    if (!logLevel.empty()) {
        auto logLevelValue = getLogLevelFromUserInput(logLevel);
        if (alexaClientSDK::avsCommon::utils::logger::Level::UNKNOWN == logLevelValue) {
            for (auto it = allLevels.begin(); it != allLevels.end(); ++it) {
                VR_ConsolePrinter::simplePrint(
                    alexaClientSDK::avsCommon::utils::logger::convertLevelToName(*it));
            }
            return false;
        }

        VR_ConsolePrinter::simplePrint(
            "Running app with log level: " +
            alexaClientSDK::avsCommon::utils::logger::convertLevelToName(logLevelValue));
        consolePrinter->setLevel(logLevelValue);
    }
    alexaClientSDK::avsCommon::utils::logger::LoggerSinkManager::instance().initialize(consolePrinter);

    /*
     * This is a required step upon startup of the SDK before any modules are created. For that reason, it is being
     * called here, before creating the MediaPlayer, audio streams, DefaultClient, etc.
     */
    std::ifstream configInfile(pathToConfig);
    if (!configInfile.good()) {
        VR_LOGI("Failed to read AlexaImpl config file!");
        return false;
    }
    if (!avsCommon::avs::initialization::AlexaClientSDKInit::initialize({&configInfile})) {
        VR_LOGI("Failed to initialize AlexaImpl SDK!");
        return false;
    }

    auto config = alexaClientSDK::avsCommon::utils::configuration::ConfigurationNode::getRoot();
    auto httpContentFetcherFactory = std::make_shared<avsCommon::utils::libcurlUtils::HTTPContentFetcherFactory>();

    VR_LOGI("AlexaImpl begin to create mediaplay instances!");
    /*
     * Creating the media players. Here, the default GStreamer based MediaPlayer is being created. However, any
     * MediaPlayer that follows the specified MediaPlayerInterface can work.
     */
    // auto speakMediaPlayer = VR_DummyMediaPlayer::create(
    speakMediaPlayer = alexaClientSDK::mediaPlayer::MediaPlayer::create(
        httpContentFetcherFactory, avsCommon::sdkInterfaces::SpeakerInterface::Type::AVS_SYNCED, "SpeakMediaPlayer");

    if (!speakMediaPlayer) {
        VR_LOGI("Failed to create AlexaImpl media player for speech!");
        avsCommon::avs::initialization::AlexaClientSDKInit::uninitialize();
        return false;
    }

    // auto audioMediaPlayer = VR_DummyMediaPlayer::create(
    audioMediaPlayer = alexaClientSDK::mediaPlayer::MediaPlayer::create(
        httpContentFetcherFactory, avsCommon::sdkInterfaces::SpeakerInterface::Type::AVS_SYNCED, "AudioMediaPlayer");
    if (!audioMediaPlayer) {
        VR_LOGI("Failed to create AlexaImpl media player for content!");
        avsCommon::avs::initialization::AlexaClientSDKInit::uninitialize();
        return false;
    }

    /*
     * The ALERTS speaker type will cause volume control to be independent and localized. By assigning this type,
     * Alerts volume/mute changes will not be in sync with AVS. No directives or events will be associated with volume
     * control.
     */
    // auto alertsMediaPlayer = VR_DummyMediaPlayer::create(
    alertsMediaPlayer = alexaClientSDK::mediaPlayer::MediaPlayer::create(
        httpContentFetcherFactory, avsCommon::sdkInterfaces::SpeakerInterface::Type::LOCAL, "AlertsMediaPlayer");
    if (!alertsMediaPlayer) {
        VR_LOGI("Failed to create AlexaImpl media player for alerts!");
        avsCommon::avs::initialization::AlexaClientSDKInit::uninitialize();
        return false;
    }

    VR_LOGI("AlexaImpl finish to create mediaplay instances!");

    /*
     * Create Speaker interfaces to control the volume. For the SDK, the MediaPlayer happens to also provide
     * volume control functionality, but this does not have to be case.
     */
    std::shared_ptr<alexaClientSDK::avsCommon::sdkInterfaces::SpeakerInterface> speakSpeaker =
        std::static_pointer_cast<alexaClientSDK::avsCommon::sdkInterfaces::SpeakerInterface>(speakMediaPlayer);
    std::shared_ptr<alexaClientSDK::avsCommon::sdkInterfaces::SpeakerInterface> audioSpeaker =
        std::static_pointer_cast<alexaClientSDK::avsCommon::sdkInterfaces::SpeakerInterface>(audioMediaPlayer);
    std::shared_ptr<alexaClientSDK::avsCommon::sdkInterfaces::SpeakerInterface> alertsSpeaker =
        std::static_pointer_cast<alexaClientSDK::avsCommon::sdkInterfaces::SpeakerInterface>(alertsMediaPlayer);

    auto audioFactory = std::make_shared<alexaClientSDK::applicationUtilities::resources::audio::AudioFactory>();

    // Creating the alert storage object to be used for rendering and storing alerts.
    auto alertStorage = std::make_shared<alexaClientSDK::capabilityAgents::alerts::storage::SQLiteAlertStorage>(audioFactory->alerts());

    /*
     * Creating settings storage object to pathToInputFolderbe used for storing <key, value> pairs of AVS Settings.
     */
    auto settingsStorage = std::make_shared<alexaClientSDK::capabilityAgents::settings::SQLiteSettingStorage>();

    /*
     * Creating the UI component that observes various components and prints to the console accordingly.
     */
    auto userInterfaceManager = std::make_shared<VR_UIManager>(m_alexaObserver);

    /*
     * Setting up a connection observer to wait for connection and authorization prior to accepting user input at
     * startup.
     */
    auto connectionObserver = std::make_shared<VR_ConnectionObserver>();

    /*
     * Creating the AuthDelegate - this component takes care of LWA and authorization of the client. At the moment,
     * this must be done and authorization must be achieved prior to making the call to connect().
     */
    std::shared_ptr<alexaClientSDK::authDelegate::AuthDelegate> authDelegate =
        alexaClientSDK::authDelegate::AuthDelegate::create();

    authDelegate->addAuthObserver(connectionObserver);

    /*
     * Creating the DefaultClient - this component serves as an out-of-box default object that instantiates and "glues"
     * together all the modules.
     */
    std::shared_ptr<alexaClientSDK::defaultClient::DefaultClient> client =
        alexaClientSDK::defaultClient::DefaultClient::create(
            speakMediaPlayer,
            audioMediaPlayer,
            alertsMediaPlayer,
            speakSpeaker,
            audioSpeaker,
            alertsSpeaker,
            audioFactory,
            authDelegate,
            alertStorage,
            settingsStorage,
            {userInterfaceManager},
            {connectionObserver, userInterfaceManager});

    if (!client) {
        VR_LOGI("Failed to create default AlexaImpl SDK client!");
        avsCommon::avs::initialization::AlexaClientSDKInit::uninitialize();
        return false;
    }

    /*
     * TODO: ACSDK-384 Remove the requirement of clients having to wait for authorization before making the connect()
     * call.
     */
    if (!connectionObserver->waitFor(
            alexaClientSDK::avsCommon::sdkInterfaces::AuthObserverInterface::State::REFRESHED)) {
        VR_LOGI("Failed to authorize AlexaImpl SDK client!");
        client.reset();
        avsCommon::avs::initialization::AlexaClientSDKInit::uninitialize();
        return false;
    }

    VR_LOGI("AlexaImpl begin to connect to AVS!");
    std::string endpoint;
    config[SAMPLE_APP_CONFIG_KEY].getString(ENDPOINT_KEY, &endpoint, DEFAULT_ENDPOINT);

    client->connect(endpoint);

    if (!connectionObserver->waitFor(avsCommon::sdkInterfaces::ConnectionStatusObserverInterface::Status::CONNECTED, std::chrono::seconds(40))) {
        VR_LOGI("AlexaImpl Failed to connect to AVS!");
        client.reset();
        avsCommon::avs::initialization::AlexaClientSDKInit::uninitialize();
        return false;
    }

    VR_LOGI("AlexaImpl finish to connect to AVS!");

    // Add userInterfaceManager as observer of locale setting.
    client->addSettingObserver("locale", userInterfaceManager);
    // Send default settings set by the user to AVS.
    client->sendDefaultSettings();

    client->addSpeakerManagerObserver(userInterfaceManager);
    client->addAudioPlayerObserver(userInterfaceManager);

    /*
     * Creating the buffer (Shared Data Stream) that will hold user audio data. This is the main input into the SDK.
     */
    size_t bufferSize = alexaClientSDK::avsCommon::avs::AudioInputStream::calculateBufferSize(
        BUFFER_SIZE_IN_SAMPLES, WORD_SIZE, MAX_READERS);
    auto buffer = std::make_shared<alexaClientSDK::avsCommon::avs::AudioInputStream::Buffer>(bufferSize);
    std::shared_ptr<alexaClientSDK::avsCommon::avs::AudioInputStream> sharedDataStream =
        alexaClientSDK::avsCommon::avs::AudioInputStream::create(buffer, WORD_SIZE, MAX_READERS);

    if (!sharedDataStream) {
        VR_LOGI("Failed to create AlexaImpl shared data stream!");
        client.reset();
        avsCommon::avs::initialization::AlexaClientSDKInit::uninitialize();
        return false;
    }

    alexaClientSDK::avsCommon::utils::AudioFormat compatibleAudioFormat;
    compatibleAudioFormat.sampleRateHz = SAMPLE_RATE_HZ;
    compatibleAudioFormat.sampleSizeInBits = WORD_SIZE * CHAR_BIT;
    compatibleAudioFormat.numChannels = NUM_CHANNELS;
    compatibleAudioFormat.endianness = alexaClientSDK::avsCommon::utils::AudioFormat::Endianness::LITTLE;
    compatibleAudioFormat.encoding = alexaClientSDK::avsCommon::utils::AudioFormat::Encoding::LPCM;

    /*
     * Creating each of the audio providers. An audio provider is a simple package of data consisting of the stream
     * of audio data, as well as metadata about the stream. For each of the three audio providers created here, the same
     * stream is used since this sample application will only have one microphone.
     */

    // Creating tap to talk audio provider
    bool tapAlwaysReadable = true;
    bool tapCanOverride = true;
    bool tapCanBeOverridden = true;

    alexaClientSDK::capabilityAgents::aip::AudioProvider tapToTalkAudioProvider(
        sharedDataStream,
        compatibleAudioFormat,
        alexaClientSDK::capabilityAgents::aip::ASRProfile::NEAR_FIELD,
        tapAlwaysReadable,
        tapCanOverride,
        tapCanBeOverridden);

    // Creating hold to talk audio provider
    bool holdAlwaysReadable = false;
    bool holdCanOverride = true;
    bool holdCanBeOverridden = false;

    alexaClientSDK::capabilityAgents::aip::AudioProvider holdToTalkAudioProvider(
        sharedDataStream,
        compatibleAudioFormat,
        alexaClientSDK::capabilityAgents::aip::ASRProfile::CLOSE_TALK,
        holdAlwaysReadable,
        holdCanOverride,
        holdCanBeOverridden);

    m_micWrapper =
        VR_PortAudioMicWrapper::create(sharedDataStream);
    if (!m_micWrapper) {
        VR_LOGI("Failed to create AlexsaImp PortAudioMicrophoneWrapper!");
        client.reset();
        avsCommon::avs::initialization::AlexaClientSDKInit::uninitialize();
        return false;
    }
    m_audioDataWriter = std::make_shared<VR_AudioDataWriter>(m_micWrapper);

    if (nullptr != m_audioMicrophone) {
        m_audioMicrophone->setAudioDataWriter(m_audioDataWriter);
    }
    VR_LOGI("setAudioDataWriter sucessfully\n");
    m_micWrapper->SetMicrophoneObj(m_audioMicrophone);
    VR_LOGI("SetMicrophoneObj sucessfully\n");

    m_audioMicrophone->initializeAEC();
    // fileSenderWrapper = std::make_shared<VR_AudioFileSender>(sharedDataStream);
    // if (!fileSenderWrapper) {
    //     VR_LOGI("Failed to create AlexsaImp VR_AudioFileSender!");
    //     return false;
    // }


    bool wakeAlwaysReadable = true;
    bool wakeCanOverride = false;
    bool wakeCanBeOverridden = true;

    alexaClientSDK::capabilityAgents::aip::AudioProvider wakeWordAudioProvider(
        sharedDataStream,
        compatibleAudioFormat,
        alexaClientSDK::capabilityAgents::aip::ASRProfile::NEAR_FIELD,
        wakeAlwaysReadable,
        wakeCanOverride,
        wakeCanBeOverridden);

    // This observer is notified any time a keyword is detected and notifies the DefaultClient to start recognizing.
    auto keywordObserver = std::make_shared<VR_KeywordObserver>(client, wakeWordAudioProvider, m_alexaObserver, m_googleObserver);

    // m_keywordDetector = kwd::NuanceKeyWordDetector::create(
    //     sharedDataStream,
    //     compatibleAudioFormat,
    //     {keywordObserver},
    //     std::unordered_set<
    //         std::shared_ptr<alexaClientSDK::avsCommon::sdkInterfaces::KeyWordDetectorStateObserverInterface>>(),
    //     {pathToInputFolder + "/acmod6_6000_enu_gen_car_f16_v1_0_1.dat", pathToInputFolder + "/clc_enu_cfg3_v6_0_2.dat", "alexa"}, 2.0f);
    // if (!m_keywordDetector) {
    //     VR_LOGI("Failed to create AlexaImpl NuanceKeywWordDetector!");
    //     return false;
    // }

    m_keywordDetector = kwd::SphinxKeyWordDetector::create(
        sharedDataStream,
        compatibleAudioFormat,
        {keywordObserver},
        std::unordered_set<
        std::shared_ptr<alexaClientSDK::avsCommon::sdkInterfaces::KeyWordDetectorStateObserverInterface>>(),
        pathToInputFolder,
        std::chrono::milliseconds(10));
    if (!m_keywordDetector) {
        VR_LOGI("Failed to create AlexaImpl SphinxKeyWordDetector!");
        client.reset();
        avsCommon::avs::initialization::AlexaClientSDKInit::uninitialize();
        if (nullptr != m_audioMicrophone) {
            m_audioMicrophone->setAudioDataWriter(nullptr);
        }
       return false;
    }



    // If wake word is enabled, then creating the interaction manager with a wake word audio provider.
    m_interactionManager = std::make_shared<VR_InteractionManager>(
        client,
        m_micWrapper,
        userInterfaceManager,
        m_alexaObserver,
        holdToTalkAudioProvider,
        tapToTalkAudioProvider,
        wakeWordAudioProvider);

    if (!m_interactionManager) {
        VR_LOGI("Failed to create AlexaImpl Interaction Manager!");
        client.reset();
        avsCommon::avs::initialization::AlexaClientSDKInit::uninitialize();
        if (nullptr != m_audioMicrophone) {
            m_audioMicrophone->setAudioDataWriter(nullptr);
        }
        return false; 
    }

    m_micWrapper->SetInteractionObj(m_interactionManager);

    client->addAlexaDialogStateObserver(m_interactionManager);

    m_interactionManager->begin();

    VR_LOGI("AlexaImpl initialize sucessfully\n");
    return true;
}

void VR_AlexaImpl::UnInitialize()
{
    VR_LOGD_FUNC();
    if (m_interactionManager) {
        m_interactionManager->shutdown();
    }


}

bool VR_AlexaImpl::Start()
{
    VR_LOGD_FUNC();
    return true;
}

void VR_AlexaImpl::Stop()
{
    VR_LOGD_FUNC();
}

VR_AlexaImpl::~VR_AlexaImpl()
{

}

void VR_AlexaImpl::ProcessMessage(const std::string& message)
{
    VR_LOGD_FUNC();
    if (!m_interactionManager) {
        VR_LOGI("AlexaImpl interaction manager is NULL!!!\n");
        return;
    }

    if ("tap" == message) {
        // m_interactionManager->startFileAudio("/pdata/GetGasStationReq.wav");  // GetGasStationReq.wav   GetParkReq.wav
        m_interactionManager->tap();
        // m_interactionManager->microphoneToggle();
    }

    if ("talkEnd" == message) {
        m_interactionManager->talkEnd();
    }

    // if ("audioFile" == message) {
    //     m_interactionManager->startFileAudio("");  // GetGasStationReq.wav   GetParkReq.wav
    // }
}

void VR_AlexaImpl::OnPlayAudioFile(const std::string& strPath)
{
    VR_LOGD_FUNC();
    if (!m_interactionManager) {
        VR_LOGI("AlexaImpl interaction manager is NULL!!!\n");
        return;
    }

    m_interactionManager->startFileAudio(strPath);
}

void VR_AlexaImpl::StopAudioFile()
{
    VR_LOGD_FUNC();
    if (!m_interactionManager) {
        VR_LOGI("AlexaImpl interaction manager is NULL!!!\n");
        return;
    }

    m_interactionManager->stopFileAudio();
}


std::shared_ptr<VR_PortAudioMicWrapper> VR_AlexaImpl::GetAudioWrapper()
{
    return m_micWrapper;
}

/* EOF */
