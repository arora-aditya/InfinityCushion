/*
logs sensor values to movement.csv
*/
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
float processSensor(int output[5]){
  /*
    pre-process sensor data
  */
  logger("DEBUG","processSensor", "entered pre-process sensor data");
  float val = 0.4 * output[3] + 0.25 * (output[0]+output[1]) + 0.3 * (output[2]);
  if(val > 0.8){
    return 1;
  }
  logger("DEBUG","processSensor", "exited pre-process sensor data");
  return 0;
}

float processHalfSensor(int output[1]){
  /*
  process data for left and right halves seperately
  */
  if(readState() >= 4){
    logger("DEBUG","processHalfSensor", "entered process data for left and right halves seperately");
  }
  else{
    logger("INFO","processHalfSensor", "processing halves");
  }
  int sum = output[0];
  if(readState() >= 4){
    logger("DEBUG","processHalfSensor", "exited process data for left and right halves seperately");
  }
  return sum;
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
  gpio.setLogLevel(DEBUG);
  gpio.init();
  logger("DEBUG", "sensorLogger", "entered loop of sensor reading and writing to file after processing");
  ofstream ofs;
  int buffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int leftBuffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int rightBuffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int j = 0;
  ofs.open ("report/movement.csv", ofstream::out | ofstream::app);
  if (!ofs.is_open()) {
    logger("FATAL", "OpenFile", "Unable to open file movement.csv", 3);
    return;
  }
  int output[5] = {0,0,0,0,0};
  ofs<<"date,movement,left,right,fsr\n";
  writeState(1); //state: START
  while(output[4] != 1){
    char val = gpio.getSensorData();
    for(int i = 0; i < 5; i++){
      output[i] = 0;
    }
    /*
      output[5] = {sensor1, sensor2, sensor3, FSR, button1}
      value     = {button1, FSR, sensor3, sensor2, sensor1}
    */
    for (int i = 0; i < 5; ++i) {
      output[i] = (val >> i) & 1;
      cout<<output[i];
    }
    cout<<'\n';
    int leftOutput[1] = {output[0]};
    int rightOutput[1] = {output[1]};
    leftBuffer[j] = processHalfSensor(leftOutput);
    rightBuffer[j] = processHalfSensor(rightOutput);
    buffer[j] = processSensor(output);
    j++;
    if(j > 9){
      j = 0;
      float sumTotal = summation(buffer);
      float sumLeft = summation(leftBuffer);
      float sumRight = summation(rightBuffer);
      ofs<<currentDateTime()<<","<<sumTotal<<","<<sumLeft<<","<<sumRight<<","<<output[3]<<"\n";
      ofs.flush();
      logger("DEBUG", "fileWrite", "written to files and flushed");
    }
  }
  if(output[4] == 1){
    writeState(0); //state: REPORT_GENERATION
  }
  logger("DEBUG", "sensorLogger", "closed all files and exited");
  ofs.close();
}

int main()
{
	sensorLogger();

	return 0;
}
