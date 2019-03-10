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

#include "VR_DP_OB_Action.h"

namespace navi
{
namespace dataprovider
{

VR_DP_OB_Action::VR_DP_OB_Action(nutshell::NCUri uri, nutshell::NCVariant variant)
    : m_uri(uri)
    , m_variant(variant)
{

}

VR_DP_OB_Action::~VR_DP_OB_Action()
{

}

void VR_DP_OB_Action::setUriAndVariant(nutshell::NCUri &uri, nutshell::NCVariant &variant)
{
    m_uri = uri;
    m_variant = variant;
}

}
}

/* EOF */
