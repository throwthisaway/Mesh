#pragma once
#include "Viewport.h"
#include "GLExtensions.h"
#include "Mesh.h"
#include "GLProgram.h"

namespace Scene 
{
class Renderer : private CGLExtensions
{
	ProgramMap& shaders;
    int _error;
    static const GLsizei TXT_COUNT = 2,
        FBO_COUNT = 1,
        RBO_COUNT = 1;
    GLuint _txt[TXT_COUNT],
        _fbo[FBO_COUNT],
        _rbo[RBO_COUNT];
    GLint _txtWidth, _txtHeight;
    CCamera& _vp;
    Meshes& _meshes;
    GLhandleARB _colorTxt, _depthTxt;
    BOOL CheckFramebufferStatus(void);
    void PrintFramebufferInfo(void);
	void Resize(void);
	void Cleanup(void);
	void Init(void);
public:
    CGLProgram* _program;
	Renderer(ProgramMap& shaders, CCamera& vp, Meshes& meshes);
    ~Renderer(void);
    void Draw(HDC hDC);
};
}
