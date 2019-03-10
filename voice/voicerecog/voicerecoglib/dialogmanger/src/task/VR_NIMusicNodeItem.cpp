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
* @file  VR_NIMusicNodeItem.cpp
* @brief Declaration file of class VR_NIMusicNodeItem.
*
* This file includes the declaration of class VR_NIMusicNodeItem.
*
* @attention used for C++ only.
*/

#include "stdafx.h"
#include "VR_CommonTool.h"
#include "VR_NIMusicNodeItem.h"
#include "VR_NIMusicShareProxy.h"
#include "navi/Search/ReqUpdateSpotlightData.pb.h"

using namespace nutshell;
using namespace nutshell::media;
using namespace navi::Search;

const  int  nSafeCount = 50;

VR_NIMusicNodeItem::VR_NIMusicNodeItem(const std::string& strUri, VR_NIMusicShareProxy* proxy)
    : m_strUri(strUri)
    , m_nStartIndex(0)
    , m_nCurrentIndex(0)
    , m_bUsbConnected(false)
    , m_fileList()
    , m_shareProxy(proxy)
{
    VR_LOGD("uri:[%s]", strUri.c_str());
}


VR_NIMusicNodeItem::~VR_NIMusicNodeItem()
{
    VR_LOGD_FUNC();
    m_fileList.clear();
}


void   VR_NIMusicNodeItem::Add()
{
    VR_LOGD_FUNC();
    m_shareProxy->GetCapability(m_strUri);
}

void   VR_NIMusicNodeItem::Remove()
{
    VR_LOGD_FUNC();
    std::unique_ptr<ReqUpdateSpotlightData>  fullUpdate(ReqUpdateSpotlightData::default_instance().New());
    fullUpdate->set_data_type(SearchDefine_SPOTLIGHT_SOURCE_TYPE_MUSIC);
    fullUpdate->set_update_type(ReqUpdateSpotlightData_Source_Update_Type_FULLUPDATE);
    fullUpdate->set_device_uri(m_strUri + "audio/all/");
    EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQUPDATESPOTLIGHTDATA, fullUpdate.release());
}

void   VR_NIMusicNodeItem::SetConnected(bool bConnected)
{
    m_bUsbConnected = bConnected;
    if (m_bUsbConnected) {
        std::string strUri = m_strUri + "audio/all/";
        m_shareProxy->GetFileListCount(strUri);
    }
    else {
        Remove();
        m_nStartIndex = 0;
        m_nCurrentIndex = 0;
        m_fileList.clear();
    }
}


void   VR_NIMusicNodeItem::SetFileList(const std::vector<NIShareFileDisplay> &fileList)
{
    m_fileList = fileList;
    VR_LOGD("Uri:[%s], fileCount:[%d]", m_strUri.c_str(), m_fileList.size());

    std::unique_ptr<ReqUpdateSpotlightData>  spotLightData(ReqUpdateSpotlightData::default_instance().New());
    spotLightData->set_data_type(SearchDefine_SPOTLIGHT_SOURCE_TYPE_MUSIC);
    spotLightData->set_update_type(ReqUpdateSpotlightData_Source_Update_Type_ADD);
    for (NIShareFileDisplay elem : m_fileList) {
        int  nSongId = elem.index;
        std::string strUri = elem.uri;
        std::string strTitle = elem.title;
        std::string strPlayUri = m_strUri + "audio/all/";
        size_t nStartPos = strUri.find_last_of("/") + 1;
        size_t nLastPos =  strUri.find_last_of("?");
        std::string strFileName = strUri.substr(nStartPos, nLastPos - nStartPos);
        VR_LOGD("songid:[%d], struri:[%s],strtitle:[%s], strFileName:[%s]", nSongId,
                                                                            strUri.c_str(),
                                                                            strTitle.c_str(),
                                                                            strFileName.c_str()
               );

        SearchDefine_Source_Music_Data* data = spotLightData->add_music_data();
        data->set_song_id(nSongId);
        data->set_song_name(strTitle);
        data->set_album_name("");
        data->set_artist_name(strFileName);
        data->set_device_uri(strPlayUri);
        data->set_playlist_uri(strUri);
    }
    EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQUPDATESPOTLIGHTDATA, spotLightData.release());
}


