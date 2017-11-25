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

bool parseLine(char line[]){
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
  ifstream ifs;
  ofstream ofs;
  ifs.open("report/movement.csv");
  ofs.open("report/hours.csv");
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
          if (ifs.eof()) {                       // End of file
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
}


int main(){
  // cout<<parseLine(",,,,1");
  hours();
}
