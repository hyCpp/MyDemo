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

#include "VR_SDSParserConcrete.h"
// #include "VR_POIInfo.h"
#include "VR_Log.h"

using namespace std;

VR_SDSParserPOI::VR_SDSParserPOI()
    : m_dwInfoNum(0), m_pcInfo(NULL)
{

}

VR_SDSParserPOI::~VR_SDSParserPOI()
{
    Release();
}

VOID VR_SDSParserPOI::Release()
{
    if (NULL != m_pcInfo) {
        delete[] m_pcInfo;
        m_pcInfo = NULL;
    }

    m_dwInfoNum = 0;

    VR_LOGD("|VR_SDSParserPOI| |Release|\n");
}

CL_BOOL VR_SDSParserPOI::ParseResult(const BL_String& rcResult)
{
    TiXmlDocument cXmlDoc;
    cXmlDoc.Parse(rcResult.GetString());

//	VR_LOGD("|VR_SDSParserPOI| |ParseResult| [POI Xml:%s]\n", rcResult.GetString());

    const TiXmlNode* pcXmlNode = cXmlDoc.RootElement();
    if (NULL == pcXmlNode) {
        return CL_FALSE;
    }
    const TiXmlElement* pcXmlElem = pcXmlNode->ToElement();
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }
    // RootElement should be "result"
    if (BL_String("result") != pcXmlElem->Value()) {
        return CL_FALSE;
    }

    return ParseResult(pcXmlElem);
}

CL_BOOL VR_SDSParserPOI::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }
    // Check node "exception"
    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("exception");
    if (NULL != pcXmlElemTmp) {
        ParseException(pcXmlElemTmp);

        m_bHasError = CL_TRUE;
        return CL_TRUE;
    }
    // Check node "info"
    pcXmlElemTmp = pcXmlElem->FirstChildElement("info");
    if (NULL == pcXmlElemTmp) {
        return CL_FALSE;
    }
    if (!ParseInfo(pcXmlElemTmp)) {
        return CL_FALSE;
    }
    // Loop node "data"
    DWORD dwIndex = 0;
    for (pcXmlElem = pcXmlElem->FirstChildElement("data"); NULL != pcXmlElem; pcXmlElem = pcXmlElem->NextSiblingElement("data")) {
        if (!ParseData(pcXmlElem, dwIndex++)) {
            VR_LOGE("|VR_SDSParserPOI| |ParseResult| [Error Index:%lu]\n", dwIndex - 1);
            return CL_FALSE;
        }
    }
    // Size check
    if (dwIndex != m_dwInfoNum) {
        return CL_FALSE;
    }

    return CL_TRUE;
}

VR_POIInfo * VR_SDSParserPOI::GetInfo(DWORD dwIndex) const
{
    return &m_pcInfo[dwIndex];
}

CL_BOOL
VR_SDSParserPOI::ParseException(const TiXmlElement* pcXmlElem)
{
    BL_String cErrorCode;
    BL_String cErrorMsg;

    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("errorcode");
    if (NULL != pcXmlElemTmp) {
        cErrorCode = pcXmlElemTmp->GetText();
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("errormsg");
    if (NULL != pcXmlElemTmp) {
        cErrorMsg = pcXmlElemTmp->GetText();
    }

    VR_LOGE("|VR_SDSParserPOI| |ParseException| Error[ErrorCode:%s ErrorMsg:%s]\n", cErrorCode.GetString(), cErrorMsg.GetString());

    return CL_TRUE;
}

CL_BOOL VR_SDSParserPOI::ParseInfo(const TiXmlElement* pcXmlElem)
{
    BL_String cErrorCode;
    BL_String cCount;

    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("count");
    if (NULL == pcXmlElemTmp) {
        VR_LOGE("|VR_SDSParserPOI| |ParseInfo| Error[Count]\n");
        return CL_FALSE;
    }

    cCount = pcXmlElemTmp->GetText();

    m_dwInfoNum = atoi(cCount.GetString());

    if (NULL != m_pcInfo) {
        delete[] m_pcInfo;
        m_pcInfo = NULL;
    }

    if (m_dwInfoNum > 0) {
        m_pcInfo = new VR_POIInfo[m_dwInfoNum];
    }

    VR_LOGD("|VR_SDSParserPOI| |ParseInfo| [InfoNum:%lu Info:%p]\n", m_dwInfoNum, m_pcInfo);

    return CL_TRUE;
}

CL_BOOL VR_SDSParserPOI::ParseData(const TiXmlElement* pcXmlElem, DWORD dwIndex)
{
   if (dwIndex >= m_dwInfoNum) {
       VR_LOGE("|VR_SDSParserPOI| |ParseData| Error[Invalid Data Count]\n");
       return CL_FALSE;
   }
   // name
   const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("name");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cName = pcXmlElemTmp->GetText();
   }

   // addr
   pcXmlElemTmp = pcXmlElem->FirstChildElement("addr");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cAddr= pcXmlElemTmp->GetText();
   }

   // distance
   pcXmlElemTmp = pcXmlElem->FirstChildElement("distance");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cDistance= pcXmlElemTmp->GetText();
   }

   // entrylat
   pcXmlElemTmp = pcXmlElem->FirstChildElement("entrylat");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cEntryLat= pcXmlElemTmp->GetText();
   }

   // entrylon
   pcXmlElemTmp = pcXmlElem->FirstChildElement("entrylon");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cEntryLon= pcXmlElemTmp->GetText();
   }

   // genre_id
   pcXmlElemTmp = pcXmlElem->FirstChildElement("genre_id");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cGenreId= pcXmlElemTmp->GetText();
   }

   // lat
   pcXmlElemTmp = pcXmlElem->FirstChildElement("lat");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cLat = pcXmlElemTmp->GetText();
   }

   // lon
   pcXmlElemTmp = pcXmlElem->FirstChildElement("lon");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cLon = pcXmlElemTmp->GetText();
   }

   // linkid
   pcXmlElemTmp = pcXmlElem->FirstChildElement("linkid");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cLinkId = pcXmlElemTmp->GetText();
   }

   // linkside
   pcXmlElemTmp = pcXmlElem->FirstChildElement("linkside");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cLinkSide = pcXmlElemTmp->GetText();
   }

   // poiid
   pcXmlElemTmp = pcXmlElem->FirstChildElement("poiid");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cPOIId = pcXmlElemTmp->GetText();
   }

   // tel
   pcXmlElemTmp = pcXmlElem->FirstChildElement("tel");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cTel = pcXmlElemTmp->GetText();
   }

   // type
   pcXmlElemTmp = pcXmlElem->FirstChildElement("type");
   if (NULL != pcXmlElemTmp) {
       m_pcInfo[dwIndex].cType = pcXmlElemTmp->GetText();
   }

    return CL_TRUE;
}



// Parse Contact list
VR_SDSParserContact::VR_SDSParserContact()
    : m_dwInfoNum(0), m_pcInfo(NULL)
{
}

VR_SDSParserContact::~VR_SDSParserContact()
{
    Release();
}

VOID VR_SDSParserContact::Release()
{
    if (NULL != m_pcInfo) {
        delete[] m_pcInfo;
        m_pcInfo = NULL;
    }

    m_dwInfoNum = 0;

    VR_LOGD("|VR_SDSParserContact| |Release|\n");
}

CL_BOOL VR_SDSParserContact::ParseResult(const BL_String& rcResult)
{
    TiXmlDocument cXmlDoc;
    cXmlDoc.Parse(rcResult.GetString());

//	VR_LOGD("|VR_SDSParserContact| |ParseResult| [POI Xml:%s]\n", rcResult.GetString());

    const TiXmlNode* pcXmlNode = cXmlDoc.RootElement();
    if (NULL == pcXmlNode) {
        return CL_FALSE;
    }

    const TiXmlElement* pcXmlElem = pcXmlNode->ToElement();
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    // RootElement should be "result"
    if (BL_String("result") != pcXmlElem->Value()) {
        return CL_FALSE;
    }

    return ParseResult(pcXmlElem);
}

