#include "output.hpp"
#include <iostream>
#include <fstream>

namespace Output {
  namespace { //private
    static std::ofstream log(logFileName);
  }

  std::ostream stream(std::cout.rdbuf());

  void SetMode(const unsigned int mode){
    if (mode >= MODE_SIZE)
      return;
    
    switch (mode){
      case MODE_STDOUT:
        stream.rdbuf(std::cout.rdbuf());
        break;
      case MODE_LOGFILE:
        stream.rdbuf(log.rdbuf());
	break;
      case MODE_SILENT:
	//stream.rdbuf();
	break;
      default:
        break;
    }
  }
  
  void Write(const char* in, unsigned int mode){
   
  }
}
