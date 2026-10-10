#pragma once
#include <string>

// Sends one ICMP echo. Returns "<n> ms" or "Timeout".
std::string PingHost(const std::string& ip, unsigned long timeoutMs = 1000);