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

#ifndef GTEST_INCLUDE_GTEST_GTEST_H_
#    include "gtest/gtest.h"
#endif

#ifndef CXX_BL_STRING_H
#    include "BL_String.h"
#endif

#ifndef CXX_BL_FULLFILE_H
#    include "BL_FullFile.h"
#endif

#ifndef CXX_BL_SYSTEMLANGUAGE_DEF_H
#    include "BL_SystemLanguage_DEF.h"
#endif

#include "TTS_EngineNuance.h"
#include "TTS_EngineCommDef.h"
#include "TTS_file_test.h"
#include <dirent.h>

static const XCHAR* const TTS_NUANCE_CONF_TEST = XTEXT("SD/.kanavijp/ndata/Sounds/tts");
static const INT          TTS_NUANCE_LANG_TEST = BL_LANGUAGE_TYPE_UKENGLISH;

static const XCHAR* const TTS_NUANCE_DIR_TEST = XTEXT("SD/.kanavijp/ndata/Sounds/tts");
static const XCHAR* const TTS_NUANCE_NULL_DIR_TEST = XTEXT("SD/.kanavijp/ndata/Sounds/tts/null");
static const std::string  TTS_NUANCE_USER_DATA_TEST = "/Nuance/user_data/";

static const TtsPitch     TTS_NUANCE_PITCH_TEST = 50;
static const TtsSpeed     TTS_NUANCE_SPEED_TEST = 100;
static const TtsVolume    TTS_NUANCE_VOLUME_TEST = 100;
static const INT          TTS_NUANCE_RATE_SAMPLE = 22050;

static const INT   TTS_NUANCE_MAX_PARAM_COUNT = 24;
static const INT   TTS_NUANCE_LANG_ID_UKENGLISH = 1;
static const INT   TTS_NUANCE_LANG_ID_ERROR         = -1;

/**
* TTS_EngineNuance_Test
*
* The class is just for TTS_EngineNuance test.
*/
class TTS_EngineNuance_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        BL_FullFile dataFile;
        BL_String cConfTest(TTS_NUANCE_CONF_TEST);
        dataFile.MakePhysicalFullPath(cConfTest, m_strConf);
    }

    virtual void TearDown()
    {
    }

public:
    BL_String m_strConf;
};

// class TTS_Engine::Listener no longer exist
#if 0
/**
* Test_Listener
*
* The class is just for TTS_Engine test.
*/
class Test_Listener : public TTS_Engine::Listener
{
public:
    virtual ~Test_Listener() {}
    virtual CL_BOOL SpeechBuffer(BYTE** pBuffer, DWORD* dwSize);
    virtual VOID SpeechReady(BYTE* pData, DWORD dwLen);
};

CL_BOOL Test_Listener::SpeechBuffer (BYTE** pBuffer, DWORD* dwSize)
{
    return CL_TRUE;
}

VOID Test_Listener::SpeechReady(BYTE* pData, DWORD dwLen)
{
}

#endif

TEST_F(TTS_EngineNuance_Test, destruct_NULLPlatform)
{
    TTS_EngineNuance tts;
    delete tts.m_platform;
    tts.m_platform = NULL;
}

TEST_F(TTS_EngineNuance_Test, TTS_UserData_bufferFalse)
{
    std::string name = "suntec";
    TTS_EngineNuance::TTS_UserData tts_user(name, NULL, 10);

}

TEST_F(TTS_EngineNuance_Test, TTS_UserData_bufferTrue)
{
    std::string name = "suntec";
    unsigned char* buffer = new unsigned char[10];
    TTS_EngineNuance::TTS_UserData tts_user(name, buffer, 10);

}


TEST_F(TTS_EngineNuance_Test, Uninit_True)
{
    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 10;

    tts.Uninit();
}

TEST_F(TTS_EngineNuance_Test, Uninit_Error)
{
    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 0;

    tts.Uninit();
}

