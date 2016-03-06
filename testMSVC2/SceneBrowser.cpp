// SceneBrowser.cpp : implementation file
//

#include "stdafx.h"
#include "testMSVC2.h"
#include "SceneBrowser.h"

#include "PropertiesWnd.h"
#include "MainFrm.h"
#include "GLProgram.h"
#include "testMSVC2Doc.h"
#include "AppConstants.h"
#include "MFCOnUpdateValueCallbackProperty.h"
#include "MFCButtonPropertyGridProperty.h"
#include "propertyGrid.h"
#include "Path.h"
#include "Logger.h"
#include "File.h"

// CSceneBrowser
using namespace Scene;

IMPLEMENT_DYNAMIC(CSceneBrowser, CDockablePane)

CSceneBrowser::CSceneBrowser()
{

}

CSceneBrowser::~CSceneBrowser()
{
}

BEGIN_MESSAGE_MAP(CSceneBrowser, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_DESTROY()
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_SURFACE_OVERRIDEPROGRAM, &CSceneBrowser::OnSurfaceOverrideprogram)
END_MESSAGE_MAP()



// CSceneBrowser message handlers

class CItemCommand
{
public:
	virtual void OnSelectionChanged(CPropertiesPane& propertiesWnd) = 0;
	virtual void DoubleClick() = 0;
	static CMFCPropertyGridProperty * CreateProperty(int resID, const COleVariant& value, DWORD_PTR data, LPCTSTR desc = _T(""))
	{
		static CString tempStr;
		static BOOL bNameValid;
		bNameValid = tempStr.LoadString(resID);
		ASSERT(bNameValid);
		CMFCPropertyGridProperty * pProperty = new CMFCPropertyGridProperty(tempStr, value, desc);
		pProperty->SetData(data);
		return pProperty;
	}
	static CMFCPropertyGridProperty * CreatePropertyList(int resID, const COleVariant& value, DWORD_PTR data, LPCTSTR* items, const int itemCount, LPCTSTR desc = _T(""))
	{
		CMFCPropertyGridProperty * pProperty = 
			 CreateProperty(resID, value, data, desc);
		for (int i = 0;i<itemCount; i++)
		{
			pProperty->AddOption(items[i]);
		}
		return pProperty;
	}
};