CL_BOOL VR_SDSParserContact::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    // FIXME: use dynamic array.
    const DWORD ITEM_COUNT = 20;
    m_pcInfo = new VR_ContactInfo[ITEM_COUNT];

    // Loop node "contact"
    const XCHAR* itemTag = "contact";
    DWORD dwIndex = 0;
    for (pcXmlElem = pcXmlElem->FirstChildElement(itemTag); NULL != pcXmlElem; pcXmlElem = pcXmlElem->NextSiblingElement(itemTag)) {
        if (!ParseData(pcXmlElem, dwIndex++)) {
            VR_LOGE("|VR_SDSParserContact| |ParseResult| [Error Index:%lu]\n", dwIndex - 1);
            return CL_FALSE;
        }

        //FIXME:
        if (dwIndex>=ITEM_COUNT) break;
    }

    m_dwInfoNum = dwIndex;
    return CL_TRUE;
}

VR_ContactInfo*
VR_SDSParserContact::GetInfo(DWORD dwIndex) const
{
    return &m_pcInfo[dwIndex];
}

CL_BOOL VR_SDSParserContact::ParseData(const TiXmlElement* pcXmlElem, DWORD dwIndex)
{
//    const TiXmlElement* pcXmlElemSub = pcXmlElem;
//    // name
//    const TiXmlElement* pcItem = pcXmlElem->FirstChildElement("name");
//    if (NULL != pcItem) {
//        m_pcInfo[dwIndex].cName = pcItem->GetText();
//    }

//    // FIXME: use dynamic array.
//    const DWORD ITEM_COUNT = 4;
//    m_pcInfo[dwIndex].cPhones = new VR_PhoneInfo[ITEM_COUNT];

//    // Loop node "phone"
//    const XCHAR* itemTag = "phone";
//    VR_PhoneInfo* pPhoneInfo = m_pcInfo[dwIndex].cPhones;
//    DWORD dwIndexSub = 0;
//    for (pcXmlElemSub = pcXmlElemSub->FirstChildElement(itemTag); NULL != pcXmlElemSub; pcXmlElemSub = pcXmlElemSub->NextSiblingElement(itemTag)) {
//        // number
//        pcItem = pcXmlElemSub->FirstChildElement("number");
//        if (NULL != pcItem) {
//            pPhoneInfo[dwIndexSub].cNumber = pcItem->GetText();
//        }

//        // type
//        pcItem = pcXmlElemSub->FirstChildElement("type");
//        if (NULL != pcItem) {
//            pPhoneInfo[dwIndexSub].cType = pcItem->GetText();
//        }

//        dwIndexSub++;

//        // FIXME:
//        if (dwIndexSub>=ITEM_COUNT) break;
//    }
//    m_pcInfo[dwIndex].cSize = dwIndexSub;

    return CL_TRUE;
}


// Parse News list result
VR_SDSParserNews::VR_SDSParserNews()
    : m_dwInfoNum(0), m_pcInfo(NULL)
{
}

VR_SDSParserNews::~VR_SDSParserNews()
{
    Release();
}

VOID VR_SDSParserNews::Release()
{
    if (NULL != m_pcInfo) {
        delete[] m_pcInfo;
        m_pcInfo = NULL;
    }

    m_dwInfoNum = 0;

    VR_LOGD("|VR_SDSParserContact| |Release|\n");
}

CL_BOOL VR_SDSParserNews::ParseResult(const BL_String& rcResult)
{
    TiXmlDocument cXmlDoc;
    cXmlDoc.Parse(rcResult.GetString());

//	VR_LOGD("|VR_SDSParserContact| |ParseResult| [POI Xml:%s]\n", rcResult.GetString());

    const TiXmlNode* pcXmlNode = cXmlDoc.RootElement();
    if (NULL == pcXmlNode) {
        return CL_FALSE;
    }

    const TiXmlElement* pcXmlElem = pcXmlNode->ToElement();
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    // RootElement should be "result"
    if (BL_String("result") != pcXmlElem->Value()) {
        return CL_FALSE;
    }

    return ParseResult(pcXmlElem);
}

CL_BOOL VR_SDSParserNews::ParseResult(const TiXmlElement* pcXmlElem)
{
    VR_LOGD_FUNC();
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    // FIXME: use dynamic array.
    const DWORD ITEM_COUNT = 20;
    m_pcInfo = new VR_NewsInfo[ITEM_COUNT];

    // Loop node "item"
    const XCHAR* itemTag = "item";
    DWORD dwIndex = 0;
    for (pcXmlElem = pcXmlElem->FirstChildElement(itemTag); NULL != pcXmlElem; pcXmlElem = pcXmlElem->NextSiblingElement(itemTag)) {
        if (!ParseData(pcXmlElem, dwIndex++)) {
            VR_LOGE("|VR_SDSParserContact| |ParseResult| [Error Index:%lu]\n", dwIndex - 1);
            return CL_FALSE;
        }

        //FIXME:
        if (dwIndex>=ITEM_COUNT) break;
    }

    m_dwInfoNum = dwIndex;
    return CL_TRUE;
}

VR_NewsInfo* VR_SDSParserNews::GetInfo(DWORD dwIndex) const
{
    return &m_pcInfo[dwIndex];
}

CL_BOOL VR_SDSParserNews::ParseData(const TiXmlElement* pcXmlElem, DWORD dwIndex)
{
    // title
//    const TiXmlElement* pcItem = pcXmlElem->FirstChildElement("title");
//    if (NULL != pcItem) {
//        m_pcInfo[dwIndex].cTitle = pcItem->GetText();
//    }

//    // description
//    pcItem = pcXmlElem->FirstChildElement("description");
//    if (NULL != pcItem) {
//        m_pcInfo[dwIndex].cDescription = pcItem->GetText();
//    }

//    // source
//    pcItem = pcXmlElem->FirstChildElement("source");
//    if (NULL != pcItem) {
//        m_pcInfo[dwIndex].cSource = pcItem->GetText();
//    }

//    // link
//    pcItem = pcXmlElem->FirstChildElement("link");
//    if (NULL != pcItem) {
//        m_pcInfo[dwIndex].cLink = pcItem->GetText();
//    }

    return CL_TRUE;
}

// Parse Help result
VR_SDSParserHelp::VR_SDSParserHelp()
    : m_dwInfoNum(0), m_pcInfo(NULL)
{
}

VR_SDSParserHelp::~VR_SDSParserHelp()
{
    Release();
}

VOID VR_SDSParserHelp::Release()
{
    if (NULL != m_pcInfo) {
        delete[] m_pcInfo;
        m_pcInfo = NULL;
    }

    m_dwInfoNum = 0;

    VR_LOGD("|VR_SDSParserContact| |Release|\n");
}

CL_BOOL VR_SDSParserHelp::ParseResult(const BL_String& rcResult)
{
    TiXmlDocument cXmlDoc;
    cXmlDoc.Parse(rcResult.GetString());

//	VR_LOGD("|VR_SDSParserHelp| |ParseResult| [POI Xml:%s]\n", rcResult.GetString());

    const TiXmlNode* pcXmlNode = cXmlDoc.RootElement();
    if (NULL == pcXmlNode) {
        return CL_FALSE;
    }

    const TiXmlElement* pcXmlElem = pcXmlNode->ToElement();
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    // RootElement should be "result"
    if (BL_String("result") != pcXmlElem->Value()) {
        return CL_FALSE;
    }

    return ParseResult(pcXmlElem);
}

CL_BOOL VR_SDSParserHelp::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    // FIXME: use dynamic array.
    const DWORD ITEM_COUNT = 20;
    m_pcInfo = new VR_MenuInfo[ITEM_COUNT];

    // Loop node "menu"
    const XCHAR* itemTag = "menu";
    DWORD dwIndex = 0;
    for (pcXmlElem = pcXmlElem->FirstChildElement(itemTag); NULL != pcXmlElem; pcXmlElem = pcXmlElem->NextSiblingElement(itemTag)) {
        if (!ParseData(pcXmlElem, dwIndex++)) {
            VR_LOGE("|VR_SDSParserHelp| |ParseResult| [Error Index:%lu]\n", dwIndex - 1);
            return CL_FALSE;
        }

        //FIXME:
        if (dwIndex>=ITEM_COUNT) break;
    }

    m_dwInfoNum = dwIndex;
    return CL_TRUE;
}

VR_MenuInfo* VR_SDSParserHelp::GetInfo(DWORD dwIndex) const
{
    return &m_pcInfo[dwIndex];
}

