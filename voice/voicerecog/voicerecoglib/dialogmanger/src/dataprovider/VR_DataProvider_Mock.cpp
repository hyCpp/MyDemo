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
#include "VR_DataProvider.h"
#include <boost/format.hpp>
#include <boost/make_shared.hpp>
#include "VR_FileTransport.h"
#include "VR_DP_OB_Action_CityList.h"
#include "VR_DialogEngineIF.h"
#include "VR_DataProviderComm.h"
#include "VR_DP_OB_Action_AudioSource.h"
#include "VR_DP_OB_Action_Radio.h"
#include "VR_DP_OB_Action_Music.h"
#include "VR_DP_ListData_Access.h"
#include "VR_DP_OB_Action_Hfp.h"
#include "VR_DP_OB_Action_HfpStatus.h"
#include "MediaProvider/NMMediaUri.h"
#include "MediaInfoProvider/NMMediaInfoUri.h"
#include "ncore/NCFile.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_ConfigureIF.h"
#include "VR_RunableThread.h"
#include "BL_AplInfo.h"
#include "VR_CityListFileBuilder.h"

#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyRecogResult.pb.h"
#include "navi/Traffic/WeatherCityListRes.pb.h"
#include "navi/Traffic/WeatherCityListUpdate.pb.h"
#include "navi/Traffic/WeatherCityListReq.pb.h"

#include "VR_DP_OB_Action_CityList.h"


using namespace navi::VoiceRecog;
using namespace nutshell::media;
using namespace nutshell;
namespace navi
{
namespace dataprovider
{
#define VR_FILE_REQUEST_RETRY_TIME 500 // Time unit : millisecond
#define VR_DCU_DOCUNENT_ROOT_PATH "http://192.168.128.129:49153/Internal/VR/"
VR_DataProvider::VR_DataProvider()
{
    VR_LOGD_FUNC();
}

VR_DataProvider::~VR_DataProvider()
{
    VR_LOGD_FUNC();
}

bool VR_DataProvider::Initialize()
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_DataProvider::RegistObserver()
{
    VR_LOGD_FUNC();
    return true;
}

void VR_DataProvider::UnRegistObserver()
{
    VR_LOGD_FUNC();
}

void VR_DataProvider::StartDPThread()
{
    VR_LOGD_FUNC();
}

void VR_DataProvider::StopDPThread()
{
    VR_LOGD_FUNC();
}

void VR_DataProvider::OnStartedNotify()
{
    VR_LOGD_FUNC();
}

bool VR_DataProvider::onReplyMusicDevStatus(std::string& id)
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_DataProvider::onReplyMusicGetLocalInfo(std::string& id)
{
    VR_LOGD_FUNC();
    return true;
}

}
}
/* EOF */
