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
 * @file VR_RadioXMLBuilder_Test.h
 * @brief Declaration file of class VR_RadioXMLBuilder_Test.
 *
 * This file includes the declaration of class VR_RadioXMLBuilder_Test.
 *
 * @attention used for C++ only.
 */
#ifndef VR_RADIO_XML_BUILDER_TEST_H
#define VR_RADIO_XML_BUILDER_TEST_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_RadioXMLBuilder.h"
#include "gtest/gtest.h"
#include "ncore/NCTypesDefine.h"
#include "dbhelper/NCContentValues.h"

namespace navi
{
namespace dataprovider
{
#define VR_RADIO_TEST_XML  "<grammar_new agent=\"media\">" \
                               "<category name=\"hdsubchannel\">" \
                                   "<formal id=\"1\" name=\"HD1\" intval=\"1\">" \
                                       "<alias name=\"HD 1\" />" \
                                   "</formal>" \
                                   "<formal id=\"2\" name=\"HD2\" intval=\"2\">" \
                                       "<alias name=\"HD 2\" />" \
                                   "</formal>" \
                                   "<formal id=\"3\" name=\"HD3\" intval=\"3\">" \
                                       "<alias name=\"HD 3\" />" \
                                   "</formal>" \
                                   "<formal id=\"4\" name=\"HD4\" intval=\"4\">" \
                                   "<alias name=\"HD 4\" />" \
                                   "</formal>" \
                                   "<formal id=\"5\" name=\"HD5\" intval=\"5\">" \
                                       "<alias name=\"HD 5\" />" \
                                   "</formal>" \
                                   "<formal id=\"6\" name=\"HD6\" intval=\"6\">" \
                                       "<alias name=\"HD 6\" />" \
                                   "</formal>" \
                                   "<formal id=\"7\" name=\"HD7\" intval=\"7\">" \
                                       "<alias name=\"HD 7\" />" \
                                   "</formal>" \
                                   "<formal id=\"8\" name=\"HD8\" intval=\"8\">" \
                                       "<alias name=\"HD 8\" />" \
                                   "</formal>" \
                               "</category>" \
                               "<category name=\"fmgenre\">" \
                                   "<formal id=\"1\" name=\"news\" />" \
                                   "<formal id=\"2\" name=\"country\">" \
                                       "<alias name=\"countryAlias\" pron=\"countryPron\" />" \
                                   "</formal>" \
                               "</category>" \
                           "</grammar_new>"


/**
 * @brief The VR_RadioXMLBuilder_Test class
 *
 * class declaration.
 */
class VR_RadioXMLBuilder_Test : public testing::Test
{
public:
    static void SetUpTestCase();
    static void TearDownTestCase();

protected:
    VR_RadioXMLBuilder_Test();
    ~VR_RadioXMLBuilder_Test();
    void SetUp();
    void TearDown();

private:
    VR_RadioXMLBuilder_Test(const VR_RadioXMLBuilder_Test&);
    VR_RadioXMLBuilder_Test& operator=(const VR_RadioXMLBuilder_Test&);

    // test suite level
    static VR_RadioXMLBuilder* m_testSuite;

    // test level
    VR_RadioXMLBuilder* m_test;
};

}
}
#endif /* VR_RADIO_XML_BUILDER_TEST_H */
/* EOF */
