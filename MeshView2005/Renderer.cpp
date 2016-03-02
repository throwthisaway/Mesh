#include "StdAfx.h"
#include "Renderer.h"
#ifdef WIN32
#include <windows.h>
#include "AppConstants.h"
#include "GLScene.h"
#endif

namespace Scene
{
unsigned int upperPowerOfTwo(unsigned int n)
{
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;
    return n;
}

Renderer::Renderer(ProgramMap& shaders, CCamera& vp, Meshes& meshes) : shaders(shaders), _vp(vp), _meshes(meshes), _program(), _error(0)
{
	Init();
}
void Renderer::Init()
{
	_txtWidth = upperPowerOfTwo(_vp.GetWidth());
	_txtHeight = upperPowerOfTwo(_vp.GetHeight());
	// _txtWidth = _txtHeight = 512;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(TXT_COUNT, _txt);
	glBindTexture(GL_TEXTURE_2D, _txt[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _txtWidth, _txtHeight, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Depth texture...
	glBindTexture(GL_TEXTURE_2D, _txt[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, _txtWidth, _txtHeight, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//glGenRenderbuffers(RBO_COUNT, _rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, _rbo[0]);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _txtWidth, _txtWidth);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//...Depth texture

	glGenFramebuffers(FBO_COUNT, _fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo[0]);
	// Depth texture...         
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo[0]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _txt[1], 0);
	//...Depth texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _txt[0], 0);

	//         glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, rbo[1]);
	_ASSERT(CheckFramebufferStatus());
	//GLenum i[] ={GL_COLOR_ATTACHMENT0/*, GL_COLOR_ATTACHMENT1*/};
	//glDrawBuffers(sizeof(i)/sizeof(i[0]), i);
	//_ASSERT(CheckFramebufferStatus());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_ASSERT(CheckFramebufferStatus());
	glDisable(GL_TEXTURE_2D);
	ProgramMap::const_iterator program = shaders.find(std::string("fbo.program"));
	_program = program->second;
	_program->UseProgram();
	_ASSERT_GL_ERROR(glGetError());
	_colorTxt = glGetUniformLocationARB(_program->handle, "colorTxt");
	_depthTxt = glGetUniformLocationARB(_program->handle, "depthTxt");
	_ASSERT_GL_ERROR(glGetError());
	glUseProgramObjectARB(0);
	//CGLProgram::FixedFunc();
}
void Renderer::Resize(void)
{
	Cleanup();
	Init();
}
void Renderer::Cleanup(void)
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(RBO_COUNT, _rbo);

	//
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(FBO_COUNT, _fbo);
	//
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(TXT_COUNT, _txt);
}
Renderer::~Renderer(void)
{
    Cleanup();
}
void Renderer::Draw(HDC hDC)
{
   
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo[0]);
     glEnable(GL_TEXTURE_2D);
     _LOG_GL_ERROR(glGetError())
   // glColor3f(0.3f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	//
       GLenum i[] ={GL_COLOR_ATTACHMENT0/*, GL_COLOR_ATTACHMENT1*/};
    glDrawBuffers(sizeof(i)/sizeof(i[0]), i);
    Meshes::iterator it = _meshes.begin();
	while (it != _meshes.end())
	{
		glTranslatef(it->second._pos.x, it->second._pos.y, it->second._pos.z);
		it->first->Draw();
		it++;
	}

    _LOG_GL_ERROR(glGetError());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
     // !!! MipMap hack only needed when MipMaps are enabled (i.e: actual texture usage on object), to
     // manually generate the mipmap levels!!!
       //         glBindTexture(GL_TEXTURE_2D, _txt[0]);
       //          _ASSERT(!glGetError());
       // glGenerateMipmap(GL_TEXTURE_2D);
       //  _ASSERT(!glGetError());
       // glBindTexture(GL_TEXTURE_2D, 0);
       //  _ASSERT(!glGetError());
       // // Depth buffer mipmap hack
       //glBindTexture(GL_TEXTURE_2D, _txt[1]);
       // _ASSERT(!glGetError());
       // glGenerateMipmap(GL_TEXTURE_2D);
       //  _ASSERT(!glGetError());
       // glBindTexture(GL_TEXTURE_2D, 0);
       //  _ASSERT(!glGetError());
//   // SwapBuffers(hDC);
//    // !!! FB not generated !!!!
//     // glScissor(0, 0, _vp.GetWidth(), _vp.GetHeight() );
//    //  glViewport(0, 0, _vp.GetWidth(), _vp.GetHeight() );
_program->UseProgram();
        // glUseProgramObjectARB(0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-_vp.GetWidth()/2.f,_vp.GetWidth()/2.f, -_vp.GetHeight()/2.f, _vp.GetHeight()/2.f, .1f, 100.f);
    glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    glLoadIdentity();
//
//    glClearColor(.5f, 0.f, 0.f, 0.f);
//    glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT*/);
    glTranslatef(0, 0, -20.f);    
//    glColor4f(1.f,1.f,1.f,1.f);
//    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT,new float[]{1f,1f,1f,1f}, 0);
//
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_DEPTH_TEST);

             glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);  

