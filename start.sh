#!/bin/sh

oled-exp -i # initialize the display
oled-exp draw infinity.lcd # draw text to display
sleep 3 # sleep before clearing the display
oled-exp -c # clear the display
oled-exp draw start.lcd
./buttonRead.o # detect button press
oled-exp -c
oled-exp draw generate.lcd
sleep 1
./sensorLogging.o # start logging sensorValues to movement.csv
./buttonRead.o
source report.sh # aggregate hours over time
oled-exp -c
oled-exp draw infinity.lcd
