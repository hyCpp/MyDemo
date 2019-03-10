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
#include "MEM_map.h"
#include <sstream>
#include <time.h>
#include "ncore/NCDir.h"
#include "ncore/NCFile.h"
#include <boost/format.hpp>
#include <boost/make_shared.hpp>
#include <algorithm>
#include <string>

#include "VR_DialogEngineIF.h"
#include "VR_PB_XmlBuildHelper.h"
#include "VR_PhoneBookListener.h"
#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"

#include "nceventsys/NCEventHandler.h"
#include "nceventsys/NCEvSysContext.h"

#include "navi/Search/ReqUpdateSpotlightData.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "NCBtSettingProxy.h"
#include "VR_XmlStrParse.h"
#include "VR_Indentifier.hpp"
#include "CL_AutoSync.h"

#include "VR_ConfigureIF.h"
#include "VR_ContentProvider.h"

using namespace navi::VoiceRecog;
using namespace nutshell;
using namespace navi::Search;

namespace navi
{
namespace dataprovider
{
   
const nutshell::NCString VR_PB_SPACE     =  nutshell::NCString("vrservice_pbnotify");
const nutshell::NCString VR_PB_IDENTITY  =  nutshell::NCString("NCVrPb_Notify");
const VR_String VR_TMP_DB_NAME = "VUIPhone_tmp.db";

template<class T> void toString(std::string& result, const T& param)
{
    std::stringstream ss;
    ss << param;
    result = ss.str();
}

VR_PhoneBookContextHelper::VR_PhoneBookContextHelper()
    : m_bLazyService(false)
    , m_emNotifyToBt(Notify_Default)
{

}

VR_PhoneBookContextHelper::~VR_PhoneBookContextHelper()
{

}

void VR_PhoneBookContextHelper::SetPhoneBookMd5(VR_String &chr)
{
    if (chr.empty()) {
        return;
    }

    CL_BOOL ret = m_cDataStorage.PutValue(BL_DATA_KEY_VR_PBMD5, chr.c_str(), VR_PB_UNSIGN_INT_MD5_MAXLEN);
    if (!ret) {
        VR_LOGD("[-- set pb-md5 failed --]");
    }
}

VR_String VR_PhoneBookContextHelper::GetPhoneBookMd5()
{
    char result[VR_PB_UNSIGN_INT_MD5_MAXLEN + 1] = {};

    BL_DataValue data;
    CL_BOOL ret = m_cDataStorage.GetValue(BL_DATA_KEY_VR_PBMD5, data);
    if (ret) {
        int size = data.GetSize();
        memcpy(result, data.GetData(), size);
    }

    VR_String strMd5(result);
    VR_LOGD("[-- get md5 from db <%s> --]", strMd5.c_str());
    return strMd5;
}

void VR_PhoneBookContextHelper::SetEmNotifyToBT(VR_PhoneBookContextHelper::emNotifyToBT rhs)
{
    VR_LOGD("[-- m_emNotifyToBt=[%d],rhs=[%d] --]", m_emNotifyToBt, rhs);
    m_emNotifyToBt = rhs;
}

VR_PhoneBookContextHelper::emNotifyToBT VR_PhoneBookContextHelper::GetEmNotifyToBT()
{
    VR_LOGD("[-- m_emNotifyToBt=[%d] --]", m_emNotifyToBt);
    return m_emNotifyToBt;
}

bool VR_PhoneBookContextHelper::IsMd5Equal()
{
    VR_String strNew = getPbMd5FromBt();
    VR_String strOld = GetPhoneBookMd5();
    VR_LOGD("[-- GetPhoneBookMd5<%s>, getPbMd5FromBt<%s> --]", strOld.c_str(), strNew.c_str());

    if (strNew == strOld) {
        return true;
    }

    return false;
}

bool VR_PhoneBookContextHelper::UpdateDB(const VrPhoneBookDataList &personNode, const int count)
{
    VR_LOGD_FUNC();

    VR_LOGD("update DB contact number : %d", count);
    bool resul = checkFilePath();
    if (!resul) {
        VR_LOGD("Error : DB path is not exist!");
        return false;
    }

    // 1 remove old file
    VR_String strDBName;
    m_strTmpDBPath = VR_ConfigureIF::Instance()->getUsrPath() + VR_TMP_DB_NAME;
    resul = (NC_TRUE == NCFile::exists(NCString(m_strTmpDBPath.c_str())));
    if (resul) {
        NCFile::remove(NCString(m_strTmpDBPath.c_str()));
        VR_LOGD("remove the old DB (%s) !", m_strTmpDBPath.c_str());
    }
    VR_LOGD("DB path : %s", m_strTmpDBPath.c_str());

    // 2 create and open new db
    NCString path(m_strTmpDBPath.c_str());
    NCSQLiteDatabase db(path);
    if (NC_FALSE == db.isOpen()) {
        VR_LOGD("Error : Open DB failed!");
        return false;
    }

    /// 3 create table
    {
        // DeviceInformation
        NCString strCreateDbSQL = VR_PB_SQL_CREAT_TABLE_DEVICE;
        INT result = db.execSQL(strCreateDbSQL);
        if (0 != result) {
            db.close();
            VR_LOGD("[-- create DeviceInformation-table failed --]");
            return false;
        }

        // Contact
        strCreateDbSQL = VR_PB_SQL_CREAT_TABLE_CONTACTS;
        result = db.execSQL(strCreateDbSQL);
        if (0 != result) {
            db.close();
            VR_LOGD("[-- create Contact-table failed --]");
            return false;
        }

        // PhoneType
        strCreateDbSQL = VR_PB_SQL_CREAT_TABLE_PHONETYPE;
        result = db.execSQL(strCreateDbSQL);
        if (0 != result) {
            db.close();
            VR_LOGD("[-- create PhoneType-table failed --]");
            return false;
        }

        // PhoneRecords
        strCreateDbSQL = VR_PB_SQL_CREAT_TABLE_PHONERECORDS;
        result = db.execSQL(strCreateDbSQL);
        if (0 != result) {
            db.close();
            VR_LOGD("[-- create PhoneRecords-table failed --]");
            return false;
        }
    }

    /// 4 insert data
    {
        VR_LOGD("Start to load Data");
        // DeviceInformation
        {
            getDevice();
            NCDBDataTable ncDevice(1, 2);
            ncDevice.setColumnType(0, NCDBDataType_String);
            ncDevice.setColumnType(1, NCDBDataType_String);
            NCString strDevice(m_strDeviceAddr.c_str());
            ncDevice.putString(0, 0, strDevice.getString(), strDevice.getLength());
            ncDevice.putString(0, 1, strDevice.getString(), strDevice.getLength());
            db.fastBatchInsertOrUpdate(VR_PB_SQL_INSERT_INFO_INTO_TABLE_DEVICE, ncDevice);
            ncDevice.reset();
        }

        // PhoneType
        {
            NCDBDataTable ncPhoneType(4, 4);
            ncPhoneType.setColumnType(0, NCDBDataType_Integer);
            ncPhoneType.setColumnType(1, NCDBDataType_String);

            NCString ncstrPhoneType("");
            for (int iRow = 0; iRow < 4; ++iRow) {
                ncPhoneType.putLong(iRow, 0, iRow);
                ncstrPhoneType = getPhoneType(iRow);
                ncPhoneType.putString(iRow, 1, ncstrPhoneType.getString(), ncstrPhoneType.getLength());
                ncstrPhoneType = getPhoneType(iRow + 4);
                ncPhoneType.putString(iRow, 2, ncstrPhoneType.getString(), ncstrPhoneType.getLength());
                ncstrPhoneType = getPhoneType(iRow + 8);
                ncPhoneType.putString(iRow, 3, ncstrPhoneType.getString(), ncstrPhoneType.getLength());
            }
            db.fastBatchInsertOrUpdate(VR_PB_SQL_INSERT_INFO_INTO_TABLE_PHONETYPE, ncPhoneType);
            ncPhoneType.reset();
        }

        size_t personNodeItems = personNode.size();
        if (0 != personNodeItems) {
            NCDBDataTable ncContacts((UINT)personNodeItems, 3);
            ncContacts.setColumnType(0, NCDBDataType_Integer);
            ncContacts.setColumnType(1, NCDBDataType_String);
            ncContacts.setColumnType(2, NCDBDataType_String);
            int rowNum = 0;

            NCDBDataTable ncPhoneNums(count, 4);
            ncPhoneNums.setColumnType(0, NCDBDataType_Integer);
            ncPhoneNums.setColumnType(1, NCDBDataType_Integer);
            ncPhoneNums.setColumnType(2, NCDBDataType_Integer);
            ncPhoneNums.setColumnType(3, NCDBDataType_String);
            int rowNum_PN = 0;

            for (auto it = personNode.begin(); it != personNode.end(); ++it) {
                std::shared_ptr<myPersonInfo> personInfo = *it;

                ncContacts.putLong(rowNum, 0, personInfo->contactId);
                ncContacts.putString(rowNum, 1, personInfo->firstName.getString(), personInfo->firstName.getLength());
                ncContacts.putString(rowNum, 2, personInfo->lastName.getString(), personInfo->lastName.getLength());

                for (auto itNumber = personInfo->lstPhoneNum.cbegin();
                    itNumber != personInfo->lstPhoneNum.cend();
                    ++itNumber) {
                    ncPhoneNums.putLong(rowNum_PN, 0, rowNum_PN + 1);
                    ncPhoneNums.putLong(rowNum_PN, 1, personInfo->contactId);
                    ncPhoneNums.putLong(rowNum_PN, 2, itNumber->type);
                    ncPhoneNums.putString(rowNum_PN, 3, itNumber->number.getString(),
                                        itNumber->number.getLength());
                    ++rowNum_PN;
                }
                ++rowNum;
            }

            // 4.3 load contacts data
            db.fastBatchInsertOrUpdate(VR_PB_SQL_INSERT_INFO_INTO_TABLE_CONTACT, ncContacts);
            ncContacts.reset();

            // 4.4 load phone number data
            db.fastBatchInsertOrUpdate(VR_PB_SQL_INSERT_INFO_INTO_TABLE_PHONERECORDS, ncPhoneNums);
            ncPhoneNums.reset();
        }
        VR_LOGD("End to load data");
    }
    
    // 5 close db
    db.close();
    return true;
}

void VR_PhoneBookContextHelper::getDBName(VR_String& strName)
{
    char buffer[100] = { 0 };
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    snprintf(buffer, sizeof(buffer), "%04d%02d%03d",
        (int)(ts.tv_sec/60), (int)(ts.tv_sec%60), (int)(ts.tv_nsec/1000000));

    getDevice();
    strName = VR_String(VR_PB_DB_NAME_VR_CONTACTS) + m_strDeviceAddr + "_" + VR_String(buffer) + ".db";
}

void VR_PhoneBookContextHelper::getDevice()
{
    m_strDeviceAddr = "";
    nutshell::NCBtSettingProxy btSettingProxy;
    UINT64 unitAddr;
    btSettingProxy.getMainPhoneDeviceAddr(unitAddr);

    char buffer[100] = { 0 };
    sprintf(buffer, "%x", unitAddr);
    m_strDeviceAddr = VR_String(buffer);
    transform(m_strDeviceAddr.begin(), m_strDeviceAddr.end(), m_strDeviceAddr.begin(), toupper);
    VR_LOGD("BT address : %s", m_strDeviceAddr.c_str());
}

NCString VR_PhoneBookContextHelper::getPhoneType(int iPhoneType)
{
    std::string strPhoneType = "";
    auto iter = m_mapPhoneType.find(iPhoneType);
    if (iter != m_mapPhoneType.cend()) {
        strPhoneType = iter->second;
    }
    VR_LOGD("phone type : %s", strPhoneType.c_str());
    NCString ncStrPhoneType(strPhoneType.c_str());
    return ncStrPhoneType;
}

void VR_PhoneBookContextHelper::loadPhoneType()
{
    m_mapPhoneType.clear();
    m_mapPhoneType.insert(std::make_pair(0, "Mobile"));
    m_mapPhoneType.insert(std::make_pair(1, "Home"));
    m_mapPhoneType.insert(std::make_pair(2, "Work"));
    m_mapPhoneType.insert(std::make_pair(3, "Other"));
    m_mapPhoneType.insert(std::make_pair(4, "移动电话"));
    m_mapPhoneType.insert(std::make_pair(5, "家"));
    m_mapPhoneType.insert(std::make_pair(6, "办公室"));
    m_mapPhoneType.insert(std::make_pair(7, "其它"));
    m_mapPhoneType.insert(std::make_pair(8, "手機號碼"));
    m_mapPhoneType.insert(std::make_pair(9, "住宅"));
    m_mapPhoneType.insert(std::make_pair(10, "公司"));
    m_mapPhoneType.insert(std::make_pair(11, "其它"));
}

VR_String VR_PhoneBookContextHelper::getPbMd5FromBt()
{
    VR_String rhs("");
    NCString nullStr;
    NCObjList<NCString> nullList;
    NCContentResolver resolver;
    NCUri uri(NCString(VR_PB_CONTACT_MD5_CODE_URI));
    ncsp<NCCursor>::sp queryResult = resolver.query(uri, nullList, nullStr, nullList, nullStr);

    if (NULL == queryResult.get()) {
        VR_LOGD("[-- getPbMd5FromBt: queryResult.get()=NULL --]");
    }
    else if (0 == queryResult->getCount()) {
        VR_LOGD("[-- getPbMd5FromBt: queryResult->getCount()=0 --]");
    }
    else if (queryResult->moveToFirst()) {
        rhs = queryResult->getString(0).getString();
    }
    else {
        VR_LOGD("[-- getPbMd5FromBt: queryResult->moveToFirst()=false --]");
    }

    VR_LOGD("[-- getPbMd5FromBt: (%s) --]", rhs.c_str());
    return rhs;
}

bool VR_PhoneBookContextHelper::checkFilePath()
{
    NCDir rootPath;
    VR_ConfigureIF* vrConfig = VR_ConfigureIF::Instance();
    if (NULL == vrConfig) {
        VR_LOGD("[-- VR_ConfigureIF instance is null --]");
        return false;
    }

    NCString strWorkPath = vrConfig->getUsrPath().c_str();
    bool result = (NC_TRUE == rootPath.exists(strWorkPath));
    if (!result) {
        VR_LOGD("[-- %s path is not exist --]", strWorkPath.getString());
        return false;
    }

    return true;
}

VR_String VR_PhoneBookContextHelper::getDBPath()
{
    return m_strDBPath;
}

bool VR_PhoneBookContextHelper::copyDB()
{
    if (m_strTmpDBPath == "") {
        m_strTmpDBPath = VR_ConfigureIF::Instance()->getUsrPath() + VR_TMP_DB_NAME;
    }

    VR_String strDBName;
    getDBName(strDBName);
    m_strDBPath = VR_ConfigureIF::Instance()->getUsrPath() + strDBName;

    return (NC_TRUE == NCFile::copy(m_strTmpDBPath.c_str(), m_strDBPath.c_str()));
}

VR_String VR_PhoneBookContextHelper::getDeviceAddr()
{
    return m_strDeviceAddr;
}

VR_PhoneBookListener* VR_PhoneBookListener::m_sPBInstance = NULL;

VR_PhoneBookListener::VR_PhoneBookListener()
{
    VR_LOGD_FUNC();
}

VR_PhoneBookListener::~VR_PhoneBookListener()
{

}

VR_PhoneBookListener *VR_PhoneBookListener::GetInstance()
{
    return(m_sPBInstance);
}

VR_PhoneBookListener *VR_PhoneBookListener::Assign()
{
    if (NULL == m_sPBInstance) {
        m_sPBInstance = VR_new VR_PhoneBookListener();
    }
    return(m_sPBInstance);
}

void VR_PhoneBookListener::Dispose()
{
    if (NULL != m_sPBInstance) {
        delete m_sPBInstance;
        m_sPBInstance = NULL;
    }
}

void VR_PhoneBookListener::Initialize()
{
    VR_LOGD_FUNC();
    InitializeDefault();
}

void VR_PhoneBookListener::onNotifyUpdate(NCUri uri, NCVariant variant)
{
    VR_LOGD_FUNC();
    onNotifyUpdateDefault(uri, variant);
}

bool VR_PhoneBookListener::GetPhoneBookInfo()
{
    VR_LOGD_FUNC();
    NCList<NCString>  ncColumns;
    ncColumns << &VR_PB_QUERY_CONTACTS_INFO_INDEX__ID
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_CONTACT_ID
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_F_NAME
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_L_NAME
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_1
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_1
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_1
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_2
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_2
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_2
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_3
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_3
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_3
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_4
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_4
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_4
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_5
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_5
              << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_5;

    NCContentResolver  ncResolver;
    NCString  ncNull;
    NCCursor*  ncCursor = ncResolver.query(NCUri(VR_PB_CONTACT_DATA_INFO_URI), ncColumns, ncNull, NCList<NCString>(), ncNull);
    if (NULL == ncCursor) {
        VR_LOGD("query phonebook info failed.[cursor null]");
        return false;
    }

    int nRecordNum = ncCursor->getCount();
    VR_LOGD("phone book recorder: [%d]", nRecordNum);
    if (nRecordNum <= 0) {
        return true;
    }

    NC_BOOL ncRet = ncCursor->moveToFirst();
    if (NC_FALSE == ncRet) {
        VR_LOGD("cursor move to first failed.");
        return false;
    }

    std::unique_ptr<ReqUpdateSpotlightData>  FullMusicData(VR_new  ReqUpdateSpotlightData);
    FullMusicData->set_data_type(SearchDefine_SPOTLIGHT_SOURCE_TYPE_PHONE);
    FullMusicData->set_update_type(ReqUpdateSpotlightData_Source_Update_Type_FULLUPDATE);
    EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQUPDATESPOTLIGHTDATA, FullMusicData.release());

