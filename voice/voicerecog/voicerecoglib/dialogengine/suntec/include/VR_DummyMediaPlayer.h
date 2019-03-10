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
 * @file VR_MsgController.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_DUMMYMEDIAPLAYER_H
#define VR_DUMMYMEDIAPLAYER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>

#include <AVSCommon/SDKInterfaces/HTTPContentFetcherInterfaceFactoryInterface.h>
#include <AVSCommon/SDKInterfaces/SpeakerInterface.h>
#include <AVSCommon/Utils/MediaPlayer/MediaPlayerObserverInterface.h>
#include <AVSCommon/Utils/MediaPlayer/MediaPlayerInterface.h>

using namespace alexaClientSDK;

/**
 * Class that handles creation of audio pipeline and playing of audio data.
 */
class VR_DummyMediaPlayer
        : public avsCommon::utils::mediaPlayer::MediaPlayerInterface
        , public avsCommon::sdkInterfaces::SpeakerInterface {
public:
    /**
     * Creates an instance of the @c MediaPlayer.
     *
     * @param contentFetcherFactory Used to create objects that can fetch remote HTTP content.
     * @param type The type used to categorize the speaker for volume control.
     * @return An instance of the @c MediaPlayer if successful else a @c nullptr.
     */
    static std::shared_ptr<VR_DummyMediaPlayer> create(
        std::shared_ptr<avsCommon::sdkInterfaces::HTTPContentFetcherInterfaceFactoryInterface> contentFetcherFactory =
            nullptr,
        avsCommon::sdkInterfaces::SpeakerInterface::Type type =
            avsCommon::sdkInterfaces::SpeakerInterface::Type::AVS_SYNCED);

    /**
     * Destructor.
     */
    ~VR_DummyMediaPlayer();

    /// @name Overridden MediaPlayerInterface methods.
    /// @{
    SourceId setSource(std::shared_ptr<avsCommon::avs::attachment::AttachmentReader> attachmentReader) override;
    SourceId setSource(std::shared_ptr<std::istream> stream, bool repeat) override;
    SourceId setSource(const std::string& url) override;

    bool play(SourceId id) override;
    bool stop(SourceId id) override;
    bool pause(SourceId id) override;
    /**
     * To resume playback after a pause, call @c resume. Calling @c play
     * will reset the pipeline and source, and will not resume playback.
     */
    bool resume(SourceId id) override;
    std::chrono::milliseconds getOffset(SourceId id) override;
    bool setOffset(SourceId id, std::chrono::milliseconds offset) override;
    void setObserver(std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerObserverInterface> observer) override;
    /// @}

    /// @name Overridden SpeakerInterface methods.
    /// @{
    bool setVolume(int8_t volume) override;
    bool adjustVolume(int8_t volume) override;
    bool setMute(bool mute) override;
    bool getSpeakerSettings(avsCommon::sdkInterfaces::SpeakerInterface::SpeakerSettings* settings) override;
    avsCommon::sdkInterfaces::SpeakerInterface::Type getSpeakerType() override;
    /// @}

    /**
     * Constructor.
     *
     * @param contentFetcherFactory Used to create objects that can fetch remote HTTP content.
     * @param type The type used to categorize the speaker for volume control.
     */
    VR_DummyMediaPlayer(
        std::shared_ptr<avsCommon::sdkInterfaces::HTTPContentFetcherInterfaceFactoryInterface> contentFetcherFactory,
        avsCommon::sdkInterfaces::SpeakerInterface::Type type);
};

#endif // VR_DUMMYMEDIAPLAYER_H
/* EOF */
