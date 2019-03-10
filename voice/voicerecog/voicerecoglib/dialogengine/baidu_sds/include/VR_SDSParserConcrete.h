#ifndef VR_SDSOPERATIONPARSERCONCRETE_H
#define VR_SDSOPERATIONPARSERCONCRETE_H

#include <map>
#include <vector>
#include <string>
#include "tinyxml.h"
#include "VR_SDSParserBase.h"
#include "VR_PlatformDEF.h"
#include "VR_SDSServerDEF.h"
#include "VR_POIInfo.h"
#include "VR_WeatherInfo.h"

typedef std::map<DWORD, BL_String> SDSParamMap;
typedef std::map<DWORD, BL_String>::iterator SDSParamIt;

// #define VR_POIInfo char
#define VR_ContactInfo char
#define VR_NewsInfo char
#define VR_MenuInfo char


// The parser for the xml return from SEARCH server
class VR_SDSParserPOI : public VR_SDSParserBase
{
public:
    VR_SDSParserPOI();
    VR_SDSParserPOI(const VR_SDSParserPOI& rcParser);
    virtual ~VR_SDSParserPOI();

    virtual bool ParseResult(const BL_String& rcResult);
    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    DWORD GetInfoNum() const { return m_dwInfoNum; }
    VR_POIInfo* GetInfo(DWORD dwIndex = 0) const;

    virtual VOID Release();

private:
    virtual bool ParseException(const TiXmlElement* pcXmlElem);
    virtual bool ParseInfo(const TiXmlElement* pcXmlElem);
    virtual bool ParseData(const TiXmlElement* pcXmlElem, DWORD dwIndex);

protected:
    DWORD m_dwInfoNum;
    VR_POIInfo* m_pcInfo;
};

// The parser for the xml return from  communication infos
class VR_SDSParserCommunication : public VR_SDSParserBase
{
public:
    VR_SDSParserCommunication();
    VR_SDSParserCommunication(const VR_SDSParserCommunication& rcParser);
    virtual ~VR_SDSParserCommunication();

    virtual bool ParseResult(const BL_String& rcResult);
    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    DWORD GetInfoNum() const { return m_dwInfoNum; }
    VR_ContactInfo* GetInfo(DWORD dwIndex = 0) const { return &m_pcInfo[dwIndex]; }

    virtual VOID Release();

private:
    virtual bool ParseException(const TiXmlElement* pcXmlElem);
    virtual bool ParseInfo(const TiXmlElement* pcXmlElem);
    virtual bool ParseData(const TiXmlElement* pcXmlElem, DWORD dwIndex);

protected:
    DWORD m_dwInfoNum;
    VR_ContactInfo* m_pcInfo;
};


// The xml parser of Contact
class VR_SDSParserContact : public VR_SDSParserBase
{
public:
    VR_SDSParserContact();
    VR_SDSParserContact(const VR_SDSParserContact& rcParser);
    virtual ~VR_SDSParserContact();

    virtual bool ParseResult(const BL_String& rcResult);
    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    DWORD GetInfoNum() const { return m_dwInfoNum; }
    VR_ContactInfo* GetInfo(DWORD dwIndex = 0) const;

    virtual VOID Release();

private:
    virtual bool ParseData(const TiXmlElement* pcXmlElem, DWORD dwIndex);

protected:
    DWORD m_dwInfoNum;
    VR_ContactInfo* m_pcInfo;
};

// The xml parser of News list
class VR_SDSParserNews : public VR_SDSParserBase
{
public:
    VR_SDSParserNews();
    VR_SDSParserNews(const VR_SDSParserNews& rcParser);
    virtual ~VR_SDSParserNews();

    virtual bool ParseResult(const BL_String& rcResult);
    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    DWORD GetInfoNum() const { return m_dwInfoNum; }
    VR_NewsInfo* GetInfo(DWORD dwIndex = 0) const;

    virtual VOID Release();

private:
    virtual bool ParseData(const TiXmlElement* pcXmlElem, DWORD dwIndex);

protected:
    DWORD m_dwInfoNum;
    VR_NewsInfo* m_pcInfo;
};

