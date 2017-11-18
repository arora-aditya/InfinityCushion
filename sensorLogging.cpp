#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <fstream>
using namespace std;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
  /*
  return currentDate and Time in string format
  */
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

  return buf;
}

float processSensor(int output[CHAR_BIT]){
  /*
    pre-process sensor data
  */
  float val = 0.4 * output[0] + 0.15 * (output[1]+output[2]+output[3]+output[4]);
  if(val > 0.8){
    return 1;
  }
  return 0;
}

float summation(int buffer[]){
  /*
  sum over buffer
  */
  float sum = 0;
  for(int i = 0; i < 10; i++){
    sum += buffer[i];
  }
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

void logger(){
  /*
  infinite loop of sensor reading and writing to file after processing
  */
  ofstream ofs;
  int buffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int j = 0;
  ofs.open ("movement.csv", ofstream::out | ofstream::app);
  ofs<<"date,movement\n";
  while(true){
    char val = getSensorData();
    int output[5] = {0,0,0,0,0} ;
    /*
      output[5] = {FSR, sensor1, sensor2, sensor3, sensor4}
      value     = {sensor4, sensor3, sensor2, sensor1, FSR}
    */
    for (int i = 0; i < 5; ++i) {
      output[i] = (val >> i) & 1;
    }
    buffer[j] = processSensor(output);
    cout<<buffer[j];
    cout<<endl;
    j++;
    if(j > 9){
      cout<<endl;
      j = 0;
      float sum = summation(buffer);
      ofs<<currentDateTime()<<","<<sum<<"\n";
    }
  }

  ofs.close();
}

int main()
{
	logger();
	return 0;
}
