#pragma once
#include <string>

// Everything we learn about one device. Add new fields here
// (open ports, OS guess...) as you add features.
struct HostInfo {
    std::string ip;
    std::string mac;
    std::string vendor   = "Unknown";
    std::string hostname = "Unknown";
    std::string latency  = "Timeout";
};