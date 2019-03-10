/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include "VR_Log.h"
#include "VR_XmlStrParse.h"
#include "VR_VehicleInfoAgent.h"
#include "VR_VehicleInfoHelper.h"
#include "VR_VehicleInfoManager.h"

#include "EV_Define.h"
#include "EV_EventSender.h"
#include "VR_ContentProvider.h"

#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"
#include "navi/Voice/VoiceRecog/VrAirconditionStatus.pb.h"
#include "navi/Voice/VoiceRecog/VrVehicleInfo.pb.h"

using namespace nutshell;
using namespace navi::VoiceRecog;

VR_VehicleInfoManager::VR_VehicleInfoManager()
    : m_spVehiHelper(VR_new VR_VehicleInfoHelper)
    , m_spVehiAgent(VR_new VR_VehicleInfoAgent)
{
    VR_LOGD_FUNC();
    if (!m_spVehiHelper) {
        VR_LOGD("vehi helper nullptr");
    }
    if (!m_spVehiAgent) {
        VR_LOGD("vehi agent nullptr");
    }
}

VR_VehicleInfoManager::~VR_VehicleInfoManager()
{
    VR_LOGD_FUNC();
}

void VR_VehicleInfoManager::QueryAirconditionStatus()
{
    VR_LOGD_FUNC();
    m_spVehiAgent->GetPropertyAsync(VR_BASIC_INFO_ACTION_TYPE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BASIC_INFO_TEMPERATURE_UNIT.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BASIC_INFO_FRONT_MAX_BLOWER.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BASIC_INFO_REAR_MAX_BLOWER.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BASIC_INFO_TEMPERATURE_STEP.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BASIC_INFO_BLOWER_STEP.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BASIC_INFO_MIN_TEMPERATURE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BASIC_INFO_MAX_TEMPERATURE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BUTTON_DISP_AIRCON_AUTO.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BUTTON_DISP_AC_MODE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BUTTON_DISP_INTERNAL_CYCLE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BUTTON_DISP_EXTERNAL_CYCLE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BUTTON_DISP_FRONT_WINDOW_DEFOG.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BUTTON_DISP_REAR_WINDOW_DEFOG.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_TEMP_DISP_DRIVER.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_TEMP_DISP_PASSAGE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_TEMP_DISP_REAR_LEFT.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_TEMP_DISP_REAR_RIGHT.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BLOWER_DISP_FRONT.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_BLOWER_DISP_REAR.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_OUTLET_DISP_DRIVER.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_OUTLET_DISP_PASSAGE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_OUTLET_DISP_REARLEFT.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_OUTLET_DISP_REARRIGHT.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_SKYLIGHTDEGREE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_CURSPEED.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_FUELWARNING.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_MAINTAININFO.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_DRIVINGRANGE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_FRONTFOGLIGHTS.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_REARFOGLIGHTS.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_HIGHBEAM.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_FUELAVERAGE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_TOTALMILEAGE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_WINDOW_DRIVER.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_WINDOW_PASSAGE.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_WINDOW_REARLEFT.c_str());
    m_spVehiAgent->GetPropertyAsync(VR_VEHICLEINFO_WINDOW_REARRIGHT.c_str());
}

