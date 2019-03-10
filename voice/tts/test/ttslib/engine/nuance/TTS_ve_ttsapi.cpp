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
#include "ve_ttsapi.h"
#include "ve_platform.h"
#include "TTS_EngineNuance.h"
#include "TTS_EngineCommDef.h"
#include "TTS_PlatformAccessor.h"
#include "TTS_Platform.h"
#include "TTS_File.h"
#include "TTS_file_test.h"
#include "TTS_PlatformFileManager.h"
#include "MEM_vector.h"

extern "C" 
{
typedef NUAN_ERROR (*p_outCb) (VE_HINSTANCE hTtsInst, 
                               VE_OUTDEV_HINSTANCE hOutDevInst, 
                               VE_CALLBACKMSG* pcbMessage, 
                               VE_USERDATA userData);

// typedef NUAN_ERROR (*pfOpen)(VE_DATA_HCLASS           hDataClass,
//                        VE_HEAP_HINSTANCE        hHeap,
//                        const char             * szBrokerString,
//                        VE_DATA_HMAPPING       * phMapping);

// typedef NUAN_ERROR (*pfClose)(VE_DATA_HMAPPING  hMapping);

// typedef NUAN_ERROR (*pfMap)(VE_DATA_HMAPPING       hMapping,
//                       NUAN_U32               u32Offset,
//                       NUAN_U32             * pcBytes,
//                       const void          ** ppData);

// typedef NUAN_ERROR (*pfUnmap)(VE_DATA_HMAPPING       hMapping,
//                         const void           * pData);

// typedef NUAN_ERROR (*pfFreeze)(VE_DATA_HMAPPING  hMapping);

void funcmap(const VE_DATA_MAPPING_INTERFACE* mapinst, VE_HSPEECH* phTtsCl);
void funcstream(const VE_DATA_STREAM_INTERFACE* streaminst, VE_HSPEECH* phTtsCl);
void funccritsec(const VE_CRITSEC_INTERFACE* critsecinst, VE_HSPEECH* phTtsCl);
void funcheap(const VE_HEAP_INTERFACE* heapinst);

NUAN_ERROR ve_ttsInitialize (
  const VE_INSTALL   * pResources,
        VE_HSPEECH   * phTtsCl)
{
    if (0 == phTtsCl->u32Check) {
        return NUAN_E_NOK;
    }

    if (NULL == pResources || NULL == phTtsCl) {
        return NUAN_E_NOK;
    }

    if (phTtsCl->u32Check == 1) {
        return NUAN_OK;
    }

    const VE_DATA_MAPPING_INTERFACE* mapinst = pResources->pIDataMapping;
    funcmap(mapinst, phTtsCl);

    const VE_DATA_STREAM_INTERFACE* streaminst = pResources->pIDataStream;
    funcstream(streaminst, phTtsCl);

    const VE_CRITSEC_INTERFACE* critsecinst = pResources->pICritSec;
    funccritsec(critsecinst, phTtsCl);

    const VE_HEAP_INTERFACE* heapinst = pResources->pIHeap;
    funcheap(heapinst);

    return NUAN_E_NOK;
}

void funcheap(const VE_HEAP_INTERFACE* heapinst)
{
    void* ptr = heapinst->pfMalloc(NULL, 10);
    free(ptr);
    ptr = heapinst->pfCalloc(NULL, 10, 1);
    void* ptr2 = heapinst->pfRealloc(NULL, ptr2, 11);
    heapinst->pfFree(NULL, ptr2);
}

void funccritsec(const VE_CRITSEC_INTERFACE* critsecinst, VE_HSPEECH* phTtsCl)
{
    {
        void* p_void = NULL;
        critsecinst->pfOpen(NULL, NULL, &p_void);
        critsecinst->pfEnter(p_void);
        critsecinst->pfLeave(p_void);
        critsecinst->pfClose(p_void);
    }

    if (3 == phTtsCl->u32Check) {
        void** pp_void = NULL;
        critsecinst->pfOpen(NULL, NULL, pp_void);
        critsecinst->pfEnter(NULL);
        critsecinst->pfLeave(NULL);
    }

    if (4 == phTtsCl->u32Check) {
        void* p_void = NULL;
        critsecinst->pfOpen(NULL, NULL, &p_void);
        critsecinst->pfLeave(p_void);
        critsecinst->pfEnter(p_void);
        critsecinst->pfEnter(p_void);
        critsecinst->pfLeave(p_void);
        critsecinst->pfLeave(p_void);
        critsecinst->pfClose(p_void);
    }

    {
        void* p_void = NULL;
        critsecinst->pfOpen(NULL, NULL, &p_void);
        critsecinst->pfEnter(p_void);
        critsecinst->pfLeave(p_void);
        critsecinst->pfLeave(p_void);
        critsecinst->pfClose(p_void);
    }
}

void funcstream(const VE_DATA_STREAM_INTERFACE* streaminst, VE_HSPEECH* phTtsCl)
{
    TTS_File s_file;
    s_file.Open("/tmp/funcstream.txt", "w+");
    // std::string strcontent = "funcstream";
    // s_file.Write(strcontent.c_str(), 1, strcontent.size());
    s_file.Write("mengfanbing", 1, 11);
    s_file.Close();

    {
        streaminst->pfOpen(NULL, NULL, NULL, NULL, NULL);
    }

    {
        int i = 10;
        streaminst->pfOpen(&i, NULL, NULL, "rw", NULL);
    }

    {
        int i = 10;
        streaminst->pfOpen(&i, NULL, "mm", NULL, NULL);
    }

    {
        streaminst->pfOpen(NULL, NULL, "mm", "rw", NULL);
    }

    {
        char* szname = new char[5000];
        memset(szname, 0, strlen(szname));
        for (int i = 0; i < 5000; i++) {
            szname[i] = 'a';
        }
        VoiceVector<TTS_Platform::TTS_EngineDataFile>::type vecdataFiles;
        void* p_void = NULL;
        std::cout << "*****before suntec pfOpen*****" << std::endl;
        streaminst->pfOpen(&vecdataFiles, NULL, szname, "rw", &p_void);
        std::cout << "*****after suntec pfOpen*****" << std::endl;
        delete[] szname;
    }

    {
        TTS_File s_file;
        s_file.Open("/tmp/mfb.dat", "wb+");
        std::string strcontent = "hello china!";
        s_file.Write(strcontent.c_str(), 1, strcontent.size());
        s_file.Close();

        VoiceVector<TTS_Platform::TTS_EngineDataFile>::type vecdataFiles;
        TTS_Platform::TTS_EngineDataFile datafile;
        datafile.m_brokerName = "mfb.dat";
        datafile.m_dirPath = "/tmp/";
        vecdataFiles.push_back(datafile);

        void* p_void = NULL;
        std::cout << "*****before mfb.dat pfOpen*****" << std::endl;
        streaminst->pfOpen(&vecdataFiles, NULL, "mfb", "rw", &p_void);
        std::cout << "*****after mfb.dat pfOpen*****" << std::endl;
        TTS_File* file = reinterpret_cast<TTS_File*> (p_void);
        if (file) {
            std::cout << "**before close***" << std::endl;
            file->Close();
        }
    }

    {
        TTS_File s_file;
        s_file.Open("/tmp/mfb.dat", "w+");
        std::string strcontent = "hello china!";
        s_file.Write(strcontent.c_str(), 1, strcontent.size());
        s_file.Close();

        VoiceVector<TTS_Platform::TTS_EngineDataFile>::type vecdataFiles;
        TTS_Platform::TTS_EngineDataFile datafile;
        datafile.m_brokerName = "mfb.dat";
        datafile.m_dirPath = "/tmp/";
        vecdataFiles.push_back(datafile);

        void* p_void = NULL;
        std::cout << "*****before three pfOpen*****" << std::endl;
        streaminst->pfOpen(&vecdataFiles, NULL, "mfb", "w+", &p_void);
        std::cout << "*****after three pfOpen*****" << std::endl;
        TTS_File* file = reinterpret_cast<TTS_File*> (p_void);
        if (file) {
            std::cout << "**before close2***" << std::endl;
            file->Close();
        }
    }

    {
        TTS_File s_file;
        s_file.Open("/tmp/mfb.dat", "w+");
        std::string strcontent = "hello china!";
        s_file.Write(strcontent.c_str(), 1, strcontent.size());
        s_file.Close();

        char* sznamee = new char[8];
        memset(sznamee, 0, strlen(sznamee));
        strncpy(sznamee, "m.fb", 5);
        VoiceVector<TTS_Platform::TTS_EngineDataFile>::type vecdataFiles;
        TTS_Platform::TTS_EngineDataFile datafile;
        datafile.m_brokerName = "m-fb.dat";
        datafile.m_dirPath = "/suntec/";
        vecdataFiles.push_back(datafile);

        void* p_void = NULL;
        std::cout << "*****before four pfOpen*****" << std::endl;
        streaminst->pfOpen(&vecdataFiles, NULL, sznamee, "r+", &p_void);
        std::cout << "*****after three pfOpen*****" << std::endl;
        delete[] sznamee;
    }

    {
        TTS_File* p_file = new TTS_File();
        streaminst->pfClose(p_file);
    }

    {
        streaminst->pfClose(NULL);
    }

    {
        streaminst->pfRead(NULL, 1, 1, NULL);
    }

    {
        int i = 10;
        streaminst->pfRead(NULL, 1, 1, &i);
    }

    {
        int i = 10;
        streaminst->pfRead(&i, 1, 1, NULL);
    }

    {
        TTS_File* p_file = new TTS_File();
        p_file->Open("/tmp/funcstream.txt", "r+");
        p_file->Seek(0, BL_FILE_SEEK_END);

        char* buffer = new char[50];
        memset(buffer, 0, 50);
        // size_t iread = 0;
        // FUNC_FAKE_SINGRET_SET(fread, iread);
        streaminst->pfRead(buffer, 1, 50, p_file);
        printf("***suntec buffer is %s\n", buffer);

        p_file->Close();
        delete p_file;
        // FUNC_FAKE_RESET1(fread);
    }

    {
        TTS_File* p_file = new TTS_File();
        p_file->Open("/tmp/funcstream.txt", "r+");

        char* buffer = new char[50];
        memset(buffer, 0, 50);
        // size_t iread = 10;
        // FUNC_FAKE_SINGRET_SET(fread, iread);
        streaminst->pfRead(buffer, 1, 4, p_file);
        printf("***suntec buffer2 is %s\n", buffer);

        p_file->Close();
        delete p_file;
        // FUNC_FAKE_RESET1(fread);
    }

    {
        streaminst->pfSeek(NULL, 1, VE_STREAM_SEEK_SET, VE_STREAM_BACKWARD);
    }

    {
        TTS_File* p_file = new TTS_File();
        p_file->Open("/tmp/mfb.txt", "r+");

        streaminst->pfSeek(p_file, 1, VE_STREAM_SEEK_SET, VE_STREAM_BACKWARD);
        p_file->Close();
        delete p_file;
    }

    {
        TTS_File* p_file = new TTS_File();
        p_file->Open("/tmp/mfb.txt", "r+");

        streaminst->pfSeek(p_file, 1, VE_STREAM_SEEK_SET, VE_STREAM_FORWARD);
        p_file->Close();
        delete p_file;
    }

    {
        TTS_File* p_file = new TTS_File();
        p_file->Open("/tmp/mfb.txt", "r+");

        streaminst->pfSeek(p_file, 1, VE_STREAM_SEEK_CUR, VE_STREAM_FORWARD);
        p_file->Close();
        delete p_file;
    }

    {
        TTS_File* p_file = new TTS_File();
        p_file->Open("/tmp/mfb.txt", "r+");

        streaminst->pfSeek(p_file, 1, VE_STREAM_SEEK_CUR, VE_STREAM_BACKWARD);
        p_file->Close();
        delete p_file;
    }

    {
        TTS_File* p_file = new TTS_File();
        p_file->Open("/tmp/mfb.txt", "r+");

        streaminst->pfSeek(p_file, 1, VE_STREAM_SEEK_END, VE_STREAM_BACKWARD);
        p_file->Close();
        delete p_file;
    }

    {
        TTS_File* p_file = new TTS_File();
        p_file->Open("/tmp/mfb.txt", "r+");

        streaminst->pfSeek(p_file, 1, VE_STREAM_SEEK_END, VE_STREAM_FORWARD);
        p_file->Close();
        delete p_file;
    }

    {
        TTS_File* p_file = new TTS_File();
        p_file->Open("/tmp/mfb.txt", "r+");

        streaminst->pfSeek(p_file, 1, VE_STREAM_SEEK_END, VE_STREAM_FORWARD);
        p_file->Close();
        delete p_file;
    }

    {
        streaminst->pfGetSize(NULL);
    }

    {
        TTS_File* p_file = new TTS_File();
        p_file->Open("/tmp/mfb.txt", "r+");

        streaminst->pfGetSize(p_file);
        p_file->Close();
        delete p_file;
    }

    streaminst->pfError(NULL);
    streaminst->pfWrite(NULL, 1, 1, NULL);

    {
        TTS_File* p_filett = new TTS_File();
        p_filett->Open("/tmp/funcstreamtt.txt", "w+");
        std::cout << "***suntec before write funcstream success" << std::endl;
        int i = streaminst->pfWrite("suntec", 1, 6, p_filett);
        std::cout << "***suntec after write funcstream success ,return is " << i << std::endl;
        p_filett->Close();
        delete p_filett;
    }

    {
        TTS_File* p_file = new TTS_File();
        // p_file->Open("/tmp/funcstream.txt", "w+");
        // p_file->Close();
        std::cout << "***suntec before write funcstream false" << std::endl;
        int i = streaminst->pfWrite("suntec", 1, 6, p_file);
        std::cout << "***suntec after write funcstream false ,return is " << i << std::endl;
        delete p_file;
    }
}

void funcmap(const VE_DATA_MAPPING_INTERFACE* mapinst, VE_HSPEECH* phTtsCl)
{
    const char* filename = "suntec.txt";
    mapinst->pfOpen(NULL, NULL, filename, NULL);

    TTS_File s_file;
    s_file.Open("/tmp/mfb.txt", "w+");
    std::string strcontent = "hello china!";
    s_file.Write(strcontent.c_str(), 1, strcontent.size());
    s_file.Close();

    {
        char* c_file = new char[5000];
        memset(c_file, 0, strlen(c_file));
        for (int i = 0; i < 5000; i++) {
            c_file[i] = 'a';
        }
        mapinst->pfOpen(NULL, NULL, c_file, NULL);
    }

    {
        VoiceVector<TTS_Platform::TTS_EngineDataFile>::type vecdataclass;
        TTS_Platform::TTS_EngineDataFile datafile;
        datafile.m_brokerName = "notfind";
        vecdataclass.push_back(datafile);
        const char* filename = "/tmp/mapOpen";

        std::cout << "*****in callback mengfanbing before not find*******" << std::endl;
        mapinst->pfOpen(&vecdataclass, NULL, filename, NULL);
    }

    {
        VoiceVector<TTS_Platform::TTS_EngineDataFile>::type vecdataclass;
        TTS_Platform::TTS_EngineDataFile datafile;
        datafile.m_brokerName = "mapopen.dat";
        TTS_File s_file;
        s_file.Open("/tmp/mapopen.dat", "w+");
        std::string strcontent = "hello china!";
        s_file.Write(strcontent.c_str(), 1, strcontent.size());
        s_file.Close();
        datafile.m_dirPath = "/tmp/";
        vecdataclass.push_back(datafile);
        const char* filename = "mapopen";

        TTS_PlatformFileManager::Destroy();
        void* p_void = NULL;
        std::cout << "****in callback mengfanbing before find*****" << std::endl;
        mapinst->pfOpen(&vecdataclass, NULL, filename, &p_void);
    }

    {
        VoiceVector<TTS_Platform::TTS_EngineDataFile>::type vecdataclass;
        TTS_Platform::TTS_EngineDataFile datafile;
        datafile.m_brokerName = "mapopen.dat";
        datafile.m_dirPath = "/tmp/";
        vecdataclass.push_back(datafile);
        const char* filename = "mapopen";

        TTS_PlatformFileManager::Destroy();
        TTS_PlatformFileManager* filemanager = TTS_PlatformFileManager::Instance();
        TTS_File* p_file = new TTS_File();
        p_file->Open("/tmp/mapopen.dat", "r+");

        TTS_PlatformAccessor* accessor = new TTS_PlatformAccessor("/tmp/mapopen.dat", p_file, 1);
        filemanager->m_fileList.Add(accessor);

        std::cout << "****in callback mengfanbing before find pFile*****" << std::endl;
        void* p_void = NULL;
        mapinst->pfOpen(&vecdataclass, NULL, filename, &p_void);
        p_file->Close();
    }
    
    {
        TTS_File* p_fileclose = new TTS_File();
        p_fileclose->Open("/tmp/mfb.txt", "r+");
        TTS_PlatformAccessor* accessor = new TTS_PlatformAccessor("suntec", p_fileclose, 1);
        mapinst->pfClose(accessor);
    }

    {
        TTS_File* p_filemap = new TTS_File();
        p_filemap->Open("/tmp/mfb.txt", "r+");
        TTS_PlatformAccessor* accessor_map = new TTS_PlatformAccessor("suntec", p_filemap, 1);
        accessor_map->m_workBuff.m_bInit = true;

        uint32_t i_map = 0;
        unsigned int* c_map = &i_map;
        void* v_map = reinterpret_cast<void*> (c_map);
        mapinst->pfMap(accessor_map, 0, &i_map, const_cast<const void**> (&v_map));
        delete accessor_map;
    }

    {
        TTS_File s_file;
        s_file.Open("/tmp/map_ppdatanull.txt", "w+");
        std::string strcontent = "hellomeng";
        s_file.Write(strcontent.c_str(), 1, strcontent.size());
        s_file.Close();

        TTS_File* p_filemap = new TTS_File();
        p_filemap->Open("/tmp/map_ppdatanull.txt", "r+");
        TTS_PlatformAccessor* accessor_map = new TTS_PlatformAccessor("/tmp/map_ppdatanull.txt", p_filemap, 1);
        accessor_map->m_workBuff.m_bInit = false;
        p_filemap->Close();

        uint32_t i_map = 0;
        unsigned int* c_map = &i_map;
        void* v_map = NULL;
        std::cout << "before pfMap" << std::endl;
        mapinst->pfMap(accessor_map, 0, &i_map, const_cast<const void**> (&v_map));
        std::cout << "after pfMap" << std::endl;
        delete accessor_map;
    }

    {
        TTS_File* p_fileunmap = new TTS_File();
        p_fileunmap->Open("/tmp/mfb.txt", "r+");
        TTS_PlatformAccessor* accessor_unmap = new TTS_PlatformAccessor("suntec", p_fileunmap, 1);
        unsigned char* pdata = accessor_unmap->m_workBuff.m_buffer;
        mapinst->pfUnmap(accessor_unmap, pdata);
        delete accessor_unmap;
    }

    mapinst->pfFreeze(NULL);
}

NUAN_ERROR  ve_ttsUnInitialize (const VE_HSPEECH hTtsCl)
{
    if (hTtsCl.u32Check == 10) {
        return NUAN_OK;
    }

    if (hTtsCl.u32Check == 0) {
        return -1;
    }

    return NUAN_OK;
}

NUAN_ERROR ve_ttsSetOutDevice(VE_HINSTANCE hTtsInst, VE_OUTDEVINFO* pOutDevInfo)
{
    VE_HINSTANCE htinst = hTtsInst;
    p_outCb p_Fun = reinterpret_cast<p_outCb> (pOutDevInfo->pfOutNotify);

    if (htinst.u32Check == 0) {
        VE_HINSTANCE hinst;
        TTS_EngineNuance* nuanceinst = NULL;
        VE_CALLBACKMSG callbackmsg;
        VE_USERDATA* usrdata = NULL;
        NUAN_ERROR eret = p_Fun(hinst, nuanceinst, &callbackmsg, usrdata);

        return eret;
    }
    else if (htinst.u32Check == 1) {
        VE_HINSTANCE hinst;
        TTS_EngineNuance* nuanceinst = new TTS_EngineNuance();
        VE_CALLBACKMSG callbackmsg;
        VE_USERDATA* usrdata = NULL;

        NUAN_ERROR eret = p_Fun(hinst, nuanceinst, &callbackmsg, usrdata);
        delete nuanceinst;
        return eret;
    }
    else if (htinst.u32Check == 2) {
        VE_HINSTANCE hinst;

        TTS_EngineNuance* nuanceinst = new TTS_EngineNuance();
        TTS_EngineBuffers* buffer = new TTS_EngineBuffers();
        nuanceinst->m_buffer = buffer;

        VE_CALLBACKMSG cbmsg;
        cbmsg.eMessage = VE_MSG_BEGINPROCESS;
        VE_USERDATA* usrdata = NULL;

        NUAN_ERROR eret = p_Fun(hinst, nuanceinst, &cbmsg, usrdata);
        delete buffer;
        delete nuanceinst;
        return eret;
    }
    else if (htinst.u32Check == 3) {
        VE_HINSTANCE hinst;

        TTS_EngineNuance* nuanceinst = new TTS_EngineNuance();
        TTS_EngineBuffers* buffer = new TTS_EngineBuffers();
        nuanceinst->m_buffer = buffer;

        VE_CALLBACKMSG cbmsg;
        cbmsg.eMessage = VE_MSG_ENDPROCESS;
        VE_USERDATA* usrdata = NULL;

        NUAN_ERROR eret = p_Fun(hinst, nuanceinst, &cbmsg, usrdata);
        delete buffer;
        delete nuanceinst;
        return eret;
    }
    else if (4 == htinst.u32Check) {
        VE_HINSTANCE hinst;

        TTS_EngineNuance* nuanceinst = new TTS_EngineNuance();
        TTS_EngineBuffers* buffer = new TTS_EngineBuffers();
        nuanceinst->m_buffer = buffer;

        VE_CALLBACKMSG cbmsg;
        VE_OUTDATA outdata;
        outdata.ulPcmBufLen = 0;
        cbmsg.eMessage = VE_MSG_OUTBUFDONE;
        cbmsg.pParam = static_cast<void*> (&outdata);

        VE_USERDATA* usrdata = NULL;

        NUAN_ERROR eret = p_Fun(hinst, nuanceinst, &cbmsg, usrdata);
        delete buffer;
        delete nuanceinst;
        return eret;
    }
    else if (5 == htinst.u32Check) {
        VE_HINSTANCE hinst;

        TTS_EngineNuance* nuanceinst = new TTS_EngineNuance();
        TTS_EngineBuffers* buffer = new TTS_EngineBuffers();
        nuanceinst->m_buffer = buffer;

        VE_CALLBACKMSG cbmsg;
        VE_OUTDATA outdata;
        outdata.ulPcmBufLen = 1;
        cbmsg.eMessage = VE_MSG_OUTBUFDONE;
        cbmsg.pParam = static_cast<void*> (&outdata);

        VE_USERDATA* usrdata = NULL;

        NUAN_ERROR eret = p_Fun(hinst, nuanceinst, &cbmsg, usrdata);
        delete buffer;
        delete nuanceinst;
        return eret;

    }
    else if (6 == htinst.u32Check) {
        VE_HINSTANCE hinst;

        TTS_EngineNuance* nuanceinst = new TTS_EngineNuance();
        TTS_EngineBuffers* buffer = new TTS_EngineBuffers();
        nuanceinst->m_buffer = buffer;

        VE_CALLBACKMSG cbmsg;
        VE_OUTDATA outdata;
        outdata.ulPcmBufLen = 1;
        cbmsg.eMessage = VE_MSG_RESUME;
        cbmsg.pParam = static_cast<void*> (&outdata);

        VE_USERDATA* usrdata = NULL;

        NUAN_ERROR eret = p_Fun(hinst, nuanceinst, &cbmsg, usrdata);
        delete buffer;
        delete nuanceinst;
        return eret;
    }
    else if (7 == htinst.u32Check) {
        VE_HINSTANCE hinst;

        TTS_EngineNuance* nuanceinst = new TTS_EngineNuance();
        TTS_EngineBuffers* buffer = new TTS_EngineBuffers();
        // buffer->m_size = 0;
        buffer->m_stop = true;
        nuanceinst->m_buffer = buffer;

        VE_CALLBACKMSG cbmsg;
        VE_OUTDATA outdata;
        outdata.ulPcmBufLen = 1;
        cbmsg.eMessage = VE_MSG_OUTBUFREQ;
        cbmsg.pParam = static_cast<void*> (&outdata);

        VE_USERDATA* usrdata = NULL;

        NUAN_ERROR eret = p_Fun(hinst, nuanceinst, &cbmsg, usrdata);
        delete buffer;
        delete nuanceinst;

        {
            VE_HINSTANCE hinst;

            TTS_EngineNuance* nuanceinst = new TTS_EngineNuance();
            TTS_EngineBuffers* buffer = new TTS_EngineBuffers();
            // buffer->m_size = 0;
            buffer->m_stop = true;
            buffer->m_writePos = 11;
            buffer->m_readPos = 12;
            nuanceinst->m_buffer = buffer;

            VE_CALLBACKMSG cbmsg;
            VE_OUTDATA outdata;
            outdata.ulPcmBufLen = 1;
            cbmsg.eMessage = VE_MSG_OUTBUFREQ;
            cbmsg.pParam = static_cast<void*> (&outdata);

            VE_USERDATA* usrdata = NULL;

            NUAN_ERROR eret = p_Fun(hinst, nuanceinst, &cbmsg, usrdata);
            delete buffer;
            delete nuanceinst;
        }

        {
            VE_HINSTANCE hinst;

            TTS_EngineNuance* nuanceinst = new TTS_EngineNuance();
            TTS_EngineBuffers* buffer = new TTS_EngineBuffers();
            // buffer->m_size = 0;
            buffer->m_stop = true;
            buffer->m_writePos = 13;
            buffer->m_readPos = 14;
            nuanceinst->m_buffer = buffer;

            VE_CALLBACKMSG cbmsg;
            VE_OUTDATA outdata;
            outdata.ulPcmBufLen = 1;
            cbmsg.eMessage = VE_MSG_OUTBUFREQ;
            cbmsg.pParam = static_cast<void*> (&outdata);

            VE_USERDATA* usrdata = NULL;

            NUAN_ERROR eret = p_Fun(hinst, nuanceinst, &cbmsg, usrdata);
            delete buffer;
            delete nuanceinst;
        }

        {
            VE_HINSTANCE hinst;

            TTS_EngineNuance* nuanceinst = new TTS_EngineNuance();
            TTS_EngineBuffers* buffer = new TTS_EngineBuffers();
            // buffer->m_size = 0;
            buffer->m_stop = true;
            buffer->m_writePos = 15;
            buffer->m_readPos = 16;
            nuanceinst->m_buffer = buffer;

            VE_CALLBACKMSG cbmsg;
            VE_OUTDATA outdata;
            outdata.ulPcmBufLen = 1;
            cbmsg.eMessage = VE_MSG_OUTBUFREQ;
            cbmsg.pParam = static_cast<void*> (&outdata);

            VE_USERDATA* usrdata = NULL;

            NUAN_ERROR eret = p_Fun(hinst, nuanceinst, &cbmsg, usrdata);
            delete buffer;
            delete nuanceinst;
        }

        return eret;
    }

    return NUAN_OK;
}

NUAN_ERROR ve_ttsGetLanguageList (const VE_HSPEECH hTtsCl, VE_LANGUAGE* pLanguages,
    NUAN_U16* pusNbrOfElements)
{
    if (NULL == pLanguages) {
        *pusNbrOfElements = 2;
        return NUAN_OK;
    }

    for (int i = 0; i < *pusNbrOfElements; i++) {
        VE_LANGUAGE lang;
        memset(&lang, 0, sizeof(lang));
        if (0 == i) {
            strncpy(lang.szLanguageTLW, "mfb", 3);
        }
        else {
            strncpy(lang.szLanguageTLW, "eng", 3);
        }
        memcpy(&pLanguages[i], &lang, sizeof(VE_LANGUAGE));
    }

    if (hTtsCl.u32Check == 0) {
        return NUAN_E_NOK;
    }

    if (hTtsCl.u32Check == 1) {
        return NUAN_OK;
    }

    if (hTtsCl.u32Check == 2) {
        return NUAN_E_NOK;
    }

    return NUAN_OK;
}

NUAN_ERROR ve_ttsOpen (
        VE_HSPEECH           hTtsCl,
        VE_HEAP_HINSTANCE    hHeap,
        VE_LOG_HINSTANCE     hLog,
        VE_HINSTANCE       * phTtsInst,
        VE_USERDATA          UserData)
{
    if (hTtsCl.u32Check == 0) {
        phTtsInst->u32Check = 0;
        return NUAN_E_NOK;
    }

    if (hTtsCl.u32Check == 1) {
        phTtsInst->u32Check = 0;
        return NUAN_OK;
    }

    if (hTtsCl.u32Check == 2) {
        phTtsInst->u32Check = 1;
        return NUAN_OK;
    }

    return NUAN_OK;
}

NUAN_ERROR ve_ttsSetParamList (
  VE_HINSTANCE        hTtsInst,
  VE_PARAM          * pTtsParam,
  NUAN_U16            usNbrOfParam)
{
    if (hTtsInst.u32Check == 0) {
        return NUAN_E_NOK;
    }

    if (hTtsInst.u32Check == 1) {
        return NUAN_OK;
    }

    return NUAN_OK;
}

NUAN_ERROR ve_ttsProcessText2Speech (
        VE_HINSTANCE     hTtsInst,
  const VE_INTEXT      * pInText)
{
    if (0 == hTtsInst.u32Check) {
        return NUAN_E_NOK;
    }

    return NUAN_OK;
}

NUAN_ERROR ve_ttsStop (VE_HINSTANCE hTtsInst)
{
    if (0 == hTtsInst.u32Check) {
        return NUAN_E_NOK;
    }

    return NUAN_OK;
}

NUAN_ERROR ve_ttsClose (VE_HINSTANCE hTtsInst)
{
    if (hTtsInst.u32Check == 0) {
        return NUAN_E_NOK;
    }

    return NUAN_OK;
}

};
/* EOF */
 