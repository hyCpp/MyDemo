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
 * @file Voice_Asr_Impl_Event_Result_Impl.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Event_Result_Impl_h__
#define __Vr_Asr_Impl_Event_Result_Impl_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

/**
 * @brief The C_Event_Result_Impl class
 *
 * C_Event_Result_Impl class
 */
class C_Event_Result_Impl : public C_Event_Result
{
  public:

    shared_ptr<string> m_spstring_Result_XML;    // For Nunace
    shared_ptr<string> m_spstring_Result_Json;   // For IFlytek
    C_Buffer m_buffer_Userword_Transcription;
    C_Buffer m_buffer_Userword_Audio;

    virtual
    ~C_Event_Result_Impl(); // Destructor

    explicit
    C_Event_Result_Impl(); // Constructor of Default

    // For Nunance
    virtual
    shared_ptr<string const> // [R] Result string XML.
    Get_XML() const; // [F] Get result XML.

    // For IFlytek
    virtual
    shared_ptr<string const> // [R] Result string Json.
    Get_Json() const; // [F] Get result Json.

    virtual
    C_Buffer // [R] Result user word transcription (for voice tag record).
    Get_Buffer_Userword_Transcription() const; // [F] Get result user word transcription (for voice tag record).

    virtual
    C_Buffer // [R] Result user word audio buffer (for voice tag record).
    Get_Buffer_Userword_Audio() const; // [F] Get result user word audio buffer (for voice tag record).
  PROTECTED:

    C_Event_Result_Impl // Constructor: Copy
    (
        C_Event_Result_Impl const & // [I]
    );

    C_Event_Result_Impl & // [R]
    operator = // Operator: =
    (
        C_Event_Result_Impl const & // [I]
    );
};

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
