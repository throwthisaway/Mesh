#include <lwserver.h>
#include <lwmeshedt.h>
#include <lwcmdseq.h>
#include <stdio.h>
#include "SaveMesh.h"

XCALL_( int ) Activate( int version, GlobalFunc *global, LWModCommand *local, void *serverData)
{
	if ( version != LWMODCOMMAND_VERSION )
		return AFUNC_BADVERSION;
		//sprintf( cmd, "LOAD Objects\\sidedness.lwo");
		// Load an UV mapped box
		//sprintf( cmd, "LOAD Objects\\box-wcheckerboard-2x2_.lwo");
		//sprintf( cmd, "LOAD D:\\NewTek\\Objects\\ball.lwo");
		//sprintf( cmd, "LOAD D:\\NewTek\\Objects\\64thunde.lwo");
		//sprintf( cmd, "LOAD Objects\\SurfaceSortTest.lwo");
		//local->evaluate( local->data, cmd );
	SaveMesh(global, local, OPLYR_PRIMARY, EDCOUNT_ALL);
	return AFUNC_OK;
}

ServerRecord ServerDesc[] = {
	{ LWMODCOMMAND_CLASS, "Mesh Export", /*(ActivateFunc(__cdecl*))*/Activate},
	{ NULL }
};