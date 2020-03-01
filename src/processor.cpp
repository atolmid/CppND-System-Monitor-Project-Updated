#include "processor.h"
//#include <string>
#include <iostream>
#include "linux_parser.h"

using std::string;

// Return the aggregate CPU utilization
float Processor::Utilization() { 
   /*string key;
    string usertimeSt, guestSt, nicetimeSt, guestniceSt; 
    string idletimeSt, ioWaitSt, systemtimeSt; 
    string irqSt, softIrqSt, virtalltimeSt;
    string stealSt;
    string line;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);*/
    // Use LinuxParser::CpuUtilization() instead?
    /*if (stream.is_open()) {
      if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> key >> usertimeSt >> nicetimeSt >> 
        systemtimeSt >> idletimeSt >> ioWaitSt >> irqSt >> softIrqSt
        >> stealSt >> guestSt >> guestniceSt;
      }
    }
    int usertime = std::stoi(usertimeSt) - std::stoi(guestSt);            
    int nicetime = std::stoi(nicetimeSt) - std::stoi(guestniceSt);                
    int idlealltime = std::stoi(idletimeSt) + std::stoi(ioWaitSt);                 
    int systemalltime = std::stoi(systemtimeSt) + std::stoi(irqSt) + std::stoi(softIrqSt);
    int virtalltime = std::stoi(guestSt) + std::stoi(guestniceSt);
    int totaltime = usertime + nicetime + systemalltime + idlealltime + std::stoi(stealSt) + virtalltime;

    return (totaltime-idlealltime)/(float)totaltime; */
    return (LinuxParser::ActiveJiffies()/(float)(LinuxParser::Jiffies()));
}