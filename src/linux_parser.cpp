#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>


#include "linux_parser.h"

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

string LinuxParser::RetrieveValue(string filename, string key) { 
  string line, k, v;
  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> k >> v) {
        if (k == key) {
          return v;
        }
      }
    }
  }
  return "-1"; 
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string memoryFilename = kProcDirectory + kMeminfoFilename;
  string totalMem = LinuxParser::RetrieveValue(memoryFilename, "MemTotal:");
  string freeMem = LinuxParser::RetrieveValue(memoryFilename, "MemFree:");
  float percentageUsed = (stof(totalMem) - stof(freeMem)) / stof(totalMem);
  return percentageUsed; 
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
map<string, int> LinuxParser::CpuUtilization() {
  map<string, int> my_map;
  string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    std::cout << line;
    my_map.insert(pair<string, int>("user", stoi(user)));
    my_map.insert(pair<string, int>("nice", stoi(nice)));
    my_map.insert(pair<string, int>("system", stoi(system)));
    my_map.insert(pair<string, int>("idle", stoi(idle)));
    my_map.insert(pair<string, int>("iowait", stoi(iowait)));
    my_map.insert(pair<string, int>("irq", stoi(irq)));
    my_map.insert(pair<string, int>("softirq", stoi(softirq)));
    my_map.insert(pair<string, int>("steal", stoi(steal)));
    my_map.insert(pair<string, int>("guest", stoi(guest)));
    my_map.insert(pair<string, int>("guest_nice", stoi(guest_nice)));
  }
  return my_map;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string statFilename = kProcDirectory + kStatFilename;
  string total_procs = LinuxParser::RetrieveValue(statFilename, "processes");
  return stoi(total_procs); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string statFilename = kProcDirectory + kStatFilename;
  string procs_running = LinuxParser::RetrieveValue(statFilename, "procs_running");
  return stoi(procs_running);  
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }