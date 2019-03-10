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
 * @file Voice_Asr_EventResult.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Event_Result_h__
#define __Vr_Asr_Event_Result_h__
#include "Vr_Asr_Def.h"

namespace N_Vr
{

namespace N_Asr
{

/**
 * @brief The C_Event_Result class
 *
 * C_Event_Result class
 */
class C_Event_Result
{
  public:

    virtual
    ~C_Event_Result(); // Destructor

    virtual
    boost::shared_ptr<std::string const> // [R] Result string XML.
    Get_XML() const = 0; // [F] Get result XML.

    virtual
    boost::shared_ptr<std::string const> // [R] Result string Json.
    Get_Json() const = 0; // [F] Get result Json.

    virtual
    C_Buffer // [R] Result user word transcription (for voice tag record).
    Get_Buffer_Userword_Transcription() const = 0; // [F] Get result user word transcription (for voice tag record).

    
    virtual
    C_Buffer // [R] Result user word audio buffer (for voice tag record).
    Get_Buffer_Userword_Audio() const = 0; // [F] Get result user word audio buffer (for voice tag record).

  protected:

    explicit
    C_Event_Result(); // Constructor of Default

    C_Event_Result // Constructor: Copy
    (
        C_Event_Result const & // [I]
    );

    C_Event_Result & // [R]
    operator = // Operator: =
    (
        C_Event_Result const & // [I]
    );
};

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