    std::unique_ptr<ReqUpdateSpotlightData>  MusicData(VR_new  ReqUpdateSpotlightData);
    MusicData->set_data_type(SearchDefine_SPOTLIGHT_SOURCE_TYPE_PHONE);
    MusicData->set_update_type(ReqUpdateSpotlightData_Source_Update_Type_ADD);

    for (int nIndex = 0; nIndex < nRecordNum; ++nIndex) {
        SearchDefine_Source_PHONE_DATA* pPhoneData = MusicData->add_phone_data();
        INT64 nContactID = ncCursor->getInt64(CONTACT_TABLE_FIELD_CONTACT_ID);
        pPhoneData->set_contact_id(nContactID);
        NCString ncFirstName = ncCursor->getString(CONTACT_TABLE_FIELD_FIRSTNAME);
        pPhoneData->set_first_name(ncFirstName);
        NCString ncLastName = ncCursor->getString(CONTACT_TABLE_FIELD_LASTNAME);
        pPhoneData->set_last_name(ncLastName);

        NCString ncPhoneNum("");
        for (int iNumIndex = 0; iNumIndex < cstPhoneNum; ++iNumIndex) {
            ncPhoneNum = ncCursor->getString(CONTACT_TABLE_FIELD_PHONE_NUMBER + iNumIndex * cstNumberTypeCount);
            if (ncPhoneNum.getLength() > 0) {
                INT64 nPhoneType = ncCursor->getInt64(CONTACT_TABLE_FIELD_PHONE_TYPE + iNumIndex * cstNumberTypeCount);
                std::string strPhoneType("");
                toString(strPhoneType, nPhoneType);
                SearchDefine_Source_PHONE_LIST* list = pPhoneData->add_phone_list();
                list->set_phone_type(strPhoneType);
                list->set_phone_number(ncPhoneNum.getString());
            }
        }

        if (NC_TRUE == ncCursor->isLast()) {
            break;
        }
        ncCursor->moveToNext();
    }

    EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQUPDATESPOTLIGHTDATA, MusicData.release());
    return true;
}

void VR_PhoneBookListener::SendMsg2DE(const VR_String &msg)
{
    VR_LOGI("msg=[%s]", msg.c_str());
    std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_DM2DE);
    loopMessage->mutable_m_vrsendmsgdm2de()->set_msg(msg);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
}

void VR_PhoneBookListener::InitializeDefault()
{
    VR_LOGD_FUNC();

    m_PBContextHelper.loadPhoneType();

    if (isPhoneConnected()) {
        if (!getContactDataDefault()) {
            VR_LOGD("[-- query contacts data --]");
        }
        GetPhoneBookInfo();
    }
    else {
        if (m_PBContextHelper.m_bLazyService) {
            sendNotifyToBt();
        }
        else {
            m_PBContextHelper.SetEmNotifyToBT(VR_PhoneBookContextHelper::Notify_NeedSend);
        }
    }
}

void VR_PhoneBookListener::onNotifyUpdateDefault(NCUri &uri, NCVariant &variant)
{
    VR_String valueXml = variant.toString();
    VR_LOGD("[URI : %s] [Value : %s]", uri.toString().getString(), valueXml.c_str());

    if (NC_TRUE == uri.equals(NCString(VR_PHONE_BOOK_CONTACT_DATA_URI))) {
        // update contacts information
        VR_LOGD("[VR_PHONE_BOOK_CONTACT_DATA_URI] >>[%s]", valueXml.c_str());
        if (isPhoneConnected()) {
            getContactDataDefault();
            GetPhoneBookInfo();
        }
    }
    else if (NC_TRUE == uri.equals(NCString(VR_PHONE_BOOK_CONTACT_DATA_DICT_UPDATE_URI))) {
        // connect BT
        m_PBContextHelper.SetEmNotifyToBT(VR_PhoneBookContextHelper::Notify_NeedSend);
        VR_LOGD("[VR_PHONE_BOOK_CONTACT_DATA_DICT_UPDATE_URI] >>[%s]", variant.toString().c_str());

        if (("0" == variant.toString())) {
            // check md5
            if (m_PBContextHelper.IsMd5Equal()) {
                sendDBPath();
            }
            else {
                getContactDataDefault();
            }
        }
    }
    else if (NC_TRUE == uri.equals(NCString(VR_PHONE_BOOK_LAZY_SERVICE_NOTIFY_URI))) {
        m_PBContextHelper.m_bLazyService = true;
        if (VR_PhoneBookContextHelper::Notify_NeedSend == m_PBContextHelper.GetEmNotifyToBT()) {
            sendNotifyToBt();
        }
    }
    else if (NC_TRUE == uri.equals(NCString(VR_DATAPROVIDER_INITPERSONDATA_URI))) {
        VR_LOGD("[-- init personal data set tag-value default --]");
    }
    else {
        VR_LOGI("[-- do nothing about this uri: %s --]", uri.toString().getString());
    }
}

