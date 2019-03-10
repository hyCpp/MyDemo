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
 * @file VR_DP_ListData_Access.h
 * @brief Declaration file of class VR_DP_ListData_Access.
 *
 * This file includes the declaration of class VR_DP_ListData_Access.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DP_LISTDATA_ACCESS_H
#define VR_DP_LISTDATA_ACCESS_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "stdafx.h"
#include "VR_Macro.h"
#include "VR_Log.h"
#include "BL_File.h"
#include "BL_Dir.h"
#include "VR_Def.h"
#include "ncore/NCTypesDefine.h"
#include "ncore/NCString.h"

namespace navi
{
namespace dataprovider
{
#define VR_AVC_LISTDATA_FILE_RWDISK_FOLDERNAME    "avclan"

// AvcLan List Data
#define VR_AVC_LISTDATA_PHONEBOOK_TYPECODE      0x00
#define VR_AVC_LISTDATA_MUSIC_INC_TYPECODE      0x01
#define VR_AVC_LISTDATA_MUSIC_EXC_TYPECODE      0x02
#define VR_AVC_LISTDATA_AUDIOSOURCE_TYPECODE    0x03
#define VR_AVC_LISTDATA_APPS_TYPECODE           0x04
#define VR_AVC_LISTDATA_RADIO_TYPECODE          0x05
#define VR_AVC_LISTDATA_RESERVE__TYPECODE       0x06
#define VR_AVC_LISTDATA_QUICKLYREPLY_TYPECODE   0x07
#define VR_AVC_LISTDATA_SERVICE_TYPECODE        0x08
#define VR_AVC_LISTDATA_CALLHISTORY_TYPECODE    0x09
#define VR_AVC_LISTDATA_MESSAGEHISTORY_TYPECODE 0x10
#define VR_AVC_LISTDATA_CITY_LIST_TYPECODE      0x11
#define VR_AVC_LISTDATA_COUNTS_TYPECODE         0X12


// Radio Station List Data

/*
* The binary file shown in the following format is transmitted to MEU when Radio station list and Radio station genre list are updated.
* The file consists of header /data unit as below, and each data is stored  with little-endian format.
* Character code of the string is UTF-8.
*/

// Record Number stored in Data Unit (sum of the station list number and the genre list number)  449
#define VR_AVC_RADIO_RESERVE_VALUE             0
#define VR_AVC_RADIO_RECORD_MAX_COUNT          450
#define VR_AVC_RADIO_STATION_MAX_COUNT         385
#define VR_AVC_RADIO_GENRE_MAX_COUNT           65
#define VR_AVC_RADIO_DIC_DEFAULT_VERSION       0x0001
#define VR_AVC_RADIO_TYPE_OF_FM                0
#define VR_AVC_RADIO_TYPE_OF_XM                1
#define VR_AVC_RADIO_TYPE_OF_DAB               2
#define VR_AVC_RADIO_DATA_TYPE_OF_STATION      0
#define VR_AVC_RADIO_DATA_TYPE_OF_GENRE        1
#define VR_AVC_RADIO_NAME_MAX_LEN              68

// Header unit
struct sAvc_RadioHeader
{
    BYTE  m_bTypeCode;    // Code type  0x05
    BYTE  m_bReserve;     // Reserve
    WORD  m_wRecordNum;   // Record Number stored in Data Unit (sum of the station list number and the genre list number)  449
    WORD  m_wStationNum;  //  Station list number
    WORD  m_wGenreNum;    //  Genre list number
    nutshell::UINT32 m_dwVersion;    // Dictionary version
    BYTE  m_bRadioType;   //  Radio type(0：FM　1:XM 2:DAB)
    BYTE  m_bzReserve[3]; // Reserve

