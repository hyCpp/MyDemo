/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include "VR_SDSRequestServer.h"
#include "VR_SDSEvent.h"
#include "VR_SDSStateBase.h"
#include "VR_SDSParserConcrete.h"
#include "VR_RequestServerDialog.h"
#include "VR_SDSStateConcrete.h"
#include "VR_VoiceBoxXmlBuilder.h"
#include "VR_Log.h"

#include "rapidjson/document.h"
#include <boost/format.hpp>

using namespace std;

VR_SDSParserDLG* VR_SDSStateBase::CreateParser()
{
    if (NULL != s_pcDLGParser) {
        delete s_pcDLGParser;
    }

    s_pcDLGParser = new VR_SDSParserDLG;

    return s_pcDLGParser;
}

void VR_SDSStateBase::DeleteParser()
{
    if (NULL != s_pcDLGParser) {
        delete s_pcDLGParser;
        s_pcDLGParser = NULL;
    }
}

VR_SDSParserDLG* VR_SDSStateBase::GetParser()
{
    return s_pcDLGParser;
}

bool VR_SDSStateBase::CancelRequest()
{
    if (VR_INVALID_REQUET_ID != GetReqID()) {
        // GetPlatformIF()->CancelRequest(GetReqID());

        VR_LOGD("|VR_SDSStateBase| |CancelRequest| [ReqID:%lu]\n", GetReqID());

//		SetReqID(VR_INVALID_REQUET_ID);
    }

    return CL_TRUE;
}

void VR_SDSStateBase::SetBeepID(const string& rcBeepID)
{
    s_dwBeepID = atoi(rcBeepID.c_str());
}

void VR_SDSStateBase::Exit()
{
    CancelRequest();
}

bool VR_SDSStateBase::OnEvent(VR_SDSEvent& rcEvent)
{
    DWORD dwID = rcEvent.GetID();
    DWORD dwType = rcEvent.GetType();

    bool bRet = CL_FALSE;

    if (VR_SDS_EVENT_TYPE_TASK == dwType) {

        if (VR_SDS_EVENT_ID_CONTACTSYNC == dwID) {

            VR_LOGD("Get server's contactMD5! begin\n");

            // GetPlatformIF()->LoadContactMD5(VR_SDSListenerFactory::GetContactMD5Listener());
        }

        if (VR_SDS_EVENT_ID_CONTACTMD5  == dwID) {
            VR_LOGD("Sync Contact! begin\n");

            // GetPlatformIF()->UploadContacts();
        }
    }

    switch (dwID) {
        case VR_SDS_EVENT_ID_BACK:
        if (this != GetStateMachine()->GetRootState()) {
                VR_SDSAbstractState* pcState = GetStateMachine()->GetState(VR_SDSSERVER_STATE_TYPE_IDLE);
                GetStateMachine()->ChangeStateTo(pcState);
            }

            bRet = CL_TRUE;
            break;

        case VR_SDS_EVENT_ID_MIC:
            if (VR_SDS_STATE_TTS == GetVRState()) {
                CancelRequest();
            }
            break;

        default:
            break;
    }

    return bRet;
}

