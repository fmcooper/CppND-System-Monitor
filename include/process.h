#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid) {
  	CalcCpuUtilization();
  }
  int Pid();
  string User();
  string Command();
  void CalcCpuUtilization();
  float CpuUtilization() const;
  string Ram();
  long int UpTime(); 
  bool operator<(Process const& a) const;


 private:
  int pid_;
  float cpuUtil_;
};

#endif