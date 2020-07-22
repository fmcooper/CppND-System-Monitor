#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>

#include "linux_parser.h"

#define SECOND_TOKEN 2
#define UTIME_TOKEN 14
#define STIME_TOKEN 15
#define CUTIME_TOKEN 16
#define CSTIME_TOKEN 17
#define STARTTIME_TOKEN 22

using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;
using std::map;
using std::pair;

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

// Retrieves a value at the 'token'th token of the line starting with 'key'
string LinuxParser::RetrieveValue(string filename, string key, int token) { 
  string line, k, v;
  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> k) {
        if (k == key) {
          for (int t = 1; t < token; t++) {
            linestream >> v;
          }
          return v;
        }
      }
    }
  }
  return "-1"; 
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string memoryFilename = kProcDirectory + kMeminfoFilename;
  string totalMem = LinuxParser::RetrieveValue(memoryFilename, "MemTotal:", SECOND_TOKEN);
  string freeMem = LinuxParser::RetrieveValue(memoryFilename, "MemFree:", SECOND_TOKEN);
  float percentageUsed = (stof(totalMem) - stof(freeMem)) / stof(totalMem);
  return percentageUsed; 
  }

// Read and return the system uptime
long int LinuxParser::UpTime() { 
  string uptime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return stol(uptime);
}


// Read and return CPU utilization information
map<string, long int> LinuxParser::CpuUtilization() {
  map<string, long int> my_map;
  string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    my_map.insert(pair<string, long int>("user", stoi(user)));
    my_map.insert(pair<string, long int>("nice", stoi(nice)));
    my_map.insert(pair<string, long int>("system", stoi(system)));
    my_map.insert(pair<string, long int>("idle", stoi(idle)));
    my_map.insert(pair<string, long int>("iowait", stoi(iowait)));
    my_map.insert(pair<string, long int>("irq", stoi(irq)));
    my_map.insert(pair<string, long int>("softirq", stoi(softirq)));
    my_map.insert(pair<string, long int>("steal", stoi(steal)));
    my_map.insert(pair<string, long int>("guest", stoi(guest)));
    my_map.insert(pair<string, long int>("guest_nice", stoi(guest_nice)));
  }
  return my_map;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string statFilename = kProcDirectory + kStatFilename;
  string total_procs = LinuxParser::RetrieveValue(statFilename, "processes", SECOND_TOKEN);
  return stoi(total_procs); 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string statFilename = kProcDirectory + kStatFilename;
  string procs_running = LinuxParser::RetrieveValue(statFilename, "procs_running", SECOND_TOKEN);
  return stoi(procs_running);  
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line, user, x, p;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return "-1";
}

// Read a nd return the memory used by a process
string LinuxParser::Ram(int pid) { 
  return RetrieveValue(kProcDirectory + to_string(pid) + kStatusFilename, "VmSize:", SECOND_TOKEN);  
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  return RetrieveValue(kProcDirectory + to_string(pid) + kStatusFilename, "Uid:", SECOND_TOKEN); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, user, x, p;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> p;
      if (LinuxParser::Uid(pid) == p) {   
        return user;
      }
    }
  }
  return "-1";
}

// Read and return the uptime of a process
long int LinuxParser::UpTime(int pid) { 
  string starttime = RetrieveValue(kProcDirectory + to_string(pid) + kStatFilename, to_string(pid), STARTTIME_TOKEN); 
  float uptime = stof(starttime) / (float) sysconf(_SC_CLK_TCK);
  return uptime; 
}

// Read and return CPU utilization for a given process
map<string, long int> LinuxParser::CpuUtilization(int pid) {
  map<string, long int> my_map;
  string line, utime, stime, cutime, cstime, starttime;
  string filename = kProcDirectory + to_string(pid) + kStatFilename;
  
  utime = LinuxParser::RetrieveValue(filename, to_string(pid), UTIME_TOKEN);
  stime = LinuxParser::RetrieveValue(filename, to_string(pid), STIME_TOKEN);
  cutime = LinuxParser::RetrieveValue(filename, to_string(pid), CUTIME_TOKEN);
  cstime = LinuxParser::RetrieveValue(filename, to_string(pid), CSTIME_TOKEN);
  starttime = LinuxParser::RetrieveValue(filename, to_string(pid), STARTTIME_TOKEN);
    
  my_map.insert(pair<string, long int>("utime", stol(utime)));
  my_map.insert(pair<string, long int>("stime", stol(stime)));
  my_map.insert(pair<string, long int>("cutime", stol(cutime)));
  my_map.insert(pair<string, long int>("cstime", stol(cstime)));
  my_map.insert(pair<string, long int>("starttime", stol(starttime)));
  my_map.insert(pair<string, long int>("systemuptime", LinuxParser::UpTime()));
  
  return my_map;
}