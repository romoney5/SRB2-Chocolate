#include <lua.h>

#include "../i_system.h"
#include "../g_game.h"
#include "../netcode/d_netfil.h"
#include "../netcode/net_command.h"
#include "../lua_libs.h"
#include "../byteptr.h"
#include "../lua_script.h"
#include "../m_misc.h"
#include "../i_time.h"

#include "lauxlib.h"
#include "lualib.h"


#define MAXBYTESPERMINUTE (10 * 1024 * 1024) // Rate limit for writing files
#define MAXOPENSPERMINUTE 50 // Rate limit for opening new files

#define FMT_FILECALLBACKID "file_callback_%d"


// Allow scripters to write files of these types to SRB2's folder
static const char *whitelist[] = {
	".bmp",
	".cfg",
	".csv",
	".dat",
	".png",
	".sav2",
	".txt",
};


static INT64 numwrittenbytes = 0;
static INT64 numopenedfiles = 0;



/*
** When creating file handles, always creates a `closed' file handle
** before opening the actual file; so, if there is a memory error, the
** file is not left opened.
*/
static FILE **newfile(lua_State *L) {
	FILE **pf = (FILE **)lua_newuserdata(L, sizeof(FILE *));
	*pf = NULL;  /* file handle is currently `closed' */
	luaL_getmetatable(L, LUA_FILEHANDLE);
	lua_setmetatable(L, -2);
	return pf;
}

// Create directories in the path
void MakePathDirs(char *path)
{
	char *c;

	for (c = path; *c; c++)
		if (*c == '/' || *c == '\\')
		{
			char sep = *c;
			*c = '\0';
			I_mkdir(path, 0755);
			*c = sep;
		}
}


static int CheckFileName(lua_State *L, const char *filename)
{
	int length = strlen(filename);
	boolean pass = false;
	size_t i;

	if (strchr(filename, '\\'))
	{
		luaL_error(L, "access denied to %s: \\ is not allowed, use / instead", filename);
		return pushresult(L, 0, filename);
	}

	for (i = 0; i < (sizeof(whitelist) / sizeof(const char *)); i++)
		if (!stricmp(&filename[length - strlen(whitelist[i])], whitelist[i]))
		{
			pass = true;
			break;
		}
	if (strstr(filename, "./")
		|| strstr(filename, "..") || strchr(filename, ':')
		|| filename[0] == '/'
		|| !pass)
	{
		luaL_error(L, "access denied to %s", filename);
		return pushresult(L, 0, filename);
	}

	return 0;
}

static int io_open(lua_State *L) {
	const char *filename = luaL_checkstring(L, 1);
	const char *mode = luaL_optstring(L, 2, "r");
	int checkresult;

	checkresult = CheckFileName(L, filename);
	if (checkresult)
		return checkresult;

	luaL_checktype(L, 3, LUA_TFUNCTION);

	if (!(strchr(mode, 'r') || strchr(mode, '+')))
		luaL_error(L, "open() is only for reading, use openlocal() for writing");

	AddLuaFileTransfer(filename, mode);

	return 0;
}


static int io_openlocal(lua_State *L) {
	FILE **pf;
	const char *filename = luaL_checkstring(L, 1);
	const char *mode = luaL_optstring(L, 2, "r");
	char *realfilename;
	luafiletransfer_t *filetransfer;
	int checkresult;

	checkresult = CheckFileName(L, filename);
	if (checkresult)
		return checkresult;

	realfilename = va("%s" PATHSEP "%s", luafiledir, filename);

	if (client && strnicmp(filename, "client/", strlen("client/")))
		I_Error("Access denied to %s\n"
			"Clients can only access files stored in luafiles/client/\n",
			filename);

	// Prevent access if the file is being downloaded
	for (filetransfer = luafiletransfers; filetransfer; filetransfer = filetransfer->next)
		if (!stricmp(filetransfer->filename, filename))
			I_Error("Access denied to %s\n"
				"Files can't be opened while being downloaded\n",
				filename);

	// Reading not restricted
	if (client && (strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+')))
	{
		if (numopenedfiles >= (I_GetTime() / (60 * TICRATE) + 1) * MAXOPENSPERMINUTE)
			I_Error("Access denied to %s\n"
				"File opening rate exceeded\n",
				filename);

		numopenedfiles++;
	}

	MakePathDirs(realfilename);

	// Open and return the file
	pf = newfile(L);
	*pf = fopen(realfilename, mode);
	return (*pf == NULL) ? pushresult(L, 0, filename) : 1;
}


void Got_LuaFile(UINT8 **cp, INT32 playernum)
{
	FILE **pf = NULL;
	UINT8 success = READUINT8(*cp); // The first (and only) byte indicates whether the file could be opened

	if (playernum != serverplayer)
	{
		CONS_Alert(CONS_WARNING, M_GetText("Illegal luafile command received from %s\n"), player_names[playernum]);
		if (server)
			SendKick(playernum, KICK_MSG_CON_FAIL);
		return;
	}

	if (!luafiletransfers)
		I_Error("No Lua file transfer\n");

	lua_settop(gL, 0); // Just in case...
	lua_pushcfunction(gL, LUA_GetErrorMessage);

	// Retrieve the callback and push it on the stack
	lua_pushfstring(gL, FMT_FILECALLBACKID, luafiletransfers->id);
	lua_gettable(gL, LUA_REGISTRYINDEX);

	// Push the first argument (file handle or nil) on the stack
	if (success)
	{
		char mode[4];

		// Ensure we are opening in binary mode
		// (if it's a text file, newlines have been converted already)
		strcpy(mode, luafiletransfers->mode);
		if (!strchr(mode, 'b'))
			strcat(mode, "b");

		pf = newfile(gL); // Create and push the file handle
		*pf = fopen(luafiletransfers->realfilename, mode); // Open the file
		if (!*pf)
			I_Error("Can't open file \"%s\"\n", luafiletransfers->realfilename); // The file SHOULD exist
	}
	else
		lua_pushnil(gL);

	// Push the second argument (file name) on the stack
	lua_pushstring(gL, luafiletransfers->filename);

	// Call the callback
	LUA_Call(gL, 2, 0, 1);
	lua_settop(gL, 0);

	if (success)
	{
		// Close the file
		if (*pf)
		{
			fclose(*pf);
			*pf = NULL;
		}

		if (client)
			remove(luafiletransfers->realfilename);
	}

	RemoveLuaFileTransfer();

	if (waitingforluafilecommand)
	{
		waitingforluafilecommand = false;
		CL_PrepareDownloadLuaFile();
	}

	if (server && luafiletransfers)
		SV_PrepareSendLuaFile();
}


void StoreLuaFileCallback(INT32 id)
{
	lua_pushfstring(gL, FMT_FILECALLBACKID, id);
	lua_pushvalue(gL, 3); // Parameter 3 is the callback
	lua_settable(gL, LUA_REGISTRYINDEX); // registry[callbackid] = callback
}


void RemoveLuaFileCallback(INT32 id)
{
	lua_pushfstring(gL, FMT_FILECALLBACKID, id);
	lua_pushnil(gL);
	lua_settable(gL, LUA_REGISTRYINDEX); // registry[callbackid] = nil
}
