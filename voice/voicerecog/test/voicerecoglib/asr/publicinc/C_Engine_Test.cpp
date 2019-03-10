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

#if defined(_WIN32) || defined(_WIN32_WCE)
#include "stdafx.h"
#else
#include <dirent.h>
#include <sys/time.h>
#include <sys/wait.h>
#endif

#include <sys/types.h>
#include <cstdlib>

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Engine.h"
#include "AudioIn_Adapter.h"
#include "C_Engine_Test.h"
#include "Record_Log.h"

#if defined(_WIN32) || defined(_WIN32_WCE)
string
Fn_Get_string_from_wstring_gtest
(
    const wstring & rwstring_I
)
{
    size_t sizeRV;
    size_t sizeDest = 2 * rwstring_I.size() + 1;
    char * a_cDest = new char[sizeDest];
    memset(a_cDest, 0, sizeDest);
    string curLocale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");
    wcstombs_s(&sizeRV, a_cDest, sizeDest, rwstring_I.c_str(), rwstring_I.size());
    setlocale(LC_ALL, curLocale.c_str());
    string result = a_cDest;
    delete[] a_cDest;
    return result;
}

#endif

C_Engine_Test::C_Engine_Test() : m_b_Context_Start(false),
    m_b_Rec_Start(false),
    m_i_list_add_num(0),
    m_i_list_add_complete(0)
{
}

C_Engine_Test::~C_Engine_Test()
{

}

int
C_Engine_Test::M_Fn_Initialize()
{

    m_b_Context_Start = false;
    m_b_Rec_Start = false;
    m_i_list_add_num = 0;
    m_i_list_add_complete = 0;

    return 0;
}

int
C_Engine_Test::M_Fn_UnInitialize()
{
    return 0;
}

void
C_Engine_Test::M_Fn_On_Event_Init_Notify
(
    const N_Vr::N_Asr::C_Event_Init_Notify& EventNotifyType
)
{

}



void
C_Engine_Test::M_Fn_Recognize_On_Event_Phase
(
    const N_Vr::N_Asr::C_Event_Phase& eventPhaseType
)
{
    switch (eventPhaseType.Get_Event_Phase_Type()) {
    case N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_Begin:
    {
        printf("=======rec: E_Event_Phase_Type_Proc_Begin ======\n");
        break;
    }
    case N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End:
    {
        printf("=======rec: E_Event_Phase_Type_Proc_End ======\n");
        break;
    }
    default:
    {
        break;
    }
    }

}

void
C_Engine_Test::M_Fn_Recognize_On_Event_Notify
(
    const N_Vr::N_Asr::C_Event_Notify& EventNotifyType
)
{
}

void
C_Engine_Test::M_Fn_Recognize_On_Event_Result
(
    const N_Vr::N_Asr::C_Event_Result& EventResultType
)
{
}


void
C_Engine_Test::M_Fn_Add_List_Context_On_Event_Phase
(
    const N_Vr::N_Asr::C_Event_Phase& eventPhaseType
)
{
    switch (eventPhaseType.Get_Event_Phase_Type()) {
    case N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_Begin:
    {
        printf("=======list: E_Event_Phase_Type_Proc_Begin ======\n");
        m_b_Context_Start = true;
        break;
    }
    case N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End:
    {
        printf("=======list: E_Event_Phase_Type_Proc_End ======\n");
        break;
    }
    default:
    {
        break;
    }
    }
}

void C_Engine_Test::M_Fn_Add_List_Context_On_Event_Notify
(
    const N_Vr::N_Asr::C_Event_Notify& EventNotifyType
)
{
}


int
C_Engine_Test::M_Fn_Get_Directory_Files
(
    const std::string&  string_path,
    std::set<std::string>& set_directory_files
)
{
    MACRO_Log_Function
#if defined(_WIN32) || defined(_WIN32_WCE)

    CFileFind fileFinder;
    CString strFileName;
    INT nFileNameLen = 0;
    BOOL bFinished = FALSE;

    wstring wstring_file_name;
    string string_file_name;

    bFinished = fileFinder.FindFile(CString(string_path.c_str()) + CString("*.*"));
    while (bFinished)  {
        bFinished = fileFinder.FindNextFile();
        if (fileFinder.IsDots()) {
            continue;
        }
        strFileName = fileFinder.GetFileName();
        nFileNameLen = strFileName.GetLength();
        wstring_file_name = strFileName.GetBuffer(nFileNameLen);
        string_file_name = Fn_Get_string_from_wstring_gtest(wstring_file_name);
        set_directory_files.insert(string_path + string_file_name);
    }

    fileFinder.Close();

#else

    DIR *dir = NULL;

    Fn_Log(string_path.c_str());

    dir = opendir(string_path.c_str());
    if (NULL == dir) {
        MACRO_Log_Error
        return -1;
    }

    set_directory_files.clear();
    for (;;) {
        struct dirent *ptr = readdir(dir);
        if (NULL == ptr) {
            break;
        }

        if (strcmp(ptr->d_name, ".") == 0
            || strcmp(ptr->d_name, "..") == 0) {
            // current dir OR parrent dir
            continue;
        }
        else if (ptr->d_type == DT_REG) { // regular file
            Fn_Log(ptr->d_name);
            set_directory_files.insert(string_path  + std::string(ptr->d_name));
        }
        else { // other file type
        }
    }

    closedir(dir);

#endif
    return 0;
}

/* EOF */