bool VR_VehicleInfoManager::ReplyVehicleACStatusToVR(const VR_String& id)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_AIRCONDITION_STATUS_RESP_XML);
    if (!res) {
        VR_LOGD("load error");
        return false;
    }

    const INT forntMinBlower = 1;
    VR_INT32 iMaxTemp = m_spVehiHelper->getMaxTemperature();
    VR_INT32 iMinTemp = m_spVehiHelper->getMinTemperature();
    float iTempStep = m_spVehiHelper->getTemperatureStep();
    float IBlowStep = m_spVehiHelper->getBlowerStep();
    VR_INT32 iFrMaxBlow = m_spVehiHelper->getFrontMaxBlower();
    Vr_Aircon_Temp iDrvTemp = m_spVehiHelper->getTempDriver();
    Vr_Aircon_Temp iPasTemp = m_spVehiHelper->getTempPassage();
    Vr_Aircon_Temp iReLeTemp = m_spVehiHelper->getTempRearLeft();
    Vr_Aircon_Temp iReRiTemp = m_spVehiHelper->getTempRearRight();
    VR_INT32 iRear = m_spVehiHelper->getRear();
    VR_INT32 iFront = m_spVehiHelper->getFront();
    VR_INT32 iOutDrv = m_spVehiHelper->getOutletDriver();
    VR_INT32 iOutPas = m_spVehiHelper->getOutletPassage();
    VR_INT32 iOutReRi = m_spVehiHelper->getOutletRearRight();
    VR_INT32 iOutReLf = m_spVehiHelper->getOutletRearLeft();
    VR_INT32 iTempUnit = m_spVehiHelper->getTemperatureUnit();
    VR_String acUnit = 'C' == iTempUnit ? "C" : "F";

    VR_String acStatus;
    if (Vr_Aircon_Temp_Type_Unknown == iDrvTemp.type) {
        acStatus = "invalid";
    }
    else if (Vr_Aircon_Temp_Type_Off == iDrvTemp.type) {
        acStatus = "off";
    }
    else {
        acStatus = "on";
    }

    parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action_result/action_id", id);
    parser.SetNodeText("//acstatus/ac", acStatus);
    parser.SetNodeText("//acstatus/temperature/max", iMaxTemp);
    parser.SetNodeText("//acstatus/temperature/min", iMinTemp);
    parser.SetNodeText("//acstatus/temperature/step", iTempStep);
    parser.SetNodeText("//acstatus/temperature/unit", acUnit);
    parser.SetNodeText("//acstatus/fanspeed/step", IBlowStep);
    parser.SetNodeText("//acstatus/fanspeed/min", forntMinBlower);
    parser.SetNodeText("//acstatus/fanspeed/max", iFrMaxBlow);
    parser.SetNodeText("//curr_driver_temp", iDrvTemp.data);
    parser.SetNodeText("//curr_passage_temp", iPasTemp.data);
    parser.SetNodeText("//curr_rearleft_temp", iReLeTemp.data);
    parser.SetNodeText("//curr_rearright_temp", iReRiTemp.data);
    parser.SetNodeText("//curr_rear_fanspeed", iRear);
    parser.SetNodeText("//curr_front_fanspeed", iFront);

    VR_String strMode;
    do {
        res = m_spVehiHelper->RenderVehicleFanStatus(iOutDrv, strMode);
        if (!res) {
            break;
        }
        parser.SetNodeText("//curr_driver_fanmode", strMode);

        res = m_spVehiHelper->RenderVehicleFanStatus(iOutPas, strMode);
        if (!res) {
            break;
        }
        parser.SetNodeText("//curr_passage_fanmode", strMode);

        res = m_spVehiHelper->RenderVehicleFanStatus(iOutReRi, strMode);
        if (!res) {
            break;
        }
        parser.SetNodeText("//curr_rearright_fanmode", strMode);

        res = m_spVehiHelper->RenderVehicleFanStatus(iOutReLf, strMode);
        if (!res) {
            break;
        }
        parser.SetNodeText("//curr_rearleft_fanmode", strMode);
    } while (0);

    if (!res) {
        parser.SetNodeText("//action_result/error_no", "VEHICLE_RESULT_ERROR");
    }
    VR_String strMsg;
    parser.GetXmlString(strMsg);

    VR_LOGD("Mesg; %s", strMsg.c_str());
    SendReqToNATP(strMsg);
    return true;
}

bool VR_VehicleInfoManager::ReplyVehicleSunroofStatusToVR(const VR_String &strId)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    if (parser.LoadString(VR_VEHICLE_SUNROOF_STATUS_RESP_XML)) {
        parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());
        parser.SetNodeText("//action_result/action_id", strId);
        VR_String strSunroofState;
        bool res = m_spVehiHelper->RenderVehicleSunroofStatus(m_spVehiHelper->getSunroof_State(), strSunroofState);
        if (res) {
            parser.SetNodeText("//action_result/sunroof_state", strSunroofState);
        }
        else {
            parser.SetNodeText("//action_result/error_no", "VEHICLE_RESULT_ERROR");
        }
        VR_String strMsg = parser.TransferXMLtoString();
        VR_LOGD("Mesg; %s", strMsg.c_str());
        SendReqToNATP(strMsg);
        return true;
    }
    else {
        VR_LOGD("LoadString [%s] error", VR_VEHICLE_SUNROOF_STATUS_RESP_XML);
        return false;
    }
}

