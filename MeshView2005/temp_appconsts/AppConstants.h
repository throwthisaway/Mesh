#pragma once
#include <tchar.h>

class CAppConstants
{
public:
	// Scene related
	static const int MaxLights = 8;
	static const int DefLights = 2;
	// Global...
	static TCHAR * app_name;
	static const char * font_path;
	// Shader related...
	static const char * shader_dir;
	static const char * program_dir;
	static const char * programs;
    static const char * program_ext;
    static const char * fs_ext;
    static const char * vs_ext;
	// Image related...
	static char * tga;
	static char * bmp;
	static const char * img_fullpath_fragment;
	static const char * img_def_path;
	//Viewport related...
	static const float fovy;
	static const float znear, zfar;
    static const char * default_image;
};
