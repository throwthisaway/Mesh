#include "panel.h"
#include <lwserver.h>
#include <lwpanel.h>
#include <string.h>
#include "shader_panel.h"

//#include <lwxpanel.h>
//#define SAVEFILECTRL 0x8000
//#define CONVERT_TO_TRIANGLES 0x8001
//#define PROGRAM_PREFIX 0x8002
//void CH_notify ( LWXPanelID panID, unsigned long cid, unsigned long vid, int event_type );
//static LWXPanelControl controls[] = {{SAVEFILECTRL, "Output file name:", "sFileName"}, {CONVERT_TO_TRIANGLES, "Convert to triangles", "iBoolean"}, {PROGRAM_PREFIX,"Shader programs directory:","string"},{0}};
//static LWXPanelHint hints_panel[] = {XpLABEL(0, "Mesh Export"),XpENABLE_( 0),XpENABLE_( SAVEFILECTRL),  XpXREQCFG(SAVEFILECTRL, LWXPREQ_SAVE, "Save Output File As...", "*.mesh"), XpEND};
////static LWXPanelHint hints_savefile[] = { , XpEND}; 
////static LWXPanelHint hints_converttotriangles[] ={XpEND};
////static LWXPanelDataDesc data[] = {{0x8000, "Output file name:", "sFileName"}};
//int ShowPanel(GlobalFunc * global)
//{
//	LWXPanelFuncs *xpanf;
//	LWXPanelID ID;
//	xpanf = global( LWXPANELFUNCS_GLOBAL, GFUSE_TRANSIENT );
//	if (ID = xpanf->create(LWXP_FORM, controls))
//	{
//		xpanf->hint(ID, 0, hints_panel);
//		xpanf->formSet(ID, SAVEFILECTRL, "c:\\test4.mesh");
//		xpanf->formSet(ID, PROGRAM_PREFIX, "c:\\data\\shader_programs");		
//		//return xpanf->open(ID);
//		return xpanf->post(ID);
//	}
//	return 0;
//}
#define DEF_WIDTH 45
#define DEF_BORDER_WIDTH 320
#define DEF_SURFACE_LB_WIDTH 280
#define DEF_SURFACE_LIST_HEIGHT 8
static struct st_controls
{
	LWControl *ctl, *save_file, *program_dir, *border, *convert_to_triangles, *surface_list, *shaderselect_button;
}controls;
static MeshExport * meshExport;

char edit[ 80 ] = "This is an edit field.";

// Required by macros...
static LWPanControlDesc desc;
static LWValue
  ival    = { LWT_INTEGER },
  ivecval = { LWT_VINT },
  fval    = { LWT_FLOAT },
  fvecval = { LWT_VFLOAT },
  sval    = { LWT_STRING };
// ...Required by macros

// surface and shaders listbox event handler
void snslb_event(LWControl *ctl,  void *userdata)
{
	
}

// surface and shaders listbox name and column callback fn
const char * snslb_columns[] = {"Surface", "Shader" };
const int snslb_columnwidths[] = {140, 140 };
char * snslb_mname( void *userdata, int index, int column )
{
	if (index<0)
		return (char*)snslb_columns[column];
	MeshExport * meshExport = (userdata) ? (MeshExport *)userdata: ::meshExport;
	return (column == 0) ? (char *)meshExport->surff->name(meshExport->surfIDList[index]) : ((meshExport->shaders_to_surfIDs[index].empty()) ? "<Shader name>":meshExport->shaders_to_surfIDs[index].c_str());
}
int snslb_colwidth( void *userdata, int index )
{
	return (index < sizeof(snslb_columns) / sizeof(snslb_columns[0])) ? snslb_columnwidths[index]:0;
}

// listbox name and count callback
int slb_count( void *userdata )
{
	MeshExport * meshExport = (userdata) ? (MeshExport *)userdata: ::meshExport;
	return meshExport->surf.GetCount();
}
char * slb_name( void *userdata, int index )
{
	MeshExport * meshExport = (userdata) ? (MeshExport *)userdata: ::meshExport;
	return (char *)meshExport->surff->name(meshExport->surfIDList[index]);
}
// shader select button event handler
#define MAX_SURFACE_NAME 256
void ssb_event(LWControl *ctl,  void *userdata)
{
	MeshExport * meshExport = (MeshExport *)userdata;
	char surface_name[MAX_SURFACE_NAME];
	ZeroMemory(surface_name, MAX_SURFACE_NAME);
	int index, selection;
	GET_INT(controls.surface_list, index);
	strcpy(surface_name, "Surface: ");
	strcat(surface_name, meshExport->surff->name(meshExport->surfIDList[index]));
	if (ShowShaderPanel(meshExport->global,  surface_name, meshExport->shaders, &selection))
	{
		meshExport->shaders_to_surfIDs[index] = meshExport->shaders[selection];
		CON_SETEVENT(controls.surface_list, snslb_event, meshExport);
	}

}
int GetSurfIDList(MeshExport * meshExport)
{
	LWStateQueryFuncs *query;

	//Get current object
	query = (LWStateQueryFuncs*)meshExport->global( LWSTATEQUERYFUNCS_GLOBAL, GFUSE_TRANSIENT );
	meshExport->name = query->object();
	//Get surfaceID list
	meshExport->surff = (LWSurfaceFuncs*)meshExport->global( LWSURFACEFUNCS_GLOBAL, GFUSE_TRANSIENT );
	meshExport->surfIDList = meshExport->surff->byObject(meshExport->name);
	meshExport->surf.m_nSurfaces = 0;
	while (meshExport->surfIDList[meshExport->surf.m_nSurfaces] != NULL) ++meshExport->surf.m_nSurfaces;	// Count the number of surfaces
	meshExport->shaders_to_surfIDs = new std::string[meshExport->surf.m_nSurfaces];
	return meshExport->surf.m_nSurfaces;
}
void AlignControl(LWControl * p)
{
    /* align */
	long w = CON_LW(p);
	ival.intv.value = 100 - w;
	p->set(p, CTL_X, &ival );
}
BOOL EnumShaders( MeshExport * meshExport)
{
	WIN32_FIND_DATAA findData;
	char path[MAX_PATH];
	strcpy(path, meshExport->shader_program_dir);
	strcat(path, "\\*.program");
	HANDLE handle = ::FindFirstFileA(path, &findData);
	if (handle == INVALID_HANDLE_VALUE)
		return FALSE;
	meshExport->shaders.push_back(std::string(findData.cFileName));
	while (::FindNextFileA(handle, &findData))
	{
		meshExport->shaders.push_back(std::string(findData.cFileName));
	}
	return TRUE;

}

