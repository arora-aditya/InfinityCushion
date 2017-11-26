#!/bin/bash

cat globalState | while read -n 1 i; do
echo $i
if (($i == 2))
  then
    oled-exp -c
    oled-exp draw report.lcd
    ./aggregateTime.o
  fi
done
