#include <lwmeshes.h>
#include <lwserver.h>
#include <lwtxtr.h>
#include <windows.h>
#include "mesh_panel.h"
#include "MeshExport.h"

extern "C" void SaveScene(GlobalFunc *global)
{

	LWObjectFuncs * objFunc = (LWObjectFuncs *)global(LWOBJECTFUNCS_GLOBAL, GFUSE_TRANSIENT);
	for (int object = 0; object<objFunc->numObjects(); object++)
	{
		CMeshExport meshExport(global);
		meshExport.Save(objFunc, object);
	}
	
}