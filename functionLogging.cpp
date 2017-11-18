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
void logger(char functionName[], char functionPurpose[], bool exitFlag){
      /*
      function[]: function names
      exitFlag: whether a function is being entered into or exited from
      */
      ofstream ofs;
      ofs.open ("functionLogging.csv", ofstream::out | ofstream::app);

      ofs<<currentDateTime()<<",";
      if(exitFlag){
          ofs<<"enter,";
      }
      else{
          ofs<<"exit,";
      }
      for(int i = 0; i < strlen(functionName); i++){
          ofs<<functionName[i];
      }
      ofs<<",";
      for(int i = 0; i < strlen(functionPurpose); i++){
          ofs<<functionPurpose[i];
      }
      ofs<<"\n";
      ofs.close();
}

int main()
{
	logger("LMAO","LMAOOFF",true);
  logger("LMAO","LMAOOFF",false);

	return 0;
}
