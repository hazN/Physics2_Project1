#ifndef _cLuaBrain_HG_
#define _cLuaBrain_HG_

extern "C" {
#include <Lua5.4.4/lua.h>
#include <Lua5.4.4/lualib.h>
#include <Lua5.4.4/lauxlib.h>
}


#include <string>
#include <vector>
//#include "cGameObject.h"
#include <map>
#include "cMeshObject.h"
#include "cCommandScheduler.h"
#include "OrientTo.h";
#include "cCommand.h"
#include "globalThings.h"
#include "MoveTo.h"
class cLuaBrain
{
public:
	// Init Lua and set callback functions
	cLuaBrain();
	~cLuaBrain();
	void LoadScript(std::string scriptName,
		std::string scriptSource);
	void DeleteScript(std::string scriptName);
	// Passes a pointer to the game object vector
	void SetObjectVector(std::vector< cMeshObject* >* p_vecGOs);
	void SetCommandGroup(cCommandGroup* command_group);
	// Call all the active scripts that are loaded
	void Update(float deltaTime);

	// Runs a script, but doesn't save it (originally used to set the ObjectID)
	void RunScriptImmediately(std::string script);
	// Called by Lua
	// Passes object ID, new velocity, etc.
	// Returns valid (true or false)
	// Passes: 
	// - position (xyz)
	// - velocity (xyz)
	// called "setObjectState" in lua
	// Something like this: "setObjectState( 24, 0, 0, 0, 0, 0, 0 )"
	static int Lua_UpdateObject(lua_State* L);
	// Passes object ID
	// Returns valid (true or false)
	// - position (xyz)
	// - velocity (xyz)
	// called "getObjectState" in lua
	static int Lua_GetObjectState(lua_State* L);
	//	"lauchDropShip()"
	//	static int Lua_DropShipLaunch(lua_State* L);

		// Update Object's name
		// Lua script name is "updateObjectName"
		// Example: updateObjectName( ID, newName );
	//static int Lua_UpdateObjectName(lua_State* L);

	static int MoveTo(lua_State* L);
	static int OrientTo(lua_State* L);
	static int FollowCurve(lua_State* L);
	static int Expand(lua_State* L);
	static int Invisible(lua_State* L);
	static int Follow(lua_State* L);

private:
	std::map< std::string /*scriptName*/,
		std::string /*scriptSource*/ > m_mapScripts;

	// Note that this is a POINTER to the vector of game object pointers
	static std::vector< cMeshObject* >* m_p_vecGOs;
	// returns nullptr if not found
	static cMeshObject* m_findObjectByID(int id);
	static cMeshObject* m_findObjectByFriendlyName(std::string name);

	lua_State* m_pLuaState;

	std::string m_decodeLuaErrorToString(int error);

	static cCommandGroup* command_group;
};

#endif
