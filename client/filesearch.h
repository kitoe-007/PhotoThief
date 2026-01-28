#pragma once
#include <windows.h>
#include <wchar.h>
#include <sec_api/string_s.h>
#include <sec_api/stdlib_s.h>
#include <stdbool.h>

#define DEF_LIST_SIZE 1

int size_increment = 0;
typedef struct PictureInfo {
	wchar_t pic_full_path[255];
	wchar_t pic_name[255];
} PICTURE_INFO;


// Get the name of the picture, pass to a struct
static inline void GetPicName(PICTURE_INFO **PictureInfo) {
	int name_idx = 0;
	int path_idx = wcslen((*PictureInfo)->pic_full_path)-1;
	while ((*PictureInfo)->pic_full_path[path_idx] != L'\\') {
		path_idx--;
	}
	path_idx++;
	while (path_idx > wcslen((*PictureInfo)->pic_full_path)-1 && (*PictureInfo)->pic_full_path[path_idx] != '\0') {
		
		(*PictureInfo)->pic_name[name_idx] = (*PictureInfo)->pic_full_path[path_idx];
		name_idx++;
		path_idx++;
	}
	(*PictureInfo)->pic_name[name_idx] = '\0';
}


static inline void DeterminePicList(PICTURE_INFO **pic_list ,wchar_t *full_path) {
	if (*pic_list == NULL) {
		*pic_list = (PICTURE_INFO*)calloc(DEF_LIST_SIZE, sizeof(PICTURE_INFO));
		size_increment = 1;
		wcscpy_s((*pic_list)->pic_full_path, sizeof((*pic_list)->pic_full_path)/sizeof(wchar_t), full_path);
	}
	else {
		size_increment++;
		*pic_list = (PICTURE_INFO*)realloc(*pic_list, sizeof(PICTURE_INFO) * size_increment );
		wcscpy_s((*pic_list)->pic_full_path, sizeof((*pic_list)->pic_full_path)/sizeof(wchar_t), full_path);
	}
}

static inline void DestroyPicList(PICTURE_INFO **pic_list) {
	free(*pic_list);
	size_increment = 0;
}

static inline size_t ToMultiByte(char **dest, wchar_t *source) {
	size_t str_size;
	wcstombs_s(&str_size, *dest, sizeof(*dest), source, sizeof(source));
	return str_size;
}
static inline size_t ToWideChar(wchar_t **dest, char *source) {
	size_t str_size;
	mbstowcs_s(&str_size, *dest, sizeof(*dest),source, sizeof(source));
	return str_size;
}
