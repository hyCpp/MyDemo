MODULE=vr-de-suntec-navi-test
GCOV_FILTER_PATH=framework/service/navi/src/voice/voicerecog
GCOV_EXCLUDE_PATH=/test/
CPPCHECK_EXCLUDE_PATH=voicerecoglib/Utility/Regex

PROROOTPATH=$PWD;

framework/service/navi/build/tools/cppcheck -j8 -f --xml-version=2 $GCOV_FILTER_PATH/* -i ${GCOV_FILTER_PATH}${GCOV_EXCLUDE_PATH} -i ${GCOV_FILTER_PATH}/${CPPCHECK_EXCLUDE_PATH} -i framework/service/navi/src/voice/voicerecog/voicerecoglib/Platform/VrEngineGoogle/libsrengine/base/ConvertUTF.c --enable=all 2>${PROROOTPATH}/${MODULE}_cppcheck.xml
