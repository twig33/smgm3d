#include "file.hpp"
#include <string>
#include <fstream>
#include <streambuf>
#include <unistd.h>

int FileExists(const char* path){
  return (access(path, F_OK) != -1);
}

int ReadFileSize(const char* path, unsigned long* size){
  /* Open the file for reading */
  std::ifstream fs(path, std::ifstream::ate | std::ifstream::binary);
  if (!fs)
    return 0;

  *size = fs.tellg();
  ++(*size);

  fs.close();

  return 1;
}

int ReadFileCStr(const char* path, char* out, unsigned long size){
  std::ifstream fs(path);
  if (!fs)
    return 0;
  
  fs.read(out, size);
  fs.close();

  return 1;
}

int ReadFile(const char* path, std::string *out){
  std::ifstream fs(path);
  if (!fs)
    return 0;
  
  /* Reserve memory in the string */
  fs.seekg(0, std::ios::end);
  out->reserve(fs.tellg());
  fs.seekg(0, std::ios::beg);

  fs.read(&(*out)[0], out->size());
  fs.close();

  return 1;
}
