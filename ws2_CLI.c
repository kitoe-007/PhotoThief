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
/* Это ТЕСТ клиента, он рабочий, но ему нужен сервер. Скорее всего
будет враппер и норм реализация */

int main() {
  WSADATA wsaData;
  int i_result;
  i_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (i_result != 0) {printf("WSAStartup failed: %d",  i_result);}
  struct addrinfo *result_pattern = NULL, *result_copy = NULL, info_set = {0};
  info_set.ai_family = AF_UNSPEC;
  info_set.ai_protocol = IPPROTO_TCP;
  info_set.ai_socktype = SOCK_STREAM;
  int call_result = getaddrinfo("localhost", PUBLIC_PORT, &info_set, &result_pattern);
  if (call_result != 0 ) {printf("getaddinfo failure, code %d", call_result);}
  result_copy = result_pattern;
  SOCKET socket1 = socket(
  result_copy->ai_family,
  result_copy->ai_socktype,
  result_copy->ai_protocol
  );
  while (true) {
    call_result = connect(socket1, result_copy->ai_addr, result_copy->ai_addrlen);
    if (call_result == SOCKET_ERROR && result_copy->ai_next != NULL) {
      printf ("Connection error, retrying...\n");
      result_copy = result_copy->ai_next;
      closesocket(socket1);
      socket1 = socket(
      result_copy->ai_family,
      result_copy->ai_socktype,
      result_copy->ai_protocol
      );
      continue;
    }
    else if (result_copy->ai_next == NULL && call_result == SOCKET_ERROR) {
      printf("connection aborted, forcefully quitting");
      exit(0);
    }
    else break;
  }
  char buftext[15] = "this is a test";
  char recvbuf[1000];
  printf("sending buffer");
  call_result = send(socket1, buftext, 512, 0);
  if (call_result == 0) printf("buffer sent!");
  else printf("%c", call_result);
  call_result = shutdown(socket1, SD_SEND);
  do {
    call_result = recv(socket1, recvbuf, strlen(recvbuf), 0);
  } while(call_result > 0);
  closesocket(socket1);
  WSACleanup();
  return 0;
}
