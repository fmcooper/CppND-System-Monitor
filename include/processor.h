#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <map>
#include <string>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    void cacheValues(std::map<std::string, int> my_map);
    int cacheUser{0}, cacheNice{0}, cacheSystem{0}, cacheIdle{0}, cacheIowait{0}, 
        cacheIrq{0}, cacheSoftirq{0}, cacheSteal{0}, cacheGuest{0}, cacheGuest_nice{0};
};

#endif