TEST_F(TTS_EngineNuance_Test, SetCallBack_NULLnuance)
{
    VE_HINSTANCE hinst;
    hinst.u32Check = 0;
    TTS_EngineNuance tts;
    NUAN_ERROR eret = tts.SetCallBack(hinst);
    EXPECT_EQ(eret, NUAN_E_NOK);
}

// temp!!!
TEST_F(TTS_EngineNuance_Test, SetCallBack_NULLbuffer)
{
    VE_HINSTANCE hinst;
    hinst.u32Check = 1;

    TTS_EngineNuance tts;
    NUAN_ERROR eret = tts.SetCallBack(hinst);
    EXPECT_EQ(eret, NUAN_E_NOK);
}

TEST_F(TTS_EngineNuance_Test, SetCallBack_VE_MSG_BEGINPROCESS)
{
    VE_HINSTANCE hinst;
    hinst.u32Check = 2;    

    TTS_EngineNuance tts;
    NUAN_ERROR eret = tts.SetCallBack(hinst);
    EXPECT_EQ(eret, NUAN_OK);

}

TEST_F(TTS_EngineNuance_Test, SetCallBack_VE_MSG_ENDPROCESS)
{
    VE_HINSTANCE hinst;
    hinst.u32Check = 3;

    TTS_EngineNuance tts;
    NUAN_ERROR eret = tts.SetCallBack(hinst);
    EXPECT_EQ(eret, NUAN_OK);
}

TEST_F(TTS_EngineNuance_Test, SetCallBack_VE_MSG_OUTBUFDONE_Zero)
{
    VE_HINSTANCE hinst;
    hinst.u32Check = 4;

    TTS_EngineNuance tts;
    NUAN_ERROR eret = tts.SetCallBack(hinst);
    EXPECT_EQ(eret, NUAN_OK);

}

TEST_F(TTS_EngineNuance_Test, SetCallBack_VE_MSG_OUTBUFDONE_NotZero)
{
    VE_HINSTANCE hinst;
    hinst.u32Check = 5;

    TTS_EngineNuance tts;
    NUAN_ERROR eret = tts.SetCallBack(hinst);
    EXPECT_EQ(eret, NUAN_OK);

}

TEST_F(TTS_EngineNuance_Test, SetCallBack_default)
{
    VE_HINSTANCE hinst;
    hinst.u32Check = 6;

    TTS_EngineNuance tts;
    NUAN_ERROR eret = tts.SetCallBack(hinst);
    EXPECT_EQ(eret, NUAN_OK);
}

TEST_F(TTS_EngineNuance_Test, SetCallBack_VE_MSG_OUTBUFREQ_NULLbuffer_ZeroLen)
{
    VE_HINSTANCE hinst;
    hinst.u32Check = 7;

    TTS_EngineNuance tts;
    NUAN_ERROR eret = tts.SetCallBack(hinst);
    EXPECT_EQ(eret, NUAN_E_TTS_USERSTOP);
}

TEST_F(TTS_EngineNuance_Test, ReadFile_fopenFail)
{
    std::string filename = "/suntec/mfb.txt";
    char* buffer = NULL;

    TTS_EngineNuance tts;
    int iret = tts.ReadFile(filename, buffer);
    EXPECT_EQ(-1, iret);
}

TEST_F(TTS_EngineNuance_Test, ReadFile_fopensuccess)
{
    std::string filename = "/tmp/mfb.txt";
    FILE* ff = fopen(filename.c_str(), "w+");
    int iwrite = 10;
    if (ff) {
        fwrite(&iwrite, sizeof(iwrite), 1, ff);
        std::cout << "*********fwrite success************" << std::endl;
        fclose(ff);
    }
    else {
        std::cout << "|||||||||||fopen failed|||||||||" << std::endl;
    }

    char* buffer = NULL;

    TTS_EngineNuance tts;
    int iret = tts.ReadFile(filename, buffer);
    EXPECT_EQ(4, iret);
}

TEST_F(TTS_EngineNuance_Test, LoadBrokerInfo_twoparams_opendirfailed)
{
    std::string path = "/suntec/";
    std::string brokeinfo = "suntec";

    TTS_EngineNuance tts;
    bool bret = tts.LoadBrokerInfo(path, brokeinfo);
    EXPECT_FALSE(bret);
}

