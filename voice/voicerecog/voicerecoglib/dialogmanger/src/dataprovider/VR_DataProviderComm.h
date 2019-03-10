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
 * @file VR_DataProviderComm.h
 * @brief Declaration file of class VR_DataProviderComm.
 *
 * This file includes the declaration of class VR_DataProviderComm.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DATA_PROVIDER_COMM_H
#define VR_DATA_PROVIDER_COMM_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>
#include "ncore/NCTypesDefine.h"
#include "MediaInfoProvider/NMMediaInfoDEF.h"
#include "VR_DP_ListData_Access.h"

namespace navi
{
namespace dataprovider
{

// dataprovider info uri
#define VR_DATAPROVIDER_INFO_URI                     "content://dataprovider/info?"
#define VR_DATAPROVIDER_RESULT_URI                   "content://dataprovider/result?"
#define VR_DATAPROVIDER_REQUEST_URI                  "content://dataprovider/request?"
#define VR_DATAPROVIDER_STATUS_INFO_URI              "content://dataprovider/status/info?"
#define VR_DATAPROVIDER_INITPERSONDATA_URI           "content://dataprovider/initpersonaldata?"
#define VR_DATAPROVIDER_MUSIC_CONN_STATUS_DEF_STR    "%04d%04d"

// system info uri
#define VR_SYSTEM_INFO_URI                           "content://system/info?"
#define VR_SYSTEM_INFO_VEHICLE_PBK_URI               "content://system/info?vehicle/pbk"
#define VR_SYSTEM_INFO_VEHICLE_OPERESTRICT_URI       "content://system/info?vehicle/operestrict"
#define VR_SYSTEM_INFO_BT_HFP_MAIN_URI               "content://system/info?bt/hfp-main"
#define VR_SYSTEM_INFO_BT_HFP_CURDEV_URI             "content://system/info?bt/hfp-curdev"
#define VR_SYSTEM_INFO_BT_HFP_DEVNUM_URI             "content://system/info?bt/hfp-devnum"
#define VR_SYSTEM_INFO_BT_RSS_URI                    "content://system/info?bt/rss"
#define VR_SYSTEM_INFO_BT_PHONE_CALL_URI             "content://system/info?bt/phonecall"
#define VR_SYSTEM_INFO_BT_PB_UPDATE_STATUS_URI       "content://system/info?bt/pbupdatestatus"
#define VR_SYSTEM_INFO_BT_AVP_URI                    "content://system/info?bt/avp"
#define VR_SYSTEM_INFO_BT_SPP_URI                    "content://system/info?bt/spp"
#define VR_SYSTRM_INFO_BT_PB_MODE_URI                "content://system/info?bt/pbmode"
#define VR_SYSTRM_INFO_BT_PB_UPDATE_URI              "content://system/info?bt/pbupdate"

/******Modify for PhoneTool*******/
#define VR_PHONE_BOOK_CONTACT_DATA_URI               "content://contacts/data?all"
#define VR_PHONE_BOOK_CONTACT_DATA_DICT_UPDATE_URI   "content://contacts/data?updatevrdict"
#define VR_PHONE_BOOK_CALL_HISTORY_URI               "content://callhistory/data?all"

#define VR_PHONE_BOOK_CONTACT_DATA_INFO_URI          "content://contacts/fulldata"
#define VR_PHONE_BOOK_CALL_HISTORY_INFO_URI          "content://callhistory/data"
#define VR_PHONE_BOOK_RECENT_MSGLIST_INFO_URI        "content://btmap/currentlist"
#define VR_PHONE_BOOK_QUICKLYREPLY_MESSSAGE_DATA_RUI "content://btmap/data?quickreplymessagelist"
#define VR_PHONE_BOOK_QUICKLYREPLY_MESSSAGE_LIST_RUI "content://btmap/quickreplymessagelist"
#define VR_PHONE_BOOK_LAZY_SERVICE_NOTIFY_URI        "content://dataprovider/lazyservicenotify?"
#define VR_PHONE_BOOK_MESSAGE_AVILIABLE_URI          "content://system/info?message/availiable"
#define VR_PHONE_SMS_MESSAGE_URI                     "content://btmap/listbytype"
#define VR_PHONE_CONTACTID_BY_NUMBER_URI             "content://contacts/mainhfpsearchcontactbynumber"

// ipod grace node info uri
#define VR_IPOD_GET_GRACENOTE_FINISHED_URI           "content://dataprovider/ipod/getfinished?"
#define VR_IPOD_CANCEL_GET_GRACENOTE_FINISHED_URI    "content://dataprovider/ipod/cancelfinished?"

// usb connect mode info uri
#define VR_USB1_CONNECT_MODE_INFO_URI                "content://system/info?usb1/mode"
#define VR_USB2_CONNECT_MODE_INFO_URI                "content://system/info?usb2/mode"

// tsl status info uri
#define VR_TSL_STATUS_INFO_URI                       "content://system/info?tsl/status"

// music
#define VR_MUSIC_USB_SCANN_FINISHED                  "content://mediadata/listener?mediashare"

// VR dialogmanager relate media sourceId
enum VR_Service_DM_MediaInfo_SrcId
{
    DM_MediaInfo_SrcID_Invalid     = nutshell::media::MediaInfo_SrcID_Invalid,
    DM_MediaInfo_SrcID_AUX         = nutshell::media::MediaInfo_SrcID_AUX,
    DM_MediaInfo_SrcID_USB1        = nutshell::media::MediaInfo_SrcID_USB1,
    DM_MediaInfo_SrcID_USB2        = nutshell::media::MediaInfo_SrcID_USB2,
    DM_MediaInfo_SrcID_BTA         = nutshell::media::MediaInfo_SrcID_BTA,
    DM_MediaInfo_SrcID_HFCall      = nutshell::media::MediaInfo_SrcID_HFCall,
    DM_MediaInfo_SrcID_HFDialIn    = nutshell::media::MediaInfo_SrcID_HFDialIn,
    DM_MediaInfo_SrcID_Apps        = nutshell::media::MediaInfo_SrcID_Apps,
    DM_MediaInfo_SrcID_MailReadout = nutshell::media::MediaInfo_SrcID_MailReadout,
    DM_MediaInfo_SrcID_Siri        = nutshell::media::MediaInfo_SrcID_Siri,
    DM_MediaInfo_SrcID_FM          = nutshell::media::MediaInfo_SrcID_FM,
    DM_MediaInfo_SrcID_AM          = nutshell::media::MediaInfo_SrcID_AM,
    DM_MediaInfo_SrcID_RSE         = nutshell::media::MediaInfo_SrcID_RSE,
    DM_MediaInfo_SrcID_SXM         = nutshell::media::MediaInfo_SrcID_SXM,
    DM_MediaInfo_SrcID_Radio       = nutshell::media::MediaInfo_SrcID_Radio,
    DM_MediaInfo_SrcID_TV          = nutshell::media::MediaInfo_SrcID_TV,
    DM_MediaInfo_SrcID_DTV         = nutshell::media::MediaInfo_SrcID_DTV,
    DM_MediaInfo_SrcID_DAB         = nutshell::media::MediaInfo_SrcID_DAB,
    DM_MediaInfo_SrcID_Disc        = nutshell::media::MediaInfo_SrcID_Disc,
    DM_MediaInfo_SrcID_Miracast    = nutshell::media::MediaInfo_SrcID_Miracast,
    DM_MediaInfo_SrcID_Mirrorlink  = nutshell::media::MediaInfo_SrcID_Mirrorlink,
    DM_MediaInfo_SrcID_Off         = nutshell::media::MediaInfo_SrcID_Off,
    DM_MediaInfo_SrcID_Apps1  = nutshell::media::MediaInfo_SrcID_Apps1,
    DM_MediaInfo_SrcID_Apps2  = nutshell::media::MediaInfo_SrcID_Apps2,
    DM_MediaInfo_SrcID_Apps3  = nutshell::media::MediaInfo_SrcID_Apps3,
    DM_MediaInfo_SrcID_Apps4  = nutshell::media::MediaInfo_SrcID_Apps4,
    DM_MediaInfo_SrcID_Apps5  = nutshell::media::MediaInfo_SrcID_Apps5,
    DM_MediaInfo_SrcID_Apps6  = nutshell::media::MediaInfo_SrcID_Apps6,
    DM_MediaInfo_SrcID_Apps7  = nutshell::media::MediaInfo_SrcID_Apps7,
    DM_MediaInfo_SrcID_Apps8  = nutshell::media::MediaInfo_SrcID_Apps8,
    DM_MediaInfo_SrcID_Apps9  = nutshell::media::MediaInfo_SrcID_Apps9,
    DM_MediaInfo_SrcID_Apps10 = nutshell::media::MediaInfo_SrcID_Apps10,
    DM_MediaInfo_SrcID_Apps11 = nutshell::media::MediaInfo_SrcID_Apps11,
    DM_MediaInfo_SrcID_Apps12 = nutshell::media::MediaInfo_SrcID_Apps12,
    DM_MediaInfo_SrcID_Apps13 = nutshell::media::MediaInfo_SrcID_Apps13,
    DM_MediaInfo_SrcID_Apps14 = nutshell::media::MediaInfo_SrcID_Apps14,
    DM_MediaInfo_SrcID_Apps15 = nutshell::media::MediaInfo_SrcID_Apps15,
    DM_MediaInfo_SrcID_Apps16 = nutshell::media::MediaInfo_SrcID_Apps16,
    DM_MediaInfo_SrcID_Apps17 = nutshell::media::MediaInfo_SrcID_Apps17,
    DM_MediaInfo_SrcID_Apps18 = nutshell::media::MediaInfo_SrcID_Apps18,
    DM_MediaInfo_SrcID_Apps19 = nutshell::media::MediaInfo_SrcID_Apps19,
    DM_MediaInfo_SrcID_Apps20 = nutshell::media::MediaInfo_SrcID_Apps20,
    DM_MediaInfo_SrcID_Apps21 = nutshell::media::MediaInfo_SrcID_Apps21,
    DM_MediaInfo_SrcID_Apps22 = nutshell::media::MediaInfo_SrcID_Apps22,
    DM_MediaInfo_SrcID_Apps23 = nutshell::media::MediaInfo_SrcID_Apps23,
    DM_MediaInfo_SrcID_Apps24 = nutshell::media::MediaInfo_SrcID_Apps24,
    DM_MediaInfo_SrcID_Apps25 = nutshell::media::MediaInfo_SrcID_Apps25,
    DM_MediaInfo_SrcID_Apps26 = nutshell::media::MediaInfo_SrcID_Apps26,
    DM_MediaInfo_SrcID_Apps27 = nutshell::media::MediaInfo_SrcID_Apps27,
    DM_MediaInfo_SrcID_Apps28 = nutshell::media::MediaInfo_SrcID_Apps28,
    DM_MediaInfo_SrcID_Apps29 = nutshell::media::MediaInfo_SrcID_Apps29,
    DM_MediaInfo_SrcID_Apps30 = nutshell::media::MediaInfo_SrcID_Apps30,
    DM_MediaInfo_SrcID_Apps31 = nutshell::media::MediaInfo_SrcID_Apps31,
    DM_MediaInfo_SrcID_Apps32 = nutshell::media::MediaInfo_SrcID_Apps32,
    DM_MediaInfo_SrcID_Apps33 = nutshell::media::MediaInfo_SrcID_Apps33,
    DM_MediaInfo_SrcID_Apps34 = nutshell::media::MediaInfo_SrcID_Apps34,
    DM_MediaInfo_SrcID_Apps35 = nutshell::media::MediaInfo_SrcID_Apps35,
    DM_MediaInfo_SrcID_Apps36 = nutshell::media::MediaInfo_SrcID_Apps36,
    DM_MediaInfo_SrcID_Apps37 = nutshell::media::MediaInfo_SrcID_Apps37,
    DM_MediaInfo_SrcID_Apps38 = nutshell::media::MediaInfo_SrcID_Apps38,
    DM_MediaInfo_SrcID_Apps39 = nutshell::media::MediaInfo_SrcID_Apps39,
    DM_MediaInfo_SrcID_Apps40 = nutshell::media::MediaInfo_SrcID_Apps40,
    DM_MediaInfo_SrcID_Apps41 = nutshell::media::MediaInfo_SrcID_Apps41,
    DM_MediaInfo_SrcID_Apps42 = nutshell::media::MediaInfo_SrcID_Apps42,
    DM_MediaInfo_SrcID_Apps43 = nutshell::media::MediaInfo_SrcID_Apps43,
    DM_MediaInfo_SrcID_Apps44 = nutshell::media::MediaInfo_SrcID_Apps44,
    DM_MediaInfo_SrcID_Apps45 = nutshell::media::MediaInfo_SrcID_Apps45,
    DM_MediaInfo_SrcID_Apps46 = nutshell::media::MediaInfo_SrcID_Apps46,
    DM_MediaInfo_SrcID_Apps47 = nutshell::media::MediaInfo_SrcID_Apps47,
    DM_MediaInfo_SrcID_Apps48 = nutshell::media::MediaInfo_SrcID_Apps48,
    DM_MediaInfo_SrcID_Apps49 = nutshell::media::MediaInfo_SrcID_Apps49,
    DM_MediaInfo_SrcID_Apps50 = nutshell::media::MediaInfo_SrcID_Apps50,
    DM_MediaInfo_SrcID_Ipod        = nutshell::media::MediaInfo_SrcID_Ipod,
    DM_MediaInfo_SrcID_Num         = nutshell::media::MediaInfo_SrcID_Num,
    DM_MediaInfo_SrcID_Ipod1,
    DM_MediaInfo_SrcID_Ipod2,
    DM_MediaInfo_SrcID_File,
};

enum VR_MUIFA_MUSIC_SOURCE_ID
{
    VR_MUIFA_SOURCE_INVALID  = 0,
    VR_MUIFA_SOURCE_U_DISK   = 1,
    VR_MUIFA_SOURCE_IPOD     = 2,
    VR_MUIFA_SOURCE_BT_AUDIO = 3,
};

// VR dialogmanager relate media device status
enum VR_Service_DM_MediaDevice_Status
{
    DM_MediaDevice_Status_Disable = 0,
    DM_MediaDevice_Status_Enable,
    DM_MediaDevice_Status_Unkown

};

} // dataprovider
} // navi

#endif /* VR_DATAPROVIDERDEF_H */
/* EOF */