    // contruction function
    sAvc_RadioHeader()
    {
        m_bTypeCode      = VR_AVC_LISTDATA_RADIO_TYPECODE;
        m_bReserve       = VR_AVC_RADIO_RESERVE_VALUE;
        m_wRecordNum     = 0;
        m_wStationNum    = 0;
        m_wGenreNum      = 0;
        m_dwVersion      = VR_AVC_RADIO_DIC_DEFAULT_VERSION;
        m_bRadioType     = VR_AVC_RADIO_TYPE_OF_FM;
        memset(m_bzReserve, 0x00, sizeof(m_bzReserve));
    }
};

struct sAvc_RadioStationUnit
{
    WORD    m_recordLen;
    BYTE    m_type;
    BYTE    m_nameLen;
    CHAR    m_sgname[VR_AVC_RADIO_NAME_MAX_LEN + 1];
    DWORD   m_chNo;

    sAvc_RadioStationUnit()
    {
        m_recordLen = 0;
        m_type = 0;
        m_nameLen = 0;
        memset(m_sgname, 0x00, sizeof(m_sgname));
        m_chNo = 0;
    }
};

// Music List Data (inc catalogDB)
#define VR_AVC_MUSIC_INC_WAKEUPID_DEFAULD_VALUE         0
#define VR_AVC_MUSIC_INC_LANGUAGE_DEFAULD_VALUE         0
#define VR_AVC_MUSIC_INC_DATA_DEFAULT_VERSION           0
#define VR_AVC_MUSIC_INC_DATA_VERSION_MAX_LEN           16
#define VR_AVC_MUSIC_INC_GRACENOTE_VERSION_MAX_LEN      16
#define VR_AVC_MUSIC_INC_SOURCE_DATA_MAX_COUNT          4
#define VR_AVC_MUSIC_INC_IPOD_MAX_COUNT                 2
#define VR_AVC_MUSIC_INC_IPOD_NAME_MAX_LEN              109

struct sAvc_MusicIncHeader
{
    BYTE  m_bTypeCode;
    BYTE  m_bzReserve[15];

    sAvc_MusicIncHeader()
    {
        m_bTypeCode  = VR_AVC_LISTDATA_MUSIC_INC_TYPECODE;
        memset(m_bzReserve, 0x00, sizeof(m_bzReserve));
    }
};

#define VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_IPOD       0x00
#define VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_USBA       0x01
#define VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_RIPPING    0x02
#define VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_DISCONNECT 0xFF

struct sAvc_MusicIncSourceDataUnit
{
    nutshell::UINT32   m_dwSourceId;
    nutshell::UINT32   m_dwSourceType;

    sAvc_MusicIncSourceDataUnit()
    {
        m_dwSourceId   = 0;
        m_dwSourceType = 0;
    }
};

struct sAvc_MusicIncVersionUnit
{
    BYTE  m_bCurDataVersionLen;
    CHAR  m_bzCurDataVersion[VR_AVC_MUSIC_INC_DATA_VERSION_MAX_LEN];
    BYTE  m_bGracenoteVersionLen;
    CHAR  m_bzGracenoteVersion[VR_AVC_MUSIC_INC_GRACENOTE_VERSION_MAX_LEN];
    BYTE  m_bOldDataVersionLen;
    CHAR  m_bzOldDataVersion[VR_AVC_MUSIC_INC_DATA_VERSION_MAX_LEN];
    BYTE  m_bOldGracenoteVersionLen;
    CHAR  m_bzOldGracenoteVersion[VR_AVC_MUSIC_INC_GRACENOTE_VERSION_MAX_LEN];

    sAvc_MusicIncVersionUnit()
    {
        m_bCurDataVersionLen      = 0;
        m_bOldDataVersionLen      = 0;
        m_bGracenoteVersionLen    = 0;
        m_bOldGracenoteVersionLen = 0;

        memset(m_bzCurDataVersion,      0x00, sizeof(m_bzCurDataVersion));
        memset(m_bzOldDataVersion,      0x00, sizeof(m_bzOldDataVersion));
        memset(m_bzGracenoteVersion,    0x00, sizeof(m_bzGracenoteVersion));
        memset(m_bzOldGracenoteVersion, 0x00, sizeof(m_bzOldGracenoteVersion));
    }
};

struct sAvc_MusicIncIPodInfoUnit
{
    nutshell::UINT32  m_dwSourceId;
    BYTE              m_bIPodNameLen;
    CHAR              m_bzIPodName[VR_AVC_MUSIC_INC_IPOD_NAME_MAX_LEN];

