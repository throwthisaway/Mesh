#include "GLProgram.h"
#include "FileReader.h"
#include "Log.h"
#include "Path.h"
#include <string>
namespace Scene
{
#define WHITE_SPACES " \t"
#define WHITE_SPACES_AND_SEMICOLON WHITE_SPACES";"
#define COMMENT '#'
	static LPCTSTR ltrim(LPCTSTR in, LPCTSTR chrs)
{
	while (!*in && _tcschr(chrs,*in)) in++;
	return in;
}

static const char * rtrim(const char * in, const char *chrs)
{
	if (!in || !*in)
		return in;
	char *p = (char*)in;
	p+=strlen(in);p--;
	while (p>in && *p && strchr(chrs,*p)) { *p = 0; --p; };
	return in;
}

static const char * GetFileName(const char * in)
{
	return rtrim(ltrim(in,WHITE_SPACES),WHITE_SPACES_AND_SEMICOLON"\r\r\n");
}

static BOOL EmptyOrComment(LPCTSTR in)
{
	LPCTSTR p = ltrim(in, WHITE_SPACES);
	if (!*p || *p == COMMENT)
		return TRUE;
	return FALSE;
}

CGLShader::~CGLShader(void) 
{ 
	Cleanup();
    DEL_ARRAY(lines);
}

void CGLShader::Cleanup(void)
{
	if (attached)
    {
		glDetachObjectARB(attached, handle);
        attached = FALSE;
    }
	if (handle)
    {
		glDeleteObjectARB(handle);
        handle = 0;
    }
	DEL_ARRAY(log);
}

void CGLShader::SetSource(const char * src)
{
    DEL_ARRAY(lines);
	lines = new char[strlen(src) + 1];
    strcpy_s(lines, strlen(src) + 1, src);
}

GLsizei CGLShader::Load(const std::string& fname)
{
	_fname = fname;
	IO::CFileReader * fl = NULL;
	try
	{	
        fl = IO::CFileReader::Open(_fname.c_str());
		DEL_ARRAY(lines);
		lines = new char[(size = fl->Size()) + 1];
		fl->Read(lines, size);
		lines[size] = 0;
		DEL(fl);
	}catch(IO::CFileNotFoundException& ex)
	{
		DEL(fl);
		Log::CLog::Write(_T("%s\n"),ex.msg.c_str());
		return 0;
	}
	return size;
}

BOOL CGLShader::Compile()
{
	if (!ShaderSupported)
	{
        Log::CLog::Write(_T("CGLShader::Compile %s: Shader extensions are not supported...\r\n"), _fname.c_str());
		return FALSE;
	}
	Cleanup();
	handle = glCreateShaderObjectARB(type);
	glShaderSourceARB(handle, 1,(const GLcharARB**)&lines, NULL);
	glCompileShaderARB(handle);	
	// Error check
	GLint status;
	glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &status);
	if (!status)
	{    
		GLint len, act_len;
		glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &len);
		DEL_ARRAY(log);
		log = new GLcharARB[len + 1];
		glGetInfoLogARB(handle, len, &act_len, log);
		log[act_len] = 0;
		Log::CLog::Write(_T("CGLShader::Compile %s:\r\n\t%s\r\n"),_fname.c_str(), log);
		Cleanup();
		return FALSE;
	}
	Log::CLog::Write(_T("CGLShader::Compile: %s compiled successfully...\r\n"), _fname.c_str());
	return TRUE;
}

LOAD CGLProgram::Load(const std::string& vs_fname, const std::string& fs_fname)
{
	if (!vs.Load(vs_fname))
	{
        Log::CLog::Write(_T("CGLProgram::Load: %s vertex shader load failed...\r\n"), vs_fname.c_str());
		return LOAD_ERROR;
	}
	
	if (!fs.Load(fs_fname))
	{
        Log::CLog::Write(_T("CGLProgram::Load: %s fragment shader load failed...\r\n"), fs_fname.c_str());
		return LOAD_ERROR;
	}
	return LOAD_OK;
}
void CGLProgram::Cleanup()
{
	DEL_ARRAY(log);
	vs.Cleanup();
	fs.Cleanup();
	if (handle)
    {
		glDeleteObjectARB(handle);
        handle = 0;
    }
}

