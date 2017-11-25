#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include "functionLogging.h"
using namespace std;

float processSensor(int output[CHAR_BIT]){
  /*
    pre-process sensor data
  */
  logger("processSensor","pre-process sensor data", true);
  float val = 0.4 * output[0] + 0.15 * (output[1]+output[2]+output[3]+output[4]);
  if(val > 0.8){
    logger("processSensor","pre-process sensor data", false);
    return 1;
  }
  logger("processSensor","pre-process sensor data", false);
  return 0;
}

float processHalfSensor(int output[2]){
  /*
  process data for left and right halves seperately
  */
  logger("processHalfSensor","process data for left and right halves seperately", true);
  int sum = output[0] + output[1];
  if(sum >= 1){
    logger("processHalfSensor","process data for left and right halves seperately", false);
    return sum;
  }
  logger("processHalfSensor","process data for left and right halves seperately", false);
  return 0;
}

float summation(int buffer[]){
  /*
  sum over buffer
  */
  logger("summation","sum over buffer", true);
  float sum = 0;
  for(int i = 0; i < 10; i++){
    sum += buffer[i];
  }
  logger("summation","sum over buffer", false);
  return sum;
}

char getSensorData(){
  char j = 0;
  for(int i = 0; i < 5; i++){
    j *= 2;
    j += rand()%2;
  }
  return j;
}

void sensorLogger(){
  /*
  infinite loop of sensor reading and writing to file after processing
  */
  ofstream ofs;
  logger("sensorLogger","infinite loop of sensor reading and writing to file after processing", true);
  int buffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int leftBuffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int rightBuffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int j = 0;
  logger("openFile","opening file", true);
  ofs.open ("report/movement1.csv", ofstream::out | ofstream::app);
  if (!ofs.is_open()){
    logger("openFile","unable to open file", false);
    return;
  }
  logger("openFile","file open successfully", false);
  ofs<<"date,movement,left,right,fsr\n";
  while(true){
    char val = getSensorData();
    logger("getSensorData","trying to get sensor data", true);
    if(val == -1){
      logger("getSensorData","unable to get sensor data", false);
      continue;
    }
    logger("getSensorData","got sensor data", false);
    int output[5] = {0,0,0,0,0} ;
    /*
      output[5] = {FSR, sensor1, sensor2, sensor3, sensor4}
      value     = {sensor4, sensor3, sensor2, sensor1, FSR}
    */
    for (int i = 0; i < 5; ++i) {
      output[i] = (val >> i) & 1;
      cout<<output[i];
    }
    cout<<'\n';
    int leftOutput[2] = {output[1],output[2]};
    int rightOutput[2] = {output[3],output[4]};
    leftBuffer[j] = processHalfSensor(leftOutput);
    rightBuffer[j] = processHalfSensor(rightOutput);
    buffer[j] = processSensor(output);
    j++;
    if(j > 9){
      logger("writeToFile", "write to file once buffers are full", true);
      j = 0;
      float sumTotal = summation(buffer);
      float sumLeft = summation(leftBuffer);
      float sumRight = summation(rightBuffer);
      ofs<<currentDateTime()<<","<<sumTotal<<","<<sumLeft<<","<<sumRight<<","<<output[0]<<"\n";
      logger("writeToFile", "written to file", false);
    }
  }

  ofs.close();
  logger("sensorLogger","all files closed", false);
}

int main()
{
	sensorLogger();
	return 0;
}