CL_BOOL VR_SDSParserHelp::ParseData(const TiXmlElement* pcXmlElem, DWORD dwIndex)
{
//    const TiXmlElement* pcXmlElemSub = pcXmlElem;
//    // icon
//    const TiXmlElement* pcItem = pcXmlElem->FirstChildElement("icon");
//    if (NULL != pcItem) {
//        m_pcInfo[dwIndex].cName = pcItem->GetText();
//    }

//    // name
//    pcItem = pcXmlElem->FirstChildElement("name");
//    if (NULL != pcItem) {
//        m_pcInfo[dwIndex].cName = pcItem->GetText();
//    }

//    // FIXME: use dynamic array.
//    const DWORD ITEM_COUNT = 4;
//    m_pcInfo[dwIndex].cMenuItems = new VR_MenuItemInfo[ITEM_COUNT];

//    const XCHAR* itemTag = "menuItem";
//    VR_MenuItemInfo* pMenuItemInfo = m_pcInfo[dwIndex].cMenuItems;
//    DWORD dwIndexSub = 0;
//    for (pcXmlElemSub = pcXmlElemSub->FirstChildElement(itemTag); NULL != pcXmlElemSub; pcXmlElemSub = pcXmlElemSub->NextSiblingElement(itemTag)) {
//        // sample
//        pcItem = pcXmlElemSub->FirstChildElement("sample");
//        if (NULL != pcItem) {
//            pMenuItemInfo[dwIndexSub].cSample = pcItem->GetText();
//        }
//        dwIndexSub++;

//        // FIXME:
//        if (dwIndexSub>=ITEM_COUNT) break;
//    }

//    m_pcInfo[dwIndex].cSize = dwIndexSub;
    return CL_TRUE;
}


// Report result
CL_BOOL VR_SDSParserRPT::ParseResult(const BL_String& rcResult)
{
    TiXmlDocument cXmlDoc;
    cXmlDoc.Parse(rcResult.GetString());

//	VR_LOGD("|VR_SDSParserRPT| |ParseRPTResult| [RPT Xml:%s]\n", rcResult.GetString());

    const TiXmlNode* pcXmlNode = cXmlDoc.RootElement();
    if (NULL == pcXmlNode) {
        return CL_FALSE;
    }
    const TiXmlElement* pcXmlElem = pcXmlNode->ToElement();
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }
    // RootElement should be "result"
    if (BL_String("result") != pcXmlElem->Value()) {
        return CL_FALSE;
    }

    INT iErcd = 1;
    if (TIXML_SUCCESS != pcXmlElem->QueryIntAttribute("code", reinterpret_cast<INT*>(&iErcd))) {
        return CL_FALSE;
    }

    if (0 != iErcd) {
        m_bHasError = CL_TRUE;
    }

    return CL_TRUE;
}

VR_SDSParserDLG::VR_SDSParserDLG()
    : m_dwDID(0)
{
    m_cOperationVector.assign(OPERATION_TYPE_MAX, NULL);
}

VR_SDSParserDLG::~VR_SDSParserDLG()
{
    Release();
}

VOID VR_SDSParserDLG::Release()
{
    for (WORD i = 0; i < m_cOperationVector.size(); ++i) {
        if (NULL != m_cOperationVector[i]) {
            delete m_cOperationVector[i];
        }
    }

    m_cOperationVector.clear();

    m_cOperationVector.assign(OPERATION_TYPE_MAX, NULL);

    m_dwDID = 0;
    m_cSID = "";

    m_bHasError = CL_FALSE;
}

CL_BOOL VR_SDSParserDLG::ParseResult(const string &rcResult)
{
    return ParseResult(BL_String(rcResult.c_str()));
}

CL_BOOL VR_SDSParserDLG::ParseResult(const BL_String& rcResult)
{
    VR_LOGD_FUNC();
    Release();

    TiXmlDocument cXmlDoc;
    cXmlDoc.Parse(rcResult.GetString());

    VR_LOGD("|VR_SDSParserDLG| |ParseResult| [%s]\n", rcResult.GetString());

    if (cXmlDoc.Error()) {
        VR_LOGE("|VR_SDSParserDLG| |ParseResult| [Error:%s, %s]\n", cXmlDoc.Value(), cXmlDoc.ErrorDesc());
        return CL_FALSE;
    }

    const TiXmlNode* pcXmlNode = cXmlDoc.RootElement();
    if (NULL == pcXmlNode) {
        return CL_FALSE;
    }
    const TiXmlElement* pcXmlElem = pcXmlNode->ToElement();
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    // RootElement should be "dialog-result"
    if (BL_String("dialog-result") != pcXmlElem->Value()) {
        return CL_FALSE;
    }

    // Check node "err"
    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("err");
    if (NULL != pcXmlElemTmp) {
        if (!ParseError(pcXmlElemTmp)) {
            VR_LOGE("|VR_SDSParserDLG| |ParseResult| [Error ParseError]\n");
            return CL_FALSE;
        }

        m_bHasError = CL_TRUE;
        m_cOperationVector.resize(1);
    }
    else {
        // Check node "did"
        pcXmlElemTmp = pcXmlElem->FirstChildElement("did");
        if (NULL == pcXmlElemTmp) {
            return CL_FALSE;
        }
        // Set DID
        m_dwDID = atoi(pcXmlElemTmp->GetText());
        // Check node "sid"
        pcXmlElemTmp = pcXmlElem->FirstChildElement("sid");
        if (NULL == pcXmlElemTmp) {
            return CL_FALSE;
        }
        m_cSID = pcXmlElemTmp->GetText();
        if (0 >= m_cSID.GetLength()) {
            VR_LOGE("|VR_SDSParserDLG| |ParseResult| [SID is empty:%s]\n", rcResult.GetString());
        }
        // Loop node "op"
        INT iOperationNum = 0;
        for (pcXmlElem = pcXmlElem->FirstChildElement("op"); NULL != pcXmlElem; pcXmlElem = pcXmlElem->NextSiblingElement("op")) {
            ++iOperationNum;
            VR_LOGD("Times: [%d]", iOperationNum);
            if (!ParseOperation(pcXmlElem)) {
                VR_LOGE("|VR_SDSParserDLG| |ParseResult| [Error ParseOperation Index:%d]\n", iOperationNum);
                continue;
                // return CL_FALSE;
            }
        }
        // Resize operation vector
        m_cOperationVector.resize(iOperationNum);
    }

    // Check operations
    if (!IsValidOperations()) {
        VR_LOGE("|VR_SDSParserDLG| |ParseResult| [Invalid Operations]\n");
        return CL_FALSE;
    }

    // clear the hints on the screen
//    if (IsAddEmptyHintsOperations()) {
//        VR_LOGD("|VR_SDSParserDLG| |ParseResult| |AddEmptyHintsOperations|\n");
//        AddEmptyHintsOperations();
//    }
    return CL_TRUE;
}

CL_BOOL VR_SDSParserDLG::ParseError(const TiXmlElement* pcXmlElem)
{
    VR_LOGD_FUNC();
    VR_SDSOperationParser* pcParser = new VR_SDSParserOperationError;

    if (NULL == pcParser) {
        VR_LOGE("|VR_SDSParserDLG| |ParseError| [NEW Fails]\n");
        return CL_FALSE;
    }

    if (!pcParser->ParseResult(pcXmlElem)) {
        VR_LOGE("|VR_SDSParserDLG| |ParseError|\n");
        return CL_FALSE;
    }

    m_cOperationVector[0] = pcParser;

    return CL_TRUE;
}

