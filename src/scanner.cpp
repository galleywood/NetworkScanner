#include "scanner.h"
#include "arp.h"
#include "hostname.h"
#include "ping.h"
#include "vendor.h"
#include <algorithm>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>

namespace {

// "192.168.1.20" -> 3232235796, so IPs compare numerically (not as text,
// where "192.168.1.100" would sort before "192.168.1.20").
std::uint32_t IpToNumber(const std::string& ip) {
    std::uint32_t result = 0, part = 0;
    for (char c : ip) {
        if (c == '.') {
            result = (result << 8) | part;
            part = 0;
        } else if (c >= '0' && c <= '9') {
            part = part * 10 + static_cast<std::uint32_t>(c - '0');
        }
    }
    return (result << 8) | part;
}

} // namespace

std::optional<HostInfo> ScanIP(const std::string& ip) {
    auto mac = ResolveMac(ip);
    if (!mac) return std::nullopt;   // offline -> stop here

    HostInfo host;
    host.ip       = ip;
    host.mac      = *mac;
    host.vendor   = LookupVendor(*mac);
    host.hostname = ResolveHostname(ip);
    host.latency  = PingHost(ip);
    return host;
}

std::vector<HostInfo> ScanSubnet(const std::string& prefix, int workers) {
    workers = std::clamp(workers, 1, 254);

    std::atomic<int> next{1};
    std::vector<HostInfo> results;
    std::mutex resultsMutex;          // protects 'results' while threads push
    std::vector<std::thread> pool;
    pool.reserve(static_cast<std::size_t>(workers));

    for (int w = 0; w < workers; ++w) {
        pool.emplace_back([&] {
            while (true) {
                int i = next++;
                if (i > 254) break;

                if (auto host = ScanIP(prefix + std::to_string(i))) {
                    std::lock_guard<std::mutex> lock(resultsMutex);
                    results.push_back(std::move(*host));
                }
            }
        });
    }
    for (auto& t : pool) t.join();

    // Threads finish in random order, so sort once at the end.
    std::sort(results.begin(), results.end(),
              [](const HostInfo& a, const HostInfo& b) {
                  return IpToNumber(a.ip) < IpToNumber(b.ip);
              });
    return results;
}