class CMeshCommand : public CItemCommand
{
public:
	CMesh * _pMesh;
	CMeshCommand(CMesh * pMesh) : _pMesh(pMesh) {};
	~CMeshCommand(void)
	{
	}
	virtual void OnSelectionChanged(CPropertiesPane& propertiesWnd)
	{
		// TODO:: Show item's property
	}
	virtual void DoubleClick()
	{
	}
};
class CSurfaceCommand  : public CItemCommand
{
public:
	Surface * _pSurface;
	CGLSurface * _pGLSurface;
private:
	static void OnUpdateSmoothing(DWORD_PTR dwData, const COleVariant& value)
	{
		float* fltSmooth = (float*)dwData;
		*fltSmooth = value.boolVal ? CAppConstants::Smooth : CAppConstants::Flat;
	}
	static void OnUpdateSidedness(DWORD_PTR dwData, const COleVariant& value)
	{
		SIDEDNESS* sidedness = (SIDEDNESS*)dwData;
		*sidedness = value.boolVal ? DOUBLE_SIDED : SINGLE_SIDED;
	}
	static void OnUpdateColor(DWORD_PTR dwData,const COLORREF color)
	{
		CSurfaceCommand *pSrf = (CSurfaceCommand*)dwData;
		pSrf->_pSurface->Color.r = pSrf->_pGLSurface->_color[0] = R(color);
		pSrf->_pSurface->Color.g = pSrf->_pGLSurface->_color[1] = G(color);
		pSrf->_pSurface->Color.b = pSrf->_pGLSurface->_color[2] = B(color);
	}
	static void OnUpdateCulling(DWORD_PTR dwData, const COleVariant& value)
	{
		CGLSurface* pSrf = (CGLSurface*)dwData;
		_bstr_t val(value.bstrVal);
		if (val == _bstr_t("Off"))
		{
			pSrf->AdjustCulling(FALSE);
		}
		else
		{
			if (val == _bstr_t("GL_FRONT"))
				pSrf->AdjustCulling(TRUE, GL_FRONT);
			else
				pSrf->AdjustCulling(TRUE, GL_BACK);
		}
	}
public:
	CSurfaceCommand(Surface * pSurface, CGLSurface * pGLSurface) : 
		_pSurface(pSurface),
		_pGLSurface(pGLSurface) {};
	~CSurfaceCommand(void)
	{
	}
	virtual void OnSelectionChanged(CPropertiesPane& propertiesWnd)
	{
		CString tempStr;
		BOOL bNameValid = tempStr.LoadString(IDS_SURFACE_COLOR);
		ASSERT(bNameValid);
		// Color
		CMFCOnUpdateValueCallbackColorProperty* pColorProp = 
			new CMFCOnUpdateValueCallbackColorProperty(tempStr, 
				RGB(_pSurface->Color.r * 255.0,
					_pSurface->Color.g * 255.0, 
					_pSurface->Color.b * 255.0),
				CMFCOnUpdateValueCallbackColorProperty::OnUpdateValueCallback(OnUpdateColor),
				NULL, _T(""), 
				(DWORD_PTR)this);
		propertiesWnd.m_wndPropList.AddProperty(pColorProp);
		// Smoothing
		bNameValid = tempStr.LoadString(IDS_SURFACE_SMOOTHING);
		ASSERT(bNameValid);
		CMFCOnUpdateValueCallbackProperty* pPropSmoothing = 
			new CMFCOnUpdateValueCallbackProperty(tempStr, 
				_variant_t(SMOOTHING(_pSurface->smoothing)),
				CMFCOnUpdateValueCallbackProperty::OnUpdateValueCallback(OnUpdateSmoothing),
				_T(""),
				(DWORD_PTR)&_pSurface->smoothing);
		propertiesWnd.m_wndPropList.AddProperty(pPropSmoothing);
		// Surface name
		bNameValid = tempStr.LoadString(IDS_SURFACE_NAME);
		ASSERT(bNameValid);
		CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(tempStr, (COleVariant&)_variant_t(_pSurface->name));
		propertiesWnd.m_wndPropList.AddProperty(pProp);
		pProp->Enable(FALSE);
		// Sidedness
		bNameValid = tempStr.LoadString(IDS_SURFACE_DOUBLESIDED);
		ASSERT(bNameValid);
		CMFCOnUpdateValueCallbackProperty* pPropSidedness = 
			new CMFCOnUpdateValueCallbackProperty(tempStr, 
				_variant_t(_pSurface->sidedness == DOUBLE_SIDED),
				CMFCOnUpdateValueCallbackProperty::OnUpdateValueCallback(OnUpdateSidedness),
				_T(""),
				(DWORD_PTR)&_pSurface->sidedness);
		propertiesWnd.m_wndPropList.AddProperty(pPropSidedness);
		// Polygon assignment starting offset
		bNameValid = tempStr.LoadString(IDS_SURFACE_POLYGON_OFFSET);
		ASSERT(bNameValid);
		// TODO:: _pSurface->poly_offset is obolote, us layer sections
		//pProp = new CMFCPropertyGridProperty(tempStr, (COleVariant&)_variant_t(_pSurface->poly_offset));
		//propertiesWnd.m_wndPropList.AddProperty(pProp);
		//pProp->Enable(FALSE);
		//// Assigned polygon count
		//bNameValid = tempStr.LoadString(IDS_SURFACE_POLYGON_COUNT);
		//ASSERT(bNameValid);
		//pProp = new CMFCPropertyGridProperty(tempStr, (COleVariant&)_variant_t(_pSurface->poly_count));
		//pProp->Enable(FALSE);
		//propertiesWnd.m_wndPropList.AddProperty(pProp);

		// Face culling
		bNameValid = tempStr.LoadString(IDS_FACE_CULLING);
		ASSERT(bNameValid);
		const int selected = (_pGLSurface->_enableCaps.find(GL_CULL_FACE) == _pGLSurface->_enableCaps.end()) ? 0 : (_pGLSurface->_cullFace == GL_FRONT) ? 1 : 2;
		static LPCTSTR faceCullingNames[]= {_T("Off"), _T("GL_FRONT"), _T("GL_BACK")};
		pProp = new CMFCOnUpdateValueCallbackProperty(tempStr,
			_variant_t(""),
				CMFCOnUpdateValueCallbackProperty::OnUpdateValueCallback(OnUpdateCulling),
				_T(""),
				(DWORD_PTR)_pGLSurface);
		pProp->AddOption(faceCullingNames[0]);
		pProp->AddOption(faceCullingNames[1]);
		pProp->AddOption(faceCullingNames[2]);
		pProp->SetValue(faceCullingNames[selected]);
		propertiesWnd.m_wndPropList.AddProperty(pProp);

		// TODO:: reflection_map, refraction_map

	}
	virtual void DoubleClick()
	{
	}
};
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember)) 
typedef void (CGLSurface::*OnUpdateValueFlt)(const GLfloat);
class CSurfInfoCommand : public CItemCommand
{
	static void OnUpdate(DWORD_PTR dwData, const COleVariant& value)
	{
		CSurfInfoCommand *pInfoCmd = (CSurfInfoCommand*)dwData;
		pInfoCmd->OnUpdateValue(value);
		// afxpropertygridctrl->getowner should register to AFX_WM_PROPERTY_CHANGED -->redraw
	}
protected:
	OnUpdateValueFlt _onUpdateValue;
	SurfInfo& _surfInfo;
	CGLSurface& _glSurface;
	virtual void OnUpdateValue(const COleVariant& value)
	{
		switch (value.vt)
		{
		case VT_R4:
			_surfInfo.val = value.fltVal;
			(_glSurface.*_onUpdateValue)(value.fltVal);
			break;
		}
	}
public:
	CSurfInfoCommand(SurfInfo& surfInfo, CGLSurface& glSurface, OnUpdateValueFlt onUpdateValue) : 
		_surfInfo(surfInfo),
		_glSurface(glSurface),
		_onUpdateValue(onUpdateValue) {}
	~CSurfInfoCommand(void)	{}
	virtual void OnSelectionChanged(CPropertiesPane& propertiesWnd)
	{
		CString tempStr;
		BOOL bNameValid = tempStr.LoadString(IDS_VALUE);
		ASSERT(bNameValid);
		/*CMFCOnUpdateValueCallbackProperty* pProperty = new CMFCOnUpdateValueCallbackProperty(tempStr, _variant_t(0.f/1.f),
			CMFCOnUpdateValueCallbackProperty::OnUpdateValueCallback(OnUpdate));*/
		CMFCButtonPropertyGridProperty* pProperty = new CMFCButtonPropertyGridProperty(tempStr, _variant_t(-1.f),
			CMFCOnUpdateValueCallbackProperty::OnUpdateValueCallback(OnUpdate));
		pProperty->SetData((DWORD_PTR)this);
		pProperty->SetValue(_surfInfo.val);
		propertiesWnd.m_wndPropList.AddProperty(pProperty);
	}
	virtual void DoubleClick()
	{
		// TODO::
	}
};

