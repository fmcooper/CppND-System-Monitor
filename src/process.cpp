#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::map;


// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
// Calculations from: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
void Process::CalcCpuUtilization() { 
  map<string, long int> my_map = LinuxParser::CpuUtilization(pid_);
  
  long int total_time = my_map["utime"] + my_map["stime"] +
    				my_map["cutime"] + my_map["cstime"];
  
  float elapsed_time = (float)my_map["systemuptime"] - ((float)my_map["starttime"] /  (float)sysconf(_SC_CLK_TCK));

  float cpu_percentage = ((float)total_time / (float)sysconf(_SC_CLK_TCK)) / elapsed_time;
    
  cpuUtil_ = cpu_percentage; 
}

// Return the calculated cpu utilisation - cached due to sorting
float Process::CpuUtilization() const { return cpuUtil_; }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  return a.cpuUtil_ < cpuUtil_;
}