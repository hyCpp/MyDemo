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
#include "VR_Log.h"
#include "VR_Command.h"

VR_Command::VR_Command(VR_ProtoMessageBase*  pMsg)
    : m_pMessage(pMsg)
{

}

VR_Command::~VR_Command()
{
    if (NULL != m_pMessage) {    //  In child thread , we can delete m_pMessage, To prevent a memory leak
        delete m_pMessage;
        m_pMessage = NULL;
    }
}
/* EOF */
