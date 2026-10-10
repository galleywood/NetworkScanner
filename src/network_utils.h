#pragma once
#include <string>

// Returns the local subnet prefix, e.g. "192.168.1."
// Falls back to "192.168.0." if detection fails.
std::string GetLocalSubnet();