MODULE=voicerecog-navi-test
GCOV_FILTER_PATH=framework/service/navi/src/voice/voicerecog
GCOV_EXCLUDE_PATH=/test/

. framework/service/navi/build/tools/gcovrtool.sh

. framework/service/navi/build/tools/check_testfile_matches.sh -s ${GCOV_FILTER_PATH} -t ${GCOV_FILTER_PATH}${GCOV_EXCLUDE_PATH} -m ${MODULE}

