#pragma once
#include "host_info.h"
#include <cstddef>
#include <vector>

// Prints one host block (thread-safe).
void PrintHost(const HostInfo& host);

// Prints every host in the order given.
void PrintHosts(const std::vector<HostInfo>& hosts);

// One-line summary: device count and scan duration.
void PrintSummary(std::size_t deviceCount, double seconds);