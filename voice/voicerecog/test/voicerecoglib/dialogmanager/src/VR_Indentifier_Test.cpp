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

#include   "stdafx.h"
#include   "gtest/gtest.h"
#include   "action/VR_ActionBase.h"
#include   "VR_Indentifier.hpp"
#include   "VR_CommonTool.h"
#include   "VR_Command.h"
#include   "VR_CommandHandleIF.h"
#include   "VR_CommandFactory.h"
#include   "VR_DataProviderIF.h"
#include   "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

using  namespace  navi::VoiceRecog;
using  namespace  navi::dataprovider;
/**
* VR_Indentifier_Test.cpp
*
* The class is just for VR_Indentifier test.
*/
class  VR_Indentifier_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();
};


void
VR_Indentifier_Test::SetUp()
{

}

void
VR_Indentifier_Test::TearDown()
{

}

TEST_F(VR_Indentifier_Test, Indentifier_Case)
{
    VR_Indentifier<VR_ActionBase>    test;
    test.SetID(100);
    test.GetID();
    VR_Indentifier<VR_ActionBase>::m_sRequestID = -88;
    VR_Indentifier<VR_ActionBase>    test1;
    test1.SetID(100);
    test1.GetID();

    SUCCEED();
}


/**
* VR_CommonTool_Test.cpp
*
* The class is just for VR_CommonTool test.
*/
class  VR_CommonTool_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();
};


void
VR_CommonTool_Test::SetUp()
{

}

void
VR_CommonTool_Test::TearDown()
{

}

TEST_F(VR_CommonTool_Test, EndianSwitch_Case)
{
    EndianSwitch<char>('a');
    EndianSwitch<int32_t>(100);
    SUCCEED();
}

/**
* VR_Commmand_Successor.cpp
*
* The class is just for VR_Command test.
*/
class  VR_Command_Successor : public VR_Command
{
public:
    VR_Command_Successor(VR_ProtoMessageBase* msg)
        : VR_Command(msg)  
    {
    }
  
    virtual void perform() {}
    virtual void replay()  {}
};

/**
* VR_CommmandHandle_Successor.cpp
*
* The class is just for VR_CommmandHandleIF test.
*/
class  VR_CommmandHandle_Successor : public VR_CommandHandleIF
{
public:
    VR_CommmandHandle_Successor()
    {
    }

    virtual void onCommand(const spVR_Command cmd)  {}
};

/**
* VR_CommandFactory_Successor.cpp
*
* The class is just for VR_CommandFactory test.
*/
class  VR_CommandFactory_Successor : public VR_CommandFactoryImp
{
public:
    VR_CommandFactory_Successor()
    {
    }
};

/**
* VR_DataProvider_Successor.cpp
*
* The class is just for VR_DataProviderIF test.
*/
class  VR_DataProvider_Successor : public VR_DataProviderIF
{
public:
    virtual bool RegistObserver()  {}
    virtual void UnRegistObserver() {}
};


/**
* VR_Commmand_Test.cpp
*
* The class is just for VR_Command test.
*/
class  VR_Commmand_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();
};


void
VR_Commmand_Test::SetUp()
{

}

void
VR_Commmand_Test::TearDown()
{

}

TEST_F(VR_Commmand_Test, Disconstruct_Case)
{
    VrAdditionalInfoReq*  req  = new  VrAdditionalInfoReq;
    VR_Command* pCommand = new  VR_Command_Successor(req);
    delete  pCommand;

    pCommand = new  VR_Command_Successor(NULL);
    delete  pCommand;

    VR_CommandHandleIF* pCommandHandle = new  VR_CommmandHandle_Successor();
    delete pCommandHandle;

    VR_CommandFactory*  pFactory = new  VR_CommandFactory_Successor();
    delete pFactory;

    VR_DataProviderIF* pDataProvider = new  VR_DataProvider_Successor();
    delete pDataProvider;

    SUCCEED();
}


/* EOF */
