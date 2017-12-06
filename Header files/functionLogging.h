#ifndef FUNCTIONTEST_H
#define FUNCTIONTEST_H
#include <string>
enum logLevel{FATAL, ERROR, WARNING, INFO, DEBUG};
int returnLevel(const char errorTag[]);
const std::string currentDateTime();
void logger(const char errorTag[], const char functionName[], const char message[], const int errorCode = 0);

#endif
