#ifndef VR_SDSSTATE_BASE_H
#define VR_SDSSTATE_BASE_H

#include "VR_SDSParserBase.h"
#include "VR_SDSAbstractState.h"

class VR_SDSParserDLG;
class VR_SDSOperationParser;


/**
 * @brief This class is the base class of all
 * concrete classes, includ idle, prestart,
 * start, dialog etc.
 */
class VR_SDSStateBase : public VR_SDSAbstractState
{
public:
    VR_SDSStateBase() {}

    VR_SDSStateBase(const std::string name) : VR_SDSAbstractState(name) {}

    virtual ~VR_SDSStateBase() {}

    virtual void Exit();

    virtual bool OnEvent(VR_SDSEvent& event);

    static void SetBeepID(const std::string& beepId);

protected:
    typedef bool (VR_SDSStateBase::*P_ACTION)(const VR_SDSOperationParser*);

    static VR_SDSParserDLG* CreateParser();
    static void DeleteParser();
    static VR_SDSParserDLG* GetParser();

    // return true: Sync
    // return false; Async
    virtual bool DoActionPrompt(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionHints(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionVR(const VR_SDSOperationParser* pcParser);

    virtual bool DoActionDisplayPOIListOn(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionDisplayContactListOn(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionDisplayNewsListOn(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionDisplayHelpListOn(const VR_SDSOperationParser* pcParser);

    virtual bool DoActionDisplayWeatherInfoListOn(const VR_SDSOperationParser* pcParser);

    virtual bool DoActionDisplayComListOn(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionDisplayListOff(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionPicture(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionDestinationNormal(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionCommitReport(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionCommitWeibo(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionCheckWeibo(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionStop(const VR_SDSOperationParser* pcParser);

    virtual bool DoActionError(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionError(VR_SDSSERVER_ERROR eError);
    virtual bool DoActionRequestDialog(const VR_SDSRequestServerBase* pcReq);
    virtual bool DoActionBeep();
    virtual bool DoActionTTS(const std::string& rcTTS);
    virtual bool DoActionDisplayVR(const std::string& rcVRText);
    virtual bool DoActionDisplayPrompt(const std::string& rcPrompt);
    virtual bool DoActionUIDisplayPrompt(const std::string& uiPrompt);
    virtual bool DoActionDisplayHints(const std::string& rcHints);
    virtual bool DoActionDisplayPicture(const std::string& rcPic);
    virtual bool DoActionStart();

    virtual bool DoActionMusicStart(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMusicStop(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMusicPause(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMusicNext(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMusicPrev(const VR_SDSOperationParser* pcParser);

    virtual bool DoActionVolume(const VR_SDSOperationParser* pcParser);

    virtual bool DoActionMapZoomIn(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMapZoomOut(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMapZoom(const VR_SDSOperationParser* pcParser);

    virtual bool DoActionMapMoveUp(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMapMoveDown(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMapMoveLeft(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMapMoveRight(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMapMoveCurrentPos(const VR_SDSOperationParser* pcParser);

    virtual bool DoActionMapDirNorthUp(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMapDirHeadingUp(const VR_SDSOperationParser* pcParser);

    virtual bool DoActionMapViewIR(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMapViewSky(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMapViewNormal(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionMapViewChange(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionCall(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionShowPos(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionShowRoute(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionSendSMS(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionLogomark(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionAppControl(const VR_SDSOperationParser* pcParser);
    virtual bool DoActionRadioControl(const VR_SDSOperationParser* pcParser);

    virtual bool CancelRequest();

    virtual VOID SetPictureContent(const std::string& rcResult) { m_cPictureContent = rcResult;}
    virtual const std::string& GetPictureContent() const { return m_cPictureContent; }

protected:
    std::string m_cPictureContent;

    static VR_SDSParserDLG* s_pcDLGParser;

    /// temp code of play the content as the related platform if are not commited
    bool playContent(const std::string& rcTTS);

private:
    static DWORD s_dwBeepID;

};

#endif // VR_SDSSTATE_BASE_H
