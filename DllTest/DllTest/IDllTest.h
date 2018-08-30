#ifndef _IDLL_TEST_H_
#define _IDLL_TEST_H_

class IDllTest
{
public:
	virtual void print()=0;
	//MessageBoxA("I am IDllTest Header!!!");
};

extern "C" __declspec(dllexport) IDllTest *CreateDllTest();
#endif
