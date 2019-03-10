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
 * @file VR_VehicleInfoHelper.h
 * @brief Declaration file of class VR_VehicleInfoHelper.
 *
 * This file includes the declaration of class VR_VehicleInfoHelper.
 *
 * @attention used for C++ only.
 */

#ifndef VR_VEHICLEINFOHELPER_H
#define VR_VEHICLEINFOHELPER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_DialogTaskBase.h"
#include "VR_VehicleInfoProps.h"
#include "navi/Voice/VoiceRecog/VrVehicleInfo.pb.h"

/**
 * @brief The VR_VehicleInfoHelper class
 *
 * declare class VR_VehicleInfoHelper
 */
class VR_VehicleInfoHelper
{
public:
    VR_VehicleInfoHelper();
    virtual ~VR_VehicleInfoHelper();

    void setAirconType(const VR_INT32& status);
    VR_INT32 getAirconType();

    void setTemperatureUnit(const VR_INT32& status);
    VR_INT32 getTemperatureUnit();

    void setFrontMaxBlower(const VR_INT32& status);
    VR_INT32 getFrontMaxBlower();

    void setRearMaxBlower(const VR_INT32& status);
    VR_INT32 getRearMaxBlower();

    void setTemperatureStep(const float& status);
    float getTemperatureStep();

    void setBlowerStep(const VR_INT32& status);
    VR_INT32 getBlowerStep();

    void setMinTemperature(const VR_INT32& status);
    VR_INT32 getMinTemperature();

    void setMaxTemperature(const VR_INT32& status);
    VR_INT32 getMaxTemperature();

    void setAirconAuto(const bool& status);
    bool getAirconAuto();

    void setACMode(const bool& status);
    bool getACMode();

    void setInternalCycle(const bool& status);
    bool getInternalCycle();

    void setExternalCycle(const bool& status);
    bool getExternalCycle();

    void setTrontWindowDefog(const bool& status);
    bool getTrontWindowDefog();

    void setRearWindowDefog(const bool& status);
    bool getRearWindowDefog();

    void setTempDriver(const Vr_Aircon_Temp& status);
    Vr_Aircon_Temp& getTempDriver();

    void setTempPassage(const Vr_Aircon_Temp& status);
    Vr_Aircon_Temp& getTempPassage();

    void setTempRearLeft(const Vr_Aircon_Temp& status);
    Vr_Aircon_Temp& getTempRearLeft();

    void setTempRearRight(const Vr_Aircon_Temp& status);
    Vr_Aircon_Temp& getTempRearRight();

    void setFront(const VR_INT32& status);
    VR_INT32 getFront();

    void setRear(const VR_INT32& status);
    VR_INT32 getRear();

    void setOutletDriver(const VR_INT32& status);
    VR_INT32 getOutletDriver();

    void setOutletPassage(const VR_INT32& status);
    VR_INT32 getOutletPassage();

    void setOutletRearLeft(const VR_INT32& status);
    VR_INT32 getOutletRearLeft();

    void setOutletRearRight(const VR_INT32& status);
    VR_INT32 getOutletRearRight();

    void setSunroof_State(const VR_Sunroof_State &Sunroof_State);
    VR_Sunroof_State getSunroof_State() const;

    void setCurSpeed(const VR_INT32 &CurSpeed);
    VR_INT32 getCurSpeed() const;

    void setFuelWarning(const bool &FuelWarning);
    bool getFuelWarning() const;

    bool RenderVehicleFanStatus(const int& status, VR_String& str);
    bool RenderVehicleSunroofStatus(const int& status, VR_String& str);

    void setMaintainInfo(const navi::VoiceRecog::VrMaintainInfo &MaintainInfo);
    navi::VoiceRecog::VrMaintainInfo getMaintainInfo() const;

    void setDrivingRange(const VR_INT32& status);
    VR_INT32 getDrivingRange() const;

    void  setFuelAverage(const float& status);
    float getFuelAverage() const;

    void  setTotalMileage(const float& status);
    float getTotalMileage() const;

    void  setWindowDriver(const VR_INT32& status);
    VR_INT32 getWindowDriver() const;

    void  setWindowPassager(const VR_INT32& status);
    VR_INT32 getWindowPassager() const;

    void  setWindowRearLeft(const VR_INT32& status);
    VR_INT32 getWindowRearLeft() const;

    void  setWindowRearRight(const VR_INT32& status);
    VR_INT32 getWindowRearRight() const;

private:
    VR_VehicleInfoHelper(const VR_VehicleInfoHelper& other);
    VR_VehicleInfoHelper& operator =(const VR_VehicleInfoHelper& other);

private:
    static VR_VehicleInfoHelper* m_instance;
    bool  m_ACMode;                  // A / C 模式
    bool  m_airconAuto;              // 空调自动打开
    bool  m_internalCycle;           // 空调内循环
    bool  m_externalCycle;           // 空调外循环
    bool  m_frontWindowDefog;        // 前挡风玻璃除雾
    bool  m_rearWindowDefog;         // 后挡风玻璃除雾
    float m_temperatureStep;         // 温度调节步进
    VR_INT32 m_rear;                 // 后席空调风量
    VR_INT32 m_front;                // 前席空调风量
    VR_INT32 m_outlet_driver;        // 驾驶员空调风向模式
    VR_INT32 m_outlet_passage;       // 副驾驶空调风向模式
    VR_INT32 m_outlet_rearLeft;      // 后席左侧空调模式
    VR_INT32 m_outlet_rearRight;     // 后席右侧空调模式
    VR_INT32 m_airconType;           // 空调类型
    VR_INT32 m_temperatureUnit;      // 温度单位
    VR_INT32 m_frontMaxBlower;       // 前排最大风量等级
    VR_INT32 m_rearMaxBlower;        // 后排最大风量等级
    VR_INT32 m_blowerStep;           // 风量调节步进
    VR_INT32 m_minTemperature;       // 调节最低温度
    VR_INT32 m_maxTemperature;       // 调节最高温度
    Vr_Aircon_Temp m_temp_driver;    // 驾驶员温度
    Vr_Aircon_Temp m_temp_passage;   // 副驾驶温度
    Vr_Aircon_Temp m_temp_rearLeft;  // 后席左侧温度
    Vr_Aircon_Temp m_temp_rearRight; // 后席右侧温度

    // Vehicle Info
    VR_Sunroof_State m_Sunroof_State; // 天窗状态
    VR_INT32         m_CurSpeed;      // 当前速度
    bool             m_FuelWarning;   // 燃油不足警告
    navi::VoiceRecog::VrMaintainInfo   m_MaintainInfo;  // 保养提醒
    VR_INT32    m_driving_range;      // 续时里程
    bool        m_front_fog_lights;   // 前雾灯状态
    bool        m_rear_fog_lights;    // 后雾灯状态
    bool        m_high_beam;          // 近光灯状态
    float       m_fuel_average;       // 平均油耗
    float       m_totalmileage;       // 总里程
    VR_INT32    m_window_driver;      // 驾驶座车窗
    VR_INT32    m_window_passager;    // 副驾驶座车窗
    VR_INT32    m_window_rear_left;   // 后席左车窗
    VR_INT32    m_window_rear_right;  // 后席右车窗
};

#endif // VR_VEHICLEINFOHELPER_H
/* EOF */
