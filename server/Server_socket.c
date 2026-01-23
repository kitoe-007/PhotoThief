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

#define SERVER_PORT "55555"

FILE *picture;
int call_result;
int main() {
	SECURITY_ATTRIBUTES secattr;
	secattr.nLength = sizeof(SECURITY_ATTRIBUTES);
	secattr.lpSecurityDescriptor = NULL;
	secattr.bInheritHandle = FALSE;
	
	// DWORD dir_len_buffer = 255;
	WCHAR dir_current_buffer[255];
	
	GetCurrentDirectoryW(255, dir_current_buffer);
	wcscat(dir_current_buffer, L"\\Search_results" );
	CreateDirectoryW(dir_current_buffer, &secattr);
	SetCurrentDirectoryW(dir_current_buffer);
	
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	
	// general connection info
	struct addrinfo general, *res_pattern = NULL, *result = NULL;
	ZeroMemory(&general, sizeof(general));
	general.ai_family = AF_INET;
	general.ai_socktype = SOCK_STREAM;
	general.ai_protocol = IPPROTO_TCP;
	general.ai_flags = AI_PASSIVE;
	
	// create a list of possible configurations
	call_result = getaddrinfo(0, SERVER_PORT, &general, &result);
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
	printf("socket bound\n");

	// listen on a socket for incoming connections
	call_result = listen(listen_socket, SOMAXCONN);
	ResultWrap(call_result);
	printf("socket is listening!\n");
	SOCKET client_socket = accept(listen_socket, NULL, NULL);
	CheckSock(client_socket);
	printf("connection accepted!\n");

	// send packets
	char buf[] = "connection established";
	call_result = send(client_socket, buf, strlen(buf), 0);
	ResultWrap(call_result);
	
	// receive packets
	char pic_size_buf[10]; // picture size buffer (in Megabytes)
	size_t pic_size; // for casting the buffer
	char pic_name_buf[255];

	do {
		//receive the name of the picture 
		call_result = recv(client_socket,pic_name_buf, strlen(pic_name_buf),0);
		
		// receive size of the picture
		call_result = recv(client_socket, pic_size_buf, strlen(pic_size_buf), 0);
		ResultWrap(call_result);
		
		pic_size = pow(1024, 2)*atoi(pic_size_buf);
		char *textbuf = calloc(pic_size, 1);
		call_result = recv(client_socket, textbuf, strlen(textbuf), 0);
		picture = fopen(pic_name_buf, "wb+");
		fwrite(textbuf, 1, sizeof(picture), picture);
		fclose(picture); 
		free(textbuf);
		printf("received string!\n");
		ResultWrap(call_result);
	} while (call_result > 0);

	
	return 0;
	
	// shutdown
	call_result = shutdown(client_socket, SD_BOTH);
	ResultWrap(call_result);
	call_result = closesocket(client_socket);
	WSACleanup();
	
}

