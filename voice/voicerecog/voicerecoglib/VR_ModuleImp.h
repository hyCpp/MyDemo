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
/**
 * @file VR_ModuleImp.h
 * @brief Declaration file of class VR_ModuleImp.
 *
 * This file includes the declaration of class VR_ModuleImp.
 *
 * @attention used for C++ only.
 */

#ifndef VR_MODULEIMP_H
#define VR_MODULEIMP_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef CXX_BL_MODULEBASE_IF_H
#   include "ModuleIFBase.h"
#endif

#include "VR_Def.h"
#include "VR_SourceManager.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ControllableIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ModuleImp);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_SourceManager);


/**
 * @brief The VR_ModuleImp class
 *
 * class declaration
 */
class VR_ModuleImp  : public ModuleIFBase
{
 public:
    static VR_ModuleImp*  Instance();

    // Inheritted from ModuleIFBase
    virtual void PreInitialize() override;
    virtual void Initialize() override;
    virtual void Start() override;
    virtual void Stop() override;
    virtual void Destory() override;

private:
    VR_ModuleImp();
    virtual ~VR_ModuleImp();
    static void Deleter(VR_ModuleImp* p);

private:
    VR_ModuleImp(const VR_ModuleImp& other);
    VR_ModuleImp& operator =(const VR_ModuleImp& other);

private:
    static spVR_ModuleImp       s_pInstance;
    spVR_ControllableIF         m_Controller;
    spVR_SourceManager          m_spSourceManager;
};

#ifdef __cplusplus
extern "C" { ModuleIFBase* GetInstance(); }
#endif

#ifdef HAVE_LINUX_NATIVEX86

#ifdef NCSTARTPERF_START
#undef NCSTARTPERF_START
#endif
#define NCSTARTPERF_START   do {;} while (0);

#ifdef NCSTARTPERF_KSTART
#undef NCSTARTPERF_KSTART
#endif
#define NCSTARTPERF_KSTART do {;} while (0);

#ifdef NCSTARTPERF_OUT
#undef NCSTARTPERF_OUT
#endif
#define NCSTARTPERF_OUT(module, log) do {;} while (0);

#ifdef NCSTARTWARN_OUT
#undef NCSTARTWARN_OUT
#endif
#define NCSTARTWARN_OUT(module, log) do {;} while (0);

#ifdef NCSTARTERR_OUT
#undef NCSTARTERR_OUT
#endif
#define NCSTARTERR_OUT(module, log) do {;} while (0);

#ifdef NCSTARTPERF_OUT_1N
#undef NCSTARTPERF_OUT_1N
#endif
#define NCSTARTPERF_OUT_1N(module, log, param) do {;} while (0);

#ifdef NCSTARTPERF_OUT_1S
#undef NCSTARTPERF_OUT_1S
#endif
#define NCSTARTPERF_OUT_1S(module, log, param) do {;} while (0);

#ifdef NCSTARTWARN_OUT_1N
#undef NCSTARTWARN_OUT_1N
#endif
#define NCSTARTWARN_OUT_1N(module, log, param) do {;} while (0);

#ifdef NCSTARTWARN_OUT_1S
#undef NCSTARTWARN_OUT_1S
#endif
#define NCSTARTWARN_OUT_1S(module, log, param) do {;} while (0);

#ifdef NCSTARTERR_OUT_1N
#undef NCSTARTERR_OUT_1N
#endif
#define NCSTARTERR_OUT_1N(module, log, param) do {;} while (0);

#ifdef NCSTARTERR_OUT_1S
#undef NCSTARTERR_OUT_1S
#endif
#define NCSTARTERR_OUT_1S(module, log, param) do {;} while (0);

#ifdef NCSTARTPERF_END
#undef NCSTARTPERF_END
#endif
#define NCSTARTPERF_END do {;} while (0);
#endif

#endif /* VR_MODULEIMP_H */
/* EOF */
