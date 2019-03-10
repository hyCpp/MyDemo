#ifndef VR_SDSSTATEMACHINE_H
#define VR_SDSSTATEMACHINE_H

#include <list>
#include <memory>
#include "VR_SDSEvent.h"
#include "VR_ControllerBase.h"
#include "VR_Platform.h"
#include "BL_Thread.h"
#include "CL_SyncObj.h"

class VR_SDSStateManager;
class VR_SDSAbstractState;

class VR_SDSStateMachine : public BL_Thread
{
public:
    VR_SDSStateMachine(std::shared_ptr<VR_ControllerBase> pController);

    virtual ~VR_SDSStateMachine();

    virtual bool Init();

    virtual bool IsDone();

    bool Start();

    void Stop();

    void PostRequest(VR_SDSEvent* pcEvent);

    void SendMessage(const std::string& message, int actionSeqId = -1);

    virtual VOID ChangeStateTo(VR_SDSAbstractState* pDesState);

    VR_SDSAbstractState* GetState(VR_SDSSERVER_STATE_TYPE eType);
    VR_SDSAbstractState* GetRootState();

    std::string  GetUserData() const { return m_strUserData; }
    void         SetUserData(std::string sUserData) { m_strUserData = sUserData; }

protected:
    virtual void InnerStop() {}

    void Run();

    VR_SDSEvent* GetRequest();

    void ClearRequest();

    VR_SDSAbstractState* m_pcState;
    VR_SDSAbstractState* m_pLastState;

private:
    VR_SDSStateMachine(const VR_SDSStateMachine &statemachine);
    VR_SDSStateMachine& operator=(const VR_SDSStateMachine& statemachine);

    CL_SyncObj m_cSyncObj;
    std::list<VR_SDSEvent*> m_cEventList;
    std::shared_ptr<VR_SDSStateManager> m_pStateManager;
    std::shared_ptr<VR_Platform>        m_pPlatform;
    std::shared_ptr<VR_ControllerBase>  m_pController;
    std::string  m_strUserData;
};

#endif // VR_SDSSTATEMACHINE_H
