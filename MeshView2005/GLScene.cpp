//#ifdef WIN32
#include "StdAfx.h"
//#endif
#include "GLScene.h"
#include "w32types.h"
#include "EditLogger.h"
#include "FileLogger.h"
#include "AppConstants.h"
#include "Tga.h"
#include <vector>
#include <algorithm>
#include "VectorMath.h"
#include <memory>
#include "../../MeshLoader/MeshLoader.h"
namespace Scene
{
#define LINUX_IMAGE_PATH "./Data/Images/"
#define FPS_INTERVAL 1000.0
#define BITMAP_FONT_SIMPLE_TEXTURE_PROGRAM "simple_texture.program"
CGLScene::States CGLScene::_enableStates;
CGLScene::States CGLScene::_disableStates;
CGLTextureManager CGLScene::_txtMgr;
/*ProgramMap CGLScene::m_Shaders;*/

LONGLONG CGLScene::period;
GLuint CGLScene::_dummyTexture = 0;
static const unsigned char dummyTexData[4] = {255, 255, 255, 255};
CGLScene::CGLScene() :  
	m_bHandleInputMsg(FALSE),
	_loaded(FALSE),
	m_pnVBOVertices(NULL)
{	
}

void CGLScene::Cleanup(void)
{
	Log::CLog::Write("Cleaning up...\r\n");
	Log::CLog::Write("\tFreeing up images...\r\n");
	_txtMgr.Cleanup();
	Log::CLog::Write("\tDeleting meshes...\r\n");
	if (VBOSupported && m_pnVBOVertices)
	{
		Log::CLog::Write("\tFinishing up VBOs...\r\n");
		glDeleteBuffersARB((GLsizei)_meshes.size(), m_pnVBOVertices);
		DEL_ARRAY(m_pnVBOVertices);
		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );	
		glDisableClientState( GL_NORMAL_ARRAY );
	}
	_loaded = FALSE;
	Log::CLog::Write("Cleanup finished...\r\n");
}

CGLScene::~CGLScene()
{	
	Cleanup();
	CleanupPrograms();
	Log::CLog::Cleanup();
	// Static deinitilazitaion
	if (_dummyTexture)
		glDeleteTextures(1, &_dummyTexture);
}

void CGLScene::CleanupPrograms(void)
{
	Log::CLog::Write("CGLScene::CleanupPrograms: Clearing programs...\r\n");
	for (ProgramMap::iterator it = m_Shaders.begin(); it != m_Shaders.end(); it++)
		delete (*it).second;
	m_Shaders.clear();
}

GLfloat * CGLScene::BuildUVMap(UVMap * uvmap, GLuint &uv_count)
{

	//TODO: Optimize, asm, MMX, SSEx, etc.
	//for (long i = 0;i<count;i++)
	//{
	//	uvmap->@@@
	//}
	return NULL;
}

//const float LightPosition[3] = {3.0f, 3.0f, -1.0f};
//GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//GLfloat mat_shininess[] = { 50.0 };
void CGLScene::InitGL()
{
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	m_LightManager.Add(LightSource());
	m_LightManager.SetupSceneLighting(SceneLighting());
	//
	glDisable(GL_NORMALIZE);
	glDisable(GL_AUTO_NORMAL);

	// Create dummy texture
	glGenTextures(1, &_dummyTexture);
	glBindTexture(GL_TEXTURE_2D, _dummyTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, dummyTexData);
	GLenum error = glGetError();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    // TODO:: dafuq did I think...
    // nope...
	//_enableStates.insert(GL_TEXTURE_COORD_ARRAY);
	//_disableStates.erase(GL_TEXTURE_COORD_ARRAY);
	_enableStates.insert(GL_NORMAL_ARRAY);
	_disableStates.erase(GL_NORMAL_ARRAY);
	_enableStates.insert(GL_VERTEX_ARRAY);
	_disableStates.erase(GL_VERTEX_ARRAY);
	for_each(_enableStates.begin(), _enableStates.end(), glEnableClientState);
	for_each(_disableStates.begin(), _disableStates.end(), glDisableClientState);

   /* Img::CImg *pImg = Img::CGLImageFactory::CreateImg(CAppConstants::default_image);
	Img::CGLImageFactory::LoadImage(CAppConstants::default_image, pImg);
    CGLColorMap * pGLColorMap = new CGLColorMap(uvmap);
    _txtMgr::_images.insert(std::pair<Image*, CGLTextureMap*>(pLayer->image, (CGLTextureMap*)pGLColorMap));
	if (!GLConfiguration::PreserveImageData)
		pImg->Cleanup();*/
}

