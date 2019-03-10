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
#include "VC_Factory.h"

#include "VC_AudioSessionAlsa.h"
#include "VC_IAudioInFile.h"
#include "VC_IAudioInMedia.h"
#include "VC_IAudioOutMedia.h"
#include "VC_DecoderOgg.h"
// #include "VC_DecoderTts.h"
#include "VC_AudioInGeneral.h"
#include "VC_AudioOutGeneral.h"
#include "VC_PlayerGeneral.h"
// #include "NMVmostCommonInc.h"


VC_Factory* VC_Factory::s_pInstance = NULL;

VC_Factory* VC_Factory::Instance()
{
    if (NULL == s_pInstance) {
        s_pInstance = new(MEM_Voice) VC_Factory();
    }
    return s_pInstance;
}

void VC_Factory::Destroy()
{
    if (NULL != s_pInstance) {
        delete s_pInstance;
        s_pInstance = NULL;
    }
}

VC_Factory::VC_Factory() : m_pAudioSession(NULL)
{
}

VC_Factory::~VC_Factory()
{
   delete m_pAudioSession;
}

VC_AudioSession* VC_Factory::CreateAudioSession(VC_MODULE_TYPE moduleType) 
{
  switch (moduleType) {
  case  VC_MODULE_NAVI:
      // return new(MEM_Voice) VC_AudioSessionAlsa(NM_VMOST_INSTID_NAVI_NAVI);
      break;
  case VC_MODULE_TTS:
      // return new(MEM_Voice) VC_AudioSessionAlsa(NM_VMOST_INSTID_NAVI_TTS);
      break;
  case VC_MODULE_VR:
      // return new(MEM_Voice) VC_AudioSessionAlsa(NM_VMOST_INSTID_NAVI_VR);
      break;
  default:
      // return new(MEM_Voice) VC_AudioSessionAlsa(NM_VMOST_INSTID_NAVI_NAVI);
      break;
  }
  RETURN_NULL;
}

VC_IAudioIn* VC_Factory::CreateIAudioIn(bool mock)
{
    if (mock) {
        return new(MEM_Voice) VC_IAudioInFile();
    }
    else {
        return new(MEM_Voice) VC_IAudioInMedia();
    }
    
}

VC_IAudioOut* VC_Factory::CreateIAudioOut(VC_AUDIO_TYPE audioType)
{
    return new(MEM_Voice) VC_IAudioOutMedia();
}

VC_AudioIn* VC_Factory::CreateAudioIn(bool mock)
{
    return new(MEM_Voice) VC_AudioInGeneral(mock);
}

VC_AudioOut* VC_Factory::CreateAudioOut(VC_AUDIO_TYPE audioType)
{
    return new(MEM_Voice) VC_AudioOutGeneral(audioType);
}

VC_Decoder* VC_Factory::CreateDecoder(VC_AUDIO_TYPE audioType, VC_DECODER_TYPE decType)
{
    switch (decType) {
    case VC_DECODER_OGG:
        return new(MEM_Voice) VC_DecoderOgg();
    case VC_DECODER_TTS:
        // return (VC_AUDIO_BEEP==audioType) ? NULL: new(MEM_Voice) VC_DecoderTts();
    default:
        RETURN_NULL;
    }
}

VC_Player* VC_Factory::CreatePlayer(VC_AUDIO_TYPE audioType)
{
    return new(MEM_Voice) VC_PlayerGeneral(audioType);

}
/* EOF */