CL_BOOL VR_SDSParserDLG::ParseOperation(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem->Attribute("type")) {
        return CL_FALSE;
    }

    BL_String cType(pcXmlElem->Attribute("type"));
    BL_String cParam(pcXmlElem->Attribute("param"));
    BL_String cParam2(pcXmlElem->Attribute("param2"));

    VR_SDSOperationParser* pcParser = GetOperationParser(cType, cParam, cParam2);

    if (NULL == pcParser) {
        VR_LOGE("|VR_SDSParserDLG| |ParseOperation| [Type:%s Param:%s Param2:%s]\n", cType.GetString(), cParam.GetString(),cParam2.GetString());
        return CL_FALSE;
    }

    if (!pcParser->ParseResult(pcXmlElem)) {
        VR_LOGE("|VR_SDSParserDLG| |ParseOperation| [OperationType:%d]\n", pcParser->GetOperationType());
        INT iIndex = 0;
        pcXmlElem->QueryIntAttribute("index", reinterpret_cast<INT*>(&iIndex));
        m_cOperationVector[iIndex] = nullptr;
        VR_LOGE("index = %d, OperationVector = %p", iIndex, m_cOperationVector[iIndex]);
        return CL_FALSE;
    }

    INT iIndex = 0;
    if (TIXML_SUCCESS != pcXmlElem->QueryIntAttribute("index", reinterpret_cast<INT*>(&iIndex))) {
        return CL_FALSE;
    }

    if (iIndex >= OPERATION_TYPE_MAX) {
        VR_LOGE("|VR_SDSParserDLG| |ParseOperation| [Vector Overflow (All:%d Actual:%d)]\n", OPERATION_TYPE_MAX, iIndex);
        return CL_FALSE;
    }

    if (NULL != m_cOperationVector[iIndex]) {
        VR_LOGE("|VR_SDSParserDLG| |ParseOperation| [Vector(%d) is set already, Type(%d, %d)]\n", iIndex, m_cOperationVector[iIndex]->GetOperationType(), pcParser->GetOperationType());
        return CL_FALSE;
    }

    VR_LOGD("Push parser in position: [%d]", iIndex, pcParser->GetOperationType());
    m_cOperationVector[iIndex] = pcParser;

    return CL_TRUE;
}


// Return
// CL_TRUE: Add Empty Hints Operations
// CL_FALSE: not need add
CL_BOOL VR_SDSParserDLG::IsAddEmptyHintsOperations()
{
    CL_BOOL bIsVr = CL_FALSE;
    for (WORD i = 0; i < m_cOperationVector.size(); ++i) {
        switch (m_cOperationVector[i]->GetOperationType()) {
            case OPERATION_TYPE_VR:
                bIsVr = CL_TRUE;
                break;
            case OPERATION_TYPE_HINTS:
                return CL_FALSE;

            default:
                break;
        }
    }

    if ( !bIsVr) {
        return CL_FALSE;
    }

    return CL_TRUE;
}


VOID VR_SDSParserDLG::AddEmptyHintsOperations()
{
    BL_String cType("Hints");
    BL_String cParam("");
    BL_String cParam2("");

    VR_SDSOperationParser* pcParser = GetOperationParser(cType, cParam, cParam2);
    if (NULL == pcParser) {
        VR_LOGE("|VR_SDSParserDLG| |AddEmptyHintsOperations| error\n");
        return ;
    }

    // m_cOperationVector.resize(m_cOperationVector.size()+1);
    m_cOperationVector.push_back(pcParser);

    return ;
}

CL_BOOL VR_SDSParserDLG::IsValidOperations()
{
    VR_LOGD_FUNC();
    BYTE byResponseNum = 0;

    for (WORD i = 0; i < m_cOperationVector.size(); ++i) {
        if (m_cOperationVector[i] == nullptr) {
            VR_LOGD("continue, i = %d, is NULL", i);
            continue;
        }
        switch (m_cOperationVector[i]->GetOperationType()) {
            case OPERATION_TYPE_VR:
            case OPERATION_TYPE_PIC:
            case OPERATION_TYPE_COMMIT_RPT:
            case OPERATION_TYPE_COMMIT_WB:
            case OPERATION_TYPE_CHECK_WB:
                ++byResponseNum;
                break;

            default:
                break;
        }
    }

    return (byResponseNum <= 1) ? CL_TRUE : CL_FALSE;
}

VR_SDSOperationParser* VR_SDSParserDLG::GetOperationParser(
        const BL_String& rcOperationType, const BL_String& rcOperationParam,
        const BL_String& rcOperationParam2)
{
    VR_SDSOperationParser* pcParser = NULL;
    VR_LOGD("Operation type is: %s, Param is %s, Param2 is %s", rcOperationType.GetString(), rcOperationParam.GetString(), rcOperationParam2.GetString());

    if (BL_String("DisplayList") == rcOperationType) {
        if (BL_String("On") == rcOperationParam) {
            if (BL_String("POI") == rcOperationParam2) {
                pcParser = new VR_SDSParserOperationDisplayPOIListOn;
            } else if (BL_String("ContactPOI") == rcOperationParam2) {
                pcParser = new VR_SDSParserOperationDisplayPOIListOn;
            } else if (BL_String("Contact") == rcOperationParam2) {
                pcParser = new VR_SDSParserOperationDisplayContactListOn;
            } else if (BL_String("News") == rcOperationParam2) {
                pcParser = new VR_SDSParserOperationDisplayNewsListOn;
            } else if (BL_String("Help") == rcOperationParam2) {
                pcParser = new VR_SDSParserOperationDisplayHelpListOn;
            } else if (BL_String("Weather") == rcOperationParam2) {
                pcParser = new VR_SDSParserOperationDisplayWeatherInfoListOn;
            } else {
                VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [DisplayList On, but param2 is invalid ]\n");
            }
        }
        else if (BL_String("Off") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_DISPLAY_LIST_OFF);
        } else {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [DisplayList, but param is invalid ]\n");
        }
    }
    else if (BL_String("Vr") == rcOperationType) {
        pcParser = new VR_SDSOperationParser(OPERATION_TYPE_VR, rcOperationParam);
    }
    else if (BL_String("Picture") == rcOperationType) {
        pcParser = new VR_SDSOperationParser(OPERATION_TYPE_PIC);
    }
    else if (BL_String("Prompt") == rcOperationType) {
        pcParser = new VR_SDSParserOperationPrompt;
    }
    else if (BL_String("Hints") == rcOperationType) {
        VR_LOGD("|VR_SDSParserDLG| |GetOperationParser| Create Hints\n");
        pcParser = new VR_SDSParserOperationHints;
    }
    else if (BL_String("Check") == rcOperationType) {
        if (BL_String("Weibo") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_CHECK_WB);
        } else {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [Check, but param is invalid ]\n");
        }
    }
    else if (BL_String("Commit") == rcOperationType) {
        if (BL_String("Report") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationCommitRPT;
        }
        else if (BL_String("Weibo") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationCommitWB;
        } else {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [Commit, but param is invalid ]\n");
        }
    }
    else if (BL_String("Destination") == rcOperationType) {
        if (BL_String("Normal") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationDestinationNormal;
        } else {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [Destination, but param is invalid ]\n");
        }
    }
    else if (BL_String("MusicControl") == rcOperationType) {
        if (BL_String("Start") == rcOperationParam) {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [MusicControl Start]\n");
            pcParser = new VR_SDSParserOperationMusicStart;
        }
        else if (BL_String("Stop") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_MUSIC_STOP);
        }
        else if (BL_String("Pause") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_MUSIC_PAUSE);
        }
        else if (BL_String("NEXT") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_MUSIC_NEXT);
        }
        else if (BL_String("Previous") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_MUSIC_PREV);
        } else {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [MusicControl, but param is invalid ]\n");
        }
    }
    else if (BL_String("VolumeControl") == rcOperationType) {
        pcParser = new VR_SDSParserOperationVolume();
    }
    else if (BL_String("MapZoom") == rcOperationType) {
        if (BL_String("In") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationMapZoom(OPERATION_TYPE_MAP_ZOOM_IN);
        }
        else if (BL_String("Out") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationMapZoom(OPERATION_TYPE_MAP_ZOOM_OUT);
        }
        else {
            pcParser = new VR_SDSParserOperationMapZoom(OPERATION_TYPE_MAP_ZOOM);
        }
    }
    else if (BL_String("MapMove") == rcOperationType) {
        if (BL_String("Up") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationMapMove(OPERATION_TYPE_MAP_MOVE_UP);
        }
        else if (BL_String("Down") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationMapMove(OPERATION_TYPE_MAP_MOVE_DOWN);
        }
        else if (BL_String("Left") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationMapMove(OPERATION_TYPE_MAP_MOVE_LEFT);
        }
        else if (BL_String("Right") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationMapMove(OPERATION_TYPE_MAP_MOVE_RIGHT);
        }
        else if (BL_String("CurrentPos") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationMapMove(OPERATION_TYPE_MAP_MOVE_CURRENT_POS);
        } else {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [MapMove, but param is invalid ]\n");
        }
    }
    else if (BL_String("MapRotate") == rcOperationType) {
        if (BL_String("NorthUp") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_MAP_DIR_NORTH_UP);
        }
        else if (BL_String("HeadingUp") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_MAP_DIR_HEADING_UP);
        } else {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [MapRotate, but param is invalid ]\n");
        }
    }
    else if (BL_String("MapView") == rcOperationType) {
        if (BL_String("IR") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_MAP_VIEW_IR);
        }
        else if (BL_String("Sky") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_MAP_VIEW_SKY);
        }
        else if (BL_String("Normal") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_MAP_VIEW_NORMAL);
        }
        else if (BL_String("Change") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_MAP_VIEW_CHANGE);
        } else {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [MapView, but param is invalid ]\n");
        }
    }
    else if (BL_String("Call") == rcOperationType) {
        pcParser = new VR_SDSParserOperationCall();
    }
    else if (BL_String("Show") == rcOperationType) {
        if (BL_String("Position") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_SHOW_POS);
        }else if (BL_String("Route") == rcOperationParam) {
            pcParser = new VR_SDSOperationParser(OPERATION_TYPE_SHOW_ROUTE);
        } else {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [Show, but param is invalid ]\n");
        }

    }
    else if (BL_String("Message") == rcOperationType) {
        if (BL_String("Send") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationSendSMS;
        }
        else {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [SendMessage, but param is invalid ]\n");
        }
    }
    else if (BL_String("Logomark") == rcOperationType) {
        pcParser = new VR_SDSParserOperationLogomark;
    }
    else if (BL_String("AppControl") == rcOperationType) {
        if (BL_String("Start") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationAppControl;
        }
        else {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [AppControl, but param is invalid ]\n");
        }
    }
    else if (BL_String("RadioControl") == rcOperationType) {
        if (BL_String("Start") == rcOperationParam) {
            pcParser = new VR_SDSParserOperationRadioControl;
        }
        else {
            VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [RadioControl, but param is invalid ]\n");
        }
    }

    else if (BL_String("Stop") == rcOperationType) {
        pcParser = new VR_SDSOperationParser(OPERATION_TYPE_STOP);
    } else {
        VR_LOGE("|VR_SDSParserDLG| |GetOperationParser| [the op type is invalid ]\n");
    }

    return pcParser;
}

