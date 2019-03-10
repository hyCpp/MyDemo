#ifndef VR_SDSABSTRACTSTATE_H
#define VR_SDSABSTRACTSTATE_H

#include <vector>
#include <memory>
#include "VR_SDSServerDEF.h"
#include "VR_SDSStateMachine.h"
#include "VR_Platform.h"
#include "VR_SDSEvent.h"

#include "BL_Base.h"
#include "BL_String.h"

class VR_SDSAbstractState
{
public:
    VR_SDSAbstractState();
    VR_SDSAbstractState(const std::string& rcName);
    virtual ~VR_SDSAbstractState() {}

    VOID SetVRState(VR_SDSSERVER_STATE eState);
    VR_SDSSERVER_STATE GetVRState();

    VOID SetPlatformIF(std::shared_ptr<VR_Platform> spcPlatformIF);
    std::shared_ptr<VR_Platform> GetPlatformIF();

    std::shared_ptr<VR_SDSStateMachine> GetStateMachine()
    {
        return m_pStateMachine;
    }

    VOID SetStateMachine(std::shared_ptr<VR_SDSStateMachine> spcStateMachine)
    {
        m_pStateMachine = spcStateMachine;
    }

    VOID SessionAdd(const BL_String& rcContent, CL_BOOL bUser = CL_FALSE,
                    BL_String rcAudioFileName = BL_String(""));
    VOID SessionCommit();

    VOID SetReqID(DWORD dwReqID)
    {
        m_dwReqID = dwReqID;
    }
    DWORD GetReqID() const
    {
        return m_dwReqID;
    }

    virtual const std::string& GetStateName() const
    {
        return m_cStateName;
    }
    virtual VOID Clear() {}

    virtual bool OnEvent(VR_SDSEvent &rEvent)   { return CL_FALSE; }

    virtual void Enter()                   { }
    virtual void Exit()                    { }
    virtual void Abort()                   { Exit(); }

protected:
    std::string TranslateVrStateToUI(VR_SDSSERVER_STATE eStateSDS);
    std::string m_cStateName;

private:
    typedef std::vector<VRCP_ConversationInfo> SessionVector;

private:
    std::shared_ptr<VR_Platform> m_pPlatformIF;
    std::shared_ptr<VR_SDSStateMachine> m_pStateMachine;

    DWORD m_dwReqID;

    VR_SDSSERVER_STATE m_eVRState;
    SessionVector m_cSessionVector;
    BL_String m_szCurSessionAudioFile;

};

#endif // VR_SDSABSTRACTSTATE_H
