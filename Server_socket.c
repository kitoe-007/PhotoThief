#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef WIN32_MEAN_AND_LEAN
#define WIN32_MEAN_AND_LEAN
#endif
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "errorhandler.h"

#define LOCAL_PORT "55555"

int main() {
  WSADATA wsadata;
  int call_result;
  WSAStartup(MAKEWORD(2, 2), &wsadata);
  // general connection info
  struct addrinfo general, *res_pattern = NULL, *result = NULL;
  ZeroMemory(&general, sizeof(general));
  general.ai_family = AF_INET;
  general.ai_socktype = SOCK_STREAM;
  general.ai_protocol = IPPROTO_TCP;
  general.ai_flags = AI_PASSIVE;
  // create a list of possible configurations
  call_result = getaddrinfo(0, LOCAL_PORT, &general, &result);
  ResultWrap(call_result);
  res_pattern = result;
  // create a new socket
  SOCKET listen_socket = socket(
  res_pattern->ai_family,
  res_pattern->ai_socktype,
  res_pattern->ai_protocol
  );
  CheckSock(listen_socket);
  // bind the socket to our IP and port
  call_result = bind(listen_socket, res_pattern->ai_addr, res_pattern->ai_addrlen);
  ResultWrap(call_result);
  printf("socket bound");
  //listen on a socket for incoming connections
  call_result = listen(listen_socket, SOMAXCONN);
  ResultWrap(call_result);
  printf("socket is listening!");
  
  
  return 0;
}
