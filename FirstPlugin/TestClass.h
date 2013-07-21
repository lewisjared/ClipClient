#pragma once

class ITemp
{
public:
	virtual int getVal() = 0;
};
class TestClass : public ITemp
{
public:
	static void* create();
	static bool destroy(void* obj);

	TestClass(void);
	~TestClass(void);

	int getVal();	
};

