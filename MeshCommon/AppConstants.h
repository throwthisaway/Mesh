#pragma once
#include <string>
#include <tchar.h>

class CAppConstants
{
public:
	static const int MaxLights = 8;
	static const int DefLights = 2;
	static const int DefWindowWidth = 640;
	static const int DefWindowHeight = 480;
	static const int DefBpp = 32;
	static const char * font_path;
	// Shader related...
	static const char * shader_dir;
	static const char * program_dir;
	static const char * programs;
    static const char * program_ext;
    static const char * fs_ext;
    static const char * vs_ext;
    //static const std::string cwd;
	// Image related...
	static char * tga;
	static char * bmp;
	static const char * img_fullpath_fragment;
	static const char * img_def_path;
    static const char * default_image;
	//Viewport related...
	static const float fovy;
	static const float znear, zfar;
	// Threading related...
	static const int DefWaitForHandle = 1000;
	static const int WaitForHandleReleaseObjects = 25000;
	// Smoothing...
	static const float Smooth;
	static const float Flat;
    static TCHAR * app_name;
};
