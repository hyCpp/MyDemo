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

#define private public
#define protected public

#include "VR_DeriveDataModel.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::Return;
using ::testing::_;
// using namespace pugi;


/**
* VR_DeriveDataModel_test.cpp
*
* The class for VR_DeriveDataModel_test.
*/
class VR_DeriveDataModel_test : public testing::Test
{
public:
    VR_DeriveDataModel_test()
    {
        m_derivedatamodel = NULL;
    }

    virtual ~VR_DeriveDataModel_test()
    {
    }

    virtual void SetUp()
    {
        m_derivedatamodel = new VR_DeriveDataModel();
    }

    virtual void TearDown()
    {
        delete m_derivedatamodel;
    }

private:
    VR_DeriveDataModel_test(const VR_DeriveDataModel_test&);
    VR_DeriveDataModel_test& operator= (const VR_DeriveDataModel_test&);

    VR_DeriveDataModel* m_derivedatamodel;
};

// assign_1 BEGIN
// TEST_F(VR_DeriveDataModel_test, assign_1_case)
// {
//    uscxml::DataModel desDataModel;
//    uscxml::DataModel souDataModel;
//    m_derivedatamodel->assign(desDataModel, souDataModel);
//    SUCCEED();
// }
// assign_1 END
 

// assign_2 BEGIN
TEST_F(VR_DeriveDataModel_test, assign_2_case)
{
    uscxml::DataModel desDataModel;
    std::string keyExpr;
    Arabica::XPath::XPathValue<std::string> valueXpath;
    m_derivedatamodel->assign(desDataModel, keyExpr, valueXpath);
    SUCCEED();
}
// assign_2 END

// getXpathDataModel BEGIN
TEST_F(VR_DeriveDataModel_test, getXpathDataModel)
{
    m_derivedatamodel->getXpathDataModel();
    SUCCEED();
}
// getXpathDataModel END

/* EOF */
