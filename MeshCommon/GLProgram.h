#pragma once
#include "GLExtensions.h"
#include "HelperMacros.h"
#include "w32types.h"
#include <string>
#include <map>
#define _SHADER_DEBUG
namespace Scene
{
class CGLProgram;

class CGLShader :
	public CGLExtensions
{	
	GLcharARB * lines, *log;
	GLsizei size;
	const GLenum type; // Shader type: GL_VERTEX_SHADER_ARB or GL_FRAGMENT_SHADER_ARB
	GLhandleARB attached; // Program handle the shader attached to

public:
    std::string _fname;
	CGLProgram& m_glProgram;
	GLhandleARB handle;	// Shader handle
	inline const GLenum GetType(void) { return type; };	
	inline void Attach(GLhandleARB program) { attached = program; glAttachObjectARB(program, handle); };
    void SetSource(const char * lines);
	GLsizei Load(const std::string& fname);
	BOOL Compile(void);
	void Cleanup(void);
	inline GLcharARB * GetLines(void) const { return lines; };
	inline GLcharARB * GetLog(void) const { return log; };
	inline LPCTSTR GetFName(void) const { return _fname.c_str(); };
	CGLShader(const GLenum _type, CGLProgram& glProgram) : 
		handle(0), 
		type(_type), 
		attached(0), 
		lines(NULL), 
		log(NULL), 
		size(0),
		m_glProgram(glProgram){};
	CGLShader::~CGLShader(void); 
    static void FixedFunc(void) { glUseProgramObjectARB(0); };
};
typedef enum eLOAD{LOAD_OK, LOAD_ERROR, COMPILE_ERROR, LINK_ERROR, NOT_SUPPORTED}LOAD;



typedef std::map<const std::string , CGLProgram *> ProgramMap;
class CGLProgram :
	public CGLExtensions
{
	std::string fname;
	GLcharARB * log;
    GLint _error;
public:
	GLhandleARB handle;	// Program handle
	CGLShader vs, fs; // Vertex and fragment shader
#pragma warning(push)
#pragma warning(disable: 4355) // 'this' : used in base member initializer list
	CGLProgram(void) : handle(0), vs(GL_VERTEX_SHADER_ARB, *this), fs(GL_FRAGMENT_SHADER_ARB, *this), log(NULL) {}
#pragma warning(pop)
	LOAD CreateAndLink();
	inline void UseProgram(void) { glUseProgramObjectARB(handle); };
	~CGLProgram(void) { Cleanup(); };
	// TODO:: clarify which load does what
	LOAD Load(const std::string& program_fname, const std::string& program_dir, const std::string& shader_dir);
	LOAD Load(const std::string& fullPath_vs, const std::string& fullPath_fs);
	static LOAD LoadPrograms(const std::string& program_list_fname, const std::string& program_dir, const std::string& shader_dir, ProgramMap& programs);
	inline const std::string& GetFName(void) const { return fname; };
	inline GLcharARB * GetLog(void) const { return log; };
private:
	void Cleanup(void);
};

}