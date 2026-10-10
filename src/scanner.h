#pragma once
#include "host_info.h"
#include <optional>
#include <string>
#include <vector>

// Probes a single IP. Returns std::nullopt if the host is offline.
std::optional<HostInfo> ScanIP(const std::string& ip);

// Scans prefix + 1..254 (e.g. "192.168.1.") with a fixed pool of worker
// threads. Returns every online host, sorted by IP address.
std::vector<HostInfo> ScanSubnet(const std::string& prefix, int workers = 64);