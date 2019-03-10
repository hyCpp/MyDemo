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
#include "VR_MusicListener.h"
#include "VR_DP_OB_Action_Music.h"
#include "VR_DataProviderComm.h"
#include "MediaProvider/NMMediaUri.h"
#include "MediaInfoProvider/NMMediaInfoUri.h"

using namespace nutshell;
using namespace media;
namespace navi
{
namespace dataprovider
{
VR_DP_OB_Action_Music::VR_DP_OB_Action_Music(NCUri                &uri,
                                             NCVariant            &variant)
    : VR_DP_OB_Action(uri, variant)
{
    VR_LOGD_FUNC();
}

VR_DP_OB_Action_Music::~VR_DP_OB_Action_Music()
{
}

void VR_DP_OB_Action_Music::DoUpdatePrepare()
{
    VR_MusicListener* listener = VR_MusicListener::GetInstance();
    bool result = listener->onNotifyUpdate(m_uri, m_variant);
    if (!result) {
        VR_LOGD("music listener onNotifyUpdate failed");
        return;
    }
}

}
}
/* EOF */
