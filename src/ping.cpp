#include "ping.h"
#include "platform.h"
#include <vector>

std::string PingHost(const std::string& ip, const unsigned long timeoutMs) {
    const HANDLE hIcmp = IcmpCreateFile();
    if (hIcmp == INVALID_HANDLE_VALUE) return "Timeout";

    char sendData[32] = "NetworkScanner Ping";
    constexpr DWORD replySize = sizeof(ICMP_ECHO_REPLY) + sizeof(sendData);
    std::vector<char> replyBuffer(replySize);

    std::string result = "Timeout";
    const DWORD ret = IcmpSendEcho(hIcmp, inet_addr(ip.c_str()),
                             sendData, sizeof(sendData), nullptr,
                             replyBuffer.data(), replySize, timeoutMs);

    if (ret != 0) {
        const auto* reply = reinterpret_cast<PICMP_ECHO_REPLY>(replyBuffer.data());
        result = std::to_string(reply->RoundTripTime) + " ms";
    }

    IcmpCloseHandle(hIcmp);
    return result;
}