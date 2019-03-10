/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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

#include "VR_AudioDataWriter.h"
#include "VR_PortAudioMicWrapper.h"
#include "VR_Log.h"
#include "VR_InteractionManager.h"

#include "VC_WavFileWriter.h"

VR_AudioDataWriter::VR_AudioDataWriter(std::shared_ptr<VR_PortAudioMicWrapper> micWrapper)
: m_micWrapper(micWrapper)
{
    VR_LOGD_FUNC();
}

VR_AudioDataWriter::~VR_AudioDataWriter()
{
    VR_LOGD_FUNC();
}

ssize_t VR_AudioDataWriter::write(const void* buf, size_t nWords)
{
    if (nullptr != m_micWrapper) {
        m_micWrapper->WriteAudioDataToSDK(buf, nWords);       
    }
    else {
        VR_LOGI("m_micWrapper is nullptr !");
    }
    return nWords;
}

/* EOF */
