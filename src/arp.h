#pragma once
#include <optional>
#include <string>

// Sends an ARP request. Returns the MAC ("AA:BB:CC:DD:EE:FF") if the
// device answered, or std::nullopt if it is offline / doesn't exist.
std::optional<std::string> ResolveMac(const std::string& ip);