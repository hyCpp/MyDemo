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
 * @file VR_PhoneBookStateListener.h
 * @brief Declaration file of class VR_PhoneBookStateListener.
 *
 * This file includes the declaration of class VR_PhoneBookStateListener.
 *
 * @attention used for C++ only.
 */

#ifndef VR_PhoneBookStateListener_H
#define VR_PhoneBookStateListener_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef NCVARIANT_H
#    include "ncore/NCVariant.h"
#endif

#ifndef NCURI_H
#    include "ncore/NCUri.h"
#endif

#ifndef NCTYPESDEFINE_H
#    include "ncore/NCTypesDefine.h"
#endif

#include "CL_SyncObj.h"

namespace navi
{
namespace dataprovider
{

    /**
     * @brief The VR_PhoneBookStateListener class
     *
     * class declaration.
     */
    class VR_PhoneBookStateListener
    {
    public:
        VR_PhoneBookStateListener();
        virtual ~VR_PhoneBookStateListener();

        static VR_PhoneBookStateListener* GetInstance();
        static VR_PhoneBookStateListener *Assign();
        static void Dispose();

        bool Initialize();
        virtual void onNotifyUpdate(nutshell::NCUri uri, nutshell::NCVariant variant);
        bool getPhoneStatus(VR_String& strStatus);
        bool getSignalStatus(VR_String& strStatus);
        bool getDeviceId(VR_String& strDevice);
        bool setSmsSupport(const VR_String& strValue);

    private:
        bool handlePhoneState();
        bool handlePhoneConnect();

        bool getPhoneConnectStatus(VR_String& strStatus);
        bool getPhoneSignalStatus(VR_String& strStatus);
        bool checkSignalStatus(const VR_String& strValue);
        void sendPhoneConnectEvent();

        bool getPhoneDeviceId(VR_String& strDeviceId);

        bool getURIValue(const VR_String& strURI, VR_String& strValue);
        void sendEventToNATP(const VR_String& strEvent);
        void getDevice();
        void SendMsg2DE(const VR_String &msg);

    private:
        bool                    m_bPhoneConnected;
        bool                    m_bSMSSupported;
        bool                    m_bPhoneInService;
        VR_String               m_strDeviceAddr;
        CL_SyncObj              m_cSync;
        static VR_PhoneBookStateListener* m_instance;

    private:
        VR_DISALLOW_EVIL_CONSTRUCTORS(VR_PhoneBookStateListener);

    };

}
}

#endif // VR_PhoneBookStateListener_H
/* EOF */
