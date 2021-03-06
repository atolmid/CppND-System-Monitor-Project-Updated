#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string key, value;
  int memTotal=-1, memFree=-1, memUsed=-1;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = std::stoi(value);          
        }
        else if (key == "MemFree:") {
          memFree = std::stoi(value);
        }
        if (memFree>=0 && memTotal>=0){
          memUsed = memTotal - memFree;
          return (memUsed/(float)memTotal);
        }
      }
    }
  }
  return 0.0; 
  }

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    //std::cout << uptime << std::endl;
    }
  return std::stol(uptime); 
  }

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long jiffies = 0;
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  for (int i = kUser_; i <= kSteal_; i++) {
    jiffies += stol(cpuUtilization[i]);
  }
  return jiffies; 
}


// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long idleJiffies = 0;
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  for (int i=kIdle_;i<=kIOwait_;i++){
    idleJiffies += stol(cpuUtilization[i]);
  }
  return idleJiffies; 
}


// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  long activeJiffies = 0;
  string value, line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> value) {
      if (i >= 13 && i <= 16) {
        activeJiffies += stol(value);
      }
      i++;
    }
  }
  return activeJiffies; 
}


// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return (LinuxParser::Jiffies()-LinuxParser::IdleJiffies()); 
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string value, line;
  vector<string> cpuValues;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value != "cpu") {
        cpuValues.push_back(value);
      }
    }
  }
  return cpuValues; 
  }

float LinuxParser::CpuUtilization(int pid) { 
    long seconds = LinuxParser::UpTime() - (LinuxParser::UpTime(pid)/sysconf(_SC_CLK_TCK));
    return (LinuxParser::ActiveJiffies(pid)/sysconf(_SC_CLK_TCK))/seconds; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);          
        }
      }
    }
  }
  return 0; 
  }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);          
        }
      }
    }
  }
  return 0; 
  }

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string command = "";
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, command);
    return command;
  } 
  return string(); 
  }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string key, value;
  string line;
  double lRam;
  std::stringstream sRam;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          lRam = stod(value);
          lRam /= 1024;
          sRam << std::fixed << std::setprecision(3) << lRam;
          return sRam.str();          
        }
      }
    }
  }
  return string(); 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;          
        }
      }
    }
  }
  return string();
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string key, value, x;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> x >> value) {
        if (value == uid) {
          return key;          
        }
      }
    }
  }
  return string(); 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
	int i = 0;
    while (linestream >> value) {
      if (i == 21) {
        return stol(value) / sysconf(_SC_CLK_TCK);
      }
      i++;
    }
  }
  return 0; 
}