void  VR_NIMusicNodeItem::SetFileList() const
{
    VR_LOGD("m_filelist count = [%d]", m_fileList.size());

    std::unique_ptr<ReqUpdateSpotlightData>  spotLightData(ReqUpdateSpotlightData::default_instance().New());
    spotLightData->set_data_type(SearchDefine_SPOTLIGHT_SOURCE_TYPE_MUSIC);
    spotLightData->set_update_type(ReqUpdateSpotlightData_Source_Update_Type_ADD);
    for (NIShareFileDisplay elem : m_fileList) {
        int  nSongId = elem.index;
        std::string strUri = elem.uri;
        std::string strTitle = elem.title;
        std::string strPlayUri = m_strUri + "audio/all/";
        size_t nStartPos = strUri.find_last_of("/") + 1;
        size_t nLastPos =  strUri.find_last_of("?");
        std::string strFileName = strUri.substr(nStartPos, nLastPos - nStartPos);
        VR_LOGD("songid:[%d], struri:[%s],strtitle:[%s], strFileName:[%s]", nSongId,
                                                                            strUri.c_str(),
                                                                            strTitle.c_str(),
                                                                            strFileName.c_str()
               );

        SearchDefine_Source_Music_Data* data = spotLightData->add_music_data();
        data->set_song_id(nSongId);
        data->set_song_name(strTitle);
        data->set_album_name("");
        data->set_artist_name(strFileName);
        data->set_device_uri(strPlayUri);
        data->set_playlist_uri(strUri);
    }
    EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQUPDATESPOTLIGHTDATA, spotLightData.release());
}


void   VR_NIMusicNodeItem::SetFileListCount(int nCount)
{
    VR_LOGD("nCount:[%d]", nCount);
    if (nCount > 0 && nCount > m_nCurrentIndex) {
        m_nStartIndex = m_nCurrentIndex;
        m_nCurrentIndex = nCount;
 //       GetFileList();
        GetFileList(m_nStartIndex, m_nCurrentIndex);
    }
}


void   VR_NIMusicNodeItem::GetFileList()
{
    VR_LOGD_FUNC();
    std::string strStart;
    toString(strStart, m_nStartIndex);
    std::string strCount;
    toString(strCount, m_nCurrentIndex - m_nStartIndex);
    std::string strUri = m_strUri + "audio/all/?fileIndex=" + strStart + "&fileCount=" + strCount;
    VR_LOGD("strUri:[%s]", strUri.c_str());
    m_shareProxy->GetFileList(strUri);
}

void  VR_NIMusicNodeItem::GetFileList(int startIndex, int endIndex)
{
    VR_LOGD("GetFileList startIndex:[%d], endIndex:[%d]", startIndex, endIndex);

    int  i = startIndex;
    int  j = 0;

    while (true) {
        j = i + nSafeCount;
        if (j >= endIndex) {
            break;
        }
        GetList(i, nSafeCount);

        i += nSafeCount;
    }
    VR_LOGD("GetFileList j:[%d]", j);
    GetList(i, (endIndex - i));
}

void  VR_NIMusicNodeItem::GetList(int startIndex, int nCount)
{
    VR_LOGD_FUNC();

    std::string  strStart("");
    toString(strStart, startIndex);
    std::string  strCount("");
    toString(strCount, nCount);

    std::string strUri = m_strUri + "audio/all/?fileIndex=" + strStart + "&fileCount=" + strCount;
    VR_LOGD("GetList strurl:[%s]", strUri.c_str());
    m_shareProxy->GetFileList(strUri);
}


std::string   VR_NIMusicNodeItem::GetUri()  const
{
 //   VR_LOGD_FUNC();
    return  m_strUri;
}

bool   VR_NIMusicNodeItem::GetUsbConnect()  const
{
    return  m_bUsbConnected;
}
/* EOF */
