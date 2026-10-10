#include "hostname.h"
#include "platform.h"
#include <cstring>

std::string ResolveHostname(const std::string& ip) {
    sockaddr_in sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &sa.sin_addr);

    char hostname[NI_MAXHOST];
    char service[NI_MAXSERV];

    if (getnameinfo(reinterpret_cast<sockaddr*>(&sa), sizeof(sa),
                    hostname, NI_MAXHOST, service, NI_MAXSERV,
                    NI_NUMERICSERV) == 0) {
        return hostname;
                    }
    return "Unknown";
}