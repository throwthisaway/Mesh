#include "mesh_panel.h"
#include "shader_panel.h"
#include <string>
#include "FileWriter.h"
#include "AppConstants.h"
#include "Path.h"
#include "lwhost.h"
#define DEF_WIDTH 45
#define DEF_BORDER_WIDTH 320
#define DEF_SURFACE_LB_WIDTH 280
#define DEF_SURFACE_LIST_HEIGHT 8

static struct st_controls
{
	LWControl *save_file, *program_dir, *border, *convert_to_triangles, *surface_list, *shaderselect_button;
}controls;

#define MAX_SURFACE_NAME 256
const char * snslb_columns[] = {"Surface", "Shader" };
const int snslb_columnwidths[] = {140, 140 };
const std::string newShaderOpt("<new>");
const char * shader_delimiter = ";\n";
const char * def_vs = "void main()\n \
{\n \
	gl_FrontColor = gl_Color;\n \
	gl_Position = ftransform();\n \
}";
const char * def_fs = "void main()\n \
{\n \
	gl_FragColor = gl_Color;\n \
}";
CMeshExport * meshExport;

// Required by macros...
static LWPanControlDesc desc;
static LWValue
  ival    = { LWT_INTEGER },
  ivecval = { LWT_VINT },
  fval    = { LWT_FLOAT },
  fvecval = { LWT_VFLOAT },
  sval    = { LWT_STRING };
// ...Required by macros

// Panel variables...
char output_file_name[MAX_PATH];
char shader_program_dir[MAX_PATH];
int shader_count;
std::vector<std::string> shaders;
BOOL convert_to_triangles;

// ...Panel variables
// surface and shaders listbox event handler
void snslb_event(LWControl *ctl,  void *userdata)
{
    
}
BOOL EnumShaders(void);
// program dir. button event handler
void prgdir_event(LWControl *ctl,  void *userdata)
{	
	shaders.clear();
	GET_STR( controls.program_dir, shader_program_dir, sizeof( shader_program_dir ));
	EnumShaders();
}
// surface and shaders listbox name and column callback fn
char * snslb_mname( void *userdata, int index, int column )
{
	if (index<0)
		return (char*)snslb_columns[column];
	CMeshExport * meshExport = (userdata) ? (CMeshExport *)userdata: ::meshExport;
	return (column == 0) ? (char *)meshExport->surff->name(meshExport->surfIDList[index]) : ((meshExport->shaders_to_surfIDs[index].empty()) ? "<Shader name>":meshExport->shaders_to_surfIDs[index].c_str());
}
int snslb_colwidth( void *userdata, int index )
{
	return (index < sizeof(snslb_columns) / sizeof(snslb_columns[0])) ? snslb_columnwidths[index]:0;
}
// listbox name and count callback
int slb_count( void *userdata )
{
	CMeshExport * meshExport = (userdata) ? (CMeshExport *)userdata: ::meshExport;
	return meshExport->surf.count;
}
char * slb_name( void *userdata, int index )
{
	CMeshExport * meshExport = (userdata) ? (CMeshExport *)userdata: ::meshExport;
	return (char *)meshExport->surff->name(meshExport->surfIDList[index]);
}
int TryCreateShaderProgram(const std::string& fName, std::string& prgName)
{
    IO::CPath prgPath(shader_program_dir);
    prgPath.Combine(fName);
    prgPath.SetExt(CAppConstants::program_ext);
    prgName = prgPath.GetFileNameWExt();
    if (IO::CFile::Exists(prgPath))
    {
        LWMessageFuncs *msgf;
        msgf = (LWMessageFuncs*)::meshExport->global( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );
        if (!msgf->yesNo("Create file", prgPath, "exists. Overwrite?"))
            return -1;
    }
    IO::CFileSystem::MkDir(shader_program_dir);
    std::auto_ptr<IO::CFileWriter> fw(IO::CFileWriter::Open(prgPath, FILE_REWRITE | FILE_TEXT));
    IO::CPath vsPath(fName);
    vsPath.SetExt(CAppConstants::vs_ext);
    fw->Write((void*)(const char*)vsPath, ((std::string&)vsPath).size());
    fw->Write((void*)shader_delimiter, strlen(shader_delimiter));
    IO::CPath fsPath(fName);
    fsPath.SetExt(CAppConstants::fs_ext);
    fw->Write((void*)(const char*)fsPath, ((std::string&)fsPath).size());
    fw->Write((void*)shader_delimiter, strlen(shader_delimiter));
    fw->Close();
    IO::CPath shaderDir(shader_program_dir);
    vsPath = shaderDir + vsPath;
    if (IO::CFile::Exists(vsPath))
    {
        LWMessageFuncs *msgf;
        msgf = (LWMessageFuncs*)::meshExport->global( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );
        if (!msgf->yesNo("Create file", vsPath, "exists. Overwrite?"))
            return -2;
    }
    IO::CFileSystem::MkDir(shader_program_dir);
    std::auto_ptr<IO::CFileWriter> vsFw(IO::CFileWriter::Open(vsPath, FILE_REWRITE | FILE_TEXT));
    vsFw->Write((void*)def_vs, strlen(def_vs));
    vsFw->Close();

    fsPath = shaderDir + fsPath;
    if (IO::CFile::Exists(fsPath))
    {
        LWMessageFuncs *msgf;
        msgf = (LWMessageFuncs*)::meshExport->global( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );
        if (!msgf->yesNo("Create file", fsPath, "exists. Overwrite?"))
            return -3;
    }
    std::auto_ptr<IO::CFileWriter> fsFw(IO::CFileWriter::Open(fsPath, FILE_REWRITE | FILE_TEXT));
    fsFw->Write((void*)def_fs, strlen(def_fs));
    fsFw->Close();
    return 0;
}
// shader select button event handler
void ssb_event(LWControl *ctl,  void *userdata)
{
	CMeshExport * meshExport = (CMeshExport *)userdata;
	char surface_name[MAX_SURFACE_NAME];
	ZeroMemory(surface_name, MAX_SURFACE_NAME);
	int index, selection;
	GET_INT(controls.surface_list, index);
    
	strcpy(surface_name, "Surface: ");
	strcat(surface_name, meshExport->surff->name(meshExport->surfIDList[index]));
    std::string srfName(meshExport->surff->name(meshExport->surfIDList[index]));
   
	if (ShowShaderPanel(meshExport->global,  surface_name, shaders, &selection))
	{
        if (shaders[selection] == newShaderOpt)
        {
            std::string prgName;
            if (TryCreateShaderProgram(srfName, prgName))
            {
                LWMessageFuncs *msgf;
                msgf = (LWMessageFuncs*)::meshExport->global( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );
                msgf->error("Create Program", "An error has occured during creating shader program...");
                return;
            }
            meshExport->shaders_to_surfIDs[index] = prgName;
        }
        else
		    meshExport->shaders_to_surfIDs[index] = shaders[selection];
		CON_SETEVENT(controls.surface_list, snslb_event, meshExport);
        controls.surface_list->draw(controls.surface_list, DR_REFRESH);
	}
}
void AlignControl(LWControl * p)
{
    /* align */
	long w = CON_LW(p);
	ival.intv.value = 100 - w;
	p->set(p, CTL_X, &ival );
}

