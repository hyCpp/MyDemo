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
 * @file VR_RadioXMLFormat.h
 * @brief Declaration file of class VR_RadioXMLFormat.
 *
 * This file includes the declaration of class VR_RadioXMLFormat.
 *
 * @attention used for C++ only.
 */

#ifndef VR_RADIO_XML_FORMAT_H
#define VR_RADIO_XML_FORMAT_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

namespace navi
{
namespace dataprovider
{

#define VR_RADIO_INIT_XML            "<grammar_new agent=\"media\">" \
                                         "<category name=\"hdsubchannel\">" \
                                             "<formal id=\"1\" name=\"HD1\" intval=\"1\">" \
                                                 "<alias name=\"HD 1\"/>" \
                                             "</formal>" \
                                             "<formal id=\"2\" name=\"HD2\" intval=\"2\">" \
                                                 "<alias name=\"HD 2\"/>" \
                                             "</formal>" \
                                             "<formal id=\"3\" name=\"HD3\" intval=\"3\">" \
                                                 "<alias name=\"HD 3\"/>" \
                                             "</formal>" \
                                             "<formal id=\"4\" name=\"HD4\" intval=\"4\">" \
                                                 "<alias name=\"HD 4\"/>" \
                                             "</formal>" \
                                             "<formal id=\"5\" name=\"HD5\" intval=\"5\">" \
                                                 "<alias name=\"HD 5\"/>" \
                                             "</formal>" \
                                             "<formal id=\"6\" name=\"HD6\" intval=\"6\">" \
                                                 "<alias name=\"HD 6\"/>" \
                                             "</formal>" \
                                             "<formal id=\"7\" name=\"HD7\" intval=\"7\">" \
                                                 "<alias name=\"HD 7\"/>" \
                                             "</formal>" \
                                             "<formal id=\"8\" name=\"HD8\" intval=\"8\">" \
                                                 "<alias name=\"HD 8\"/>" \
                                             "</formal>" \
                                           "</category>" \
                                       "</grammar_new>"

#define VR_RADIO_NEW_XML           "<grammar_new agent=\"media\"></grammar_new>"

#define VR_RADIO_AGENT_FIELD_PHRASE          "grammar_new"
#define VR_RADIO_CATEGORY_FIELD_PHRASE       "category"
#define VR_RADIO_CATEGORY_ATTR               "name"
#define VR_RADIO_FORMAL_FIELD_PHRASE         "formal"
#define VR_RADIO_FORMAL_ATTR_ID              "id"
#define VR_RADIO_FORMAL_ATTR_NAME            "name"
#define VR_RADIO_FORMAL_ATTR_INTVAL          "intval"
#define VR_RADIO_ALIAS_FIELD_PHRASE          "alias"
#define VR_RADIO_ALIAS_ATTR_NAME             "name"
#define VR_RADIO_ALIAS_ATTR_PRON             "pron"

#define VR_RADIO_CATEGORY_NAME_FMGENRE               "fmgenre"
#define VR_RADIO_CATEGORY_NAME_SATCHANNELNAME        "satchannelname"
#define VR_RADIO_CATEGORY_NAME_SATCHANNELNUMBER      "satchannelnumber"
#define VR_RADIO_CATEGORY_NAME_SATGENRE              "satgenre"
#define VR_RADIO_CATEGORY_NAME_FMSTATION             "fmstation"
#define VR_RADIO_CATEGORY_NAME_DABSTATION            "dabstation"

} // dataprovider
} // navi

#endif /* VR_RADIO_XML_FORMAT_H */
/* EOF */
