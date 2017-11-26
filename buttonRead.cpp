#include <stdlib.h>
#include "functionLogging.h"
#include "state.h"
#include "sensorRead.h"

void buttonReadStart(){
  gpioReader gpio;
  gpio.init();
  gpio.setLogLevel(DEBUG);
  int output[7] = {0,0,0,0,0,1,0};
  logger("INFO", "buttonRead", "reading button values");
  while(output[5] != 1){
    char val = gpio.getSensorData();
    for(int i = 0; i < 7; i++){
      output[i] = 0;
    }
    for (int i = 0; i < 7; ++i) {
      output[i] = (val >> i) & 1;
    }
  }
}

void buttonReadReport(){
  gpioReader gpio;
  gpio.init();
  gpio.setLogLevel(DEBUG);
  int output[7] = {0,0,0,0,0,1,0};
  logger("INFO", "buttonRead", "reading button values");
  while(output[6] != 1){
    char val = gpio.getSensorData();
    for(int i = 0; i < 7; i++){
      output[i] = 0;
    }
    for (int i = 0; i < 7; ++i) {
      output[i] = (val >> i) & 1;
    }
  }
}

int main(const int argc, const char* const argv[]){
  if(atoi(argv[1]) == 5){
    buttonReadStart();
  }
  else if(atoi(argv[1]) == 6){
    buttonReadReport();
  }
  return 0;
}