    sAvc_MusicIncIPodInfoUnit()
    {
        m_dwSourceId   = 0;
        m_bIPodNameLen = 0x00;
        memset(m_bzIPodName, 0x00, sizeof(m_bzIPodName));
    }
};

struct sAvc_MusicIncActiveUpdateUnit
{
    BYTE    m_bIsActive;
    BYTE    m_bGrammarDiffUpdate;
    BYTE    m_bMusicAgentAlbums;
    BYTE    m_bMusicAgentAlbumsSongs;
    BYTE    m_bMusicAgentArtists;
    BYTE    m_bMusicAgentArtistsSongs;
    BYTE    m_bMusicAgentAudiobooks;
    BYTE    m_bMusicAgentComposers;
    BYTE    m_bMusicAgentComposersSongs;
    BYTE    m_bMusicAgentGenres;
    BYTE    m_bMusicAgentGenresSongs;
    BYTE    m_bMusicAgentPlaylists;
    BYTE    m_bMusicAgentPlaylistsSongs;
    BYTE    m_bMusicAgentPodcasts;
    BYTE    m_bMusicAgentSongs;

    sAvc_MusicIncActiveUpdateUnit()
    {
        m_bIsActive = 0x00;                         // 1: Presence 0: Absence
        m_bGrammarDiffUpdate          = 0x00;       // 1: Update   0: Not update
        m_bMusicAgentAlbums           = 0x00;
        m_bMusicAgentAlbumsSongs      = 0x00;
        m_bMusicAgentArtists          = 0x00;
        m_bMusicAgentArtistsSongs     = 0x00;
        m_bMusicAgentAudiobooks       = 0x00;
        m_bMusicAgentComposers        = 0x00;
        m_bMusicAgentComposersSongs   = 0x00;
        m_bMusicAgentGenres           = 0x00;
        m_bMusicAgentGenresSongs      = 0x00;
        m_bMusicAgentPlaylists        = 0x00;
        m_bMusicAgentPlaylistsSongs   = 0x00;
        m_bMusicAgentPodcasts         = 0x00;
        m_bMusicAgentSongs            = 0x00;
    }
};

struct sAvc_MusicIncUnit
{
    WORD                           m_wRecordLen;
    nutshell::UINT32               m_dwWakeUpId;
    nutshell::UINT32               m_dwActiveDevice;
    nutshell::UINT32               m_dwSourceNum;
    nutshell::UINT32               m_dwLanguage;
    sAvc_MusicIncSourceDataUnit    m_stSourceData[VR_AVC_MUSIC_INC_SOURCE_DATA_MAX_COUNT];
    sAvc_MusicIncVersionUnit       m_stVersion;
    sAvc_MusicIncIPodInfoUnit      m_stIPodInfo[VR_AVC_MUSIC_INC_IPOD_MAX_COUNT];
    sAvc_MusicIncActiveUpdateUnit  m_stActiveUpdateUnit;

