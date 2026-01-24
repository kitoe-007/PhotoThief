/* ЖЕСТКАЯ идея - упаковать все пути, имена, веса картинок в динамический список структур
(calloc + ссылка на структуру)
Я распакую список, получу значения и пофайлово буду итерировать по списку, отправляя на сервер  */
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef WIN32_MEAN_AND_LEAN
#define WIN32_MEAN_AND_LEAN 1
#endif
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <math.h>
#include "errorhandler.h"

#define LOCAL_PORT "55555"
#define SERVER_IP "178.157.159.197"

int call_result;
int main() {
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	// general connection info
	struct addrinfo general, *res_pattern = NULL, *result = NULL;
	ZeroMemory(&general, sizeof(general));
	general.ai_family = AF_INET;
	general.ai_socktype = SOCK_STREAM;
	general.ai_protocol = IPPROTO_TCP;
	
	// create a list of possible configurations
	call_result = getaddrinfo(SERVER_IP, LOCAL_PORT, &general, &result);
	ResultWrap(call_result);
	res_pattern = result;
	
	// create a new socket
	SOCKET send_sock = socket(
	res_pattern->ai_family,
	res_pattern->ai_socktype,
	res_pattern->ai_protocol
	);
	CheckSock(send_sock);
	call_result = connect(send_sock, res_pattern->ai_addr, res_pattern->ai_addrlen);
	ResultWrap(call_result);
	
	char recv_buf[23];
	// receive a startup test buffer
	call_result = recv(send_sock, recv_buf, strlen(recv_buf), 0);
	call_result = shutdown(send_sock, SD_RECEIVE);
	
	// send new buffers
	size_t pic_size;
	char pic_size_buf[10]; // picture size buffer (in Megabytes)
	char pic_name_buf[255];
	// char pic_text_buf
	int remains_objects; // THIS WILL BE COLLECTED FROM THE STRUCT
	while (remains_objects > 0) {
	
		// send the name of the picture
		call_result = send(send_sock, pic_name_buf, strlen(pic_name_buf), 0);
		ResultWrap(call_result);
		
		// send the lenght of the picture
		call_result = send(send_sock, pic_size_buf, strlen(pic_size_buf), 0);
		
		// picture itself
		char *pic_text_buf = calloc(pic_size, 1);
		call_result = send(send_sock, pic_text_buf, strlen(pic_text_buf), 0);
		ResultWrap(call_result);
		free(pic_text_buf);
	}
	
	return 0;
}