// The xml parser of Help list
class VR_SDSParserHelp : public VR_SDSParserBase
{
public:
    VR_SDSParserHelp();
    VR_SDSParserHelp(const VR_SDSParserHelp& rcParser);
    virtual ~VR_SDSParserHelp();

    virtual bool ParseResult(const BL_String& rcResult);
    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    DWORD GetInfoNum() const { return m_dwInfoNum; }
    VR_MenuInfo* GetInfo(DWORD dwIndex = 0) const;

    virtual VOID Release();

private:
    virtual bool ParseData(const TiXmlElement* pcXmlElem, DWORD dwIndex);

protected:
    DWORD m_dwInfoNum;
    VR_MenuInfo* m_pcInfo;
};

// The parser for the xml return from SNS server
class VR_SDSParserRPT : public VR_SDSParserBase
{
public:
    virtual ~VR_SDSParserRPT() {}

    virtual bool ParseResult(const BL_String& rcResult);
};

class VR_SDSParserDLG : public VR_SDSParserBase
{
public:
    typedef std::vector<VR_SDSOperationParser*>::iterator OperationIt;
    typedef std::vector<VR_SDSOperationParser*> OperationVector;

public:
    VR_SDSParserDLG();
    virtual ~VR_SDSParserDLG();

    virtual bool ParseResult(const BL_String& rcResult);
    virtual bool ParseResult(const std::string& rcResult);

    const BL_String& GetSID() const { return m_cSID; }
    DWORD GetDID() const { return m_dwDID; }
    OperationVector GetOperationVector() { return m_cOperationVector; }

private:
    VR_SDSOperationParser* GetOperationParser(const BL_String& rcType, const BL_String& rcParam, const BL_String& rcParam2);

    bool ParseError(const TiXmlElement* pcXmlElem);
    bool ParseOperation(const TiXmlElement* pcXmlElem);
    bool IsValidOperations();
    bool IsAddEmptyHintsOperations();
    VOID AddEmptyHintsOperations();

    VOID Release();

private:
    DWORD m_dwDID;
    BL_String m_cSID;

    OperationVector m_cOperationVector;
};

class VR_SDSParserOperationPrompt : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationPrompt();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    const BL_String& GetTTS() const { return m_cTTS; }
    const BL_String& GetDisplay() const { return m_cDisplay; }
    const BL_String& GetUiDisplay() const { return m_uiDisplay; }

private:
    BL_String m_cTTS;
    BL_String m_cDisplay;
    BL_String m_uiDisplay;
};

class VR_SDSParserOperationHints : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationHints();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    const BL_String& GetHintsContent() const { return m_cHintsContent; }

private:
    BL_String m_cHintsContent;
};

class VR_SDSParserOperationCommitRPT : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationCommitRPT();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    const BL_String& GetContent() const { return m_cContent; }
    bool IsPictureAttached() const { return m_bPictureAttached; }

protected:
    VOID SetOperationType(VR_SDSSERVER_OPERATION_TYPE eType) { m_operationType = eType; }

private:
    BL_String m_cContent;
    bool m_bPictureAttached;
};

class VR_SDSParserOperationCommitWB : public VR_SDSParserOperationCommitRPT
{
public:
    VR_SDSParserOperationCommitWB();
};

class VR_SDSParserOperationDisplayPOIListOn : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationDisplayPOIListOn();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);
    VR_POIInfo* GetPOIInfo() const { return m_cDisplayPOIParser.GetInfo(); }

    DWORD GetPOIInfoNum() const { return m_cDisplayPOIParser.GetInfoNum(); }


private:
    VR_SDSParserPOI m_cDisplayPOIParser;

};

class VR_SDSParserOperationDisplayComListOn : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationDisplayComListOn();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    VR_ContactInfo* GetComInfo() const { return m_cDisplayComParser.GetInfo(); }

    DWORD GetComInfoNum() const { return m_cDisplayComParser.GetInfoNum(); }

private:
    VR_SDSParserCommunication m_cDisplayComParser;
};

class VR_SDSParserOperationDisplayContactListOn : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationDisplayContactListOn();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);
    VR_ContactInfo* GetContactInfo() const { return m_cParser.GetInfo(); }

    DWORD GetContactInfoNum() const { return m_cParser.GetInfoNum(); }

