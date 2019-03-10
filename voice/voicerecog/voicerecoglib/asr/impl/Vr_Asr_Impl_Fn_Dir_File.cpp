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
#include "Vr_Asr_Impl_Fn_Dir_File.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

int
Fn_File_Access
(
    string const & ro_string_Path_File
)
{
    return (::access(ro_string_Path_File.c_str(), 0) == 0);
}

int
Fn_File_Delete
(
    string const & ro_string_Path_File
)
{
    return ::remove(ro_string_Path_File.c_str());
}

int
Fn_File_Rename
(
    string const & ro_string_File_Old_Name,
    string const & ro_string_File_New_Name
)
{
    return ::rename(ro_string_File_Old_Name.c_str(), ro_string_File_New_Name.c_str());
}


int
Fn_Directory_Create
(
    string const & ro_string_Path_Dir
)
{
    int i_ret = 0;
    
#if defined(_WIN32) || defined(_WIN32_WCE)

    i_ret = _mkdir(ro_string_Path_Dir.c_str());
    
#else

    i_ret = mkdir(ro_string_Path_Dir.c_str(), 0755);
    
#endif

    return i_ret;
}

int
Fn_Directory_Create_Deep
(
    string const & ro_string_Path_Dir
)
{   
    int i_Ret = 0;
    string string_inner_path;
    IF (Fn_File_Access(ro_string_Path_Dir)) {
        return 0;
    }
    ELSE {
        string_inner_path = std::string(ro_string_Path_Dir, 0, ro_string_Path_Dir.find_last_of('/'));
        i_Ret = Fn_Directory_Create_Deep(string_inner_path);
    }

    IF (0 != i_Ret) {
        Fn_Log("Failed to Create Directory : " + string_inner_path);
        MACRO_Log_Error
        return -1;
    }
    
    return Fn_Directory_Create(ro_string_Path_Dir);
}

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
