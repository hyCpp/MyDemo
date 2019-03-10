TEMPLATE = lib
TARGET = voicerecog-navi
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11

DEFINES += NPCOMMONLIB_LIBRARY COMP_OPT_PCORE _LINUX TARGET_PRODUCT_DCU TARGET_PRODUCT_MEU HAVE_NUTSHELL_OS

INCLUDEPATH += $$PWD/../../../../../../system/core/include
INCLUDEPATH += $$PWD/../../../../../../system/evip/include
INCLUDEPATH += $$PWD/../../../../../../platform/policy/AVManager/include
INCLUDEPATH += $$PWD/../../../../../../platform/service/SettingService/include
INCLUDEPATH += $$PWD/../../../../../../platform/service/MediaProvider/include
INCLUDEPATH += $$PWD/../../../../../../platform/service/AudioService/include
INCLUDEPATH += $$PWD/../../../../../../framework/service/media/MediaIpod/globalID/include
INCLUDEPATH += $$PWD/../../../../../../framework/service/Telematics/GBook/include
INCLUDEPATH += $$PWD/../../../../../../framework/service/Telematics/GBook/common
INCLUDEPATH += $$PWD/../../../../../../framework/service/Telematics/GViewer/include
INCLUDEPATH += $$PWD/../../../../../../framework/service/navi/src/base/eventsys/include
INCLUDEPATH += $$PWD/../../../../../../framework/api/idl-generated/include
INCLUDEPATH += $$PWD/../../../../../../platform/base/ContentProvider/include
INCLUDEPATH += $$PWD/../../../../../../platform/service/VehicleServer/include
INCLUDEPATH += $$PWD/../../../../../../platform/base/AppFramework/include
INCLUDEPATH += $$PWD/../../../../../../platform/service/SystemServer/include
INCLUDEPATH += $$PWD/../../../../../../framework/base/CompetePlayer/include
INCLUDEPATH += $$PWD/../../../../../../framework/base/AppFramework/include
INCLUDEPATH += $$PWD/../../../../../../framework/navi/base/dataengine/DspLib/publicinc
INCLUDEPATH += $$PWD/../../../../../../platform/base/ContentProvider/include
INCLUDEPATH += $$PWD/../../../../../../platform/base/DBHelper/include
INCLUDEPATH += $$PWD/../../../../../../platform/api/AppFramework/include
INCLUDEPATH += $$PWD/../../../../../../platform/service/MediaProvider/MediaDataProvider
INCLUDEPATH += $$PWD/../../../../../../framework/service/InstallManager/include
INCLUDEPATH += $$PWD/../../../../../../framework/service/BtAppService/include
INCLUDEPATH += $$PWD/../../../../../../framework/service/ServiceDB/include
INCLUDEPATH += $$PWD/../../../../../../framework/base/NGraphics/include
INCLUDEPATH += $$PWD/../../../../../../externals/protobuf/src
INCLUDEPATH += $$PWD/../../../../../../externals/rapidjson/include
INCLUDEPATH += $$PWD/../../../../../../externals/libcurl/include
INCLUDEPATH += $$PWD/../../../../../../externals/sqlite/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/paris/obj/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/michelle2/obj/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/generic/obj/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/0t/obj/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/e2/obj/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/dcue2/obj/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/meum2w/obj/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/m3muifa/obj/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/m2w/obj/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/m2n/obj/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/h3/obj/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/m3muifa/obj/include
INCLUDEPATH += $$PWD/../../../../../../out/target/product/m3muifa/obj/include/navi/Voice/VoiceRecog
INCLUDEPATH += $$PWD/../../../../externals/boost
INCLUDEPATH += $$PWD/../../../../externals/tinyxpath/publicinc
INCLUDEPATH += $$PWD/../../../../externals/rapidjson/include
INCLUDEPATH += $$PWD/../../../../base/protofiles
INCLUDEPATH += $$PWD/../../../../base/commonlib/commonlib/include
INCLUDEPATH += $$PWD/../../../../base/commonlib/aplcommonlib/include
INCLUDEPATH += $$PWD/../../../../base/commonlib/magiclog/include
INCLUDEPATH += $$PWD/../../../../base/navicommon/include
INCLUDEPATH += $$PWD/../../../../base/eventsys/include
INCLUDEPATH += $$PWD/../../../base/w3lib/PNetLib/publicinc
INCLUDEPATH += $$PWD/../../../base/w3lib/W3Lib/publicinc
INCLUDEPATH += $$PWD/../../../base/w3lib/DspLib/publicinc
INCLUDEPATH += $$PWD/../../../base/memorytracker/include
INCLUDEPATH += $$PWD/../../voicecomm/publicinc
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/publicinc
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/include
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/utility
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/avcunit
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/avcunit/src
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/avcunit/include
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/eventmanger/include
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/eventmanger/tutorial/include
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/include
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src/command
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src/command/include
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src/command/VrAgent
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src/command/VrUnit
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src/avclanobs
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src/dataprovider
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src/dataprovider/include
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src/task
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src/task/VrAgent
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src/task/VrUnit
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogengine/publicinc
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src/common/VrUnit
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogmanger/src/common/VrAgent
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/configure/include
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/utility/include
INCLUDEPATH += $$PWD/../../../../../../../platform/service/SystemServer/include
INCLUDEPATH += $$PWD/../../../../../../../platform/base/DBHelper/include
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogengine/suntec/include
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/asr/publicinc
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/dialogengine/suntec/dataaccessor/include
INCLUDEPATH += $$PWD/../../../externals/uscxml
INCLUDEPATH += $$PWD/../../../externals/uscxml/uscxml/libsource/arabica/include
INCLUDEPATH += $$PWD/../../../externals/uscxml/uscxml/src/evws
INCLUDEPATH += $$PWD/../../../externals/uscxml/uscxml/plugins
INCLUDEPATH += $$PWD/../../../externals/uscxml/uscxml/src/jsmn
INCLUDEPATH += $$PWD/../../../../../../../vendor/renesas/expat/M2N/usr/include
INCLUDEPATH += $$PWD/../../../../externals/pugixml
INCLUDEPATH += $$PWD/../../../../base/eventsys/NaviEventFactory/include
INCLUDEPATH += $$PWD/../../../../base/memorytracker/include
INCLUDEPATH += $$PWD/../../../../../../framework/service/DrivingAssistant/drvinfoservice/include

