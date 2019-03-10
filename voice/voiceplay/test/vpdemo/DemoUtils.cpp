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

#include "DemoUtils.h"
using namespace std;
using namespace ::navi::VoicePlay;

BL_APLINFO_REGION DemoUtils::region = BL_APLINFO_REGION_INVALID;
int DemoUtils::language = 0;

long DemoUtils::wrapSoundId(long unique_id)
{
    long regionId = 0x0;

    switch (region) {
    case BL_APLINFO_REGION_CHINA:
        regionId = getChinaId(); 
        break;
    case BL_APLINFO_REGION_US:
        regionId = getNorthAmericaId();
        break;
    case BL_APLINFO_REGION_SEA:
        regionId = getSouthEastAsiaId();
        break;
    case BL_APLINFO_REGION_SA:
        regionId = getSouthAfricaId();
        break;
    case BL_APLINFO_REGION_TW:
        regionId = getTaiwanId();
        break;
    case BL_APLINFO_REGION_OC:
        regionId = getOceaniaId();
        break;
    case BL_APLINFO_REGION_HK:
        regionId = getHongKongId();
        break;
    case BL_APLINFO_REGION_IN:
        regionId = getIndiaId();
        break;
    case BL_APLINFO_REGION_BR:
        regionId = getBrazilId(); 
        break;
    case BL_APLINFO_REGION_QG:
        regionId = getMiddleEastId();
        break;
    default:
        regionId = 0x0;
        VP_LOGD("Unsupported region id [%d]\n", region);
        break;
    }

    return (regionId + unique_id);
}

long DemoUtils::getChinaId()
{
    if (language == BL_LANGUAGE_TYPE_SIMPLIFIED_CHINESE) {
        return 0x41000000;
    }
    else if (language == BL_LANGUAGE_TYPE_TRADITIONAL_CHINESE) {
        return 0x43000000;
    }
    else if (language == BL_LANGUAGE_TYPE_ENGLISH) {
        return 0x11000000;
    }

    return 0x0;
}

long DemoUtils::getNorthAmericaId()
{
    if (language == BL_LANGUAGE_TYPE_ENGLISH) {
        return 0x11000000;
    }

    return 0x0;
}

long DemoUtils::getSouthEastAsiaId()
{
    if (language == BL_LANGUAGE_TYPE_THAI) {
        return 0x47000000;
    }
    else if (language == BL_LANGUAGE_TYPE_INDONESIAN) {
        return 0x48000000;
    }
    else if (language == BL_LANGUAGE_TYPE_ENGLISH) {
        return 0x12000000;
    }
    else if (language == BL_LANGUAGE_TYPE_VIETNAMESE) {
        return 0x49000000;
    }
    else if (language == BL_LANGUAGE_TYPE_MALAY) {
        return 0x4A000000;
    }

    return 0x0;
}

long DemoUtils::getTaiwanId()
{
    return 0x45000000;
}

long DemoUtils::getSouthAfricaId()
{
    return 0x12000000;
}

long DemoUtils::getOceaniaId()
{
    return 0x13000000;
}

long DemoUtils::getHongKongId()
{
    if (language == BL_LANGUAGE_TYPE_SIMPLIFIED_CHINESE) {
        return 0x42000000;
    }
    else if (language == BL_LANGUAGE_TYPE_TRADITIONAL_CHINESE) {
        return 0x44000000;
    }
    else if (language == BL_LANGUAGE_TYPE_ENGLISH) {
        return 0x12000000;
    }

    return 0x0;
}

long DemoUtils::getIndiaId()
{
    if (language == BL_LANGUAGE_TYPE_ENGLISH) {
        return 0x15000000;
    }
    else if (language == BL_LANGUAGE_TYPE_HINDI) {
        return 0x51000000;
    }

    return 0x0;
}

long DemoUtils::getBrazilId()
{
    if (language == BL_LANGUAGE_TYPE_ENGLISH) {
        return 0x12000000;
    }
    else if (language == BL_LANGUAGE_TYPE_SPANISH) {
        return 0x21000000;
    }
    else if (language == BL_LANGUAGE_TYPE_PORTUGUESE) {
        return 0x37000000;
    }

    return 0x0;
}

long DemoUtils::getMiddleEastId()
{
    if (language == BL_LANGUAGE_TYPE_ENGLISH) {
        return 0x14000000;
    }
    else if (language == BL_LANGUAGE_TYPE_ARABIC) {
        return 0x4B000000;
    }

    return 0x0;
}

VpPlaySoundList* DemoUtils::createDemoPlayList()
{
    VpPlaySoundList* soundList = new(MEM_Voice) VpPlaySoundList();
    if (nullptr == soundList) {
        VP_LOGD("create play list failed!");
        return nullptr;
    }

    VpPlaySoundList_VpPhrase* phrase1 = soundList->add_phrases();

    phrase1->set_type(VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_FILE);
    phrase1->set_filename("/etc/beep/BEEP1.wav");

    // phrase1->set_soundid(wrapSoundId(0x00101005));
    // phrase1->set_type(VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_ID);

    // phrase2->set_soundid(wrapSoundId(0x00101016));
    // phrase2->set_type(VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_ID);

    // phrase3->set_soundid(wrapSoundId(0x00101975));
    // phrase3->set_type(VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_ID);

//    ttsPhrase->set_text("To the destination, ");
//    ttsPhrase->set_ttstype(VpPlaySoundList_VpPhrase_VP_TTS_TYPE_VP_TTS_GENERAL);
//    ttsPhrase->set_type(VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_TTS);
  
//    phrase->set_text("it is about twenty minutes.");
//    phrase->set_ttstype(VpPlaySoundList_VpPhrase_VP_TTS_TYPE_VP_TTS_GENERAL);
//    phrase->set_type(VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_TTS);

    soundList->set_sender("VPDemo");
    soundList->set_userdata(0);
    soundList->set_needplayendnotificy(true);

    return soundList;
}

void DemoUtils::loadRegion()
{
    region = BL_AplInfo::GetRegion();
    VP_LOGD("Region is: [%d]\n", region);
}

void DemoUtils::loadLanguage()
{
    language = BL_SystemLanguage::GetVoiceLanguage();
    VP_LOGD("Language is: [%d]\n", language);
}

/* EOF */