         //      GLuint id = CGLScene::_txtMgr._images.begin()->second->GetID();
                 glActiveTextureARB(GL_TEXTURE0);
                   glEnable(GL_TEXTURE_2D);
               
               glBindTexture(GL_TEXTURE_2D, _txt[0]);
       
//        //        glClientActiveTexture(GL_TEXTURE0);
////        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
////        txtCoords.rewind();
////        glTexCoordPointer(2, GL_FLOAT, 0, txtCoords);
//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, _txt[0]);
        glUniform1iARB(_colorTxt, 0);
//        
//    
        glActiveTextureARB(GL_TEXTURE1);
           glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, _txt[1]);
//          //  glClientActiveTexture(GL_TEXTURE1);
//
////        txtCoords.rewind();
////        glTexCoordPointer(2, GL_FLOAT, 0, txtCoords);
     
       
        glUniform1iARB(_depthTxt, 1);
        glColor4f(1.f,1.f,1.f, 1.f);
        glBegin(GL_TRIANGLE_STRIP);
        
            //glMultiTexCoord2fARB(GL_TEXTURE0, 0.f, (float)_vp.GetHeight()/(float)_txtHeight);
            //glMultiTexCoord2fARB(GL_TEXTURE1,0.f, (float)_vp.GetHeight()/(float)_txtHeight);
            glTexCoord2f(0.f, (float)_vp.GetHeight()/(float)_txtHeight);glVertex3f(-_vp.GetWidth()/2.f, _vp.GetHeight()/2.f, 0.f);
            //glMultiTexCoord2fARB(GL_TEXTURE0, 0.f,  0.f);
            //glMultiTexCoord2fARB(GL_TEXTURE1, 0.f,  0.f);
            glTexCoord2f(0.f, 0.f);glVertex3f(-_vp.GetWidth()/2.f, -_vp.GetHeight()/2.f, 0.f);
            //glMultiTexCoord2fARB(GL_TEXTURE0, (float)_vp.GetWidth()/(float)_txtWidth,   (float)_vp.GetHeight()/(float)_txtHeight);
            //glMultiTexCoord2fARB(GL_TEXTURE1, (float)_vp.GetWidth()/(float)_txtWidth,  (float) _vp.GetHeight()/(float)_txtHeight);
            glTexCoord2f((float)_vp.GetWidth()/(float)_txtWidth,(float)_vp.GetHeight()/(float)_txtHeight);glVertex3f(_vp.GetWidth()/2.f,  _vp.GetHeight()/2.f, 0.f);
        
            //glMultiTexCoord2fARB(GL_TEXTURE0, (float)_vp.GetWidth()/(float)_txtWidth,  0.f);
            //glMultiTexCoord2fARB(GL_TEXTURE1, (float)_vp.GetWidth()/(float)_txtWidth,  0.f);
            glTexCoord2f((float)_vp.GetWidth()/(float)_txtWidth, 0.f);glVertex3f(_vp.GetWidth()/2.f, - _vp.GetHeight()/2.f, 0.f);
        glEnd();

       
