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
 * @file  VR_SpotlightSearchTask.cpp
 * @brief Declaration file of class VR_SpotlightSearchTask and Derived classes.
 *
 * This file includes the declaration of class VR_SpotlightSearchTask and Derived classes.
 *
 * @attention used for C++ only.
 */

#include   "stdafx.h"
#include   "sstream"
#include   "VR_ConfigureIF.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include   "EV_EventSender.h"
#include   "boost/format.hpp"
#include   "VR_EventSenderIF.h"
#include   "VR_SpotlightSearchTask.h"
#include   "VR_CommonTool.h"
#include   "VR_ContentProvider.h"
#include   "VR_NIMusicShareProxy.h"

#include   "navi/Search/ReqSpotlightSearch.pb.h"
#include   "navi/Search/SpotlightSearchResult.pb.h"
#include   "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include   "navi/Voice/VoiceRecog/RespVrCommonPropertyResult.pb.h"

#include   "navi/Search/ReqSearchModuleStatus.pb.h"
#include   "navi/Search/SearchModuleStatus.pb.h"
#include   "navi/Search/ReqCancel.pb.h"

#include   "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using namespace  navi::Search;
using namespace  navi::VoiceRecog;

VR_SpotlightSearchTask::VR_SpotlightSearchTask(spVR_NIMusicShareProxy proxy)
    : m_bUpdate(false)
    , m_requestId(-1)
    , m_bMusicFinish(false)
    , m_bPhoneFinish(false)
    , m_bPoiFinish(false)
    , m_strMusicItems("")
    , m_strPhoneItems("")
    , m_strPoiItems("")
    , m_spContext()
    , m_spShareProxy(proxy)
{
    VR_LOGD_FUNC();
}


void
VR_SpotlightSearchTask::OnRequestCommonProperty(const EV_EventContext& context, const VR_ProtoMessageBase& msg)
{
    VR_LOGD_FUNC();

    if (m_spContext.get()) {
        m_spContext.reset();
    }

    m_strMusicItems = "";
    m_strPhoneItems = "";
    m_strPoiItems = "";

    m_spContext = VR_EventSenderIF::CopyEventContext(&context);

    const  VrReqKeyWordsSearchInfo&  searchInfo = (const VrReqKeyWordsSearchInfo&)msg;
    int  nRequestID = searchInfo.requestid();
    m_requestId = nRequestID;
    std::string  strKeyWord = searchInfo.searchinfo();
    nutshell::INT32  nLon(0);
    nutshell::INT32  nLat(0);
    nutshell::INT32  nRoadKind(0);
    bool  bDemonOn(false);
    VR_ConfigureIF::Instance()->getLocateInfo(nRoadKind, nLon, nLat, bDemonOn);

    VR_LOGD("search keyword = [%s], RequestID = [%d], nLon = [%d], nLat = [%d]",
            strKeyWord.c_str(), nRequestID, nLon, nLat);



    std::unique_ptr<ReqSpotlightSearch>   ReqSearchMsg(VR_new   ReqSpotlightSearch);
    bool bUsbConnect = isUsbConnected();
    if (bUsbConnect) {
        ReqSearchMsg->add_source_list(SearchDefine_SPOTLIGHT_SOURCE_TYPE_MUSIC);
        m_bMusicFinish = false;
    }
    else {
        m_bMusicFinish = true;
    }

    bool bPhoneConnected = isPhoneConnected();
    if (bPhoneConnected) {
        ReqSearchMsg->add_source_list(SearchDefine_SPOTLIGHT_SOURCE_TYPE_PHONE);
        m_bPhoneFinish = false;
    }
    else {
        m_bPhoneFinish = true;
    }

    m_bPoiFinish = false;
    ReqSearchMsg->add_source_list(SearchDefine_SPOTLIGHT_SOURCE_TYPE_POI);
    ReqSearchMsg->set_first_source(SearchDefine_SPOTLIGHT_SOURCE_TYPE_POI);

    VR_LOGD("phone connect: [%d], usb connect: [%d]", bPhoneConnected, bUsbConnect);

    ReqSearchMsg->set_keyword(strKeyWord);
    ReqSearchMsg->set_reqid(m_requestId);
    ReqSearchMsg->mutable_map_lonlat()->set_lon(nLon);
    ReqSearchMsg->mutable_map_lonlat()->set_lat(nLat);

    EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQSPOTLIGHTSEARCH, ReqSearchMsg.release());
}


 void   VR_SpotlightSearchTask::OnRequestCancelSearchResult(const EV_EventContext& context, const VR_ProtoMessageBase& msg)
 {
     VR_LOGD_FUNC();

     EV_EventContext  tempContext = context;
     tempContext.GetStringData();

     const  VrReqCancelSearchFor&  cancelSearch = (const VrReqCancelSearchFor&)msg;
     int  nRequestID = cancelSearch.requestid();
     VR_LOGD("user request cancle id : [%d], current request id: [%d]", nRequestID, m_requestId);

     if (nRequestID == m_requestId) {
         ReqCancel* reqCancel = new  ReqCancel;
         reqCancel->set_reqid(m_requestId);
         reqCancel->set_sender(ReqCancel_SENDER_TYPE_VR);
         EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQCANCEL, reqCancel);
         m_requestId = -1;
     }
 }