VR_SDSParserOperationPrompt::VR_SDSParserOperationPrompt()
    : VR_SDSOperationParser(OPERATION_TYPE_PROMPT)
    , m_cTTS("")
    , m_cDisplay("")
    , m_uiDisplay("")
{

}

CL_BOOL VR_SDSParserOperationPrompt::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("prompt");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    m_cTTS = pcXmlElem->Attribute("tts");
    const char* display = pcXmlElem->Attribute("display");
    if (NULL == display) {
        m_cDisplay = m_cTTS;
    } else {
        m_cDisplay = display;
        m_uiDisplay = display;
    }

    return CL_TRUE;
}

VR_SDSParserOperationHints::VR_SDSParserOperationHints()
    : VR_SDSOperationParser(OPERATION_TYPE_HINTS)
{

}

CL_BOOL VR_SDSParserOperationHints::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("hints");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("content");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    m_cHintsContent = pcXmlElem->GetText();

    VR_LOGD("|VR_SDSParserOperationHints| |ParseResult| content=%s\n", m_cHintsContent.GetString());

    return CL_TRUE;
}

VR_SDSParserOperationCommitRPT::VR_SDSParserOperationCommitRPT()
    : VR_SDSOperationParser(OPERATION_TYPE_COMMIT_RPT)
{

}

CL_BOOL VR_SDSParserOperationCommitRPT::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("report");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }
    // Set "content"
    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("content");
    if (NULL == pcXmlElemTmp) {
        return CL_FALSE;
    }
    m_cContent = pcXmlElemTmp->GetText();
    // Set "picture"
    pcXmlElemTmp = pcXmlElem->FirstChildElement("picture");
    if (NULL == pcXmlElemTmp) {
        return CL_FALSE;
    }
    BL_String cPicture = pcXmlElemTmp->GetText();
    if (BL_String("Yes") == cPicture) {
        m_bPictureAttached = CL_TRUE;
    }
    else if (BL_String("No") == cPicture) {
        m_bPictureAttached = CL_FALSE;
    }
    else {
        return CL_FALSE;
    }

    return CL_TRUE;
}

VR_SDSParserOperationCommitWB::VR_SDSParserOperationCommitWB()
{
    SetOperationType(OPERATION_TYPE_COMMIT_WB);
}

// Show POI list
VR_SDSParserOperationDisplayPOIListOn::VR_SDSParserOperationDisplayPOIListOn()
    : VR_SDSOperationParser(OPERATION_TYPE_DISPLAY_POI_LIST_ON)
{

}

CL_BOOL VR_SDSParserOperationDisplayPOIListOn::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("result");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    return m_cDisplayPOIParser.ParseResult(pcXmlElem);
}


// Show Contact list
VR_SDSParserOperationDisplayContactListOn::VR_SDSParserOperationDisplayContactListOn()
    : VR_SDSOperationParser(OPERATION_TYPE_DISPLAY_CONTACT_LIST_ON)
{
}

CL_BOOL VR_SDSParserOperationDisplayContactListOn::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("result");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    return m_cParser.ParseResult(pcXmlElem);
}

// Show News list
VR_SDSParserOperationDisplayNewsListOn::VR_SDSParserOperationDisplayNewsListOn()
    : VR_SDSOperationParser(OPERATION_TYPE_DISPLAY_NEWS_LIST_ON)
{
}

CL_BOOL VR_SDSParserOperationDisplayNewsListOn::ParseResult(const TiXmlElement* pcXmlElem)
{
    VR_LOGD_FUNC();
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("result");
    if (NULL == pcXmlElem) {
        VR_LOGE("FirstChildElement result NULL ERROR");
        return CL_FALSE;
    }

    return m_cParser.ParseResult(pcXmlElem);
}


// Show Help list
VR_SDSParserOperationDisplayHelpListOn::VR_SDSParserOperationDisplayHelpListOn()
    : VR_SDSOperationParser(OPERATION_TYPE_DISPLAY_HELP_LIST_ON)
{
}

CL_BOOL VR_SDSParserOperationDisplayHelpListOn::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("result");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    return m_cParser.ParseResult(pcXmlElem);
}


VR_SDSParserOperationDestinationNormal::VR_SDSParserOperationDestinationNormal()
    : VR_SDSOperationParser(OPERATION_TYPE_DESTINATION_NORMAL)
{

}

CL_BOOL VR_SDSParserOperationDestinationNormal::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("navigation");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("d");
    if (NULL == pcXmlElemTmp) {
        return CL_FALSE;
    }

    pcXmlElemTmp = pcXmlElemTmp->FirstChildElement("result");
    if (NULL == pcXmlElemTmp) {
        return CL_FALSE;
    }

    if (!m_cDestinationParser.ParseResult(pcXmlElemTmp)) {
        return CL_FALSE;
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("p");
    if (NULL != pcXmlElemTmp) {
        pcXmlElemTmp = pcXmlElemTmp->FirstChildElement("result");

        if (NULL == pcXmlElemTmp) {
            return CL_FALSE;
        }

        return m_cWayPointParser.ParseResult(pcXmlElemTmp);
    }

    return CL_TRUE;
}

VR_SDSParserOperationMusicStart::VR_SDSParserOperationMusicStart()
    : VR_SDSOperationParser(OPERATION_TYPE_MUSIC_START),
      m_bPlayInfo(CL_FALSE),
      m_strPlayList(XTEXT("")),
      m_strArtist(XTEXT("")),
      m_strSong(XTEXT("")),
      m_strAlbum(XTEXT("")),
      m_strGenre(XTEXT(""))
{

}

