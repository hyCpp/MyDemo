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
 * @file VR_CityListListener_VrAgent.h
 * @brief Declaration file of class VR_CityListListener_VrAgent.
 *
 * This file includes the declaration of class VR_CityListListener_VrAgent.
 *
 * @attention used for C++ only.
 */

#ifndef VR_CITYLISTLISTENER_VRAGENT_H
#define VR_CITYLISTLISTENER_VRAGENT_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#    include "VR_Macro.h"
#endif

#include <ncore/NCTypesDefine.h>
#include <ncore/NCString.h>
#include <ncore/NCUri.h>
#include <ncore/NCVariant.h>
#include "VR_CommandHandleIF.h"

namespace navi
{
namespace Traffic
{
    class WeatherCityListRes;
    class WeatherCityListUpdate;
}
}

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_CommandHandleIF);


namespace navi
{
namespace dataprovider
{
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_CityListFileBuilder);



/**
 * @brief The VR_CityListListener_VrAgent class
 *
 * class declartion
 */
class VR_CityListListener_VrAgent
{
public:
    ~VR_CityListListener_VrAgent();

    bool Initialize();
    void onNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant, const VR_ProtoMessageBase &pMsg);

    void SetDMHandle(spVR_CommandHandleIF spDMHandle);

    static VR_CityListListener_VrAgent* GetInstance();
    static VOID Dispose();
    static VR_String cityListFindNameFromId(int& cityId);

protected:
    std::string protoToBufferCityListRes(const navi::Traffic::WeatherCityListRes &pMsg);
    std::string protoToBufferCityListUpdate(const navi::Traffic::WeatherCityListUpdate &pMsg);

    void SendFileToMEU(const nutshell::NCCHAR* srcPath, const nutshell::NCCHAR* dstPath);
    void UpdataMapCityListRes(const navi::Traffic::WeatherCityListRes &pMsg);
    void UpdataMapCityListUpdate(const navi::Traffic::WeatherCityListUpdate &pMsg);

private:
    spVR_CommandHandleIF           m_spDMHandle;
    spVR_CityListFileBuilder       m_fileBuilder;

    VoiceMap<int, VR_String>::type m_mapCityList;

private:
    VR_CityListListener_VrAgent();
    VR_CityListListener_VrAgent(const VR_CityListListener_VrAgent&);
    VR_CityListListener_VrAgent& operator=(const VR_CityListListener_VrAgent&);
    static VR_CityListListener_VrAgent* m_instance;

};

}
}
#endif // VR_CITYLISTLISTENER_VRAGENT_H
/* EOF */






