#!/bin/bash
export PICO_SDK_PATH=/opt/pico-sdk/
export PICO_EXTRAS_PATH=/opt/pico-extras/

mkdir -p "build/" && cd "build/"
if [[ "$?" != 0 ]]; then
    echo 'directory error!'
    exit
fi

cmake -DCMAKE_BUILD_TYPE=Release .. && make

if [[ "$?" != 0 ]]; then
    echo 'build failed at make or cmake!'
    exit
fi

PICO_DIR="/run/media/me/RPI-RP2/"
if [[ -f "$PICO_DIR/INFO_UF2.TXT" ]]; then
    echo "moving fw to $PICO_DIR"
    cp "./parakeyt_fw.uf2" "$PICO_DIR"
fi

if [[ "$1" != "" ]]; then
    echo "Press enter to view pico TTY"
    read -r
    screen /dev/ttyACM* 9600
fi
