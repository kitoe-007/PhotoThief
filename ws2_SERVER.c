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
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define PUBLIC_PORT "55555"
#define DEFAULT_IP_ADRESS "localhost"

// COMPILE WITH -lws2_32 if on clang or use #pragma with linking parameters for ws2_32 lib
char recvbuf[1000];
#ifdef PUBLIC_PORT
int main() {
  WSADATA wsadata;
  WSAStartup(MAKEWORD(2, 2), &wsadata);
  struct addrinfo server_addr = {0}, *result_pr = NULL, *result = NULL;
  server_addr.ai_family = AF_INET;
  server_addr.ai_protocol = IPPROTO_TCP;
  server_addr.ai_socktype = SOCK_STREAM;
  server_addr.ai_flags = AI_PASSIVE;
  int call_result = getaddrinfo(NULL, DEFAULT_IP_ADRESS, &server_addr, &result);
  printf("getaddrinfo done");
  result_pr = result;
  SOCKET listening_socket = socket(
  result_pr->ai_family,
  result_pr->ai_socktype,
  result_pr->ai_protocol
  );
  call_result = bind(listening_socket, result_pr->ai_addr, result_pr->ai_addrlen);
  printf("binding done");
  listen(listening_socket, SOMAXCONN);
  SOCKET client_socket = INVALID_SOCKET;
  while(client_socket == INVALID_SOCKET) {
  printf ("trying to accept...");
    client_socket = accept(listening_socket, NULL, NULL);
    }
  closesocket(listening_socket);
  int send_call_result;
  do {
    call_result = recv(client_socket, recvbuf, strlen(recvbuf), 0);
    if (call_result > 0) printf("String received!");
    send_call_result = send(client_socket, recvbuf, strlen(recvbuf), 0);
    if (send_call_result == SOCKET_ERROR) {
      printf("send call failed with error: %d\n", WSAGetLastError());
      closesocket(client_socket);
      WSACleanup();
      }
    } while (call_result > 0);
  call_result = shutdown(client_socket, SD_SEND);
  closesocket(client_socket);
  WSACleanup();
  return 0;
}
#endif

