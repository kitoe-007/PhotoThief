#include <windows.h>

typedef struct PictureInfo {
	int pic_size;
	char pic_full_path[255];
	char pic_name;
} PICTURE_INFO;

static inline char GetPicName();
static inline PictureInfo PictureList();