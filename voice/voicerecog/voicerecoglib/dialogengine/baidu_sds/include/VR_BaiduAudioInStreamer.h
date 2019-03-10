#ifndef VR_BAIDU_AUDIO_IN_STREAMER_H
#define VR_BAIDU_AUDIO_IN_STREAMER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

// #include "GooAssPublic.h"
#include "VR_SDSController.h"
// #include "VR_Def.h"


/**
 * @brief The VR_BaiduAudioInStreamer class
 *
 * class declaration
 */
class VR_BaiduAudioInStreamer : public VR_AudioInStreamer
{
public:
    VR_BaiduAudioInStreamer(std::shared_ptr<VR_SDSController> controller);
    virtual ~VR_BaiduAudioInStreamer() {}
    virtual int onAudioInData(short* buf, size_t len) override;

private:
    std::shared_ptr<VR_SDSController> m_controller;
};

#endif // VR_BAIDU_AUDIO_IN_STREAMER_H
/* EOF */