# 清屏
clear
reset
# 设置cmake使用的环境参数(需要根据自己的实际目录进行修改)
# export FmDev="/Users/kevin.jiang/Dev/src/github-res/fm.rbfx.wasm.apps"
export FmDev=$(pwd)
echo $FmDev
export app_name=testAS
#
export fm_links_exe_flags="\
  --preload-file ${FmDev}/${app_name}/data@/ \
  "
#
rm -rf wasm_builds
rm -rf website/test5
emcmake cmake -B ./wasm_builds -S .
cd ./wasm_builds
make
#
