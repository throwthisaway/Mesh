#ifndef _PANEL_H
#define _PANEL_H
#include <w32types.h>
#include <lwserver.h>
#include <lwsurf.h>
#include <lwcmdseq.h>
#include <lwmeshedt.h>
#include "MeshSurface.h"
#include <vector>
#include <string>

typedef struct st_MeshExport
{
	LWSurfaceFuncs *surff;
	GlobalFunc * global;
	const char * name;
	char output_file_name[MAX_PATH];
	char shader_program_dir[MAX_PATH];
	int shader_count;
	std::vector<std::string> shaders;
	LWSurfaceID *surfIDList;
	std::string *shaders_to_surfIDs;
	BOOL convert_to_triangles;
	Surfaces surf;
}MeshExport;
#ifdef __cplusplus
extern "C" int ShowPanel(GlobalFunc *, MeshExport *);
#else
int ShowPanel(GlobalFunc *, MeshExport *);
#endif
int GetSurfIDList(MeshExport *);
void CleanupMeshExport(void);
#endif