#ifndef FUNCTION_LOGGING
#define FUNCTION_LOGGING
enum logLevel{FATAL, ERROR, WARNING, INFO, DEBUG};
int returnLevel(char errorTag[]);
const std::string currentDateTime();
void logger(char errorTag[], char functionName[], char message[], int errorCode = 0);
#endif
