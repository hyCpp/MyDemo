#ifndef VR_SDSSTATEMANAGER_H
#define VR_SDSSTATEMANAGER_H

#include <memory>
#include <map>
#include "VR_SDSServerDEF.h"
#include "VR_SDSAbstractState.h"
#include "VR_SDSStateMachine.h"

class VR_SDSStateManager
{
public:
    VR_SDSStateManager(std::shared_ptr<VR_SDSStateMachine> pStateMachine,
                       std::shared_ptr<VR_Platform> pPlatform);
    ~VR_SDSStateManager();

    bool Initialize();

    VR_SDSAbstractState* GetRootState() {
        return GetState(VR_SDSSERVER_STATE_TYPE_IDLE);
    }

    VR_SDSAbstractState* GetState(VR_SDSSERVER_STATE_TYPE eType) {
        return m_stateMap[eType];
    }

    std::shared_ptr<VR_SDSStateMachine> GetStateMachine();

protected:
    bool BuildState();

private:
    VR_SDSStateManager(const VR_SDSStateManager&);
    VR_SDSStateManager& operator=(const VR_SDSStateManager&);

    std::map<VR_SDSSERVER_STATE_TYPE, VR_SDSAbstractState* > m_stateMap;
    std::shared_ptr<VR_SDSStateMachine> m_pcStateMachine;
    std::shared_ptr<VR_Platform>        m_pPlatform;

};

#endif // VR_SDSSTATEMANAGER_H
