#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdbool.h>
#ifdef CALLWRAP

// check the result integer for a potential errors, retrieve last error
void ResultWrap(int result) {
  if (result != 0) {
    printf("Error occured upon calling: %d, error code in result: %d\n", WSAGetLastError(), result);
    WSACleanup();
  }
  else if (result == SOCKET_ERROR) {
    printf("Socket failed with an error: %d\n", WSAGetLastError());
    WSACleanup();
  }
}
// check the socket for potential errors
void CheskSock(SOCKET s){
  if (s == INVALID_SOCKET){
    printf("something's wrong with the socket settings, stopping now (%d)\n", WSAGetLastError());
    closesocket(s);
    WSACleanup();
    }
}
#endif
