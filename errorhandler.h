#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef WIN32_MEAN_AND_LEAN
#define WIN32_MEAN_AND_LEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
// check the result integer for a potential errors, retrieve last error
static inline void ResultWrap(int result, LPADDRINFO addrinforef) {
  if (result != 0) {
    printf("Error occured upon calling: %d, error code in result: %d\n", WSAGetLastError(), result);
    WSACleanup();
  }
  else if (result == SOCKET_ERROR) {
    printf("Socket failed with an error: %d\n", WSAGetLastError());
    freeaddrinfo(addrinforef);
    WSACleanup();
  }
}
// check the socket for potential errors
static inline void CheckSock(SOCKET s) {
  if (s == INVALID_SOCKET){
    printf("something's wrong with the socket settings, stopping now (%d)\n", WSAGetLastError());
    closesocket(s);
    WSACleanup();
    }
}