    sAvc_MusicIncUnit()
    {
        m_wRecordLen     = sizeof(sAvc_MusicIncUnit);
        m_dwWakeUpId     = 0;
        m_dwActiveDevice = 0;
        m_dwSourceNum    = 0;
        m_dwLanguage     = 0;
    }
};

// Music List Data (exc catalogDB)
#define VR_AVC_MUSIC_EXC_RESERVE_VALUE                  0
#define VR_AVC_MUSIC_EXC_RECORD_MAX_COUNT               500
#define VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_ADD               1
#define VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL               2
#define VR_AVC_MUSIC_EXC_TABLE_NAME_MAX_LEN             32
#define VR_AVC_MUSIC_EXC_CNAME_MAX_LEN                  256
#define VR_AVC_MUSIC_EXC_CPRONUNCIATION_MAX_LEN         256
#define VR_AVC_MUSIC_EXC_NPRONUNCIATION_DEFAULT_ID      -1

struct sAvc_MusicExcHeader
{
    BYTE  m_bTypeCode;
    BYTE  m_bReserve;
    WORD  m_wRecordNum;
    BYTE  m_bzReserve[12];

    sAvc_MusicExcHeader()
    {
        m_bTypeCode    = VR_AVC_LISTDATA_MUSIC_EXC_TYPECODE;
        m_bReserve     = VR_AVC_MUSIC_EXC_RESERVE_VALUE;
        m_wRecordNum   = 0;
        memset(m_bzReserve, 0x00, sizeof(m_bzReserve));
    }
};

// Audio Source List Data
#define VR_AVC_AUDIOSOURCE_RECORD_MAX_COUNT    36
#define VR_AVC_AUDIOSOURCE_INVALID_MODE        0
#define VR_AVC_AUDIOSOURCE_NAME_MAX_LEN        64

struct sAvc_AudioSourceHeader
{
    BYTE  m_bTypeCode;
    BYTE  m_bReserve;
    WORD  m_wRecordNum;
    BYTE  m_bzReserve[12];

    sAvc_AudioSourceHeader()
    {
        m_bTypeCode   = VR_AVC_LISTDATA_AUDIOSOURCE_TYPECODE;
        m_bReserve    = 0x00;
        m_wRecordNum  = 0;
        memset(m_bzReserve, 0x00, sizeof(m_bzReserve));
    }
};

// Phone Book List Data
#define VR_AVC_PHONE_RECORD_MAX_COUNT 5000
#define VR_VR_AVC_PHONE_RESERVE_VALUE 0
#define VR_AVC_PHONE_NAME_SIDE_MAX_COUNT       72
#define VR_AVC_PHONE_NUMBER_MAX_COUNT          32
#define VR_AVC_PHONE_ADDRESS_MAX_COUNT         256

struct sAvc_PhoneBookHeader
{
    BYTE  m_bTypeCode;
    BYTE  m_bReserve;
    WORD  m_wRecordNum;
    BYTE  m_bzReserve[12];

    sAvc_PhoneBookHeader()
    {
        m_bTypeCode   = VR_AVC_LISTDATA_PHONEBOOK_TYPECODE;
        m_bReserve    = 0x00;
        m_wRecordNum  = 0;
        memset(m_bzReserve, 0x00, sizeof(m_bzReserve));
    }
};

struct sAvc_PhoneNameUnit
{
    BYTE m_fNameLen;
    CHAR m_fName[VR_AVC_PHONE_NAME_SIDE_MAX_COUNT + 1];
    BYTE m_lNameLen;
    CHAR m_lName[VR_AVC_PHONE_NAME_SIDE_MAX_COUNT + 1];
    BYTE m_NULL;

    sAvc_PhoneNameUnit()
    {
        m_fNameLen = 0x00;
        memset(m_fName, 0x00, sizeof(m_fName));
        m_lNameLen = 0x00;
        memset(m_lName, 0x00, sizeof(m_lName));
        m_NULL = 0x00;
    }
};

struct sAvc_PhoneNumberUnit
{
    BYTE m_NymLen;
    CHAR m_Number[VR_AVC_PHONE_NUMBER_MAX_COUNT + 1];
    BYTE m_numType;
    BYTE m_numKind;

    sAvc_PhoneNumberUnit()
    {
        m_NymLen = 0x00;
        memset(m_Number, 0x00, sizeof(m_Number));
        m_numType = 0xFF;
        m_numKind = 0x00;
    }
};

struct sAvc_AddressNumberUint
{
    WORD m_nameLen;
    CHAR m_address[VR_AVC_PHONE_ADDRESS_MAX_COUNT + 1];

