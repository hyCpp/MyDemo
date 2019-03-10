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
 * @file VR_PhoneBookXMLFormat.h
 * @brief Declaration file of phone book xml format.
 *
 * This file includes the declaration of phone book xml format.
 *
 * @attention used for C++ only.
 */

#ifndef VR_PHONE_BOOK_XML_FORMAT_H
#define VR_PHONE_BOOK_XML_FORMAT_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef STDAFX_H
#    include "stdafx.h"
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#include "ncore/NCString.h"
#include <boost/assign/list_of.hpp>
#include "VR_DataProviderComm.h"

namespace navi
{
namespace dataprovider
{

#define VR_PB_SIGN_INT_DEFAULT              -1
#define VR_PB_SIGN_INT_ZERO                  0
#define VR_PB_UNSIGN_INT_COUNTS_ONE          1
#define VR_PB_UNSIGN_INT_COUNTS_TWO          2
#define VR_PB_UNSIGN_INT_COUNTS_THREE        3
#define VR_PB_UNSIGN_INT_COUNTS_FOUR         4
#define VR_PB_UNSIGN_INT_COUNTS_FIVE         5
#define VR_PB_UNSIGN_INT_COUNTS_SIX          6
#define VR_PB_UNSIGN_INT_COUNTS_EIGHT           8
#define VR_PB_UNSIGN_INT_COUNTS_NINE         9
#define VR_PB_UNSIGN_INT_COUNTS_TEN          10
#define VR_PB_UNSIGN_INT_COUNTS_ELEVEN       11
#define VR_PB_UNSIGN_INT_COUNTS_TWELVE       12
#define VR_PB_UNSIGN_INT_COUNTS_THIRTEEN     13
#define VR_PB_UNSIGN_INT_COUNTS_FIFTEEN      15
#define VR_PB_UNSIGN_INT_COUNTS_TWENYFOUR       24
#define VR_PB_UNSIGN_INT_MD5_MAXLEN          32

const int cstPhoneNum = 5;
const int cstNumberTypeCount = 3;
const int cstResultOK = 0;
const int cstResultNG = 1;

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  phone state /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#define VR_PB_DBRESULT_OK                    0
#define VR_PB_DBRESULT_NG                   1

#define VR_PB_GRAMRESULT_OK                   0
#define VR_PB_GRAMRESULT_NG                  1

#define VR_PB_CONTACT_MD5_CODE_URI           "content://contacts/mainmd5"

#define VR_PB_KEY_NAME_VEHICLE_PBK           "PBK"
#define VR_PB_KEY_NAME_PHONE_CONNECTED       "PHONE_STATE_CONNECTED"
#define VR_PB_KEY_NAME_PHONE_DEVICE_ID       "PHONE_DEVICE_ID"
#define VR_PB_KEY_NAME_PHONE_REGISTERED      "PHONE_STATE_REGISTERED"
#define VR_PB_KEY_NAME_PHONE_NET_SIGNAL      "PHONE_STATE_NO_NET_SIGNAL"
#define VR_PB_KEY_NAME_PHONE_INCALL          "PHONE_STATE_INCALL"
#define VR_PB_KEY_NAME_BTAUDIO_CONNECTED     "BTAUDIO_CONNECTED"
#define VR_PB_KEY_NAME_BT_PBMODE             "BT_PB_MODE"
#define VR_PB_KEY_NAME_VEHICLE_OPERESTRICT   "OPERESTRICT"
#define VR_PB_KEY_NAME_MESSAGE_AVAILIABLE    "MESSAGE_AVAILIABLE"

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// phone book db ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// PHONEBOOK DATABASE DEFINE
#define VR_PB_DB_TABLE_NAME_DEVICE    "DeviceInformation"
#define VR_PB_DB_TABLE_NAME_CONTACT  "Contact"
#define VR_PB_DB_TABLE_NAME_PHONETYPE    "PhoneType"
#define VR_PB_DB_TABLE_NAME_PHONERECORDS    "PhoneRecords"
#define VR_PB_DB_NAME_VR_CONTACTS    "VUIPhone_"

#define VR_PB_SQL_INSERT_INFO_INTO_TABLE_DEVICE \
    "INSERT INTO " VR_PB_DB_TABLE_NAME_DEVICE" VALUES (?,?)"

#define VR_PB_SQL_INSERT_INFO_INTO_TABLE_CONTACT \
    "INSERT INTO " VR_PB_DB_TABLE_NAME_CONTACT" VALUES (?,?,?)"

#define VR_PB_SQL_INSERT_INFO_INTO_TABLE_PHONETYPE \
    "INSERT INTO " VR_PB_DB_TABLE_NAME_PHONETYPE" VALUES (?,?,?,?)"

#define VR_PB_SQL_INSERT_INFO_INTO_TABLE_PHONERECORDS \
    "INSERT INTO " VR_PB_DB_TABLE_NAME_PHONERECORDS" VALUES (?,?,?,?)"

#define VR_PB_SQL_CREAT_TABLE_DEVICE \
    "CREATE TABLE IF NOT EXISTS " VR_PB_DB_TABLE_NAME_DEVICE" (" \
    "name  TEXT," \
    "address   TEXT);"

#define VR_PB_SQL_CREAT_TABLE_CONTACTS \
    "CREATE TABLE IF NOT EXISTS " VR_PB_DB_TABLE_NAME_CONTACT" (" \
    "id     INTEGER," \
    "first_name  TEXT," \
    "last_name   TEXT);"

#define VR_PB_SQL_CREAT_TABLE_PHONETYPE \
    "CREATE TABLE IF NOT EXISTS " VR_PB_DB_TABLE_NAME_PHONETYPE" (" \
    "id     INTEGER," \
    "type_eng   TEXT," \
    "type_mnc   TEXT," \
    "type_cah   TEXT);"

#define VR_PB_SQL_CREAT_TABLE_PHONERECORDS \
    "CREATE TABLE IF NOT EXISTS " VR_PB_DB_TABLE_NAME_PHONERECORDS" (" \
    "id         INTEGER," \
    "contact_id     INTEGER," \
    "type_id    INTEGER," \
    "number     TEXT);"

#define VR_PB_SQL_TRUNCATE_TABLE_PHONETYPE \
    "TRUNCATE TABLE " VR_PB_DB_TABLE_NAME_PHONETYPE " ;"

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  enum define /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// PHONE BOOK CONTACTS INDXE
typedef enum emVrContactTableFieldIndex
{
    CONTACT_TABLE_FIELD_CONTACT_ID    = 1,
    CONTACT_TABLE_FIELD_FIRSTNAME     = 2,
    CONTACT_TABLE_FIELD_LASTNAME      = 3,
    CONTACT_TABLE_FIELD_PHONE_TYPE   = 4,
    CONTACT_TABLE_FIELD_PHONE_INDEX  = 5,
    CONTACT_TABLE_FIELD_PHONE_NUMBER = 6
}ContactTableFieldIndex;

// PHONE SMS MESSAGE INDXE
typedef enum emVrSMSMessageTableFieldIndex
{
    MESSAGE_TABLE_FIELD_HANDLE      = 0,
    MESSAGE_TABLE_FIELD_INSTANCEID  = 1,
    MESSAGE_TABLE_FIELD_TIME        = 2,
    MESSAGE_TABLE_FIELD_ADDRESS     = 3,
    MESSAGE_TABLE_FIELD_NAME        = 4
}SMSMessageTableFieldIndex;

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  struct define ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// DECLARE STRUCT FOR BEING SAVED CONTACTSINFO
struct myPhoneNumber
{
    nutshell::INT64    type;
    nutshell::INT64    index;
    nutshell::NCString number;
    myPhoneNumber() : type(VR_PB_SIGN_INT_DEFAULT), number("")
    {
    }
};

struct myPersonInfo
{
    nutshell::INT64 contactId;
    nutshell::NCString firstName;
    nutshell::NCString lastName;
    VoiceList<myPhoneNumber>::type lstPhoneNum;
    myPersonInfo() : contactId(0), firstName(""), lastName("")
    {
        lstPhoneNum.clear();
    }
};

typedef VoiceList<std::shared_ptr<myPersonInfo>>::type VrPhoneBookDataList;

struct VR_MessageInfo
{
    nutshell::INT64 iInstanceId;
    nutshell::NCString strMessageId;
    nutshell::NCString strName;
    nutshell::INT64 iContactId;
    nutshell::NCString strNumber;
    nutshell::NCString strMinute;
    nutshell::NCString strHour;
    nutshell::NCString strDay;
    nutshell::NCString strMonth;
    nutshell::NCString strYear;
};

typedef VoiceVector<std::shared_ptr<VR_MessageInfo>>::type VrMessageInfo;

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////  NCString define ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// QUERY CONTACTS PROVIDER INDEX
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX__ID;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_CONTACT_ID;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_F_NAME;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_L_NAME;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_1;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_1;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_1;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_2;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_2;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_2;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_3;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_3;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_3;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_4;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_4;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_4;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_5;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_5;
extern nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_5;

// query message info index
extern nutshell::NCString VR_PB_QUERY_MSGHIST_INFO_INDEX_HANDLE;
extern nutshell::NCString VR_PB_QUERY_MSGHIST_INFO_INDEX_INSTANCEID;
extern nutshell::NCString VR_PB_QUERY_MSGHIST_INFO_INDEX_TIME;
extern nutshell::NCString VR_PB_QUERY_MSGHIST_INFO_INDEX_ADDRESS;
extern nutshell::NCString VR_PB_QUERY_MSGHIST_INFO_INDEX_NAME;

// URI PATH FOR GETTING PHONE DATA
extern const nutshell::NCString VR_PB_CONTACT_DATA_INFO_URI;


}
}
#endif /* VR_PHONEBOOKXMLFORMAT_H */
/* EOF */
