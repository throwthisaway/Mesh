#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include "GLExtensions.h"
#include "Log.h"
#ifdef WIN32
#define GLGETPROCADDRESS wglGetProcAddress
#else
#define GLGETPROCADDRESS glXGetProcAddress
#endif
namespace Scene
{
bool CGLExtensions::VBOSupported = FALSE,
    CGLExtensions::MultiTextureSupported = FALSE, 
CGLExtensions::ShaderSupported = FALSE,
CGLExtensions::ImagingSupported = FALSE,
CGLExtensions::FBOSupported = FALSE,
CGLExtensions::init = false;
char * CGLExtensions::extensions = NULL;
// VBOs
PFNGLGENBUFFERSARBPROC CGLExtensions::glGenBuffersARB = NULL;
PFNGLDELETEBUFFERSARBPROC CGLExtensions::glDeleteBuffersARB = NULL;
PFNGLBINDBUFFERARBPROC CGLExtensions::glBindBufferARB = NULL;
PFNGLBUFFERDATAARBPROC CGLExtensions::glBufferDataARB = NULL;
PFNGLMAPBUFFERPROC CGLExtensions::glMapBufferARB = NULL;
//Multitextures
PFNGLMULTITEXCOORD1FARBPROC CGLExtensions::glMultiTexCoord1fARB = NULL;
PFNGLMULTITEXCOORD2FARBPROC CGLExtensions::glMultiTexCoord2fARB = NULL;
PFNGLMULTITEXCOORD3FARBPROC CGLExtensions::glMultiTexCoord3fARB = NULL;
PFNGLMULTITEXCOORD4FARBPROC CGLExtensions::glMultiTexCoord4fARB = NULL;
PFNGLACTIVETEXTUREARBPROC CGLExtensions::glActiveTextureARB = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC CGLExtensions::glClientActiveTextureARB = NULL;
GLint CGLExtensions::maxTexelUnits = 0;
GLint CGLExtensions::baseTexelUnit = GL_TEXTURE0_ARB;
// Shaders
PFNGLCREATESHADEROBJECTARBPROC CGLExtensions::glCreateShaderObjectARB = NULL;
PFNGLSHADERSOURCEARBPROC CGLExtensions::glShaderSourceARB = NULL;
PFNGLCOMPILESHADERARBPROC CGLExtensions::glCompileShaderARB = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC CGLExtensions::glCreateProgramObjectARB = NULL;
PFNGLATTACHOBJECTARBPROC CGLExtensions::glAttachObjectARB = NULL;
PFNGLLINKPROGRAMARBPROC CGLExtensions::glLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC CGLExtensions::glUseProgramObjectARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC CGLExtensions::glGetObjectParameterivARB = NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC CGLExtensions::glGetObjectParameterfvARB = NULL;
PFNGLGETINFOLOGARBPROC CGLExtensions::glGetInfoLogARB = NULL;
PFNGLDETACHOBJECTARBPROC CGLExtensions::glDetachObjectARB = NULL;
PFNGLDELETEOBJECTARBPROC CGLExtensions::glDeleteObjectARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC CGLExtensions::glGetUniformLocationARB = NULL;
PFNGLUNIFORM1FARBPROC CGLExtensions::glUniform1fARB = NULL;
PFNGLUNIFORM2FARBPROC CGLExtensions::glUniform2fARB = NULL;
PFNGLUNIFORM3FARBPROC CGLExtensions::glUniform3fARB = NULL;
PFNGLUNIFORM4FARBPROC CGLExtensions::glUniform4fARB = NULL;
PFNGLUNIFORM1IARBPROC CGLExtensions::glUniform1iARB = NULL;
PFNGLUNIFORM2IARBPROC CGLExtensions::glUniform2iARB = NULL;
PFNGLUNIFORM3IARBPROC CGLExtensions::glUniform3iARB = NULL;
PFNGLUNIFORM4IARBPROC CGLExtensions::glUniform4iARB = NULL;
PFNGLUNIFORM1FVARBPROC CGLExtensions::glUniform1fvARB = NULL;
PFNGLUNIFORM2FVARBPROC CGLExtensions::glUniform2fvARB = NULL;
PFNGLUNIFORM3FVARBPROC CGLExtensions::glUniform3fvARB = NULL;
PFNGLUNIFORM4FVARBPROC CGLExtensions::glUniform4fvARB = NULL;
PFNGLUNIFORM1IVARBPROC CGLExtensions::glUniform1ivARB = NULL;
PFNGLUNIFORM2IVARBPROC CGLExtensions::glUniform2ivARB = NULL;
PFNGLUNIFORM3IVARBPROC CGLExtensions::glUniform3ivARB = NULL;
PFNGLUNIFORM4IVARBPROC CGLExtensions::glUniform4ivARB = NULL;
PFNGLUNIFORMMATRIX2FVARBPROC CGLExtensions::glUniformMatrix2fvARB = NULL;
PFNGLUNIFORMMATRIX3FVARBPROC CGLExtensions::glUniformMatrix3fvARB = NULL;
PFNGLUNIFORMMATRIX4FVARBPROC CGLExtensions::glUniformMatrix4fvARB = NULL;
PFNGLGETATTRIBLOCATIONARBPROC CGLExtensions::glGetAttribLocationARB = NULL;
PFNGLVERTEXATTRIB1DARBPROC CGLExtensions::glVertexAttrib1dARB = NULL;
PFNGLVERTEXATTRIB1DVARBPROC CGLExtensions::glVertexAttrib1dvARB = NULL;
PFNGLVERTEXATTRIB1FARBPROC CGLExtensions::glVertexAttrib1fARB = NULL;
PFNGLVERTEXATTRIB1FVARBPROC CGLExtensions::glVertexAttrib1fvARB = NULL;
PFNGLVERTEXATTRIB1SARBPROC CGLExtensions::glVertexAttrib1sARB = NULL;
PFNGLVERTEXATTRIB1SVARBPROC CGLExtensions::glVertexAttrib1svARB = NULL;
PFNGLVERTEXATTRIB2DARBPROC CGLExtensions::glVertexAttrib2dARB = NULL;
PFNGLVERTEXATTRIB2DVARBPROC CGLExtensions::glVertexAttrib2dvARB = NULL;
PFNGLVERTEXATTRIB2FARBPROC CGLExtensions::glVertexAttrib2fARB = NULL;
PFNGLVERTEXATTRIB2FVARBPROC CGLExtensions::glVertexAttrib2fvARB = NULL;
PFNGLVERTEXATTRIB2SARBPROC CGLExtensions::glVertexAttrib2sARB = NULL;
PFNGLVERTEXATTRIB2SVARBPROC CGLExtensions::glVertexAttrib2svARB = NULL;
PFNGLVERTEXATTRIB3DARBPROC CGLExtensions::glVertexAttrib3dARB = NULL;
PFNGLVERTEXATTRIB3DVARBPROC CGLExtensions::glVertexAttrib3dvARB = NULL;
PFNGLVERTEXATTRIB3FARBPROC CGLExtensions::glVertexAttrib3fARB = NULL;
PFNGLVERTEXATTRIB3FVARBPROC CGLExtensions::glVertexAttrib3fvARB = NULL;
PFNGLVERTEXATTRIB3SARBPROC CGLExtensions::glVertexAttrib3sARB = NULL;
PFNGLVERTEXATTRIB3SVARBPROC CGLExtensions::glVertexAttrib3svARB = NULL;
PFNGLVERTEXATTRIB4NBVARBPROC CGLExtensions::glVertexAttrib4NbvARB = NULL;
PFNGLVERTEXATTRIB4NIVARBPROC CGLExtensions::glVertexAttrib4NivARB = NULL;
PFNGLVERTEXATTRIB4NSVARBPROC CGLExtensions::glVertexAttrib4NsvARB = NULL;
PFNGLVERTEXATTRIB4NUBARBPROC CGLExtensions::glVertexAttrib4NubARB = NULL;
PFNGLVERTEXATTRIB4NUBVARBPROC CGLExtensions::glVertexAttrib4NubvARB = NULL;
PFNGLVERTEXATTRIB4NUIVARBPROC CGLExtensions::glVertexAttrib4NuivARB = NULL;
PFNGLVERTEXATTRIB4NUSVARBPROC CGLExtensions::glVertexAttrib4NusvARB = NULL;
PFNGLVERTEXATTRIB4BVARBPROC CGLExtensions::glVertexAttrib4bvARB = NULL;
PFNGLVERTEXATTRIB4DARBPROC CGLExtensions::glVertexAttrib4dARB = NULL;
PFNGLVERTEXATTRIB4DVARBPROC CGLExtensions::glVertexAttrib4dvARB = NULL;
PFNGLVERTEXATTRIB4FARBPROC CGLExtensions::glVertexAttrib4fARB = NULL;
PFNGLVERTEXATTRIB4FVARBPROC CGLExtensions::glVertexAttrib4fvARB = NULL;
PFNGLVERTEXATTRIB4IVARBPROC CGLExtensions::glVertexAttrib4ivARB = NULL;
PFNGLVERTEXATTRIB4SARBPROC CGLExtensions::glVertexAttrib4sARB = NULL;
PFNGLVERTEXATTRIB4SVARBPROC CGLExtensions::glVertexAttrib4svARB = NULL;
PFNGLVERTEXATTRIB4UBVARBPROC CGLExtensions::glVertexAttrib4ubvARB = NULL;
PFNGLVERTEXATTRIB4UIVARBPROC CGLExtensions::glVertexAttrib4uivARB = NULL;
PFNGLVERTEXATTRIB4USVARBPROC CGLExtensions::glVertexAttrib4usvARB = NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC CGLExtensions::glVertexAttribPointerARB = NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC CGLExtensions::glEnableVertexAttribArrayARB = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC CGLExtensions::glDisableVertexAttribArrayARB = NULL;
PFNGLPROGRAMSTRINGARBPROC CGLExtensions::glProgramStringARB = NULL;
PFNGLBINDPROGRAMARBPROC CGLExtensions::glBindProgramARB = NULL;
PFNGLDELETEPROGRAMSARBPROC CGLExtensions::glDeleteProgramsARB = NULL;
PFNGLGENPROGRAMSARBPROC CGLExtensions::glGenProgramsARB = NULL;
PFNGLPROGRAMENVPARAMETER4DARBPROC CGLExtensions::glProgramEnvParameter4dARB = NULL;
PFNGLPROGRAMENVPARAMETER4DVARBPROC CGLExtensions::glProgramEnvParameter4dvARB = NULL;
PFNGLPROGRAMENVPARAMETER4FARBPROC CGLExtensions::glProgramEnvParameter4fARB = NULL;
PFNGLPROGRAMENVPARAMETER4FVARBPROC CGLExtensions::glProgramEnvParameter4fvARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4DARBPROC CGLExtensions::glProgramLocalParameter4dARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4DVARBPROC CGLExtensions::glProgramLocalParameter4dvARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC CGLExtensions::glProgramLocalParameter4fARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC CGLExtensions::glProgramLocalParameter4fvARB = NULL;
PFNGLGETPROGRAMENVPARAMETERDVARBPROC CGLExtensions::glGetProgramEnvParameterdvARB = NULL;
PFNGLGETPROGRAMENVPARAMETERFVARBPROC CGLExtensions::glGetProgramEnvParameterfvARB = NULL;
PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC CGLExtensions::glGetProgramLocalParameterdvARB = NULL;
PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC CGLExtensions::glGetProgramLocalParameterfvARB = NULL;
PFNGLGETPROGRAMIVARBPROC CGLExtensions::glGetProgramivARB = NULL;
PFNGLGETPROGRAMSTRINGARBPROC CGLExtensions::glGetProgramStringARB = NULL;
PFNGLGETVERTEXATTRIBDVARBPROC CGLExtensions::glGetVertexAttribdvARB = NULL;
PFNGLGETVERTEXATTRIBFVARBPROC CGLExtensions::glGetVertexAttribfvARB = NULL;
PFNGLGETVERTEXATTRIBIVARBPROC CGLExtensions::glGetVertexAttribivARB = NULL;
PFNGLGETVERTEXATTRIBPOINTERVARBPROC CGLExtensions::glGetVertexAttribPointervARB = NULL;
PFNGLISPROGRAMARBPROC CGLExtensions::glIsProgramARB = NULL;
// Imaging
PFNGLBLENDEQUATIONPROC CGLExtensions::glBlendEquation = NULL;
PFNGLBLENDEQUATIONSEPARATEPROC CGLExtensions::glBlendEquationSeparate = NULL;
// Frame Buffer Object
PFNGLGENFRAMEBUFFERSEXTPROC CGLExtensions::glGenFramebuffers;
PFNGLBINDFRAMEBUFFEREXTPROC CGLExtensions::glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSEXTPROC CGLExtensions::glDeleteFramebuffers;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC CGLExtensions::glCheckFramebufferStatus;
PFNGLDRAWBUFFERSPROC CGLExtensions::glDrawBuffers;
PFNGLFRAMEBUFFERTEXTURE2DPROC CGLExtensions::glFramebufferTexture2D;
// Render Buffer Objects
PFNGLBINDRENDERBUFFERPROC CGLExtensions::glBindRenderbuffer;
PFNGLDELETERENDERBUFFERSPROC CGLExtensions::glDeleteRenderbuffers;
PFNGLGENRENDERBUFFERSPROC CGLExtensions::glGenRenderbuffers;
PFNGLRENDERBUFFERSTORAGEPROC CGLExtensions::glRenderbufferStorage;
PFNGLGENERATEMIPMAPPROC CGLExtensions::glGenerateMipmap;
PFNGLFRAMEBUFFERRENDERBUFFERPROC CGLExtensions::glFramebufferRenderbuffer;
//CGLExtensions::CGLExtensions(void)
//{
//}
//
CGLExtensions::~CGLExtensions(void)
{
	if (extensions)
	{
		free(extensions);
		extensions = NULL;
	}
}
CGLExtensions::CGLExtensions(void)
{
	if (init)
		return;
	SetupARB_MULTITEXTURE();
	SetupARB_SHADER();
	//CGLScene::ShaderSupported = FALSE;
	SetupIMAGING();
	SetupFBO();
	SetupARB_VERTEX_BUFFER_OBJECT();
	init = true;
}
void CGLExtensions::DumpExtenstions()
{
	if (!GetExtensionStrings())
		return;
	size_t len;
	char * exts = new char[(len = strlen(extensions))+1];
	strcpy(exts, extensions);
	exts[len] = 0;
	char *p = strtok(exts, " ");
	Log::CLog::Write("Supported extensions:\r\n");
	while (p)
	{
		Log::CLog::Write("\t%s\r\n", p);
		p = strtok(NULL, " ");
	};
	delete [] exts;
}
bool CGLExtensions::GetExtensionStrings()
{
	if (!extensions)
	{
		extensions = _strdup((const char *) glGetString(GL_EXTENSIONS));
		if (!extensions)
		{
			Log::CLog::Write("Cannot get extension strings...\r\n");
			return false;
		}
		if (dumpExtensions)
			DumpExtenstions();
	}
	//Log::CLog::Write(extensions);
	return true;
}

bool CGLExtensions::IsExtensionSupported(const char * ext)
{
	if (!GetExtensionStrings())
		return false;
	char * p = extensions;
	size_t len = strlen(extensions), ext_len = strlen(ext), ext_pos = 0;
	while (*p)
	{
		while (*p && (*p == ext[ext_pos]) && (*p != ' '))
		{
			p++;
			ext_pos++;
		}
		if (ext_pos == ext_len)
			return true;
		while (*p && (*p != ' ')) p++;
		if (*p == ' ') p++;
		ext_pos = 0;
	}
	return false;
}

bool CGLExtensions::GetProcAddress(const char * ext, void ** fct)
{
	return true;
}

bool CGLExtensions::SetupARB_VERTEX_BUFFER_OBJECT()
{
    if (VBOSupported)
        return true;
	if (VBOSupported = IsExtensionSupported(GL_ARB_VERTEX_BUFFER_OBJECT))
	{		
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) GLGETPROCADDRESS("glGenBuffersARB");	
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC) GLGETPROCADDRESS("glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC) GLGETPROCADDRESS("glBufferDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) GLGETPROCADDRESS("glDeleteBuffersARB");
		glMapBufferARB = (PFNGLMAPBUFFERPROC) GLGETPROCADDRESS("glMapBufferARB");
		Log::CLog::Write("VBO supported...\r\n");		
	}
	else
		Log::CLog::Write("VBO not supported...\r\n");
	return VBOSupported;
}