class CImageCommand // : public CItemCommand
{
public:
	static void AddImageProperties(Image * image, CMFCPropertyGridProperty * pGroup)
	{
		ASSERT(image);
		if (!image)
			return;
		// Path
		pGroup->AddSubItem(
			CItemCommand::CreateProperty(IDS_PATH, 
				COleVariant(image->path),
				(DWORD_PTR)&image->path));
		// Image type
		static LPCTSTR imageTypeNames[]= {_T("STILL_IMAGE"), _T("SEQUENCE_IMAGE")};
		pGroup->AddSubItem(
			CItemCommand::CreatePropertyList(IDS_IMAGE_TYPE, 
			COleVariant(imageTypeNames[image->type]),
				(DWORD_PTR)&image->type, imageTypeNames, ARRAY_SIZE(imageTypeNames)));
		// Brightness
		pGroup->AddSubItem(
			CItemCommand::CreateProperty(IDS_BRIGHTNESS, 
				COleVariant(image->brightness),
				(DWORD_PTR)&image->brightness));
		// Contrast
		pGroup->AddSubItem(
			CItemCommand::CreateProperty(IDS_CONTRAST, 
				COleVariant(image->contrast),
				(DWORD_PTR)&image->contrast));
		// Hue
		pGroup->AddSubItem(
			CItemCommand::CreateProperty(IDS_HUE, 
				COleVariant(image->hue),
				(DWORD_PTR)&image->hue));

		// Saturation
		pGroup->AddSubItem(
			CItemCommand::CreateProperty(IDS_SATURATION, 
				COleVariant(image->saturation),
				(DWORD_PTR)&image->saturation));

		// Gamma
		pGroup->AddSubItem(
			CItemCommand::CreateProperty(IDS_GAMMA, 
				COleVariant(image->gamma),
				(DWORD_PTR)&image->gamma));

		// Invert
		pGroup->AddSubItem(
			CItemCommand::CreateProperty(IDS_INVERT, 
				COleVariant((long)image->invert, VT_BOOL),
				(DWORD_PTR)&image->invert));
	}
};

