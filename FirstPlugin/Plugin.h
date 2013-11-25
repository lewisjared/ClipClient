/**
 \file	plugin\Plugin.h

 \brief Declares the interface for loading plugins.
 A plugin module must define the InitPlugin function.
 */

#pragma once

#include <string>

struct ObjInterface_t;

struct Version_t
{
	unsigned int major;
	unsigned int minor;
};

typedef bool (*RegisterObject)(const char*, const ObjInterface_t*);

struct RegisterModule_t
{
	RegisterObject registerObject;
	Version_t version;
};

/**
 \typedef	void* (*CreateFunc)(void)

 \brief	Defines the Create Object function.
 */

typedef void* (*CreateFunc)(void);

/**
 \typedef	bool (*DestroyFunc)(void *)

 \brief	Defines the Destroy Object function.
 */
typedef bool (*DestroyFunc)(void *);

/**
 \typedef	int (*ExitFunc)()

 \brief	Defines the ExitFunction for unloading the plugin module.
 */

typedef int (*ExitFunc)();

/**
 \typedef	ExitFunc(*InitFunc)()

 \brief	Defines the InitFunction for loading the plugin module.
 */
typedef ExitFunc(*InitFunc)(const RegisterModule_t*);

/**
 \struct	ObjInterface_t

 \brief	Contains the information for a Object module.
 */
struct ObjInterface_t{
	CreateFunc create;
	DestroyFunc destroy;
};

#ifndef EXPORT_FUNC
	#ifdef WIN32
		#define EXPORT_FUNC  extern "C" __declspec(dllexport) 
	#else
		#define EXPORT_FUNC extern "C"
	#endif // WIN32
#endif