////        glClientActiveTexture(GL_TEXTURE0);
        glActiveTextureARB(GL_TEXTURE0);
       glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
//        
////        glClientActiveTexture(GL_TEXTURE0);
        glActiveTextureARB(GL_TEXTURE1);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
//        

        glActiveTextureARB(GL_TEXTURE0);

        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
 
        glUseProgramObjectARB(0);
         //CGLProgram::FixedFunc();
//         glPopMatrix();
//        
}
BOOL Renderer::CheckFramebufferStatus(void)
{
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        Log::CLog::Write("Framebuffer complete.\r\n");
        return TRUE;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        Log::CLog::Write("[ERROR] Framebuffer incomplete: Attachment is NOT complete.\r\n");
        return FALSE;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        Log::CLog::Write("[ERROR] Framebuffer incomplete: No image is attached to FBO.\r\n");
        return FALSE;
/*
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
        Log::CLog::Write("[ERROR] Framebuffer incomplete: Attached images have different dimensions.\r\n");
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
        Log::CLog::Write("[ERROR] Framebuffer incomplete: Color attached images have different internal formats.\r\n");
        return false;
*/
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        Log::CLog::Write("[ERROR] Framebuffer incomplete: Draw buffer.\r\n");
        return FALSE;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        Log::CLog::Write("[ERROR] Framebuffer incomplete: Read buffer.\r\n");
        return FALSE;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        Log::CLog::Write("[ERROR] Framebuffer incomplete: Unsupported by FBO implementation.\r\n");
        return FALSE;

    default:
        Log::CLog::Write("[ERROR] Framebuffer incomplete: Unknown error.\r\n");
        return FALSE;
    }
}



///////////////////////////////////////////////////////////////////////////////
// print out the FBO infos
///////////////////////////////////////////////////////////////////////////////
void Renderer::PrintFramebufferInfo(void)
{
    Log::CLog::Write("\n===== FBO STATUS =====\n");

    // print max # of colorbuffers supported by FBO
    int colorBufferCount = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
    Log::CLog::Write("Max Number of Color Buffer Attachment Points: %d",colorBufferCount);

    int objectType;
    int objectId;

    // print info of the colorbuffer attachable image
    for(int i = 0; i < colorBufferCount; ++i)
    {
       /* glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_COLOR_ATTACHMENT0+i,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                              &objectType);
        if(objectType != GL_NONE)
        {
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                                  GL_COLOR_ATTACHMENT0+i,
                                                  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                                  &objectId);

            std::string formatName;

            Log::CLog::Write("Color Attachment %d:",i);
            if(objectType == GL_TEXTURE)
            {
                Log::CLog::Write("GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
            }
            else if(objectType == GL_RENDERBUFFER)
            {
               Log::CLog::Write("GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
            }
        }*/
    }

    //// print info of the depthbuffer attachable image
    //glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
    //                                      GL_DEPTH_ATTACHMENT,
    //                                      GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
    //                                      &objectType);
    //if(objectType != GL_NONE)
    //{
    //    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
    //                                          GL_DEPTH_ATTACHMENT,
    //                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
    //                                          &objectId);

    //   Log::CLog::Write("Depth Attachment: ");
    //    switch(objectType)
    //    {
    //    case GL_TEXTURE:
    //        Log::CLog::Write("GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
    //        break;
    //    case GL_RENDERBUFFER:
    //        Log::CLog::Write("GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
    //        break;
    //    }
    //}

    //// print info of the stencilbuffer attachable image
    //glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
    //                                      GL_STENCIL_ATTACHMENT,
    //                                      GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
    //                                      &objectType);
    //if(objectType != GL_NONE)
    //{
    //    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
    //                                          GL_STENCIL_ATTACHMENT,
    //                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
    //                                          &objectId);

    //    Log::CLog::Write("Stencil Attachment: ");
    //    switch(objectType)
    //    {
    //    case GL_TEXTURE:
    //        Log::CLog::Write("GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
    //        break;
    //    case GL_RENDERBUFFER:
    //       Log::CLog::Write("GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
    //        break;
    //    }
    //}

    Log::CLog::Write("\r\n");
}

}