bool VR_VehicleInfoManager::ReplyVehicleFuelConsumption(const VR_String& strId)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse parser;
    parser.LoadString(VR_GET_FUEL_CONSUMPTION_RESULT);

    parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action_result/action_id", strId);

    float mpg = m_spVehiHelper->getFuelAverage();
    char buff[5] = { 0 };     //  classic data size is 0.0 ~ 99.9
    snprintf(buff, 5, "%0.1f", mpg);

    VR_String data(buff);
    if (std::string::npos == data.find(".0")) {
        parser.SetNodeText("//action_result/fuel_num", data);
    }
    else {
        parser.SetNodeText("//action_result/fuel_num", data.substr(0, data.find(".")));
    }

    VR_String mesg;
    parser.GetXmlString(mesg);
    SendReqToNATP(mesg);
    return true;
}

bool VR_VehicleInfoManager::ReplyVehicleMileAgeLeft(const VR_String& strId)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse parser;
    parser.LoadString(VR_GET_MILEAGELEFT_RESULT);

    parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action_result/action_id", strId);

    float mpg = m_spVehiHelper->getDrivingRange();

    char buff[7] = { 0 };     // classic data size is 0.0 ~ 9999.9
    snprintf(buff, 7, "%0.1f", mpg);

    VR_String data(buff);
    if (std::string::npos == data.find(".0")) {
        parser.SetNodeText("//action_result/mileage_num", data);
    }
    else {
        parser.SetNodeText("//action_result/mileage_num", data.substr(0, data.find(".")));
    }

    VR_String mesg;
    parser.GetXmlString(mesg);
    SendReqToNATP(mesg);
    return true;
}

bool VR_VehicleInfoManager::ReplyVehicleMileAgePeriod(const VR_String& strId)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse parser;
    parser.LoadString(VR_GET_MILEAGE_PERIOD_RESULT);

    parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action_result/action_id", strId);

    float mpg = m_spVehiHelper->getTotalMileage();

    char buff[9] = { 0 };    // classic data size 0.0 ~ 999999.9
    snprintf(buff, 9, "%0.1f", mpg);

    VR_String data(buff);
    if (std::string::npos == data.find(".0")) {
        parser.SetNodeText("//action_result/mileage_num", data);
    }
    else {
        parser.SetNodeText("//action_result/mileage_num", data.substr(0, data.find(".")));
    }

    VR_String mesg;
    parser.GetXmlString(mesg);
    SendReqToNATP(mesg);
    return true;
}

bool VR_VehicleInfoManager::ReplyVehicleWindowsStatus(const VR_String& strId)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse parser;
    parser.LoadString(VR_GET_WINDOWS_STATUS_RESULT);

    int drvStat = m_spVehiHelper->getWindowDriver();
    int passStat = m_spVehiHelper->getWindowPassager();
    int rearLStat = m_spVehiHelper->getWindowRearLeft();
    int rearRStat = m_spVehiHelper->getWindowRearRight();

    parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action_result/action_id", strId);

    if (0 == drvStat) {
        parser.SetNodeText("//action_result/driver_window_state", "WINDOW_STATE_OFF");
    }
    else if (100 == drvStat) {
        parser.SetNodeText("//action_result/driver_window_state", "WINDOW_STATE_FULL_OPEN");
    }
    else {
        parser.SetNodeText("//action_result/driver_window_state", "WINDOW_STATE_OPEN_A_GAP");
    }

    if (0 == passStat) {
        parser.SetNodeText("//action_result/front_passenger_window_state", "WINDOW_STATE_OFF");
    }
    else if (100 == passStat) {
        parser.SetNodeText("//action_result/front_passenger_window_state", "WINDOW_STATE_FULL_OPEN");
    }
    else {
        parser.SetNodeText("//action_result/front_passenger_window_state", "WINDOW_STATE_OPEN_A_GAP");
    }

    if (0 == rearLStat && 0 == rearRStat) {
        parser.SetNodeText("//action_result/back_seat_window_state", "WINDOW_STATE_OFF");
    }
    else if (100 == rearLStat || 100 == rearRStat) {
        parser.SetNodeText("//action_result/back_seat_window_state", "WINDOW_STATE_FULL_OPEN");
    }
    else {
        parser.SetNodeText("//action_result/back_seat_window_state", "WINDOW_STATE_OPEN_A_GAP");
    }

    VR_String mesg;
    parser.GetXmlString(mesg);
    SendReqToNATP(mesg);
    return true;
}

bool VR_VehicleInfoManager::ReplyVehicleNextMaintanceInfo(const VR_String& strId)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse parser;
    parser.LoadString(VR_GET_MAINTAININFO_RESULT);

    parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action_result/action_id", strId);

    VrMaintainInfo info = m_spVehiHelper->getMaintainInfo();

    int64_t mile = info.value_mile();
    int64_t date = info.value_date();

    if (VrMaintainStatusType_Expired == info.type_maintainmile()) {
        mile = 0 - mile;
    }

    if (VrMaintainStatusType_Expired == info.type_maintaindate()) {
        date = 0 - date;
    }

    parser.SetNodeText("//action_result/mileage_num", std::to_string(mile));
    parser.SetNodeText("//action_result/day_num", std::to_string(date));

    VR_String mesg;
    parser.GetXmlString(mesg);
    SendReqToNATP(mesg);
    return true;
}

bool VR_VehicleInfoManager::ReplyVehicleCurSpeedToVR(const VR_String &strId)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    if (parser.LoadString(VR_VEHICLE_CURSPEED_RESP_XML)) {
//        VR_ContentProvider vr_cp;
//        VR_String strVehicle = vr_cp.GetContentProviderValue(VR_VEHICLE_MOTION);
//        VR_String strIsMoving = "";
//        if (strVehicle == "on") {
//            strIsMoving = "true";
//        }
//        else {
//            strIsMoving = "false";
//        }
        VR_String strIsMoving = "";
        VR_INT32 curSpeed = m_spVehiHelper->getCurSpeed();
        if (0 != curSpeed) {
            strIsMoving = "true";
        }
        else {
            strIsMoving = "false";
        }
        parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());
        parser.SetNodeText("//action_result/action_id", strId);
        parser.SetNodeText("//action_result/is_moving", strIsMoving);
        parser.SetNodeText("//action_result/cur_speed", curSpeed);

        VR_String strMsg = parser.TransferXMLtoString();
        VR_LOGD("Mesg; %s", strMsg.c_str());
        SendReqToNATP(strMsg);
        return true;
    }
    else {
        VR_LOGD("LoadString [%s] error", VR_VEHICLE_CURSPEED_RESP_XML);
        return false;
    }
}

