#include <fstream>
#include "functionLogging.h"
#include "state.h"
#include <iostream>

using namespace std;

int readState(){
  // read State from global state
  ifstream ifk;
  int globalState;
  ifk.open("globalState");
  if (!ifk.is_open()) {
    //checking if file is open
    logger("FATAL", "OpenFile", "Unable to open file globalState", 3);
    return -1;
  }
  ifk>>globalState;
  ifk.close();
  return globalState;
}

int writeState(int globalState){
  ofstream ofk;
  ofk.open("globalState");
  if(!ofk.is_open()) {
    //checking if file is open
    logger("FATAL", "OpenFile", "Unable to open file globalState", 3);
    return -1;
  }
  ofk<<globalState;
  ofk.flush();
  ofk.close();
  char logMessage[50] = "State changed to   ";
  logMessage[17] = (char) globalState + '0';
  logger("INFO","StateChanged",logMessage);
  return globalState;
}
