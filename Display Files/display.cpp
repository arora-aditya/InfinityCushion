#include <oled-exp.h>
#include <string.h>
#include "state.h"

int displayState(){
  int status;
  int status  = oledDriverInit();
  if(status == EXIT_SUCCESS){
    return -1;
  }
  uint8_t *buffer = malloc(OLED_EXP_WIDTH*OLED_EXP_HEIGHT/8 * sizeof *buffer); // allocate memory for the buffer
  char param[100]="/root/";
  int globalState = readState();
  switch(globalState){
    case 0: strcat(param, "stop.lcd"); break;
    case 1: strcat(param, "start.lcd"); break;
    case 2: strcat(param, "report.lcd"); break;
    default: return 100;
  }

  // read data from file
  status  = oledReadLcdFile(param, buffer);

  // draw on display
  if (status == EXIT_SUCCESS) {
      status  = oledDraw(buffer, OLED_EXP_WIDTH*OLED_EXP_HEIGHT/8);
  }
  return status;
}

int main(){
  return displayState();
}