SOURCES += \
    ../*.cpp \
    ../voicerecoglib/*.cpp \
    ../voicerecoglib/avcunit/src/*.cpp \
    ../voicerecoglib/eventmanger/src/*.cpp \
    ../voicerecoglib/eventmanger/tutorial/*.cpp \
    ../voicerecoglib/dialogmanger/src/*.cpp \
    ../voicerecoglib/dialogmanger/src/command/*.cpp \
    ../voicerecoglib/dialogmanger/src/command/VrAgent/*.cpp \
    ../voicerecoglib/dialogmanger/src/command/VrUnit/*.cpp \
    ../voicerecoglib/dialogmanger/src/action/*.cpp \
    ../voicerecoglib/dialogmanger/src/action/VrUnit/*.cpp \
    ../voicerecoglib/dialogmanger/src/action/VrAgent/*.cpp \
    ../voicerecoglib/dialogmanger/src/task/*.cpp \
    ../voicerecoglib/dialogmanger/src/task/VrUnit/*.cpp \
    ../voicerecoglib/dialogmanger/src/task/VrAgent/*.cpp \
    ../voicerecoglib/dialogmanger/src/dataprovider/*.cpp \
    ../voicerecoglib/dialogmanger/src/avclanobs/*.cpp \
    ../voicerecoglib/dialogmanger/src/avclanobs/VrUnit/*.cpp \
    ../voicerecoglib/dialogmanger/src/avclanobs/VrAgent/*.cpp \
    ../voicerecoglib/dialogmanger/src/action/VR_ActionVehicleInfoAction.cpp \
    ../voicerecoglib/dialogengine/VR_DialogEngineFactory.cpp \
    ../voicerecoglib/dialogmanger/src/VR_UIStartingCondition.cpp \
    ../voicerecoglib/eventmanger/src/VR_SettingListener.cpp \
    ../voicerecoglib/configure/src/*.cpp \
    ../../../externals/pugixml/*.cpp \
    ../voicerecoglib/utility/Regex/slre.c \
    ../voicerecoglib/utility/VR_Macro.cpp \
    ../voicerecoglib/utility/VR_ContentProvider.cpp \
    ../voicerecoglib/dialogmanger/src/avclanobs/VrAgent/*.cpp \
    ../voicerecoglib/dialogmanger/src/avclanobs/VrUnit/*.cpp \
    ../voicerecoglib/dialogengine/suntec/src/*.cpp \
    ../voicerecoglib/asr/impl/*.cpp \
    ../voicerecoglib/dialogengine/suntec/src/*.cpp \
    ../voicerecoglib/dialogengine/suntec/dataaccessor/src/*.cpp \
    ../../../externals/uscxml/uscxml/plugins/datamodel/xpath/XPathDataModel.cpp \
    ../../../externals/uscxml/uscxml/plugins/datamodel/null/NULLDataModel.cpp\
    ../../../externals/uscxml/uscxml/plugins/invoker/vr/VRInvoker.cpp\
    ../../../externals/uscxml/uscxml/plugins/invoker/scxml/USCXMLInvoker.cpp\
    ../../../externals/uscxml/uscxml/plugins/Pluma/Dir.cpp\
    ../../../externals/uscxml/uscxml/plugins/Pluma/DLibrary.cpp\
    ../../../externals/uscxml/uscxml/plugins/Pluma/Host.cpp\
    ../../../externals/uscxml/uscxml/plugins/Pluma/PluginManager.cpp\
    ../../../externals/uscxml/uscxml/plugins/Pluma/Provider.cpp\
    ../../../externals/uscxml/uscxml/plugins/Plugins.cpp\
    ../../../externals/uscxml/uscxml/util/Base64.c\
    ../../../externals/uscxml/uscxml/util/MD5.c\
    ../../../externals/uscxml/uscxml/util/SHA1.c\
    ../../../externals/uscxml/uscxml/util/Trie.cpp\
    ../../../externals/uscxml/uscxml/DOMUtils.cpp\
    ../../../externals/uscxml/uscxml/Factory.cpp\
    ../../../externals/uscxml/uscxml/Interpreter.cpp\
    ../../../externals/uscxml/uscxml/Message.cpp\
    ../../../externals/uscxml/uscxml/UUID.cpp\
    ../../../externals/uscxml/uscxml/URL.cpp\
    ../../../externals/uscxml/uscxml/src/evws/evws.c\
    ../../../externals/uscxml/uscxml/src/jsmn/jsmn.c\
    ../../../externals/uscxml/uscxml/concurrency/DelayedEventQueue.cpp\
    ../../../externals/uscxml/uscxml/concurrency/EventBase.cpp\
    ../../../externals/uscxml/uscxml/concurrency/tinythread.cpp\
    ../../../externals/uscxml/uscxml/interpreter/InterpreterDraft6.cpp\
    ../../../externals/uscxml/uscxml/interpreter/InterpreterRC.cpp\
    ../../../externals/uscxml/uscxml/messages/Blob.cpp\
    ../../../externals/uscxml/uscxml/messages/Data.cpp\
    ../../../externals/uscxml/uscxml/messages/Event.cpp\
    ../../../externals/uscxml/uscxml/messages/InvokeRequest.cpp\
    ../../../externals/uscxml/uscxml/messages/MMIMessages.cpp\
    ../../../externals/uscxml/uscxml/messages/SendRequest.cpp\
    ../../../externals/uscxml/uscxml/plugins/ioprocessor/scxml/SCXMLIOProcessor.cpp\
    ../../../externals/uscxml/uscxml/server/HTTPServer.cpp\
    ../../../externals/uscxml/uscxml/server/InterpreterServlet.cpp\
    ../../../externals/uscxml/uscxml/server/Socket.cpp\
    ../../../externals/uscxml/uscxml/plugins/invoker/http/HTTPServletInvoker.cpp\
    ../../../externals/uscxml/uscxml/plugins/ioprocessor/basichttp/BasicHTTPIOProcessor.cpp \
    ../voicerecoglib/dialogmanger/src/VR_PersonDataEventHandler.cpp \
    ../voicerecoglib/dialogmanger/src/VR_DebugRecorderEventHandler.cpp \
    ../voicerecoglib/dialogengine/voicebox/src/VR_VoiceBoxEngine.cpp \
    ../voicerecoglib/dialogengine/voicebox/src/VR_VoiceBoxDialogEngine.cpp \
    ../voicerecoglib/dialogmanger/src/VR_PhoneCallResultHandler.cpp \
    ../voicerecoglib/dialogmanger/src/action/VR_EngineStatusAction.cpp \
    ../voicerecoglib/dialogmanger/src/command/VR_FileTransport.cpp \
    ../voicerecoglib/dialogmanger/src/action/VR_GBookCenterUrl.cpp \
    ../voicerecoglib/dialogmanger/src/command/VrAgent/VR_RequestWebSearchEngine_VrAgent.cpp \
    ../voicerecoglib/dialogmanger/src/task/VR_StorageEventHandler.cpp \
    ../voicerecoglib/dialogmanger/src/task/VrAgent/VR_DebugRecorderEventHandler_VrAgent.cpp \
    ../voicerecoglib/dialogmanger/src/VR_DebugRecorderEventHandler_Null.cpp \
    ../voicerecoglib/dialogmanger/src/VR_DialogManger_Null.cpp \
    ../voicerecoglib/dialogmanger/src/task/VrUnit/VR_StorageEventHandler.cpp \
    ../voicerecoglib/dialogmanger/src/dataprovider/VR_MusicGracenoteListener.cpp \
    ../voicerecoglib/dialogengine/voicebox/src/VR_VoiceBoxCatalogAudio.cpp \
    ../voicerecoglib/dialogengine/voicebox/src/VR_VoiceBoxCatalogManager.cpp \
    ../voicerecoglib/dialogmanger/src/command/VrUnit/VR_NaviPointListReceiver.cpp \
    ../voicerecoglib/dialogmanger/src/dataprovider/VR_MusicDBHelper_FileTemp.cpp \
    ../voicerecoglib/dialogmanger/src/dataprovider/VR_MusicDBHelper_FileVBT.cpp \
    ../voicerecoglib/dialogmanger/src/task/VR_SpotlightSearchTask.cpp \
    ../voicerecoglib/dialogmanger/src/command/VR_MessageSpotLightSearch.cpp \
    ../voicerecoglib/dialogmanger/testphoneinfo/VR_NCPhoneInfoDatabaseHelper.cpp \
    ../voicerecoglib/dialogmanger/testphoneinfo/VR_NCPhoneInfoOperator.cpp \
    ../voicerecoglib/dialogmanger/testphoneinfo/VR_NCPhoneInfoProvider.cpp \
    ../voicerecoglib/dialogmanger/testphoneinfo/VR_NCTestPhoneInfoProcess.cpp \
    ../voicerecoglib/dialogmanger/src/MMVR_CommandSet.cpp \
    ../voicerecoglib/dialogmanger/src/task/VR_MusicSpotLightUpdater.cpp \
    ../voicerecoglib/dialogmanger/src/task/VR_NIMusicShareProxy.cpp \
    ../voicerecoglib/dialogmanger/src/task/VR_NIMusicShareReplier.cpp \
    ../voicerecoglib/dialogmanger/src/task/VR_NIMusicNodeItem.cpp \
    ../voicerecoglib/dialogmanger/src/VR_XmlStrParse.cpp \
    ../voicerecoglib/dialogmanger/src/task/VR_NaviStatusManager.cpp \
    ../voicerecoglib/dialogmanger/src/dataprovider/VR_MusicDBHelper_Engine.cpp \
    ../voicerecoglib/dialogmanger/src/command/VR_RespCategorySearchInfo.cpp \
    ../voicerecoglib/dialogmanger/src/task/VR_VehicleInfoManager.cpp \
    ../voicerecoglib/dialogmanger/src/action/VR_NuanceActionAdaptor.cpp \
    ../voicerecoglib/dialogmanger/src/task/VR_TunerStatusManager.cpp \
    ../voicerecoglib/dialogmanger/src/VR_UserManager.cpp \
    ../voicerecoglib/dialogmanger/src/VR_CMResourceHelper.cpp \
    ../voicerecoglib/dialogmanger/src/VR_CMInfoManager.cpp \
    ../voicerecoglib/dialogmanger/src/task/VR_NCDrvInfoProxy.cpp \
    ../voicerecoglib/dialogmanger/src/task/VR_NCDrvInfoReplier.cpp

HEADERS += \
    ../*.h \
    ../voicerecoglib/*.h \
    ../voicerecoglib/avcunit/include/*. \
    ../voicerecoglib/include/*.h \
    ../voicerecoglib/eventmanger/include/*.h \
    ../voicerecoglib/eventmanger/tutorial/include/*.h \
    ../voicerecoglib/dialogmanger/include/*.h \
    ../voicerecoglib/eventmanger/src/*.h \
    ../voicerecoglib/dialogmanger/src/*.h \
    ../voicerecoglib/dialogmanger/src/avclanobs/*.h \
    ../voicerecoglib/dialogmanger/src/avclanobs/VrUnit/*.h \
    ../voicerecoglib/dialogmanger/src/avclanobs/VrAgent/*.h \
    ../voicerecoglib/dialogmanger/src/command/*.h \
    ../voicerecoglib/dialogmanger/src/command/VrAgent/*.h \
    ../voicerecoglib/dialogmanger/src/command/VrUnit/*.h \
    ../voicerecoglib/dialogmanger/src/command/include/*.h \
    ../voicerecoglib/dialogmanger/src/task/*.h \
    ../voicerecoglib/dialogmanger/src/task/VrUnit/*.h \
    ../voicerecoglib/dialogmanger/src/task/VrAgent/*.h \
    ../voicerecoglib/dialogmanger/src/action/*.h \
    ../voicerecoglib/dialogmanger/src/action/VrUnit/*.h \
    ../voicerecoglib/dialogmanger/src/action/VrAgent/*.h \
    ../voicerecoglib/dialogmanger/src/dataprovider/*.h \
    ../voicerecoglib/dialogmanger/src/dataprovider/include/*.h \
    ../voicerecoglib/dialogmanger/src/VR_UIStartingCondition.h \
    ../voicerecoglib/configure/include/VR_ConfigureIF.h \
    ../voicerecoglib/configure/include/*.h \
    ../voicerecoglib/configure/include/VR_ConfigureDEF.h \
    ../voicerecoglib/eventmanger/include/VR_SettingListener.h \
    ../../../externals/pugixml/*.hpp \
    ../voicerecoglib/utility/Regex/slre.h \
    ../voicerecoglib/utility/VR_Macro.h \
    ../voicerecoglib/utility/VR_Indentifier.hpp \
    ../voicerecoglib/utility/include/VR_ContentProvider.h \
    ../voicerecoglib/dialogmanger/include/*.hpp \
    ../voicerecoglib/dialogmanger/src/dataprovider/*.h \
    ../voicerecoglib/dialogmanger/include/*.h \
    ../voicerecoglib/dialogmanger/src/avclanobs/VrAgent/*.h \
    ../voicerecoglib/dialogmanger/src/avclanobs/VrUnit/*.h \
    ../voicerecoglib/avcunit/include/*.h \
    ../voicerecoglib/dialogengine/suntec/include/*.h \
    ../voicerecoglib/asr/impl/*.h \
    ../voicerecoglib/dialogengine/suntec/dataaccessor/include/*.h \
    ../voicerecoglib/dialogmanger/src/VR_PersonDataEventHandler.h \
    ../voicerecoglib/dialogmanger/src/VR_DebugRecorderEventHandler.h \
    ../voicerecoglib/dialogmanger/src/VR_PhoneCallResultHandler.h \
    ../voicerecoglib/dialogmanger/src/action/VR_EngineStatusAction.h \
    ../voicerecoglib/dialogmanger/src/dataprovider/VR_DP_OB_Action_FileTrans.h \
    ../voicerecoglib/dialogmanger/src/command/VR_FileTransport.h \
    ../voicerecoglib/dialogmanger/src/dataprovider/VR_FileTransferContext.h \
    ../voicerecoglib/dialogmanger/src/dataprovider/VR_FileTransferState.h \
    ../voicerecoglib/dialogmanger/src/dataprovider/VR_FileTransferStateFactory.h \
    ../voicerecoglib/dialogmanger/src/action/VR_GBookCenterUrl.h \
    ../voicerecoglib/dialogmanger/src/command/VrAgent/VR_RequestWebSearchEngine_VrAgent.h \
    ../voicerecoglib/dialogmanger/src/VR_StorageEventHandler.h \
    ../voicerecoglib/dialogmanger/src/task/VrAgent/VR_DebugRecorderEventHandler_VrAgent.h \
    ../voicerecoglib/dialogmanger/src/VR_DebugRecorderEventHandler_Null.h \
    ../voicerecoglib/dialogmanger/src/VR_DialogManger_Null.h \
    ../voicerecoglib/dialogmanger/src/task/VrUnit/VR_StorageEventHandler.h \
    ../voicerecoglib/dialogmanger/src/command/VrUnit/VR_NaviPointListReceiver.h \
    ../voicerecoglib/dialogmanger/src/task/VR_SpotlightSearchTask.h \
    ../voicerecoglib/dialogmanger/src/command/VR_MessageSpotLightSearch.h \
    ../voicerecoglib/dialogmanger/testphoneinfo/README.md \
    ../voicerecoglib/dialogmanger/testphoneinfo/VR_NCPhoneInfoDatabaseHelper.h \
    ../voicerecoglib/dialogmanger/testphoneinfo/VR_NCPhoneInfoOperator.h \
    ../voicerecoglib/dialogmanger/testphoneinfo/VR_NCPhoneInfoProvider.h \
    ../voicerecoglib/dialogmanger/testphoneinfo/VR_NCTestPhoneInfoProcess.h \
    ../voicerecoglib/dialogmanger/include/MMVR_CommandSet.h \
    ../voicerecoglib/dialogmanger/src/task/VR_MusicSpotLightUpdater.h \
    ../voicerecoglib/dialogmanger/src/task/VR_NIMusicShareProxy.h \
    ../voicerecoglib/dialogmanger/src/task/VR_NIMusicShareReplier.h \
    ../voicerecoglib/dialogmanger/src/task/VR_NIMusicNodeItem.h \
    ../voicerecoglib/dialogmanger/include/VR_XmlStrParse.h \
    ../voicerecoglib/dialogmanger/src/task/VR_NaviStatusManager.h \
    ../voicerecoglib/dialogmanger/src/dataprovider/VR_MusicDBHelper_Engine.h \
    ../voicerecoglib/dialogmanger/src/command/VR_RespCategorySearchInfo.h \
    ../voicerecoglib/dialogmanger/src/common/m3muifa/VR_Common_DeviceDef.h \
    ../voicerecoglib/dialogmanger/src/common/m3wukong/VR_Common_DeviceDef.h \
    ../voicerecoglib/dialogmanger/src/task/VR_VehicleInfoHelper.h \
    ../voicerecoglib/dialogmanger/src/task/VR_VehicleInfoManager.h \
    ../voicerecoglib/dialogmanger/src/action/VR_NuanceActionAdaptor.h \
    ../voicerecoglib/dialogmanger/include/VR_XmlStrParseDEF.h \
    ../voicerecoglib/dialogmanger/src/task/VR_TunerStatusManager.h \
    ../voicerecoglib/dialogmanger/VR_UserManager.h \
    ../voicerecoglib/dialogmanger/src/VR_CMResourceHelper.h \
    ../voicerecoglib/dialogmanger/src/VR_CMInfoManager.h \
    ../voicerecoglib/dialogmanger/src/task/VR_NCDrvInfoReplier.h

OTHER_FILES += \
    ../voicerecog-navi.cm \
    ../voicerecoglib/eventmanger/vr_eventsys-navi.cm \
    ../voicerecoglib/dialogmanger/vr_dialog-navi.cm \
    ../voicerecoglib/dialogmanger/src/vr_dataprovider-navi.cm \
    ../voicerecoglib/configure/vr_configure-navi.cm \
    ../../../externals/pugixml/*.cm \
    ../voicerecoglib/avcunit/vr_avcunit-navi.cm \
    ../voicerecoglib/dialogengine/vr_dialogengine-navi.cm \
    ../voicerecoglib/dialogengine/voicebox/audiostream/vr_audiostream_vbt-navi.cm \
    ../voicerecoglib/dialogengine/bargein/vr_dialogengine_bargein-navi.cm \
    ../voicerecoglib/dialogengine/suntec/vr_dialogengine_suntec_iflytek-navi.cm \
    ../voicerecoglib/dialogengine/suntec/vr_dialogengine_suntec_nuance-navi.cm \
    ../voicerecoglib/dialogengine/suntec/vr_dialogengine_suntec_baidu-navi.cm \
    ../voicerecoglib/stub/vr_stub-navi.cm \
    ../voicerecoglib/dialogmanger/testphoneinfo/TestPhoneInfo.cm \
    ../voicerecoglib/dialogmanger/testphoneinfo/TestPhoneInfo.mk

DISTFILES += \
    batch_convert.sh \
    make.sh \
    LicenseTemplates
