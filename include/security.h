#ifndef SECURITY_H
#define SECURITY_H

void security();
void logToFile(const char *filepath, const char *message);
void logEntry(const char *userId, const char *logFilePath);
void logExit(const char *userId, const char *logFilePath);
void addEntry();
void addExit();
void viewLastNEntry();
void viewLastNExit();

#endif
