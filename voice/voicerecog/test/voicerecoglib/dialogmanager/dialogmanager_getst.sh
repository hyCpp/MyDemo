#！bin/bash
UsrPath="/home/niechaoqun/17Cy"
MockMachine="dcu_e2"
EmulterLib="/usr/lib"
AsrLibPath="$UsrPath/vendor/renesas/voicebox/$MockMachine/lib"
IflykPath="$UsrPath/vendor/renesas/iflytek/$MockMachine/lib"

#cd $UsrPath

#. build/envsetup.sh 

#lunch generic_dcue2T2-eng 

#make voicerecog-dialogmanager-tests -j10

adb remount

echo "------push libvr_dataprovider-navi.so----"
adb push $UsrPath/out/target/product/dcue2/obj/lib/libvr_dataprovider-navi.so $EmulterLib

echo "------push asr.so-------"
adb push $AsrLibPath/libgenericdca.so $EmulterLib
adb push $AsrLibPath/libvocon3200_asr.so $EmulterLib
adb push $AsrLibPath/libvocon3200_base.so $EmulterLib
adb push $AsrLibPath/libvocon3200_gram2.so $EmulterLib
adb push $AsrLibPath/libvocon3200_platform.so $EmulterLib
adb push $AsrLibPath/libvocon3200_pron.so $EmulterLib
adb push $AsrLibPath/libvocon_ext_heap.so $EmulterLib
adb push $AsrLibPath/libvocon_ext_stream.so $EmulterLib
#adb push $AsrLibPath/libuuid.so.1 $EmulterLib
#adb push $AsrLibPath/libVBTAddressEntryAgent.so $EmulterLib
#adb push $AsrLibPath/libVBTDestinationAgent.so $EmulterLib
#adb push $AsrLibPath/libVBTEngineClient.so $EmulterLib
#adb push $AsrLibPath/libVBTHFDAgent.so $EmulterLib
#adb push $AsrLibPath/libVBTLoader.so $EmulterLib
#adb push $AsrLibPath/libVBTMusicAgent.so $EmulterLib
#adb push $AsrLibPath/libVBTVaalAudioStream.so $EmulterLib
#adb push $AsrLibPath/libVECIScript.so $EmulterLib
#adb push $AsrLibPath/libVECITransport.so $EmulterLib
echo "asr.so Complete!!!"

echo "------push iFlyk.so ----- "
adb push $IflykPath $EmulterLib
#adb push $IflykPath/libbargein.so $EmulterLib
#adb push $IflykPath/libiFlyNLI.so $EmulterLib
#adb push $IflykPath/libissauth.so $EmulterLib
#adb push $IflykPath/libisssr.so $EmulterLib
#adb push $IflykPath/libivAECKernel.so $EmulterLib
#adb push $IflykPath/libivMAEKernel.so $EmulterLib
#adb push $IflykPath/liblsa.so $EmulterLib
#adb push $IflykPath/libmsc.so $EmulterLib

echo "iFlyk.so Complete!!!!"

echo "------push Other.so-------"
adb push $UsrPath/out/target/product/dcue2/obj/lib/libcrypto.so.1.0.0 $EmulterLib
adb push $UsrPath/vendor/renesas/navi/$MockMachine/lib/libvr_asr-navi.so $EmulterLib
adb push $UsrPath/out/target/product/dcue2/obj/lib/libssl.so $EmulterLib
echo "Other.so Complete!!!!"

./framework/service/navi/src/voice/voicerecog/test/voicerecog_run_test.sh 