    sAvc_AddressNumberUint()
    {
        m_nameLen = 0;
        memset(m_address, 0x00, sizeof(m_address));
    }
};

struct sAvc_PhoneName_KANA
{
    BYTE m_fnameLen;
    CHAR m_fname[VR_AVC_PHONE_NAME_SIDE_MAX_COUNT + 1];
    BYTE m_lnameLen;
    CHAR m_lname[VR_AVC_PHONE_NAME_SIDE_MAX_COUNT + 1];

    sAvc_PhoneName_KANA()
    {
        m_fnameLen = 0;
        memset(m_fname, 0x00, sizeof(m_fname));
        m_lnameLen = 0;
        memset(m_lname, 0x00, sizeof(m_lname));
    }
};

#define VR_PB_INVAIL_INDEX 0xFFFFFFFF
struct sAvc_PhoneBookUnit
{
    WORD m_recordLen;
    nutshell::UINT32 m_index;
    sAvc_PhoneNameUnit m_Name;
    sAvc_PhoneNumberUnit m_Number[4];
    sAvc_AddressNumberUint m_address[4];
    sAvc_PhoneName_KANA m_nameKANA;

    sAvc_PhoneBookUnit()
    {
        m_recordLen = sizeof(sAvc_PhoneBookUnit);
        m_index = VR_PB_INVAIL_INDEX;
    }
};

// Message Fixed Format List Data Header Unit
#define VR_AVC_MSGFIXED_RECORD_MAX_COUNT 15
#define VR_AVC_QUICK_MSG_PHRASE_MAXCOUNT 161

struct sAvc_MsgFixedFormatHeader
{
    BYTE  m_bTypeCode;
    BYTE  m_bReserve;
    WORD  m_wRecordNum;
    BYTE  m_bzReserve[12];

    sAvc_MsgFixedFormatHeader()
    {
        m_bTypeCode   = VR_AVC_LISTDATA_QUICKLYREPLY_TYPECODE;
        m_bReserve    = VR_VR_AVC_PHONE_RESERVE_VALUE;
        m_wRecordNum  = 0;
        memset(m_bzReserve, 0x00, sizeof(m_bzReserve));
    }
};

struct sAvc_MsgFixedFormatUnit
{
    WORD m_recordLen;
    WORD m_phraseLen;
    CHAR m_phrase[VR_AVC_QUICK_MSG_PHRASE_MAXCOUNT + 1];

    sAvc_MsgFixedFormatUnit()
    {
        m_recordLen = 0;
        m_phraseLen = 0;
        memset(m_phrase, 0x00, sizeof(m_phrase));
    }
};

// service list data
#define VR_AVC_SERVICE_RECORD_MAX_NUM 12
#define VR_AVC_SERVICE_NAME_MAX_COUNT 32

struct sAvc_ServiceHeader
{
    BYTE  m_bTypeCode;
    BYTE  m_bReserve;
    WORD  m_wRecordNum;
    BYTE  m_bzReserve[12];

    sAvc_ServiceHeader()
    {
        m_bTypeCode   = VR_AVC_LISTDATA_SERVICE_TYPECODE;
        m_bReserve    = VR_VR_AVC_PHONE_RESERVE_VALUE;
        m_wRecordNum  = 0;
        memset(m_bzReserve, 0x00, sizeof(m_bzReserve));
    }
};

struct sAvc_ServiceUnit
{
    WORD  m_recordLen;
    BYTE  m_bNameLen;
    CHAR  m_cName[VR_AVC_SERVICE_NAME_MAX_COUNT + 1];
    BYTE  m_bSeviceId;
    BYTE  m_bSupportType;

