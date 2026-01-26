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
#include "filesearch.h"

#define LOCAL_PORT "55555"
#define SERVER_IP "178.157.159.197"
extern PICTURE_INFO *pic_list;
int call_result;

int main() {
	FILE *picture;
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	// general connection info
	struct addrinfo general_info, *res_pattern = NULL, *result = NULL;
	ZeroMemory(&general_info, sizeof(general_info));
	general_info.ai_family = AF_INET;
	general_info.ai_socktype = SOCK_STREAM;
	general_info.ai_protocol = IPPROTO_TCP;
	
	// create a list of possible configurations
	call_result = getaddrinfo(SERVER_IP, LOCAL_PORT, &general_info, &result);
	ResultWrap(call_result);
	res_pattern = result;
	
	// create a new socket
	SOCKET send_sock = socket(
	res_pattern->ai_family,
	res_pattern->ai_socktype,
	res_pattern->ai_protocol
	);
	CheckSock(send_sock);
	while (true) {
		call_result = connect(send_sock, res_pattern->ai_addr, res_pattern->ai_addrlen);
		if (call_result != 0 && res_pattern->ai_next != NULL) {
			res_pattern = res_pattern->ai_next;
			continue;
		}
		else if (call_result != 0 && res_pattern == NULL) {
			ResultWrap(call_result);
			break;
		}
		else if (call_result == 0) break;
	} 
	
	char recv_buf[23];
	// receive a startup test buffer
	call_result = recv(send_sock, recv_buf, strlen(recv_buf), 0);
	call_result = shutdown(send_sock, SD_RECEIVE);
	
	// send new buffers
	wchar_t pic_name_buf[255];
	
	//actual buffers 
	double picture_byte_size;
	char pic_size_buf[10]; // picture size buffer (in Megabytes)
	
	while (picture_inf.remains_pictures > 0) {
		
		picture = fopen(pic_list->pic_name, "rb");
		fseek(picture, 0, SEEK_END);
		picture_byte_size = ftell(picture);
		rewind(picture);
		char *picture_binary = calloc(picture_byte_size, sizeof(char));
		fread(picture_binary, sizeof(char), picture_byte_size, picture);
		sprintf(pic_size_buf, "%lf", (picture_byte_size/pow(1024, 2)));
		
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
		pic_list->remains_pictures --;
	}
	
	return 0;
}