BOOL EnumShaders(void)
{
	WIN32_FIND_DATAA findData;
	char path[MAX_PATH];
	strcpy(path, shader_program_dir);
	strcat(path, "\\*.program");
	HANDLE handle = ::FindFirstFileA(path, &findData);
	if (handle == INVALID_HANDLE_VALUE)
		return FALSE;
	shaders.push_back(std::string(findData.cFileName));
	while (::FindNextFileA(handle, &findData))
	{
		shaders.push_back(std::string(findData.cFileName));
	}
     shaders.push_back(newShaderOpt);
	return TRUE;
}

int ShowPanel(CMeshExport* meshExport)
{
	int ret;
	LWPanelFuncs *panf;
	LWPanelID ID;
	ret = 0;
    ::meshExport = meshExport;
	if (panf = (LWPanelFuncs*)meshExport->global( LWPANELFUNCS_GLOBAL, GFUSE_TRANSIENT ))
	{
		ID = PAN_CREATE(panf,"Mesh Export");
		//panf->user_data = meshExport;
		// Init meshExport...
		strcpy(output_file_name, "f:\\prg2\\Projects\\MeshExport\\testMSVC2\\Objects\\out.mesh");
		strcpy(shader_program_dir, "f:\\prg2\\Projects\\MeshExport\\testMSVC2\\Shaders\\" /*"C:\\Data\\Shaders"*/);
		EnumShaders();
		convert_to_triangles = 1;
		meshExport->GetSurfIDList();
		// Create controls...

		// Save file dialog
		controls.save_file = SAVE_CTL(panf, ID, "Save file as", DEF_WIDTH);
		SET_STR( controls.save_file, output_file_name, sizeof(output_file_name));
		AlignControl(controls.save_file);
		// Shaders directory
		controls.program_dir = DIR_CTL(panf, ID, "Shaders directory", DEF_WIDTH);
		SET_STR( controls.program_dir, shader_program_dir, sizeof(shader_program_dir));
		CON_SETEVENT(controls.program_dir, prgdir_event, meshExport);
		AlignControl(controls.program_dir);
		// Separator 
		controls.border = BORDER_CTL(panf, ID, 0, DEF_BORDER_WIDTH, 0);
		// Triangle checkbox
		controls.convert_to_triangles = BOOL_CTL(panf, ID, "Convert polygons to triangles");
		SET_INT(controls.convert_to_triangles, convert_to_triangles);
		// Surfaces listbox
		//controls.surface_list = LISTBOX_CTL(panf, ID, "Surfaces", DEF_SURFACE_LB_WIDTH, DEF_SURFACE_LIST_HEIGHT, slb_name, slb_count);
        
		controls.surface_list = MULTILIST_CTL(panf, ID, "Surfaces", DEF_SURFACE_LB_WIDTH, DEF_SURFACE_LIST_HEIGHT, snslb_mname, slb_count, snslb_colwidth);
       // controls.surface_list->set(CTL_USERDATA, 
		CON_SETEVENT(controls.surface_list, snslb_event, meshExport);
		controls.shaderselect_button = BUTTON_CTL( panf, ID, "Select shader" );
		CON_SETEVENT(controls.shaderselect_button, ssb_event, meshExport);
		//controls.surface_list->set(ID, CTL_USERDATA, (LWValue *)meshExport);
		if ( ret = panf->open( ID, PANF_BLOCKING | PANF_CANCEL | PANF_FRAME))
		{
			GET_STR( controls.save_file, output_file_name, sizeof( output_file_name ));
			//GET_STR( controls.program_dir, meshExport->shader_program_dir, sizeof( meshExport->shader_program_dir ));
			GET_INT( controls.convert_to_triangles, convert_to_triangles);
		}
		PAN_KILL( panf, ID );
		shaders.clear();
		return ret;
	}
	return ret;
}