TEST_F(TTS_EngineNuance_Test, LoadBrokerInfo_twoparams_readdirfailed)
{
    std::string path = "/tmp/";
    std::string brokeinfo = "suntec";

    // struct dirent {
    //     ino_t          d_ino;       /* inode number */
    //     off_t          d_off;       /* offset to the next dirent */
    //     unsigned short d_reclen;     length of this record 
    //     unsigned char  d_type;      /* type of file; not supported
    //                                   by all file system types */
    //     char           d_name[256]; /* filename */
    // };

    struct dirent* p_dir = new dirent;
    delete p_dir;
    p_dir = NULL;

    FUNC_FAKE_SINGRET_SET(readdir, p_dir);

    TTS_EngineNuance tts;
    bool bret = tts.LoadBrokerInfo(path, brokeinfo);
    EXPECT_TRUE(bret);

    FUNC_FAKE_RESET1(readdir);
}

TEST_F(TTS_EngineNuance_Test, LoadBrokerInfo_twoparams_NotDT_REG)
{
    std::string path = "/tmp/";
    std::string brokeinfo = "suntec";

    // struct dirent {
    //     ino_t          d_ino;       /* inode number */
    //     off_t          d_off;       /* offset to the next dirent */
    //     unsigned short d_reclen;     length of this record 
    //     unsigned char  d_type;      /* type of file; not supported
    //                                   by all file system types */
    //     char           d_name[256]; /* filename */
    // };

    struct dirent* p_dir = new dirent;
    p_dir->d_type = DT_DIR;

    struct dirent* p_dirNull = NULL;

    FUNC_FAKE_SEQRET_SET(readdir, struct dirent*, myReturnVals, p_dir, p_dirNull);

    TTS_EngineNuance tts;
    bool bret = tts.LoadBrokerInfo(path, brokeinfo);
    EXPECT_TRUE(bret);

    delete p_dir;
    FUNC_FAKE_RESET1(readdir);
}

TEST_F(TTS_EngineNuance_Test, LoadBrokerInfo_twoparams_DT_REG_Nothdr)
{
    std::string path = "/tmp/";
    std::string brokeinfo = "suntec";

    // struct dirent {
    //     ino_t          d_ino;       /* inode number */
    //     off_t          d_off;       /* offset to the next dirent */
    //     unsigned short d_reclen;     length of this record 
    //     unsigned char  d_type;      /* type of file; not supported
    //                                   by all file system types */
    //     char           d_name[256]; /* filename */
    // };

    struct dirent* p_dir = new dirent;
    memset(p_dir, 0, sizeof(struct dirent));
    p_dir->d_type = DT_REG;
    strncpy(p_dir->d_name, "ff", 2);

    struct dirent* p_dirpoint = new dirent;
    memset(p_dirpoint, 0, sizeof(struct dirent));
    p_dirpoint->d_type = DT_REG;
    strncpy(p_dirpoint->d_name, ".testcp", 7);

    struct dirent* p_dircirclefalse = new dirent;
    memset(p_dircirclefalse, 0, sizeof(struct dirent));
    p_dircirclefalse->d_type = DT_REG;
    strncpy(p_dircirclefalse->d_name, "suntec.txt", 7);

    struct dirent* p_dirNull = NULL;

    FUNC_FAKE_SEQRET_SET(readdir, struct dirent*, myReturnVals, p_dir, p_dirpoint, 
        p_dircirclefalse, p_dirNull);

    TTS_EngineNuance tts;
    bool bret = tts.LoadBrokerInfo(path, brokeinfo);
    EXPECT_TRUE(bret);

    delete p_dir;
    delete p_dirpoint;
    delete p_dircirclefalse;
    FUNC_FAKE_RESET1(readdir);
}

