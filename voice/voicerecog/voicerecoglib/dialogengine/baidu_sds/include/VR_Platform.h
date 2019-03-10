#ifndef VR_PLATFORM_H
#define VR_PLATFORM_H

#include "VR_SDSRequestServer.h"
#include "tinyxml.h"

class VR_Platform
{
public:
    VR_Platform();
    virtual ~VR_Platform() {}

    virtual CL_BOOL Initialize(const TiXmlElement* pcXmlElem);
    virtual VOID Destroy();
    virtual VOID Start();
    virtual VOID Stop();

    virtual CL_BOOL IsNetworkConnected();
    virtual CL_ERROR GetLocation(LONG& lLon, LONG& lLat);

//    virtual CL_ERROR FindContact(VRCP_ListenerBase* pcListener, DWORD& dwReqID,
//                                 const BL_String &name, INT PhoneType = PHONE_NONE);

//    virtual CL_ERROR Play(VRCP_PlaySoundListener* pcListener, DWORD& dwReqID, DWORD dwToneID);
//    virtual CL_ERROR Play(VRCP_PlaySoundListener* pcListener, DWORD& dwReqID, const BL_String &ttsContent);
//    virtual CL_BOOL  StopPlay(DWORD dwReqID);

//    virtual CL_ERROR UploadKeywords(VRCP_UploadKeywordsListener* pcListener, DWORD& dwReqID, const BL_String& szKeywords);

//    virtual CL_ERROR TakePicture(VRCP_TakePictureListener* pcListener, DWORD& dwReqID, INT nWidth = 0, INT nHeight = 0);

//    virtual CL_ERROR TurnTrafficRecord(VRCP_TurnTrafficListener* pcListener, DWORD& dwReqID, CL_BOOL bFlag);

//    virtual CL_ERROR QueryPOI(VRCP_QueryPOIListener* pcListener, DWORD& dwReqID, const BL_String &poiName, const BL_String& cat);
//    virtual CL_ERROR SendReport(VRCP_SendReportListener* pcListener, DWORD& dwReqID, const BL_String &content, const BL_String& szImagePath = "");

//    virtual CL_BOOL IsWeiboBind();
//    virtual CL_BOOL IsHomeRegister(VRCP_ListenerBase* pcListener, DWORD& dwReqID);
//    virtual CL_BOOL IsCorpRegister(VRCP_ListenerBase *pcListener, DWORD& dwReqID);
//    virtual VOID SetUserID(const BL_String& szUserID);
//    virtual CL_BOOL GetUserID(BL_String& szUserID);
//    virtual VOID SetDeviceID(const BL_String& szDeviceID);
//    virtual CL_ERROR SendWeibo(VRCP_SendWeiboListener* pcListener, DWORD& dwReqID, const BL_String& szContent, const BL_String& szImagePath = "");
//    virtual CL_ERROR RequestDialog(VRCP_RequestDialogListener* pcListener, DWORD& dwReqID, const BL_String &szUrl);  // url: "besides host:port"
//    virtual CL_ERROR SendConversation(const std::vector<VRCP_ConversationInfo>& vConversationInfo);

//    virtual CL_BOOL CancelRequest(DWORD dwReqID);

//    //interface about UI--start
//    virtual CL_BOOL UIShowText(const BL_String& szText, VR_UITextType eType = UITEXT_TYPE_PROMPT);
//    virtual CL_BOOL UIShowImage(const BL_String& szImagePath);
//    virtual CL_BOOL UIShowPOIList(VR_POIInfo* pcPOIInfo, DWORD dwSize);
//    virtual CL_BOOL UISetDestination(VR_POIInfo* pcDestInfo, VR_POIInfo* pcWayPointInfo = NULL, INT nWayPointNum = 0);

//    virtual CL_BOOL UIShowContactList(VR_ContactInfo* pcInfo, DWORD dwSize);
//    virtual CL_BOOL UIShowNewsList(VR_NewsInfo* pcInfo, DWORD dwSize);
//    virtual CL_BOOL UIShowHelpList(VR_MenuInfo* pcInfo, DWORD dwSize);

//    virtual CL_BOOL UIChangeVRState(VR_VRSTATE eState);
//    virtual CL_BOOL UIHidePOIList();
//    virtual CL_BOOL UIGOHome();
//    virtual CL_BOOL UIGOCorp();
//    virtual CL_BOOL UIEnterVR();
//    virtual CL_BOOL UIExitVR();
//    virtual VOID SetUIListener(VRCP_UIListener* pcListener);
//    virtual VOID SetDialogMode(CL_BOOL bServer);
//    virtual VOID OnMic();
//    virtual VOID OnBack();
//    virtual VOID OnSelect(DWORD dwIndex);
//    virtual CL_ERROR StartVR(INT nVRType = 0);
//    virtual CL_ERROR StartVR(DWORD& dwReqID, BL_String szPrompt);

//    virtual CL_BOOL StartVR(const BL_String& szFile,VRE_MODE_TYPE eMode, const BL_String& szGrammerID, VRE_Engine::Listener* pListener, const NCHAR* lang=NULL, unsigned int maxHypotheses = 1, int vadTimeout=0);

//    virtual CL_BOOL CancelVR();

//    //interface about UI--end
//    virtual void OnVoiceInput(){;}

//    virtual VOID SetJniListener(VR_JNI_Listener *pListener);
//    virtual VOID OnVrResult(DWORD dwReqID, const BL_String& szContent, int nConfident, int nErrCode);

//    virtual VOID OnTakePictureResult(DWORD reqid, int result, const char* picPath);
//    virtual VOID OnTrafficOperResult(DWORD dwOperType, DWORD dwOperResult);
//    virtual VOID OnSendWeiboResult(DWORD dwRetCode);
//    virtual VOID OnLocationChange(LONG lon, LONG lat);

//    virtual VR_PlatformConfig& GetPlatformConfig() { return m_cPlatformConfig; }
//    //virtual VRE_Engine* GetVrEngine() { return m_pcEngine; }
//    virtual VOID OnSetVREngine(const int enginetype) ;
//    virtual VR_ContactAccessor* GetContactAccessor() {return m_pContactAccessor;}
//    virtual VR_MusicAccessor* GetMusicAccessor() {return m_pMusicAccessor;}
//    virtual VR_JNI_Listener* GetJniListener() { return m_pJniListener; }

//    // event message
//    virtual VOID OnVpPlayEnd(DWORD dwRet, DWORD dwReqId);

//    virtual VOID EraseRequest(DWORD reqId);
//    virtual void PushRequest(DWORD reqId, VR_RequestBase* req);
//    virtual VR_RequestBase* PopRequest(DWORD reqId);

//    virtual VOID SetMute(VR_VOLUME_TYPE eType, bool mute);
//    virtual VOID IncVolume(VR_VOLUME_TYPE eType, int volume = 1);
//    virtual VOID DecVolume(VR_VOLUME_TYPE eType, int volume = 1);

//    virtual VOID UploadContacts();
//    virtual VOID SendSMS(const BL_String &name, const BL_String &phoneNum, const BL_String &content);
//    virtual VOID UIHideContactList();

//    virtual VOID PlayMusic(const VR_MusicInfo &info);
//    virtual VOID PlayMusic(VRCP_ListenerBase *pcListener, DWORD &dwReqID, const VR_MusicInfo &info);
//    virtual VOID PauseMusic();
//    virtual VOID StopMusic();
//    virtual VOID PlayNextMusic();
//    virtual VOID PlayPrevMusic();

//    virtual VOID StartApp(VRCP_ListenerBase* pcListener, DWORD& dwReqID, const BL_String &name);
//    virtual VOID StartAppWithParam(VRCP_ListenerBase *pcListener, DWORD &dwReqID, const BL_String &name, const BL_String &param);

//    virtual VOID OnSearchAddressResponse(DWORD reqid, int result, int type, const VR_POIInfo& poi);
//    virtual VOID OnMediaScanUpdated(const bool bStatus, const char* dbPath, const long long deviceID);

//    virtual BL_String GetServerMD5() {return m_szServerMD5;}
//    virtual VOID SetServerMD5(const BL_String &md5) {m_szServerMD5 = md5;}
//    virtual VOID ChangeState(SESSION_STATE s, VRCP_ListenerBase *pListener = NULL);
//    virtual VOID LoadContactMD5(VRCP_ListenerBase *pListener = NULL);
//    virtual void OnTtsPlayEnd(DWORD dwRet, DWORD dwReqId);

protected:

//    VOID TurnAudioIn(CL_BOOL bIn);
//    CL_BOOL SendMessage(EV_EVENT_ID msgId, EV_MSG_PTR msg);
//    CL_BOOL SendIntent(EV_EVENT_ID intId, IntentData* intent);

//    typedef std::map<DWORD, VR_RequestBase*> MapRequest;
//    VRE_Engine*             m_pcEngine;

//    VRE_Engine*		    m_pcNativeEngine;
//    VRE_Engine*		    m_pcBtEngine;

//    VRCP_UIListener*        m_pcUIListener;
//    VR_JNI_Listener*        m_pJniListener;

//    VR_PlatformConfig       m_cPlatformConfig;
//    VR_ContactAccessor*     m_pContactAccessor;
//    VR_AppNaturalAccessor*  m_pAppNaturalAccessor;
//    VR_MusicAccessor*       m_pMusicAccessor;

//    //VR_POIInfo              m_pcPOIInfo[4]; //index 0 is destnation, 1-3 is waypoint, max way point number is 3
//    MapRequest              m_requests;
//    CL_SyncObj             m_syncMapRequests;
//    CL_BOOL                 m_bServer;
//    CL_BOOL                 m_bUpdateContact;
//    BL_String               m_szServerMD5;

//    //CL_SyncObj		    m_syncEngine;

//private:
//    enum TYPE {
//        TYPE_SYSTEM = 1,
//        TYPE_MUSIC
//    };
//    TYPE GetVolumeType(VR_VOLUME_TYPE eType);
};

#endif // VR_PLATFORM_H
