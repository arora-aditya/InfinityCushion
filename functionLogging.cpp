#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <fstream>
using namespace std;

int returnLevel(char errorTag[]){
  if(!strcmp(errorTag, "FATAL")){
    return 0;
  }
  else if(!strcmp(errorTag, "ERROR")){
    return 1;
  }
  else if(!strcmp(errorTag, "WARNING")){
    return 2;
  }
  else if(!strcmp(errorTag, "INFO")){
    return 3;
  }
  else if(!strcmp(errorTag, "DEBUG")){
    return 4;
  }
  else{
    return 4;
  }
}

const std::string currentDateTime() {
    /*
    return currentDate and Time in string format
    format is YYYY-MM-DD.HH:mm:ss
    */
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void logger(char errorTag[], char functionName[], char message[], int errorCode){
      /*
      function[]: function names
      exitFlag: whether a function is being entered into or exited from
      */
      ifstream ift;
      ift.open("logLevel");
      int logLevel;
      ift>>logLevel;
      ift.close();
      ofstream oft;
      if(logLevel > 4){
        logLevel = 4;
      }
      if(returnLevel(errorTag) > logLevel){
        return;
      }
      oft.open ("functionLogging.csv", ofstream::out | ofstream::app);
      oft<<'['<<currentDateTime()<<"] ";
      oft<<'(';
      for(int i = 0; i < strlen(errorTag); i++){
          oft<<errorTag[i];
      }
      oft<<"): ";
      for(int i = 0; i < strlen(functionName); i++){
          oft<<functionName[i];
      }
      oft<<" ";
      for(int i = 0; i < strlen(message); i++){
          oft<<message[i];
      }
      if(errorCode != 0){
        oft<<" ";
        oft<<errorCode;
      }
      oft<<"\n";
      oft.close();
}