TEST_F(TTS_EngineNuance_Test, LoadBrokerInfo_twoparams_bufsizebigzero_bufNotNULL)
{
    std::string path = "/tmp/";
    std::string brokeinfo = "suntec";

    // struct dirent {
    //     ino_t          d_ino;       /* inode number */
    //     off_t          d_off;       /* offset to the next dirent */
    //     unsigned short d_reclen;     length of this record 
    //     unsigned char  d_type;      /* type of file; not supported
    //                                   by all file system types */
    //     char           d_name[256]; /* filename */
    // };

    FILE* ff = NULL;
    ff = fopen("/tmp/mfb.hdr", "w+");
    if (ff) {
        std::string name = "mengfanbing";
        int iw = fwrite(name.c_str(), 1, name.size(), ff);
        std::cout << "********iw is " << iw << "*************" << std::endl;
        fclose(ff);

    }

    struct dirent* p_dirhdr = new dirent;
    memset(p_dirhdr, 0, sizeof(struct dirent));
    p_dirhdr->d_type = DT_REG;
    strncpy(p_dirhdr->d_name, "mfb.hdr", 7);

    struct dirent* p_dirhdrnull = new dirent;
    memset(p_dirhdrnull, 0, sizeof(struct dirent));
    p_dirhdrnull->d_type = DT_REG;
    strncpy(p_dirhdrnull->d_name, "wrong.hdr", 9);

    struct dirent* p_dirNull = NULL;

    FUNC_FAKE_SEQRET_SET(readdir, struct dirent*, myReturnVals, p_dirhdr, p_dirhdrnull, p_dirNull);

    TTS_EngineNuance tts;
    bool bret = tts.LoadBrokerInfo(path, brokeinfo);
    EXPECT_TRUE(bret);

    delete p_dirhdr;
    delete p_dirhdrnull;
    FUNC_FAKE_RESET1(readdir);
}

TEST_F(TTS_EngineNuance_Test, LoadBrokerInfo_threeparams_bothtrueorbothfalse)
{
    VoiceVector<std::string>::type roleFiles;
    roleFiles.push_back(std::string("mfb.hdr"));
    roleFiles.push_back(std::string("wrong.hdr"));

    std::string path = "/tmp/";
    std::string brokeinfo;

    TTS_EngineNuance tts;
    bool bret = tts.LoadBrokerInfo(path, roleFiles, brokeinfo);
    EXPECT_TRUE(bret);
}

TEST_F(TTS_EngineNuance_Test, LoadDataFileInfo_opendirFailed)
{
    std::string path = "/suntec/";
    VoiceVector<TTS_Platform::TTS_EngineDataFile>::type dataFiles;

    TTS_EngineNuance tts;
    bool bret = tts.LoadDataFileInfo(path, dataFiles);
    EXPECT_FALSE(bret);
}

TEST_F(TTS_EngineNuance_Test, LoadDataFileInfo_readdirFailed)
{
    std::string path = "/tmp/";
    VoiceVector<TTS_Platform::TTS_EngineDataFile>::type dataFiles;

    struct dirent* p_dirNull = NULL;

    FUNC_FAKE_SINGRET_SET(readdir, p_dirNull);

    TTS_EngineNuance tts;
    bool bret = tts.LoadDataFileInfo(path, dataFiles);
    EXPECT_TRUE(bret);

    FUNC_FAKE_RESET1(readdir);
}

