#!/bin/sh

oled-exp -c # clear the display
oled-exp draw report.lcd
./aggregateTime.o
