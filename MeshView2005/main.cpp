// Friendly.c
// A Friendly OpenGL Program
// OpenGL SuperBible, 2nd Edition
// Richard S. Wright Jr.

#include "intel_compatibility.h"
#include "w32types.h"
#include <GL/glut.h>
#include "GLScene.h"
#include "Log.h"
#include "Bmp.h"

using namespace Log;
using namespace Scene;
CGLScene scene;
// Called to draw scene
CBmp bmp;
void RenderScene(void)
	{
	// Clear the window with current clearing color
	//glClear(GL_COLOR_BUFFER_BIT);
	//glDrawPixels(bmp.GetInfo()->width,bmp.GetInfo()->height, (bmp.GetInfo()->bpp == 32)?GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE, bmp.GetInfo()->data);

	
	// Flush drawing commands
	//glFlush();
	scene.Draw(0);
	}

// InitGL();
GLfloat LightAmbient[]=  { 0.5f, 0.5f, 0.5f, 0.3f };	// Ambient Light Values
GLfloat LightDiffuse[]=	 { 0.5f, 0.5f, 0.5f, 0.3f };	// Diffuse Light Values
GLfloat LightPosition[]= { 2.5f, 2.5f, 2.0f, 1.0f };	// Light Position
//const _FLOAT LightPosition[3] = {3.0f, 3.0f, -1.0f};
//GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//GLfloat mat_shininess[] = { 50.0 };
BOOL InitGL()
{
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glEnable(GL_COLOR_MATERIAL);
	glLightfv(GL_LIGHT1,GL_SPECULAR,LightAmbient);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,LightDiffuse);
	glLightfv(GL_LIGHT1,GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	return TRUE;
}
// Setup the rendering state
static int glut_wnd, width, height;
// ResizeGLScene(...);
GLvoid ResizeGLScene(GLint width,GLint height)
{
	scene.vp.SetupViewport(width, height);
	::width=width;
	::height=height;
}
void SetupRC(int x, int y, int width, int height, char * name)
{
    // Set clear color to blue
   // glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(x, y);
	glutInitWindowSize(::width = width, ::height = height);
	ResizeGLScene(width, height);
	glut_wnd = glutCreateWindow(name);
	
	glutSetWindow(glut_wnd);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//InitGL();
}
void HandleMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		scene.vp.SetMouseState(LEFT_MOUSE_BUTTON, state == GLUT_DOWN);
	if (button == GLUT_RIGHT_BUTTON)
		scene.vp.SetMouseState(RIGHT_MOUSE_BUTTON, state == GLUT_DOWN);
	if (button == GLUT_MIDDLE_BUTTON)
		scene.vp.SetMouseState(MIDDLE_MOUSE_BUTTON, state == GLUT_DOWN);	
	scene.vp.SetMouseStartPos(x, y);
}
void HandleMotion(int x, int y)
{		
	scene.vp.SetMousePos(x, y);
	glutPostRedisplay();
}
void ChangeSize(int width, int height)
{
	scene.vp.SetupViewport(width, height);
	::width=width;
	::height=height;
}

// Main program entry point
int main(int argc, char ** argv)
{
	//CLog::Add(new CFileLogger("./log.txt",FILE_APPEND));
	//CLog::Write("Log started...");
	//if (bmp.Load("./Data/Font/font.bmp") != ID_BMP_OK)
	/*if (bmp.Load("../test.bmp") != ID_BMP_OK)
	{
		printf("Can't load bitmap...\n"); 
		return 0;
	}	 */
	
	/*glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);
	glutMainLoop();*/
	glutInit(&argc, argv);	
	SetupRC(0,0, 640, 480, "Simple");
	glutDisplayFunc(RenderScene);
	glutMouseFunc(HandleMouse);
	glutMotionFunc(HandleMotion);
	glutReshapeFunc(ChangeSize);
	//glutIdleFunc(RenderScene);
	scene.Init(0);
	glutMainLoop();
	glutDestroyWindow(glut_wnd);
    /*CMesh mesh;
    try
    {
    	mesh.Load(new CMeshLoader("/home/itried/workspace/test6.mesh"));
    
    }
    catch(...)
    {
    }*/
	return 0;
}