bool VR_PhoneBookListener::getContactDataDefault()
{
    VR_LOGD_FUNC();
    NCList<NCString> columns;
    columns << &VR_PB_QUERY_CONTACTS_INFO_INDEX__ID
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_CONTACT_ID
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_F_NAME
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_L_NAME
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_1
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_1
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_1
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_2
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_2
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_2
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_3
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_3
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_3
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_4
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_4
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_4
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_5
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_5
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_5;

    NCContentResolver resolver;
    NCCursor* cursor = resolver.query(NCUri(VR_PB_CONTACT_DATA_INFO_URI), columns,
                                      NCString(), NCList<NCString>(), NCString());

    INT rowNum = 0;
    if (NULL == cursor) {
        VR_LOGD("[-- QUERY [cursor==null] --]  URI->[%s]", VR_PB_CONTACT_DATA_INFO_URI.getString());
    }
    else {
        rowNum = cursor->getCount();
    }

    int pbNumCounts = 0;
    VoiceList<std::shared_ptr<myPersonInfo>>::type personNodeList;

    VR_LOGD("[-- QUERY-[%d]-CONTACTS-ITEMS-BEGIN --] ", rowNum);

    if (0 == rowNum) {
        VR_LOGD("[-- NO DATA [counts = 0] --]  URI->[%s]", VR_PB_CONTACT_DATA_INFO_URI.getString());
    }
    else {
        if (NC_FALSE == cursor->moveToFirst()) {
            VR_LOGD("[-- MoveToFirst = false --]  URI->[%s]", VR_PB_CONTACT_DATA_INFO_URI.getString());
            delete cursor;
            cursor = NULL;
            return false;
        }

        for (INT i = 0; i < rowNum; i++) {
            std::shared_ptr<myPersonInfo> personNode(VR_new myPersonInfo);

            personNode->contactId = cursor->getInt64(CONTACT_TABLE_FIELD_CONTACT_ID);
            personNode->firstName = cursor->getString(CONTACT_TABLE_FIELD_FIRSTNAME);
            personNode->lastName = cursor->getString(CONTACT_TABLE_FIELD_LASTNAME);

            if (0 == personNode->firstName.getLength()) {
                personNode->firstName = personNode->lastName;
                personNode->lastName = "";
            }

            NCString pNumber("");
            myPhoneNumber phoneNum;
            for (int iNumIndex = 0; iNumIndex < cstPhoneNum; ++iNumIndex) {
                pNumber = cursor->getString(CONTACT_TABLE_FIELD_PHONE_NUMBER + iNumIndex * cstNumberTypeCount);
                if (pNumber.getLength() > 0) {
                    phoneNum.type = cursor->getInt64(CONTACT_TABLE_FIELD_PHONE_TYPE + iNumIndex * cstNumberTypeCount);
                    phoneNum.index = cursor->getInt64(CONTACT_TABLE_FIELD_PHONE_INDEX + iNumIndex * cstNumberTypeCount);
                    phoneNum.number = pNumber;
                    personNode->lstPhoneNum.push_back(phoneNum);
                    pbNumCounts++;
                }
            }

            personNodeList.push_back(personNode);
            cursor->moveToNext();
        }
    }

    VR_LOGD("[-- QUERY-[%d]-CONTACTS-ITEMS-END --] ", rowNum);
    if (cursor) {
        delete cursor;
        cursor = NULL;
    }

    // 2.insert data into table contacts_bak
    bool result = m_PBContextHelper.UpdateDB(personNodeList, pbNumCounts);
    if (result) {
        VR_String strTmp = m_PBContextHelper.getPbMd5FromBt();
        m_PBContextHelper.SetPhoneBookMd5(strTmp);
    }
    else {
        VR_LOGD("[-- bool m_PBContextHelper.UpdateDB(list) failed --] ");
        return false;
    }

    sendDBPath();

    return true;
}

