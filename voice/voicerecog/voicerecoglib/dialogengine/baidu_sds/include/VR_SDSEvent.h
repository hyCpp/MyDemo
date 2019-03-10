#ifndef VR_SDSEVENT_H
#define VR_SDSEVENT_H

#include "BL_Base.h"
#include "VR_SDSServerDEF.h"

class VR_SDSEvent
{
public:
    VR_SDSEvent(DWORD dwType = VR_SDS_EVENT_TYPE_CUSTOM, DWORD dwID = VR_SDS_EVENT_ID_UNKNOWN, DWORD dwReqID = 0xFFFFFFFF, DWORD dwParam = 0);
    virtual ~VR_SDSEvent();

    VOID SetID(DWORD dwID);
    DWORD GetID() const;

    VOID SetReqID(DWORD dwReqID);
    DWORD GetReqID() const;

    VOID SetParam(DWORD dwParam);
    DWORD GetParam() const;

    VOID SetText(const std::string& rcText);
    const std::string& GetText() const;

    VOID  SetType(DWORD dwType);
    DWORD GetType() const;

private:
    DWORD       m_dwType;
    DWORD		m_dwID;
    DWORD		m_dwReqID;
    DWORD		m_dwParam;
    std::string	m_cText;
};

#endif // VR_SDSEVENT_H