TEST_F(TTS_EngineNuance_Test, LoadDataFileInfo_DT_REG)
{
    std::string path = "/tmp/";
    VoiceVector<TTS_Platform::TTS_EngineDataFile>::type dataFiles;

    struct dirent* p_dirhdr = new dirent;
    memset(p_dirhdr, 0, sizeof(struct dirent));
    p_dirhdr->d_type = DT_DIR;
    strncpy(p_dirhdr->d_name, "mfb.hdr", 7);

    struct dirent* p_dirhdrreg = new dirent;
    memset(p_dirhdrreg, 0, sizeof(struct dirent));
    p_dirhdrreg->d_type = DT_REG;
    strncpy(p_dirhdrreg->d_name, "mfb.dat", 7);

    struct dirent* p_dirhdrtxt = new dirent;
    memset(p_dirhdrtxt, 0, sizeof(struct dirent));
    p_dirhdrtxt->d_type = DT_REG;
    strncpy(p_dirhdrtxt->d_name, "mfb.txt", 7);

    struct dirent* p_dirNull = NULL;

    FUNC_FAKE_SEQRET_SET(readdir, struct dirent*, myReturnVals, p_dirhdr, p_dirhdrreg,
        p_dirhdrtxt, p_dirNull);

    TTS_EngineNuance tts;
    bool bret = tts.LoadDataFileInfo(path, dataFiles);
    EXPECT_TRUE(bret);

    delete p_dirhdr;
    delete p_dirhdrreg;
    delete p_dirhdrtxt;
    FUNC_FAKE_RESET1(readdir);
}

TEST_F(TTS_EngineNuance_Test, InitDataFile)
{
    TtsEngineLanguages vecenginelang;
    VoiceVector<TTS_Platform::TTS_EngineDataFile>::type dataFiles;
    std::string strbrokeinfo;

    TTS_EngineLanguage englang1;
    TTS_EngineLanguage englang2;
    vecenginelang.push_back(englang1);
    vecenginelang.push_back(englang2);

    englang1.name = "eng";
    TTS_EngineRole engrole;
    engrole.roleFile = "suntec";
    englang1.roles.push_back(engrole);

    englang2.roles.clear();

    TTS_EngineNuance tts;
    tts.InitDataFile(vecenginelang, strbrokeinfo, dataFiles);
}

TEST_F(TTS_EngineNuance_Test, GetLanguageInfo_False)
{
    std::string langcode = "eng";
    VE_LANGUAGE lang;

    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 0;

    bool bret = tts.GetLanguageInfo(langcode, lang);
    EXPECT_FALSE(bret);
}

TEST_F(TTS_EngineNuance_Test, GetLanguageInfo_True)
{
    std::string langcode = "eng";
    VE_LANGUAGE lang;

    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 1;

    bool bret = tts.GetLanguageInfo(langcode, lang);
    EXPECT_TRUE(bret);
}

TEST_F(TTS_EngineNuance_Test, SetParamValue_False)
{
    TTS_EngineLanguage enginelang;
    enginelang.name = "eng";

    VE_PARAM ttsParam[24];
    NUAN_U16 cttsParam = 0;
    memset(ttsParam, 0, sizeof(ttsParam));

    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 0;
    bool bret = tts.SetParamValue(enginelang, ttsParam, cttsParam);

    EXPECT_FALSE(bret);
}

TEST_F(TTS_EngineNuance_Test, SetParamValue_True)
{
    TTS_EngineLanguage enginelang;
    enginelang.name = "eng";
    enginelang.defaultRole = 1;
    TTS_EngineRole role;
    role.roleName = "componet";
    enginelang.roles.push_back(role);

    VE_PARAM ttsParam[24];
    NUAN_U16 cttsParam = 0;
    memset(ttsParam, 0, sizeof(ttsParam));

    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 1;
    bool bret = tts.SetParamValue(enginelang, ttsParam, cttsParam);

    EXPECT_TRUE(bret);
}

TEST_F(TTS_EngineNuance_Test, Open_ve_ttsOpenFailed)
{
    TTS_EngineLanguage enginelang;
    enginelang.name = "eng";
    enginelang.defaultRole = 1;
    TTS_EngineRole role;
    role.roleName = "componet";
    enginelang.roles.push_back(role);


    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 0;
    bool bret = tts.Open(enginelang);

    EXPECT_FALSE(bret);
}

TEST_F(TTS_EngineNuance_Test, Open_ve_ttsOpenSuccess_SetPramTrue_SetListFailed)
{
    TTS_EngineLanguage enginelang;
    enginelang.name = "eng";
    enginelang.defaultRole = 1;
    TTS_EngineRole role;
    role.roleName = "componet";
    enginelang.roles.push_back(role);


    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 1;
    bool bret = tts.Open(enginelang);

    EXPECT_FALSE(bret);
}

