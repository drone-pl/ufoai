#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <dlfcn.h>

#include "../../common/common.h"
#include "../system.h"

void Sys_Init (void)
{
	sys_os = Cvar_GetOrCreate("sys_os", "macosx", CVAR_SERVERINFO, nullptr);
	sys_affinity = Cvar_GetOrCreate("sys_affinity", "0", CVAR_ARCHIVE, nullptr);
	sys_priority = Cvar_GetOrCreate("sys_priority", "0", CVAR_ARCHIVE, "Process nice level");
}
