#pragma once
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#ifdef WIN32
//#include <GL/glaux.h>
#else
#include <GL/glx.h>
#endif
#include "glext.h"

namespace Scene
{

#define GL_ARB_MULTITEXTURE "GL_ARB_multitexture"
#define GL_EXT_TEXTURE_ENV_COMBINE "GL_EXT_texture_env_combine"
#define GL_ARB_VERTEX_BUFFER_OBJECT "GL_ARB_vertex_buffer_object"
#define GL_ARB_FRAGMENT_SHADER "GL_ARB_fragment_shader"
#define GL_ARB_VERTEX_SHADER "GL_ARB_vertex_shader"
#define GL_ARB_SHADER_OBJECTS "GL_ARB_shader_objects"
#define GL_ARB_SHADING_LANGUAGE_100 "GL_ARB_shading_language_100"
#define GL_EXT_VERTEX_SHADER "GL_EXT_vertex_shader"
#define GL_ATI_FRAGMENT_SHADER "GL_ATI_fragment_shader"
#define GL_ARB_IMAGING "GL_ARB_imaging"
#define GL_EXT_BLEND_EQUATION_SEPARATE "GL_EXT_blend_equation_separate"
#define GL_ARB_FRAMEBUFFER_OBJECT "GL_ARB_framebuffer_object"
class CGLExtensions
{
	static char * extensions;
	static const bool dumpExtensions = false;
	static bool GetExtensionStrings();
public:
	static bool VBOSupported,
        MultiTextureSupported,
        ShaderSupported,
        ImagingSupported,
		FBOSupported,
		init;
protected:	
	CGLExtensions(void);
	~CGLExtensions(void);
	// VBOs
	static PFNGLGENBUFFERSARBPROC glGenBuffersARB;
	static PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
	static PFNGLBINDBUFFERARBPROC glBindBufferARB;
	static PFNGLBUFFERDATAARBPROC glBufferDataARB;
	static PFNGLMAPBUFFERPROC glMapBufferARB;
	// Multitextures
	static GLint maxTexelUnits, baseTexelUnit;
	static PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB;
	static PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
	static PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB;
	static PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB;
	static PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
	static PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
	// Shaders
	static PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
	static PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
	static PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
	static PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
	static PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
	static PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
	static PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
	static PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
	static PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
	static PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
	static PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
	static PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
	static PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
	static PFNGLUNIFORM1FARBPROC glUniform1fARB;
	static PFNGLUNIFORM2FARBPROC glUniform2fARB;
	static PFNGLUNIFORM3FARBPROC glUniform3fARB;
	static PFNGLUNIFORM4FARBPROC glUniform4fARB;
	static PFNGLUNIFORM1IARBPROC glUniform1iARB;
	static PFNGLUNIFORM2IARBPROC glUniform2iARB;
	static PFNGLUNIFORM3IARBPROC glUniform3iARB;
	static PFNGLUNIFORM4IARBPROC glUniform4iARB;
	static PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
	static PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
	static PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
	static PFNGLUNIFORM4FVARBPROC glUniform4fvARB;
	static PFNGLUNIFORM1IVARBPROC glUniform1ivARB;
	static PFNGLUNIFORM2IVARBPROC glUniform2ivARB;
	static PFNGLUNIFORM3IVARBPROC glUniform3ivARB;
	static PFNGLUNIFORM4IVARBPROC glUniform4ivARB;
	static PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB;
	static PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB;
	static PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB;
	static PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocationARB;
	static PFNGLVERTEXATTRIB1DARBPROC glVertexAttrib1dARB;
	static PFNGLVERTEXATTRIB1DVARBPROC glVertexAttrib1dvARB;
	static PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB;
	static PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB;
	static PFNGLVERTEXATTRIB1SARBPROC glVertexAttrib1sARB;
	static PFNGLVERTEXATTRIB1SVARBPROC glVertexAttrib1svARB;
	static PFNGLVERTEXATTRIB2DARBPROC glVertexAttrib2dARB;
	static PFNGLVERTEXATTRIB2DVARBPROC glVertexAttrib2dvARB;
	static PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB;
	static PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB;
	static PFNGLVERTEXATTRIB2SARBPROC glVertexAttrib2sARB;
	static PFNGLVERTEXATTRIB2SVARBPROC glVertexAttrib2svARB;
	static PFNGLVERTEXATTRIB3DARBPROC glVertexAttrib3dARB;
	static PFNGLVERTEXATTRIB3DVARBPROC glVertexAttrib3dvARB;
	static PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB;
	static PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB;
	static PFNGLVERTEXATTRIB3SARBPROC glVertexAttrib3sARB;
	static PFNGLVERTEXATTRIB3SVARBPROC glVertexAttrib3svARB;
	static PFNGLVERTEXATTRIB4NBVARBPROC glVertexAttrib4NbvARB;
	static PFNGLVERTEXATTRIB4NIVARBPROC glVertexAttrib4NivARB;
	static PFNGLVERTEXATTRIB4NSVARBPROC glVertexAttrib4NsvARB;
	static PFNGLVERTEXATTRIB4NUBARBPROC glVertexAttrib4NubARB;
	static PFNGLVERTEXATTRIB4NUBVARBPROC glVertexAttrib4NubvARB;
	static PFNGLVERTEXATTRIB4NUIVARBPROC glVertexAttrib4NuivARB;
	static PFNGLVERTEXATTRIB4NUSVARBPROC glVertexAttrib4NusvARB;
	static PFNGLVERTEXATTRIB4BVARBPROC glVertexAttrib4bvARB;
	static PFNGLVERTEXATTRIB4DARBPROC glVertexAttrib4dARB;
	static PFNGLVERTEXATTRIB4DVARBPROC glVertexAttrib4dvARB;
	static PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB;
	static PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB;
	static PFNGLVERTEXATTRIB4IVARBPROC glVertexAttrib4ivARB;
	static PFNGLVERTEXATTRIB4SARBPROC glVertexAttrib4sARB;
	static PFNGLVERTEXATTRIB4SVARBPROC glVertexAttrib4svARB;
	static PFNGLVERTEXATTRIB4UBVARBPROC glVertexAttrib4ubvARB;
	static PFNGLVERTEXATTRIB4UIVARBPROC glVertexAttrib4uivARB;
	static PFNGLVERTEXATTRIB4USVARBPROC glVertexAttrib4usvARB;
	static PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;
	static PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
	static PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;
	static PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
	static PFNGLBINDPROGRAMARBPROC glBindProgramARB;
	static PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
	static PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
	static PFNGLPROGRAMENVPARAMETER4DARBPROC glProgramEnvParameter4dARB;
	static PFNGLPROGRAMENVPARAMETER4DVARBPROC glProgramEnvParameter4dvARB;
	static PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB;
	static PFNGLPROGRAMENVPARAMETER4FVARBPROC glProgramEnvParameter4fvARB;
	static PFNGLPROGRAMLOCALPARAMETER4DARBPROC glProgramLocalParameter4dARB;
	static PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB;
	static PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
	static PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
	static PFNGLGETPROGRAMENVPARAMETERDVARBPROC glGetProgramEnvParameterdvARB;
	static PFNGLGETPROGRAMENVPARAMETERFVARBPROC glGetProgramEnvParameterfvARB;
	static PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glGetProgramLocalParameterdvARB;
	static PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glGetProgramLocalParameterfvARB;
	static PFNGLGETPROGRAMIVARBPROC glGetProgramivARB;
	static PFNGLGETPROGRAMSTRINGARBPROC glGetProgramStringARB;
	static PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdvARB;
	static PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfvARB;
	static PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribivARB;
	static PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointervARB;
	static PFNGLISPROGRAMARBPROC glIsProgramARB;
	// Imaging
	static PFNGLBLENDEQUATIONPROC glBlendEquation;
	static PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
    // Frame Buffer Objects
    static PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffers;
    static PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebuffer;
    static PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffers;
    static PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatus;
    static PFNGLDRAWBUFFERSPROC glDrawBuffers;
    static PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
    // Render Buffer Objects
    static PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
    static PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
    static PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
    static PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
    static PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
    static PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
	//
	static bool IsExtensionSupported(const char *);
	static bool GetProcAddress(const char *, void **);
	static bool SetupARB_VERTEX_BUFFER_OBJECT(void);
	static bool SetupARB_MULTITEXTURE(void);
	static bool SetupARB_SHADER(void);
	static bool SetupIMAGING(void);
    static bool SetupFBO(void);
	static void DumpExtenstions();
};

}