bool   VR_SpotlightSearchTask::isPhoneConnected()  const
{
    VR_LOGD_FUNC();
//    VR_ContentProvider  vr_cp;
//    std::string   strValue = vr_cp.GetContentProviderValue(vr_phone_connect_url);
//    VR_LOGD("strvalue : [%s]", strValue.c_str());
//    if ("connected" == strValue) {
//        return true;
//    }
//    return false;

    std::string  strPhoneConnect("disconnected");
    VR_ConfigureIF::Instance()->getStateValueByStateKey(VR_PHONE_STATE_CONNECTED, strPhoneConnect);
    VR_LOGD("strPhoneConnect:[%s]", strPhoneConnect.c_str());
    if ("connected" == strPhoneConnect) {
        return true;
    }
    return false;
}

bool   VR_SpotlightSearchTask::isUsbConnected() const
{
#ifdef HAVE_NUTSHELL_OS
    bool bConnect = m_spShareProxy->IsUsbConnected();
    VR_LOGD("proxy usb connect:[%d]", bConnect);
    return bConnect;
#else
    return true;
#endif
}


void
VR_SpotlightSearchTask::OnMessageSearchStatus(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    if (m_bUpdate) {
        return;
    }
    const SearchModuleStatus& moduleStatus = (const SearchModuleStatus&)msg;
    SearchModuleStatus_SEARCH_MODULE_STATUS eStatus = moduleStatus.module_status();
    SearchDefine_SEARCH_PROCESS process = moduleStatus.process();
    VR_LOGD("search status [%d], process = [%d]", eStatus, process);
    if (SearchDefine_SEARCH_PROCESS_IVI == process && SearchModuleStatus_SEARCH_MODULE_STATUS_OK == eStatus) {
        m_bUpdate = true;
   //     m_spShareProxy->SyncCacheData();
        m_spShareProxy->Initialize();
    }
}

void
VR_SpotlightSearchTask::ReqModuleStatus()
{
    VR_LOGD_FUNC();
    std::unique_ptr<ReqSearchModuleStatus> reqModuleStatus(VR_new ReqSearchModuleStatus);
    reqModuleStatus->set_process(SearchDefine_SEARCH_PROCESS_IVI);
    EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQSEARCHMODULESTATUS, reqModuleStatus.release());
}

void
VR_SpotlightSearchTask::ResponceSearchInfoResult(const std::string &value)
{
    std::unique_ptr<RespVrCommonPropertyResult>  replyMeg(RespVrCommonPropertyResult::default_instance().New());
    VrReqKeyWordsSearchInfoResult* searchResult = replyMeg->mutable_reqkeywordssearchinforesult();
    searchResult->set_searchinforesult(value);

    if (m_spContext.get()) {
        EV_EventSender().Reply(*m_spContext,  replyMeg.release());
    }
}