class CLayerCommand : public CItemCommand, CImageCommand
{
	SurfLayer * _pLayers;
public:
	CLayerCommand(SurfLayer * pLayers) : _pLayers(pLayers) {};
	~CLayerCommand(void)
	{
	}
	virtual void OnSelectionChanged(CPropertiesPane& propertiesWnd)
	{
		SurfLayer * pLayer = _pLayers;
		static LPCTSTR layerTypeNames[] = {_T("IMAGE_LAYER"), _T("PROCEDURAL_LAYER"), _T("GRADIENT_LAYER")},
			blendModeNames[] = {_T("NORMAL_BLEND"), _T("ADDITIVE_BLEND"), _T("SUBSTRACTIVE_BLEND"), _T("DIFFERENCE_BLEND"), _T("MULTIPLY_BLEND"), _T("DIVIDE_BLEND"), _T("ALPHA_BLEND"), _T("TEXTURE_DISPLACEMENT_BLEND")};
		while (pLayer)
		{
			CString strLayerName;
			strLayerName.Format(_T("%s, %s"),
				(pLayer->image)? _tcsrchr(pLayer->image->path, '\\') : layerTypeNames[pLayer->type],
				blendModeNames[pLayer->blend_mode]);
			CMFCPropertyGridProperty*  pLayerGroup = new CMFCPropertyGridProperty(strLayerName);
			
			// Type
			CString tempStr;
			BOOL bNameValid = tempStr.LoadString(IDS_TYPE);
			ASSERT(bNameValid);
			CMFCPropertyGridProperty*  pProperty = new CMFCPropertyGridProperty(tempStr, COleVariant(layerTypeNames[pLayer->type]));
			for (int i = 0;i<ARRAY_SIZE(layerTypeNames); i++)
			{
				pProperty->AddOption(layerTypeNames[i]);
			}
			// TODO:: wire up value handling, or set it to readonly
			pLayerGroup->AddSubItem(pProperty);

			// Blend mode
			bNameValid = tempStr.LoadString(IDS_BLEND_MODE);
			ASSERT(bNameValid);
			pProperty = new CMFCPropertyGridProperty(tempStr, COleVariant(blendModeNames[pLayer->blend_mode]));
			for (int i = 0;i<ARRAY_SIZE(blendModeNames); i++)
			{
				pProperty->AddOption(blendModeNames[i]);
			}
			// TODO:: wire up value handling, or set it to readonly
			pLayerGroup->AddSubItem(pProperty);

			// Projection
			static const char* projectionNames[] = {"PLANAR_PROJECTION", "CYLINDERAL_PROJECTION", "SPHERICAL_PROJECTION", "CUBIC_PROJECTION", "FRONT_PROJECTION", "UV_PROJECTION"};
			bNameValid = tempStr.LoadString(IDS_PROJECTION);
			ASSERT(bNameValid);
			pProperty = new CMFCPropertyGridProperty(tempStr, COleVariant(projectionNames[pLayer->projection]));
			for (int i = 0;i<ARRAY_SIZE(projectionNames); i++)
			{
				pProperty->AddOption(projectionNames[i]);
			}
			// TODO:: wire up value handling, or set it to readonly
			pLayerGroup->AddSubItem(pProperty);
			
			// UV map
			bNameValid = tempStr.LoadString(IDS_UVMAP);
			ASSERT(bNameValid);
			pProperty = new CMFCPropertyGridProperty(tempStr, _variant_t(pLayer->uvmap), _T(""));
			pProperty->SetData((DWORD_PTR)&pLayer->uvmap);
			pLayerGroup->AddSubItem(pProperty);

			// Opacity
			bNameValid = tempStr.LoadString(IDS_OPACITY);
			ASSERT(bNameValid);
			pProperty = new CMFCPropertyGridProperty(tempStr, COleVariant(pLayer->opacity), _T(""));
			pProperty->SetData((DWORD_PTR)&pLayer->opacity);
			pLayerGroup->AddSubItem(pProperty);

			// Opacity
			bNameValid = tempStr.LoadString(IDS_INVERT);
			ASSERT(bNameValid);
			pProperty = new CMFCPropertyGridProperty(tempStr, _variant_t((bool)pLayer->invert), _T(""));
			pProperty->SetData((DWORD_PTR)&pLayer->invert);
			pLayerGroup->AddSubItem(pProperty);

			static const char* tileTypeNames[] = {"RESET_TILE", "REPEAT_TILE", "MIRROR_TILE", "EDGE_TILE"};
			// Width tile
			bNameValid = tempStr.LoadString(IDS_WIDTH_TILE);
			ASSERT(bNameValid);
			pProperty = new CMFCPropertyGridProperty(tempStr, COleVariant(tileTypeNames[pLayer->width_tile]));
			for (int i = 0;i<ARRAY_SIZE(tileTypeNames); i++)
			{
				pProperty->AddOption(tileTypeNames[i]);
			}
			// TODO:: wire up value handling, or set it to readonly
			pLayerGroup->AddSubItem(pProperty);


			// Height tile
			bNameValid = tempStr.LoadString(IDS_HEIGHT_TILE);
			ASSERT(bNameValid);
			pProperty = new CMFCPropertyGridProperty(tempStr, COleVariant(tileTypeNames[pLayer->height_tile]));
			for (int i = 0;i<ARRAY_SIZE(tileTypeNames); i++)
			{
				pProperty->AddOption(tileTypeNames[i]);
			}
			// TODO:: wire up value handling, or set it to readonly
			pLayerGroup->AddSubItem(pProperty);

			// Pixel blending
			bNameValid = tempStr.LoadString(IDS_PIXEL_BLENDING);
			ASSERT(bNameValid);
			pProperty = new CMFCPropertyGridProperty(tempStr, _variant_t((bool)pLayer->pixel_blending), _T(""));
			pProperty->SetData((DWORD_PTR)&pLayer->pixel_blending);
			pLayerGroup->AddSubItem(pProperty);

			if (pLayer->image)
			{
				bNameValid = tempStr.LoadString(IDS_IMAGE);
				ASSERT(bNameValid);
				CMFCPropertyGridProperty*  pImageGroup = new CMFCPropertyGridProperty(tempStr);
				CImageCommand::AddImageProperties(pLayer->image, pImageGroup);
				pLayerGroup->AddSubItem(pImageGroup);
			}

			propertiesWnd.m_wndPropList.AddProperty(pLayerGroup);
			pLayer = pLayer->next;
		}
	}
	virtual void DoubleClick()
	{
		// TODO::
	}
};
class CRefMapCommand : public CItemCommand, CImageCommand
{
	RefMap * _pRefMap;
public:
	CRefMapCommand(RefMap * pRefMap) : _pRefMap(pRefMap) {};
	~CRefMapCommand(void)
	{
	}
	virtual void OnSelectionChanged(CPropertiesPane& propertiesWnd)
	{
		// Ref. opt.
		static const char * refMapOptNames[] = {"BACKDROP", "RAY_TRACING_BACKDROP", "SPHERICAL", "RAY_TRACING_SPHERICAL"};
		CMFCPropertyGridProperty * pProperty;
		propertiesWnd.m_wndPropList.AddProperty(pProperty = CItemCommand::CreatePropertyList(IDS_OPTION, 
			_pRefMap ? COleVariant(refMapOptNames[_pRefMap->opt]) : COleVariant(_T("")), 
			(DWORD_PTR)(_pRefMap ? &_pRefMap->opt : NULL),
			refMapOptNames,
			ARRAY_SIZE(refMapOptNames)));
		pProperty->Enable(_pRefMap != NULL);
		
		// Image seam angle
		propertiesWnd.m_wndPropList.AddProperty(pProperty = CItemCommand::CreateProperty(IDS_SEAM_ANGLE, 
			_pRefMap ? COleVariant(_pRefMap->seam_angle) : COleVariant(.0f),
			(DWORD_PTR)(_pRefMap ? &_pRefMap->seam_angle : NULL)));
		pProperty->Enable(_pRefMap != NULL);

		// Blurring
		propertiesWnd.m_wndPropList.AddProperty(pProperty = CItemCommand::CreateProperty(IDS_BLURRING, 
			_pRefMap ? COleVariant(_pRefMap->blurring) : COleVariant(.0f), 
			(DWORD_PTR)(_pRefMap ? &_pRefMap->blurring : NULL)));
		pProperty->Enable(_pRefMap != NULL);

		// Image
		CString tempStr;
		BOOL bNameValid = tempStr.LoadString(IDS_IMAGE);
		ASSERT(bNameValid);
		CMFCPropertyGridProperty*  pImageGroup = new CMFCPropertyGridProperty(tempStr);
		if (_pRefMap)
			CImageCommand::AddImageProperties(_pRefMap->map, pImageGroup);
		pImageGroup->Enable(_pRefMap != NULL);
		propertiesWnd.m_wndPropList.AddProperty(pImageGroup);
		
	}
	virtual void DoubleClick()
	{
		// TODO::
	}
};

