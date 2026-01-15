#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <string.h>
#include <winsock2.h>
//#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define PUBLIC_PORT "55555"

// COMPILE WITH -lws2_32 if on clang or use #pragma with linking parameters for ws2_32 lib
