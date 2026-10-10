#pragma once
// Single place for Windows networking includes.
// Order matters: winsock2.h must come before windows.h.
 
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif
 
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>
 
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")