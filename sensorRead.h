#ifndef SENSORREAD_H
#define SENSORREAD_H

#define BUF_LEN 128

#define PIN_0 "0"
#define PIN_1 "1"
#define PIN_2 "2"
#define FSR_PIN "3"
#define BUTN_PIN "18"
#define RST_PIN "19"

#define GPIO_PATH "/sys/class/gpio/"
#define GPIO_PIN_PATH "/sys/class/gpio/gpio"

#define INVARIANT_TEST (_invariant == 0xDEADBEEF)

#include "functionLogging.h"

class gpioReader{
  public:
    int init();
    void setLogLevel(logLevel level);
    
    char getSensorData();
    
    gpioReader();
    gpioReader(logLevel level);
    ~gpioReader();
  private:
    int _gpioFiles[6];
    bool _init;
    logLevel _logLevel;
    int _invariant;
};

#endif