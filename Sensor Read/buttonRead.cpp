/*
  detects if button is pressed
*/

#include <stdlib.h>
#include "functionLogging.h"
#include "state.h"
#include "sensorRead.h"

void buttonRead(){
  //reads button values from gpio until it gets one
  gpioReader gpio;
  gpio.setLogLevel(DEBUG);
  gpio.init();
  int output[5] = {0,0,0,0,0};
  logger("INFO", "buttonRead", "reading button values");
  while(output[4] != 1){
    char val = gpio.getSensorData();
    for(int i = 0; i < 5; i++){
      output[i] = 0;
    }
    for (int i = 0; i < 5; ++i) {
      output[i] = (val >> i) & 1;
    }
  }
}

int main(const int argc, const char* const argv[]){
  buttonRead();
  return 0;
}
