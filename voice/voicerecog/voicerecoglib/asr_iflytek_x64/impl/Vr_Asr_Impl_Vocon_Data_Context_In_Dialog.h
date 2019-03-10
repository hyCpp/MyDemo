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
 * @file Vr_Asr_Impl_Data_Context_In_Dialog.h
 * @brief C_Data_Context_In_Dialog class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Data_Context_In_Dialog_h__
#define __Vr_Asr_Impl_Data_Context_In_Dialog_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

/**
 * @brief The C_Data_Context_In_Dialog class
 *
 * C_Data_Context_In_Dialog class
 */

class C_Data_Context_In_Dialog
{
  public:

    string m_string_Id_Context;
    int m_b_Is_Host_For_Merge;
    shared_ptr<C_Request_Activate> m_spo_Request_Activate;
    map<string, pair<unsigned long, long> > m_map_Ctx_Param;

    ~C_Data_Context_In_Dialog(); // [F]Destructor

    explicit
    C_Data_Context_In_Dialog(); // [F]Constructor of Default

  PROTECTED:

    C_Data_Context_In_Dialog // [F]Constructor: Copy
    (
        C_Data_Context_In_Dialog const & // [I]
    );

    C_Data_Context_In_Dialog & // [R]
    operator = // [F]Operator: =
    (
        C_Data_Context_In_Dialog const & // [I]
    );
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */