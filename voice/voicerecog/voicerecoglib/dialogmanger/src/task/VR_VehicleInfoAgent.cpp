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

/**
 * @file VR_VehicleInfoAgent.cpp
 * @brief Implementation of class VR_VehicleInfoAgent
 */


#include "VR_Log.h"
#include "VR_VehicleInfoAgent.h"
#include "VR_VehicleInfoProps.h"
#include "VR_VehicleInfoReplier.h"

#include "EV_Define.h"
#include "EV_EventSender.h"

#include <boost/assign.hpp>
#include <evip/NEIpcServiceHelper.h>
#include <evip/NEIpcServiceProxy.h>

#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "navi/Voice/VoiceRecog/VrAirconditionStatus.pb.h"
#include "navi/Voice/VoiceRecog/VrVehicleInfo.pb.h"

using namespace nutshell;
using namespace navi::VoiceRecog;

std::list<VR_String>VR_VehicleInfoAgent::m_ListUri = boost::assign::list_of
        (VR_BASIC_INFO_ACTION_TYPE)
        (VR_BASIC_INFO_TEMPERATURE_UNIT)
        (VR_BASIC_INFO_FRONT_MAX_BLOWER)
        (VR_BASIC_INFO_REAR_MAX_BLOWER)
        (VR_BASIC_INFO_TEMPERATURE_STEP)
        (VR_BASIC_INFO_BLOWER_STEP)
        (VR_BASIC_INFO_MIN_TEMPERATURE)
        (VR_BASIC_INFO_MAX_TEMPERATURE)
        (VR_BUTTON_DISP_AIRCON_AUTO)
        (VR_BUTTON_DISP_AC_MODE)
        (VR_BUTTON_DISP_INTERNAL_CYCLE)
        (VR_BUTTON_DISP_EXTERNAL_CYCLE)
        (VR_BUTTON_DISP_FRONT_WINDOW_DEFOG)
        (VR_BUTTON_DISP_REAR_WINDOW_DEFOG)
        (VR_TEMP_DISP_DRIVER)
        (VR_TEMP_DISP_PASSAGE)
        (VR_TEMP_DISP_REAR_LEFT)
        (VR_TEMP_DISP_REAR_RIGHT)
        (VR_BLOWER_DISP_FRONT)
        (VR_BLOWER_DISP_REAR)
        (VR_OUTLET_DISP_DRIVER)
        (VR_OUTLET_DISP_PASSAGE)
        (VR_OUTLET_DISP_REARLEFT)
        (VR_OUTLET_DISP_REARRIGHT)
        (VR_VEHICLEINFO_SKYLIGHTDEGREE)
        (VR_VEHICLEINFO_CURSPEED)
        (VR_VEHICLEINFO_FUELWARNING)
        (VR_VEHICLEINFO_MAINTAININFO)
        (VR_VEHICLEINFO_DRIVINGRANGE)
        (VR_VEHICLEINFO_FRONTFOGLIGHTS)
        (VR_VEHICLEINFO_REARFOGLIGHTS)
        (VR_VEHICLEINFO_HIGHBEAM)
        (VR_VEHICLEINFO_FUELAVERAGE)
        (VR_VEHICLEINFO_TOTALMILEAGE)
        (VR_VEHICLEINFO_WINDOW_DRIVER)
        (VR_VEHICLEINFO_WINDOW_PASSAGE)
        (VR_VEHICLEINFO_WINDOW_REARLEFT)
        (VR_VEHICLEINFO_WINDOW_REARRIGHT);

VR_VehicleInfoAgent::VR_VehicleInfoAgent()
    : m_spProxy(nullptr)
    , m_spReplier(nullptr)
{
    VR_LOGD_FUNC();
    m_spProxy = VR_new nutshell::NCVehicleInfo;
    m_spReplier = VR_new VR_VehicleInfoReplier(this);
    RegisterReplyReceiver();
}

VR_VehicleInfoAgent::~VR_VehicleInfoAgent()
{
    VR_LOGD_FUNC();
}

void VR_VehicleInfoAgent::SubScribleProperty()
{
    VR_LOGD_FUNC();
    NC_BOOL res = m_spProxy->subscribeProperty(m_ListUri);
    if (!res) {
        VR_LOGI("subscribe falied");
    }
}