void VR_VehicleInfoManager::UpdateAircondStatus(const VR_ProtoMessageBase& mesg)
{
    const VrAirconditionStatus& status = (VrAirconditionStatus&)mesg;

    // the status which need notify to natp
    VR_INT32 isTraget = 0;

    if (status.has_m_vrbasicinfo()) {
        VR_LOGD("basic info");
        VrBasicInfo basicInfo = status.m_vrbasicinfo();

        if (basicInfo.has_aircontype()) {
            m_spVehiHelper->setAirconType(basicInfo.aircontype());
        }
        else if (basicInfo.has_temperatureunit()) {
            m_spVehiHelper->setTemperatureUnit(basicInfo.temperatureunit());
        }
        else if (basicInfo.has_frontmaxblower()) {
            m_spVehiHelper->setFrontMaxBlower(basicInfo.frontmaxblower());
        }
        else if (basicInfo.has_rearmaxblower()) {
            m_spVehiHelper->setRearMaxBlower(basicInfo.rearmaxblower());
        }
        else if (basicInfo.has_temperaturestep()) {
            m_spVehiHelper->setTemperatureStep(basicInfo.temperaturestep());
        }
        else if (basicInfo.has_blowerstep()) {
            m_spVehiHelper->setBlowerStep(basicInfo.blowerstep());
        }
        else if (basicInfo.has_mintemperature()) {
            m_spVehiHelper->setMinTemperature(basicInfo.mintemperature());
        }
        else if (basicInfo.has_maxtemperature()) {
            m_spVehiHelper->setMaxTemperature(basicInfo.maxtemperature());
        }
    }
    else if (status.has_m_vrbuttondisplay()) {
        VR_LOGD("button dispaly");
        VrButtonDisplay buttonDisplay = status.m_vrbuttondisplay();

        if (buttonDisplay.has_airconauto()) {
            m_spVehiHelper->setAirconAuto(buttonDisplay.airconauto());
        }
        else if (buttonDisplay.has_acmode()) {
            m_spVehiHelper->setACMode(buttonDisplay.acmode());
        }
        else if (buttonDisplay.has_internalcycle()) {
            m_spVehiHelper->setInternalCycle(buttonDisplay.internalcycle());
        }
        else if (buttonDisplay.has_externalcycle()) {
            m_spVehiHelper->setExternalCycle(buttonDisplay.externalcycle());
        }
        else if (buttonDisplay.has_frontwindowdefog()) {
            m_spVehiHelper->setTrontWindowDefog(buttonDisplay.frontwindowdefog());
        }
        else if (buttonDisplay.has_rearwindowdefog()) {
            m_spVehiHelper->setRearWindowDefog(buttonDisplay.rearwindowdefog());
        }
    }
    else if (status.has_m_vrtemperaturedisplay()) {
        VR_LOGD("temperature display");
        VrTemperatureDisplay tempDisplay = status.m_vrtemperaturedisplay();

        if (tempDisplay.has_m_driver()) {
            Vr_Aircon_Temp temp;
            VrTemperatureDisplay_VrAirconTemperature driverTemp = tempDisplay.m_driver();
            temp.type = static_cast<Vr_Aircon_Temp_Type>(driverTemp.temtype());
            temp.data = driverTemp.temnumber();
            m_spVehiHelper->setTempDriver(temp);
            isTraget = VR_PROP_TEMP_DISP_DRIVER;
        }
        else if (tempDisplay.has_m_passage()) {
            Vr_Aircon_Temp temp;
            VrTemperatureDisplay_VrAirconTemperature passageTemp = tempDisplay.m_passage();
            temp.type = static_cast<Vr_Aircon_Temp_Type>(passageTemp.temtype());
            temp.data = passageTemp.temnumber();
            m_spVehiHelper->setTempPassage(temp);
            isTraget = VR_PROP_TEMP_DISP_PASSAGE;
        }
        else if (tempDisplay.has_m_rearleft()) {
            Vr_Aircon_Temp temp;
            VrTemperatureDisplay_VrAirconTemperature rearleftTemp = tempDisplay.m_rearleft();
            temp.type = static_cast<Vr_Aircon_Temp_Type>(rearleftTemp.temtype());
            temp.data = rearleftTemp.temnumber();
            m_spVehiHelper->setTempRearLeft(temp);
            isTraget = VR_PROP_TEMP_DISP_REAR_LEFT;
        }
        else if (tempDisplay.has_m_rearright()) {
            Vr_Aircon_Temp temp;
            VrTemperatureDisplay_VrAirconTemperature rearrightTemp = tempDisplay.m_rearright();
            temp.type = static_cast<Vr_Aircon_Temp_Type>(rearrightTemp.temtype());
            temp.data = rearrightTemp.temnumber();
            m_spVehiHelper->setTempRearRight(temp);
            isTraget = VR_PROP_TEMP_DISP_REAR_RIGHT;
        }
    }
    else if (status.has_m_vrblowerdisplay()) {
        VR_LOGD("blower display");
        VrBlowerDisplay blowerDisplay = status.m_vrblowerdisplay();

        if (blowerDisplay.has_front()) {
            m_spVehiHelper->setFront(blowerDisplay.front());
            isTraget = VR_PROP_BLOWER_DISP_FRONT;
        }
        else if (blowerDisplay.has_rear()) {
            m_spVehiHelper->setRear(blowerDisplay.rear());
            isTraget = VR_PROP_BLOWER_DISP_REAR;
        }
    }
    else if (status.has_m_vroutletdisplay()) {
        VR_LOGD("outlet display");
        VrOutletDisplay outletDisplay = status.m_vroutletdisplay();

        if (outletDisplay.has_driver()) {
            m_spVehiHelper->setOutletDriver(outletDisplay.driver());
            isTraget = VR_PROP_OUTLET_DISP_DRIVER;
        }
        else if (outletDisplay.has_passage()) {
            m_spVehiHelper->setOutletPassage(outletDisplay.passage());
            isTraget = VR_PROP_OUTLET_DISP_PASSAGE;
        }
        else if (outletDisplay.has_rearleft()) {
            m_spVehiHelper->setOutletRearLeft(outletDisplay.rearleft());
            isTraget = VR_PROP_OUTLET_DISP_REARLEFT;
        }
        else if (outletDisplay.has_rearright()) {
            m_spVehiHelper->setOutletRearRight(outletDisplay.rearright());
            isTraget = VR_PROP_OUTLET_DISP_REARRIGHT;
        }
    }

    if (0 != isTraget) {
        bool res = NotifyVehicleStatus(isTraget);
        if (res) {
            VR_LOGD("notify error.");
        }
    }
}

