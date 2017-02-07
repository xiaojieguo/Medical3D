#ifndef MEDICAL_COMMON
#define MEDICAL_COMMON

#define  USERDATASIZE 1024*2

typedef struct _UserData
{
	char name[USERDATASIZE+1];
}UserData;

typedef struct _colorinfo
{
	int r;
	int g;
	int b;
}ColorInfo;

typedef QVector<ColorInfo> ColorArray;

extern const char  *g_dbFileName;
extern const char  *g_folderMapFileName;

#endif