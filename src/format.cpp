#include <string>
#include <iostream>
#include "format.h"

using std::string;
using std::to_string;

#define SECS_IN_HOUR 3600
#define SECS_IN_MIN 60
#define PAD_CHAR '0'

// Formats time
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int num_hours = seconds / SECS_IN_HOUR;
    seconds = seconds - num_hours * SECS_IN_HOUR;
    int num_minutes = seconds / SECS_IN_MIN;
    int num_seconds = seconds - num_minutes * SECS_IN_MIN;

    string num_hours_formatted = to_string(num_hours);
    num_hours_formatted.insert(
        num_hours_formatted.begin(), 
        2 - num_hours_formatted.length(), 
        PAD_CHAR);
    string num_mins_formatted = to_string(num_minutes);
    num_mins_formatted.insert(
        num_mins_formatted.begin(),
        2 - num_mins_formatted.length(),
        PAD_CHAR);
    string num_secs_formatted = to_string(num_seconds);
    num_secs_formatted.insert(
        num_secs_formatted.begin(),
        2 - num_secs_formatted.length(),
        PAD_CHAR);
    return num_hours_formatted + ":" + num_mins_formatted + ":" + num_secs_formatted; 
}