bool CGLExtensions::SetupARB_MULTITEXTURE()
{
    if (MultiTextureSupported)
        return true;
	// TODO: finish...
	if (MultiTextureSupported = (IsExtensionSupported(GL_ARB_MULTITEXTURE) && IsExtensionSupported(GL_EXT_TEXTURE_ENV_COMBINE)))
	{
		glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) GLGETPROCADDRESS("glMultiTexCoord1fARB");
		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) GLGETPROCADDRESS("glMultiTexCoord2fARB");
		glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) GLGETPROCADDRESS("glMultiTexCoord3fARB");
		glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC) GLGETPROCADDRESS("glMultiTexCoord4fARB");
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) GLGETPROCADDRESS("glActiveTextureARB");
		glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) GLGETPROCADDRESS("glClientActiveTextureARB");
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxTexelUnits);
		Log::CLog::Write("Multitexture extension supported... Max texture units: %d\r\n", maxTexelUnits);
	}
	else
		Log::CLog::Write("Multitexture extension not supported...\r\n");
	return MultiTextureSupported;
}
bool CGLExtensions::SetupARB_SHADER()
{
    if (ShaderSupported)
        return true;
	if (ShaderSupported = ((IsExtensionSupported(GL_EXT_VERTEX_SHADER) || IsExtensionSupported(GL_ARB_VERTEX_SHADER)) && IsExtensionSupported(GL_ARB_SHADER_OBJECTS) && IsExtensionSupported(GL_ARB_SHADING_LANGUAGE_100)))
	{		
		glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC) GLGETPROCADDRESS("glCreateShaderObjectARB");
		glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) GLGETPROCADDRESS("glShaderSourceARB");
		glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC) GLGETPROCADDRESS("glCompileShaderARB");
		glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) GLGETPROCADDRESS("glCreateProgramObjectARB");
		glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC) GLGETPROCADDRESS("glAttachObjectARB");
		glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) GLGETPROCADDRESS("glLinkProgramARB");
		glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC) GLGETPROCADDRESS("glUseProgramObjectARB");
		glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) GLGETPROCADDRESS("glGetObjectParameterivARB");
		glGetObjectParameterfvARB = (PFNGLGETOBJECTPARAMETERFVARBPROC) GLGETPROCADDRESS("glGetObjectParameterfvARB");
		glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) GLGETPROCADDRESS("glGetInfoLogARB");
		glDetachObjectARB = (PFNGLDETACHOBJECTARBPROC) GLGETPROCADDRESS("glDetachObjectARB");
		glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC) GLGETPROCADDRESS("glDeleteObjectARB");
		glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC) GLGETPROCADDRESS("glGetUniformLocationARB");
		glUniform1fARB = (PFNGLUNIFORM1FARBPROC) GLGETPROCADDRESS("glUniform1fARB");
		glUniform2fARB = (PFNGLUNIFORM2FARBPROC) GLGETPROCADDRESS("glUniform2fARB");
		glUniform3fARB = (PFNGLUNIFORM3FARBPROC) GLGETPROCADDRESS("glUniform3fARB");
		glUniform4fARB = (PFNGLUNIFORM4FARBPROC) GLGETPROCADDRESS("glUniform4fARB");
		glUniform1iARB = (PFNGLUNIFORM1IARBPROC) GLGETPROCADDRESS("glUniform1iARB");
		glUniform2iARB = (PFNGLUNIFORM2IARBPROC) GLGETPROCADDRESS("glUniform2iARB");
		glUniform3iARB = (PFNGLUNIFORM3IARBPROC) GLGETPROCADDRESS("glUniform3iARB");
		glUniform4iARB = (PFNGLUNIFORM4IARBPROC) GLGETPROCADDRESS("glUniform4iARB");
		glUniform1fvARB = (PFNGLUNIFORM1FVARBPROC) GLGETPROCADDRESS("glUniform1fvARB");
		glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC) GLGETPROCADDRESS("glUniform2fvARB");
		glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC) GLGETPROCADDRESS("glUniform3fvARB");
		glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC) GLGETPROCADDRESS("glUniform4fvARB");
		glUniform1ivARB = (PFNGLUNIFORM1IVARBPROC) GLGETPROCADDRESS("glUniform1ivARB");
		glUniform2ivARB = (PFNGLUNIFORM2IVARBPROC) GLGETPROCADDRESS("glUniform2ivARB");
		glUniform3ivARB = (PFNGLUNIFORM3IVARBPROC) GLGETPROCADDRESS("glUniform3ivARB");
		glUniform4ivARB = (PFNGLUNIFORM4IVARBPROC) GLGETPROCADDRESS("glUniform4ivARB");
		glUniformMatrix2fvARB = (PFNGLUNIFORMMATRIX2FVARBPROC) GLGETPROCADDRESS("glUniformMatrix2fvARB");
		glUniformMatrix3fvARB = (PFNGLUNIFORMMATRIX3FVARBPROC) GLGETPROCADDRESS("glUniformMatrix3fvARB");
		glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC) GLGETPROCADDRESS("glUniformMatrix4fvARB");
		glVertexAttrib1dARB = (PFNGLVERTEXATTRIB1DARBPROC) GLGETPROCADDRESS("glVertexAttrib1dARB");
		glVertexAttrib1dvARB = (PFNGLVERTEXATTRIB1DVARBPROC) GLGETPROCADDRESS("glVertexAttrib1dvARB");
		glVertexAttrib1fARB = (PFNGLVERTEXATTRIB1FARBPROC) GLGETPROCADDRESS("glVertexAttrib1fARB");
		glVertexAttrib1fvARB = (PFNGLVERTEXATTRIB1FVARBPROC) GLGETPROCADDRESS("glVertexAttrib1fvARB");
		glVertexAttrib1sARB = (PFNGLVERTEXATTRIB1SARBPROC) GLGETPROCADDRESS("glVertexAttrib1sARB");
		glVertexAttrib1svARB = (PFNGLVERTEXATTRIB1SVARBPROC) GLGETPROCADDRESS("glVertexAttrib1svARB");
		glVertexAttrib2dARB = (PFNGLVERTEXATTRIB2DARBPROC) GLGETPROCADDRESS("glVertexAttrib2dARB");
		glVertexAttrib2dvARB = (PFNGLVERTEXATTRIB2DVARBPROC) GLGETPROCADDRESS("glVertexAttrib2dvARB");
		glVertexAttrib2fARB = (PFNGLVERTEXATTRIB2FARBPROC) GLGETPROCADDRESS("glVertexAttrib2fARB");
		glVertexAttrib2fvARB = (PFNGLVERTEXATTRIB2FVARBPROC) GLGETPROCADDRESS("glVertexAttrib2fvARB");
		glVertexAttrib2sARB = (PFNGLVERTEXATTRIB2SARBPROC) GLGETPROCADDRESS("glVertexAttrib2sARB");
		glVertexAttrib2svARB = (PFNGLVERTEXATTRIB2SVARBPROC) GLGETPROCADDRESS("glVertexAttrib2svARB");
		glVertexAttrib3dARB = (PFNGLVERTEXATTRIB3DARBPROC) GLGETPROCADDRESS("glVertexAttrib3dARB");
		glVertexAttrib3dvARB = (PFNGLVERTEXATTRIB3DVARBPROC) GLGETPROCADDRESS("glVertexAttrib3dvARB");
		glVertexAttrib3fARB = (PFNGLVERTEXATTRIB3FARBPROC) GLGETPROCADDRESS("glVertexAttrib3fARB");
		glVertexAttrib3fvARB = (PFNGLVERTEXATTRIB3FVARBPROC) GLGETPROCADDRESS("glVertexAttrib3fvARB");
		glVertexAttrib3sARB = (PFNGLVERTEXATTRIB3SARBPROC) GLGETPROCADDRESS("glVertexAttrib3sARB");
		glVertexAttrib3svARB = (PFNGLVERTEXATTRIB3SVARBPROC) GLGETPROCADDRESS("glVertexAttrib3svARB");
		glVertexAttrib4NbvARB = (PFNGLVERTEXATTRIB4NBVARBPROC) GLGETPROCADDRESS("glVertexAttrib4NbvARB");
		glVertexAttrib4NivARB = (PFNGLVERTEXATTRIB4NIVARBPROC) GLGETPROCADDRESS("glVertexAttrib4NivARB");
		glVertexAttrib4NsvARB = (PFNGLVERTEXATTRIB4NSVARBPROC) GLGETPROCADDRESS("glVertexAttrib4NsvARB");
		glVertexAttrib4NubARB = (PFNGLVERTEXATTRIB4NUBARBPROC) GLGETPROCADDRESS("glVertexAttrib4NubARB");
		glVertexAttrib4NubvARB = (PFNGLVERTEXATTRIB4NUBVARBPROC) GLGETPROCADDRESS("glVertexAttrib4NubvARB");
		glVertexAttrib4NuivARB = (PFNGLVERTEXATTRIB4NUIVARBPROC) GLGETPROCADDRESS("glVertexAttrib4NuivARB");
		glVertexAttrib4NusvARB = (PFNGLVERTEXATTRIB4NUSVARBPROC) GLGETPROCADDRESS("glVertexAttrib4NusvARB");
		glVertexAttrib4bvARB = (PFNGLVERTEXATTRIB4BVARBPROC) GLGETPROCADDRESS("glVertexAttrib4bvARB");
		glVertexAttrib4dARB = (PFNGLVERTEXATTRIB4DARBPROC) GLGETPROCADDRESS("glVertexAttrib4dARB");
		glVertexAttrib4dvARB = (PFNGLVERTEXATTRIB4DVARBPROC) GLGETPROCADDRESS("glVertexAttrib4dvARB");
		glVertexAttrib4fARB = (PFNGLVERTEXATTRIB4FARBPROC) GLGETPROCADDRESS("glVertexAttrib4fARB");
		glVertexAttrib4fvARB = (PFNGLVERTEXATTRIB4FVARBPROC) GLGETPROCADDRESS("glVertexAttrib4fvARB");
		glVertexAttrib4ivARB = (PFNGLVERTEXATTRIB4IVARBPROC) GLGETPROCADDRESS("glVertexAttrib4ivARB");
		glVertexAttrib4sARB = (PFNGLVERTEXATTRIB4SARBPROC) GLGETPROCADDRESS("glVertexAttrib4sARB");
		glVertexAttrib4svARB = (PFNGLVERTEXATTRIB4SVARBPROC) GLGETPROCADDRESS("glVertexAttrib4svARB");
		glVertexAttrib4ubvARB = (PFNGLVERTEXATTRIB4UBVARBPROC) GLGETPROCADDRESS("glVertexAttrib4ubvARB");
		glVertexAttrib4uivARB = (PFNGLVERTEXATTRIB4UIVARBPROC) GLGETPROCADDRESS("glVertexAttrib4uivARB");
		glVertexAttrib4usvARB = (PFNGLVERTEXATTRIB4USVARBPROC) GLGETPROCADDRESS("glVertexAttrib4usvARB");
		glVertexAttribPointerARB = (PFNGLVERTEXATTRIBPOINTERARBPROC) GLGETPROCADDRESS("glVertexAttribPointerARB");
		glEnableVertexAttribArrayARB = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC) GLGETPROCADDRESS("glEnableVertexAttribArrayARB");
		glDisableVertexAttribArrayARB = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) GLGETPROCADDRESS("glDisableVertexAttribArrayARB");
		glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC) GLGETPROCADDRESS("glProgramStringARB");
		glBindProgramARB = (PFNGLBINDPROGRAMARBPROC) GLGETPROCADDRESS("glBindProgramARB");
		glDeleteProgramsARB = (PFNGLDELETEPROGRAMSARBPROC) GLGETPROCADDRESS("glDeleteProgramsARB");
		glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC) GLGETPROCADDRESS("glGenProgramsARB");
		glProgramEnvParameter4dARB = (PFNGLPROGRAMENVPARAMETER4DARBPROC) GLGETPROCADDRESS("glProgramEnvParameter4dARB");
		glProgramEnvParameter4dvARB = (PFNGLPROGRAMENVPARAMETER4DVARBPROC) GLGETPROCADDRESS("glProgramEnvParameter4dvARB");
		glProgramEnvParameter4fARB = (PFNGLPROGRAMENVPARAMETER4FARBPROC) GLGETPROCADDRESS("glProgramEnvParameter4fARB");
		glProgramEnvParameter4fvARB = (PFNGLPROGRAMENVPARAMETER4FVARBPROC) GLGETPROCADDRESS("glProgramEnvParameter4fvARB");
		glProgramLocalParameter4dARB = (PFNGLPROGRAMLOCALPARAMETER4DARBPROC) GLGETPROCADDRESS("glProgramLocalParameter4dARB");
		glProgramLocalParameter4dvARB = (PFNGLPROGRAMLOCALPARAMETER4DVARBPROC) GLGETPROCADDRESS("glProgramLocalParameter4dvARB");
		glProgramLocalParameter4fARB = (PFNGLPROGRAMLOCALPARAMETER4FARBPROC) GLGETPROCADDRESS("glProgramLocalParameter4fARB");
		glProgramLocalParameter4fvARB = (PFNGLPROGRAMLOCALPARAMETER4FVARBPROC) GLGETPROCADDRESS("glProgramLocalParameter4fvARB");
		glGetProgramEnvParameterdvARB = (PFNGLGETPROGRAMENVPARAMETERDVARBPROC) GLGETPROCADDRESS("glGetProgramEnvParameterdvARB");
		glGetProgramEnvParameterfvARB = (PFNGLGETPROGRAMENVPARAMETERFVARBPROC) GLGETPROCADDRESS("glGetProgramEnvParameterfvARB");
		glGetProgramLocalParameterdvARB = (PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC) GLGETPROCADDRESS("glGetProgramLocalParameterdvARB");
		glGetProgramLocalParameterfvARB = (PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC) GLGETPROCADDRESS("glGetProgramLocalParameterfvARB");
		glGetProgramivARB = (PFNGLGETPROGRAMIVARBPROC) GLGETPROCADDRESS("glGetProgramivARB");
		glGetProgramStringARB = (PFNGLGETPROGRAMSTRINGARBPROC) GLGETPROCADDRESS("glGetProgramStringARB");
		glGetVertexAttribdvARB = (PFNGLGETVERTEXATTRIBDVARBPROC) GLGETPROCADDRESS("glGetVertexAttribdvARB");
		glGetVertexAttribfvARB = (PFNGLGETVERTEXATTRIBFVARBPROC) GLGETPROCADDRESS("glGetVertexAttribfvARB");
		glGetVertexAttribivARB = (PFNGLGETVERTEXATTRIBIVARBPROC) GLGETPROCADDRESS("glGetVertexAttribivARB");
		glGetVertexAttribPointervARB = (PFNGLGETVERTEXATTRIBPOINTERVARBPROC) GLGETPROCADDRESS("glGetVertexAttribPointervARB");
		glIsProgramARB = (PFNGLISPROGRAMARBPROC) GLGETPROCADDRESS("glIsProgramARB");
	}
	return ShaderSupported;
}
bool CGLExtensions::SetupIMAGING()
{
    if (ImagingSupported)
        return true;
	if (ImagingSupported = IsExtensionSupported(GL_ARB_IMAGING))
	{		
		glBlendEquation = (PFNGLBLENDEQUATIONPROC) GLGETPROCADDRESS("glBlendEquation");
		Log::CLog::Write("Imaging supported...\r\n");		
	}
	else
		Log::CLog::Write("Imaging not supported...\r\n");
	if (ImagingSupported = IsExtensionSupported(GL_EXT_BLEND_EQUATION_SEPARATE))
	{		
		glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC) GLGETPROCADDRESS("glBlendEquationSeparate");	
		Log::CLog::Write("GL_EXT_blend_equation_separate supported...\r\n");		
	}
	else
		Log::CLog::Write("GL_EXT_blend_equation_separate not supported...\r\n");
	return ImagingSupported;
}
bool CGLExtensions::SetupFBO(void)
{
    if (FBOSupported)
        return true;
    if (FBOSupported = IsExtensionSupported(GL_ARB_FRAMEBUFFER_OBJECT))
    {
        // Frame Buffer Objects
        glGenFramebuffers = (PFNGLGENFRAMEBUFFERSEXTPROC)GLGETPROCADDRESS("glGenFramebuffers");
        _ASSERT(glGenFramebuffers);
        glBindFramebuffer = (PFNGLBINDFRAMEBUFFEREXTPROC)GLGETPROCADDRESS("glBindFramebuffer");
        _ASSERT(glBindFramebuffer);
        glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSEXTPROC)GLGETPROCADDRESS("glDeleteFramebuffers");
        _ASSERT(glDeleteFramebuffers);
        glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)GLGETPROCADDRESS("glCheckFramebufferStatus");
        _ASSERT(glCheckFramebufferStatus);
        glDrawBuffers = (PFNGLDRAWBUFFERSPROC)GLGETPROCADDRESS("glDrawBuffers");
        _ASSERT(glDrawBuffers);
        glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)GLGETPROCADDRESS("glFramebufferTexture2D");
        _ASSERT(glFramebufferTexture2D);
        // Render Buffer Objects
        glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)GLGETPROCADDRESS("glBindRenderbuffer");
        _ASSERT(glBindRenderbuffer);
        glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)GLGETPROCADDRESS("glDeleteRenderbuffers");
        _ASSERT(glDeleteRenderbuffers);
        glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)GLGETPROCADDRESS("glGenRenderbuffers");
        _ASSERT(glGenRenderbuffers);
        glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)GLGETPROCADDRESS("glRenderbufferStorage");
        _ASSERT(glRenderbufferStorage);
        glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)GLGETPROCADDRESS("glGenerateMipmap");
        _ASSERT(glRenderbufferStorage);
        glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)GLGETPROCADDRESS("glFramebufferRenderbuffer");
        _ASSERT(glRenderbufferStorage);
        Log::CLog::Write("Framebuffer Objects supported...\r\n");		
    }
    else
    {
		Log::CLog::Write("Framebuffer Objects not supported...\r\n");
        return false;
    }
    return true;
}
}
