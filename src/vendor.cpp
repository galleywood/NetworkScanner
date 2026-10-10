#include "vendor.h"
#include <cctype>
#include <fstream>
#include <unordered_map>
#include <vector>

namespace {

// Key: first 3 bytes of the MAC as 6 uppercase hex chars, e.g. "B827EB"
std::unordered_map<std::string, std::string> g_vendors;

// Small fallback so the feature works even without oui.csv
const struct { const char* oui; const char* name; } kBuiltin[] = {
    {"005056", "VMware"},          {"000C29", "VMware"},
    {"000569", "VMware"},          {"080027", "VirtualBox"},
    {"525400", "QEMU/KVM"},        {"00155D", "Microsoft Hyper-V"},
    {"B827EB", "Raspberry Pi"},    {"DCA632", "Raspberry Pi"},
    {"E45F01", "Raspberry Pi"},    {"28CDC1", "Raspberry Pi"},
    {"D83ADD", "Raspberry Pi"},    {"00000C", "Cisco"},
};

std::string ExtractOui(const std::string& mac) {
    std::string oui;
    for (char c : mac) {
        if (std::isxdigit(static_cast<unsigned char>(c))) {
            oui += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            if (oui.size() == 6) break;
        }
    }
    return oui;
}

// Locally-administered bit (0x02 in the first byte) => randomized/private MAC
bool IsRandomized(const std::string& oui) {
    if (oui.size() < 2) return false;
    int firstByte = std::stoi(oui.substr(0, 2), nullptr, 16);
    return (firstByte & 0x02) != 0;
}

// Minimal CSV splitter that respects "quoted, fields"
std::vector<std::string> SplitCsv(const std::string& line) {
    std::vector<std::string> fields;
    std::string cur;
    bool inQuotes = false;
    for (char c : line) {
        if (c == '"')                  inQuotes = !inQuotes;
        else if (c == ',' && !inQuotes) { fields.push_back(cur); cur.clear(); }
        else if (c != '\r')            cur += c;
    }
    fields.push_back(cur);
    return fields;
}

bool IsHex6(const std::string& s) {
    if (s.size() != 6) return false;
    for (char c : s) if (!std::isxdigit(static_cast<unsigned char>(c))) return false;
    return true;
}

} // namespace

std::size_t LoadVendorDatabase(const std::string& csvPath) {
    g_vendors.clear();
    for (const auto& e : kBuiltin) g_vendors[e.oui] = e.name;

    std::ifstream file(csvPath);
    if (!file) return 0;

    // IEEE format: Registry,Assignment,Organization Name,Organization Address
    std::size_t loaded = 0;
    std::string line;
    while (std::getline(file, line)) {
        auto f = SplitCsv(line);
        if (f.size() < 3 || !IsHex6(f[1])) continue;   // skips header/bad rows
        std::string key;
        for (char c : f[1]) key += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        g_vendors[key] = f[2];
        ++loaded;
    }
    return loaded;
}

std::string LookupVendor(const std::string& mac) {
    std::string oui = ExtractOui(mac);
    if (oui.size() < 6) return "Unknown";
    if (IsRandomized(oui)) return "Randomized MAC (private)";

    auto it = g_vendors.find(oui);
    return it != g_vendors.end() ? it->second : "Unknown";
}