void VR_VehicleInfoManager::UpdateVehicleStatus(const VR_ProtoMessageBase &mesg)
{
    VR_LOGD_FUNC();
    const VrVehicleInfo& status = dynamic_cast<const VrVehicleInfo&>(mesg);
    if (status.has_m_vrskylightdegree()) {
        VR_LOGD("Vehicle SkyLightDegree");
        if (0 == status.m_vrskylightdegree()) {
            m_spVehiHelper->setSunroof_State(VR_SUNROOF_STATE_CLOSE);
        }
        else if (50 == status.m_vrskylightdegree()) {
            m_spVehiHelper->setSunroof_State(VR_SUNROOF_STATE_HALF_OPEN);
        }
        else if (100 == status.m_vrskylightdegree()) {
            m_spVehiHelper->setSunroof_State(VR_SUNROOF_STATE_FULL_OPEN);
        }
        else {
            m_spVehiHelper->setSunroof_State(VR_SUNROOF_STATE_OPEN_A_GAP);
        }
    }
    else if (status.has_m_vrcurspeed()) {
        VR_LOGD("Vehicle CurSpeed: %d", status.m_vrcurspeed());
        m_spVehiHelper->setCurSpeed(status.m_vrcurspeed());
    }
    else if (status.has_m_vrdrivingrange()) {
        m_spVehiHelper->setDrivingRange(status.m_vrdrivingrange());
    }
    else if (status.has_m_vrmaintaininfo()) {
        m_spVehiHelper->setMaintainInfo(status.m_vrmaintaininfo());
    }
    else if (status.has_m_vrfuelaverage()) {
        VR_LOGD("fuel average: %f", status.m_vrfuelaverage());
        m_spVehiHelper->setFuelAverage(status.m_vrfuelaverage());
    }
    else if (status.has_m_vrtotalmileage()) {
        VR_LOGD("fuel average: %f", status.m_vrtotalmileage());
        m_spVehiHelper->setTotalMileage(status.m_vrtotalmileage());
    }
    else if (status.has_m_vrwindowsdriver()) {
        m_spVehiHelper->setWindowDriver(status.m_vrwindowsdriver());
    }
    else if (status.has_m_vrwindowspassager()) {
        m_spVehiHelper->setWindowPassager(status.m_vrwindowspassager());
    }
    else if (status.has_m_vrwindowsrearleft()) {
        m_spVehiHelper->setWindowRearLeft(status.m_vrwindowsrearleft());
    }
    else if (status.has_m_vrwindowsrearright()) {
        m_spVehiHelper->setWindowRearRight(status.m_vrwindowsrearright());
    }
}