void VR_VehicleInfoAgent::UnSubScribleProperty()
{
    VR_LOGD_FUNC();
    NC_BOOL res = m_spProxy->unsubscribeProperty(m_ListUri);
    if (!res) {
        VR_LOGI("subscribe falied");
    }
}

void VR_VehicleInfoAgent::handleData(const char *property, const nutshell::NCVariant &val)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_VEHICLE_INFO_RESP);


    ctMapIter iter = map_propertyType.find(property);
    if (iter == map_propertyType.end()) {
        VR_LOGD("unknown property. %s", property);
        return;
    }

    bool result = true;
    VR_LOGD("vehicle property is [%d]", iter->second);
    switch (iter->second) {
    case VR_PROP_BASIC_INFO_ACTION_TYPE:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrBasicInfo* basicInfo = airCondStatus->mutable_m_vrbasicinfo();
        int ivalue;
        result = val.value<int>(ivalue);
        if (!result) {
            break;
        }
        basicInfo->set_aircontype(ivalue);
        VR_LOGD("Aircon type value is [%d]", ivalue);
        break;
    }
    case VR_PROP_BASIC_INFO_TEMPERATURE_UNIT:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrBasicInfo* basicInfo = airCondStatus->mutable_m_vrbasicinfo();
        int ivalue;
        result = val.value<int>(ivalue);
        if (!result) {
            break;
        }
        basicInfo->set_temperatureunit(ivalue);
        VR_LOGD("Tempu unit value is [%d]", ivalue);
        break;
    }
    case VR_PROP_BASIC_INFO_FRONT_MAX_BLOWER:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrBasicInfo* basicInfo = airCondStatus->mutable_m_vrbasicinfo();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        basicInfo->set_frontmaxblower(ivalue);
        VR_LOGD("Front max blower value is [%d]", ivalue);
        break;
    }
    case VR_PROP_BASIC_INFO_REAR_MAX_BLOWER:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrBasicInfo* basicInfo = airCondStatus->mutable_m_vrbasicinfo();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        basicInfo->set_rearmaxblower(ivalue);
        VR_LOGD("Rear max blower value is [%d]", ivalue);
        break;
    }
    case VR_PROP_BASIC_INFO_TEMPERATURE_STEP:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrBasicInfo* basicInfo = airCondStatus->mutable_m_vrbasicinfo();
        float fvalue;
        result = val.value(fvalue);
        if (!result) {
            break;
        }
        basicInfo->set_temperaturestep(fvalue);
        VR_LOGD("Tempu step value is [%.3f]", fvalue);
        break;
    }
    case VR_PROP_BASIC_INFO_BLOWER_STEP:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrBasicInfo* basicInfo = airCondStatus->mutable_m_vrbasicinfo();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        basicInfo->set_blowerstep(ivalue);
        VR_LOGD("Blow step value is [%d]", ivalue);
        break;
    }
    case VR_PROP_BASIC_INFO_MIN_TEMPERATURE:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrBasicInfo* basicInfo = airCondStatus->mutable_m_vrbasicinfo();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        basicInfo->set_mintemperature(ivalue);
        VR_LOGD("Tempu min value is [%d]", ivalue);
        break;
    }
    case VR_PROP_BASIC_INFO_MAX_TEMPERATURE:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrBasicInfo* basicInfo = airCondStatus->mutable_m_vrbasicinfo();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        basicInfo->set_maxtemperature(ivalue);
        VR_LOGD("Tempu max value is [%d]", ivalue);
        break;
    }
    case VR_PROP_BUTTON_DISP_AIRCON_AUTO:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrButtonDisplay* buttonDisplay = airCondStatus->mutable_m_vrbuttondisplay();
        bool bvalue;
        result = val.value(bvalue);
        if (!result) {
            break;
        }
        buttonDisplay->set_airconauto(bvalue);
        VR_LOGD("Airco auto value is [%d]", bvalue);
        break;
    }
    case VR_PROP_BUTTON_DISP_AC_MODE:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrButtonDisplay* buttonDisplay = airCondStatus->mutable_m_vrbuttondisplay();
        bool bvalue;
        result = val.value(bvalue);
        if (!result) {
            break;
        }
        buttonDisplay->set_acmode(bvalue);
        VR_LOGD("AC mode value is [%d]", bvalue);
        break;
    }
    case VR_PROP_BUTTON_DISP_INTERNAL_CYCLE:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrButtonDisplay* buttonDisplay = airCondStatus->mutable_m_vrbuttondisplay();
        bool bvalue;
        result = val.value(bvalue);
        if (!result) {
            break;
        }
        buttonDisplay->set_internalcycle(bvalue);
        VR_LOGD("Internal cycle value is [%d]", bvalue);
        break;
    }
    case VR_PROP_BUTTON_DISP_EXTERNAL_CYCLE:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrButtonDisplay* buttonDisplay = airCondStatus->mutable_m_vrbuttondisplay();
        bool bvalue;
        result = val.value(bvalue);
        if (!result) {
            break;
        }
        buttonDisplay->set_externalcycle(bvalue);
        VR_LOGD("External cycle value is [%d]", bvalue);
        break;
    }
    case VR_PROP_BUTTON_DISP_FRONT_WINDOW_DEFOG:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrButtonDisplay* buttonDisplay = airCondStatus->mutable_m_vrbuttondisplay();
        bool bvalue;
        result = val.value(bvalue);
        if (!result) {
            break;
        }
        buttonDisplay->set_frontwindowdefog(bvalue);
        VR_LOGD("Front window defog value is [%d]", bvalue);
        break;
    }
    case VR_PROP_BUTTON_DISP_REAR_WINDOW_DEFOG:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrButtonDisplay* buttonDisplay = airCondStatus->mutable_m_vrbuttondisplay();
        bool bvalue;
        result = val.value(bvalue);
        if (!result) {
            break;
        }
        buttonDisplay->set_rearwindowdefog(bvalue);
        VR_LOGD("Rear window defog value is [%d]", bvalue);
        break;
    }
    case VR_PROP_TEMP_DISP_DRIVER:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrTemperatureDisplay* tempDisplay = airCondStatus->mutable_m_vrtemperaturedisplay();
        VrTemperatureDisplay_VrAirconTemperature* driver = tempDisplay->mutable_m_driver();
        std::string svalue;
        result = val.value(svalue);
        if (!result) {
            break;
        }
        driver->ParseFromString(svalue);
        VR_LOGD("Disp driver aircon temp value is [%s]", svalue.c_str());
        break;
    }
    case VR_PROP_TEMP_DISP_PASSAGE:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrTemperatureDisplay* tempDisplay = airCondStatus->mutable_m_vrtemperaturedisplay();
        VrTemperatureDisplay_VrAirconTemperature* passage = tempDisplay->mutable_m_passage();
        std::string svalue;
        result = val.value(svalue);
        if (!result) {
            break;
        }
        passage->ParseFromString(svalue);
        VR_LOGD("Disp passage aircon temp value is [%s]", svalue.c_str());
        break;
    }
    case VR_PROP_TEMP_DISP_REAR_LEFT:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrTemperatureDisplay* tempDisplay = airCondStatus->mutable_m_vrtemperaturedisplay();
        VrTemperatureDisplay_VrAirconTemperature* rearLeft = tempDisplay->mutable_m_rearleft();
        std::string svalue;
        result = val.value(svalue);
        if (!result) {
            break;
        }
        rearLeft->ParseFromString(svalue);
        VR_LOGD("Disp rearlf aircon temp value is [%s]", svalue.c_str());
        break;
    }
    case VR_PROP_TEMP_DISP_REAR_RIGHT:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrTemperatureDisplay* tempDisplay = airCondStatus->mutable_m_vrtemperaturedisplay();
        VrTemperatureDisplay_VrAirconTemperature* rearRight = tempDisplay->mutable_m_rearright();
        std::string svalue;
        result = val.value(svalue);
        if (!result) {
            break;
        }
        rearRight->ParseFromString(svalue);
        VR_LOGD("Disp rearri aircon temp value is [%s]", svalue.c_str());
        break;
    }
    case VR_PROP_BLOWER_DISP_FRONT:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrBlowerDisplay* blowerDisplay = airCondStatus->mutable_m_vrblowerdisplay();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        blowerDisplay->set_front(ivalue);
        VR_LOGD("Disp fron blower value is [%d]", ivalue);
        break;
    }
    case VR_PROP_BLOWER_DISP_REAR:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrBlowerDisplay* blowerDisplay = airCondStatus->mutable_m_vrblowerdisplay();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        blowerDisplay->set_rear(ivalue);
        VR_LOGD("Disp rear blower value is [%d]", ivalue);
        break;
    }
    case VR_PROP_OUTLET_DISP_DRIVER:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrOutletDisplay* outletDisplay = airCondStatus->mutable_m_vroutletdisplay();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        outletDisplay->set_driver(ivalue);
        VR_LOGD("Disp driver outlet value is [%d]", ivalue);
        break;
    }
    case VR_PROP_OUTLET_DISP_PASSAGE:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrOutletDisplay* outletDisplay = airCondStatus->mutable_m_vroutletdisplay();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        outletDisplay->set_passage(ivalue);
        VR_LOGD("Disp passage outlet value is [%d]", ivalue);
        break;
    }
    case VR_PROP_OUTLET_DISP_REARLEFT:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrOutletDisplay* outletDisplay = airCondStatus->mutable_m_vroutletdisplay();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        outletDisplay->set_rearleft(ivalue);
        VR_LOGD("Disp rearlf outlet value is [%d]", ivalue);
        break;
    }
    case VR_PROP_OUTLET_DISP_REARRIGHT:
    {
        VrAirconditionStatus* airCondStatus = loopMessage->mutable_m_airconditonstatus();
        VrOutletDisplay* outletDisplay = airCondStatus->mutable_m_vroutletdisplay();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        outletDisplay->set_rearright(ivalue);
        VR_LOGD("Disp rearri outlet value is [%d]", ivalue);
        break;
    }
    case VR_PROP_VEHICLEINFO_SKYLIGHTDEGREE:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        vehicleInfo->set_m_vrskylightdegree(ivalue);
        VR_LOGD("skylightdegree value is [%d]", ivalue);
        break;
    }
    case VR_PROP_VEHICLEINFO_CURSPEED:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();
        int ivalue;
        result = val.value(ivalue);
        if (!result) {
            break;
        }
        vehicleInfo->set_m_vrcurspeed(ivalue);
        VR_LOGD("current speed value is [%d]", ivalue);
        break;
    }
    case VR_PROP_VEHICLEINFO_FUELWARNING:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();
        bool bvalue;
        result = val.value(bvalue);
        if (!result) {
            break;
        }
        vehicleInfo->set_m_vrfuelwarning(bvalue);
        VR_LOGD("FuelWarning value is [%d]", bvalue);
        break;
    }
    case VR_PROP_VEHICLEINFO_MAINTAININFO:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();
        VrMaintainInfo* maintainInfo = vehicleInfo->mutable_m_vrmaintaininfo();
        std::string svalue;
        result = val.value(svalue);
        if (!result) {
            break;
        }
        maintainInfo->ParseFromString(svalue);
        VR_LOGD("maintain type mile[%d]", maintainInfo->type_maintainmile());
        VR_LOGD("maintain type date[%d]", maintainInfo->type_maintaindate());
        VR_LOGD("maintain mile[%d]", maintainInfo->value_mile());
        VR_LOGD("maintain date[%d]", maintainInfo->value_date());
        break;
    }
    case VR_PROP_VEHICLEINFO_DRIVINGRANGE:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();

        int iVal;
        result = val.value(iVal);
        if (!result) {
            break;
        }

        vehicleInfo->set_m_vrdrivingrange(iVal);
        VR_LOGD("driving range: %d km", iVal);
        break;
    }
    case VR_PROP_VEHICLEINFO_FRONTFOGLIGHTS:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();

        bool bVal;
        result = val.value(bVal);
        if (!result) {
            break;
        }

        vehicleInfo->set_m_vrfrontfoglights(bVal);
        VR_LOGD("front fog lights status: %d", bVal);
        break;
    }
    case VR_PROP_VEHICLEINFO_REARFOGLIGHTS:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();

        bool bVal;
        result = val.value(bVal);
        if (!result) {
            break;
        }

        vehicleInfo->set_m_vrrearfoglights(bVal);
        VR_LOGD("rear fog lights status: %d", bVal);
        break;
    }
    case VR_PROP_VEHICLEINFO_HIGHBEAM:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();

        bool bVal;
        result = val.value(bVal);
        if (!result) {
            break;
        }

        vehicleInfo->set_m_vrhighbeam(bVal);
        VR_LOGD("high beam status: %d", bVal);
        break;
    }
    case VR_PROP_VEHICLEINFO_FUELAVERAGE:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();

        float fVal;
        result = val.value(fVal);
        if (!result) {
            break;
        }

        vehicleInfo->set_m_vrfuelaverage(fVal);
        VR_LOGD("fuel average status: %f", fVal);
        break;
    }
    case VR_PROP_VEHICLEINFO_TOTALMILEAGE:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();

        int32_t iVal;
        result = val.value(iVal);
        if (!result) {
            break;
        }

        vehicleInfo->set_m_vrtotalmileage(iVal);
        VR_LOGD("high beam status: %f", iVal);
        break;
    }
    case VR_PROP_VEHICLEINFO_WINDOW_DRIVER:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();

        int Val;
        result = val.value(Val);
        if (!result) {
            break;
        }

        vehicleInfo->set_m_vrwindowsdriver(Val);
        VR_LOGD("window driver: %d", Val);
        break;
    }
    case VR_PROP_VEHICLEINFO_WINDOW_PASSAGE:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();

        int Val;
        result = val.value(Val);
        if (!result) {
            break;
        }

        vehicleInfo->set_m_vrwindowspassager(Val);
        VR_LOGD("window passager: %d", Val);
        break;
    }
    case VR_PROP_VEHICLEINFO_WINDOW_REARLEFT:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();

        int Val;
        result = val.value(Val);
        if (!result) {
            break;
        }

        vehicleInfo->set_m_vrwindowsrearleft(Val);
        VR_LOGD("window rear left: %d", Val);
        break;
    }
    case VR_PROP_VEHICLEINFO_WINDOW_REARRIGHT:
    {
        VrVehicleInfo* vehicleInfo = loopMessage->mutable_m_vehicleinfo();

        int Val;
        result = val.value(Val);
        if (!result) {
            break;
        }

        vehicleInfo->set_m_vrwindowsrearright(Val);
        VR_LOGD("window rear right: %d", Val);
        break;
    }
    default:
        result = false;
    };

    if (!result) {
        VR_LOGD("error happened");
        return;
    }

    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
}

