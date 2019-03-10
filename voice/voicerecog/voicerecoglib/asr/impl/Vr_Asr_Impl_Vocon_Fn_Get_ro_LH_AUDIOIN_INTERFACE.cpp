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
#include "Vr_Asr_Impl_Vocon_Fn_Get_ro_LH_AUDIOIN_INTERFACE.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

#ifdef MACRO_Dynamic_Debug
static
#endif
int
S_Fn_Audio_In_Start
(
    void * pvoid_Audio_In
)
{
    C_Audio_In * po_Audio_In = reinterpret_cast<C_Audio_In *>(pvoid_Audio_In);
    IF (!po_Audio_In) {
        return 1;
    }

    return po_Audio_In->M_Fn_Start();
}

#ifdef MACRO_Dynamic_Debug
static
#endif
int
S_Fn_Audio_In_Stop
(
    void * pvoid_Audio_In
)
{
    C_Audio_In * po_Audio_In = reinterpret_cast<C_Audio_In *>(pvoid_Audio_In);
    IF (!po_Audio_In) {
        return 1;
    }

    return po_Audio_In->M_Fn_Stop();
}

#ifdef MACRO_Dynamic_Debug
static
#endif
int
S_Fn_Audio_In_Has_Data
(
    void * pvoid_Audio_In,
    LH_BOOL * pb_Has_Data
)
{
    C_Audio_In * po_Audio_In = reinterpret_cast<C_Audio_In *>(pvoid_Audio_In);
    IF (!po_Audio_In) {
        return 1;
    }

    bool b_Data_Available = false;
    int i_Result = po_Audio_In->M_Fn_Data_Available_Check(&b_Data_Available);
    IF (i_Result != 0) {
        return i_Result;
    }

    *pb_Has_Data = b_Data_Available ? LH_TRUE : LH_FALSE;

    return 0;
}

#ifdef MACRO_Dynamic_Debug
static
#endif
int
S_Fn_Audio_In_Fetch_Data
(
    void * pvoid_Audio_In,
    void ** ppvoid_Buffer,
    size_t * pi_Size
)
{
    C_Audio_In * po_Audio_In = reinterpret_cast<C_Audio_In *>(pvoid_Audio_In);
    IF (!po_Audio_In) {
        return 1;
    }

    return po_Audio_In->M_Fn_Data_Fetch(
        ppvoid_Buffer,
        pi_Size
    );
}

#ifdef MACRO_Dynamic_Debug
static
#endif
int
S_Fn_Audio_In_Return_Data
(
    void * pvoid_Audio_In,
    void * pvoid_Buffer
)
{
    C_Audio_In * po_Audio_In = reinterpret_cast<C_Audio_In *>(pvoid_Audio_In);
    IF (!po_Audio_In) {
        return 1;
    }

    return po_Audio_In->M_Fn_Data_Return(
        pvoid_Buffer
    );
}

LH_AUDIOIN_INTERFACE &
Fn_Get_ro_LH_AUDIOIN_INTERFACE()
{
    static C_Mutex o_Mutex;
    C_Lock_Mutex o_Lock_Mutex(o_Mutex);

    static LH_AUDIOIN_INTERFACE o_LH_AUDIOIN_INTERFACE;
    static bool b_Init = false;
    IF (!b_Init) {
        o_LH_AUDIOIN_INTERFACE.pfstart = S_Fn_Audio_In_Start;
        o_LH_AUDIOIN_INTERFACE.pfstop = S_Fn_Audio_In_Stop;
        o_LH_AUDIOIN_INTERFACE.pfhasdata = S_Fn_Audio_In_Has_Data;
        o_LH_AUDIOIN_INTERFACE.pffetchdata = S_Fn_Audio_In_Fetch_Data;
        o_LH_AUDIOIN_INTERFACE.pfreturndata = S_Fn_Audio_In_Return_Data;

        b_Init = true;
    }

    return o_LH_AUDIOIN_INTERFACE;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