TEST_F(TTS_EngineNuance_Test, Open_ve_ttsOpenSuccess_SetParamFalse_SetListTrue)
{
    TTS_EngineLanguage enginelang;
    enginelang.name = "eng";
    enginelang.defaultRole = 1;
    TTS_EngineRole role;
    role.roleName = "componet";
    enginelang.roles.push_back(role);


    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 2;
    bool bret = tts.Open(enginelang);

    EXPECT_TRUE(bret);
}


TEST_F(TTS_EngineNuance_Test, LoadUserResourceFile_dcb)
{
    std::string path = "/tmp/";
    std::string filename = "suntec.dcb";
    std::shared_ptr<TTS_EngineNuance::TTS_UserResource> source;

    TTS_EngineNuance tts;
    tts.LoadUserResourceFile(path, filename, source);
}

TEST_F(TTS_EngineNuance_Test, LoadUserResourceFile_rul)
{
    std::string path = "/tmp/";
    std::string filename = "suntec.rul";
    std::shared_ptr<TTS_EngineNuance::TTS_UserResource> source;

    TTS_EngineNuance tts;
    tts.LoadUserResourceFile(path, filename, source);
}

TEST_F(TTS_EngineNuance_Test, LoadUserResourceFile_dat)
{
    std::string path = "/tmp/";
    std::string filename = "suntec.dat";
    std::shared_ptr<TTS_EngineNuance::TTS_UserResource> source;

    TTS_EngineNuance tts;
    tts.LoadUserResourceFile(path, filename, source);
}

TEST_F(TTS_EngineNuance_Test, LoadUserResourceFile_txt)
{
    std::string path = "/tmp/";
    std::string filename = "suntec.txt";
    std::shared_ptr<TTS_EngineNuance::TTS_UserResource> source;

    TTS_EngineNuance tts;
    tts.LoadUserResourceFile(path, filename, source);
}

TEST_F(TTS_EngineNuance_Test, LoadUserResources_opendirFailed)
{
    std::string path = "/suntec/";
    std::shared_ptr<TTS_EngineNuance::TTS_UserResource> source;

    TTS_EngineNuance tts;
    tts.LoadUserResources(path, source);
}

TEST_F(TTS_EngineNuance_Test, LoadUserResources_readdirFailed)
{
    std::string path = "/tmp/";
    std::shared_ptr<TTS_EngineNuance::TTS_UserResource> source;

    // struct dirent {
    //     ino_t          d_ino;       /* inode number */
    //     off_t          d_off;       /* offset to the next dirent */
    //     unsigned short d_reclen;     length of this record 
    //     unsigned char  d_type;      /* type of file; not supported
    //                                   by all file system types */
    //     char           d_name[256]; /* filename */
    // };

    struct dirent* p_dirrul = new dirent;
    memset(p_dirrul, 0, sizeof(struct dirent));
    p_dirrul->d_type = DT_REG;
    strncpy(p_dirrul->d_name, "suntec.rul", 10);

    struct dirent* p_dir = new dirent;
    memset(p_dir, 0, sizeof(struct dirent));
    p_dir->d_type = DT_DIR;
    strncpy(p_dir->d_name, ".", 1);

    struct dirent* p_dirup = new dirent;
    memset(p_dirup, 0, sizeof(struct dirent));
    p_dirup->d_type = DT_DIR;
    strncpy(p_dirup->d_name, "..", 2);

    struct dirent* p_dirsub = new dirent;
    memset(p_dirsub, 0, sizeof(struct dirent));
    p_dirsub->d_type = DT_REG;
    strncpy(p_dirsub->d_name, "data", 4);

    struct dirent* p_dirother = new dirent;
    memset(p_dirother, 0, sizeof(struct dirent));
    p_dirother->d_type = DT_SOCK;
    strncpy(p_dirother->d_name, "sock", 4);

    struct dirent* p_dirNull = NULL;

    FUNC_FAKE_SEQRET_SET(readdir, struct dirent*, myReturnVals, p_dirrul, p_dir, 
        p_dirup, p_dirsub, p_dirother, p_dirNull);

    TTS_EngineNuance tts;
    tts.LoadUserResources(path, source);

    delete p_dir;
    delete p_dirrul;
    delete p_dirup;
    delete p_dirsub;
    delete p_dirother;
    FUNC_FAKE_RESET1(readdir);
}

