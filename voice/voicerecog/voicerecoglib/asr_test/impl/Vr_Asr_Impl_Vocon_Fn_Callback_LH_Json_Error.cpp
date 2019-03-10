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

#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Impl_Vocon_Fn_Callback_LH_Json_Error.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

extern "C"
void
Fn_Callback_LH_Json_Error
(
    void * pvoid_Instance,
    const char * pi_Message
)
{
    stringstream stringstream_Log;
    Fn_Log("::: ************");
    Fn_Log("::: LH JSON ERROR callback :::");

    stringstream_Log.clear();
    stringstream_Log.str("");
    stringstream_Log << "::: Message   : " << (pi_Message ? pi_Message : "<null>");
    Fn_Log(stringstream_Log.str());


    Fn_Log("::: ************");

    stringstream stringstream_Error;

    stringstream_Error << " Message: " << (pi_Message ? pi_Message : "<null>");

    Fn_Err(stringstream_Error.str());
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