void VR_PhoneBookListener::sendNotifyToBt()
{
    nutshell::NCEvSysContext context;
    nutshell::NCEvMessageIF* eventSender = context.queryEventSender();
    nutshell::NCEvent* event = context.obtainEvent(VR_PB_SPACE, VR_PB_IDENTITY);

    if ((NULL != event) && (NULL != eventSender)) {
        if (nutshell::NC_FALSE == eventSender->sendEvent(event)) {
            VR_LOGD("[-- Send event error --]");
        }
        else {
            VR_LOGD("[-- Send Event Ok --]");
        }
    }
    else {
        VR_LOGD("Send event error.");
    }

    context.releaseEvent(event);
}

void VR_PhoneBookListener::sendDBPath()
{
    CL_AutoSync cAutoSync(m_cSync);
    VR_String strDBPath = VR_ConfigureIF::Instance()->getUsrPath() + VR_TMP_DB_NAME;
    if (NC_TRUE != NCFile::exists(NCString(strDBPath.c_str()))) {
        VR_LOGD("DB is not existed, (%s)", strDBPath.c_str());
        getContactDataDefault();
        return;
    }

    if (!m_PBContextHelper.copyDB()) {
        VR_LOGD("copy DB failed");
        return;
    }

    VR_String strDeviceAddr = m_PBContextHelper.getDeviceAddr();
    m_strDBPath = m_PBContextHelper.getDBPath();

    VR_XmlStrParse parser;
    parser.LoadString(VR_PHONE_CONTACTS_CHANGED_XML);
    parser.SetNodeText("//notification/id", VR_IdGenerator().GetID());
    parser.SetNodeText("//notification/phone_data/device_id", strDeviceAddr);
    parser.SetNodeText("//notification/phone_data/data_path", m_strDBPath);

    VR_String strXML;
    parser.GetXmlString(strXML);

    VR_ContentProvider vr_cp;
    VR_String strEngineType = vr_cp.GetContentProviderValue(VR_IAUTO_VR_ENGINETYPE);
    VR_LOGI("strEngineType = [%s]", strEngineType.c_str());
    if ("Nuance" != strEngineType) {
        SendMsg2DE(strXML);
        return;
    }
    sendEventToNATP(strXML);
}