TEST_F(TTS_EngineNuance_Test, init_ve_ttsInitializeFalse)
{
    const char* conf = "/tmp/";
    TTS_EngineInfo vecenginelang;
    std::string deflang = "eng";

    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 0;
    int iret = tts.Init(conf, vecenginelang);

    EXPECT_EQ(iret, 0);
}

TEST_F(TTS_EngineNuance_Test, init_ve_ttsInitializeFalse_m_speech2)
{
    const char* conf = "/tmp/";
    TTS_EngineInfo vecenginelang;
    std::string deflang = "eng";

    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 2;
    int iret = tts.Init(conf, vecenginelang);

    EXPECT_EQ(iret, 0);
}

TEST_F(TTS_EngineNuance_Test, init_ve_ttsInitializeFalse_m_speech3_s_ILock_ptrNULL)
{
    const char* conf = "/tmp/";
    TTS_EngineInfo vecenginelang;
    std::string deflang = "eng";

    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 3;
    int iret = tts.Init(conf, vecenginelang);

    EXPECT_EQ(iret, 0);
}

TEST_F(TTS_EngineNuance_Test, init_ve_ttsInitializeFalse_m_speech3_s_ILock_startselfthread_endnotselfthread)
{
    const char* conf = "/tmp/";
    TTS_EngineInfo vecenginelang;
    std::string deflang = "eng";

    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 4;
    int iret = tts.Init(conf, vecenginelang);

    EXPECT_EQ(iret, 0);
}

TEST_F(TTS_EngineNuance_Test, init_ve_ttsInitializeTrue)
{
    const char* conf = "/tmp/";
    TTS_EngineInfo vecenginelang;
    std::string deflang = "eng";

    TTS_EngineNuance tts;
    tts.m_speech.u32Check = 1;
    int iret = tts.Init(conf, vecenginelang);

    // EXPECT_EQ(iret, 0);
}

TEST_F(TTS_EngineNuance_Test, SynthesizeText_languageempty_mapempty)
{
    const char* pszText = "hello";
    std::string strmodule = "TTSNUANCE";
    std::string lang = "";
    TTS_EngineBuffers* buffer = new TTS_EngineBuffers();

    TTS_EngineNuance tts;
    tts.m_defaultLang = "eng";
    bool bret = tts.SynthesizeText(pszText, strmodule, lang, buffer);

    EXPECT_FALSE(bret);
}

TEST_F(TTS_EngineNuance_Test, SynthesizeText_languageNotempty_mapfind_error)
{
    const char* pszText = "hello";
    std::string strmodule = "TTSNUANCE";
    std::string lang = "eng";
    TTS_EngineBuffers* buffer = new TTS_EngineBuffers();

    TTS_EngineNuance tts;
    tts.m_defaultLang = "eng";
    VE_HINSTANCE hinst;
    hinst.u32Check = 0;
    tts.m_mapInstance.insert(std::make_pair(std::string("eng"), hinst));
    bool bret = tts.SynthesizeText(pszText, strmodule, lang, buffer);

    EXPECT_FALSE(bret);
}

TEST_F(TTS_EngineNuance_Test, SynthesizeText_languageNotempty_mapfind_true)
{
    const char* pszText = "hello";
    std::string strmodule = "TTSNUANCE";
    std::string lang = "eng";
    TTS_EngineBuffers* buffer = new TTS_EngineBuffers();

    TTS_EngineNuance tts;
    tts.m_defaultLang = "eng";
    VE_HINSTANCE hinst;
    hinst.u32Check = 1;
    tts.m_mapInstance.insert(std::make_pair(std::string("eng"), hinst));
    bool bret = tts.SynthesizeText(pszText, strmodule, lang, buffer);

    EXPECT_TRUE(bret);
}

