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
#include "VR_TaskFactoryIF.h"
#include "VR_ConfigureIF.h"
#include "VR_TaskFactory.h"
// #include "VR_TaskFactory_VrUnitAgent.h"

spVR_TaskFactoryIF
VR_TaskFactoryIF::getTaskFactory()
{
    spVR_TaskFactoryIF spTaskFactory(VR_new VR_TaskFactory());
    return spTaskFactory;
}

/* EOF */
