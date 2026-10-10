#include "printer.h"
#include <iomanip>
#include <iostream>
#include <mutex>

namespace {
    std::mutex consoleMutex;   // keeps output from interleaving if called from threads
}

void PrintHost(const HostInfo& host) {
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "[+] Online: " << host.ip << "\n"
              << "    MAC:      " << host.mac << "\n"
              << "    Vendor:   " << host.vendor << "\n"
              << "    Host:     " << host.hostname << "\n"
              << "    Latency:  " << host.latency << "\n"
              << "----------------------------------------\n";
}

void PrintHosts(const std::vector<HostInfo>& hosts) {
    for (const auto& host : hosts) PrintHost(host);
}

void PrintSummary(std::size_t deviceCount, double seconds) {
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "Subnet sweep complete: " << deviceCount << " device(s) found in "
              << std::fixed << std::setprecision(2) << seconds << " s" << std::endl;
}