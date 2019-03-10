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
#include "VR_EUDialogEngine.h"
#include "VR_InitPersonDate.h"
#include <boost/bind.hpp>
#include <boost/assign/list_of.hpp>
#include "uscxml/messages/SendRequest.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <sstream>

#include <ncore/NCStartPerformance.h>


using namespace nutshell;
using namespace std;
using namespace N_Vr;
using namespace N_Asr;

VR_InitPersonDate::VR_InitPersonDate()
{
    m_taskComplete = false;
}

VR_InitPersonDate::~VR_InitPersonDate()
{
}

bool VR_InitPersonDate::execute(VR_EUDialogEngine* engine)
{
    VR_LOGD_FUNC();
    bool stopSucess = engine->stopCurrentDialog();

    if (stopSucess) {
        engine->initializeAsrPersonalData();
        m_taskComplete = true;
    }
    else {
        m_taskComplete = false;
    }
    return m_taskComplete;
}

bool VR_InitPersonDate::onEvent(VR_EUDialogEngine* engine, uscxml::Event evt)
{
    VR_LOGD_FUNC();
    if (VR_ASR_EVENT_NAME == evt.getName() && VR_ASR_EVT_END == evt.getRaw()) {
        VR_LOGD("Clear User Date, And Init Asr");
        engine->initializeAsrPersonalData();
        m_taskComplete = true;
    }

    return true;
}

bool VR_InitPersonDate::isFinished()
{
    return m_taskComplete;
}

/* EOF */