class CShaderProgramCommand : public CItemCommand
{
	CGLShader* _pShader;
public:
	CShaderProgramCommand(CGLShader* pShader) : _pShader(pShader) {};
	~CShaderProgramCommand(void)
	{
	}
	virtual void OnSelectionChanged(CPropertiesPane& propertiesWnd)
	{
		// TODO:: Show item's property
	}
	virtual void DoubleClick()
	{
	//		CString tmp;
	//pDocTemplate->GetDocString(tmp, CDocTemplate::filterName);
	//pDocTemplate->GetDocString(tmp, CDocTemplate::filterExt);
		CtestMSVC2App* pApp = static_cast<CtestMSVC2App*>(::AfxGetApp());
		////CString path(::CAppConstants::shader_dir);
		////path += _pShader->GetFName();
		CDocument * pDoc = pApp->m_pDocManager->OpenDocumentFile(_pShader->GetFName());
		if (pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CtestMSVC2Doc)))
		{
			((CtestMSVC2Doc*)pDoc)->m_pShader = _pShader;
		}
		//POSITION pos = pApp->m_pDocManager->GetFirstDocTemplatePosition();
		//CDocTemplate* pTemplate = pApp->m_pDocManager->GetNextDocTemplate(pos);
		//CDocument* pDoc = NULL;
		//while (pTemplate)
		//{
		//	if (CDocTemplate::yesAttemptNative == pTemplate->MatchDocType(_pShader->GetFName(), pDoc))
		//	{
		//		if (pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CtestMSVC2Doc)))
		//		{
		//			// TODO::
		//			//	Activate the already opened view
		//			POSITION pos = pDoc->GetFirstViewPosition();
		//			ASSERT(pos);
		//			pDoc->GetNextView(pos)->SetFocus();
		//		}
		//		else
		//		{
		//			CString path(::CAppConstants::shader_dir);
		//			path += _pShader->GetFName();
		//			CDocument * pDoc = pTemplate->OpenDocumentFile(path);

		//		}
		//		break;
		//	}

		//	if (!pos)
		//		break;
		//	pTemplate = pApp->m_pDocManager->GetNextDocTemplate(pos);
		//}
		//CtestMSVC2Doc * pShaderProgramDoc = static_cast<CtestMSVC2Doc *>(pDoc);
		
	//pTemplate->CreateNewDocument(
	//CDocTemplate* pTemplate = m_pGLTemplate;
	//ASSERT(pTemplate != NULL);
	//ASSERT_KINDOF(CDocTemplate, pTemplate);
	//pTemplate->OpenDocumentFile(NULL);
	//POSITION docPos = pTemplate->GetFirstDocPosition();
	//CDocument * pDoc = pTemplate->GetNextDoc(docPos);
	//ASSERT_KINDOF(CGLDoc, pDoc);
	//CGLDoc * pGLDoc = (CGLDoc*)pDoc;

	}
	
};

