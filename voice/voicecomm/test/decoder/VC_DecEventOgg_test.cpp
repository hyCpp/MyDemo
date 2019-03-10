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
#include "stdafx.h"
#include "VC_DecEventOgg.h"
#include "voicecomm_test.h"

#include "VC_DecoderOgg_mock.h"


using ::testing::_;
using ::testing::AtLeast;


/**
 * Summary for class VC_DecEventOgg_Test
 * 
 * This is a detail description for class VC_DecEventOgg_Test
 *
 */
class VC_DecEventOgg_Test:public testing::Test
{
    public:
        virtual void SetUp()
        {
            m_pVC_DecEventOgg = new(MEM_Voice) VC_DecEventOgg("./test.txt", 0, 0, NULL, NULL);
        }

        virtual void TearDown()
        {
             if (NULL != m_pVC_DecEventOgg) {
                  delete m_pVC_DecEventOgg;
                  m_pVC_DecEventOgg = NULL;
            }
        }
        
    public:
        VC_DecEventOgg  *m_pVC_DecEventOgg;
};

TEST_F(VC_DecEventOgg_Test, DoAction)
{
    MockVC_DecoderOgg *pmockDecoderOgg = new(MEM_Voice) MockVC_DecoderOgg();
    m_pVC_DecEventOgg->m_pDecoder = pmockDecoderOgg;
    EXPECT_CALL(*pmockDecoderOgg, Decode(_, _, _, _)).Times(AtLeast(1));
    m_pVC_DecEventOgg->DoAction();
    delete pmockDecoderOgg;
}

TEST_F(VC_DecEventOgg_Test, DoAction_nullName)
{
     VC_DecEventOgg  *m_pVC_DecEventOgg1 = new(MEM_Voice) VC_DecEventOgg(NULL, 0, NULL, NULL);
     MockVC_DecoderOgg *pmockDecoderOgg = new(MEM_Voice) MockVC_DecoderOgg();
     m_pVC_DecEventOgg1->m_pDecoder = pmockDecoderOgg;
     EXPECT_CALL(*pmockDecoderOgg, Decode(_, _, _)).Times(AtLeast(1));
     m_pVC_DecEventOgg1->DoAction();
     delete pmockDecoderOgg;

}
/* EOF */
