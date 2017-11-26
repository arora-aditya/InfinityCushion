#!/bin/sh

oled-exp -i
oled-exp draw infinity.lcd
sleep 3
oled-exp -c
oled-exp draw start.lcd
./buttonRead.o
oled-exp -c
oled-exp draw generate.lcd
sleep 1
./sensorLogging.o
./buttonRead.o
source report.sh
oled-exp -c
oled-exp draw infinity.lcd
