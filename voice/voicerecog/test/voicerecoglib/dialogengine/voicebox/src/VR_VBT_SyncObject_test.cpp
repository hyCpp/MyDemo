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
#include <string>
#include "gtest/gtest.h"
#include "VBTExternalErrors.h"
#include "VR_VBT_EngineIF_mock.h"
#include "VR_VoiceBoxSyncObject.h"

/**
* VR_VBT_SyncObject_Test
*
* The class is just for VR_VoiceBoxSyncObject test.
*/
class VR_VBT_SyncObject_Test : public testing::Test
{
public:
    VR_VBT_SyncObject_Test()
    {
    }

    virtual void SetUp();
    virtual void TearDown();

private:
    VR_VoiceBoxSyncObject* m_syncObject;
};

void
VR_VBT_SyncObject_Test::SetUp()
{
    m_syncObject = new VR_VoiceBoxSyncObject();
}

void
VR_VBT_SyncObject_Test::TearDown()
{
    delete m_syncObject;
    m_syncObject = NULL;
}

TEST_F(VR_VBT_SyncObject_Test, SynchronizeStart)
{
    m_syncObject->SynchronizeStart();
}

TEST_F(VR_VBT_SyncObject_Test, SynchronizeEnd)
{
    m_syncObject->SynchronizeEnd();
}

/* EOF */
