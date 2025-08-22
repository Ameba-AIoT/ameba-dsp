COMPILE=/opt/xtensa/Xplorer-9.0.18/xplorer
DSP_BATCH=dsp_batch.xml

DIR=$(pwd)
IMPORT_DIR=${DIR}/..

chmod +x ../img_utility/elf2bin
rm -rf ../../auto_ws/*
rm -rf ../../auto_ws/.m*
${COMPILE} --headless ${DIR%}/${DSP_BATCH}  --import-dir ${IMPORT_DIR} --import-projects all --copy-to-workspace --cleanbuild -- -data  ${DIR}/../../auto_ws