bool VR_SDSStateBase::DoActionPrompt(const VR_SDSOperationParser* pcParser)
{
   VR_LOGD("|VR_SDSStateBase| |DoActionPrompt|\n");
   const VR_SDSParserOperationPrompt* pcParserTmp = static_cast<const VR_SDSParserOperationPrompt*>(pcParser);

   DoActionDisplayPrompt(pcParserTmp->GetDisplay().GetString());

   DoActionUIDisplayPrompt(pcParserTmp->GetUiDisplay().GetString());

   DoActionTTS(pcParserTmp->GetTTS().GetString());

   return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionHints(const VR_SDSOperationParser* pcParser)
{
//    VR_LOGD("|VR_SDSStateBase| |DoActionHints|\n");

//    const VR_SDSParserOperationHints* pcParserTmp = static_cast<const VR_SDSParserOperationHints*>(pcParser);

//    if ( 0 == pcParserTmp->GetHintsContent().GetLength() ) {
//        VR_LOGD("|VR_SDSStateBase| |DoActionHints| is empty hints\n");
//    }

//    DoActionDisplayHints(pcParserTmp->GetHintsContent());
    
    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionVR(const VR_SDSOperationParser* pcParser)
{
//    DoActionBeep();

    SetVRState(VR_SDS_STATE_SPEACH);
    // tell the engine retrieve the data again

//    DWORD dwReqID = VR_INVALID_REQUET_ID;
//    BL_String param = pcParser->GetOperationParam();
//    int nVadTimeout = 0;
//    if (param.GetLength()>0) {
//        nVadTimeout = atoi(param.GetString());
//    }

//    //TODO
//    GetPlatformIF()->StartSpeechRecog(VR_SDSListenerFactory::GetVRListener(), dwReqID, "", VRE_MODE_KEYWORD, nVadTimeout);
//    SetReqID(dwReqID);

//    VR_LOGD("|VR_SDSStateBase| |DoActionVR| [ReqID:%lu]\n", dwReqID);

    // return CL_FALSE;
    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionDisplayPOIListOn(const VR_SDSOperationParser* pcParser)
{
    VR_LOGD_FUNC();
    const VR_SDSParserOperationDisplayPOIListOn* pcParserTmp = static_cast<const VR_SDSParserOperationDisplayPOIListOn*>(pcParser);

//    GetPlatformIF()->UIShowPOIList(pcParserTmp->GetPOIInfo(), pcParserTmp->GetPOIInfoNum());

    VR_LOGD("|VR_SDSStateBase| |DoActionDisplayPOIListOn| [POIInfo:%p POINum:%lu]\n", pcParserTmp->GetPOIInfo(), pcParserTmp->GetPOIInfoNum());


    VR_POIInfo* poiInfo = pcParserTmp->GetPOIInfo();
    int nPioInfoNum = pcParserTmp->GetPOIInfoNum();

    if (0 >= nPioInfoNum) {
        return CL_FALSE;
    }

    std::string strPoiListXml = "<display agent=\"Common\" content=\"ScreenDisplay\">";
    
    strPoiListXml.append("<agent>navi</agent>"); 
    strPoiListXml.append("<content>FindPOIList</content>");
    strPoiListXml.append("<guidePoint>");

    std::string strInfoItem = "<item>"
                                 "<poiname>%1%</poiname>"
                                 "<addr>%2%</addr>"
                                 "<lon>%3%</lon>"
                                 "<lat>%4%</lat>"
                                 "<distance>%5%</distance>"          
                                 "<url></url>"
                              "</item>";

    std::string strItems = "";

    for(int i = 0;i < nPioInfoNum; ++i) {
        std::string strinfo = poiInfo[i].GetJsonStr().GetString();
        VR_LOGD("DoActionDisplayPOIListOn info:%s", strinfo.c_str());

        std::string strPOIName = "";
        std::string strAddr = "";
        std::string strLon = "";
        std::string strLat = "";
        std::string strDistance = "";

        rapidjson::Document jsonInfoDoc;
        jsonInfoDoc.Parse<0>(strinfo.c_str());

        if (jsonInfoDoc.HasParseError()) {
            VR_LOGE("json format was error");
            continue;
        }

        if (jsonInfoDoc.HasMember("VR_POIInfo")) {
            const rapidjson::Value& objVrPoiInfo = jsonInfoDoc["VR_POIInfo"];
            if (objVrPoiInfo.IsObject()) {
                if (objVrPoiInfo.HasMember("cName")) {
                    strPOIName = objVrPoiInfo["cName"].GetString();
                }

                if (objVrPoiInfo.HasMember("cAddr")) {
                    strAddr = objVrPoiInfo["cAddr"].GetString();
                }

                if (objVrPoiInfo.HasMember("cLon")) {
                    strLon = objVrPoiInfo["cLon"].GetString();
                }

                if (objVrPoiInfo.HasMember("cLat")) {
                    strLat = objVrPoiInfo["cLat"].GetString();
                }

                if (objVrPoiInfo.HasMember("cDistance")) {
                    strDistance = objVrPoiInfo["cDistance"].GetString();
                }
                
            }           
        }

        strItems += (boost::format(strInfoItem) 
                    % strPOIName
                    % strAddr
                    % strLon
                    % strLat
                    % strDistance).str();
    }

    
    strPoiListXml.append(strItems);

    strPoiListXml.append("</guidePoint></display>");

    VR_LOGD("strPoiListXml:%s", strPoiListXml.c_str());

    GetStateMachine()->SendMessage(strPoiListXml);

    return CL_TRUE;

}

bool
VR_SDSStateBase::DoActionDisplayContactListOn(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationDisplayContactListOn* pcParserTmp = static_cast<const VR_SDSParserOperationDisplayContactListOn*>(pcParser);
//    GetPlatformIF()->UIShowContactList(pcParserTmp->GetContactInfo(), pcParserTmp->GetContactInfoNum());
//    VR_LOGD("|VR_SDSStateBase| |DoActionDisplayContactListOn| [POIInfo:%p POINum:%lu]\n", pcParserTmp->GetContactInfo(), pcParserTmp->GetContactInfoNum());
    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionDisplayNewsListOn(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationDisplayNewsListOn* pcParserTmp = static_cast<const VR_SDSParserOperationDisplayNewsListOn*>(pcParser);
//    GetPlatformIF()->UIShowNewsList(pcParserTmp->GetNewsInfo(), pcParserTmp->GetNewsInfoNum());
//    VR_LOGD("|VR_SDSStateBase| |DoActionDisplayNewsListOn| [POIInfo:%p POINum:%lu]\n", pcParserTmp->GetNewsInfo(), pcParserTmp->GetNewsInfoNum());
    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionDisplayHelpListOn(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationDisplayHelpListOn* pcParserTmp = static_cast<const VR_SDSParserOperationDisplayHelpListOn*>(pcParser);
//    GetPlatformIF()->UIShowHelpList(pcParserTmp->GetMenuInfo(), pcParserTmp->GetMenuInfoNum());
//    VR_LOGD("|VR_SDSStateBase| |DoActionDisplayHelpListOn| [POIInfo:%p POINum:%lu]\n", pcParserTmp->GetMenuInfo(), pcParserTmp->GetMenuInfoNum());
    return CL_TRUE;
}


bool
VR_SDSStateBase::DoActionDisplayComListOn(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationDisplayComListOn* pcParserTmp = static_cast<const VR_SDSParserOperationDisplayComListOn*>(pcParser);

//    GetPlatformIF()->UIShowContactList(pcParserTmp->GetComInfo(), pcParserTmp->GetComInfoNum());

//    VR_LOGD("|VR_SDSStateBase| |DoActionDisplayComListOn| [POIInfo:%p POINum:%lu]\n", pcParserTmp->GetComInfo(), pcParserTmp->GetComInfoNum());

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionDisplayListOff(const VR_SDSOperationParser* pcParser)
{
//    GetPlatformIF()->UIHidePOIList();

//    VR_LOGD("|VR_SDSStateBase| |DoActionDisplayListOff|\n");

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionPicture(const VR_SDSOperationParser* pcParser)
{
//    SetVRState(VR_SDS_STATE_BUSY);

//    DWORD dwReqID = VR_INVALID_REQUET_ID;

//    GetPlatformIF()->TakePicture(VR_SDSListenerFactory::GetPICListener(), dwReqID);
//    SetReqID(dwReqID);

//    VR_LOGD("|VR_SDSStateBase| |DoActionPicture| [ReqID:%lu]\n", dwReqID);

    return CL_FALSE;
}

bool
VR_SDSStateBase::DoActionDestinationNormal(const VR_SDSOperationParser* pcParser)
{
    VR_LOGD_FUNC();

    const VR_SDSParserOperationDestinationNormal* pcParserTmp = static_cast<const VR_SDSParserOperationDestinationNormal*>(pcParser);

//    GetPlatformIF()->UISetDestination(pcParserTmp->GetDestinationInfo(), pcParserTmp->GetWayPointInfo(), pcParserTmp->GetWayPointInfoNum());

    VR_LOGD("|VR_SDSStateBase| |DoActionDestinationNormal| [DestInfo:%p DestNum:%lu WayPointInfo:%p WayPointNum:%lu]\n",
                pcParserTmp->GetDestinationInfo(), pcParserTmp->GetDestinationInfoNum(),
                pcParserTmp->GetWayPointInfo(), pcParserTmp->GetWayPointInfoNum());

    VR_POIInfo* pioInfo = pcParserTmp->GetDestinationInfo();
    std::string strDestinationInfo = pioInfo->GetJsonStr().GetString();
    VR_LOGD("destiantion info:%s", strDestinationInfo.c_str());

    VR_POIInfo* poiViaPointInfo = NULL;
    std::string strViaInfo = "";

    bool bIsViaPointCalc = false;
    int nViaPointNum = 0;
    nViaPointNum = pcParserTmp->GetWayPointInfoNum();
    if (0 < nViaPointNum) {
        bIsViaPointCalc = true;
        poiViaPointInfo = pcParserTmp->GetWayPointInfo();
        strViaInfo = poiViaPointInfo->GetJsonStr().GetString();
        VR_LOGD("Way point info:%s", strViaInfo.c_str());

    }

    // fValue = fValue * 256 * 3600;

    static int nDesId = 1;
    std::string strDesAddress = "";

    boost::format strFloat("%.4f");

    float fLon = 0.0;
    float fLat = 0.0;
    std::string strAddr = "";
    std::string strLon = "";
    std::string strLat = "";
    std::string strCalcLon = "";
    std::string strCalcLat = "";

    float fViaPointLon = 0.0;
    float fViaPointLat = 0.0;
    std::string strViaPointAddr = "";
    std::string strViaPointLon = "";
    std::string strViaPointLat = "";
    std::string strViaPointCalcLon = "";
    std::string strViaPointCalcLat = "";

    std::string strCalcDestActionXml = "";

    rapidjson::Document jsonInfoDoc;
    jsonInfoDoc.Parse<0>(strDestinationInfo.c_str());

    if (jsonInfoDoc.HasParseError()) {
        VR_LOGE("json format was error");
        return CL_FALSE;
    }

    if (jsonInfoDoc.HasMember("VR_POIInfo")) {
        const rapidjson::Value& objVrPoiInfo = jsonInfoDoc["VR_POIInfo"];
        if (objVrPoiInfo.IsObject()) {

            if (objVrPoiInfo.HasMember("cAddr")) {
                strAddr = objVrPoiInfo["cAddr"].GetString();
            }

            if (objVrPoiInfo.HasMember("cLon")) {
                strLon = objVrPoiInfo["cLon"].GetString();
            }

            if (objVrPoiInfo.HasMember("cLat")) {
                strLat = objVrPoiInfo["cLat"].GetString();
            }
                
        }           
    }

    if ("" != strLon) {
        fLon = atof(strLon.c_str()) / 256 / 3600;
        strCalcLon = (strFloat % fLon).str();
    }

    if ("" != strLat) {
        fLat = atof(strLat.c_str()) / 256 / 3600;
        strCalcLat = (strFloat % fLat).str();
    }

    if (bIsViaPointCalc) {

        rapidjson::Document jsonViaPointInfoDoc;
        jsonViaPointInfoDoc.Parse<0>(strViaInfo.c_str());

        if (jsonViaPointInfoDoc.HasParseError()) {
            VR_LOGE("jsonViaPointInfoDoc json format was error");
            return CL_FALSE;
        }

        if (jsonViaPointInfoDoc.HasMember("VR_POIInfo")) {
            const rapidjson::Value& objVrPoiInfo = jsonViaPointInfoDoc["VR_POIInfo"];
            if (objVrPoiInfo.IsObject()) {

                if (objVrPoiInfo.HasMember("cAddr")) {
                    strViaPointAddr = objVrPoiInfo["cAddr"].GetString();
                }

                if (objVrPoiInfo.HasMember("cLon")) {
                    strViaPointLon = objVrPoiInfo["cLon"].GetString();
                }

                if (objVrPoiInfo.HasMember("cLat")) {
                    strViaPointLat = objVrPoiInfo["cLat"].GetString();
                }
                
            }           
        }

        if ("" != strViaPointLon) {
            fViaPointLon = atof(strViaPointLon.c_str()) / 256 / 3600;
            strViaPointCalcLon = (strFloat % fViaPointLon).str();
        }

        if ("" != strViaPointLat) {
            fViaPointLat = atof(strViaPointLat.c_str()) / 256 / 3600;
            strViaPointCalcLat = (strFloat % fViaPointLat).str();
        }

        std::string strClacViaPointDestXml = "<action>"   
                                             "<id>%1%</id>"
                                             "<type>Navi_ToDestinationWithViaPoint</type>"
                                             "<params>"  
                                                 "<navi_dst_addr>%2%</navi_dst_addr>"
                                                 "<addr_longitude>%3%</addr_longitude>" 
                                                 "<addr_latitude>%4%</addr_latitude>"
                                                 "<navi_via_point_name>%5%</navi_via_point_name>"
                                                 "<navi_via_point_longitude>%6%</navi_via_point_longitude>"
                                                 "<navi_via_point_latitude>%7%</navi_via_point_latitude>"
                                                 "<position>1</position>"
                                             "</params>" 
                                             "</action>";
        
        strCalcDestActionXml = (boost::format(strClacViaPointDestXml) 
                                       % nDesId++ 
                                       % strAddr
                                       % strLon
                                       % strLat
                                       % strViaPointAddr
                                       % strViaPointLon
                                       % strViaPointLat).str();


    }
    else {
        std::string strCalcDestXml = "<action>"
                                         "<id>%1%</id>"
                                         "<type>Navi_ToDestination</type>"
                                         "<params>"
                                            "<navi_dst_addr>%2%</navi_dst_addr>"
                                            "<addr_longitude>%3%</addr_longitude>"
                                            "<addr_latitude>%4%</addr_latitude>"
                                         "</params>"
                                     "</action>";

        strCalcDestActionXml = (boost::format(strCalcDestXml) 
                                       % nDesId++ 
                                       % strAddr
                                       % strCalcLon
                                       % strCalcLat).str();

    }

    GetStateMachine()->SendMessage(strCalcDestActionXml);

    return CL_TRUE;

}

bool
VR_SDSStateBase::DoActionCommitReport(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationCommitRPT* pcParserTmp = static_cast<const VR_SDSParserOperationCommitRPT*>(pcParser);

//    SetVRState(VR_SDS_STATE_BUSY);

//    const BL_String& rcReportContent = pcParserTmp->GetContent();
//    BL_String rcPictureContent;
//    if (pcParserTmp->IsPictureAttached()) {
//        rcPictureContent = GetPictureContent();
//    }

//    DWORD dwReqID = VR_INVALID_REQUET_ID;

//    GetPlatformIF()->SendReport(VR_SDSListenerFactory::GetRPTListener(), dwReqID, rcReportContent, rcPictureContent);
//    SetReqID(dwReqID);

//    VR_LOGD("|VR_SDSStateBase| |DoActionCommitReport| [ReqID:%lu Report:%s Picture:%s]\n", dwReqID, rcReportContent.GetString(), rcPictureContent.GetString());

    return CL_FALSE;
}

bool
VR_SDSStateBase::DoActionCommitWeibo(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationCommitWB* pcParserTmp = static_cast<const VR_SDSParserOperationCommitWB*>(pcParser);

//    SetVRState(VR_SDS_STATE_BUSY);

//    const BL_String& rcReportContent = pcParserTmp->GetContent();
//    BL_String rcPictureContent;
//    if (pcParserTmp->IsPictureAttached()) {
//        rcPictureContent = GetPictureContent();
//    }

//    DWORD dwReqID = VR_INVALID_REQUET_ID;

//    GetPlatformIF()->SendWeibo(VR_SDSListenerFactory::GetWBListener(), dwReqID, rcReportContent, rcPictureContent);
//    SetReqID(dwReqID);

//    VR_LOGD("|VR_SDSStateBase| |DoActionCommitWeibo| [ReqID:%lu Report:%s Picture:%s]\n", dwReqID, rcReportContent.GetString(), rcPictureContent.GetString());

    return CL_FALSE;
}

bool
VR_SDSStateBase::DoActionCheckWeibo(const VR_SDSOperationParser* pcParser)
{
//    bool bStatus	= GetPlatformIF()->IsWeiboBind();

//    VR_SDSResponseServerWBStatus cReq(bStatus);

//    DoActionRequestDialog(&cReq);

//    VR_LOGD("|VR_SDSStateBase| |DoActionCheckWeibo| [Status:%d]\n", bStatus);

    return CL_FALSE;
}

bool
VR_SDSStateBase::DoActionCall(const VR_SDSOperationParser* parser)
{
    VR_LOGD_FUNC();

    const VR_SDSParserOperationCall* callParser = static_cast<const VR_SDSParserOperationCall*>(parser);
    VR_VoiceBoxXmlBuilder xmlBuilder;
    string phoneNumber = callParser->GetPhone().GetString();

    string showPhoneView = "<action agent=\"phone\" op=\"showPhoneCall\"><phonename>";
    showPhoneView += phoneNumber;
    showPhoneView += "</phonename><phonenumber>";
    showPhoneView += phoneNumber;
    showPhoneView += "</phonenumber></action>";
    GetStateMachine()->SendMessage(showPhoneView);


    pugi::xml_node node = xmlBuilder.buildStartActionElement("phone", "dial");

    xmlBuilder.buildGivenElement(node, "dialType", "CALL_NUMBER", "", "");
    pugi::xml_node nodePhoneInfo = xmlBuilder.buildGivenElement(node, "phoneInfo", "", "", "");

    xmlBuilder.buildGivenElement(nodePhoneInfo, "contactName", "", "", "");
    xmlBuilder.buildGivenElement(nodePhoneInfo, "number", phoneNumber.c_str(), "", "");
    xmlBuilder.buildGivenElement(nodePhoneInfo, "contactId", "0", "", "");
    xmlBuilder.buildGivenElement(nodePhoneInfo, "phoneType", "-1", "", "");
    xmlBuilder.buildGivenElement(nodePhoneInfo, "listType", "0", "", "");

    std::string action = xmlBuilder.getXmlString();
    VR_LOGD("Call phone: [%s]", action.c_str());

    GetStateMachine()->SendMessage(action);

    return CL_TRUE;
}

bool VR_SDSStateBase::DoActionStop(const VR_SDSOperationParser* pcParser)
{
    VR_LOGD("|VR_SDSStateBase| |DoActionStop|\n");

    std::shared_ptr<VR_SDSStateMachine> pcStateMachine = GetStateMachine();
    if (NULL == pcStateMachine) {
        VR_LOGE("Base DoActionStop pcStateMachine is NULL\n");
        return CL_FALSE;
    }

    VR_SDSAbstractState* pcState = pcStateMachine->GetState(VR_SDSSERVER_STATE_TYPE_IDLE);
    pcStateMachine->ChangeStateTo(pcState);

    return CL_FALSE;
}

bool
VR_SDSStateBase::DoActionError(const VR_SDSOperationParser* pcParser)
{
    VR_LOGD_FUNC();
    const VR_SDSParserOperationError* pcParserTmp = static_cast<const VR_SDSParserOperationError*>(pcParser);

    VR_LOGE("|VR_SDSStateBase| |DoActionError| [Operation:%p Ercd:%d Info:%s]\n", pcParserTmp, pcParserTmp->GetErrorCode(), pcParserTmp->GetErrorInfo().GetString());

    DoActionError(pcParserTmp->GetErrorCode());

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionError(VR_SDSSERVER_ERROR eError)
{
    VR_LOGD_FUNC();
    VR_LOGE("|VR_SDSStateBase| |DoActionError| [Error:%d]\n", eError);

    std::shared_ptr<VR_SDSStateMachine> pcStateMachine = GetStateMachine();
    if (NULL == pcStateMachine) {
        VR_LOGE("Base DoActionError pcStateMachine is NULL\n");
        return CL_FALSE;
    }

    VR_SDSAbstractState* pcState = pcStateMachine->GetState(VR_SDSSERVER_STATE_TYPE_ERROR);

    VR_SDSStateServerError* pErrorState = static_cast<VR_SDSStateServerError*>(pcState);
    pErrorState->SetError(eError);

    pcStateMachine->ChangeStateTo(pErrorState);

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionRequestDialog(const VR_SDSRequestServerBase* pcReq)
{
    DWORD dwReqID = VR_INVALID_REQUET_ID;

    VR_RequestServerDialog* pcRequest = new VR_RequestServerDialog(
                GetStateMachine(), dwReqID, pcReq->GetRequest());

    if(nullptr == pcRequest) {
        return VR_PLATFORM_ERR_NOMEM;
    }
    SetReqID(dwReqID);

    pcRequest->DoAction();

    VR_LOGD("|VR_SDSStateBase| |DoActionRequestDialog| [ReqID:%lu Dialog:%s]\n",
           dwReqID, pcReq->GetRequest().GetString());

    return CL_FALSE;
}

bool
VR_SDSStateBase::DoActionBeep()
{
//    DWORD dwReqID = VR_INVALID_REQUET_ID;

//    GetPlatformIF()->Play(NULL, dwReqID, s_dwBeepID);
//    Sleep(200);

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionTTS(const string& rcTTS)
{
    SetVRState(VR_SDS_STATE_TTS);
    DWORD dwReqID = VR_INVALID_REQUET_ID;
    string action = "<action agent = \"prompt\" op = \"playTts\">";
    action += "<language>zh-cn</language>";
    action += "<text>" + rcTTS + "</text>";
    action += "<bargein>false</bargein>";
    action += "</action>";

    GetStateMachine()->SendMessage(action);

    // GetPlatformIF()->Play(VR_SDSListenerFactory::GetTTSListener(), dwReqID, rcTTS);
    // SetReqID(dwReqID);

    VR_LOGD("|VR_SDSStateBase| |DoActionTTS| [ReqID:%lu]\n", dwReqID);

    return CL_FALSE;
}

bool
VR_SDSStateBase::DoActionDisplayVR(const string& rcVRText)
{
//    GetPlatformIF()->UIShowText(rcVRText, UITEXT_TYPE_VR);

//    SessionAdd(rcVRText, CL_TRUE);

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionDisplayPrompt(const string& rcPrompt)
{
    // GetPlatformIF()->UIShowText(rcPrompt, UITEXT_TYPE_PROMPT);

    // SessionAdd(rcPrompt.c_str());

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionUIDisplayPrompt(const string &uiPrompt)
{
    if (uiPrompt.empty()) {
        return CL_TRUE;
    }

    static int nSquenceid = 0;
    char szData[32] = { 0 };
    sprintf(szData, "%d", nSquenceid++);
    std::string strSequence(szData);
    std::string strDisplay = "<action>"
            "<id>"+strSequence+"</id>"
            "<type>HMI_ShowList</type>"
            "<params>"
                   "<scenario>FREE_CHAT</scenario>"
                   "<HMI_HINT></HMI_HINT>"
                   "<HMI_TITLE></HMI_TITLE>"
                   "<HMI_USER_DATA>"+GetStateMachine()->GetUserData()+"</HMI_USER_DATA>"
                   "<HMI_CP_DATA>"+uiPrompt+"</HMI_CP_DATA>"
                   "<HMI_ICON_ID></HMI_ICON_ID>"
            "</params>"
       "</action>";

    VR_LOGD("DoActionUIDisplayPrompt = [%s]", strDisplay.c_str());
    GetStateMachine()->SendMessage(strDisplay);
    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionDisplayHints(const string& rcHints)
{
    // GetPlatformIF()->UIShowText(rcHints, UITEXT_TYPE_PROMPT_HINT);

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionDisplayPicture(const string& rcPic)
{
    // GetPlatformIF()->UIShowImage(rcPic);

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionStart()
{
    VR_LOGD("|VR_SDSStateBase| |DoActionStart|\n");

    // GetPlatformIF()->UIEnterVR();

    return CL_TRUE;
}


bool
VR_SDSStateBase::DoActionMusicStart(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationMusicStart* pcParserTmp = static_cast<const VR_SDSParserOperationMusicStart*>(pcParser);
//    DWORD dwReqID = VR_INVALID_REQUET_ID;
//    // call the platform IF to start play music
//    VR_MusicInfo sMusicInfo = {pcParserTmp->GetPlayList(),pcParserTmp->GetArtist(), pcParserTmp->GetSong(),
//                                pcParserTmp->GetAlbum(), pcParserTmp->GetGenre()};

//    GetPlatformIF()->PlayMusic(VR_SDSListenerFactory::GetPlayMusicListener(), dwReqID, sMusicInfo);
//    SetReqID(dwReqID);

//    VR_LOGD("|VR_SDSStateBase| |DoActionMusicStart| [PlayList:[%s] Artist:[%s] Song:[%s] Album:[%s] Genre:[%s]]\n",
//                pcParserTmp->GetPlayList().GetString(), pcParserTmp->GetArtist().GetString(), pcParserTmp->GetSong().GetString(),
//                pcParserTmp->GetAlbum().GetString(), pcParserTmp->GetGenre().GetString());

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMusicStop(const VR_SDSOperationParser* pcParser)
{
    // call the platform IF to stop play music
//    GetPlatformIF()->StopMusic();
    VR_LOGD("|VR_SDSStateBase| |DoActionMusicStop|\n");
/*
    /// temp code begin as the related method
    BL_String strPlayContent;
    strPlayContent.Format("停止播放音乐");
    return playContent(strPlayContent);
*/
    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMusicPause(const VR_SDSOperationParser* pcParser)
{
    // call the platform IF to Pause music play
//    GetPlatformIF()->PauseMusic();
    VR_LOGD("|VR_SDSStateBase| |DoActionMusicPause|\n");
/*
    /// temp code begin as the related method
    BL_String strPlayContent;
    strPlayContent.Format("暂停播放音乐");
    return playContent(strPlayContent);
*/
    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMusicNext(const VR_SDSOperationParser* pcParser)
{
    // call the platform IF to play next music
//    GetPlatformIF()->PlayNextMusic();
    VR_LOGD("|VR_SDSStateBase| |DoActionMusicNext|\n");
/*
    /// temp code begin as the related method
    BL_String strPlayContent;
    strPlayContent.Format("播放下一曲音乐");
    return playContent(strPlayContent);
*/
    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMusicPrev(const VR_SDSOperationParser* pcParser)
{
    // call the platform IF to play previous music
//    GetPlatformIF()->PlayPrevMusic();
    VR_LOGD("|VR_SDSStateBase| |DoActionDestinationNormal|\n");
/*
    /// temp code begin as the related method
    BL_String strPlayContent;
    strPlayContent.Format("播放上一曲音乐");
    return playContent(strPlayContent);
*/
    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionVolume(const VR_SDSOperationParser* pcParser)
{
    const VR_SDSParserOperationVolume* pcParserTmp = static_cast<const VR_SDSParserOperationVolume*>(pcParser);

    // call the platform IF to set the volume
    /// GetPlatformIF()->VolumeChange(pcParserTmp->GetType(), pcParserTmp->GetOperation(), pcParserTmp->GetValue());
    VR_LOGD("|VR_SDSStateBase| |DoActionVolume| [Type: %d, Operation:%d, Value: %lu]\n",
                pcParserTmp->GetType(), pcParserTmp->GetOperation(), pcParserTmp->GetValue());

//    switch (pcParserTmp->GetOperation()) {
//        case VR_VOLUME_OP_UP:
//            GetPlatformIF()->IncVolume(pcParserTmp->GetType(), pcParserTmp->GetValue());
//            break;
//        case VR_VOLUME_OP_DOWN:
//            GetPlatformIF()->DecVolume(pcParserTmp->GetType(), pcParserTmp->GetValue());
//            break;
//        case VR_VOLUME_OP_OFF:
//            GetPlatformIF()->SetMute(pcParserTmp->GetType(), true);
//            break;
//        case VR_VOLUME_OP_ON:
//            GetPlatformIF()->SetMute(pcParserTmp->GetType(), false);
//            break;
//    }

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapZoomIn(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationMapZoom* pcParserTmp = static_cast<const VR_SDSParserOperationMapZoom*>(pcParser);

//    // call the platform IF to zoom in on the map
//    /// GetPlatformIF()->MapZoom(VR_MAP_ZOOM_IN, pcParserTmp->GetZoomTimes());
//    VR_LOGD("|VR_SDSStateBase| |DoActionMapZoomIn| [ZoomTimes:%lu]\n", pcParserTmp->GetZoomTimes());

//    /// temp code begin as the related method
//    BL_String strPlayContent;
//    strPlayContent.Format("放大地图：放大倍数为%lu", pcParserTmp->GetZoomTimes());
//    return playContent(strPlayContent);


    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapZoomOut(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationMapZoom* pcParserTmp = static_cast<const VR_SDSParserOperationMapZoom*>(pcParser);

//    // call the platform IF to zoom out on the map
//    /// GetPlatformIF()->MapZoom(VR_MAP_ZOOM_OUT, pcParserTmp->GetZoomTimes());
//    VR_LOGD("|VR_SDSStateBase| |DoActionMapZoomOut| [ZoomTimes:%lu]\n", pcParserTmp->GetZoomTimes());

//    /// temp code begin as the related method
//    BL_String strPlayContent;
//    strPlayContent.Format("缩小地图：缩小倍数为%lu", pcParserTmp->GetZoomTimes());
//    return playContent(strPlayContent);


    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapZoom(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationMapZoom* pcParserTmp = static_cast<const VR_SDSParserOperationMapZoom*>(pcParser);

//    // call the platform IF to zoom upto a level on the map
//    /// GetPlatformIF()->MapZoom(pcParserTmp->GetZoomLevel());
//    VR_LOGD("|VR_SDSStateBase| |DoActionMapZoom| [ZoomLevel:%d]\n", pcParserTmp->GetZoomLevel());

//    /// temp code begin as the related method
//    BL_String strPlayContent;
//    strPlayContent.Format("调整地图大小：大小为%lu", pcParserTmp->GetZoomLevel());
//    return playContent(strPlayContent);


    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapMoveUp(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationMapMove* pcParserTmp = static_cast<const VR_SDSParserOperationMapMove*>(pcParser);

//    // call the platform IF to move the map to the up direction
//    if (0 != pcParserTmp->GetDistance()) {
//        /// GetPlatformIF()->MapMove(VR_MAP_DIR_UP, pcParserTmp->GetDistance());
//        VR_LOGD("|VR_SDSStateBase| |DoActionMapMoveUp| [MoveDistance:%lu]\n", pcParserTmp->GetDistance());

//        /// temp code begin as the related method
//        BL_String strPlayContent;
//        strPlayContent.Format("向上移动地图：距离为%lu", pcParserTmp->GetDistance());
//        return playContent(strPlayContent);
//    }
//    else {
//        ///GetPlatformIF()->MoveMapScreen(VR_MAP_DIR_UP, pcParserTmp->GetScreenSize()));
//        VR_LOGD("|VR_SDSStateBase| |DoActionMapMoveUp| [MoveScreen:%d]\n", pcParserTmp->GetScreenSize());

//        /// temp code begin as the related method
//        BL_String strPlayContent;
//        if (VR_MAP_SCREEN_HALF == pcParserTmp->GetScreenSize()) {
//            strPlayContent = NTEXT("向上移动地图半屏。");
//        } else if (VR_MAP_SCREEN_FULL == pcParserTmp->GetScreenSize()) {
//            strPlayContent = NTEXT("向上移动地图一屏。");
//        } else {
//            strPlayContent = NTEXT("向上移动地图，距离有误。");
//        }
//        return playContent(strPlayContent);
//    }

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapMoveDown(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationMapMove* pcParserTmp = static_cast<const VR_SDSParserOperationMapMove*>(pcParser);

//    // call the platform IF to move the map to the down direction
//    if (0 != pcParserTmp->GetDistance()) {
//        /// GetPlatformIF()->MapMove(VR_MAP_DIR_DOWN, pcParserTmp->GetDistance());
//        VR_LOGD("|VR_SDSStateBase| |DoActionMapMoveDown| [MoveDistance:%lu]\n", pcParserTmp->GetDistance());

//        /// temp code begin as the related method
//        BL_String strPlayContent;
//        strPlayContent.Format("向下移动地图：距离为%lu", pcParserTmp->GetDistance());
//        return playContent(strPlayContent);
//    }
//    else {
//        ///GetPlatformIF()->MoveMapScreen(VR_MAP_DIR_DOWN, pcParserTmp->GetScreenSize()));
//        VR_LOGD("|VR_SDSStateBase| |DoActionMapMoveDown| [MoveScreen:%d]\n", pcParserTmp->GetScreenSize());

//        /// temp code begin as the related method
//        BL_String strPlayContent;
//        if (VR_MAP_SCREEN_HALF == pcParserTmp->GetScreenSize()) {
//            strPlayContent = NTEXT("向下移动地图半屏。");
//        } else if (VR_MAP_SCREEN_FULL == pcParserTmp->GetScreenSize()) {
//            strPlayContent = NTEXT("向下移动地图一屏。");
//        } else {
//            strPlayContent = NTEXT("向下移动地图，距离有误。");
//        }
//        return playContent(strPlayContent);
//    }

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapMoveLeft(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationMapMove* pcParserTmp = static_cast<const VR_SDSParserOperationMapMove*>(pcParser);

//    // call the platform IF to move the map to the left direction
//    if (0 != pcParserTmp->GetDistance()) {
//        /// GetPlatformIF()->MapMove(VR_MAP_DIR_LEFT, pcParserTmp->GetDistance());
//        VR_LOGD("|VR_SDSStateBase| |DoActionMapMoveLeft| [MoveDistance:%lu]\n", pcParserTmp->GetDistance());

//        /// temp code begin as the related method
//        BL_String strPlayContent;
//        strPlayContent.Format("向左移动地图：距离为%lu", pcParserTmp->GetDistance());
//        return playContent(strPlayContent);
//    }
//    else {
//        ///GetPlatformIF()->MoveMapScreen(VR_MAP_DIR_LEFT, pcParserTmp->GetScreenSize()));
//        VR_LOGD("|VR_SDSStateBase| |DoActionMapMoveLeft| [MoveScreen:%d]\n", pcParserTmp->GetScreenSize());

//        /// temp code begin as the related method
//        BL_String strPlayContent;
//        if (VR_MAP_SCREEN_HALF == pcParserTmp->GetScreenSize()) {
//            strPlayContent = NTEXT("向左移动地图半屏。");
//        } else if (VR_MAP_SCREEN_FULL == pcParserTmp->GetScreenSize()) {
//            strPlayContent = NTEXT("向左移动地图一屏。");
//        } else {
//            strPlayContent = NTEXT("向左移动地图，距离有误。");
//        }
//        return playContent(strPlayContent);
//    }

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapMoveRight(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationMapMove* pcParserTmp = static_cast<const VR_SDSParserOperationMapMove*>(pcParser);

//    // call the platform IF to move the map to the right direction
//    if (0 != pcParserTmp->GetDistance()) {
//        /// GetPlatformIF()->MapMove(VR_MAP_DIR_RIGHT, pcParserTmp->GetDistance());
//        VR_LOGD("|VR_SDSStateBase| |DoActionMapMoveRight| [MoveDistance:%lu]\n", pcParserTmp->GetDistance());

//        /// temp code begin as the related method
//        BL_String strPlayContent;
//        strPlayContent.Format("向右移动地图：距离为%lu", pcParserTmp->GetDistance());
//        return playContent(strPlayContent);
//    }
//    else {
//        ///GetPlatformIF()->MoveMapScreen(VR_MAP_DIR_RIGHT, pcParserTmp->GetScreenSize()));
//        VR_LOGD("|VR_SDSStateBase| |DoActionMapMoveRight| [MoveScreen:%d]\n", pcParserTmp->GetScreenSize());

//        /// temp code begin as the related method
//        BL_String strPlayContent;
//        if (VR_MAP_SCREEN_HALF == pcParserTmp->GetScreenSize()) {
//            strPlayContent = NTEXT("向右移动地图半屏。");
//        } else if (VR_MAP_SCREEN_FULL == pcParserTmp->GetScreenSize()) {
//            strPlayContent = NTEXT("向右移动地图一屏。");
//        } else {
//            strPlayContent = NTEXT("向右移动地图，距离有误。");
//        }
//        return playContent(strPlayContent);
//    }

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapMoveCurrentPos(const VR_SDSOperationParser* pcParser)
{
//    const VR_SDSParserOperationMapMove* pcParserTmp = static_cast<const VR_SDSParserOperationMapMove*>(pcParser);

//    // call the platform IF to set the current position as the center of the map
//    /// GetPlatformIF()->MapMoveCurrentPos();
//    VR_LOGD("|VR_SDSStateBase| |DoActionMapMoveCurrentPos|\n");

//    /// temp code begin as the related method
//    BL_String strPlayContent;
//    strPlayContent.Format("移动地图到当前位置。");
//    return playContent(strPlayContent);

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapDirNorthUp(const VR_SDSOperationParser* pcParser)
{

//    // call the platform IF to set the map as northup direction
//    /// GetPlatformIF()->MapChangeDir(VR_MAP_DIR_NORTH_UP);
//    VR_LOGD("|VR_SDSStateBase| |DoActionMapDirNorthUp|\n");

//    /// temp code begin as the related method
//    BL_String strPlayContent;
//    strPlayContent.Format("设置地图北面向上。");
//    return playContent(strPlayContent);


    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapDirHeadingUp(const VR_SDSOperationParser* pcParser)
{

//    // call the platform IF to set the map as headingup direction
//    /// GetPlatformIF()->MapChangeDir(VR_MAP_DIR_HEADING_UP);

//    /// temp code begin as the related method
//    BL_String strPlayContent;
//    strPlayContent.Format("设置地图车头向上。");
//    return playContent(strPlayContent);

//    VR_LOGD("|VR_SDSStateBase| |DoActionMapDirHeadingUp|\n");

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapViewIR(const VR_SDSOperationParser* pcParser)
{
//    // call the platform IF to set the map as IR view
//    /// GetPlatformIF()->MapChangeView(VR_MAP_VIEW_IR);
//    VR_LOGD("|VR_SDSStateBase| |DoActionMapViewIR|\n");

//    /// temp code begin as the related method
//    BL_String strPlayContent;
//    strPlayContent.Format("设置地图视图为IR模式。");
//    return playContent(strPlayContent);


    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapViewSky(const VR_SDSOperationParser* pcParser)
{
//    // call the platform IF to set the map as sky view
//    /// GetPlatformIF()->MapChangeView(VR_MAP_VIEW_SKY);
//    VR_LOGD("|VR_SDSStateBase| |DoActionMapViewSky|\n");

//    /// temp code begin as the related method
//    BL_String strPlayContent;
//    strPlayContent.Format("设置地图视图为3D模式。");
//    return playContent(strPlayContent);

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapViewNormal(const VR_SDSOperationParser* pcParser)
{

//    // call the platform IF to set the map as normal view
//    /// GetPlatformIF()->MapChangeView(VR_MAP_VIEW_NORMAL);
//    VR_LOGD("|VR_SDSStateBase| |DoActionMapViewNormal|\n");

//    /// temp code begin as the related method
//    BL_String strPlayContent;
//    strPlayContent.Format("设置地图视图为正常模式。");
//    return playContent(strPlayContent);


    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionMapViewChange(const VR_SDSOperationParser* pcParser)
{
//    // call the platform IF to change the map view
//    /// GetPlatformIF()->MapChangeView(VR_MAP_VIEW_NEXT);
//    VR_LOGD("|VR_SDSStateBase| |DoActionMapViewChange|\n");

//    /// temp code begin as the related method
//    BL_String strPlayContent;
//    strPlayContent.Format("循环切换地图视图模式。");
//    return playContent(strPlayContent);


    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionShowPos(const VR_SDSOperationParser* pcParser)
{
//    //TODO: call the platform IF to show current position
//    /// GetPlatformIF()->ShowPos();
//    VR_LOGD("|VR_SDSStateBase| |DoActionShowPos in|\n");

//    /// temp code begin as the related method
//    BL_String strPlayContent;
//    strPlayContent.Format("显示当前位置");
//    return playContent(strPlayContent);

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionShowRoute(const VR_SDSOperationParser* pcParser)
{
//    // call the platform IF to show current route
//    /// GetPlatformIF()->ShowRoute();
//    VR_LOGD("|VR_SDSStateBase| |DoActionShowRoute in|\n");

//    /// temp code begin as the related method
//    BL_String strPlayContent;
//    strPlayContent.Format("显示导航路线");
//    return playContent(strPlayContent);

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionSendSMS(const VR_SDSOperationParser* pcParser)
{
//    VR_LOGD("|VR_SDSStateBase| |DoActionSendSMS|\n");

//    const VR_SDSParserOperationSendSMS* pcParserTmp = static_cast<const VR_SDSParserOperationSendSMS*>(pcParser);
//    GetPlatformIF()->SendSMS(pcParserTmp->GetName(), pcParserTmp->GetPhone(), pcParserTmp->GetContent());

//    VR_LOGD("|VR_SDSStateBase| |DoActionSendSMS| Content =%s\n", pcParserTmp->GetContent().GetString());
//    VR_LOGD("|VR_SDSStateBase| |DoActionSendSMS| name =%s\n", pcParserTmp->GetName().GetString());
//    VR_LOGD("|VR_SDSStateBase| |DoActionSendSMS| phone =%s\n", pcParserTmp->GetPhone().GetString());

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionLogomark(const VR_SDSOperationParser* pcParser)
{
//    VR_LOGD("|VR_SDSStateBase| |DoActionLogomark|\n");

//    const VR_SDSParserOperationLogomark* pcParserTmp = static_cast<const VR_SDSParserOperationLogomark*>(pcParser);

//    VR_LOGD("|VR_SDSStateBase| |DoActionLogomark| Content =%s\n", pcParserTmp->GetID().GetString());
//    VR_LOGD("|VR_SDSStateBase| |DoActionLogomark| name =%s\n", pcParserTmp->GetName().GetString());

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionAppControl(const VR_SDSOperationParser* pcParser)
{
//    VR_LOGD("|VR_SDSStateBase| |DoActionAppControl|\n");

//    const VR_SDSParserOperationAppControl* pcParserTmp = static_cast<const VR_SDSParserOperationAppControl*>(pcParser);

//    DWORD dwReqID = VR_INVALID_REQUET_ID;
//    GetPlatformIF()->StartApp(VR_SDSListenerFactory::GetStartAppListener(), dwReqID, pcParserTmp->GetAppNameContent());
//    SetReqID(dwReqID);

//    VR_LOGD("|VR_SDSStateBase| |DoActionAppControl| appname =%s reqID=%lu\n", pcParserTmp->GetAppNameContent().GetString(), dwReqID);

    return CL_TRUE;
}

bool
VR_SDSStateBase::DoActionRadioControl(const VR_SDSOperationParser* pcParser)
{
//    VR_LOGD("|VR_SDSStateBase| |DoActionRadioControl|\n");

//    const VR_SDSParserOperationRadioControl* pcParserTmp = static_cast<const VR_SDSParserOperationRadioControl*>(pcParser);

//    BL_String cRadioappname = pcParserTmp->GetAppNameContent();
//    BL_String cAppParam;

//    if (BL_String("AM") == pcParserTmp->GetRadioType()) {
//        cAppParam.Format("TYPE:AM|FREQ:%s", pcParserTmp->GetRadioFreq().GetString());
//    }
//    else if(BL_String("FM") == pcParserTmp->GetRadioType()) {
//        cAppParam.Format("TYPE:FM|FREQ:%s",  pcParserTmp->GetRadioFreq().GetString());
//    }
//    else {
//    }

//    VR_LOGD("start Radio %s, param=%s\n", cRadioappname.GetString(), cAppParam.GetString());

//    DWORD dwReqID = VR_INVALID_REQUET_ID;
//    GetPlatformIF()->StartAppWithParam(VR_SDSListenerFactory::GetStartRadioListener(), dwReqID, cRadioappname, cAppParam);
//    SetReqID(dwReqID);

    return CL_FALSE;

}

/// temp code of play the content as the related platform if are not commited
bool
VR_SDSStateBase::playContent(const string& rcTTS)
{
    VR_LOGD("|VR_SDSStateBase| |VR_SDSStateBase| [the related platform IF is not ready just play TTS]\n");
    DoActionDisplayPrompt(rcTTS);
    return DoActionTTS(rcTTS);
}


bool
VR_SDSStateBase::DoActionDisplayWeatherInfoListOn(const VR_SDSOperationParser* pcParser)
{

    VR_LOGD_FUNC();

    const VR_SDSParserOperationDisplayWeatherInfoListOn* pcParserTmp = static_cast<const VR_SDSParserOperationDisplayWeatherInfoListOn*>(pcParser);

//    GetPlatformIF()->UIShowPOIList(pcParserTmp->GetPOIInfo(), pcParserTmp->GetPOIInfoNum());

    VR_LOGD("|VR_SDSStateBase| |DoActionDisplayWeatherInfoListOn| [WeatherForecastInfo:%p WeatherForecastNum:%lu]\n", pcParserTmp->GetWeatherForecastInfo(), pcParserTmp->GetWeatherForecastNum());


    VR_WeatherInfoForecast* weatherForecastInfo = pcParserTmp->GetWeatherForecastInfo();
    int nWeatherForecastNum = pcParserTmp->GetWeatherForecastNum();


    VR_WeatherInfoToday* weatherTodayInfo = pcParserTmp->GetWeahterTodayInfo();
    VR_WeatherBasicInfo* weatheBasicInfo = pcParserTmp->GetWeatherBasicInfo();


    std::string strTodayInfo = weatherTodayInfo->GetJsonStr().GetString();
    std::string strBasicInfo = weatheBasicInfo->GetJsonStr().GetString();
    VR_LOGD("todayinfo:%s", strTodayInfo.c_str());
    VR_LOGD("basicinfo:%s", strBasicInfo.c_str());


    std::string weatherInfoHeadXML = "<display agent=\"Common\" content=\"ScreenDisplay\">"          
                                               "<pictureId></pictureId>"
                                               "<content>WeatherTemplate</content>"
                                               "<mainTitle>%1%</mainTitle>"
                                               "<subTitle>%5%</subTitle>"
                                               "<lowTemperature>%2%</lowTemperature>"
                                               "<currentWeather>%3%</currentWeather>"
                                               "<highTemperature>%4%</highTemperature>"
                                               "<currentWeatherUrl></currentWeatherUrl>";

    std::string strWeatherInfoDisplayXML = ""; 

    std::string strMainTitle = "";
    std::string strLowTemperature = "";
    std::string strCurrentWeather = "";
    std::string strHighTemperature = "";
    std::string strSubTitle = "";

    rapidjson::Document jsonBasicInfoDoc;
    jsonBasicInfoDoc.Parse<0>(strBasicInfo.c_str());

    if (jsonBasicInfoDoc.HasParseError()) {
        VR_LOGE("jsonBasicInfoDoc:json format was error");
        return CL_FALSE;
    }

    if (jsonBasicInfoDoc.HasMember("VR_WeatherBasicInfo")) {
        const rapidjson::Value& objVrBasicInfo = jsonBasicInfoDoc["VR_WeatherBasicInfo"];
        if (objVrBasicInfo.IsObject()) {
            if (objVrBasicInfo.HasMember("adminArea")) {
                strMainTitle = objVrBasicInfo["adminArea"].GetString();
            }
        }
    }

    rapidjson::Document jsonTodayInfoDoc;
    jsonTodayInfoDoc.Parse<0>(strTodayInfo.c_str());

    if (jsonTodayInfoDoc.HasParseError()) {
        VR_LOGE("jsonTodayInfoDoc:json format was error");
        return CL_FALSE;
    }

    if (jsonTodayInfoDoc.HasMember("VR_WeatherTodayInfo")) {
        const rapidjson::Value& objVrTodayInfo = jsonTodayInfoDoc["VR_WeatherTodayInfo"];
        if (objVrTodayInfo.IsObject()) {
            if (objVrTodayInfo.HasMember("temperature")) {
                strCurrentWeather = objVrTodayInfo["temperature"].GetString();
            }

            if (objVrTodayInfo.HasMember("fl")) {
                strLowTemperature = objVrTodayInfo["fl"].GetString();
            }
        }
    }

    // strWeatherInfoDisplayXML = (boost::format(weatherInfoHeadXML)
    //                              % strMainTitle
    //                              % strLowTemperature
    //                              % strCurrentWeather
    //                              % strHighTemperature
    //                              % strSubTitle).str();

    std::string weatherForecastXML = "<weatherForecast>"
                                                   "<highTemperature>%1%</highTemperature>"
                                                   "<date>%2%</date>"
                                                   "<lowTemperature>%3%</lowTemperature>"
                                                   "<contentDescription>%4%</contentDescription>"
                                                   "<url></url>"
                                    "</weatherForecast>";


    // VR_WeatherInfoForecast*

    std::string strItemsWeatherForecasts = "";

    for(int i = 0;i < nWeatherForecastNum; ++i) {
        std::string strForecastInfo = weatherForecastInfo[i].GetJsonStr().GetString();
        VR_LOGD("DoActionDisplayWeatherInfoListOn:%s", strForecastInfo.c_str());
    
        rapidjson::Document jsonForecastDoc;
        jsonForecastDoc.Parse<0>(strForecastInfo.c_str());

        if (jsonForecastDoc.HasParseError()) {
            VR_LOGE("json format was error");
            return CL_FALSE;
        }

        std::string maxTemperature = "";
        std::string date = "";
        std::string minTemperature = "";
        std::string contentDescription = "";

        if (jsonForecastDoc.HasMember("VR_WeatherForecastInfo")) {
            const rapidjson::Value& objVrForecastInfo = jsonForecastDoc["VR_WeatherForecastInfo"];
            if (objVrForecastInfo.IsObject()) {

                if (objVrForecastInfo.HasMember("date")) {
                    date = objVrForecastInfo["date"].GetString();
                }

                if (objVrForecastInfo.HasMember("descOfDayWeather")) {
                    contentDescription = objVrForecastInfo["descOfDayWeather"].GetString();
                }

                if (objVrForecastInfo.HasMember("maxTemperature")) {
                    maxTemperature = objVrForecastInfo["maxTemperature"].GetString();
                }

                if (objVrForecastInfo.HasMember("minTemperature")) {
                    minTemperature = objVrForecastInfo["minTemperature"].GetString();
                }

            }
        }

        if (0 == i) {
            strLowTemperature = minTemperature;
            strHighTemperature = maxTemperature;
            strSubTitle = date;
        }

        date = date.substr(5);

        strItemsWeatherForecasts += (boost::format(weatherForecastXML) 
                                 % maxTemperature
                                 % date
                                 % minTemperature
                                 % contentDescription).str();
    }

    strItemsWeatherForecasts +=  "<weatherForecast>"
                                                   "<highTemperature>-</highTemperature>"
                                                   "<date>-</date>"
                                                   "<lowTemperature>-</lowTemperature>"
                                                   "<contentDescription></contentDescription>"
                                                   "<url></url>"
                                               "</weatherForecast>";
    
    strItemsWeatherForecasts +=  "<weatherForecast>"
                                                   "<highTemperature>-</highTemperature>"
                                                   "<date>-</date>"
                                                   "<lowTemperature>-</lowTemperature>"
                                                   "<contentDescription></contentDescription>"
                                                   "<url></url>"
                                               "</weatherForecast>";


    strWeatherInfoDisplayXML = (boost::format(weatherInfoHeadXML)
                                 % strMainTitle
                                 % strLowTemperature
                                 % strCurrentWeather
                                 % strHighTemperature
                                 % strSubTitle).str();

    strWeatherInfoDisplayXML += strItemsWeatherForecasts;

    strWeatherInfoDisplayXML += "</display>";



 /*   std::string strWeatherInfoDisplayXML = "<display agent=\"Common\" content=\"ScreenDisplay\">"          
                                               "<pictureId></pictureId>"
                                               "<content>WeatherTemplate</content>"
                                               "<mainTitle>Seattle, WA</mainTitle>"
                                               "<subTitle>Wednesday, January 3, 2018</subTitle>"
                                               "<lowTemperature>1°</lowTemperature>"
                                               "<currentWeather>6°</currentWeather>"
                                               "<highTemperature>10°</highTemperature>"
                                               "<currentWeatherUrl></currentWeatherUrl>"      
                                               "<weatherForecast>"
                                                   "<highTemperature>9°</highTemperature>"
                                                   "<date>Jan 4</date>"
                                                   "<lowTemperature>6°</lowTemperature>"
                                                   "<contentDescription>Clear night</contentDescription>"
                                                   "<url></url>"
                                               "</weatherForecast>"
                                               "<weatherForecast>"
                                                   "<highTemperature>9°</highTemperature>"
                                                   "<date>Jan 4</date>"
                                                   "<lowTemperature>1°</lowTemperature>"
                                                   "<contentDescription>Clear night</contentDescription>"
                                                   "<url></url>"
                                               "</weatherForecast>"
                                               "<weatherForecast>"
                                                   "<highTemperature>9°</highTemperature>"
                                                   "<date>Jan 4</date>"
                                                   "<lowTemperature>6°</lowTemperature>"
                                                   "<contentDescription>Clear night</contentDescription>"
                                                   "<url></url>"
                                               "</weatherForecast>"
                                               // "<weatherForecast>"
                                               //     "<highTemperature>9°</highTemperature>"
                                               //     "<date>Jan 4</date>"
                                               //     "<lowTemperature>6°</lowTemperature>"
                                               //     "<contentDescription>Clear night</contentDescription>"
                                               //     "<url></url>"
                                               // "</weatherForecast>"
                                               // "<weatherForecast>"
                                               //     "<highTemperature>9°</highTemperature>"
                                               //     "<date>Jan 4</date>"
                                               //     "<lowTemperature>6°</lowTemperature>"
                                               //     "<contentDescription>Clear night</contentDescription>"
                                               //     "<url></url>"
                                               // "</weatherForecast>"
                                           "</display>";
*/
    VR_LOGD("DoActionDisplayWeatherInfoListOn = [%s]", strWeatherInfoDisplayXML.c_str());
    GetStateMachine()->SendMessage(strWeatherInfoDisplayXML);

    return CL_TRUE;
}