#pragma once
#include <string>

// Reverse-resolves an IP to a hostname (DNS / NetBIOS / LLMNR).
// Returns "Unknown" if nothing is found.
std::string ResolveHostname(const std::string& ip);