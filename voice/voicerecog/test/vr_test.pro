TEMPLATE = lib
TARGET = voicerecog-navi-test
CONFIG -= qt

include($$PWD/../qtprj/vr.pro)

DEFINES += NPCOMMONLIB_LIBRARY COMP_OPT_PCORE _LINUX

INCLUDEPATH += $$PWD/../../../../../../../system/core/include
INCLUDEPATH += $$PWD/../../../../../../../framework/base/AppFramework/include
INCLUDEPATH += $$PWD/../../../../../../../framework/service/InstallManager/include
INCLUDEPATH += $$PWD/../../../../../../../framework/service/BtAppService/include
INCLUDEPATH += $$PWD/../../../../../../../framework/service/ServiceDB/include
INCLUDEPATH += $$PWD/../../../../../../../externals/protobuf/src
INCLUDEPATH += $$PWD/../../../../../../../externals/libcurl/include
INCLUDEPATH += $$PWD/../../../../../../../externals/sqlite/include
INCLUDEPATH += $$PWD/../../../../../../../out/target/product/paris/obj/include
INCLUDEPATH += $$PWD/../../../../../../../out/target/product/michelle2/obj/include

INCLUDEPATH += $$PWD/../../../externals/boost
INCLUDEPATH += $$PWD/../../../externals/tinyxpath/publicinc
INCLUDEPATH += $$PWD/../../../externals/rapidjson/include
INCLUDEPATH += $$PWD/../../../protofiles
INCLUDEPATH += $$PWD/../../../base/commonlib/commonlib/include
INCLUDEPATH += $$PWD/../../../base/commonlib/aplcommonlib/include
INCLUDEPATH += $$PWD/../../../base/commonlib/magiclog/include
INCLUDEPATH += $$PWD/../../../base/navicommon/include
INCLUDEPATH += $$PWD/../../../base/eventsys/include
INCLUDEPATH += $$PWD/../../../base/w3lib/PNetLib/publicinc
INCLUDEPATH += $$PWD/../../../base/w3lib/W3Lib/publicinc
INCLUDEPATH += $$PWD/../../../base/w3lib/DspLib/publicinc
INCLUDEPATH += $$PWD/../../voicecomm/publicinc
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/publicinc
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/include
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/sds
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/sds/Local
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/sds/Server
INCLUDEPATH += $$PWD/../../voicerecog/voicerecoglib/Platform
INCLUDEPATH += $$PWD/../../voicerecog/test/src/voicerecoglib/sds
INCLUDEPATH += $$PWD/../../../../../../../externals/googletest/include
INCLUDEPATH += $$PWD/../../../../../../../externals/googlemock/include

