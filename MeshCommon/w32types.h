#ifndef W32TYPES_H_
#define W32TYPES_H_

//TODO: make it linux compliant
#ifndef WIN32
#define MAX_PATH 260
typedef int BOOL;
typedef char BYTE;
typedef unsigned int UINT;
#define FALSE false
#define TRUE true
char * strlwr(char * s);
typedef int HWND;
typedef int HDC;
typedef int HINSTANCE;
typedef void * WNDPROC
#else
#include <windows.h>
#endif
#endif /*W32TYPES_H_*/
