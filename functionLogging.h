#ifndef FUNCTION_LOGGING
#define FUNCTION_LOGGING
#include <string>
enum logLevel{FATAL, ERROR, WARNING, INFO, DEBUG};
int returnLevel(char errorTag[]);
const std::string currentDateTime();
void logger(const char errorTag[], const char functionName[], const char message[], int errorCode = 0);
#endif