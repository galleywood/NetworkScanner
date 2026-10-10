#include "network_utils.h"
#include "platform.h"
#include <cstring>
#include <iostream>

std::string GetLocalSubnet() {
    const std::string fallback = "192.168.0.";

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) return fallback;

    // "Connecting" a UDP socket sends no data, but forces the OS routing
    // table to pick the correct local adapter for this socket.
    sockaddr_in serv;
    std::memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("8.8.8.8");
    serv.sin_port = htons(53);

    if (connect(sock, reinterpret_cast<const sockaddr*>(&serv), sizeof(serv)) == SOCKET_ERROR) {
        closesocket(sock);
        return fallback;
    }

    sockaddr_in name;
    int namelen = sizeof(name);
    if (getsockname(sock, reinterpret_cast<sockaddr*>(&name), &namelen) == 0) {
        std::string localIP = inet_ntoa(name.sin_addr);
        closesocket(sock);

        std::cout << "Detected active adapter IP: " << localIP << "\n";

        size_t lastDot = localIP.find_last_of('.');
        if (lastDot != std::string::npos) {
            return localIP.substr(0, lastDot + 1);
        }
        return fallback;
    }

    closesocket(sock);
    return fallback;
}