void CleanupMeshExport(void)
{
	DEL_ARRAY(meshExport->shaders_to_surfIDs);
	meshExport->shaders.clear();
}

// program dir. button event handler
void prgdir_event(LWControl *ctl,  void *userdata)
{	
	MeshExport * meshExport = (MeshExport *)userdata;
	meshExport->shaders.clear();
	GET_STR( controls.program_dir, meshExport->shader_program_dir, sizeof( meshExport->shader_program_dir ));
	EnumShaders(meshExport);
}
int ShowPanel(GlobalFunc * global, MeshExport * meshExport)
{

	int ret;
	LWPanelFuncs *panf;
	LWPanelID ID;
	ret = 0;
	::meshExport = meshExport;
	meshExport->global = global;
	if (panf = (LWPanelFuncs*)global( LWPANELFUNCS_GLOBAL, GFUSE_TRANSIENT ))
	{
		ID = PAN_CREATE(panf,"Mesh Export");
		//panf->user_data = meshExport;
		// Init meshExport...
		strcpy(meshExport->output_file_name, "C:\\test6.mesh");
		strcpy(meshExport->shader_program_dir, "C:\\Users\\throwthisaway\\Documents\\Visual Studio 2008\\Projects\\MeshExport2008\\MeshView2005\\Shaders\\" /*"C:\\Data\\Shaders"*/);
		EnumShaders(meshExport);
		meshExport->convert_to_triangles = 1;
		GetSurfIDList(meshExport);
		// Create controls...
		controls.ctl = STR_CTL( panf, ID, "Edit Me", DEF_WIDTH );
		SET_STR( controls.ctl, edit, sizeof( edit ));
		AlignControl(controls.ctl);

		// Save file dialog
		controls.save_file = SAVE_CTL(panf, ID, "Save file as", DEF_WIDTH);
		SET_STR( controls.save_file, meshExport->output_file_name, sizeof(meshExport->output_file_name));
		AlignControl(controls.save_file);
		// Shaders directory
		controls.program_dir = DIR_CTL(panf, ID, "Shaders directory", DEF_WIDTH);
		SET_STR( controls.program_dir, meshExport->shader_program_dir, sizeof(meshExport->shader_program_dir));
		CON_SETEVENT(controls.program_dir, prgdir_event, meshExport);
		AlignControl(controls.program_dir);
		// Separator 
		controls.border = BORDER_CTL(panf, ID, 0, DEF_BORDER_WIDTH, 0);
		// Triangle checkbox
		controls.convert_to_triangles = BOOL_CTL(panf, ID, "Convert polygons to triangles");
		SET_INT(controls.convert_to_triangles, meshExport->convert_to_triangles);
		// Surfaces listbox
		//controls.surface_list = LISTBOX_CTL(panf, ID, "Surfaces", DEF_SURFACE_LB_WIDTH, DEF_SURFACE_LIST_HEIGHT, slb_name, slb_count);
		controls.surface_list = MULTILIST_CTL(panf, ID, "Surfaces", DEF_SURFACE_LB_WIDTH, DEF_SURFACE_LIST_HEIGHT, snslb_mname, slb_count, snslb_colwidth);
		CON_SETEVENT(controls.surface_list, snslb_event, meshExport);
		controls.shaderselect_button = BUTTON_CTL( panf, ID, "Select shader" );
		CON_SETEVENT(controls.shaderselect_button, ssb_event, meshExport);
		//controls.surface_list->set(ID, CTL_USERDATA, (LWValue *)meshExport);
		if ( ret = panf->open( ID, PANF_BLOCKING | PANF_CANCEL | PANF_FRAME))
		{
			GET_STR( controls.ctl, edit, sizeof( edit ));

			GET_STR( controls.save_file, meshExport->output_file_name, sizeof( meshExport->output_file_name ));
			//GET_STR( controls.program_dir, meshExport->shader_program_dir, sizeof( meshExport->shader_program_dir ));
			GET_INT( controls.convert_to_triangles, meshExport->convert_to_triangles);
		}

		PAN_KILL( panf, ID );
		return ret;

	}
	return ret;
}