void
VR_SpotlightSearchTask::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    std::string strErrorCode = "0";
    std::string strCount = "-1";
    std::string strResult = "";
    std::string strPoiItemXml = "";
    const  SpotlightSearchResult&  respSearchResult = (const SpotlightSearchResult&)msg;

    int nRequestID = respSearchResult.reqid();
    VR_LOGD("user requestid = [%d], search responceID = [%d]", m_requestId, nRequestID);

    if (m_requestId != nRequestID) {
        VR_LOGD("Search Result ReqID != User request ID");
        return;
    }
    else if (SpotlightSearchResult_Spotlight_Error_Type_NO_ERROR != respSearchResult.errortype()) {
        VR_LOGD("Search Result error type is error.");
        return;
    }
    else {
        int nCount(0);
        int nSourceListSize = respSearchResult.data_srouce_list_size();
        SearchDefine_SPOTLIGHT_SOURCE_TYPE sourceType;
        for (int index = 0; index < nSourceListSize; ++index) {
             sourceType = respSearchResult.data_srouce_list(index);

            if (SearchDefine_SPOTLIGHT_SOURCE_TYPE_MUSIC == sourceType) {
                 int nMusicSize = respSearchResult.music_data_size();
                 nCount += nMusicSize;
                 for (int index = 0; index < nMusicSize; ++index) {
                     const SearchDefine_Source_Music_Data&  item = respSearchResult.music_data(index);
                     std::string strSongId("");
                     toString(strSongId, item.song_id());
                     std::string strSongName = item.song_name();
                     strSongName = VR_CommonTool::Encode(strSongName);
                     std::string strAlbumId("");
                     toString(strAlbumId, item.album_id());
                     std::string strAlbumName = item.album_name();
                     strAlbumName = VR_CommonTool::Encode(strAlbumName);
                     std::string strArtistId("");
                     toString(strArtistId, item.artist_id());
                     std::string strArtistName = item.artist_name();
                     strArtistName = VR_CommonTool::Encode(strArtistName);
                     std::string strUri = item.playlist_uri();
                     strUri = VR_CommonTool::Encode(strUri);
                     std::string strPlayListUri = item.device_uri();
                     strPlayListUri = VR_CommonTool::Encode(strPlayListUri);
                     size_t nStartPos = strUri.find_last_of("/") + 1;
                     size_t nLastPos =  strUri.find_last_of("?");
                     std::string strFileName = strUri.substr(nStartPos, nLastPos - nStartPos);
                     VR_LOGD("songname = [%s], uri = [%s], playlisturi = [%s], strFileName = [%s]",
                             strSongName.c_str(), strUri.c_str(), strPlayListUri.c_str(), strFileName.c_str());

                     std::string strMusicItemXml = (boost::format(MusicItemXml)
                                                    % strSongId
                                                    % strSongName
                                                    % strAlbumId
                                                    % strAlbumName
                                                    % strArtistId
                                                    % strFileName
                                                    % strUri
                                                    % strPlayListUri).str();

                     m_strMusicItems.append(strMusicItemXml);
                 }
            }
            else if (SearchDefine_SPOTLIGHT_SOURCE_TYPE_POI == sourceType) {
                int nPoiSize = respSearchResult.poi_data_size();
                nCount += nPoiSize;
                for (int index = 0; index < nPoiSize; ++index) {
                    const SpotlightSearchResult_Source_POI_Data&  data = respSearchResult.poi_data(index);
                    std::string strPoiType = data.poitype();
                    strPoiType = VR_CommonTool::Encode(strPoiType);
                    std::string strPoiId("");
                    toString(strPoiId, data.poiid());
                    std::string strPoiName = data.name();
                    strPoiName = VR_CommonTool::Encode(strPoiName);
                    std::string strPosLat("");
                    std::string strPosLon("");
                    const SearchDefine_MapLonLat& mapLonLat = data.pos();
                    toString(strPosLon, mapLonLat.lon());
                    toString(strPosLat, mapLonLat.lat());
                    std::string strTel = data.tel();
                    strTel = VR_CommonTool::Encode(strTel);
                    std::string strAddress = data.address();
                    strAddress = VR_CommonTool::Encode(strAddress);
                    std::string strLinkId("");
                    toString(strLinkId, data.linkid());
                    std::string strDistance("");
                    toString(strDistance, data.distance());
                    std::string strPoiDataXml = (boost::format(PoiItemXml)
                                                 % strPoiId
                                                 % strPoiName
                                                 % strPosLon
                                                 % strPosLat
                                                 % strPosLon
                                                 % strPosLat
                                                 % strTel
                                                 % strAddress
                                                 % strLinkId
                                                 % strDistance).str();

                   m_strPoiItems.append(strPoiDataXml);
                }
            }
            else if (SearchDefine_SPOTLIGHT_SOURCE_TYPE_PHONE == sourceType) {
                int nPhoneSize = respSearchResult.phone_data_size();
                nCount += nPhoneSize;
                for (int index = 0; index < nPhoneSize; ++index) {
                    const SearchDefine_Source_PHONE_DATA& data = respSearchResult.phone_data(index);
                    std::string strContactid("");
                    toString(strContactid, data.contact_id());
                    std::string strFirstName = data.first_name();
                    strFirstName = VR_CommonTool::Encode(strFirstName);
                    std::string strLastName = data.last_name();
                    strLastName = VR_CommonTool::Encode(strLastName);
                    int phoneNum = data.phone_list_size();
                    std::string listType[] = { "", "", "", "" };
                    std::string listNumber[] = { "", "", "", "" };
                    for (int n = 0; n < phoneNum; ++n) {
                        const SearchDefine_Source_PHONE_LIST& list = data.phone_list(n);
                        listType[n] = list.phone_type();
                        listNumber[n] = list.phone_number();
                        if (n >= 3) {
                            break;
                        }
                    }
                    std::string strPhoneDataXml = (boost::format(PhoneItemXml)
                                                   % strContactid
                                                   % strFirstName
                                                   % strLastName
                                                   % listType[0]
                                                   % listNumber[0]
                                                   % listType[1]
                                                   % listNumber[1]
                                                   % listType[2]
                                                   % listNumber[2]
                                                   % listType[3]
                                                   % listNumber[3]
                                                  ).str();
                    m_strPhoneItems.append(strPhoneDataXml);
                }
            }
        }

        SpotlightSearchResult_Spotlight_Finish_Type  finishType = respSearchResult.finish_type();

        VR_LOGD("Search result SourceListSize: [%d], SourceType: [%d], FinishType: [%d], nCount = [%d]",
                                                                     nSourceListSize, sourceType, finishType, nCount);

        if (SpotlightSearchResult_Spotlight_Finish_Type_PART == finishType
            &&  SearchDefine_SPOTLIGHT_SOURCE_TYPE_PHONE == sourceType) {
            strPoiItemXml.append(m_strPhoneItems);
            strResult = (boost::format(NaviPoiSearchXml) % strErrorCode % strCount % strPoiItemXml).str();
            ResponceSearchInfoResult(strResult);
            return;
        }
        else if (SpotlightSearchResult_Spotlight_Finish_Type_ALL == finishType
            && SearchDefine_SPOTLIGHT_SOURCE_TYPE_PHONE == sourceType) {
            m_bPhoneFinish = true;
        }
        else if (SpotlightSearchResult_Spotlight_Finish_Type_ALL == finishType
            && SearchDefine_SPOTLIGHT_SOURCE_TYPE_MUSIC == sourceType) {
            m_bMusicFinish = true;
        }
        else if (SearchDefine_SPOTLIGHT_SOURCE_TYPE_POI == sourceType) {
            m_bPoiFinish = true;
        }


        if (m_bPhoneFinish && m_bMusicFinish) {
            if (m_strPhoneItems.empty() && m_strMusicItems.empty()) {
                if (!m_bPoiFinish) {
                    return;
                }
            }

            strPoiItemXml.append(m_strPhoneItems);
            strPoiItemXml.append(m_strMusicItems);
            strPoiItemXml.append(m_strPoiItems);
            strResult = (boost::format(NaviPoiSearchXml) % strErrorCode % strCount % strPoiItemXml).str();
            ResponceSearchInfoResult(strResult);
            return;
        }
        else if (m_bPhoneFinish) {
            if (m_strPhoneItems.empty() && m_strMusicItems.empty()) {
                return;
            }
            strPoiItemXml.append(m_strPhoneItems);
            strPoiItemXml.append(m_strMusicItems);
            strResult = (boost::format(NaviPoiSearchXml) % strErrorCode % strCount % strPoiItemXml).str();
            ResponceSearchInfoResult(strResult);
            return;
        }
    }
}

/* EOF */
