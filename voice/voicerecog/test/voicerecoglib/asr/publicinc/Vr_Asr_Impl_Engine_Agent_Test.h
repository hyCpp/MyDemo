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
 * @file Vr_Asr_Impl_Engine_Agent_Test.h
 * @brief Declaration file of class C_Engine_Agent_Test.
 *
 * This file includes the declaration of class C_Engine_Agent_Test.
 *
 * @attention used for C++ only.
 */

#ifndef _Vr_Asr_Impl_Engine_Agent_Test_h_
#define _Vr_Asr_Impl_Engine_Agent_Test_h_

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

class C_Engine_Agent;
class C_Engine_Base_Mock;

/**
 * @brief The C_Engine_Agent_Test class
 *
 * C_Engine_Agent_Test class
 */

class C_Engine_Agent_Test : public testing::Test
{
public:
    C_Engine_Agent_Test();
    virtual ~C_Engine_Agent_Test();

    virtual void SetUp();
    virtual void TearDown();

private:
    C_Engine_Agent_Test(const C_Engine_Agent_Test&);
    C_Engine_Agent_Test& operator=(const C_Engine_Agent_Test&);

    C_Engine_Agent* m_p_C_Engine_Agent;
    scoped_ptr<C_Engine_Base_Mock> m_scop_C_Engine_Base_Mock;
};

} // namespace N_Impl

} // namespace N_Asr

} // namespace N_Vr

#endif // #ifndef _Vr_Asr_Impl_Engine_Agent_Test_h_
/* EOF */
