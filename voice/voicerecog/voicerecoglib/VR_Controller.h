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
 * @file VR_Controller.h
 * @brief Declaration file of class VR_Controller.
 *
 * This file includes the declaration of class VR_Controller.
 *
 * @attention used for C++ only.
 */

#ifndef VR_CONTROLLER_H
#define VR_CONTROLLER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_CONTROLLABLEIF_H
#   include "VR_ControllableIF.h"
#endif

#ifndef VR_DM_COMMANDHANDLE_H
#   include "VR_DM_CommandHandle.h"
#endif

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_MessageProcess);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ReceiverCb);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DM_CommandHandle);

/**
 * @brief The VR_Controller class
 *
 * class declaration
 */
class VR_Controller : public VR_ControllableIF
{
public:
    VR_Controller();
    virtual ~VR_Controller();

public:
    virtual void Initialize();
    virtual void Start();
    virtual void Stop();
    virtual void Destory();

protected:
    spVR_MessageProcess         m_spMessageProcess;
    spVR_ReceiverCb             m_spReceiverCb;
    spVR_DM_CommandHandle       m_spDMCommandHandler;
//    spVR_AvcListenerIF          m_spAvcListener;
//    spVR_AvcLanSenderIF         m_spAvcSender;
//    spVR_AvcUnit                m_spAvcUnit;

private:
    // disable the copy constructor
    VR_DISALLOW_EVIL_CONSTRUCTORS(VR_Controller);
};

#endif /* VR_CONTROLLER_H */
/* EOF */
