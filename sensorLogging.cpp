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

float processSensor(int ouput[CHAR_BIT]){
  /*
    pre-process sensor data
  */
  float val = 0.4 * output[0] + 0.15 * (output[1]+output[2]+output[3]+output[4]);
  if(val > 0.8){
    return val;
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

void logger(){
  /*
  infinite loop of sensor reading and writing to file after processing
  */
  ofstream ofs;
  int buffer[10];
  int j = 0;
  ofs.open ("movement.csv", ofstream::out | ofstream::app);
  ofs<<"date,movement\n";
  while(true){
    char val = getSensorData();
    int output[5];
    for (int i = 0; i < 5; ++i) {
      output[i] = (val >> i) & 1;
    }
    buffer[j] = processSensor(output);
    j++;
    if(j > 9){
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