    sAvc_ServiceUnit()
    {
        m_recordLen   = sizeof(sAvc_ServiceUnit);
        m_bNameLen    = 0x00;
        memset(m_cName, 0x00, sizeof(m_cName));
        m_bSeviceId  = 0x00;
        m_bSupportType = 0x00;
    }
};

// Incoming/Outgoing call history List Data
#define VR_AVC_CALLHISTORY_RECORD_MAX_COUNT 90
#define VR_AVC_CALL_TIME_MAX_COUNT 28
#define VR_AVC_CALL_NUMBER_MAX_COUNT 32

struct sAvc_CallHistoryHeader
{
    BYTE  m_bTypeCode;
    BYTE  m_bReserve;
    WORD  m_wRecordNum;
    BYTE  m_bzReserve[12];

    sAvc_CallHistoryHeader()
    {
        m_bTypeCode   = VR_AVC_LISTDATA_CALLHISTORY_TYPECODE;
        m_bReserve    = 0x00;
        m_wRecordNum  = 0;
        memset(m_bzReserve, 0x00, sizeof(m_bzReserve));
    }
};

struct sAvc_CallHistNumberUnit
{
    BYTE m_NymLen;
    CHAR m_Number[VR_AVC_CALL_NUMBER_MAX_COUNT + 1];

    sAvc_CallHistNumberUnit()
    {
        m_NymLen = 0x00;
        memset(m_Number, 0x00, sizeof(m_Number));
    }
};

struct sAvc_CallHistoryUnit
{
    WORD  m_recordLen;
    BYTE  m_DataTime[VR_AVC_CALL_TIME_MAX_COUNT];
    DWORD m_iPB_EntryNum;
    sAvc_PhoneNameUnit  m_PhoneName;
    sAvc_CallHistNumberUnit m_PhoneNumber;
    BYTE  m_bPhoneType;
    BYTE  m_bHistType;

    sAvc_CallHistoryUnit()
    {
        m_recordLen = sizeof(sAvc_CallHistoryUnit);
        memset(m_DataTime, 0x00, sizeof(m_DataTime));
        m_iPB_EntryNum = 0;
        m_bPhoneType = 0x00;
        m_bHistType = 0x00;
    }
};

// message transmission and reception history Header Unit
#define VR_AVC_MSGHITORY_RECORD_MAX_COUNT 960
#define VR_AVC_MESSAGE_HANDLE_MAX_COUNT 16
#define VR_AVC_MESSAGE_STRING_MAX_COUNT 256

struct sAvc_MessageHistoryHeader
{
    BYTE  m_bTypeCode;
    BYTE  m_bReserve;
    WORD  m_wRecordNum;
    BYTE  m_bzReserve[12];

    sAvc_MessageHistoryHeader()
    {
        m_bTypeCode   = VR_AVC_LISTDATA_MESSAGEHISTORY_TYPECODE;
        m_bReserve    = 0x00;
        m_wRecordNum  = 0;
        memset(m_bzReserve, 0x00, sizeof(m_bzReserve));
    }
};

struct sAvc_MsgHistAddressUnit
{
    WORD m_Len;
    CHAR m_cAddress[VR_AVC_MESSAGE_STRING_MAX_COUNT + 1];

    sAvc_MsgHistAddressUnit()
    {
        m_Len = 0;
        memset(m_cAddress, 0x00, sizeof(m_cAddress));
    }
};

struct sAvc_MsgHistSubjectUnit
{
    WORD m_Len;
    CHAR m_cSubject[VR_AVC_MESSAGE_STRING_MAX_COUNT + 1];

    sAvc_MsgHistSubjectUnit()
    {
        m_Len = 0;
        memset(m_cSubject, 0x00, sizeof(m_cSubject));
    }
};

struct sAvc_MsgHistSenderUnit
{
    WORD m_Len;
    CHAR m_cSenderName[VR_AVC_MESSAGE_STRING_MAX_COUNT + 1];