CL_BOOL VR_SDSParserOperationMusicStart::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("playInfo");
    if (NULL == pcXmlElem) {
        m_bPlayInfo = CL_FALSE;
        return CL_TRUE;
    }
    m_bPlayInfo = CL_TRUE;

    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("playList");
    if (NULL != pcXmlElemTmp) {
        m_strPlayList = pcXmlElemTmp->GetText();
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("artist");
    if (NULL != pcXmlElemTmp) {
        m_strArtist= pcXmlElemTmp->GetText();
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("song");
    if (NULL != pcXmlElemTmp) {
        m_strSong= pcXmlElemTmp->GetText();
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("album");
    if (NULL != pcXmlElemTmp) {
        m_strAlbum= pcXmlElemTmp->GetText();
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("genre");
    if (NULL != pcXmlElemTmp) {
        m_strGenre= pcXmlElemTmp->GetText();
    }

    return CL_TRUE;
}

VR_SDSParserOperationVolume::VR_SDSParserOperationVolume()
    : VR_SDSOperationParser(OPERATION_TYPE_VOLUME),
      m_bVolumeInfo(CL_FALSE),
      m_dwValue(VR_SDS_DEFAULT_VOLUME),
      m_eType(VR_VOLUME_TYPE_DEFAULT),
      m_eOperation(VR_VOLUME_OP_UP)
{
    m_pfVolumeTypeMap[VR_VOLUME_TYPE_DEFAULT] = BL_String(XTEXT("Default"));
    m_pfVolumeTypeMap[VR_VOLUME_TYPE_SYSTEM] = BL_String(XTEXT("Sys"));
    m_pfVolumeTypeMap[VR_VOLUME_TYPE_MUSIC] = BL_String(XTEXT("Music"));
    m_pfVolumeTypeMap[VR_VOLUME_TYPE_NAVI] = BL_String(XTEXT("Navi"));

    m_pfVolumeOperationMap[VR_VOLUME_OP_UP] = BL_String(XTEXT("Up"));
    m_pfVolumeOperationMap[VR_VOLUME_OP_DOWN] = BL_String(XTEXT("Down"));
    m_pfVolumeOperationMap[VR_VOLUME_OP_ON] = BL_String(XTEXT("On"));
    m_pfVolumeOperationMap[VR_VOLUME_OP_OFF] = BL_String(XTEXT("Off"));
}

CL_BOOL VR_SDSParserOperationVolume::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("volume");
    if (NULL == pcXmlElem) {
        m_bVolumeInfo = CL_FALSE;
        return CL_TRUE;
    }
    m_bVolumeInfo = CL_TRUE;

    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("operation");
    if (NULL != pcXmlElemTmp) {
        for (SDSParamIt it = m_pfVolumeOperationMap.begin(); it != m_pfVolumeOperationMap.end(); ++it) {
            if (BL_String(pcXmlElemTmp->GetText()) == it->second) {
                m_eOperation = static_cast<VR_VOLUME_OP>(it->first);
                break;
            }
        }
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("value");
    if (NULL != pcXmlElemTmp) {
        m_dwValue = atoi(pcXmlElemTmp->GetText());
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("type");
    if (NULL != pcXmlElemTmp) {
        for (SDSParamIt it = m_pfVolumeTypeMap.begin(); it != m_pfVolumeTypeMap.end(); ++it) {
            if (BL_String(pcXmlElemTmp->GetText()) == it->second) {
                m_eType = static_cast<VR_VOLUME_TYPE>(it->first);
                break;
            }
        }
    }

    return CL_TRUE;
}

VR_SDSParserOperationMapMove::VR_SDSParserOperationMapMove(VR_SDSSERVER_OPERATION_TYPE eType)
    : VR_SDSOperationParser(eType),
      m_dwDistance(0),
      m_eScreenSize(VR_MAP_SCREEN_HALF)
{

}

CL_BOOL VR_SDSParserOperationMapMove::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    BL_String strMoveParam = pcXmlElem->Attribute("param2", reinterpret_cast<int*>(&m_dwDistance));
    if (0 == m_dwDistance) {
        if ((BL_String("Half") == strMoveParam)
            || (BL_String("") == strMoveParam)) {
            m_eScreenSize= VR_MAP_SCREEN_HALF;
        }
        else if (BL_String("Full") == strMoveParam) {
            m_eScreenSize = VR_MAP_SCREEN_FULL;
        }
        else {
            VR_LOGE("|VR_SDSParserOperationMapMove| |ParseResult| param2: %s is invalid", strMoveParam.GetString());
            return CL_FALSE;
        }
    }

    return CL_TRUE;
}

VR_SDSParserOperationMapZoom::VR_SDSParserOperationMapZoom(VR_SDSSERVER_OPERATION_TYPE eType)
    : VR_SDSOperationParser(eType),
      m_dwZoomTimes(VR_SDS_DEFAULT_ZOOM_TIMES),
      m_eZoomLevel(VR_MAP_ZOOM_LEVEL_MAX)
{

}

CL_BOOL VR_SDSParserOperationMapZoom::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    if (OPERATION_TYPE_MAP_ZOOM == m_operationType) {
        pcXmlElem->Attribute("param", reinterpret_cast<int*>(&m_eZoomLevel));
        if ((VR_MAP_ZOOM_LEVEL_ONE > m_eZoomLevel) ||
            (VR_MAP_ZOOM_LEVEL_MAX <= m_eZoomLevel)) {
            VR_LOGE("|VR_SDSParserOperationMapZoom| |ParseResult| m_eZoomLevel: %d is invalid", m_eZoomLevel);
            return CL_FALSE;
        }
    }
    else if ((OPERATION_TYPE_MAP_ZOOM_IN == m_operationType) ||
               (OPERATION_TYPE_MAP_ZOOM_OUT == m_operationType)) {
        pcXmlElem->Attribute("param2", reinterpret_cast<int*>(&m_dwZoomTimes));
    }
    else {
        VR_LOGE("|VR_SDSParserOperationMapZoom| |ParseResult| m_operationType: %d is invalid", m_operationType);
        return CL_FALSE;
    }

    return CL_TRUE;
}

VR_SDSParserOperationCall::VR_SDSParserOperationCall()
    : VR_SDSOperationParser(OPERATION_TYPE_CALL),
      m_strName(XTEXT("")),
      m_strPhone(XTEXT(""))
{

}

CL_BOOL VR_SDSParserOperationCall::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("call");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("phone");
    if (NULL != pcXmlElemTmp) {
        m_strPhone= pcXmlElemTmp->GetText();
    } else {
        return CL_FALSE;
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("name");
    if (NULL != pcXmlElemTmp) {
        m_strName = pcXmlElemTmp->GetText();
    }

    return CL_TRUE;
}

VR_SDSParserOperationSendSMS::VR_SDSParserOperationSendSMS()
    : VR_SDSOperationParser(OPERATION_TYPE_SEND_MESSAGE)
{

}

CL_BOOL VR_SDSParserOperationSendSMS::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("message");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("content");
    if (NULL != pcXmlElemTmp) {
        m_strContent = pcXmlElemTmp->GetText();
    } else {
        return CL_FALSE;
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("name");
    if (NULL != pcXmlElemTmp) {
        m_strName = pcXmlElemTmp->GetText();
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("phone");
    if (NULL != pcXmlElemTmp) {
        m_strPhone = pcXmlElemTmp->GetText();
    }

    return CL_TRUE;
}

VR_SDSParserOperationLogomark::VR_SDSParserOperationLogomark()
    : VR_SDSOperationParser(OPERATION_TYPE_LOGOMARK)
{

}

CL_BOOL VR_SDSParserOperationLogomark::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("logomark");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("id");
    if (NULL != pcXmlElemTmp) {
        m_strID= pcXmlElemTmp->GetText();
    } else {
        return CL_FALSE;
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("name");
    if (NULL != pcXmlElemTmp) {
        m_strName = pcXmlElemTmp->GetText();
    }

    return CL_TRUE;
}

VR_SDSParserOperationAppControl::VR_SDSParserOperationAppControl()
    : VR_SDSOperationParser(OPERATION_TYPE_APP_CONTROL)
{

}

CL_BOOL VR_SDSParserOperationAppControl::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("appName");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    m_cAppName = pcXmlElem->GetText();

    return CL_TRUE;
}

VR_SDSParserOperationRadioControl::VR_SDSParserOperationRadioControl()
    : VR_SDSOperationParser(OPERATION_TYPE_RADIO_CONTROL)
{

}

CL_BOOL VR_SDSParserOperationRadioControl::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    const TiXmlElement* pcXmlElemTmp =  pcXmlElem->FirstChildElement("appName");
    if (NULL == pcXmlElemTmp) {
        return CL_FALSE;
    }
    m_cAppName = pcXmlElemTmp->GetText();

    pcXmlElem =  pcXmlElem->FirstChildElement("params");
    if (NULL != pcXmlElem) {
        pcXmlElemTmp = pcXmlElem->FirstChildElement("type");
        if (NULL != pcXmlElemTmp) {
            m_cRadioType = pcXmlElemTmp->GetText();

            if (m_cRadioType.Find("FM") > 0) {
                m_cRadioType.Set("FM");
            }
            else if (m_cRadioType.Find("AM") > 0) {
                m_cRadioType.Set("AM");
            }
        }

        pcXmlElemTmp = pcXmlElem->FirstChildElement("freq");
        if (NULL != pcXmlElemTmp) {
            BL_String cFreq = pcXmlElemTmp->GetText();
            ConvertCNtofr(cFreq);
            m_cRadioFreq = cFreq;
        }

        pcXmlElemTmp = pcXmlElem->FirstChildElement("title");
        if (NULL != pcXmlElemTmp) {
            m_cRadioTitle = pcXmlElemTmp->GetText();
        }
    }

    return CL_TRUE;
}

VOID VR_SDSParserOperationRadioControl::ConvertCNtofr(BL_String &cRadiofr) const
{
    if ("" == cRadiofr) {
        return;
    }

    const static BL_String s_cSrcArab[] = {BL_String("点"), BL_String("一"), BL_String("二"), BL_String("三"), BL_String("四"), BL_String("五"),
                                    BL_String("六"), BL_String("七"), BL_String("八"), BL_String("九"), BL_String("十")};

    const static BL_String s_cDesArab[] = {BL_String("."), BL_String("1"), BL_String("2"), BL_String("3"), BL_String("4"), BL_String("5"),
                                    BL_String("6"), BL_String("7"), BL_String("8"), BL_String("9"), BL_String("10")};

    DWORD dwIndex = 0;
    for (; dwIndex < sizeof(s_cSrcArab)/sizeof(s_cSrcArab[0]); ++dwIndex) {
        cRadiofr.Replace(s_cSrcArab[dwIndex].GetString(), s_cDesArab[dwIndex].GetString());
    }

    return;
}

VR_SDSParserOperationError::VR_SDSParserOperationError()
    : VR_SDSOperationParser(OPERATION_TYPE_ERROR)
    , m_eErcd(VR_SDSSERVER_ERROR_NO_ERROR)
{
}

CL_BOOL VR_SDSParserOperationError::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("code");
    if (NULL == pcXmlElemTmp) {
        return CL_FALSE;
    }
    m_eErcd = static_cast<VR_SDSSERVER_ERROR>(atoi(pcXmlElemTmp->GetText()));

    pcXmlElemTmp = pcXmlElem->FirstChildElement("info");
    if (NULL == pcXmlElemTmp) {
        return CL_FALSE;
    }
    m_cInfo = pcXmlElemTmp->GetText();

    return CL_TRUE;
}

