# toe_fw DDK

## Preparing toolchain
Clone ESP-OPEN-SDK
Clone and follow instruction to build toolchain.
[ESP-OPEN-SDK](https://github.com/pfalcon/esp-open-sdk)

## Create enviroment variables

export PATH=/work/projects/esp-open-sdk/xtensa-lx106-elf/bin:$PATH
export ESP_SDK=/work/projects/ESP8266_NONOS_SDK
export COMPILE=gcc

## Clone toe_fw and compile blink app
git clone https://github.com/lewuskip/toe_fw
cd prorduct/blink
make flash

This shal compile blinking app and flash image to the board.
Currently only D1Mini and nodeMCU is supported.

Make sure you have permission on /dev/ttyUSBx


