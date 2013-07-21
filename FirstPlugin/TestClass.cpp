#include "TestClass.h"

void* TestClass::create()
{
	return new TestClass();
}

bool TestClass::destroy(void* obj)
{
	if (obj == 0)
		return false;
	delete (TestClass*)obj;
	return true;
}


TestClass::TestClass(void)
{
}


TestClass::~TestClass(void)
{
}

int TestClass::getVal()
{
	return 5;
}