bool VR_VehicleInfoManager::NotifyVehicleStatus(const VR_INT32& propertyType)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_AIRCONDITION_STATUS_NOTIFY_XML);
    if (!res) {
        VR_LOGD("load xml failed");
        return false;
    }
    Vr_Aircon_Temp iDrvTemp = m_spVehiHelper->getTempDriver();
    VR_String acStatus;
    if (Vr_Aircon_Temp_Type_Unknown == iDrvTemp.type) {
        acStatus = "invalid";
    }
    else if (Vr_Aircon_Temp_Type_Off == iDrvTemp.type) {
        acStatus = "off";
    }
    else {
        acStatus = "on";
    }
    parser.SetNodeText("//acstatus/ac", acStatus);
    parser.SetNodeText("//notification/id", VR_IdGenerator().GetID());

    switch (propertyType) {
    case VR_PROP_TEMP_DISP_DRIVER: {
        parser.AddNode("//notification/acstatus", "curr_driver_temp", iDrvTemp.data);
        break;
    }
    case VR_PROP_TEMP_DISP_PASSAGE: {
        Vr_Aircon_Temp iPasTemp = m_spVehiHelper->getTempPassage();
        parser.AddNode("//notification/acstatus", "curr_passage_temp", iPasTemp.data);
        break;
    }
    case VR_PROP_TEMP_DISP_REAR_LEFT: {
        Vr_Aircon_Temp iReLeTemp = m_spVehiHelper->getTempRearLeft();
        parser.AddNode("//notification/acstatus", "curr_rearleft_temp", iReLeTemp.data);
        break;
    }
    case VR_PROP_TEMP_DISP_REAR_RIGHT: {
        Vr_Aircon_Temp iReRiTemp = m_spVehiHelper->getTempRearRight();
        parser.AddNode("//notification/acstatus", "curr_rearright_temp", iReRiTemp.data);
        break;
    }
    case VR_PROP_BLOWER_DISP_FRONT: {
        VR_INT32 iBlowFr = m_spVehiHelper->getFront();
        parser.AddNode("//notification/acstatus", "curr_front_fanspeed", iBlowFr);
        break;
    }
    case VR_PROP_BLOWER_DISP_REAR: {
        VR_INT32 iRear = m_spVehiHelper->getRear();
        parser.AddNode("//notification/acstatus", "curr_rear_fanspeed", iRear);
        break;
    }
    case VR_PROP_OUTLET_DISP_DRIVER: {
        VR_INT32 iOutDrv = m_spVehiHelper->getOutletDriver();
        VR_String strMode;
        res = m_spVehiHelper->RenderVehicleFanStatus(iOutDrv, strMode);
        if (!res) {
            return false;
        }
        parser.AddNode("//notification/acstatus", "curr_driver_fanmode", strMode);
        break;
    }
    case VR_PROP_OUTLET_DISP_PASSAGE: {
        VR_INT32 iOutPas = m_spVehiHelper->getOutletPassage();
        VR_String strMode;
        res = m_spVehiHelper->RenderVehicleFanStatus(iOutPas, strMode);
        if (!res) {
            return false;
        }
        parser.AddNode("//notification/acstatus", "curr_passage_fanmode", strMode);
        break;
    }
    case VR_PROP_OUTLET_DISP_REARLEFT: {
        VR_INT32 iOutReLf = m_spVehiHelper->getOutletRearLeft();
        VR_String strMode;
        res = m_spVehiHelper->RenderVehicleFanStatus(iOutReLf, strMode);
        if (!res) {
            return false;
        }
        parser.AddNode("//notification/acstatus", "curr_rearleft_fanmode", strMode);
        break;
    }
    case VR_PROP_OUTLET_DISP_REARRIGHT: {
        VR_INT32 iOutReRi = m_spVehiHelper->getOutletRearRight();
        VR_String strMode;
        res = m_spVehiHelper->RenderVehicleFanStatus(iOutReRi, strMode);
        if (!res) {
            return false;
        }
        parser.AddNode("//notification/acstatus", "curr_rearright_fanmode", strMode);
        break;
    }
    default:{
        VR_LOGD("default!");
        return false;
    }
    }

    VR_String strMsg;
    parser.GetXmlString(strMsg);

    VR_LOGD("Mesg: %s", strMsg.c_str());
    SendReqToNATP(strMsg);
    return true;
}

void VR_VehicleInfoManager::SendReqToNATP(VR_String &strXml)
{
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(strXml);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}
/* EOF */
