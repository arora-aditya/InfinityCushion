#!/bin/bash

cat globalState | while read -n 1 i; do
echo $i
if (($i == 0))
  then
    oled-exp -i
    oled-exp draw infinity.lcd
    sleep 3
    oled-exp -c
    oled-exp start.lcd
    ./buttonRead.o 5
    oled-exp -c
    oled-exp draw generate.lcd
    ./sensorLogging.o
    ./buttonRead.o 6
    source report.sh
    oled-exp -c
    oled-exp draw infinity.lcd
  fi
done
