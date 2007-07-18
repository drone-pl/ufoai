/**
 * @file qcommon.h
 * @brief definitions common between client and server, but not game lib
 */

/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef QCOMMON_DEFINED
#define QCOMMON_DEFINED

#include "../game/q_shared.h"
#include "../game/inv_shared.h"
#include "../game/game.h"

#define UFO_VERSION "2.2-dev"

#define	BASEDIRNAME	"base"

#ifdef _WIN32
#  ifdef DEBUG
#    define BUILDSTRING "Win32 DEBUG"
#  else
#    define BUILDSTRING "Win32 RELEASE"
#  endif
#  ifndef SHARED_EXT
#    define SHARED_EXT "dll"
#  endif
#  if defined _M_IX86 || defined __i386__
#    define CPUSTRING "x86"
#  elif defined _M_AMD64 || define _M_X64 || defined __x86_x64__
#    define CPUSTRING "x86_64"
#  elif defined _M_IA64 || defined __ia64__
#    define CPUSTRING "x86_64"
#  elif defined _M_ALPHA || defined __alpha__
#    define CPUSTRING "AXP"
#  else
#    define CPUSTRING "Unknown"
#  endif

#elif defined __linux__
#  ifdef DEBUG
#    define BUILDSTRING "Linux DEBUG"
#  else
#    define BUILDSTRING "Linux RELEASE"
#  endif
#  ifndef SHARED_EXT
#    define SHARED_EXT "so"
#  endif
#  if defined  __i386__
#    define CPUSTRING "i386"
#  elif defined __x86_x64__
#    define CPUSTRING "x86_64"
#  elif defined __alpha__
#    define CPUSTRING "axp"
#  else
#    define CPUSTRING "Unknown"
#  endif

#elif defined __FreeBSD__ || defined __NetBSD__
#  define BUILDSTRING "FreeBSD"
#  ifndef SHARED_EXT
#    define SHARED_EXT "so"
#  endif
#  if defined  __i386__
#    define CPUSTRING "i386"
#  elif defined __x86_x64__
#    define CPUSTRING "x86_64"
#  elif defined __alpha__
#    define CPUSTRING "axp"
#  else
#    define CPUSTRING "Unknown"
#  endif

#elif defined __sun
#  define BUILDSTRING "Solaris"
#  ifndef SHARED_EXT
#    define SHARED_EXT "so"
#  endif
#  if defined __i386__
#    define CPUSTRING "i386"
#  else
#    define CPUSTRING "sparc"
#  endif

#elif defined (__APPLE__) || defined (MACOSX)
#  define BUILDSTRING "MacOSX"
#  ifndef SHARED_EXT
#    define SHARED_EXT "dylib"
#  endif
#  if defined __i386__
#    define CPUSTRING "i386"
#  elif defined __powerpc__
#    define CPUSTRING "ppc"
#  else
#    define CPUSTRING "Unknown"
#  endif

#else
#  define BUILDSTRING "NON-WIN32"
#  define CPUSTRING	"NON-WIN32"
#endif

#ifndef DEFAULT_BASEDIR
# define DEFAULT_BASEDIR BASEDIRNAME
#endif							/* DEFAULT_BASEDIR */
#ifndef DEFAULT_LIBDIR
# define DEFAULT_LIBDIR DEFAULT_BASEDIR
#endif							/* DEFAULT_LIBDIR */

#include "common.h"

void Info_Print(char *s);


/*
==============================================================
IRC
==============================================================
*/
void Irc_Init(void);
void Irc_Shutdown(void);

/* client side */
void Irc_Logic_Frame(int now, void *data);
void Irc_Input_KeyEvent(int key);
void Irc_Input_Deactivate(void);
void Irc_Input_Activate(void);

/*
==============================================================
PROTOCOL
==============================================================
*/

/* protocol.h -- communications protocols */

#define	PROTOCOL_VERSION	3

#define	PORT_CLIENT	27901
#define	PORT_SERVER	27910

/**
 * @brief server to client
 *
 * the svc_strings[] array in cl_parse.c should mirror this
 */
enum svc_ops_e {
	svc_bad,

	/** these ops are known to the game dll */
	svc_inventory,

	/** the rest are private to the client and server */
	svc_nop,
	svc_disconnect,
	svc_reconnect,
	svc_breaksound,					/**< <see code> */
	svc_print,					/**< [byte] id [string] null terminated string */
	svc_stufftext,				/**< [string] stuffed into client's console buffer, should be \n terminated */
	svc_serverdata,				/**< [long] protocol ... */
	svc_configstring,			/**< [short] [string] */
	svc_spawnbaseline,
	svc_centerprint,			/**< [string] to put in center of the screen */
	svc_playerinfo,				/**< variable */
	svc_event,					/**< ... */
        svc_oob = 0xff
};

/*============================================== */

/**
 * @brief client to server
 */
enum clc_ops_e {
	clc_bad,
	clc_nop,
	clc_endround,
	clc_teaminfo,
	clc_action,
	clc_userinfo,				/**< [[userinfo string] */
	clc_stringcmd,				/**< [string] message */
        clc_oob = 0xff
};


/*============================================== */

/* a sound without an ent or pos will be a local only sound */
#define	SND_VOLUME		(1<<0)	/* a byte */
#define	SND_ATTENUATION	(1<<1)	/* a byte */
#define	SND_POS			(1<<2)	/* three coordinates */
#define	SND_ENT			(1<<3)	/* a short 0-2: channel, 3-12: entity */
#define	SND_OFFSET		(1<<4)	/* a byte, msec offset from frame start */