bool VR_PhoneBookListener::notifyPhoneFinishedData()
{
    CL_AutoSync cAutoSync(m_cSync);
    if (m_strDBPath.empty()) {
        VR_LOGD("There is no phone DB");
        return false;
    }

    if (!VR_ConfigureIF::Instance()->isFileExist(m_strDBPath)) {
        VR_LOGD("Phone DB is not exist");
        return false;
    }

    VR_String strDeviceAddr = m_PBContextHelper.getDeviceAddr();
    if (strDeviceAddr.empty()) {
        VR_LOGD("There is no bt address");
        return false;
    }

    VR_XmlStrParse parser;
    parser.LoadString(VR_PHONE_CONTACTS_CHANGED_XML);
    parser.SetNodeText("//notification/id", VR_IdGenerator().GetID());
    parser.SetNodeText("//notification/phone_data/device_id", strDeviceAddr);
    parser.SetNodeText("//notification/phone_data/data_path", m_strDBPath);

    VR_String strXML;
    parser.GetXmlString(strXML);

    sendEventToNATP(strXML);
    return true;
}

void VR_PhoneBookListener::sendEventToNATP(const VR_String& strEvent)
{
    VR_LOGD("Event : %s", strEvent.c_str());
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(strEvent);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}

bool VR_PhoneBookListener::isPhoneConnected()
{
    NCString uriString(VR_SYSTEM_INFO_BT_HFP_MAIN_URI);
    NCUri uri(uriString);
    NCContentResolver resolver;
    ncsp<NCCursor>::sp queryResult = resolver.query(uri, NCList<NCString>(), NCString(),
                                                    NCList<NCString>(), NCString());

    if (NULL == queryResult.get() || 0 == queryResult->getCount()) {
        VR_LOGD("[-- :query content://system/info?bt/hfp-main failed --] ");
    }
    else if (queryResult->moveToFirst()) {
        if ("connected" == (queryResult->getString(0))) {
            return true;
        }
    }

    return false;
}

