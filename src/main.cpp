#include "network_utils.h"
#include "platform.h"
#include "printer.h"
#include "scanner.h"
#include "vendor.h"
#include <chrono>
#include <iostream>

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }


    if (const std::size_t loaded = LoadVendorDatabase("oui.csv"))
        std::cout << "Loaded " << loaded << " vendor entries.\n";
    else
        std::cout << "oui.csv not found, using small built-in vendor list.\n";

    const std::string subnet = GetLocalSubnet();

    std::cout << "Booting Network Scanner...\n";
    std::cout << "Scanning " << subnet << "0/24\n\n";

    const auto start = std::chrono::steady_clock::now();
    const std::vector<HostInfo> hosts = ScanSubnet(subnet);
    const std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;

    PrintHosts(hosts);
    PrintSummary(hosts.size(), elapsed.count());

    WSACleanup();

    std::cin.get();
    return 0;
}