class CProgramCommand : public CItemCommand
{
	CGLProgram* _pProgram;
public:
	CProgramCommand(CGLProgram* pProgram) : _pProgram(pProgram) {};
	virtual void OnSelectionChanged(CPropertiesPane& propertiesWnd)
	{
		CString tempStr;
		BOOL bNameValid = tempStr.LoadString(IDS_PATH);
		ASSERT(bNameValid);
		CMFCPropertyGridProperty* pProperty = new CMFCPropertyGridProperty(tempStr, COleVariant(_pProgram->GetFName().c_str()));
		propertiesWnd.m_wndPropList.AddProperty(pProperty);
		bNameValid = tempStr.LoadString(IDS_VERTEX_PROGRAM);
		ASSERT(bNameValid);
		pProperty = new CMFCPropertyGridProperty(tempStr, COleVariant(_pProgram->vs.GetFName()));
		propertiesWnd.m_wndPropList.AddProperty(pProperty);
		bNameValid = tempStr.LoadString(IDS_FRAGMENT_PROGRAM);
		ASSERT(bNameValid);
		pProperty = new CMFCPropertyGridProperty(tempStr, COleVariant(_pProgram->fs.GetFName()));
		propertiesWnd.m_wndPropList.AddProperty(pProperty);
	}
	virtual void DoubleClick()
	{
	}
};

int CSceneBrowser::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndTree.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	OnChangeVisualStyle();
	return 0;
}

