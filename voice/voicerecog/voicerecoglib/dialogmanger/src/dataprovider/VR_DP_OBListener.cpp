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

#include "VR_Log.h"
#include "VR_Configure.h"
#include "VR_DP_OBListener.h"
#include "VR_DialogEngineIF.h"
#include "VR_DP_OB_Action_Hfp.h"
#include "VR_DataProviderComm.h"
#include "VR_DP_OB_Action_Music.h"
#include "VR_DP_OB_Action_Radio.h"
#include "VR_DP_OB_Action_CityList.h"
#include "VR_DP_OB_Action_HfpStatus.h"
#include "VR_DP_OB_Action_TslStatus.h"
#include "VR_DP_OB_Action_TslAppList.h"
#include "VR_DP_OB_Action_AudioSource.h"
#include "MediaProvider/NMMediaUri.h"
#include "MediaInfoProvider/NMMediaInfoUri.h"
#include "navi/Traffic/WeatherCityListRes.pb.h"

using namespace std;
using namespace nutshell;
using namespace nutshell::media;

namespace navi
{
namespace dataprovider
{
tDPListenerURI_Map  VR_DP_OBListener::m_DPUri_Map = boost::assign::map_list_of
    (NCString(VR_SYSTEM_INFO_BT_HFP_MAIN_URI),                           URI_HFPSTATUS)
    (NCString(VR_SYSTEM_INFO_BT_HFP_CURDEV_URI),                         URI_HFPSTATUS)
    (NCString(VR_SYSTEM_INFO_BT_RSS_URI),                                URI_HFPSTATUS)
    (NCString(VR_PHONE_BOOK_CONTACT_DATA_URI),                           URI_HFP)
    (NCString(VR_PHONE_BOOK_CONTACT_DATA_DICT_UPDATE_URI),               URI_HFP)
    (NCString(VR_MUSIC_USB_SCANN_FINISHED),                              URI_MUSIC)
    (NMMediaInfoUri::getKeyContentUri(MediaInfoKey_iPod1DBStatus),       URI_MUSIC)
    (NMMediaInfoUri::getKeyContentUri(MediaInfoKey_AllSrcStatus),        URI_AUDIO)
    (NMMediaInfoUri::getKeyContentUri(MediaInfoKey_UsbConnectState),     URI_AUDIO)
    (NCString(VR_SYSTEM_INFO_BT_AVP_URI),                                URI_AUDIO)
    (NMMediaInfoUri::getKeyContentUri(MediaInfoKey_CurSrc),              URI_MUSICRADIO);

VR_DP_OBListener::VR_DP_OBListener(const NCString &name,
                                   const NCRunnableLooper &looper,
                                   spVR_DP_ThreadWorking spWorkingThread)
    : NCContentListener(name, looper)
    , m_spWorkingThread(spWorkingThread)
{
    VR_LOGD_FUNC();
}

VR_DP_OBListener::~VR_DP_OBListener()
{
    VR_LOGD_FUNC();
}

void VR_DP_OBListener::Initialize()
{
    VR_LOGD_FUNC();

    // 1.send initialize action
    NCUri uri = NCString(VR_DATAPROVIDER_INFO_URI);
    NCVariant variant(0);

    // music action-observer
    spVR_DP_OB_Action spMusicAction = spVR_DP_OB_Action_Music(VR_new VR_DP_OB_Action_Music(
                                                             uri, variant));
    m_spWorkingThread->onUpdataPreare(spMusicAction);

    // AudioSource observer
    spVR_DP_OB_Action spAudioSAction = spVR_DP_OB_Action_AudioSource(VR_new VR_DP_OB_Action_AudioSource(
                                                                   uri, variant));
    m_spWorkingThread->onUpdataPreare(spAudioSAction);

    // Hfp observer
    spVR_DP_OB_Action spHFPAction = spVR_DP_OB_Action_Hfp(VR_new VR_DP_OB_Action_Hfp(
                                                           uri, variant));
    m_spWorkingThread->onUpdataPreare(spHFPAction);

    // HfpStatus observer
    spVR_DP_OB_Action spHfpStatusAction = spVR_DP_OB_Action_HfpStatus(VR_new VR_DP_OB_Action_HfpStatus(
                                                                 uri, variant));
    m_spWorkingThread->onUpdataPreare(spHfpStatusAction);
}

void VR_DP_OBListener::Regist()
{
    VoiceList<NCUri>::type list;
    tDPListenerURI_Map::const_iterator iter;
    for (iter = m_DPUri_Map.begin(); iter != m_DPUri_Map.end(); ++iter) {
        NCString cUrl = iter->first;
        VR_LOGD("Regist url: %s", cUrl.getString());
        list.push_back(cUrl);
    }

    bool result = listenUri(list);
    if (!result) {
        VR_LOGD("VR_DP_OBListener unlistenUri failed");
    }
}

void VR_DP_OBListener::UnRegist()
{
    VoiceList<NCUri>::type list;

    tDPListenerURI_Map::const_iterator iter;
    for (iter = m_DPUri_Map.begin(); iter != m_DPUri_Map.end(); ++iter) {
        NCString cUrl = iter->first;
        VR_LOGD("UnRegist url: %s", cUrl.getString());
        list.push_back(cUrl);
    }

    bool result = unlistenUri(list);
    if (!result) {
        VR_LOGD("VR_DP_OBListener unlistenUri failed");
    }
}

void VR_DP_OBListener::AddOneUri(tDPListenerURI_Pair &pair)
{
    m_DPUri_Map.insert(pair);
}

void VR_DP_OBListener::onNotifyUpdate(NCUri &uri, NCVariant &variant)
{
    VR_LOGD("URI->[%s], VARIANT->[%s]", uri.toString().getString(), variant.toString().c_str());
    NCString uriNCStr = uri.toString();
    tDPListenerURI_Map::const_iterator iter;
    iter = m_DPUri_Map.find(uriNCStr);
    if (iter == m_DPUri_Map.end()) {
        return;
    }

    DPListener_type uriType = iter->second;
    switch (uriType) {
    case URI_MUSIC:
    {
        spVR_DP_OB_Action spMusicAction = spVR_DP_OB_Action_Music(VR_new VR_DP_OB_Action_Music(
                                                                 uri, variant));
        m_spWorkingThread->onUpdataPreare(spMusicAction);
        break;
    }
    case URI_AUDIO:
    {
        spVR_DP_OB_Action spAudioSAction = spVR_DP_OB_Action_AudioSource(VR_new VR_DP_OB_Action_AudioSource(
                                                                       uri, variant));
        m_spWorkingThread->onUpdataPreare(spAudioSAction);
        break;
    }
    case URI_HFP:
    {
        spVR_DP_OB_Action spHFPAction = spVR_DP_OB_Action_Hfp(VR_new VR_DP_OB_Action_Hfp(
                                                               uri, variant));
        m_spWorkingThread->onUpdataPreare(spHFPAction);
        break;
    }
    case URI_HFPSTATUS:
    {
        spVR_DP_OB_Action spHfpStatusAction = spVR_DP_OB_Action_HfpStatus(VR_new VR_DP_OB_Action_HfpStatus(
                                                                     uri, variant));
        m_spWorkingThread->onUpdataPreare(spHfpStatusAction);
        break;
    }
    case URI_MUSICRADIO:
    {
        spVR_DP_OB_Action spRadioAction = spVR_DP_OB_Action_Radio(VR_new VR_DP_OB_Action_Radio(
                                                                 uri, variant));
        m_spWorkingThread->onUpdataPreare(spRadioAction);

        break;
    }
    default:
    {
        VR_LOGD("UnRegist URL %s ", uriNCStr.getString());
        break;
    }
    }
}

}
}
/* EOF */
