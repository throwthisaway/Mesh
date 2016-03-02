#pragma once
#include "intel_compatibility.h"
#include <vector>
#include <map>
#include <utility>
#include <string>

#include "Mesh.h"
#include "Log.h"

#include "GLProgram.h"
#include "GLLightManager.h"
#ifdef WIN32
#include "Viewport.h"
//#include "WinEditor.h"
#include "timer.h"
#else
#include "Viewport_glut.h"
#include "timer_linux.h"
#endif
#include "GLBitmapFont.h"
#include "Hud.h"
#include "Img\\GLImageFactory.h"
#include "LightSourceParameters.h"
#include "GLTextureManager.h"
#include "Renderer.h"
#include "IItem.h"

namespace Scene
{
   
class CGLScene : CGLExtensions
{	
	// Shaders 
	void CleanupPrograms(void);
	/*CGLProgram m_ColorShaderProgram;
	CGLProgram m_SimpleTextureShaderProgram;
	CGLProgram m_TextureShaderNormalProgram;*/
	// ...Shaders 
	typedef GLEnumSet States;
	static States _enableStates, _disableStates;
	int frames;
	static LONGLONG period;
	#ifdef WIN32
	CHighPerfTimer timer;
	//CWinEditor winEditor;
	#else
	CGLUTTimer timer;
	#endif
	const BOOL m_bHandleInputMsg;
	
	unsigned int * m_pnVBOVertices;
	CHud hud;
	GLfloat * BuildUVMap(UVMap * uvmap, GLuint &uv_count);
	//GLvoid TextOutXY(float x,float y,const char *text,...);
	void InitGL();
	void ShowLight(float x, float y, float z);
	std::vector<std::unique_ptr<IItem>> items;
public:
    // Shaders 
	// TODO: const char * comparer should come here...
    /*static*/ ProgramMap m_Shaders;
	
	Meshes _meshes;
	typedef std::map<std::string, std::shared_ptr<CMesh>> MeshRepoMap;
	MeshRepoMap _meshRepository;
	static CGLTextureManager _txtMgr;
	static GLuint _dummyTexture;
	CGLLightManager m_LightManager;
	BOOL _loaded;	// Scene loaded successfully...
	CCamera vp;
    std::auto_ptr<Renderer> _renderer;
	BOOL Init(HWND hWnd, HINSTANCE hInst);
	void Cleanup(void);
	void Draw(HDC hDC);
	void ProgramReload();
	CCamera& GetGetCurrentViewPort(void) { return vp; };
	int LoadMeshes(int count, const char ** fnames, const Vector<float> pos[]);
	void Add(Vector<float>& pos, CMesh * mesh);
	void Add(std::unique_ptr<IItem> sphere);
	#ifdef WIN32
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	#endif
	CGLScene();
	~CGLScene();
};

}
