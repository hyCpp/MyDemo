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
#include "stdafx.h"
#include "VR_AudioPlayAndShow.h"
#include <boost/assign/list_of.hpp>
#include <boost/format.hpp>
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"
#include "VR_ActionInnerParameterDef.h"
#include "VR_ActionSenderDef.h"
#include "VR_ActionEventSender.h"
#include "VR_AudioSourceListener.h"
#include "EV_Define.h"
using namespace navi::VoiceRecog;
using namespace navi::dataprovider;

std::string strPlayAllXml = "<action agent=\"media\" op = \"%1%\">"
                              "<sourceid>%2%</sourceid>"
                            "</action>";

std::string strPlayAlbumOrArtistXml = "<action agent=\"media\" op = \"%1%\">"
                                        "<sourceid>%2%</sourceid>"
                                        "<%3%>%4%</%3%>"
                                        "</action>";

std::string strPlayAllAlbumXml = "<action agent=\"media\" op = \"%1%\">"
                                    "<sourceid>%2%</sourceid>"
                                    "<genreid>%3%</genreid>"
                                    "<artistid>%4%</artistid>"
                                   "</action>";

std::string strShowOnePara = "<action agent=\"media\" op = \"%1%\">"
                                "<sourceid>%2%</sourceid>"
                             "</action>";

std::string strShowTwoPara = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                              "<sourceid>%4%</sourceid>"
                            "</action>";

std::string strShowThreePara = "<action agent=\"media\" op = \"%1%\">"
                                "<genreid>%2%</genreid>"
                                "<artistid>%3%</artistid>"
                                "<sourceid>%4%</sourceid>"
                               "</action>";


VR_AudioPlayAndShow::VR_AudioPlayAndShow(int id, int type, std::string msg, spVR_ActionEventSender sender)
    : VR_ActionBase(sender)
    , m_EngineReqId(id)
    , m_ActionType(type)
    , m_ReqMsg(msg)
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
}

VR_AudioPlayAndShow::~VR_AudioPlayAndShow()
{

}

void VR_AudioPlayAndShow::Run()
{
    RunAction();
    this->SetActionComplete();
}

void VR_AudioPlayAndShow::OnReceive(const VR_ProtoMessageBase &proto)
{

}

void VR_AudioPlayAndShow::RunAction()
{
    VR_LOGI("id=[%d],msg=[%s]", m_EngineReqId, m_ReqMsg.c_str());
    pugi::xml_document doc;
    if (doc.load(m_ReqMsg.c_str())) {
        std::string op = doc.root().first_child().attribute("op").value();
        if (!op.compare("playByFilter")) {
            EuropeSpecialPlay(doc);
        }
        else if (!op.compare("showMediaScreen")) {
            EuropeSpecialShow(doc);
        }
        else {
            VR_LOGE("Action Op[%s] IS Abnormal", op.c_str());
        }
    }
    else {
        VR_LOGE("Action XML  Is Abnormal ");
    }
}

void VR_AudioPlayAndShow::EuropeSpecialPlay(pugi_vr::xml_document &doc)
{
    VR_UNUSED_VAR(doc);
}

void VR_AudioPlayAndShow::EuropeSpecialShow(pugi_vr::xml_document &doc)
{
    VR_UNUSED_VAR(doc);
}

std::string VR_AudioPlayAndShow::ShowAllArtist(std::string &strGenre, std::string &strSourceID)
{
    std::string resultXml;
    if (!strGenre.compare("none")) {
        resultXml = (boost::format(strShowOnePara) % VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_ARTIST % strSourceID
                       ).str();
    }
    else {
        resultXml = (boost::format(strShowTwoPara) % VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_ARTIST_BY_GENRE
                     % VR_VOICE_ACTION_TAG_KEY_GENRE_ID % strGenre % strSourceID).str();
    }
    return resultXml;
}

std::string VR_AudioPlayAndShow::ShowAllAlbum(std::string &strGenre, std::string &strArtist, std::string &strSourceID)
{
    std::string resultXml;
    if (!strGenre.compare("none")) {
        if (!strArtist.compare("none")) {
            resultXml = (boost::format(strShowOnePara) % VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_ALBUM % strSourceID
                           ).str();
        }
        else {
            resultXml = (boost::format(strShowTwoPara) % VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_ALBUM_BY_ARTIST
                         % VR_VOICE_ACTION_TAG_KEY_ARTIST_ID % strArtist % strSourceID).str();
        }
    }
    else {
        if (strArtist.compare("none")) {
            resultXml = (boost::format(strShowThreePara) % VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_ALBUM_BY_GENRE_ARTIST
                           % strGenre % strArtist % strSourceID).str();
        }
    }
    return resultXml;
}
/* EOF */
