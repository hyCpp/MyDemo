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
 * @file  VR_VehicleMarodef.h
 * @brief Declaration class of VR_VehicleMacordef
 */
#ifndef VR_VEHICLEMACRODEF_H
#define VR_VEHICLEMACRODEF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (Use a .cpp suffix)
#endif


#ifndef VR_HVAC_GETFUNC_DEF
#define VR_HVAC_GETFUNC_DEF(GET_CALLBACK, SEND_CALLBACK) \
    VehicleData data /*= {0}*/; \
    data.actionId = nActionId;  \
    data.pGetCallFunc = &VR_VehicleInfoProxy::GET_CALLBACK; \
    data.pSendCallFunc = &VR_VehicleInfoProxy::SEND_CALLBACK; \
    \
    PushGetData(data)
#endif

#ifndef VR_HVAC_GETCALLBACK_DEF
#define VR_HVAC_GETCALLBACK_DEF(STRPROPERTY)  \
    GetPropertyAsync(STRPROPERTY.c_str())
#endif

#ifndef VR_HVAC_SETFUNC_DEF
#define VR_HVAC_SETFUNC_DEF(SET_CALLBACK, SEND_CALLBACKON, SEND_CALLBACKOFF, FUNC_VALUE) \
        VehicleData data /*= {0}*/;  \
        data.actionId = nActionId; \
        data.pSetCallFunc = &VR_VehicleInfoProxy::SET_CALLBACK;  \
        if (FUNC_VALUE) { \
            data.pSendCallFunc = &VR_VehicleInfoProxy::SEND_CALLBACKON; \
        }   \
        else {     \
            data.pSendCallFunc = &VR_VehicleInfoProxy::SEND_CALLBACKOFF; \
        }   \
        nutshell::NCVariant  value(FUNC_VALUE);  \
        data.ncValue = value;   \
        \
        PushSetData(data)
#endif

#ifndef VR_HVAC_SETCALLBACK_DEF
#define VR_HVAC_SETCALLBACK_DEF(STRPROPERTY, NCVALUE)  \
    SetPropertyAsync(STRPROPERTY.c_str(), NCVALUE)
#endif

#ifndef VR_HVAC_SPEED_CASE
#define VR_HVAC_SPEED_CASE(SPEED_TYPE, SPEED_VAR, SPEED_VALUE)  \
    case (SPEED_TYPE) :  \
    {    \
        SPEED_VAR = SPEED_VALUE; \
    }    \
        break
#endif

#ifndef VR_HVAC_SPEED_END
#define VR_HVAC_SPEED_END  \
default :  \
    break
#endif

#ifndef VR_HVAC_BLOWER_CASE
#define VR_HVAC_BLOWER_CASE(BLOWER_TYPE, BLOWER_VAR, SPEED_MODE)   \
    case  (BLOWER_TYPE) :   \
    {   \
        BLOWER_VAR = SPEED_MODE;   \
    }  \
        break
#endif

#ifndef VR_HVAC_BLOWER_END
#define VR_HVAC_BLOWER_END   \
default  :   \
    break
#endif

#ifndef VR_HVAC_GET_FRONT
#define VR_HVAC_GET_FRONT(PDATA)  \
    VehicleData * PDATA = Front();   \
    if (NULL == PDATA) {  \
        return;  \
    }
#endif

#ifndef VR_HVAC_HANDLE_GETSTATUS
#define VR_HVAC_HANDLE_GETSTATUS(DM_ACTION_TYPE, DM_ACTION_RSP_TYPE, DM_ACTION_CLASS, \
                                  MUTABLE_ACTION_RESP_FUNC, SET_ACTION_FUNC)     \
    bool bStatus(false);   \
    ncValue.value(bStatus);   \
    VrActionFC2DM * pResultAction = new(MEM_Voice) VrActionFC2DM;    \
    if (NULL == pResultAction) {   \
        return;  \
    }  \
    pResultAction->set_id(pData->actionId);   \
    pResultAction->set_vr_dm_action_type(DM_ACTION_TYPE);   \
    VrAdditionalInfoRsp * pAdditionRsp = pResultAction->mutable_additional_info_rsp();   \
    if (NULL == pAdditionRsp) {    \
        return;    \
    }   \
    pAdditionRsp->set_response_id(DM_ACTION_RSP_TYPE);   \
    DM_ACTION_CLASS * pAdditionAction = pAdditionRsp->MUTABLE_ACTION_RESP_FUNC();   \
    if (pAdditionAction) {   \
        return ;  \
    }     \
    pAdditionAction->SET_ACTION_FUNC(bStatus);   \
    pResultAction->set_allocated_additional_info_rsp(pAdditionRsp);
#endif

#ifndef VR_HVAC_FUNC_FACTORY
#define VR_HVAC_FUNC_FACTORY(FUNCTION_NAME, ACTION_TYPE)  \
    void VR_VehicleInfoProxy::FUNCTION_NAME(bool bSuccess, const nutshell::NCVariant &ncValue) \
    {   \
        if (bSuccess) {   \
            VehicleData *pData = Front();   \
            if (NULL == pData) {    \
                return;  \
            }    \
                    \
            bool bStatus(false);   \
            ncValue.value(bStatus);  \
            VrActionFC2DM*  vrResultAction = new(MEM_Voice) VrActionFC2DM;   \
            if (NULL == vrResultAction) {     \
                return;   \
            }    \
            vrResultAction->set_id(pData->actionId);   \
            vrResultAction->set_vr_dm_action_type(ActionType_RECOG_RESULT_CONFIRM);   \
            VrRecogResultConfirm* vrResultConfirm = vrResultAction->mutable_recog_result_confirm();   \
            if (NULL == vrResultConfirm) {     \
                return;   \
            }   \
            vrResultConfirm->set_action_id(ACTION_TYPE);  \
            vrResultConfirm->set_processing_result(bStatus);  \
        }   \
        else {        \
        }        \
    }
#endif

#ifndef VR_HVAC_FUNC_FACTORY_BEGIN
#define VR_HVAC_FUNC_FACTORY_BEGIN
#endif

#ifndef VR_HVAC_FUNC_FACTORY_END
#define VR_HVAC_FUNC_FACTORY_END
#endif

#ifndef VR_HVAC_SEND_FUNC_CASE
#define VR_HVAC_SEND_FUNC_CASE(CASE_TYPE, SENDCALLBACK) \
    case  (CASE_TYPE) :   \
    {     \
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::SENDCALLBACK;  \
    }     \
        break
#endif

#ifndef VR_HVAC_SEND_FUNC_END
#define VR_HVAC_SEND_FUNC_END(SENDCALLBACK)    \
    default:  \
    {      \
        data.m_pSendCallFunc = &VR_VehicleInfoProxy::SENDCALLBACK;  \
    }    \
    break
#endif


#endif // VR_VEHICLEMACRODEF_H
/* EOF */
