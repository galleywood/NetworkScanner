#include "arp.h"
#include "platform.h"
#include <cstdio>

std::optional<std::string> ResolveMac(const std::string& ip) {
    ULONG destIp = inet_addr(ip.c_str());
    ULONG macAddr[2];
    ULONG macAddrLen = 6;

    if (SendARP(destIp, 0, macAddr, &macAddrLen) != NO_ERROR) {
        return std::nullopt;
    }

    const BYTE* b = reinterpret_cast<const BYTE*>(macAddr);
    char buf[18];
    std::snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
                  b[0], b[1], b[2], b[3], b[4], b[5]);
    return std::string(buf);
}