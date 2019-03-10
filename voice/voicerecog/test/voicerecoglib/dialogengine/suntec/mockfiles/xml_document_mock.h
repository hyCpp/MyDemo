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

/**
 * @file VR_Configure_mock.h
 * @brief inteface for interpeter or asr engine to perfer command
 *
 *
 * @attention used for C++ only.
 */
#ifndef XML_DOCUMENT_MOCK_H
#define XML_DOCUMENT_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "pugixml.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"


/**
* xml_document_mock
*
* The class is a mock class for xml_document.
*/
class xml_document_mock : public pugi::xml_document
{
public:
    xml_document_mock() : xml_document()
    {
    }

    MOCK_METHOD0(first_child, pugi::xml_node());
    MOCK_METHOD1(remove_child, bool(const pugi::char_t* name_));
};


#endif
/* EOF */
