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
 * @file VR_DataAccessorContactEU.h
 * @brief Declaration file of VR_DataAccessorContactEU.
 *
 * This file includes the declaration of VR_DataAccessorContactEU.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DATA_ACCESSOR_CONTACT_EU_H
#define VR_DATA_ACCESSOR_CONTACT_EU_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_DataAccessorContact.h"
#include "MEM_set.h"


/**
 * @brief The VR_DataAccessorContactEU class
 *
 * provide interface for query contacts
 */

class VR_DataAccessorContactEU : public VR_DataAccessorContact
{
public:
    VR_DataAccessorContactEU();
    virtual ~VR_DataAccessorContactEU() {}

    virtual bool getInfo(const std::string &operation, const std::string &reqMsg, std::string &response) override;

    bool phoneTypeAvailable(int phoneTypeID);

protected:
    VoiceSet<int>::type m_phoneTypeSet;
    std::string m_phoneListStr; // used in buildInfoList and getInfoByType

    virtual bool getPhoneInfo(const std::string &reqMsg, std::string &response) override;

    // filter screenList by phoneList
    bool buildInfoList(const std::string &reqMsg, std::string &response);

    // invoke after buildInfoList, need to save phoneInfoList of buildInfoList reqMsg
    bool getInfoByType(const std::string &reqMsg, std::string &response);

    // get phone type string used for script, used in buildInfoList and getInfoByType
    std::string getPhoneTypeString(int id);
};

#endif /* VR_DATA_ACCESSOR_CONTACT_EU_H */
/* EOF */