SOURCES += \
    src/*.cpp \
    src/voicerecoglib/*.cpp \
    src/voicerecoglib/include/*.cpp \
    src/voicerecoglib/Configure/*.cpp \
    src/voicerecoglib/sds/*.cpp \
    src/voicerecoglib/Utility/*.cpp \
    src/voicerecoglib/Utility/Regex/*.cpp \
    src/voicerecoglib/Platform/*.cpp \
    src/voicerecoglib/Platform/VrEngineKeda/*.cpp \
    src/voicerecoglib/Platform/VrEngineGoogle/*.cpp \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/*.cc \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/base/*.cc \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/asr/*.cc \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/audio/*.cc \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/ipc/*.cc \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/net/*.cc \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/base/*.cc \
    src/voicerecoglib/Platform/Contact/*.cpp \
    src/voicerecoglib/Platform/Music/*.cpp \
    src/voicerecoglib/Platform/App/*.cpp \
    src/voicerecoglib/sds/Server/*.cpp \
    src/voicerecoglib/sds/Local/*.cpp \
    src/voicerecoglib/sds/Local/VR_SDSStateLocalManager_test.cpp \
    src/voicerecoglib/sds/Local/VR_SDSHandle_test.cpp \
    src/voicerecoglib/sds/Local/VR_SDSStateLocal_test.cpp \
    src/voicerecoglib/sds/Local/VR_SDSStateMachineLocal_test.cpp \
    src/voicerecoglib/sds/VR_SDSConfigDEF_test.cpp \
    src/voicerecoglib/sds/VR_SDSConfigIF_test.cpp \
    src/voicerecoglib/sds/VR_SDSEvent_test.cpp \
    src/voicerecoglib/sds/VR_SDSListener_test.cpp \
    src/voicerecoglib/sds/VR_SDSFactory_test.cpp \
    src/voicerecoglib/sds/VR_SDSStateManager_test.cpp \
    src/voicerecoglib/sds/Server/VR_SDSDataParserModel_test.cpp \
    src/voicerecoglib/sds/Server/VR_SDSStateServer_test.cpp \
    src/voicerecoglib/sds/Server/VR_SDSStateServerBase_test.cpp \
    voicerecoglib/avcunit/*.cpp \
    voicerecoglib/dialogmanager/src/action/*.cpp \
    voicerecoglib/dialogmanager/src/action/VrUnit/*.cpp \
    voicerecoglib/dialogmanager/src/action/VrAgent/*.cpp \
    voicerecoglib/dialogmanager/src/dataprovider/*.cpp  \
    voicerecoglib/dialogmanager/src/task/*.cpp \
    voicerecoglib/dialogmanager/src/*.cpp \
    voicerecoglib/voicerecoglib_test.cpp \
    voicerecoglib/eventmanager/tutorial/*.cpp \
    voicerecoglib/eventmanager/src/*.cpp \
    voicerecoglib/dialogmanager/src/VR_Indentifier_Test.cpp \
    voicerecoglib/eventmanager/tutorial/VR_NCTutorialHelp.cpp \
    voicerecoglib/dialogmanager/src/dataprovider/VR_PhoneBookStateListener_Test.cpp \
    voicerecoglib/dialogmanager/src/dataprovider/VR_DP_OB_Action_Test.cpp

HEADERS += \
    src/*.h \
    src/voicerecoglib/*.h \
    src/voicerecoglib/include/*.h \
    src/voicerecoglib/Configure/*.h \
    src/voicerecoglib/sds/*.h \
    src/voicerecoglib/Utility/*.h \
    src/voicerecoglib/Utility/Regex/*.h \
    src/voicerecoglib/publicinc/*.h \
    src/voicerecoglib/Platform/*.h \
    src/voicerecoglib/Platform/VrEngineKeda/*.h \
    src/voicerecoglib/Platform/VrEngineGoogle/*.h \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/*.h \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/include/*.h \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/public/*.h \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/asr/*.h \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/base/*.h \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/audio/*.h \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/ipc/*.h \
    src/voicerecoglib/Platform/VrEngineGoogle/libsrengine/net/*.h \
    src/voicerecoglib/Platform/Contact/*.h \
    src/voicerecoglib/Platform/Music/*.h \
    src/voicerecoglib/Platform/App/*.h \
    src/voicerecoglib/sds/Server/*.h \
    src/voicerecoglib/sds/Local/*.h \
    voicerecoglib/dialogmanager/src/action/*.h \
    voicerecoglib/dialogmanager/src/action/VrAgent/*.h \
    voicerecoglib/dialogmanager/src/action/VrUnit/*.h \
    voicerecoglib/dialogmanager/src/dataprovider/*.h \
    voicerecoglib/dialogmanager/src/task/*.h \
    voicerecoglib/dialogmanager/src/*.h \
    voicerecoglib/dialogmanager/src/action/VR_EventSender_mock.h \
    voicerecoglib/eventmanager/tutorial/*.h \
    voicerecoglib/eventmanager/src/*.h \
    voicerecoglib/eventmanager/tutorial/VR_NCTutorialHelp.h

OTHER_FILES += \
    ../voicerecog-navi.cm \
    voicerecog-navi-test.cm \
    voicerecoglib/dialogmanager/voicerecog-dialogmanager-test.cm \
    voicerecoglib/dialogmanager/src/voicerecog-dataprovider-test.cm \
    voicerecoglib/eventmanager/voicerecog-eventmanager-test.cm