bool VR_PhoneBookListener::getDBPath(VR_String& strDBPath)
{
    CL_AutoSync cAutoSync(m_cSync);
    if (m_strDBPath == "") {
        return false;
    }
    strDBPath = m_strDBPath;
    return true;
}

bool VR_PhoneBookListener::getPhoneNumber(int iContactId, myPersonInfo& personInfo)
{
    VR_LOGD_FUNC();
    nutshell::NCUri uri(nutshell::NCString("content://contacts/searchbyid"));
    NCList<NCString> columns;
    columns << &VR_PB_QUERY_CONTACTS_INFO_INDEX__ID
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_CONTACT_ID
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_F_NAME
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_L_NAME
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_1
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_1
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_1
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_2
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_2
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_2
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_3
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_3
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_3
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_4
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_4
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_4
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_5
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_5
            << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_5;

    nutshell::NCString where;
    where.format("%s=?", "contact_id");
    nutshell::NCObjList<nutshell::NCString> whereArgs;
    nutshell::NCString* argId = VR_new nutshell::NCString();
    argId->format("%d", iContactId);
    whereArgs << argId;

    nutshell::NCContentResolver resolver;
    nutshell::NCCursor* cursor = resolver.query(uri, columns, where, whereArgs, NCString());

    bool bSuccessful = true;
    if ((NULL != cursor) && (0 != cursor->getCount()) && (cursor->moveToFirst())) {
        personInfo.contactId = cursor->getInt64(CONTACT_TABLE_FIELD_CONTACT_ID);
        personInfo.firstName = cursor->getString(CONTACT_TABLE_FIELD_FIRSTNAME);
        personInfo.lastName = cursor->getString(CONTACT_TABLE_FIELD_LASTNAME);
        personInfo.lstPhoneNum.clear();

        myPhoneNumber phoneNum;
        NCString pNumber("");
        for (int iNumIndex = 0; iNumIndex < cstPhoneNum; ++iNumIndex) {
            pNumber = cursor->getString(CONTACT_TABLE_FIELD_PHONE_NUMBER + iNumIndex * cstNumberTypeCount);
            if (pNumber.getLength() > 0) {
                phoneNum.type = cursor->getInt64(CONTACT_TABLE_FIELD_PHONE_TYPE + iNumIndex * cstNumberTypeCount);
                phoneNum.index = cursor->getInt64(CONTACT_TABLE_FIELD_PHONE_INDEX + iNumIndex * cstNumberTypeCount);
                phoneNum.number = pNumber;
                personInfo.lstPhoneNum.push_back(phoneNum);
            }
        }
    }
    else {
        bSuccessful = false;
    }

    if (cursor) {
        delete cursor;
        cursor = NULL;
    }

    return bSuccessful;
}

} // namespace dataprovider
} // namespace navi

/* EOF */
