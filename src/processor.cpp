#include <map>
#include <string>

#include "processor.h"
#include "linux_parser.h"

using std::map;
using std::string;

// Cache the current system values for calculation of change in CPU utilisation
void Processor::cacheValues(map<string, long int> my_map) {
    cacheUser = my_map["user"];
    cacheNice = my_map["nice"];
    cacheSystem = my_map["system"];
    cacheIdle = my_map["idle"];
    cacheIowait = my_map["iowait"];
    cacheIrq = my_map["irq"];
    cacheSoftirq = my_map["softirq"];
    cacheSteal = my_map["steal"];
    cacheGuest = my_map["guest"];
    cacheGuest_nice = my_map["guest_nice"];
}

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    map<string, long int> my_map = LinuxParser::CpuUtilization();
    cacheIdle = cacheIdle + cacheIowait;
    long int idle = my_map["idle"] + my_map["iowait"];
    long int cacheNonIdle = cacheUser + cacheNice + cacheSystem + cacheIrq +
                        cacheSoftirq + cacheSteal;
    long int nonIdle = my_map["user"] + my_map["nice"] + my_map["system"] + 
                    my_map["irq"] + my_map["softirq"] + my_map["steal"];
    long int cacheTotal = cacheIdle + cacheNonIdle;
    long int total = idle + nonIdle;

    long int totaldifference = total - cacheTotal;
    long int idledifference = idle - cacheIdle;

    cacheValues(my_map);

    return (float)(totaldifference - idledifference) / (float)totaldifference;
  }