private:
    VR_SDSParserContact m_cParser;
};


class VR_SDSParserOperationDisplayNewsListOn : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationDisplayNewsListOn();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);
    VR_NewsInfo* GetNewsInfo() const { return m_cParser.GetInfo(); }

    DWORD GetNewsInfoNum() const { return m_cParser.GetInfoNum(); }

private:
    VR_SDSParserNews m_cParser;
};

class VR_SDSParserOperationDisplayHelpListOn : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationDisplayHelpListOn();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);
    VR_MenuInfo* GetMenuInfo() const { return m_cParser.GetInfo(); }

    DWORD GetMenuInfoNum() const { return m_cParser.GetInfoNum(); }

private:
    VR_SDSParserHelp m_cParser;
};


class VR_SDSParserOperationDestinationNormal : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationDestinationNormal();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    VR_POIInfo* GetDestinationInfo() const { return m_cDestinationParser.GetInfo(); }
    VR_POIInfo* GetWayPointInfo() const { return m_cWayPointParser.GetInfo(); }

    DWORD GetDestinationInfoNum() const { return m_cDestinationParser.GetInfoNum(); }
    DWORD GetWayPointInfoNum() const { return m_cWayPointParser.GetInfoNum(); }

private:
    VR_SDSParserPOI m_cDestinationParser;
    VR_SDSParserPOI m_cWayPointParser;
};

class VR_SDSParserOperationMusicStart : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationMusicStart();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    BL_String GetPlayList() const { return m_strPlayList; }
    BL_String GetArtist() const { return m_strArtist; }
    BL_String GetSong() const { return m_strSong; }
    BL_String GetAlbum() const { return m_strAlbum; }
    BL_String GetGenre() const { return m_strGenre; }
    bool HasPlayInfo() const { return m_bPlayInfo; }

private:
    bool 		m_bPlayInfo;
    BL_String 		m_strPlayList;
    BL_String 		m_strArtist;
    BL_String 		m_strSong;
    BL_String 		m_strAlbum;
    BL_String 		m_strGenre;
};

class VR_SDSParserOperationVolume : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationVolume();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    DWORD GetValue() const { return m_dwValue; }
    VR_VOLUME_TYPE GetType() const { return m_eType; }
    VR_VOLUME_OP GetOperation() const { return m_eOperation; }
    bool HasVolumeInfo() const { return m_bVolumeInfo; }

private:
    bool 		m_bVolumeInfo;
    DWORD 			m_dwValue;
    VR_VOLUME_TYPE	m_eType;
    VR_VOLUME_OP	m_eOperation;
    SDSParamMap 	m_pfVolumeTypeMap;
    SDSParamMap 	m_pfVolumeOperationMap;
    enum {VR_SDS_DEFAULT_VOLUME = 1} ;
};

class VR_SDSParserOperationMapMove : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationMapMove(VR_SDSSERVER_OPERATION_TYPE eType);

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    DWORD GetDistance() const { return m_dwDistance; }
    VR_MAP_SCREEN_SIZE GetScreenSize() const { return m_eScreenSize; }

private:
    DWORD				m_dwDistance;
    VR_MAP_SCREEN_SIZE	m_eScreenSize;
};

class VR_SDSParserOperationMapZoom : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationMapZoom(VR_SDSSERVER_OPERATION_TYPE eType);

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    DWORD GetZoomTimes() const { return m_dwZoomTimes; }
    VR_MAP_ZOOM GetZoomLevel() const { return m_eZoomLevel; }

private:
    DWORD			m_dwZoomTimes;
    VR_MAP_ZOOM	m_eZoomLevel;
    enum {VR_SDS_DEFAULT_ZOOM_TIMES = 1} ;
};

class VR_SDSParserOperationCall : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationCall();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    BL_String GetName() const { return m_strName; }
    BL_String GetPhone() const { return m_strPhone; }

private:
    BL_String m_strName;
    BL_String m_strPhone;
};

class VR_SDSParserOperationSendSMS : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationSendSMS();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    BL_String GetName() const { return m_strName; }
    BL_String GetPhone() const { return m_strPhone; }
    BL_String GetContent() const { return m_strContent; }

