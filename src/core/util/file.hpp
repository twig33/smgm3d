#ifndef FILE_HPP_
#define FILE_HPP_

#include <string>

int FileExists(const char* path);
int ReadFileSize(const char* path, unsigned long* out);
int ReadFileCStr(const char* path, char* out, unsigned long size);
int ReadFile(const char* path, std::string* out);

#endif
