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
 * @file Vr_Asr_Impl_Fn_Dir_File.h
 * @brief interface define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Dir_File_h__
#define __Vr_Asr_Impl_Dir_File_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

extern
int
Fn_File_Access
(
    string const & ro_string_Path_File
);

extern
int
Fn_File_Delete
(
    string const & ro_string_Path_File
);

extern
int
Fn_File_Rename
(
    string const & ro_string_File_Old_Name,
    string const & ro_string_File_New_Name
);



extern
int
Fn_Directory_Create
(
    string const & ro_string_Path_Dir
);

extern
int
Fn_Directory_Create_Deep
(
    string const & ro_string_Path_Dir
);

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