#define DEFAULT_SOUND_PACKET_VOLUME	1.0
#define DEFAULT_SOUND_PACKET_ATTENUATION 1.0

/*============================================== */

#include "cmd.h"

#include "cvar.h"

#include "cmodel.h"

#include "filesys.h"

#include "scripts.h"

#include "net.h"

#include "netpack.h"

typedef enum actor_sounds_s {
	SOUND_DEATH,
	SOUND_HURT,

	MAX_SOUND_TYPES
} actor_sounds_t;

/*
==============================================================
MISC
==============================================================
*/


#define	ERR_FATAL	0			/* exit the entire game with a popup window */
#define	ERR_DROP	1			/* print to console and disconnect from game */
#define	ERR_QUIT	2			/* not an error, just a normal exit */

#define	EXEC_NOW	0			/* don't return until completed */
#define	EXEC_INSERT	1			/* insert at current position, but don't run yet */
#define	EXEC_APPEND	2			/* add to end of the command buffer */

#define	PRINT_ALL		0
#define PRINT_DEVELOPER	1		/* only print when "developer 1" */

void Com_BeginRedirect(int target, char *buffer, int buffersize, void (*flush) (int, char *));
void Com_EndRedirect(void);
void Com_Printf(const char *msg, ...) __attribute__((format(printf, 1, 2)));
void Com_DPrintf(const char *msg, ...) __attribute__((format(printf, 1, 2)));
void Com_Error(int code, const char *fmt, ...) __attribute__((noreturn, format(printf, 2, 3)));
void Com_Drop(void);
void Com_Quit(void);

int Com_ServerState(void);		/* this should have just been a cvar... */
void Com_SetServerState(int state);

#include "md4.h"
char *Com_MD5File(const char *fn, int length);

extern cvar_t *developer;
extern cvar_t *dedicated;
extern cvar_t *host_speeds;
extern cvar_t *sv_maxclients;
extern cvar_t *sv_reaction_leftover;
extern cvar_t *sv_shot_origin;
extern cvar_t *cl_maxfps;
extern cvar_t *teamnum;
extern cvar_t *gametype;

/* host_speeds times */
extern int time_before_game;
extern int time_after_game;
extern int time_before_ref;
extern int time_after_ref;

extern csi_t csi;

extern char map_entitystring[MAX_MAP_ENTSTRING];


#define MAX_CVARLISTINGAMETYPE 16
typedef struct cvarlist_s {
	char name[MAX_VAR];
	char value[MAX_VAR];
} cvarlist_t;

typedef struct gametype_s {
	char id[MAX_VAR];	/**< script id */
	char name[MAX_VAR];	/**< translated menu name */
	struct cvarlist_s cvars[MAX_CVARLISTINGAMETYPE];
	int num_cvars;
} gametype_t;

/** game types */
#define MAX_GAMETYPES 16
extern gametype_t gts[MAX_GAMETYPES];
extern int numGTs;


#include "mem.h"

qboolean Qcommon_LocaleSet(void);
void Qcommon_Init(int argc, const char **argv);
void Qcommon_Frame(void);
void Qcommon_Shutdown(void);
qboolean Qcommon_ServerActive(void);
void Com_SetGameType(void);

#define NUMVERTEXNORMALS	162
extern const vec3_t bytedirs[NUMVERTEXNORMALS];

/** this is in the client code, but can be used for debugging from server */
void SCR_DebugGraph(float value, int color);

/* Event timing */

typedef void event_func(int now, void *data);
void Schedule_Event(int when, event_func *func, void *data);
void Schedule_Timer(cvar_t *interval, event_func *func, void *data);

/*
==============================================================
NON-PORTABLE SYSTEM SERVICES
==============================================================
*/

void Sys_Init(void);
void Sys_NormPath(char *path);
void Sys_OSPath(char* path);
void Sys_Sleep(int milliseconds);
void Sys_AppActivate(void);
const char *Sys_GetCurrentUser(void);

void Sys_UnloadGame(void);
/** loads the game dll and calls the api init function */
game_export_t *Sys_GetGameAPI(game_import_t * parms);

char *Sys_ConsoleInput(void);
void Sys_ConsoleOutput(const char *string);
void Sys_SendKeyEvents(void);
void Sys_Error(const char *error, ...) __attribute__((noreturn, format(printf, 1, 2)));
void Sys_Quit(void);
char *Sys_GetClipboardData(void);
char *Sys_GetHomeDirectory(void);

void Sys_ConsoleInputShutdown(void);
void Sys_ConsoleInputInit(void);

void *Sys_LoadLibrary(const char *name, int flags);
void Sys_FreeLibrary(void *libHandle);
void *Sys_GetProcAddress(void *libHandle, const char *procName);

void Sys_Minimize(void);
void Sys_DisableTray(void);
void Sys_EnableTray(void);

/*
==============================================================
CLIENT / SERVER SYSTEMS
==============================================================
*/

void CL_Init(void);
void CL_Drop(void);
void CL_Shutdown(void);
void CL_Frame(int now, void *);
void CL_SlowFrame(int now, void *);
void CL_ParseClientData(const char *type, const char *name, const char **text);
void CIN_RunCinematic(int now, void *data);
void Con_Print(const char *text);
void SCR_BeginLoadingPlaque(void);
void MN_PrecacheMenus(void);
void CL_InitAfter(void);

void SV_Init(void);
void SV_Clear(void);
void SV_Shutdown(const char *finalmsg, qboolean reconnect);
void SV_ShutdownWhenEmpty(void);
void SV_Frame(int now, void *);
qboolean SV_RenderTrace(vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end);

#endif							/* QCOMMON_DEFINED */
