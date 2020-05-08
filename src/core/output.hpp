#ifndef OUTPUT_HPP_
#define OUTPUT_HPP_

#include <string>
#include <sstream>

namespace Output {
  static const char* logFileName = "debuglog.txt";
  extern std::ostream stream;
  
  enum {
	MODE_LOGFILE,
	MODE_STDOUT,
	MODE_SILENT,
	MODE_SIZE //always last
  };

  void SetMode(const unsigned int mode);
}

#endif
