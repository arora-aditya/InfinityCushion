#include <stdlib.h>
#include "functionLogging.h"
#include "state.h"
#include "sensorRead.h"

void buttonRead(){
  gpioReader gpio;
  gpio.init();
  gpio.setLogLevel(DEBUG);
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
