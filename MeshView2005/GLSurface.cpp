#include "StdAfx.h"
#include "GLSurface.h"
#include "GLScene.h"
#include "GLColorMap.h"
#include "GLColorMapN.h"
#include "GLColorMapAlpha.h"
#include "GLBumpMap.h"
#include "FileReader.h"
#include "AppConstants.h"
#include "GLConfiguration.h"
#include <utility>
#include <algorithm>

using namespace MeshLoader;
namespace Scene
{
	const GLfloat CGLSurface::_shinFactor = 128.f;
	CGLSurface::CGLSurface(const Surface& pSrf)
	{
		//_enableCaps.insert(GL_COLOR_MATERIAL);
		//_disableCaps.erase(GL_COLOR_MATERIAL);
		// Face culling
		if (pSrf.sidedness == SINGLE_SIDED)
		{
			_cullFace = GL_BACK;
			_frontFace = GL_FRONT;
			_enableCaps.insert(GL_CULL_FACE);
			_disableCaps.erase(GL_CULL_FACE);
		}
		else
		{
            _cullFace = GL_NONE;
			_frontFace = GL_FRONT_AND_BACK;
			_disableCaps.insert(GL_CULL_FACE);
			_enableCaps.erase(GL_CULL_FACE);
		}

		// Color
		_color[0] = pSrf.color[0];
		_color[1] = pSrf.color[1];
		_color[2] = pSrf.color[2];
		// Blending
		AdjustBlending(pSrf.surface_infos[TRANSPARENCY_MAP].val);

		// Specularity
		AdjustSpecularity(pSrf.surface_infos[SPECULARITY_MAP].val);
		AdjustShininess(pSrf.surface_infos[GLOSSINESS_MAP].val);

		CGLTextureManager::ImageMap& images = CGLScene::_txtMgr._images;
		// Layers
		
		CGLTextureMap::ActiveTextureProvider::Reset();
		// TODO:: reflection/refraction_map
		SurfInfo * pSurfInfo  = pSrf.surface_infos;
		for (int i = 0; i<NUM_MAP_TYPE; i++, pSurfInfo++)
		{
			for (SurfLayer * pLayer = pSurfInfo->layers; pLayer; pLayer = pLayer->next)
			{
				if (!pLayer->image)
					continue;
				CGLTextureManager::ImageMap::iterator it = images.find(pLayer->image);
				if (it != images.end())
				{
					_colorMaps.push_back((*it).second);
					continue;
				}
				GLenum nAct = CGLTextureMap::ActiveTextureProvider::Next();
				CGLColorMap * pGLColorMap = NULL;
				if (pSurfInfo->type == COLOR_MAP)
				{
					switch (pLayer->blend_mode)
					{
					case NORMAL_BLEND:
						if (nAct == CGLTextureMap::ActiveTextureProvider::First())
							pGLColorMap = new CGLColorMap(pLayer->uvmap);
						else
							pGLColorMap = new CGLColorMapN(pLayer->uvmap);
						break;
					case ALPHA_BLEND:
						pGLColorMap = new CGLColorMapAlpha(pLayer->uvmap);
						break;
					default:
						Log::CLog::Write("Unhandled blending mode: %d\r\n", pLayer->blend_mode);
						break;
					}
				}
				else if (pSurfInfo->type == BUMP_MAP)
				{
					pGLColorMap = new CGLBumpMap(pLayer->uvmap);
				}
				if (pGLColorMap)
				{
					Img::CImg *pImg = Img::CGLImageFactory::CreateImg(pLayer->image);
                    Img::CGLImageFactory::LoadImage(pLayer->image->path, pImg);
					pGLColorMap->FromImage(pImg);
					pGLColorMap->Init(nAct, pSurfInfo->val);
					images.insert(std::pair<Image*, CGLTextureMap*>(pLayer->image, (CGLTextureMap*)pGLColorMap));
					_colorMaps.push_back(pGLColorMap);
					if (!GLConfiguration::PreserveImageData)
						pImg->Cleanup();
				}
			}
		}
	}

	CGLSurface::~CGLSurface(void)
	{
		Cleanup();
	}
	void CGLSurface::AdjustBlending(GLfloat value)
	{
		_color[3] = value;
		if (OPAQUE_COMPONENT(value))
		{
			_disableCaps.insert(GL_BLEND);
			_enableCaps.erase(GL_BLEND);
		}
		else
		{
			_enableCaps.insert(GL_BLEND);
			_disableCaps.erase(GL_BLEND);

		}
	}
	void CGLSurface::AdjustCulling(BOOL bEnable, GLenum mode)
	{
		if (bEnable)
		{
			_enableCaps.insert(GL_CULL_FACE);
			_disableCaps.erase(GL_CULL_FACE);
			if (mode)
			{
				_frontFace = ((_cullFace = mode) == GL_FRONT) ? GL_BACK : GL_FRONT;
			}
		}
		else
		{
			_enableCaps.erase(GL_CULL_FACE);
			_disableCaps.insert(GL_CULL_FACE);
			_frontFace = GL_FRONT_AND_BACK;
		}
	}
	void CGLSurface::Blending(void)
	{
		//...glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	void CGLSurface::AdjustSpecularity(GLfloat value)
	{
		if (value > .0f)
		{
			_colorMat = GL_SPECULAR;
		}
		else
		{
			_colorMat = GL_AMBIENT_AND_DIFFUSE;
		}
		_spec[0] = _spec[1] = _spec[2] = value;
		_spec[3] = 1.f;
	}
	void CGLSurface::AdjustShininess(GLfloat value)
	{
		_shin = value * _shinFactor;
	}
	void CGLSurface::Setup(void)
    {
		for_each(_enableCaps.begin(), _enableCaps.end(), glEnable);
         _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
		for_each(_disableCaps.begin(), _disableCaps.end(), glDisable);
         _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
		//glColor4f(_color[0], _color[1], _color[2], _color[3]);
         if (_cullFace)
		    glCullFace(_cullFace);
         _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
		Blending();
         _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
		glColorMaterial(_frontFace, _colorMat);
         _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
		glMaterialfv(_frontFace, GL_DIFFUSE,_color);
         _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
		glMaterialfv(_frontFace, GL_SPECULAR,_spec);
         _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
		glMaterialf(_frontFace, GL_SHININESS, _shin);
         _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
	}
	void CGLSurface::Bind(void)
	{
		CGLTextureMap::ActiveTextureProvider::Reset();
		int i = 0;
		for (ColorMaps::iterator it = _colorMaps.begin(); it != _colorMaps.end();it++,i++)
		{
			//if (i >= 2) continue;
			(*it)->Bind();
			
		}
	}
	void CGLSurface::TearDown(void)
	{
		CGLTextureMap::ActiveTextureProvider::Reset();
		for (ColorMaps::iterator it = _colorMaps.begin();it != _colorMaps.end();it++)
		{
			(*it)->TearDown();
		}
	}
	void CGLSurface::Cleanup(void)
	{
	}
}
