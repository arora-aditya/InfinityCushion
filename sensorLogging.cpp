#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include "functionLogging.h"
#include "state.h"
#include "sensorRead.h"
using namespace std;

// void logger(char errorTag[], char functionName[], char message[], int errorCode);
float processSensor(int output[CHAR_BIT]){
  /*
    pre-process sensor data
  */
  logger("DEBUG","processSensor", "entered pre-process sensor data");
  float val = 0.4 * output[4] + 0.15 * (output[0]+output[1]+output[2]+output[3]);
  if(val > 0.8){
    return 1;
  }
  logger("DEBUG","processSensor", "exited pre-process sensor data");
  return 0;
}

float processHalfSensor(int output[2]){
  /*
  process data for left and right halves seperately
  */
  logger("DEBUG","processHalfSensor", "entered process data for left and right halves seperately");
  int sum = output[0] + output[1];
  if(sum >= 1){
    return sum;
  }
  logger("DEBUG","processHalfSensor", "exited process data for left and right halves seperately");
  return 0;
}

float summation(int buffer[]){
  /*
  sum over buffer
  */
  logger("DEBUG","summation", "entered sum over buffer");
  float sum = 0;
  for(int i = 0; i < 10; i++){
    sum += buffer[i];
  }
  logger("DEBUG","summation", "exited sum over buffer");
  return sum;
}

void sensorLogger(){
  /*
  loop of sensor reading and writing to file after processing
  */
  gpioReader gpio;
  gpio.init();
  logger("DEBUG", "sensorLogger", "entered loop of sensor reading and writing to file after processing");
  ofstream ofs;
  int buffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int leftBuffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int rightBuffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int j = 0;
  ofs.open ("movement.csv", ofstream::out | ofstream::app);
  if (!ofs.is_open()) {
    logger("FATAL", "OpenFile", "Unable to open file movement.csv", 3);
    return;
  }
  int output[7] = {0,0,0,0,0,1,0};
  ofs<<"date,movement,left,right,fsr\n";
  writeState(1); //state: START
  while(output[5] == 1){
    char val = gpio.getSensorData();
    for(int i = 0; i < 7; i++){
      output[i] = 0;
    }
    /*
      output[5] = {sensor1, sensor2, sensor3, sensor4, FSR, button1, button2}
      value     = {button1, button2, FSR, sensor4, sensor3, sensor2, sensor1}
    */
    for (int i = 0; i < 7; ++i) {
      output[i] = (val >> i) & 1;
      cout<<output[i];
    }
    cout<<'\n';
    int leftOutput[2] = {output[0],output[1]};
    int rightOutput[2] = {output[2],output[3]};
    leftBuffer[j] = processHalfSensor(leftOutput);
    rightBuffer[j] = processHalfSensor(rightOutput);
    buffer[j] = processSensor(output);
    j++;
    if(j > 9){
      j = 0;
      float sumTotal = summation(buffer);
      float sumLeft = summation(leftBuffer);
      float sumRight = summation(rightBuffer);
      ofs<<currentDateTime()<<","<<sumTotal<<","<<sumLeft<<","<<sumRight<<","<<output[4]<<"\n";
      ofs.flush();
      logger("DEBUG", "fileWrite", "written to files and flushed");
    }
  }
  if(output[6] == 1){
    writeState(2); //state: REPORT_GENERATION
  }
  if(output[5] == 0){
    writeState(0); //state: STOP
  })
  logger("DEBUG", "sensorLogger", "closed all files and exited");
  ofs.close();
}

int main()
{
	sensorLogger();

	return 0;
}