LOAD CGLProgram::CreateAndLink()
{
	if (!ShaderSupported)
	{
		Log::CLog::Write(_T("CGLProgram::CreateAndLink %s: Shader extensions are not supported...\r\n"), fname.c_str());
		return NOT_SUPPORTED;
	}
	Cleanup();
	if (!vs.Compile() || !fs.Compile())
		return COMPILE_ERROR;
	handle = glCreateProgramObjectARB();
	vs.Attach(handle);
	fs.Attach(handle);
	glLinkProgramARB(handle);
	// Error check
	GLint status;
	glGetObjectParameterivARB(handle, GL_OBJECT_LINK_STATUS_ARB, &status);
	if (!status)
	{
		GLint len, act_len;
		glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &len);
		DEL_ARRAY(log);
		log = new GLcharARB[len + 1];
		glGetInfoLogARB(handle, len, &act_len, log);
		log[act_len] = 0;
		Log::CLog::Write(_T("CGLProgram::CreateAndLink %s:\r\n\t%s\r\n"),this->fname.c_str(), log);
		Cleanup();
		return LINK_ERROR;
	}		

	Log::CLog::Write(_T("CGLProgram::CreateAndLink: link successfull...\r\n"));
	return LOAD_OK;
}
LOAD CGLProgram::Load(const std::string& program_fname, const std::string& program_dir, const std::string& shader_dir)
{
	LOAD res = LOAD_OK;
	static TCHAR cwd[MAX_PATH + 1] = _T("");
	IO::CFileReader * fr = NULL;
	IO::CFileSystem::GetCWD(cwd, MAX_PATH);
	try
	{
        //CFileSystem::ChDir(program_dir.c_str());
		fname = std::string(cwd) + IO::CPath::GetCurrentSeparator() + program_dir + program_fname;
        fr = IO::CFileReader::Open(fname.c_str(), FILE_READ | FILE_TEXT);
		static TCHAR vs_fname[MAX_PATH+2];
		static TCHAR fs_fname[MAX_PATH + 2];
		BOOL read_ok = FALSE;
		do
		{
			read_ok = fr->GetLine(vs_fname, MAX_PATH+2);
		}while (read_ok && EmptyOrComment(vs_fname));
		if (!read_ok)
		{
            Log::CLog::Write(_T("CGLProgram::Load %s: can't find vertex shader %s...\r\n"), program_fname.c_str(), vs_fname);
			res = LOAD_ERROR;
			goto _cleanup;
		}
		Log::CLog::Write(_T("CGLProgram::Load %s loaded...\r\n"), vs_fname);
		do
		{
			read_ok = fr->GetLine(fs_fname, MAX_PATH+2);
		}while (read_ok && EmptyOrComment(fs_fname));
		if (!read_ok)
		{
            Log::CLog::Write(_T("CGLProgram::Load %s: can't find fragment shader %s...\r\n"), program_fname.c_str(), fs_fname);
			res = LOAD_ERROR;
			goto _cleanup;
		}
		Log::CLog::Write(_T("CGLProgram::Load %s loaded...\r\n"), fs_fname);
		//CFileSystem::ChDir(cwd);
		//CFileSystem::ChDir(shader_dir);
		// Compile only, if shader programs are avilable, and shaders are supported
        std::string fullPath_vs(GetFileName(vs_fname)), fullPath_fs(GetFileName(fs_fname));
        fullPath_vs = std::string(cwd) + shader_dir + fullPath_vs;
        fullPath_fs = std::string(cwd) + shader_dir + fullPath_fs;
		if ((Load(fullPath_vs, fullPath_fs) == LOAD_OK) && CGLProgram::ShaderSupported)
			res = CreateAndLink();

	}catch (IO::CFileNotFoundException& ex)
	{
		Log::CLog::Write(_T("CGLProgram::Load: %s...\r\n"), ex.msg.c_str());
		res = LOAD_ERROR;
	}
_cleanup:
	DEL(fr);
	//CFileSystem::ChDir(cwd);
	return res;
}
LOAD CGLProgram::LoadPrograms(const std::string& programs_fname, const std::string& program_dir, const std::string& shader_dir, ProgramMap& programs)
{
	LOAD res = LOAD_OK;
	try
	{
		IO::CFileReader *fr = IO::CFileReader::Open(programs_fname.c_str(), FILE_READ | FILE_TEXT);
        Log::CLog::Write(_T("CGLProgram::LoadPrograms using program dir.: %s\r\n"), program_dir.c_str());
        Log::CLog::Write(_T("CGLProgram::LoadPrograms using shader dir.: %s\r\n"), shader_dir.c_str());
		static TCHAR fname[MAX_PATH + 1];
		while (!fr->Eof())
		{			
			fr->GetLine(fname, MAX_PATH);
			if (EmptyOrComment(fname))
				continue;
			CGLProgram *program = new CGLProgram();				
            program->Load(GetFileName(fname), program_dir, shader_dir);
            programs.insert(ProgramMap::value_type(GetFileName(fname), program));
			//if (program->Load(GetFileName(fname), program_dir, shader_dir) == LOAD_OK)
			//{
			//	/* for debugging
			//	std::pair<const std::string*, CGLProgram*> p;
			//	p.first = program->GetFName();
			//	p.second = program;
			//	*/
			//	programs.insert(ProgramMap::value_type(GetFileName(fname), program));
			//}
			//else
			//	delete program;
		}		
		delete fr;
	}catch (IO::CFileNotFoundException& ex)
	{
		Log::CLog::Write(_T("%s...\r\n"),ex.msg);
		res = LOAD_ERROR;
	}
	/*m_ColorShaderProgram.Load("Shaders\\color.program");
		m_Shaders.insert(pair <const char *, CGLProgram *>("color.program", &m_ColorShaderProgram));
		m_SimpleTextureShaderProgram.Load("Shaders\\simple_texture.program");
		m_Shaders.insert(pair <const char *, CGLProgram *>("simple_texture.program", &m_SimpleTextureShaderProgram));
		m_TextureShaderNormalProgram.Load("Shaders\\simple_texture_normal.program");		
		m_Shaders.insert(pair <const char *, CGLProgram *>("simple_texture_normal.program", &m_TextureShaderNormalProgram));*/
	Log::CLog::Flush();
	return res;
}
}
