/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_DataProvider.h
 * @brief Declaration file of class VR_DataProvider.
 *
 * This file includes the declaration of class VR_DataProvider.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DATA_PROVIDER_H
#define VR_DATA_PROVIDER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#include "VR_Def.h"
#include "VR_Macro.h"
#include "VR_DP_OBListener.h"
#include "VR_DataProviderIF.h"
#include "VR_DP_ThreadWorking.h"

#include "ncore/NCTypesDefine.h"
#include "MediaDataProviderIF/include/NMMediaDataProviderIF.h"

#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "VR_PhoneBookXMLFormat.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_CommandHandleIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RunableThread);

namespace navi
{
namespace VoiceRecog
{
    class VrDPDictionaryResult;
    class VrFileUpdateRightReleaseRequest;
    class VrAgentActivation;
}
}

namespace navi
{
namespace Traffic
{
    class WeatherCityListRes;
    class WeatherCityListUpdate;
}
}

namespace nutshell
{
namespace media
{
    class NMMediaDataProviderIF;
}
}
typedef boost::shared_ptr<nutshell::media::NMMediaDataProviderIF> spNMMediaDataProviderIF;

namespace navi
{
namespace dataprovider
{

typedef struct __stPlayInfo {
    bool                    IsPlaying; // true: playing, false: not playing
    bool                    HasArtist; // empty name has no artist
    std::string             Artist;
    bool                    HasSong;   // empty song has no song
    std::string             Song;
    bool                    IsEnable;  // source is enable
    std::list<VR_String>    GenreList; // only usb support genre list 

    __stPlayInfo()
    {
        IsPlaying = false;
        HasArtist = false;
        HasSong = false;
        IsEnable = false;
    }

} stPlayInfo;

/**
 * @brief The VR_DataProvider class
 *
 * class declaration
 */
class VR_DataProvider : public VR_DataProviderIF
{
public:
#ifdef HAVE_NUTSHELL_OS
    VR_DataProvider();
#else
    VR_DataProvider();
#endif
    virtual ~VR_DataProvider();

    virtual bool Initialize();
    virtual bool RegistObserver();
    virtual void UnRegistObserver();
    virtual void StartDPThread();
    virtual void StopDPThread();

    void setMediaProvider(spNMMediaDataProviderIF& ptr);
    void OnStartedNotify();
    void SendMesgNATP(VR_String& mesg);
    bool OnReplyMusicDevStatus(VR_String& id, VR_String& srcType);
    bool OnReplyMusicGetLocalInfo(VR_String& strxml);
    bool OnReplyMusicGetDBData(VR_String& id, VR_String& srcId);

    bool OnReplyPhoneStatus(const VR_String& strId);
    bool OnReplyPhoneDBData(const VR_String& strId);
    bool OnReplyPhoneNumberGet(const VR_String& strId, const VR_String& strContactId, const VR_String& strName);
    bool OnSetSMSSupportState(const VR_String& strMsg);

    bool NotifyFinishedData();
    bool NotifyCheckUDiskSuccess();

private:
    bool GetUsbPlayInfo(stPlayInfo& info);
    bool GetIPodPlayInfo(stPlayInfo& info);
    bool GetBTAPlayInfo(stPlayInfo& info);
    bool GetOptionSourceState(const VR_String& key, VR_String& srcStateXml, VR_String& state);

    bool GetPhoneDataId(VR_String& strDevice);
    bool GetPhoneDBPath(VR_String& strPath);
    bool GetPhoneStatus(VR_String& strStatus);
    bool GetPhoneSignalStatus(VR_String& strStatus);
    bool GetPhoneNumber(const VR_String& strContactId, myPersonInfo& personInfo);
    bool GetPhoneType(int iType, VR_String& strPhoneType);
    bool GetCurrentMediaSource(int& srcId);

private:
#ifdef HAVE_NUTSHELL_OS
    spVR_DP_OBListener                m_spDataProviderListener;
    spVR_DP_ThreadWorking             m_spDPThread;
    spVR_RunableThread                m_spRunableThread;
    spNMMediaDataProviderIF           m_mediaDataProvider;
#endif
};

}
}
#endif /* VR_DARA_PROVIDER_H */
/* EOF */
