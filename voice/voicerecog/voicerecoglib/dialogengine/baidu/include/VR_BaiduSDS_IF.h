#ifndef VR_BAIDUSDS_IF_H
#define VR_BAIDUSDS_IF_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

// #include "GooAssPublic.h"
#include "VR_Def.h"
#include <memory>


// DialogState
enum class VR_API VR_DialogStateBaidu{
    IDLE = 0,
    LISTENING,
    THINKING,
    SPEAKING,
    ERROR
};

// AudioInstreamer
class VR_API VR_AudioInStreamer
{
public:
    virtual ~VR_AudioInStreamer() {}
    virtual int onAudioInData(short* buf, size_t len) = 0;
};

class VR_SDSController;
class VR_MsgController;
class VR_BaiduAudioInStreamer;

/**
 * @brief The VR_BaiduSDSListener interface
 *
 * interface declaration
 */
class VR_API VR_BaiduSDSListener
{
public:
    virtual ~VR_BaiduSDSListener() {}
    virtual void onDialogStateChanged(VR_DialogStateBaidu state) = 0;
    virtual void OnRequestAction(const std::string& message, int seqId) = 0;
    virtual void OnStarted() = 0;
    virtual void OnStopped() = 0;
};

/**
 * @brief The VR_BaiduSDS class
 *
 * class declaration
 */
class VR_API VR_BaiduSDS
{
public:
    virtual ~VR_BaiduSDS() {}
    static VR_BaiduSDS* CreateBaiduSDS(VR_BaiduSDSListener* listener);
    static void Destroy();

    // Session begin
    virtual VR_AudioInStreamer* startDialog();

    // Session stop
    virtual int stopDialog();

    virtual bool onMessage(const std::string& message, int actionSeqId);

private:
    VR_BaiduSDS();
    VR_BaiduSDS(const VR_BaiduSDS&);
    VR_BaiduSDS& operator=(const VR_BaiduSDS&);

    void initialize(VR_BaiduSDSListener* listener);
    void unInitialize();
    void start();
    void stop();

    static VR_BaiduSDS*     pBaiduSDSInstance;
    VR_BaiduSDSListener*    m_pListener;
    std::shared_ptr<VR_SDSController>           m_pController;
    std::shared_ptr<VR_MsgController>           m_msgController;
    std::shared_ptr<VR_BaiduAudioInStreamer>    m_audioStreamer;
};

#endif // VR_BAIDUSDS_IF_H
/* EOF */