void CGLScene::ProgramReload()
{
	Log::CLog::Write("CGLScene::ProgramReload: Rebuilding shader programs...\r\n");
	CleanupPrograms();
	CGLProgram::LoadPrograms(::CAppConstants::programs, ::CAppConstants::program_dir, ::CAppConstants::shader_dir, m_Shaders);
}

void CGLScene::Add(Vector<float>& pos, CMesh * mesh)
{
	ISD isd = {pos};
	_meshes.push_back(std::make_pair(std::shared_ptr<CMesh>(mesh), isd));
}

int CGLScene::LoadMeshes(int count, const char ** fnames/*"..\\..\\obj\\multilayer.mesh"*/, const Vector<float> pos[])
{
	//Cleanup();

	if (!VBOSupported)
		return -1;

	m_pnVBOVertices = new unsigned int[count]; // TODO:: it should be meshRepo size...
	glGenBuffersARB(count, m_pnVBOVertices);
	GLenum error = glGetError();
	for (long i = 0; i < count; i++)
	{	
		MeshRepoMap::iterator it = _meshRepository.find(std::string(fnames[i]));
		if (it != _meshRepository.end())
		{
			ISD isd = {pos[i]};
			_meshes.push_back(std::make_pair(it->second, isd));
			continue;
		}
		Log::CLog::Write("Loading %s...\r\n", fnames[i]);
		std::shared_ptr<CMesh> mesh(new CMesh(m_Shaders));
		std::unique_ptr<IO::CFileReader> fileLoader(IO::CFileReader::Open(fnames[i]));
		long size = fileLoader->Size();
		std::vector<char> data(size);
		fileLoader->Read(&data.front(), size);
		LoadMesh(&data.front(), size, *mesh.get());
		mesh->m_szName = fnames[i];
		//meshes[i].Load( new CMeshLoader("D:\\NewTek\\LightWave 3D 9\\multilayer.mesh"));
		//meshes[i].Load( new CMeshLoader("D:\\NewTek\\LightWave 3D 9\\bumptest.mesh"));
		//meshes[i].Load( new CMeshLoader("D:\\NewTek\\LightWave 3D 9\\box-wcheckerboard-2x2.mesh"));
		//meshes[i].Load( new CMeshLoader("D:\\NewTek\\LightWave 3D 9\\vmap_test.mesh"));
		mesh->Setup( m_pnVBOVertices[i]);
		//  Bind, bufferdata
		// texture VBO...
		mesh->SetupGLSurfaces(m_Shaders);
		ISD isd = {pos[i]};
		std::pair<MeshRepoMap::iterator, bool> inserted = _meshRepository.insert(std::make_pair(std::string(fnames[i]), mesh));
		_meshes.push_back(std::make_pair(mesh, isd));
		
		//meshes[i].BuildList();
	}
	_loaded = TRUE;
	//Vector<float> v1 = _meshes[1]->_pos - _meshes[2]->_pos, v0(.0f, .0f, .0f);
	//CVectorMath<float>::Normalize(v1);
	//Vector<float> v3 = CVectorMath<float>::Cross(v0, v1);
	return TRUE;
}

