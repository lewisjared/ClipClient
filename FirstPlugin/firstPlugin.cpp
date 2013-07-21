#include "plugin/Plugin.h"
#include "TestClass.h"


EXPORT_FUNC int exitFunc()
{
	return 0;
}

EXPORT_FUNC ExitFunc initPlugin(const RegisterModule_t* rm)
{
	//Check version
	
	ObjInterface_t iface;
	iface.create = TestClass::create;
	iface.destroy = TestClass::destroy;

	rm->registerObject("TestClass", &iface);

	return exitFunc;
}