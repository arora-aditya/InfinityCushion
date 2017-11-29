/*
  accumulates the fsr values from movement.csv to hours.csv
*/

#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include "state.h"
#include "functionLogging.h"

using namespace std;

bool parseLine(char line[]){
  // parse line to read only the fsr
  int commaCount = 0;
  int i = 0;
  while(commaCount != 4){
    if(line[i] == ','){
      commaCount++;
    }
    i++;
  }
  return line[i] == '1';
}

void hours(){
  // design hours.csv using movement.csv
  logger("INFO", "hours", "logging hours to hours.csv");
  ifstream ifs;
  ofstream ofs;
  ifs.open("report/movement.csv");
  if (!ifs.is_open()) {
    //checking if file is open
    logger("FATAL", "OpenFile", "Unable to open file movement.csv", 3);
    return;
  }
  ofs.open("report/hours.csv");
  if (!ofs.is_open()) {
    //checking if file is open
    logger("FATAL", "OpenFile", "Unable to open file movement.csv", 3);
    return;
  }
  int seconds = 0;
  char date[11];
  char previousDate[11];
  bool dateSet = false;
  char line[100];
  bool done = false;
  int fileLineNumber = 0;
  while (!done) {             // Read from file
      ++fileLineNumber;
      if (!ifs.getline(line, 99)) { // Get next line
          if (ifs.eof()) {                       // End of file check
              done = true;
              break;
          }
      }
      if(fileLineNumber-1){
        for(int i = 0; i < 10; i++){
          date[i] = line[i];
        }
        date[10] = '\0';
        if(!dateSet){
          strcpy(previousDate, date);
          dateSet = true;
        }
        if(strcmp(date, previousDate) != 0){
          // only write when the date changes
          for(int i = 0; i < 10; i++){
            ofs<<previousDate[i];
          }
          ofs<<','<<(float)seconds/3600<<endl;
          seconds=0;
          if(parseLine(line)){
            seconds++;
          }
          strcpy(previousDate, date);
        }
        else{
          if(parseLine(line)){
            seconds++;
          }
        }
      }
      else{
        ofs<<"date,hoursStudying\n";
      }
    }
    for(int i = 0; i < 10; i++){
      ofs<<date[i];
    }
    ofs<<','<<(float) seconds/3600<<endl;
    ifs.close();
    ofs.close();
    logger("INFO", "hours", "finished logging hours");
}


int main(){
  hours();
  // change global state to 0
  writeState(0);
}