private:
    BL_String m_strName;
    BL_String m_strPhone;
    BL_String m_strContent;
};

class VR_SDSParserOperationLogomark : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationLogomark();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    BL_String GetName() const { return m_strName; }
    BL_String GetID() const { return m_strID; }

private:
    BL_String m_strName;
    BL_String m_strID;
};

class VR_SDSParserOperationAppControl : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationAppControl();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    const BL_String& GetAppNameContent() const { return m_cAppName; }

private:
    BL_String m_cAppName;
};

class VR_SDSParserOperationRadioControl : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationRadioControl();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    const BL_String& GetRadioType() const { return m_cRadioType; }
    const BL_String& GetRadioFreq() const { return m_cRadioFreq; }
    const BL_String& GetRadioTitle() const { return m_cRadioTitle; }
    const BL_String& GetAppNameContent() const { return m_cAppName; }

private:
    VOID ConvertCNtofr(BL_String &cRadiofr) const;

    BL_String m_cRadioType;
    BL_String m_cRadioFreq;
    BL_String m_cRadioTitle;
    BL_String m_cAppName;
};

class VR_SDSParserOperationError : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationError();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    VR_SDSSERVER_ERROR GetErrorCode() const { return m_eErcd;}
    const BL_String& GetErrorInfo() const { return m_cInfo; }

private:
    VR_SDSSERVER_ERROR m_eErcd;
    BL_String m_cInfo;
};


class VR_SDSParserWeatherInfo : public VR_SDSParserBase
{
public:
    VR_SDSParserWeatherInfo();
    VR_SDSParserWeatherInfo(const VR_SDSParserWeatherInfo& rcParser);
    virtual ~VR_SDSParserWeatherInfo();

    virtual bool ParseResult(const BL_String& rcResult);
    virtual bool ParseResult(const TiXmlElement* pcXmlElem);

    DWORD GetWeatherForecastNum() const { return m_dwWeatherForecastNum; }
    VR_WeatherInfoForecast* GetWeatherForecastInfo(DWORD dwIndex = 0) const;

    VR_WeatherBasicInfo* GetWeatherBasicInfo() const {return m_pWeatherBasicInfo;}
    VR_WeatherInfoToday* GetWeahterTodayInfo() const {return m_pWeatherInfoToday;}

    virtual VOID Release();

private:
    // virtual bool ParseException(const TiXmlElement* pcXmlElem);
    // virtual bool ParseWeatherForecastInfo(const TiXmlElement* pcXmlElem);
    virtual bool ParseWeatherForecastData(const TiXmlElement* pcXmlElem, DWORD dwIndex);
    virtual bool ParseWeatherBaiciInfoData(const TiXmlElement* pcXmlElem);
    virtual bool ParseWeatherTodayData(const TiXmlElement* pcXmlElem);

protected:
    // DWORD m_dwInfoNum;
    // VR_POIInfo* m_pcInfo;
    DWORD m_dwWeatherForecastNum;
    VR_WeatherInfoToday* m_pWeatherInfoToday;
    VR_WeatherInfoForecast* m_pWeatherInfoForecast;
    VR_WeatherBasicInfo* m_pWeatherBasicInfo;
};


class VR_SDSParserOperationDisplayWeatherInfoListOn : public VR_SDSOperationParser
{
public:
    VR_SDSParserOperationDisplayWeatherInfoListOn();

    virtual bool ParseResult(const TiXmlElement* pcXmlElem);
    
    VR_WeatherInfoForecast* GetWeatherForecastInfo() const { return m_cDisplayWeatherParser.GetWeatherForecastInfo(); }
    VR_WeatherBasicInfo* GetWeatherBasicInfo() const {return m_cDisplayWeatherParser.GetWeatherBasicInfo();}
    VR_WeatherInfoToday* GetWeahterTodayInfo() const {return m_cDisplayWeatherParser.GetWeahterTodayInfo();}

    DWORD GetWeatherForecastNum() const { return m_cDisplayWeatherParser.GetWeatherForecastNum(); }


private:
    VR_SDSParserWeatherInfo m_cDisplayWeatherParser;

};

#endif // VR_SDSOPERATIONPARSERCONCRETE_H
