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
 * @file VR_PhoneBookListener.h
 * @brief Declaration file of class VR_PhoneBookListener.
 *
 * This file includes the declaration of class VR_PhoneBookListener.
 *
 * @attention used for C++ only.
 */
#ifndef VR_PHONE_BOOK_LISTENER_H
#define VR_PHONE_BOOK_LISTENER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef NCVARIANT_H
#   include "ncore/NCVariant.h"
#endif

#ifndef NCURI_H
#   include "ncore/NCUri.h"
#endif

#ifndef NCENVIRONMENT_H
#   include "system/NCEnvironment.h"
#endif

#ifndef NCCONTENTVALUES_H
#   include "dbhelper/NCContentValues.h"
#endif

#ifndef NCCONTENTRESOLVER_H
#   include "contentresolver/NCContentResolver.h"
#endif

#ifndef NCSQLITEOPENHELPER_H
#   include "dbhelper/NCSQLiteOpenHelper.h"
#endif

#ifndef CXX_BL_DATA_STORAGE_H
#include "BL_DataStorage.h"
#endif

#include "BL_File.h"
#include "VR_ConfigureIF.h"
#include "VR_PhoneBookXMLFormat.h"
#include "CL_SyncObj.h"

class sqlite3;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_CommandHandleIF);

namespace navi
{
namespace dataprovider
{
    /**
     * @brief The VR_PhoneBookContextHelper class
     *
     * class declaration
     */
    class VR_PhoneBookContextHelper
    {
    public:
        enum emNotifyToBT
        {
            Notify_Default,
            Notify_NeedSend,
        };

    public:
        VR_PhoneBookContextHelper();
        ~VR_PhoneBookContextHelper();

        void SetPhoneBookMd5(VR_String &chr);
        VR_String GetPhoneBookMd5();

        void SetEmNotifyToBT(emNotifyToBT rhs);
        emNotifyToBT GetEmNotifyToBT();

        bool IsMd5Equal();

        bool UpdateDB(const VrPhoneBookDataList &personNode, const int count);
        void loadPhoneType();

        VR_String getPbMd5FromBt();
        VR_String getDBPath();
        VR_String getDeviceAddr();

        bool copyDB();

    public:
        bool             m_bLazyService;
        emNotifyToBT     m_emNotifyToBt;

    protected:
        bool checkFilePath();

    private:
        VR_PhoneBookContextHelper(const VR_PhoneBookContextHelper&);
        VR_PhoneBookContextHelper& operator=(const VR_PhoneBookContextHelper&);
        void getDevice();
        nutshell::NCString getPhoneType(int iPhoneType);
        void getDBName(VR_String& strName);

        BL_DataStorage  m_cDataStorage;
        VoiceMap<int, std::string>::type m_mapPhoneType;
        VR_String m_strDBPath;
        VR_String m_strTmpDBPath;
        VR_String m_strDeviceAddr;
    };

    /**
     * @brief The VR_PhoneBookListener class
     *
     * class declaration
     */
    class VR_PhoneBookListener
    {
    public:
        static VR_PhoneBookListener* GetInstance();
        static VR_PhoneBookListener* Assign();
        static void Dispose();

        void Initialize();
        void onNotifyUpdate(nutshell::NCUri uri, nutshell::NCVariant variant);

        bool getDBPath(VR_String& strDBPath);
        bool getPhoneNumber(int iContactId, myPersonInfo& personInfo);
        bool notifyPhoneFinishedData();

    // the workspace if only DCU
    protected:
        void InitializeDefault();
        void onNotifyUpdateDefault(nutshell::NCUri &uri, nutshell::NCVariant &variant);

        bool getContactDataDefault();
        void sendNotifyToBt();
        bool GetPhoneBookInfo();
        void SendMsg2DE(const VR_String &msg);

    protected:
        void sendDBPath();
        bool isPhoneConnected();

    private:
        VR_PhoneBookContextHelper     m_PBContextHelper;
        VR_String m_strDBPath;
        CL_SyncObj m_cSync;

    private:
        VR_PhoneBookListener();
        ~VR_PhoneBookListener();
        VR_PhoneBookListener(const VR_PhoneBookListener&);
        VR_PhoneBookListener& operator=(const VR_PhoneBookListener&);
        static VR_PhoneBookListener*  m_sPBInstance;

        void sendEventToNATP(const VR_String& strEvent);
    };

} // dataprovider
} // navi

#endif /* VR_PHONE_BOOK_LISTENER_H */
/* EOF */
