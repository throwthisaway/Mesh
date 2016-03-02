#include "AppConstants.h"
#include "File.h"

//#define BASE_DIR "../MeshView2005/"
//
//const char * CAppConstants::shader_dir = BASE_DIR"./Shaders/";
//const char * CAppConstants::program_dir = BASE_DIR"./Shaders/";
//const char * CAppConstants::programs = BASE_DIR"./Shaders/""programs";
//char * CAppConstants::tga = "tga";
//const char * CAppConstants::img_fullpath_fragment = "\\Images\\";
//const char * CAppConstants::img_def_path = "../../imgs/";
//const float CAppConstants::fovy = 45.0f;
//const float CAppConstants::znear = 0.1f;
//const float CAppConstants::zfar = 1000.0f;
//const float CAppConstants::Smooth = 1.5620697f;
//const float CAppConstants::Flat = 0.f;
#define BASE_DIR 
#define IMAGE_DIR ".\\Images\\"

const char * CAppConstants::shader_dir = BASE_DIR".\\Shaders\\";
const char * CAppConstants::program_dir = BASE_DIR".\\Shaders\\";
const char * CAppConstants::programs = BASE_DIR".\\Shaders\\""programs";
const char * CAppConstants::program_ext = "program";
const char * CAppConstants::fs_ext = "fshader";
const char * CAppConstants::vs_ext = "vshader";
char * CAppConstants::tga = "tga";
char * CAppConstants::bmp = "bmp";
const char * CAppConstants::img_fullpath_fragment = "\\Images\\";
const char * CAppConstants::img_def_path = IMAGE_DIR;
const char * CAppConstants::default_image = IMAGE_DIR"default.tga";
const char * CAppConstants::font_path = ".\\Data\\Font\\font.bmp";
const float CAppConstants::fovy = 45.0f;
const float CAppConstants::znear = 0.1f;
const float CAppConstants::zfar = 1000.0f;
const float CAppConstants::Smooth = 1.5620697f;
const float CAppConstants::Flat = 0.f;
TCHAR * CAppConstants::app_name = _T("undefinedGLApp");
//const std::string CAppConstants::cwd(CFileSystem::GetCWD(cwd, MAX_PATH));