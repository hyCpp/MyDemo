/**
 * Copyright @ 2013 - 2015 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_NCMonitor.h
 *
 * @brief monitor usb
 *
 * This class monitor the usb event
 *
 * @attention used for C++ only
 */

#ifndef VR_NCMONITOR_H
#define VR_NCMONITOR_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef NCUSBMANAGER_H
#include <connectmanager/NCUsbManager.h>
#endif

#ifndef NCUSBSTORAGEREPLIER_H
#include <connectmanager/NCUsbStorageReplier.h>
#endif

#ifndef NCSYNCOBJ_H
#include <ncore/NCSyncObj.h>
#endif

#ifndef NCLIST_H
#include <ncore/NCList.h>
#endif

#ifndef NCSTRING_H
#include <ncore/NCString.h>
#endif

#ifndef VR_MACRO_H
#include "VR_Macro.h"
#endif

enum VR_VBTLogSwithType
{
    VR_VBTLogSwithType_OFF = 0,
    VR_VBTLogSwithType_ON,
    VR_VBTLogSwithType_NoChange
};

class VR_USBMoniter;
typedef nutshell::ncsp<nutshell::NCUsbManager>::sp spNCUsbManager;

#define VR_VBTLOG_START_FILE "/vbtsdk_start_4d6fee22-a48d-11e5-a9a7-080027bd3c87.bin"
#define VR_VBTLOG_END_FILE "/vbtsdk_end_d6b76f4e-a491-11e5-9f14-080027bd3c87.bin"

/**
 * monitor the usb event
 *
 * This class monitor the usb event
 *
 *
 */
class VR_NCMonitor : public nutshell::NCUsbStorageReplier
{
public:
    /**
     * VR_NCMonitor
     *
     * Constractor
     *
     * @param [IN] usbManager : ncsp<NCUsbManager>::sp
     *
     * @return none
     */
    VR_NCMonitor(spNCUsbManager usbManager, VR_USBMoniter* pcCallBack);

    /**
     * ~NMGracenoteStub
     *
     * Destoractor
     *
     * @param : none
     *
     * @return none
     */
    virtual ~VR_NCMonitor();

    /**
     * onMounted
     *
     * This notify is used to inform other modules when a storage device is mounted
     *
     * @param [IN] id : The ID of a USB device. It will be "usb1", "usb2".
     *
     * @return none
     */
    virtual VOID onMounted(const nutshell::NCString& id);

    /**
     * onUnmounted
     *
     * This notify is used to inform other modules when a storage device is unmounted.
     *
     * @param [IN] id : The ID of a USB device. It will be "usb1", "usb2".
     *
     * @return none
     */
    virtual VOID onUnmounted(const nutshell::NCString& id);

    /**
     * onBadRemoval
     *
     * This notify is used to inform other modules when a storage device is bad removal
     *
     * @param [IN] id : The ID of a USB device. It will be "usb1", "usb2".
     *
     * @return none
     */
    virtual VOID onBadRemoval(const nutshell::NCString& id);

    /**
     * onConnected
     *
     * service connected
     *
     * @param : none
     *
     * @return none
     */
    virtual VOID onConnected();

private:
    spNCUsbManager m_usbManager;
    VR_USBMoniter* m_pcUsbMoniterCallBack;
    VR_VBTLogSwithType checkVBTLogSwitch(nutshell::NCString usb1path);

private:
    /// copy forbidden
    VR_NCMonitor(const VR_NCMonitor&);

    /// equal forbidden
    VR_NCMonitor& operator=(const VR_NCMonitor&);
};

typedef nutshell::ncsp<VR_NCMonitor>::sp spVRUsbMoniter;

#endif /* VR_NCMONITOR_H */
/* EOF */