VR_SDSParserCommunication::VR_SDSParserCommunication()
    : m_dwInfoNum(0), m_pcInfo(NULL)
{

}

VR_SDSParserCommunication::~VR_SDSParserCommunication()
{
    Release();
}

VOID
VR_SDSParserCommunication::Release()
{
    if (NULL != m_pcInfo) {
        delete[] m_pcInfo;
        m_pcInfo = NULL;
    }

    m_dwInfoNum = 0;

    VR_LOGD("|VR_SDSParserCommunication| |Release|\n");
}

CL_BOOL
VR_SDSParserCommunication::ParseResult(const BL_String& rcResult)
{
    TiXmlDocument cXmlDoc;
    cXmlDoc.Parse(rcResult.GetString());

//	VR_LOGD("|VR_SDSParserCommunication| |ParseResult| [POI Xml:%s]\n", rcResult.GetString());

    const TiXmlNode* pcXmlNode = cXmlDoc.RootElement();
    if (NULL == pcXmlNode) {
        return CL_FALSE;
    }
    const TiXmlElement* pcXmlElem = pcXmlNode->ToElement();
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }
    // RootElement should be "communication"
    if (BL_String("communication") != pcXmlElem->Value()) {
        return CL_FALSE;
    }

    return ParseResult(pcXmlElem);
}

CL_BOOL
VR_SDSParserCommunication::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }
    // Check node "exception"
    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("exception");
    if (NULL != pcXmlElemTmp) {
        ParseException(pcXmlElemTmp);

        m_bHasError = CL_TRUE;
        return CL_TRUE;
    }
    // Check node "info"
    pcXmlElemTmp = pcXmlElem->FirstChildElement("info");
    if (NULL == pcXmlElemTmp) {
        return CL_FALSE;
    }
    if (!ParseInfo(pcXmlElemTmp)) {
        return CL_FALSE;
    }
    // Loop node "data"
    DWORD dwIndex = 0;
    for (pcXmlElem = pcXmlElem->FirstChildElement("data"); NULL != pcXmlElem; pcXmlElem = pcXmlElem->NextSiblingElement("data")) {
        if (!ParseData(pcXmlElem, dwIndex++)) {
            VR_LOGE("|VR_SDSParserCommunication| |ParseResult| [Error Index:%lu]\n", dwIndex - 1);
            return CL_FALSE;
        }
    }
    // Size check
    if (dwIndex != m_dwInfoNum) {
        return CL_FALSE;
    }

    return CL_TRUE;
}

CL_BOOL
VR_SDSParserCommunication::ParseException(const TiXmlElement* pcXmlElem)
{
    BL_String cErrorCode;
    BL_String cErrorMsg;

    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("errorcode");
    if (NULL != pcXmlElemTmp) {
        cErrorCode = pcXmlElemTmp->GetText();
    }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("errormsg");
    if (NULL != pcXmlElemTmp) {
        cErrorMsg = pcXmlElemTmp->GetText();
    }

    VR_LOGE("|VR_SDSParserCommunication| |ParseException| Error[ErrorCode:%s ErrorMsg:%s]\n", cErrorCode.GetString(), cErrorMsg.GetString());

    return CL_TRUE;
}

CL_BOOL
VR_SDSParserCommunication::ParseInfo(const TiXmlElement* pcXmlElem)
{
    BL_String cCount;

    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("count");
    if (NULL == pcXmlElemTmp) {
        VR_LOGE("|VR_SDSParserCommunication| |ParseInfo| Error[Count]\n");
        return CL_FALSE;
    }

    cCount = pcXmlElemTmp->GetText();

    m_dwInfoNum = atoi(cCount.GetString());

    if (NULL != m_pcInfo) {
        delete[] m_pcInfo;
        m_pcInfo = NULL;
    }

    if (m_dwInfoNum > 0) {
        m_pcInfo = new VR_ContactInfo[m_dwInfoNum];
    }

    VR_LOGD("|VR_SDSParserCommunication| |ParseInfo| [InfoNum:%lu Info:%p]\n", m_dwInfoNum, m_pcInfo);

    return CL_TRUE;
}

CL_BOOL
VR_SDSParserCommunication::ParseData(const TiXmlElement* pcXmlElem, DWORD dwIndex)
{
//    if (dwIndex >= m_dwInfoNum) {
//        VR_LOGE("|VR_SDSParserCommunication| |ParseData| Error[Invalid Data Count]\n");
//        return CL_FALSE;
//    }
//    // name
//    const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("name");
//    if (NULL != pcXmlElemTmp) {
//        m_pcInfo[dwIndex].cName = pcXmlElemTmp->GetText();
//    }

//    // phone
//    pcXmlElemTmp = pcXmlElem->FirstChildElement("phone");
//    if (NULL != pcXmlElemTmp) {
//        //FIXME:
//        //m_pcInfo[dwIndex].cPhone= pcXmlElemTmp->GetText();
//    }

    return CL_TRUE;
}



VR_SDSParserOperationDisplayComListOn::VR_SDSParserOperationDisplayComListOn()
    : VR_SDSOperationParser(OPERATION_TYPE_DISPLAY_COM_LIST_ON)
{

}

CL_BOOL
VR_SDSParserOperationDisplayComListOn::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("communication");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    return m_cDisplayComParser.ParseResult(pcXmlElem);
}


