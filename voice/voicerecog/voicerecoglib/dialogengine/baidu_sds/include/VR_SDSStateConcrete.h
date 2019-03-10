#ifndef VR_SDSSTATECONCRETE_H
#define VR_SDSSTATECONCRETE_H

#include <map>
#include <string>
#include "VR_SDSStateBase.h"
#include "VR_SDSParserConcrete.h"

class VR_SDSStateServerIdle : public VR_SDSStateBase
{
public:
    VR_SDSStateServerIdle() {}

    VR_SDSStateServerIdle(const std::string& name) : VR_SDSStateBase(name) {}
    virtual void Enter();

    virtual bool OnEvent(VR_SDSEvent& rcEvent);
};

class VR_SDSStateServerPreStart : public VR_SDSStateBase
{
public:
    VR_SDSStateServerPreStart() {}

    VR_SDSStateServerPreStart(const std::string& name) : VR_SDSStateBase(name) {}

    virtual void Enter();
};

class VR_SDSStateServerStart : public VR_SDSStateBase
{
public:
    VR_SDSStateServerStart();

    VR_SDSStateServerStart(const std::string& name) : VR_SDSStateBase(name) {}

    virtual void Enter();

    virtual bool OnEvent(VR_SDSEvent& rcEvent);
};

class VR_SDSStateServerDialog : public VR_SDSStateBase
{
public:
    VR_SDSStateServerDialog(const std::string& name = "");

    virtual void Enter();

    virtual bool OnEvent(VR_SDSEvent& rcEvent);

private:
    bool DoOperation();

    bool OnDialog(const VR_SDSEvent& rcEvent);
    bool OnVR(const VR_SDSEvent& rcEvent);
    bool OnSelect(const VR_SDSEvent& rcEvent);
    bool OnPicture(const VR_SDSEvent& rcEvent);
    bool OnReport(const VR_SDSEvent& rcEvent);
    bool OnWeibo(const VR_SDSEvent& rcEvent);
    bool OnPlayMusic(const VR_SDSEvent& rcEvent);
    bool OnAppControl(const VR_SDSEvent& rcEvent);

private:
    std::map<int, P_ACTION> m_pfActionMap;

    VR_SDSParserDLG::OperationVector m_cOperationVector;

    bool m_bSelect;
};

class VR_SDSStateServerError : public VR_SDSStateBase
{
public:
    VR_SDSStateServerError();

    VR_SDSStateServerError(const std::string& name) : VR_SDSStateBase(name) {}

    virtual void Enter();

    virtual bool OnEvent(VR_SDSEvent& rcEvent);

    void SetError(VR_SDSSERVER_ERROR eError) { m_eError = eError; }
    void BuildErrorMap(VR_SDSSERVER_ERROR eError, const BL_String& rcError);

private:
    VR_SDSSERVER_ERROR GetError() const { return m_eError; }


private:
    VR_SDSSERVER_ERROR m_eError;

    std::map<VR_SDSSERVER_ERROR, std::string> m_pfErrorMap;
};


#endif // VR_SDSSTATECONCRETE_H
