#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "Plugin/DynamicLibrary.h"
#include "Plugin/Plugin.h"


/**
 \class	PluginManager

 \brief	Manages the plugin system.

 */

class PluginManager
{
	typedef std::map<std::string, std::shared_ptr<DynamicLibrary> > DynLibraryMap;
public:
	typedef std::map<std::string,ObjInterface_t> ObjMap; 
	~PluginManager(void);

	static PluginManager& getInstance();

	/**
	 \fn	bool PluginManager::closeAll();
	
	 \brief	Closes all the loaded dynamic libraries. 
	 The user must not call any plugin loaded functions once this has been called.
	
	 \return	true if it succeeds, false if it fails.
	 */

	int closeAll();

	/**
	 \fn	void* PluginManager::createObject(const std::string& objectType);
	
	 \brief	Creates an object a given type.

	 The user must destroy the object.
	
	 \param	objectType	Type of the object.
	
	 \return	null if it fails, else the new object. 
	 */

	void* createObject(const std::string& objectType);

	/**
	 \fn	bool PluginManager::loadAll(const std::string& pluginDir);
	
	 \brief	Loads all the dynamic libraries in a given directory.
	
	 \param	pluginDir	The plugin dir.
	
	 \return	true if it succeeds, false if it fails.
	 */

	bool loadAll(const std::string& pluginDir);

	/**
	 \fn	const ObjMap& PluginManager::getObjectTypes();
	
	 \brief	Gets a map of the object types with their names as keys.
	
	 \return	The object types.
	 */

	const ObjMap& getObjectTypes()
	{
		return m_objectTypes;
	}

	/**
	 \fn	static bool PluginManager::registerObject(const ObjInterface_t* params);
	
	 \brief	Registers the object described by params.
	
	 \param	params	Options for controlling the operation.
	
	 \return	true if it succeeds, false if it fails.
	 */

	static bool registerObject(const char* name, const ObjInterface_t* params);
private:
	PluginManager();

	ObjMap m_objectTypes;
	DynLibraryMap m_dynLibraryMap;
	std::vector<ExitFunc> m_exitFuncs;

};