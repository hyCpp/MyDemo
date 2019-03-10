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

#include "VR_DummyMediaPlayer.h"
#include "VR_Log.h"


std::shared_ptr<VR_DummyMediaPlayer> VR_DummyMediaPlayer::create(
    std::shared_ptr<avsCommon::sdkInterfaces::HTTPContentFetcherInterfaceFactoryInterface> contentFetcherFactory,
    SpeakerInterface::Type type) {
    std::shared_ptr<VR_DummyMediaPlayer> mediaPlayer(new VR_DummyMediaPlayer(contentFetcherFactory, type));
    return mediaPlayer;
};

VR_DummyMediaPlayer::~VR_DummyMediaPlayer() {

}

VR_DummyMediaPlayer::SourceId VR_DummyMediaPlayer::setSource(std::shared_ptr<avsCommon::avs::attachment::AttachmentReader> reader) {
    ACSDK_DEBUG9(LX("setSourceCalled").d("sourceType", "AttachmentReader"));
    std::promise<VR_DummyMediaPlayer::SourceId> promise;
    auto future = promise.get_future();
    return future.get();
}

VR_DummyMediaPlayer::SourceId VR_DummyMediaPlayer::setSource(std::shared_ptr<std::istream> stream, bool repeat) {
    ACSDK_DEBUG9(LX("setSourceCalled").d("sourceType", "istream"));
    std::promise<VR_DummyMediaPlayer::SourceId> promise;
    auto future = promise.get_future();
    return future.get();
}

VR_DummyMediaPlayer::SourceId VR_DummyMediaPlayer::setSource(const std::string& url) {
    ACSDK_DEBUG9(LX("setSourceForUrlCalled").sensitive("url", url));
    std::promise<VR_DummyMediaPlayer::SourceId> promise;
    auto future = promise.get_future();
    return future.get();
}

bool VR_DummyMediaPlayer::play(VR_DummyMediaPlayer::SourceId id) {
    return false;
}

bool VR_DummyMediaPlayer::stop(VR_DummyMediaPlayer::SourceId id) {
    return false;
}

bool VR_DummyMediaPlayer::pause(VR_DummyMediaPlayer::SourceId id) {
    return false;
}

bool VR_DummyMediaPlayer::resume(VR_DummyMediaPlayer::SourceId id) {
    return false;
}

std::chrono::milliseconds VR_DummyMediaPlayer::getOffset(VR_DummyMediaPlayer::SourceId id) {
    std::chrono::milliseconds promise;
    return promise;
}

bool VR_DummyMediaPlayer::setOffset(VR_DummyMediaPlayer::SourceId id, std::chrono::milliseconds offset) {
    return false;
}

void VR_DummyMediaPlayer::setObserver(std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerObserverInterface> observer) {
    return;
}

bool VR_DummyMediaPlayer::setVolume(int8_t volume) {
    return false;
}

bool VR_DummyMediaPlayer::adjustVolume(int8_t delta) {
    return false;
}

bool VR_DummyMediaPlayer::setMute(bool mute) {
    return false;
}

bool VR_DummyMediaPlayer::getSpeakerSettings(SpeakerInterface::SpeakerSettings* settings) {
    return false;
}

avsCommon::sdkInterfaces::SpeakerInterface::Type VR_DummyMediaPlayer::getSpeakerType() {
    return avsCommon::sdkInterfaces::SpeakerInterface::Type::AVS_SYNCED;
}

VR_DummyMediaPlayer::VR_DummyMediaPlayer(
    std::shared_ptr<avsCommon::sdkInterfaces::HTTPContentFetcherInterfaceFactoryInterface> contentFetcherFactory,
    SpeakerInterface::Type type)
{
}

/* EOF */
