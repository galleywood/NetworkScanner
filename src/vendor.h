#pragma once
#include <cstddef>
#include <string>

// Loads the IEEE OUI database (oui.csv) and a small built-in fallback table.
// Call ONCE before starting scan threads. Returns how many entries came from
// the file (0 = file missing, built-in list only).
std::size_t LoadVendorDatabase(const std::string& csvPath);

// Takes "AA:BB:CC:DD:EE:FF" and returns the manufacturer, "Randomized MAC
// (private)" for locally-administered addresses, or "Unknown".
// Safe to call from many threads after LoadVendorDatabase().
std::string LookupVendor(const std::string& mac);