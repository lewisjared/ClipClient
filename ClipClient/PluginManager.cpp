#include "StdAfx.h"
#include "PluginManager.h"
#include "boost/filesystem.hpp"

using namespace boost::filesystem;


PluginManager::PluginManager(void)
{
}


PluginManager::~PluginManager(void)
{
	closeAll();
}

PluginManager& PluginManager::getInstance()
{
	static PluginManager instance;
	return instance;
}

int PluginManager::closeAll()
{
	int result = 0;
	for (auto it= m_exitFuncs.begin(); it != m_exitFuncs.end(); it++)
	{
		try
		{
			result = (*it)();
		}
		catch (...)
		{
			result= -1;
		}
	}
	m_exitFuncs.clear();
	m_dynLibraryMap.clear();
	m_objectTypes.clear();

	return  result;
}


void* PluginManager::createObject(const std::string& objectType)
{
	if (m_objectTypes.find(objectType) != m_objectTypes.end())
	{
		void* obj = m_objectTypes[objectType].create();

		return obj;
	}

	return NULL;
}

bool PluginManager::loadAll(const std::string& pluginDir)
{
	path p(pluginDir);

	if (!is_directory(p))
	{
		std::cout << pluginDir <<" is not a directory." << std::endl;
		return false;
	}
	std::wstring error;

	std::vector<path> filenames;
	copy(directory_iterator(p), directory_iterator(), back_inserter(filenames));

	RegisterModule_t rm;
	rm.registerObject = registerObject;
	rm.version.major = 1;
	rm.version.minor = 0;

	for (auto it = filenames.begin(); it != filenames.end(); it++)
	{
		if (it->extension() == ".dll")
		{
			DynamicLibrary* lib = DynamicLibrary::load(it->wstring(), error);
			//Disregard bad non libraries
			if (lib != NULL) 
			{
				//get the entry point
				InitFunc initFunc = (InitFunc) (lib->getSymbol("initPlugin"));
				if (initFunc == NULL)
					break;
				ExitFunc exitFunc = initFunc(&rm);
				if (exitFunc == NULL)
					break;

				//Loaded plugin is good so store the exitFunc and the library
				m_exitFuncs.push_back(exitFunc);
				m_dynLibraryMap[it->string()] = std::shared_ptr<DynamicLibrary> (lib);
			}
		}
	}

	return true;
}

bool PluginManager::registerObject(const char* name, const ObjInterface_t* params)
{
	assert(params->create);
	assert(params->destroy);

	PluginManager& pm = PluginManager::getInstance();

	//Check that the object of that type is not registered yet
	if (pm.m_objectTypes.find(name) != pm.m_objectTypes.end())
		return false;
	pm.m_objectTypes[name] = *params;

	return true;
}