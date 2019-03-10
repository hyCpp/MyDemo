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
* @file  VR_MusicSpotlightUpdater.cpp
* @brief Declaration file of class VR_MusicSpotlightUpdater.
*
* This file includes the declaration of class VR_MusicSpotlightUpdater.
*
* @attention used for C++ only.
*/

#include "stdafx.h"
#include "VR_EventSenderIF.h"
#include "EV_EventSender.h"
#include "VR_MusicSpotLightUpdater.h"

#include "navi/Search/ReqUpdateSpotlightData.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"

using namespace navi::VoiceRecog;
using namespace navi::Search;

VR_MusicSpotLightUpdater::VR_MusicSpotLightUpdater()
    : m_lFileIndex(0)
    , m_lCurrent(0)
    , m_lFileCount(0)
{
    VR_LOGD_FUNC();
}

VR_MusicSpotLightUpdater::~VR_MusicSpotLightUpdater()
{
    VR_LOGD_FUNC();
}

// receive music data from fc
void  VR_MusicSpotLightUpdater::OnRequestCommonProerty(const EV_EventContext& context, const VR_ProtoMessageBase& msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty& commonProperty = (const RequestVrCommonProperty&)msg;
    RequestVrCommonProperty_VrRequestCommonFunc funcCode = commonProperty.funccode();
    if (RequestVrCommonProperty_VrRequestCommonFunc_ReqFileListCount == funcCode) {
        const VrReqFileListCount& fileListCount = commonProperty.reqfilelistcount();
        OnFileListCountMessage(fileListCount);
    }
    else if (RequestVrCommonProperty_VrRequestCommonFunc_ReqFileListData == funcCode) {
        const VrReqFileListData& fileListData = commonProperty.reqfilelistdata();
        OnFileListDataMessage(fileListData);
    }
}

// file list count
void  VR_MusicSpotLightUpdater::OnFileListCountMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrReqFileListCount& fileListCount = (const VrReqFileListCount&)msg;
    NIMediaCinemoMediaType type = fileListCount.cinemomediatype();
    VR_LOGD("the media type: [%d]", type);
    if (NIMediaCinemoMediaType_Audio != type) {
        return;
    }

    if (0 == m_lFileIndex) {
        VR_LOGD("m_lFileIndex = [%lu]", m_lFileIndex);
        std::unique_ptr<ReqUpdateSpotlightData>  fullUpdate(ReqUpdateSpotlightData::default_instance().New());
        fullUpdate->set_data_type(SearchDefine_SPOTLIGHT_SOURCE_TYPE_MUSIC);
        fullUpdate->set_update_type(ReqUpdateSpotlightData_Source_Update_Type_FULLUPDATE);
        EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQUPDATESPOTLIGHTDATA, fullUpdate.release());
    }

    long lCurrent = fileListCount.count();
    VR_LOGD("the current file list count: [%lu]", lCurrent);

    if (lCurrent > 0 && m_lCurrent < lCurrent) {
        m_lFileIndex = m_lCurrent;
        m_lCurrent = lCurrent;
        RequestFileList();
    }
}

void  VR_MusicSpotLightUpdater::ClearUsbData()
{
    VR_LOGD_FUNC();

    m_lFileIndex = 0;
    m_lCurrent = 0;
    m_lFileCount = 0;
}

// file list data
void  VR_MusicSpotLightUpdater::OnFileListDataMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrReqFileListData& fileListData = (const VrReqFileListData&)msg;
    const NIMediaCinemoMediaInfoList& infoList = fileListData.listdata();
    int nListSize = infoList.mediainfo_size();
    VR_LOGD("the list data size: [%d]", nListSize);

    if (nListSize <= 0) {
        return;
    }


    std::unique_ptr<ReqUpdateSpotlightData>  spotLightData(ReqUpdateSpotlightData::default_instance().New());
    spotLightData->set_data_type(SearchDefine_SPOTLIGHT_SOURCE_TYPE_MUSIC);
    spotLightData->set_update_type(ReqUpdateSpotlightData_Source_Update_Type_ADD);
    for (int i = 0; i < nListSize; ++i) {
        const NIMediaCinemoMediaInfo& mediaInfo = infoList.mediainfo(i);
        NIMediaCinemoMediaType  type = mediaInfo.mediatype();
        if (NIMediaCinemoMediaType_Audio != type) {
            continue;
        }
        const NIMediaCinemoFileInfo& fileInfo = mediaInfo.fileinfo();
        INT trackid = fileInfo.trackid();
        std::string title = fileInfo.title();
        std::string album = fileInfo.album();
        std::string artist = fileInfo.artist();
        SearchDefine_Source_Music_Data* data = spotLightData->add_music_data();
        data->set_song_id(trackid);
        data->set_song_name(title);
        data->set_album_name(album);
        data->set_artist_name(artist);
        VR_LOGD("trackid = [%d], title = [%s]", trackid, title.c_str());
    }
    EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQUPDATESPOTLIGHTDATA, spotLightData.release());
}

// request list count
void  VR_MusicSpotLightUpdater::RequestFileListCount()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyCommonProperty> notifyProperty(VrNotifyCommonProperty::default_instance().New());
    notifyProperty->set_sender("dm");
    notifyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyFileListCount);
    notifyProperty->mutable_listcount();
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyProperty.release());
}

// request file list
void  VR_MusicSpotLightUpdater::RequestFileList()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyCommonProperty>  notifyProperty(VrNotifyCommonProperty::default_instance().New());
    notifyProperty->set_sender("dm");
    notifyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyFileListData);
    VrNotifyFileListData* listData = notifyProperty->mutable_listdata();
    listData->set_fileindex(m_lFileIndex);
    listData->set_filecount(m_lCurrent - m_lFileIndex);
    listData->set_cinemomediatype(NIMediaCinemoMediaType_Audio);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyProperty.release());
}

/* EOF */
