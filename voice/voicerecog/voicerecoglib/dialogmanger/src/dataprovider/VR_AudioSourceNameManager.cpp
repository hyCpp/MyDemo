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
#include "VR_DataProviderComm.h"
#include "VR_AudioSourceNameManager.h"
#include <boost/assign.hpp>

using namespace nutshell;
namespace navi
{
namespace dataprovider
{

// Aux audio source.
const static VoiceList<ASAliasProperty>::type AuxASNameList = boost::assign::list_of
        (ASAliasProperty("Aux",            "Auxiliary"))
        (ASAliasProperty("Aux Jack",       "Auxiliary"))
        (ASAliasProperty("Auxiliary",      "Auxiliary"))
        (ASAliasProperty("Audio jack",     "Auxiliary"))
        (ASAliasProperty("Stereo plug",    "Auxiliary"))
        (ASAliasProperty("Headphone jack", "Auxiliary"))
        (ASAliasProperty("AUX",            "Auxiliary"));

const static ASNameProperty AuxNameProperty = {
    0x13,
    2,
    "Auxiliary",
    &AuxASNameList
};

// USB1 audio source.
const static VoiceList<ASAliasProperty>::type USB1ASNameList = boost::assign::list_of
        (ASAliasProperty("USB",          "USB"))
        (ASAliasProperty("USB Audio",    "USB"))
        (ASAliasProperty("USB 1",        "USB"))
        (ASAliasProperty("MP3 Player",   "USB"))
        (ASAliasProperty("MP3 Player 1", "USB"))
        (ASAliasProperty("MP3 1",        "USB"))
        (ASAliasProperty("MP3",          "USB"))
        (ASAliasProperty("USB1",         "USB"));

const static ASNameProperty Usb1NameProperty = {
    0x0D,
    3,
    "USB",
    &USB1ASNameList
};

// USB2 audio source.
const static VoiceList<ASAliasProperty>::type USB2ASNameList = boost::assign::list_of
        (ASAliasProperty("USB Audio 2",   "USB2"))
        (ASAliasProperty("USB 2",         "USB2"))
        (ASAliasProperty("MP3 Player 2",  "USB2"))
        (ASAliasProperty("MP3 2",         "USB2"))
        (ASAliasProperty("USB2",          "USB2"));

const static ASNameProperty Usb2NameProperty = {
    0x0E,
    4,
    "USB2",
    &USB2ASNameList
};

// Bluetooth audio source.
const static VoiceList<ASAliasProperty>::type BTAASNameList = boost::assign::list_of
        (ASAliasProperty("Bluetooth",       "Bluetooth Audio"))
        (ASAliasProperty("Bluetooth Audio", "Bluetooth Audio"))
        (ASAliasProperty("BT Audio",        "Bluetooth Audio"))
        (ASAliasProperty("BTA",             "Bluetooth Audio"));

const static ASNameProperty BTANameProperty = {
    0x0F,
    5,
    "Bluetooth Audio",
    &BTAASNameList
};

// FM audio source.
const static VoiceList<ASAliasProperty>::type FMASNameList = boost::assign::list_of
        (ASAliasProperty("FM",       "FM"))
        (ASAliasProperty("FM Radio", "FM"))
        (ASAliasProperty("Radio",    "FM"));

const static ASNameProperty FMNameProperty = {
    0x03,
    11,
    "FM",
    &FMASNameList
};

// AM audio source.
const static VoiceList<ASAliasProperty>::type AMASNameList = boost::assign::list_of
        (ASAliasProperty("AM",              "AM"))
        (ASAliasProperty("AM Radio",        "AM"));

const static ASNameProperty AMNameProperty = {
    0x01,
    12,
    "AM",
    &AMASNameList
};

// Rear Entertainment System audio source.
const static VoiceList<ASAliasProperty>::type RESASNameList = boost::assign::list_of
        (ASAliasProperty("Rear",                         "Rear Entertainment System"))
        (ASAliasProperty("Back Seat",                    "Rear Entertainment System"))
        (ASAliasProperty("Rear Seat Entertainment",      "Rear Entertainment System"))
        (ASAliasProperty("Rear System",                  "Rear Entertainment System"))
        (ASAliasProperty("RSE",                          "Rear Entertainment System"))
        (ASAliasProperty("RES",                          "Rear Entertainment System"))
        (ASAliasProperty("Rear Entertainment System",    "Rear Entertainment System"))
        (ASAliasProperty("DVD",                          "Rear Entertainment System"))
        (ASAliasProperty("DVD Player",                   "Rear Entertainment System"));

const static ASNameProperty RSENameProperty = {
    0x17,
    13,
    "Rear Entertainment System",
    &RESASNameList
};

// XM audio source.
const static VoiceList<ASAliasProperty>::type SXMASNameList = boost::assign::list_of
        (ASAliasProperty("XM",               "XM"))
        (ASAliasProperty("Sirius",           "XM"))
        (ASAliasProperty("Sirius XM",        "XM"))
        (ASAliasProperty("Sat",              "XM"))
        (ASAliasProperty("Satellite",        "XM"))
        (ASAliasProperty("Satellite Radio",  "XM"));

const static ASNameProperty SXMNameProperty = {
    0x06,
    14,
    "XM",
    &SXMASNameList
};

// DAB audio source.
const static VoiceList<ASAliasProperty>::type DABASNameList = boost::assign::list_of
        (ASAliasProperty("DAB",    "DAB"));

const static ASNameProperty DABNameProperty = {
    0x08,
    18,
    "DAB",
    &DABASNameList
};

// Disc audio source.
const static VoiceList<ASAliasProperty>::type DiscASNameList = boost::assign::list_of
        (ASAliasProperty("CD",         "CD"))
        (ASAliasProperty("CD Player",  "CD"))
        (ASAliasProperty("CD Changer", "CD"))
        (ASAliasProperty("Disc",       "CD"))
        (ASAliasProperty("MP3 CD",     "CD"));

const static ASNameProperty DiscNameProperty = {
    0x09,
    21,
    "CD",
    &DiscASNameList
};

// Ipod1 audio source
const static VoiceList<ASAliasProperty>::type Ipod1ASNameList = boost::assign::list_of
        (ASAliasProperty("iPod",    "iPod"))
        (ASAliasProperty("iPod 1",  "iPod"));

const static ASNameProperty Ipod1NameProperty = {
    0xF0,
    78,
    "iPod",
    &Ipod1ASNameList
};

// Ipod2 audio source
const static VoiceList<ASAliasProperty>::type Ipod2ASNameList = boost::assign::list_of
        (ASAliasProperty("iPod 2",  "iPod2"));

const static ASNameProperty Ipod2NameProperty = {
    0xF1,
    79,
    "iPod2",
    &Ipod2ASNameList
};

VR_AudioSourceNameManager::AudioSourceNameMap VR_AudioSourceNameManager::m_audioSourceMap = boost::assign::map_list_of
        ((INT)DM_MediaInfo_SrcID_AUX,         reinterpret_cast<INT64>(&AuxNameProperty))
        ((INT)DM_MediaInfo_SrcID_USB1,        reinterpret_cast<INT64>(&Usb1NameProperty))
        ((INT)DM_MediaInfo_SrcID_USB2,        reinterpret_cast<INT64>(&Usb2NameProperty))
        ((INT)DM_MediaInfo_SrcID_BTA,         reinterpret_cast<INT64>(&BTANameProperty))
        ((INT)DM_MediaInfo_SrcID_HFCall,      NULL)
        ((INT)DM_MediaInfo_SrcID_HFDialIn,    NULL)
        ((INT)DM_MediaInfo_SrcID_Apps,        NULL)
        ((INT)DM_MediaInfo_SrcID_MailReadout, NULL)
        ((INT)DM_MediaInfo_SrcID_Siri,        NULL)
        ((INT)DM_MediaInfo_SrcID_FM,          reinterpret_cast<INT64>(&FMNameProperty))
        ((INT)DM_MediaInfo_SrcID_AM,          reinterpret_cast<INT64>(&AMNameProperty))
        ((INT)DM_MediaInfo_SrcID_RSE,         reinterpret_cast<INT64>(&RSENameProperty))
        ((INT)DM_MediaInfo_SrcID_SXM,         reinterpret_cast<INT64>(&SXMNameProperty))
        ((INT)DM_MediaInfo_SrcID_Radio,       NULL)
        ((INT)DM_MediaInfo_SrcID_TV,          NULL)
        ((INT)DM_MediaInfo_SrcID_DTV,         NULL)
        ((INT)DM_MediaInfo_SrcID_DAB,         reinterpret_cast<INT64>(&DABNameProperty))
        ((INT)DM_MediaInfo_SrcID_Disc,        reinterpret_cast<INT64>(&DiscNameProperty))
        ((INT)DM_MediaInfo_SrcID_Miracast,    NULL)
        ((INT)DM_MediaInfo_SrcID_Mirrorlink,  NULL)
        ((INT)DM_MediaInfo_SrcID_Off,         NULL)
        ((INT)DM_MediaInfo_SrcID_Ipod,        NULL)
        ((INT)DM_MediaInfo_SrcID_Ipod1,       reinterpret_cast<INT64>(&Ipod1NameProperty))
        ((INT)DM_MediaInfo_SrcID_Ipod2,       reinterpret_cast<INT64>(&Ipod2NameProperty));

ASNameProperty* VR_AudioSourceNameManager::FindNameProperty(nutshell::INT sourceId)
{
    AudioSourceNameMap::iterator iter = m_audioSourceMap.find(sourceId);
    if (iter != m_audioSourceMap.end()) {
        return reinterpret_cast<ASNameProperty*>(iter->second);
    }

    return NULL;
}

nutshell::INT VR_AudioSourceNameManager::FindSourceId(const nutshell::NCString& formalName)
{
    AudioSourceNameMap::iterator iter = m_audioSourceMap.begin();
    for (; iter != m_audioSourceMap.end(); ++iter) {
        ASNameProperty* nameProperty = reinterpret_cast<ASNameProperty*>(iter->second);
        if (NULL == nameProperty) {
            continue;
        }

        if (nameProperty->formalName == formalName) {
            return iter->first;
        }

        VoiceList<ASAliasProperty>::const_iterator iterAlias = nameProperty->aliasList->cbegin();
        for (; iterAlias != nameProperty->aliasList->cend(); ++iterAlias) {
            ASAliasProperty aliasProperty = *iterAlias;

            if (aliasProperty.m_alias == formalName) {
                return iter->first;
            }
        }
    }

    return DM_MediaInfo_SrcID_Invalid;
}

nutshell::INT VR_AudioSourceNameManager::FindFcSourceId(const nutshell::NCString& formalName)
{
    INT nSourceId = FindSourceId(formalName);
    if (DM_MediaInfo_SrcID_Invalid == nSourceId) {
        return ASNAME_PROPERTY_FC_SOURCEID_INVALID_VALUE;
    }

    AudioSourceNameMap::iterator iter = m_audioSourceMap.find(nSourceId);
    if (iter != m_audioSourceMap.end()) {
        ASNameProperty* nameProperty = reinterpret_cast<ASNameProperty*>(iter->second);
        if (NULL == nameProperty) {
            return ASNAME_PROPERTY_FC_SOURCEID_INVALID_VALUE;
        }
        return nameProperty->FcSourceId;
    }

    return ASNAME_PROPERTY_FC_SOURCEID_INVALID_VALUE;
}

nutshell::INT VR_AudioSourceNameManager::FindFcSourceId(const nutshell::INT& sourceId)
{
    AudioSourceNameMap::iterator iter = m_audioSourceMap.find(sourceId);
    if (iter != m_audioSourceMap.end()) {
        ASNameProperty* nameProperty = reinterpret_cast<ASNameProperty*>(iter->second);
        if (NULL == nameProperty) {
            return ASNAME_PROPERTY_FC_SOURCEID_INVALID_VALUE;
        }
        return nameProperty->FcSourceId;
    }

    return ASNAME_PROPERTY_FC_SOURCEID_INVALID_VALUE;
}

nutshell::INT VR_AudioSourceNameManager::FindModeId(const nutshell::NCString& formalName)
{
    INT nSourceId = FindSourceId(formalName);
    if (DM_MediaInfo_SrcID_Invalid == nSourceId) {
        return ASNAME_PROPERTY_MODEID_INVALID_VALUE;
    }

    AudioSourceNameMap::iterator iter = m_audioSourceMap.find(nSourceId);
    if (iter != m_audioSourceMap.end()) {
        ASNameProperty* nameProperty = reinterpret_cast<ASNameProperty*>(iter->second);
        if (NULL == nameProperty) {
            return ASNAME_PROPERTY_MODEID_INVALID_VALUE;
        }
        return nameProperty->ModeId;
    }

    return ASNAME_PROPERTY_MODEID_INVALID_VALUE;
}
}
}
/* EOF */
