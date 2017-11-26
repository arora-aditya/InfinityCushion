#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "sensorRead.h"

#include "functionLogging.h"

const char* _cat(char* dest, const char* const a, const char* const b){
  strncpy(dest, a, BUF_LEN);
  strcat(dest, b);
  return dest;
}

int gpioReader::init(){
  printf("%d\n",_logLevel);
  if(_logLevel >= DEBUG)
    logger("DEBUG", "gpioReader:", "entered init function", 0);
  const char* gpioPins[6];
  gpioPins[0] = PIN_0;
  gpioPins[1] = PIN_1;
  gpioPins[2] = PIN_2;
  gpioPins[3] = FSR_PIN;
  gpioPins[4] = BUTN_PIN;
  gpioPins[5] = RST_PIN;

  int file;
  char buffer[BUF_LEN];
  if((file = open(_cat(buffer, GPIO_PATH, "export"), O_WRONLY)) == -1){
    logger("FATAL", "gpioReader:", "failed to open gpio file, open() returned error code:", errno);
    return -1;
  }
  for(int i = 0; i < 6; ++i){
    strcpy(buffer, gpioPins[i]);
    if(write(file, buffer, strlen(buffer)) == -1){
      logger("FATAL", "gpioReader:", "failed to allocate GPIO, write() returned error code:", errno);
      return -2;
    }
  }
  
  close(file);
  if(_logLevel >= INFO)
    logger("INFO", "gpioReader:", "successfully alloacted GPIOs", 0);
  
  for(int i = 0; i < 5; ++i){
    _cat(buffer, GPIO_PIN_PATH, gpioPins[i]);
    strcat(buffer, "/direction");
    if((file = open(buffer, O_WRONLY)) == -1){
      logger("FATAL", "gpioReader:", "failed to open GPIO direction file, open() returned error code:", errno);
      return -3;
    }
    strcpy(buffer, "in");
    if(write(file, buffer, strlen(buffer)) == -1){
      logger("FATAL", "gpioReader:", "failed to set GPIO direction, write() returned error code:", errno);
      return -4;
    }
    close(file);
  }
   _cat(buffer, GPIO_PIN_PATH, gpioPins[5]);
  strcat(buffer, "/direction");
  if((file = open(buffer, O_WRONLY)) == -1){
    logger("FATAL", "gpioReader:", "failed to open GPIO direction file, open() returned error code:", errno);
    return -3;
  }
  strcpy(buffer, "out");
  if(write(file, buffer, strlen(buffer)) == -1){
    logger("FATAL", "gpioReader:", "failed to set GPIO direction, write() returned error code:", errno);
    return -4;
  }
  close(file);
  
  if(_logLevel >= INFO)
    logger("INFO", "gpioReader:", "successfully set direction of GPIOs", 0);
  
  for(int i = 0; i < 6; ++i){
    _cat(buffer, GPIO_PIN_PATH, gpioPins[i]);
    strcat(buffer, "/value");
    if((file = open(buffer, O_RDWR)) == -1){
      logger("FATAL", "gpioReader:", "failed to open GPIO value file, open() returned error code:", errno);
      return -1;
    }
    _gpioFiles[i] = file;
  }
  
  if(_logLevel >= INFO)
    logger("INFO", "gpioReader:", "successfully initialized GPIOs", 0);
  
  return 0;
}

void gpioReader::setLogLevel(logLevel level){
  logger("INFO", "gpioReader:", "assigning new log level", 0);
  _logLevel = level;
  return;
}

gpioReader::gpioReader(){
  _invariant = 0xDEADBEEF;
  _logLevel = INFO;
  _init = false;
}

gpioReader::gpioReader(logLevel level){
  _logLevel = level;
  if(_logLevel >= DEBUG)
    logger("DEBUG", "gpioReader:", "creating new gpioreader object", 0);
  _invariant = 0xDEADBEEF;
  _init = false;
}

gpioReader::~gpioReader(){
  if(_logLevel >= DEBUG){
    logger("DEBUG", "gpioReader:", "destroying gpioreader object", 0);
    if(!INVARIANT_TEST){
      logger("ERROR", "gpioReader:", "failed invariant test, something is very wrong here...", 0);
    }
  }
  if(_init){
    if(_logLevel >= DEBUG)
      logger("DEBUG", "gpioReader:", "closing GPIO files", 0);
    for(int i = 0; i < 6; ++i){
      close(_gpioFiles[i]);
    }
  }
  _init = false;
}

char gpioReader::getSensorData(){
  if(_init){
    if(_logLevel >= WARNING)
      logger("WARNING", "gpioReader:", "tried to read without initializing, will return 0", 0);
    return 0;
  }
  if(_logLevel >= DEBUG){
    if(!INVARIANT_TEST){
      logger("ERROR", "gpioReader:", "failed invariant test, something is very wrong here...", 0);
    }
    logger("DEBUG", "gpioReader:", "getting sensor data", 0);
    char* buffer = new char;
    char output = 0;
    for(int i = 0; i < 5; ++i){
      if(lseek(_gpioFiles[i], 0, SEEK_SET) == -1)
        logger("ERROR", "gpioReader:", "failed to read data, lseek() returned error code", errno);
      if(read(_gpioFiles[i], buffer, 1) == -1)
        logger("ERROR", "gpioReader:", "failed to read data, read() returned error code", errno);
      else
        output += (*buffer == '1') << i;
    }
    if(lseek(_gpioFiles[5], 0, SEEK_SET) == -1)
      logger("ERROR", "gpioReader:", "failed to write data, lseek() returned error code", errno);
    if(write(_gpioFiles[5], "0", 1) == -1)
      logger("ERROR", "gpioReader:", "failed to write data, write() returned error code", errno);
    if(lseek(_gpioFiles[5], 0, SEEK_SET) == -1)
      logger("ERROR", "gpioReader:", "failed to write data, lseek() returned error code", errno);
    if(write(_gpioFiles[5], "1", 1) == -1)
      logger("ERROR", "gpioReader:", "failed to write data, write() returned error code", errno);
    output |= 0XE0;
    return output;
  }
  char* buffer = new char;
  char output = 0;
  for(int i = 0; i < 5; ++i){
    lseek(_gpioFiles[i], 0, SEEK_SET);
    read(_gpioFiles[i], buffer, 1);
    output += (*buffer == '1') << i;
  }
  lseek(_gpioFiles[5], 0, SEEK_SET);
  write(_gpioFiles[5], "0", 1);
  lseek(_gpioFiles[5], 0, SEEK_SET);
  write(_gpioFiles[5], "1", 1);
  output |= 0XE0;
  return output;
}


#ifdef SENSOR_TEST
int main(){
	gpioReader gpio;
  gpio.setLogLevel(DEBUG);
	gpio.init();
  while(1){
    char a = gpio.getSensorData();
    printf("%d\n", a);
    usleep(1000000);
  }
	return 0;
}
#endif