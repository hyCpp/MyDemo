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

#include <boost/scoped_ptr.hpp>

#include "VR_Controller.h"
#include "VR_ReceiverCb.h"
#include "VR_CommandFactory.h"
#include "VR_MessageProcess.h"
#include "VR_DM_CommandHandle.h"
#include "ncore/NCFile.h"
#include "VR_ContentProvider.h"

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>

VR_String cmd = "/usr/bin/natp.exe";

char * args[] = { "natp.exe", "-f", "/etc/natp.cfg", "-s", "natp_config", "-c",
                 "HostAgent:NODDS", "SpeechBLService", "MonitorService", "GDM",
                 "SASService", "FileService", "SHPSService", "DragonDriveCommonService",
                 "DragonDriveAsrService", "DragonDrivePrompterService", "DragonDriveBiometryService",
                 "-k", "VerboseLevel=2" };

const long NATP_SERVIEC_STACK_SIZE = 786432;

const long NATP_SERVIEC_STACK_SIZE_MAX = 8388608;

static void START_NATP_SERVICE()
{
    VR_LOGD_FUNC();
#ifndef COMP_IMX_IMX8X_VOICETAG
    pid_t pid;
    pid = fork();
    if (-1 == pid) {
        exit(-1);
    }

    if (0 == pid) { // child process
       VR_LOGD("start natp service");

       struct rlimit res = { NATP_SERVIEC_STACK_SIZE, NATP_SERVIEC_STACK_SIZE_MAX };

       setrlimit(RLIMIT_STACK, &res);
       int result = execv(cmd.c_str(), args);
       if (-1 == result) {
            VR_LOGD("execv error: %s", strerror(errno));
            exit(-1);
       }

       exit(0);
    }
#else
   VR_LOGD("Do not support NATP");
#endif // COMP_IMX_IMX8X_VOICETAG
}

static void sig_handle(int signum)
{
#ifndef COMP_IMX_IMX8X_VOICETAG
    int saveerr = errno;

    if (waitpid(-1, NULL, 0) < 0) {
        VR_LOGD("waitpid error : %s", strerror(errno));
        return;
    }

    // 1: when natp service stop, restart natp service
    START_NATP_SERVICE();
    VR_LOGD("wait child process end");
    errno = saveerr;
#else
       VR_LOGD("Do not support NATP");
#endif // COMP_IMX_IMX8X_VOICETAG
}

VR_Controller::VR_Controller()
    : m_spMessageProcess()
    , m_spReceiverCb()
    , m_spDMCommandHandler()
{
    VR_LOGD_FUNC();
#ifndef COMP_IMX_IMX8X_VOICETAG
    VR_ContentProvider vr_cp;
    VR_String strEngineType = vr_cp.GetContentProviderValue(VR_IAUTO_VR_ENGINETYPE);
    VR_LOGI("strEngineType = [%s]", strEngineType.c_str());
    if ("Nuance" == strEngineType) {
        // 1: register child process exit signal
        bool res = (signal(SIGCHLD, sig_handle) != SIG_ERR);
        if (!res) {
            VR_LOGD("signal error : %s\n", strerror(errno));
            return;
        }

        // 2: start natp service
        START_NATP_SERVICE();
    }
#else
    VR_LOGD("Do not support NATP");
#endif // COMP_IMX_IMX8X_VOICETAG
}

VR_Controller::~VR_Controller()
{
    VR_LOGD_FUNC();
}



void VR_Controller::Initialize()
{
    VR_LOGD_FUNC();
    // commnad handler
    m_spDMCommandHandler = spVR_DM_CommandHandle(VR_new VR_DM_CommandHandle);

    // command factory
    spVR_CommandFactory spVrCommandFactory = VR_CommandFactory::getCommandFactory();

    // avc lan listener
    // m_spAvcListener = VR_AvcListenerIF::getAvcListener(m_spDMCommandHandler, spVrCommandFactory);
    // avc lan sender
    // m_spAvcSender = VR_AvcLanSenderIF::getAvcLanSender(m_spAvcUnit, m_spAvcListener);

    // messageprocess
    m_spMessageProcess.reset(VR_new VR_MessageProcess(m_spDMCommandHandler));
    m_spMessageProcess->Initialize();

    // set message processer to listener
    VR_LOGI("m_spMessageProcess = [%p]", m_spMessageProcess.get());

    // receicer
    m_spReceiverCb.reset(VR_new VR_ReceiverCb(m_spMessageProcess, spVrCommandFactory, m_spDMCommandHandler));
}

VOID
VR_Controller::Start()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->Start();
    m_spReceiverCb->StartReceive();   // now . we think StartReceive , VR start over
    m_spDMCommandHandler->StartDMThread();

    m_spMessageProcess->CheckStatus();
}

VOID
VR_Controller::Stop()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->Stop();
    m_spDMCommandHandler->StopDMThread();
    m_spReceiverCb->StopReceive();
}

void VR_Controller::Destory()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->Destory();
    m_spDMCommandHandler.reset();
    m_spMessageProcess.reset();
    m_spReceiverCb.reset();
}

/* EOF */