bool VR_VehicleInfoAgent::SetPropertyAsync(const nutshell::NCCHAR *property, const nutshell::NCVariant &var)
{
    VR_LOGD_FUNC();
    bool res = m_spProxy->setPropertyAsyn(property, var);
    if (!res) {
        VR_LOGD("set property async failed");
    }
    return res;
}

void VR_VehicleInfoAgent::QueryListUriState()
{
    VR_LOGD_FUNC();
    for (VR_String item : m_ListUri) {
        bool res = m_spProxy->isSupportPro(item.c_str());
        if (!res) {
            VR_LOGD("query aysnc failed");
        }
    }
}

bool VR_VehicleInfoAgent::GetPropertyAsync(const nutshell::NCCHAR *property)
{
    VR_LOGD_FUNC();
    bool res = m_spProxy->getPropertyAsyn(property);
    if (!res) {
        VR_LOGI("get property async failed");
    }
    return res;
}

void VR_VehicleInfoAgent::RegisterReplyReceiver()
{
    VR_LOGD_FUNC();
    const NCString serviceName = "nutshell.vehicleinfo";
    auto ipc = NEIpcServiceHelper::findService(serviceName, 0);

    if (nullptr == ipc.get()) {
        VR_LOGD("binder nullptr");
        return;
    }

    m_spProxy->setReplyReceiver(m_spReplier);
    auto res = m_spProxy->bindService(ipc);
    if (!res) {
        VR_LOGI("bind service error");
    }
}
/* EOF */
