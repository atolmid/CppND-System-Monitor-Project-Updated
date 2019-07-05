#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// TODO: Move this into ProcessParser
class Path {
 public:
  inline static const std::string base{"/proc/"};
  inline static const std::string cmdline{"/cmdline"};
  inline static const std::string cpuinfo{"/cpuinfo"};
  inline static const std::string status{"/status"};
  inline static const std::string stat{"/stat"};
  inline static const std::string uptime{"/uptime"};
  inline static const std::string meminfo{"/meminfo"};
  inline static const std::string version{"/version"};
  inline static const std::string users{"/etc/passwd"};
};

#endif