TEST_F(TTS_EngineNuance_Test, Stop_ST_INIT)
{
    std::string lang = "eng";

    TTS_EngineNuance tts;
    tts.m_state = TTS_EngineNuance::ST_INIT;
    CL_ERROR eret = tts.Stop(lang);

    EXPECT_EQ(eret, TTS_ERR_NO_ERROR);
}

TEST_F(TTS_EngineNuance_Test, Stop_ST_RUNING_mapempty)
{
    std::string lang = "eng";

    TTS_EngineNuance tts;
    tts.m_state = TTS_EngineNuance::ST_RUNING;
    CL_ERROR eret = tts.Stop(lang);

    EXPECT_EQ(eret, TTS_ERR_INVALID_PARAM);
}

TEST_F(TTS_EngineNuance_Test, Stop_ve_ttsStopError)
{
    std::string lang = "eng";

    TTS_EngineNuance tts;
    tts.m_state = TTS_EngineNuance::ST_RUNING;
    VE_HINSTANCE hinst;
    hinst.u32Check = 0;
    tts.m_mapInstance.insert(std::make_pair(std::string("eng"), hinst));

    CL_ERROR eret = tts.Stop(lang);

    EXPECT_EQ(eret, TTS_ERR_NO_ERROR);
}

TEST_F(TTS_EngineNuance_Test, Stop_ve_ttsStopTrue)
{
    std::string lang = "eng";

    TTS_EngineNuance tts;
    tts.m_state = TTS_EngineNuance::ST_RUNING;
    VE_HINSTANCE hinst;
    hinst.u32Check = 1;
    tts.m_mapInstance.insert(std::make_pair(std::string("eng"), hinst));

    CL_ERROR eret = tts.Stop(lang);

    EXPECT_EQ(eret, TTS_ERR_NO_ERROR);
}

TEST_F(TTS_EngineNuance_Test, Close)
{
    TTS_EngineNuance tts;

    VE_HINSTANCE hinst;
    hinst.u32Check = 1;
    tts.m_mapInstance.insert(std::make_pair(std::string("eng"), hinst));

    VE_HINSTANCE hinst2;
    hinst2.u32Check = 0;
    tts.m_mapInstance.insert(std::make_pair(std::string("eng"), hinst2));

    NUAN_ERROR eret = tts.Close();
    EXPECT_EQ(NUAN_OK, eret);
}

TEST_F(TTS_EngineNuance_Test, ResetEngineParam_mapempty)
{
    struct TTS_EngineLanguage enginelang;
    enginelang.name = "eng";

    TTS_EngineNuance tts;
    tts.ResetEngineParam(enginelang);
}

TEST_F(TTS_EngineNuance_Test, ResetEngineParam_ve_ttsSetParamListFalse)
{
    struct TTS_EngineLanguage enginelang;
    enginelang.name = "eng";
    TTS_EngineRole role;
    role.roleName = "componet";
    enginelang.roles.push_back(role);
    enginelang.defaultRole = 1;
    
    TTS_EngineNuance tts;
    VE_HINSTANCE hinst;
    hinst.u32Check = 0;
    tts.m_mapInstance.insert(std::make_pair(std::string("eng"), hinst));

    tts.ResetEngineParam(enginelang);
}

TEST_F(TTS_EngineNuance_Test, ResetEngineParam_ve_ttsSetParamListTrue)
{
    struct TTS_EngineLanguage enginelang;
    enginelang.name = "eng";
    TTS_EngineRole role;
    role.roleName = "componet";
    enginelang.roles.push_back(role);
    enginelang.defaultRole = 1;
    
    TTS_EngineNuance tts;
    VE_HINSTANCE hinst;
    hinst.u32Check = 1;
    tts.m_mapInstance.insert(std::make_pair(std::string("eng"), hinst));

    tts.ResetEngineParam(enginelang);
}

/* EOF */