void CSceneBrowser::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CSceneBrowser::AdjustLayout(void)
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndTree.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CSceneBrowser::OnSceneOpen(Scene::CGLScene * pGLScene)
{
	CString caption;
	BOOL bNameValid = caption.LoadString(IDS_SCENE);
	ASSERT(bNameValid);
	HTREEITEM hRoot = m_wndTree.InsertItem(caption, 0, 0);
	m_wndTree.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	// Lights...
	CWinApp* pWinApp = ::AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pWinApp->m_pMainWnd;
	CLightsWnd * pLightsWnd = (CLightsWnd*)pMainFrame->GetPane(ID_VIEW_LIGHTSWND);
    // Renderer....
    bNameValid = caption.LoadString(IDS_RENDERER);
	ASSERT(bNameValid);
    HTREEITEM hRenderer = m_wndTree.InsertItem(caption, 1, 1, hRoot);
	// Renderer program objects...
	bNameValid = caption.LoadString(IDS_PROGRAM);
	ASSERT(bNameValid);
	HTREEITEM hProgram = m_wndTree.InsertItem(caption, 4, 4, hRenderer);
	m_wndTree.SetItemData(hProgram, (DWORD_PTR)new CProgramCommand(pGLScene->_renderer->_program));

	bNameValid = caption.LoadString(IDS_VERTEX_SHADER);
	ASSERT(bNameValid);
	HTREEITEM hVertex = m_wndTree.InsertItem(caption, 5, 5, hProgram);
	m_wndTree.SetItemData(hVertex, (DWORD_PTR)new CShaderProgramCommand(&pGLScene->_renderer->_program->vs));

	bNameValid = caption.LoadString(IDS_FRAGMENT_SHADER);
	ASSERT(bNameValid);
	HTREEITEM hFragment = m_wndTree.InsertItem(caption, 6, 6, hProgram);
	m_wndTree.SetItemData(hFragment, (DWORD_PTR)new CShaderProgramCommand(&pGLScene->_renderer->_program->fs));
	m_wndTree.Expand(hRenderer, TVE_EXPAND);

	// Meshes...
	if (!pGLScene->_loaded)
		return;
	pLightsWnd->InitLightsList(pGLScene);
	for (Meshes::iterator it = pGLScene->_meshes.begin(); it != pGLScene->_meshes.end(); it++)
	{
		CMesh * pMesh = it->first.get();
		HTREEITEM hMesh = m_wndTree.InsertItem(pMesh->m_szName.c_str(), 1, 1, hRoot);
		m_wndTree.SetItemData(hMesh, (DWORD_PTR) new CMeshCommand(pMesh));
		// Surfaces...
		for (long j = 0; j < pMesh->surfaces.size(); j++)
		{
			Surface * pSurface = &pMesh->surfaces[j];
			CGLSurface * pGLSurface = &pMesh->_glSurfaces[j];
			HTREEITEM hSurface = m_wndTree.InsertItem(pSurface->name, 2, 2, hMesh);
			
			m_wndTree.SetItemData(hSurface, (DWORD_PTR) new CSurfaceCommand(pSurface, pGLSurface));
			// Program objects...
			bNameValid = caption.LoadString(IDS_PROGRAM);
			ASSERT(bNameValid);
			HTREEITEM hProgram = m_wndTree.InsertItem(caption, 4, 4, hSurface);
			m_wndTree.SetItemData(hProgram, (DWORD_PTR)new CProgramCommand(pGLSurface->_program));

			bNameValid = caption.LoadString(IDS_VERTEX_SHADER);
			ASSERT(bNameValid);
			HTREEITEM hVertex = m_wndTree.InsertItem(caption, 5, 5, hProgram);
			m_wndTree.SetItemData(hVertex, (DWORD_PTR)new CShaderProgramCommand(&pGLSurface->_program->vs));

			bNameValid = caption.LoadString(IDS_FRAGMENT_SHADER);
			ASSERT(bNameValid);
			HTREEITEM hFragment = m_wndTree.InsertItem(caption, 6, 6, hProgram);
			m_wndTree.SetItemData(hFragment, (DWORD_PTR)new CShaderProgramCommand(&pGLSurface->_program->fs));
			m_wndTree.Expand(hSurface, TVE_EXPAND);

			// Fill map types
			const int mapTypeResIDs[] = {IDS_COLOR_MAP,
				IDS_LUMINOSITY_MAP,
				IDS_DIFFUSE_MAP,
				IDS_SPECULAR_MAP,
				IDS_GLOSSINESS_MAP,
				IDS_REFLECTION_MAP,
				IDS_TRANSPARENCY_MAP,
				IDS_REFRACTION_MAP,
				IDS_TRANSLUCENCY_MAP,
				IDS_BUMP_MAP};
			HTREEITEM hTreeItem;
			SurfInfo * pSurfInfo = pSurface->surface_infos;
			for (int i = 0;i<(int)NUM_MAP_TYPE;i++)
			{
				// Surface info
				bNameValid = caption.LoadString(mapTypeResIDs[i]);
				ASSERT(bNameValid);
				hTreeItem = m_wndTree.InsertItem(caption, 5, 5, hSurface);
				switch(i)
				{
				case TRANSPARENCY_MAP:
					m_wndTree.SetItemData(hTreeItem, (DWORD_PTR)new CSurfInfoCommand(*pSurfInfo, *pGLSurface, &CGLSurface::AdjustBlending ));
					break;
				case SPECULARITY_MAP:
					m_wndTree.SetItemData(hTreeItem, (DWORD_PTR)new CSurfInfoCommand(*pSurfInfo, *pGLSurface, &CGLSurface::AdjustSpecularity ));
					break;
				case GLOSSINESS_MAP:
					m_wndTree.SetItemData(hTreeItem, (DWORD_PTR)new CSurfInfoCommand(*pSurfInfo, *pGLSurface, &CGLSurface::AdjustShininess ));
					break;
				default:
					m_wndTree.SetItemData(hTreeItem, (DWORD_PTR)new CSurfInfoCommand(*pSurfInfo, *pGLSurface, &CGLSurface::AdjustDummy ));
					break;
				}
				
				// Layers
				if (pSurfInfo->layers)
				{
					bNameValid = caption.LoadString(IDS_LAYERS);
					ASSERT(bNameValid);
					hTreeItem = m_wndTree.InsertItem(caption, 6, 6, hTreeItem);
					m_wndTree.SetItemData(hTreeItem, (DWORD_PTR)new CLayerCommand(pSurfInfo->layers));
				}
				pSurfInfo++;
			}
			// Reflection map
			bNameValid = caption.LoadString(IDS_REFLECTION_MAP);
			ASSERT(bNameValid);
			hTreeItem = m_wndTree.InsertItem(caption, 1, 1, hSurface);
			m_wndTree.SetItemData(hTreeItem, (DWORD_PTR)new CRefMapCommand(pSurface->reflection_map));
			// Refraction map
			bNameValid = caption.LoadString(IDS_REFRACTION_MAP);
			ASSERT(bNameValid);
			hTreeItem = m_wndTree.InsertItem(caption, 1, 1, hSurface);
			m_wndTree.SetItemData(hTreeItem, (DWORD_PTR)new CRefMapCommand(pSurface->refraction_map));
		}
		
		m_wndTree.Expand(hMesh, TVE_EXPAND);
	}
	m_wndTree.Expand(hRoot, TVE_EXPAND);
}

void CSceneBrowser::OnChangeVisualStyle()
{
	m_treeViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_treeViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_treeViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndTree.SetImageList(&m_treeViewImages, TVSIL_NORMAL);
}
void CSceneBrowser::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndTree.SetFocus();
}

void CSceneBrowser::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