BOOL CGLScene::Init(HWND hWnd, HINSTANCE hInst)
{	
	//GLenum error;
	Cleanup();
	Log::CLog::Add(new Log::CFileLogger("log.txt", FILE_REWRITE));
	//Log::CLog::Add(new Log::CEditLogger(hWnd));
	#ifdef WIN32
	vp.SetupRawInputDevice(m_bHandleInputMsg, hWnd);	
	#endif
	InitGL();
	//CGLExtensions::Init();
	ProgramReload();
	// TODO: error handling
	if (CGLScene::ShaderSupported)
	{
		ProgramMap::const_iterator program = m_Shaders.find(std::string(BITMAP_FONT_SIMPLE_TEXTURE_PROGRAM));
		if (program != m_Shaders.end())
			hud.Init(vp.GetWidth(), vp.GetHeight(), (*program->second).handle);
		else
			Log::CLog::Write("CGLScene::Init: Can't find shader program for bitmap font...\r\n");
	}
	else
	{
		hud.Init(vp.GetWidth(), vp.GetHeight(),/*TODO::*/ 0);
		Log::CLog::Write("CGLScene::Init: Shader extensions are not supported...\r\n"/*"Exiting...\r\n"*/);
		//return FALSE;
	}
	if (!VBOSupported)
	{
		Log::CLog::Write("CGLScene::Init: Vertex Buffer Objects not supported...\r\n""Exiting...\r\n");
		return FALSE;
	}
	frames = 0;
	period = 0;
//#ifdef WIN32
//	winEditor.CreateAndShow(800, 0 , 400, 600, hWnd, hInst);
//#endif
    _renderer = std::auto_ptr<Renderer>(new Renderer(this->m_Shaders, vp, _meshes));
	return TRUE;
}
void CGLScene::ShowLight(float x, float y, float z)
{
	glDisable(GL_LIGHTING);
	GLboolean bTextureEnabled;
	glGetBooleanv(GL_TEXTURE_2D, &bTextureEnabled);
	glDisable(GL_TEXTURE_2D);
	//glEnable(GL_VERTEX_PROGRAM_TWO_SIDE);
	glDisable(GL_CULL_FACE);
	glColor3f(1.0f, 1.0f, 1.0f);

	// TODO::
	if (ShaderSupported)
	{
		//glUseProgramObjectARB(handle);
	}
	Vector<float> pos(x, y, z);
	//CGLLightManager::GetPos(0, pos);
	const GLfloat d = 1.f / 5.f;
	//glPointSize(10.f);
	glBegin(GL_TRIANGLES);
		glVertex3f(pos.x, pos.y, pos.z);
		glVertex3f(pos.x + d, pos.y, pos.z + d);
		glVertex3f(pos.x - d, pos.y + d, pos.z);
	glEnd();
	if (bTextureEnabled)
		glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void CGLScene::Draw(HDC hDC)
{
	static char fps[25];
	//static int frames;
	timer.start();
    vp.SetProjection();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();						// Reset The View
	//glColor3f(1.0f, 1.0f, 1.0f);
	
	//glDisable(GL_TEXTURE_2D);
	//glBegin(GL_TRIANGLES);
	//glVertex3f(0.0f,0.0f,0.0f);
	//glVertex3f(1.0f,0.0f,0.0f);
	//glVertex3f(0.5f,1.0f,0.0f);
	//glEnd();
	//glFlush();
	//#ifdef WIN32
	//SwapBuffers(hDC);
	//#endif;
	//return;
	std::for_each(m_LightManager._lightSources.begin(), m_LightManager._lightSources.end(), std::mem_fun_ref(&LightSource::Position));
	std::for_each(m_LightManager._lightSources.begin(), m_LightManager._lightSources.end(), std::mem_fun_ref(&LightSource::Enable));

	ShowLight(3.f, 3.f, 3.f);
	vp.AdjustTranslation();
	vp.AdjustRotation();

	long i = 0;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	for (auto& it : items)
	{
		it->Draw();
	}
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
    _renderer->Draw(hDC);

	GLfloat mv[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mv);
	hud.PrintXY(0, 100, "%3.06g %3.06g %3.06g %3.06g", mv[12], mv[13], mv[14], mv[15]);
	hud.PrintXY(0, 115, "%3.06g %3.06g %3.06g %3.06g", mv[8], mv[9], mv[10], mv[11]);
	hud.PrintXY(0, 130, "%3.06g %3.06g %3.06g %3.06g", mv[4], mv[5], mv[6], mv[7]);
	hud.PrintXY(0, 145, "%3.06g %3.06g %3.06g %3.06g", mv[0], mv[1], mv[2], mv[3]);

	hud.Set(0, fps);
	hud.Set(1, vp.GetPos().x, vp.GetPos().y, vp.GetPos().z);
	hud.Set(2, vp.GetRot().x, vp.GetRot().y, vp.GetRot().z);
	hud.Set(3, vp.GetMouseState(0), vp.GetMouseState(1), vp.GetMouseState(2));
	//hud.Set(4, "F1 : Fullscreen");
	//hud.Set(5, "F7 : Compile");
/*	TextOutXY(10.0,(float)vp.GetHeight()-50.0,"fps: %g", fps);
	TextOutXY(10.0,10.0,"Pos: %g %g %g", vp.GetPos().a, vp.GetPos().b, vp.GetPos().c);
	TextOutXY(10.0,25.0,"Rot: %g %g %g", vp.GetRot().a, vp.GetRot().b, vp.GetRot().c);
	TextOutXY(10.0,40.0,"State: %d %d %d", vp.GetMouseState(0), vp.GetMouseState(1), vp.GetMouseState(2));*/
	hud.Draw();
	frames++;
	#ifdef WIN32
	SwapBuffers(hDC);
	#else
	//glFlush();
	glutSwapBuffers();
	#endif 
	timer.stop();
	if ((period += timer.get_period()) >= timer._frequency())
	{
		sprintf(fps,"%.03f", (double)frames * (double)timer._frequency() / (double)period);
		frames = 0;
		period = 0;
	}

	/*#ifdef WIN32
	fps = 1000000.0 / timer.get_microseconds();
	#else
	fps = 1000.0 / timer.get_miliseconds();
	#endif*/
}
void CGLScene::Add(std::unique_ptr<IItem> sphere)
{
	items.push_back(std::move(sphere));
}
#ifdef WIN32
LRESULT CGLScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

#ifndef _VS6
	if (m_bHandleInputMsg)
	{
		switch (message)
		{
		case WM_INPUT:
			vp.HandleRawData(lParam);
			return 0;
		}
	}
#endif
	switch (message)
	{
		case WM_MOUSEMOVE:
			vp.SetMousePos((int)(lParam & 0xffff), (int)(lParam >> 16));
			return 0;
		case WM_LBUTTONDOWN:
			::SetCapture(hWnd);
			vp.SetMouseState(LEFT_MOUSE_BUTTON, TRUE);
			return 0;
		case WM_RBUTTONDOWN:
			::SetCapture(hWnd);
			vp.SetMouseState(RIGHT_MOUSE_BUTTON, TRUE);
			return 0;
		case WM_LBUTTONUP:
			::ReleaseCapture();
			vp.SetMouseState(LEFT_MOUSE_BUTTON, FALSE);
			return 0;
		case WM_RBUTTONUP:
			::ReleaseCapture();
			vp.SetMouseState(RIGHT_MOUSE_BUTTON, FALSE);
			return 0;
	}
	return 1;
}
#endif
/*GLvoid CGLScene::TextOutXY(float x,float y,const char *text,...)
{
	static char line[LINELEN];
	va_list arglist;
	va_start(arglist,text);
	int len=_vsnprintf(line,LINELEN,text,arglist);
	if (len<=0)
		return;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,vp.GetWidth(),0,vp.GetHeight(),-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glListBase(font.GetList()-32);
	glTranslatef(x,y,0);
	glCallLists(len,GL_UNSIGNED_BYTE,line);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	va_end(arglist);		
}*/
}