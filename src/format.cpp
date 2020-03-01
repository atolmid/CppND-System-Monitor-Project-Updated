#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    string elapsedTime = " "; 
    string elapsedHours = "";
    string elapsedMinutes = "";
    string elapsedSeconds = "";
    int hours = seconds/3600;
    if (hours<10){
        elapsedHours += '0';
    }
    elapsedHours += std::to_string(hours);
    seconds = seconds%3600;
    int minutes = seconds/60;
    if (minutes<10){
        elapsedMinutes += '0';
    }
    elapsedMinutes += std::to_string(minutes);
    seconds = seconds%60;
    if (seconds<10){
        elapsedSeconds += '0';
    }
    elapsedSeconds += std::to_string(seconds);
    elapsedTime = elapsedTime + elapsedHours + ":" + elapsedMinutes + ":" + elapsedSeconds;
    return elapsedTime; 
    }