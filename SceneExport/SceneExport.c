#include <lwserver.h>
#include <lwgeneric.h>
#include "SaveScene.h"

XCALL_( int ) Activate( int version, GlobalFunc *global, LWLayoutGeneric *local, void *serverData)
{
	if ( version != LWLAYOUTGENERIC_VERSION )
		return AFUNC_BADVERSION;
	SaveScene(global);
	return AFUNC_OK;
}

ServerRecord ServerDesc[] = {
	{ LWLAYOUTGENERIC_CLASS, "Scene Export", /*(ActivateFunc(__cdecl*))*/Activate},
	{ NULL }
};