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

#include "stdafx.h"

#ifndef VR_CONTROLLER_H
#   include "VR_Controller.h"
#endif

#ifndef VR_CONTROLLABLENULL_H
#   include "VR_ControllableNull.h"
#endif

#include <ncore/NCStartPerformance.h>
#include "MEM_MemoryTrackerIF.h"

#ifndef VR_DIALOGENGINEFACTORY_H
#include "VR_DialogEngineFactory.h"
#endif

#ifndef VR_MODULEIMP_H
#   include "VR_ModuleImp.h"
#endif

using namespace nutshell;

boost::shared_ptr<VR_ModuleImp>  VR_ModuleImp::s_pInstance;


VR_ModuleImp::VR_ModuleImp()
    : m_Controller(new(MEM_Voice) VR_ControllableNull())
    , m_spSourceManager(VR_new VR_SourceManager())
{
    VR_LOGD_FUNC();
}

VR_ModuleImp::~VR_ModuleImp()
{
    VR_LOGD_FUNC();
}

void VR_ModuleImp::Deleter(VR_ModuleImp *p)
{
    VR_LOGD_FUNC();
    delete p;
}

VR_ModuleImp *VR_ModuleImp::Instance()
{
    VR_LOGD_FUNC();
    if (NULL == s_pInstance.get()) {
        s_pInstance = boost::shared_ptr<VR_ModuleImp>(new(MEM_Voice) VR_ModuleImp(), VR_ModuleImp::Deleter);
    }
    return s_pInstance.get();
}

void VR_ModuleImp::PreInitialize()
{
    VR_LOGD_FUNC();
    NCSTARTPERF_START;
    NCSTARTPERF_OUT("VR", "Module PreInitialize begin");
#ifndef _ENABLE_VR_HIBERNATE_TEST
    VR_DialogEngineFactory::LoadAllLibraries();
#endif
    m_spSourceManager->onHibernate();
    // Dump memory usage
    //    std::string memStatus = MEM_NAMESPACE_NAME::MemoryTrackerIF::GetCurInstance()->ReportStatus();
    //    std::string::size_type prev = 0;
    //    const char sep = '\n';
    //    std::string::size_type pos = memStatus.find(sep);
    //    while (pos != std::string::npos) {
    //        std::string s = memStatus.substr(prev, pos - 1);
    //        NCSTARTPERF_OUT("VR", s.c_str());
    //        prev = pos + 1;
    //        pos = memStatus.find(sep, prev);
    //    }

    NCSTARTPERF_OUT("VR", "Module PreInitialize end");
    NCSTARTPERF_END;
}

void VR_ModuleImp::Initialize()
{
    VR_LOGD_FUNC();
#ifndef _ENABLE_VR_HIBERNATE_TEST
    NCSTARTPERF_START;
    NCSTARTPERF_OUT("VR", "Module Initialize begin");

#if ((!defined(COMP_M2W_L2_VOICETAG)) && (!defined(COMP_M2W_T2_VOICETAG)))
    m_Controller.reset(new(MEM_Voice) VR_Controller());
#endif

    m_Controller->Initialize();
    NCSTARTPERF_OUT("VR", "Module Initialize end");
    NCSTARTPERF_END;
#endif
}

void VR_ModuleImp::Start()
{
    VR_LOGD_FUNC();
#ifndef _ENABLE_VR_HIBERNATE_TEST
    NCSTARTPERF_START;
    NCSTARTPERF_OUT("VR", "Module Start begin");
    m_Controller->Start();
    NCSTARTPERF_OUT("VR", "Module Start end");
    NCSTARTPERF_END;
#endif
}

void VR_ModuleImp::Stop()
{
    VR_LOGD_FUNC();
#ifndef _ENABLE_VR_HIBERNATE_TEST
    m_Controller->Stop();
#endif
}

void VR_ModuleImp::Destory()
{
    VR_LOGD_FUNC();
#ifndef _ENABLE_VR_HIBERNATE_TEST
    NCSTARTPERF_START;
    NCSTARTPERF_OUT("VR", "Module Destory begin");
    m_Controller->Destory();
    NCSTARTPERF_OUT("VR", "Module Destory end");
    NCSTARTPERF_END;
#endif
}

#ifdef __cplusplus
extern "C"
{
    MODULE_API ModuleIFBase* GetInstance()
    {
        return VR_ModuleImp::Instance();
    }
}
#endif /* __cplusplus */
/* EOF */