VR_SDSParserOperationDisplayWeatherInfoListOn::VR_SDSParserOperationDisplayWeatherInfoListOn()
    : VR_SDSOperationParser(OPERATION_TYPE_DISPLAY_WEATHERINFO_LIST_ON)
{

}

CL_BOOL VR_SDSParserOperationDisplayWeatherInfoListOn::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    pcXmlElem = pcXmlElem->FirstChildElement("weatherOfCities");
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }

    return m_cDisplayWeatherParser.ParseResult(pcXmlElem);

    // return CL_TRUE;

}

VR_SDSParserWeatherInfo::VR_SDSParserWeatherInfo()
    : m_dwWeatherForecastNum(0),
      m_pWeatherInfoToday(NULL), 
      m_pWeatherInfoForecast(NULL),
      m_pWeatherBasicInfo(NULL)
{

}

VR_SDSParserWeatherInfo::~VR_SDSParserWeatherInfo()
{
    Release();
}

VOID VR_SDSParserWeatherInfo::Release()
{
    if (NULL != m_pWeatherInfoForecast) {
        delete[] m_pWeatherInfoForecast;
        m_pWeatherInfoForecast = NULL;
    }

    if (NULL != m_pWeatherInfoToday) {
        delete m_pWeatherInfoToday;
        m_pWeatherInfoToday = NULL;
    }

    if (NULL != m_pWeatherBasicInfo) {
        delete m_pWeatherBasicInfo;
        m_pWeatherBasicInfo = NULL;
    }

    m_dwWeatherForecastNum = 0;

    VR_LOGD("|VR_SDSParserWeatherInfo| |Release|\n");
}

CL_BOOL VR_SDSParserWeatherInfo::ParseResult(const BL_String& rcResult)
{
    TiXmlDocument cXmlDoc;
    cXmlDoc.Parse(rcResult.GetString());

    const TiXmlNode* pcXmlNode = cXmlDoc.RootElement();
    if (NULL == pcXmlNode) {
        return CL_FALSE;
    }
    const TiXmlElement* pcXmlElem = pcXmlNode->ToElement();
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }
    // RootElement should be "result"
    if (BL_String("weatherOfCities") != pcXmlElem->Value()) {
        return CL_FALSE;
    }

    return ParseResult(pcXmlElem);
}

CL_BOOL VR_SDSParserWeatherInfo::ParseResult(const TiXmlElement* pcXmlElem)
{
    if (NULL == pcXmlElem) {
        return CL_FALSE;
    }
    // Check node "exception"
    // const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("exception");
    //if (NULL != pcXmlElemTmp) {
    //    ParseException(pcXmlElemTmp);

    //    m_bHasError = CL_TRUE;
    //    return CL_TRUE;
    //}

    // Check node "info"
    //pcXmlElemTmp = pcXmlElem->FirstChildElement("info");
    // if (NULL == pcXmlElemTmp) {
    //    return CL_FALSE;
    // }
    // if (!ParseInfo(pcXmlElemTmp)) {
    //    return CL_FALSE;
    // }
    // Loop node "data"

    if (NULL != m_pWeatherBasicInfo) {
        delete m_pWeatherBasicInfo;
        m_pWeatherBasicInfo = NULL;
    }

    if (NULL != m_pWeatherInfoToday) {
        delete m_pWeatherInfoToday;
        m_pWeatherInfoToday = NULL;
    }

    if (NULL != m_pWeatherInfoForecast) {
        delete[] m_pWeatherInfoForecast;
        m_pWeatherInfoForecast = NULL;
    }

    m_pWeatherBasicInfo = new VR_WeatherBasicInfo();

    const TiXmlElement* pcXmlElemBasicInfo = pcXmlElem->FirstChildElement("basicInfo");
    if (NULL != pcXmlElemBasicInfo) {
        ParseWeatherBaiciInfoData(pcXmlElemBasicInfo);
    }

    m_pWeatherInfoToday = new VR_WeatherInfoToday();

    const TiXmlElement* pcXmlElemTodayInfo = pcXmlElem->FirstChildElement("weather_of_today");
    if (NULL != pcXmlElemTodayInfo) {
        ParseWeatherTodayData(pcXmlElemTodayInfo);
    }

    const DWORD FORECAST_COUNT = 5;

    m_pWeatherInfoForecast = new VR_WeatherInfoForecast[FORECAST_COUNT];

    DWORD dwIndex = 0;
    for (pcXmlElem = pcXmlElem->FirstChildElement("weather_of_forecast"); NULL != pcXmlElem; pcXmlElem = pcXmlElem->NextSiblingElement("weather_of_forecast")) {
        if (!ParseWeatherForecastData(pcXmlElem, dwIndex++)) {
            VR_LOGE("|VR_SDSParserWeatherInfo| |ParseResult| [Error Index:%lu]\n", dwIndex - 1);
            return CL_FALSE;
        }

        if (dwIndex >= FORECAST_COUNT) break;
    }

    m_dwWeatherForecastNum = dwIndex;

    // Size check
    // if (dwIndex != m_dwInfoNum) {
    //     return CL_FALSE;
    // }

    return CL_TRUE;
}


VR_WeatherInfoForecast* VR_SDSParserWeatherInfo::GetWeatherForecastInfo(DWORD dwIndex) const
{
    return &m_pWeatherInfoForecast[dwIndex];
}

CL_BOOL VR_SDSParserWeatherInfo::ParseWeatherForecastData(const TiXmlElement* pcXmlElem, DWORD dwIndex)
{
   // if (dwIndex >= m_dwInfoNum) {
   //   VR_LOGE("|VR_WeatherInfoForecast| |ParseData| Error[Invalid Data Count]\n");
   //    return CL_FALSE;
   // }

   // date
   const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("date");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherInfoForecast[dwIndex].date = pcXmlElemTmp->GetText();
   }

   // descOfDayWeather
   pcXmlElemTmp = pcXmlElem->FirstChildElement("descOfDayWeather");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherInfoForecast[dwIndex].descOfDayWeather= pcXmlElemTmp->GetText();
   }

   // maxTemperature
   pcXmlElemTmp = pcXmlElem->FirstChildElement("maxTemperature");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherInfoForecast[dwIndex].maxTemperature= pcXmlElemTmp->GetText();
   }

   // minTemperature
   pcXmlElemTmp = pcXmlElem->FirstChildElement("minTemperature");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherInfoForecast[dwIndex].minTemperature= pcXmlElemTmp->GetText();
   }

    return CL_TRUE;

}

bool VR_SDSParserWeatherInfo::ParseWeatherBaiciInfoData(const TiXmlElement* pcXmlElem)
{

   const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("adminArea");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherBasicInfo->adminArea = pcXmlElemTmp->GetText();
   }

   pcXmlElemTmp = pcXmlElem->FirstChildElement("cid");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherBasicInfo->cid = pcXmlElemTmp->GetText();
   }

   pcXmlElemTmp = pcXmlElem->FirstChildElement("country");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherBasicInfo->country= pcXmlElemTmp->GetText();
   }


   pcXmlElemTmp = pcXmlElem->FirstChildElement("latitude");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherBasicInfo->latitude = pcXmlElemTmp->GetText();
   }

    pcXmlElemTmp = pcXmlElem->FirstChildElement("location");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherBasicInfo->location= pcXmlElemTmp->GetText();
   }

   pcXmlElemTmp = pcXmlElem->FirstChildElement("longitude");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherBasicInfo->longitude = pcXmlElemTmp->GetText();
   }

   pcXmlElemTmp = pcXmlElem->FirstChildElement("parentCity");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherBasicInfo->parentCity = pcXmlElemTmp->GetText();
   }
    return CL_TRUE;
}

bool VR_SDSParserWeatherInfo::ParseWeatherTodayData(const TiXmlElement* pcXmlElem)
{

   const TiXmlElement* pcXmlElemTmp = pcXmlElem->FirstChildElement("temperature");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherInfoToday->temperature = pcXmlElemTmp->GetText();
   }

   pcXmlElemTmp = pcXmlElem->FirstChildElement("fl");
   if (NULL != pcXmlElemTmp) {
       m_pWeatherInfoToday->fl = pcXmlElemTmp->GetText();
   }

   return CL_TRUE;

}
