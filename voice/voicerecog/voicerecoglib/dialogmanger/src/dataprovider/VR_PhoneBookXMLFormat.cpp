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

#ifndef STDAFX_H
#    include "stdafx.h"
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#include "VR_PhoneBookXMLFormat.h"

namespace navi
{
namespace dataprovider
{

nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX__ID           = "_id";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_CONTACT_ID    = "contact_id";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_F_NAME        = "firstname";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_L_NAME        = "lastname";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_1  = "phone1_type";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_1 = "phone1_index";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_1   = "phone1_number";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_2  = "phone2_type";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_2 = "phone2_index";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_2   = "phone2_number";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_3  = "phone3_type";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_3 = "phone3_index";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_3   = "phone3_number";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_4  = "phone4_type";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_4 = "phone4_index";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_4   = "phone4_number";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_5  = "phone5_type";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_5 = "phone5_index";
nutshell::NCString VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_5  = "phone5_number";

nutshell::NCString VR_PB_QUERY_MSGHIST_INFO_INDEX_HANDLE     = "handle";
nutshell::NCString VR_PB_QUERY_MSGHIST_INFO_INDEX_INSTANCEID = "instanceid";
nutshell::NCString VR_PB_QUERY_MSGHIST_INFO_INDEX_TIME       = "time";
nutshell::NCString VR_PB_QUERY_MSGHIST_INFO_INDEX_ADDRESS    = "address";
nutshell::NCString VR_PB_QUERY_MSGHIST_INFO_INDEX_NAME       = "name";

/*********Modify for phoneTool***********/
const nutshell::NCString VR_PB_CONTACT_DATA_INFO_URI          = "content://contacts/fulldata";

}
}

/* EOF */
