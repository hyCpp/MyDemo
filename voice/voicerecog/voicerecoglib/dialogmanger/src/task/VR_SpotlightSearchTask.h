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
 * @file VR_SpotlightSearchTask.h
 * @brief Declaration file of class VR_SpotlightSearchTask.
 *
 * This file includes the declaration of class VR_SpotlightSearchTask.
 *
 * @attention used for C++ only.
 */
#ifndef VR_SPOTLIGHTSEARCHTASK_H
#define VR_SPOTLIGHTSEARCHTASK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

class EV_EventContext;
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventContext);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_NIMusicShareProxy);

/**
 *  search xml define
 *
 *   music and poi
 */
const std::string  NaviPoiSearchXml =
        "<display agent=\"Navi\" content=\"SearchResult\" errcode=\"%1%\">"
            "<count>%2%</count>"
            "<items>"
                "%3%"
            "</items>"
        "</display>";


const std::string MusicItemXml =
        "<item>"
            "<type>music</type>"
            "<songid>%1%</songid>"
            "<songname>%2%</songname>"
            "<albumid>%3%</albumid>"
            "<albumname>%4%</albumname>"
            "<artistid>%5%</artistid>"
            "<artistname>%6%</artistname>"
            "<uri>%7%</uri>"
            "<playlisturi>%8%</playlisturi>"
            "<sourceid>3</sourceid>"
        "</item>";

const std::string PhoneItemXml =
        "<item>"
            "<type>phone</type>"
            "<contactid>%1%</contactid>"
            "<firstname>%2%</firstname>"
            "<lastname>%3%</lastname>"
            "<phonetype1>%4%</phonetype1>"
            "<phonenumber1>%5%</phonenumber1>"
            "<phonetype2>%6%</phonetype2>"
            "<phonenumber2>%7%</phonenumber2>"
            "<phonetype3>%8%</phonetype3>"
            "<phonenumber3>%9%</phonenumber3>"
            "<phonetype4>%10%</phonetype4>"
            "<phonenumber4>%11%</phonenumber4>"
        "</item>";

const std::string PoiItemXml =
        "<item>"
            "<type>poi</type>"
            "<poiid>%1%</poiid>"
            "<poiname>%2%</poiname>"
            "<poilon>%3%</poilon>"
            "<poilat>%4%</poilat>"
            "<guidepoint>"
                "<lon>%5%</lon>"
                "<lat>%6%</lat>"
            "</guidepoint>"
            "<tel>%7%</tel>"
            "<address>%8%</address>"
            "<linkid>%9%</linkid>"
            "<distance>%10%</distance>"
         "</item>";


const  std::string  null_string = "";
const std::string   vr_phone_connect_url = "content://system/info?bt/hfp-main";


/**
 * @brief The VR_SpotlightSearchTask class
 *
 * class declaration
 */
class  VR_SpotlightSearchTask
{
public:
    VR_SpotlightSearchTask(spVR_NIMusicShareProxy proxy);
    void   OnRequestCommonProperty(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void   OnRequestCancelSearchResult(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void   OnMessage(const VR_ProtoMessageBase& msg);
    void   OnMessageSearchStatus(const VR_ProtoMessageBase& msg);
    void   ReqModuleStatus();

protected:
    bool  isPhoneConnected() const;
    bool  isUsbConnected() const;

    void  ResponceSearchInfoResult(const std::string& value);

private:
    bool   m_bUpdate;
    int    m_requestId;
    bool   m_bMusicFinish;
    bool   m_bPhoneFinish;
    bool   m_bPoiFinish;
    std::string    m_strMusicItems;
    std::string    m_strPhoneItems;
    std::string    m_strPoiItems;
    spEV_EventContext    m_spContext;
    spVR_NIMusicShareProxy   m_spShareProxy;
};

#endif /* VR_SPOTLIGHTSEARCHTASK_H */
/* EOF */