    sAvc_MsgHistSenderUnit()
    {
        m_Len = 0;
        memset(m_cSenderName, 0x00, sizeof(m_cSenderName));
    }
};

struct sAvc_MessageHistoryUnit
{
    WORD  m_recordLen;
    CHAR  m_cMsgId[VR_AVC_MESSAGE_HANDLE_MAX_COUNT + 1];
    BYTE  m_bMsgType;
    BYTE  m_bServiceId;
    BYTE  m_bReception;
    BYTE  m_bReserve;
    BYTE  m_bReadStatus;
    BYTE  m_bAttachMent;
    CHAR  m_bDataTime[VR_AVC_MESSAGE_HANDLE_MAX_COUNT + 1];
    sAvc_MsgHistAddressUnit m_Address;
    sAvc_MsgHistSubjectUnit m_Subject;
    sAvc_MsgHistSenderUnit  m_SenderName;

    sAvc_MessageHistoryUnit()
    {
        m_recordLen = sizeof(sAvc_MessageHistoryUnit);
        memset(m_cMsgId, 0x00, sizeof(m_cMsgId));
        m_bMsgType = 0x00;
        m_bServiceId = 0x00;
        m_bReception = 0x00;
        m_bReserve = 0x00;
        m_bReadStatus = 0x00;
        m_bAttachMent = 0x00;
        memset(m_bDataTime, 0x00, sizeof(m_bDataTime));
    }
};

// City List Data
#define VR_AVC_CITY_NAME_MAX_COUNT        255
#define VR_AVC_CITY_LIST_MAX_COUNT        500
#define VR_AVC_CITY_LIST_DEFAULT_VERSION  0x0001

struct sAvc_CityListDataHeader
{
    BYTE  m_bTypeCode;
    BYTE  m_bReserve;
    WORD  m_wRecordNum;
    CHAR  m_dTimeStamp[4];
    DWORD m_dVersion;
    BYTE  m_bzReserve[4];

    sAvc_CityListDataHeader()
    {
        m_bTypeCode   = VR_AVC_LISTDATA_CITY_LIST_TYPECODE;
        m_bReserve    = 0x00;
        m_wRecordNum  = 0;
        m_dVersion    = 0;
        memset(m_dTimeStamp, 0x00, sizeof(m_dTimeStamp));
        memset(m_bzReserve, 0x00, sizeof(m_bzReserve));
    }
};

struct sAvc_CityListDataUnit
{
    WORD  m_recordLen;
    DWORD m_cityIDKey;
    WORD  m_dCityNameLen;
    CHAR  m_cCityName[VR_AVC_CITY_NAME_MAX_COUNT + 1];
    sAvc_CityListDataUnit()
    {
        m_recordLen = sizeof(sAvc_CityListDataUnit);
        m_cityIDKey = 0;
        m_dCityNameLen = 0;
        memset(m_cCityName, 0x00, sizeof(m_cCityName));
    }
};

// App List Data
#define VR_AVC_APP_MANIFEST_MAX_LEN        1025
#define VR_AVC_APP_LIST_MAX_COUNT          100

struct sAvc_AppListDataHeader
{
    BYTE  m_bTypeCode;
    BYTE  m_bReserve;
    WORD  m_wRecordNum;
    BYTE  m_bzReserve[12];

    sAvc_AppListDataHeader()
    {
        m_bTypeCode   = VR_AVC_LISTDATA_APPS_TYPECODE;
        m_bReserve    = 0x00;
        m_wRecordNum  = 0;
        memset(m_bzReserve, 0x00, sizeof(m_bzReserve));
    }
};

struct sAvc_AppListDataUnit
{
    WORD m_recordLen;
    WORD m_dManiFestLen;
    CHAR m_cManiFest[VR_AVC_APP_MANIFEST_MAX_LEN + 1];
    sAvc_AppListDataUnit()
    {
        m_recordLen = sizeof(sAvc_AppListDataUnit);
        m_dManiFestLen = 0;
        memset(m_cManiFest, 0x00, sizeof(m_cManiFest));
    }
};

/**
 * @brief The VR_DP_ListData_Access class
 *
 * declare class VR_DP_ListData_Access
 */
class VR_DP_ListData_Access
{
public:
    VR_DP_ListData_Access();
    virtual ~VR_DP_ListData_Access();