BOOL CSceneBrowser::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	LPNMHDR nmhdr = (LPNMHDR) lParam;
	switch (nmhdr->code)
	{
	case TVN_SELCHANGED:
		{
			LPNMTREEVIEW pnmtv = (LPNMTREEVIEW) lParam;
			HTREEITEM hItem = pnmtv->itemNew.hItem;
			if (!pnmtv->itemNew.hItem)
				break;
			DWORD_PTR ptr = m_wndTree.GetItemData(hItem);
			if (!ptr)
				break;
			CItemCommand* pItemCommand = reinterpret_cast<CItemCommand*>(ptr);
			CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
			pMainFrame->m_wndProperties.ClearProperties();
			pItemCommand->OnSelectionChanged(pMainFrame->m_wndProperties);
			break;
		}
	case NM_DBLCLK:
		{
			HTREEITEM hItem = m_wndTree.GetSelectedItem();
			DWORD_PTR ptr = m_wndTree.GetItemData(hItem);
			if (!ptr)
				break;
			CItemCommand* pItemCommand = (CItemCommand*)ptr;
			pItemCommand->DoubleClick();
			break;
		}
	}
	return CDockablePane::OnNotify(wParam, lParam, pResult);
}

void CSceneBrowser::DestroyItemData(HTREEITEM hItem)
{
	DWORD_PTR ptr = m_wndTree.GetItemData(hItem);
	if (ptr)
	{
		delete (CItemCommand*)ptr;
		m_wndTree.SetItemData(hItem, NULL);
	}
}

void CSceneBrowser::DestroyAllItemData(HTREEITEM hItem)
{
	if (!hItem)
		return;
	DestroyItemData(hItem);
	HTREEITEM hChildItem = m_wndTree.GetNextItem(hItem, TVGN_CHILD);
	DestroyAllItemData(hChildItem);
	HTREEITEM hNextItem = m_wndTree.GetNextItem(hItem, TVGN_NEXT);
	DestroyAllItemData(hNextItem);
}

void CSceneBrowser::DestroyAllItemData()
{
	HTREEITEM hItemRoot = m_wndTree.GetRootItem();
	DestroyAllItemData(hItemRoot);
}

void CSceneBrowser::OnDestroy()
{
	DestroyAllItemData();
	CDockablePane::OnDestroy();
}


void CSceneBrowser::OnContextMenu(CWnd* pWnd, CPoint point)
{
    CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndTree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}
    if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
        pWndTree->SetFocus();
        CItemCommand* pCmdItem = reinterpret_cast<CItemCommand*>(pWndTree->GetItemData(hTreeItem));
        CSurfaceCommand* pSrfCmd = dynamic_cast<CSurfaceCommand*>(pCmdItem);
        if (pSrfCmd)
        {
           
            theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_SURFACE, point.x, point.y, this, TRUE);
        }
        
	}
    else
	    pWndTree->SetFocus();
	
}


void CSceneBrowser::OnSurfaceOverrideprogram()
{
    CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndTree;
    ASSERT_VALID(pWndTree);
    HTREEITEM hTreeItem = pWndTree->GetSelectedItem();
    CItemCommand* pCmdItem = reinterpret_cast<CItemCommand*>(pWndTree->GetItemData(hTreeItem));
    ASSERT(pCmdItem);
    CSurfaceCommand* pSrfCmd = dynamic_cast<CSurfaceCommand*>(pCmdItem);
    ASSERT(pSrfCmd);
    // Retrieve mesh file name...
    HTREEITEM hParent = pWndTree->GetParentItem(hTreeItem);
    pCmdItem = reinterpret_cast<CItemCommand*>(pWndTree->GetItemData(hParent));
    ASSERT(pCmdItem);
    CMeshCommand * pMeshCmd = dynamic_cast<CMeshCommand*>(pCmdItem);
    ASSERT(pMeshCmd);
    // ...
    std::string str(::CAppConstants::program_dir);
    IO::CPath path(str);
    path.Combine(IO::CPath::GetFileName(pMeshCmd->_pMesh->m_szName));
    IO::CFileSystem::MkDir(path);
    path.Combine(pSrfCmd->_pSurface->name);
    path.SetExt(std::string(_T(::CAppConstants::program_ext)));
    Log::CLog::Write(_T("Creating %s"), path);
    try
    {
        IO::CFileWriter * fw = IO::CFileWriter::Open(path, FILE_REWRITE | FILE_TEXT);
        IO::CPath vsName(pSrfCmd->_pSurface->name);
        vsName.SetExt(::CAppConstants::vs_ext);
        Log::CLog::Write(_T("Writing %s"), vsName.operator const std::string &().c_str()); 
        fw->Write((void*)vsName.operator const char *(), vsName.operator const std::string &().length());
        fw->Write((void*)"\n", 1);
        IO::CPath fsName(pSrfCmd->_pSurface->name);
        fsName.SetExt(::CAppConstants::fs_ext);
         Log::CLog::Write(_T("Writing %s"), fsName.operator const std::string &().c_str()); 
        fw->Write((void*)fsName.operator const char *(), fsName.operator const std::string &().length());
        fw->Write((void*)"\n", 1);
        fw->Close();
    }catch(CFileCannotOpenException& ex)
    {
         Log::CLog::Write(_T("Cannot create..."));
         return;
    }
    // TODO:: create shaders, and contents
}

