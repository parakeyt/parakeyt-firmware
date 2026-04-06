#!/bin/bash
cd "$(dirname $0)"
export PICO_PLATFORM=rp2040

if [[ PICO_SDK_PATH == "" ]]; then
    export PICO_SDK_PATH="/opt/pico-sdk/"
fi

if [[ ! -d "$PICO_SDK_PATH" ]]; then
    git clone "https://github.com/raspberrypi/pico-sdk" "$PWD/pico-sdk" --depth 1
    export PICO_SDK_PATH="$PWD/pico-sdk"
fi

mkdir -p "build/" && cd "build/"
if [[ "$?" != 0 ]]; then
    echo 'directory error!'
    exit
fi

cmake -DCMAKE_BUILD_TYPE=Release .. && make --jobs $(nproc)

if [[ "$?" != 0 ]]; then
    echo 'build failed at make or cmake!'
    exit
fi

PICO_USB_DIR="/run/media/$USER/RPI-RP2/"
if [[ -f "$PICO_USB_DIR/INFO_UF2.TXT" ]]; then
    echo "moving fw to $PICO_USB_DIR"
    cp "./parakeyt_fw.uf2" "$PICO_USB_DIR"
fi

if [[ "$1" != "" ]]; then
    echo "Press enter to view pico TTY"
    read -r
    screen /dev/ttyACM* 9600
fi