    enum eFileType
    {
        eFileType_CityList              = 0,
        eFileType_RadioStationList      = 1,
        eFileType_MusicIncList          = 2,
        eFileType_MusicExcList          = 3,
        eFileType_MusicCatalogDB        = 4,
        eFileType_MusicContentsDB       = 5,
        eFileType_AudioSourceList       = 6,
        eFileType_PhoneBookList         = 7,
        eFileType_MsgFixedFormatList    = 8,
        eFileType_ServiceList           = 9,
        eFileType_CallHistoryList       = 10,
        eFileType_MsgHistoryList        = 11,
        eFileType_AppList               = 12,
        eFileType_RadioFMStationList    = 13,
        eFileType_RadioXMStationList    = 14,
        eFileType_RadioDABStationList   = 15,
        eFileType_RadioFMGenreList      = 16,
        eFileType_RadioXMGenreList      = 17,
        eFileType_MusicExcListDiffTmp   = 18,
        eFileType_MusicIncListFullTmp   = 19,
        eFileType_MusicIncListDiffTmp   = 20,
        eFileType_MusicContentsDBFullTmp = 21,
        eFileType_MusicContentsDBDiffTmp = 22,
        eFileType_MusicCatalogDBFullTmp  = 23,
        eFileType_AudioSourceListTmp     = 24,
        eFileType_PhoneBookListTmp       = 25,
        eFileType_MsgFixedFormatListTmp  = 26,
        eFileType_CallHistoryListTmp     = 27,
        eFileType_MsgHistoryListTmp      = 28,
        eFileType_AppListTmp             = 29,
        eFileType_CityListTmp            = 30,
        eFileType_Num,
        eFileType_Invalid
    };

    enum eSeekType
    {
        eSeekType_Start              = 0,
        eSeekType_Cur                = 1,
        eSeekType_End                = 2
    };

    static const nutshell::NCCHAR* g_czFileListName[eFileType_Num];
    static eFileType GetFileType(BL_String filePath);
    static BL_String GetFileRelativePath(eFileType fileType);
    static BL_String GetAvcLanRootPath();

    bool ReadFileStart(eFileType eFType);
    bool ReadFileEnd();

    bool WriteFileStart(eFileType eFType, nutshell::UINT32 dwFileSize);
    bool WriteFileEnd();

    bool Seek(eSeekType eSType, nutshell::INT32 dwStep); // iSeekType: 0:Seek Start 1:Seek Cur 2:Seek End
    void GetFilePath(VR_String& filePath);
public:
    bool WriteByte(BYTE bSrc);
    bool WriteWord(WORD wSrc);
    bool WriteDWord(nutshell::UINT32 dwSrc);
    bool WriteBuffer(const VOID* pbSrc, nutshell::UINT32 wWriteSize, nutshell::UINT32& wWritedSize);

    bool ReadByte(BYTE& bDest);
    bool ReadWord(WORD& wDest);
    bool ReadDWord(nutshell::UINT32& dwDest);
    bool ReadBuffer(VOID* pbDest, nutshell::UINT32 wReadSize, nutshell::UINT32& wReadedSize);

private:
    eFileType             m_curFileType;
    BL_File               m_cAccessFile;
    BYTE*                 m_pszFileData;
    nutshell::UINT32      m_dwFileSize;
    nutshell::UINT32      m_dwCursor;

private:
    VR_DP_ListData_Access(const VR_DP_ListData_Access&);
    VR_DP_ListData_Access& operator=(const VR_DP_ListData_Access&);

};

}

}

#endif /* VR_DP_LISTDATA_ACCESS_H */
/* EOF */
