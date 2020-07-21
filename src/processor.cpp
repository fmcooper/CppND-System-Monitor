#include <map>
#include <string>

#include "processor.h"
#include "linux_parser.h"

using std::map;
using std::string;

void Processor::cacheValues(map<string, int> my_map) {
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
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    map<string, int> my_map = LinuxParser::CpuUtilization();
    cacheIdle = cacheIdle + cacheIowait;
    int idle = my_map["idle"] + my_map["iowait"];
    int cacheNonIdle = cacheUser + cacheNice + cacheSystem + cacheIrq +
                        cacheSoftirq + cacheSteal;
    int nonIdle = my_map["user"] + my_map["nice"] + my_map["system"] + 
                    my_map["irq"] + my_map["softirq"] + my_map["steal"];
    int cacheTotal = cacheIdle + cacheNonIdle;
    int total = idle + nonIdle;

    int totaldifference = total - cacheTotal;
    int idledifference = idle - cacheIdle;

    cacheValues(my_map);

    return (float)(totaldifference - idledifference) / (float)totaldifference;
    }