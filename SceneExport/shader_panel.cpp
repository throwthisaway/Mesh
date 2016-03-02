#include "shader_panel.h"
#include <lwpanel.h>

#define DEF_SHADER_LB_WIDTH 140
#define DEF_SHADER_LIST_HEIGHT 8

static struct st_controls
{
	LWControl *shader_list;
}controls;

// Required by macros...
static LWPanControlDesc desc;
static LWValue
  ival    = { LWT_INTEGER },
  ivecval = { LWT_VINT },
  fval    = { LWT_FLOAT },
  fvecval = { LWT_VFLOAT },
  sval    = { LWT_STRING };
// ...Required by macros

typedef struct st_ShaderUserdata
{
	int count;
	std::vector<std::string>& shaders;
}ShaderUserdata;

// shader select listbox name and count callback
int sslb_count( void * userdata )
{
	if (!userdata)
		return 0;
	ShaderUserdata * userData = (ShaderUserdata * )userdata;
	return userData->count;
}
char * sslb_name( void *userdata, int index )
{
	if (!userdata)
		return 0;
	ShaderUserdata * userData = (ShaderUserdata * )userdata;
	return (char *)userData->shaders[index].c_str();
}

void sslb_event(LWControl *ctl,  void *userdata)
{
	
}

int ShowShaderPanel(GlobalFunc * global, const char * title, std::vector<std::string>& shaders, int * selection)
{
	int ret;
	LWPanelFuncs *panf;
	LWPanelID ID;
	ret = 0;
	if (panf = (LWPanelFuncs*)global( LWPANELFUNCS_GLOBAL, GFUSE_TRANSIENT ))
	{
		ID = PAN_CREATE(panf, (char*)title);
		controls.shader_list = LISTBOX_CTL(panf, ID, "Shaders", DEF_SHADER_LB_WIDTH, DEF_SHADER_LIST_HEIGHT, sslb_name, sslb_count);
		ShaderUserdata userData = {shaders.size(), shaders};
		CON_SETEVENT(controls.shader_list, sslb_event, &userData);
		if ( ret = panf->open( ID, PANF_BLOCKING | PANF_CANCEL | PANF_FRAME))
		{			
			GET_INT(controls.shader_list, *selection);
		}
	}
	return ret;
	
}