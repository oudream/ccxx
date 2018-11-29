
#include <ccxx/ccxx.h>


///////////////////////////////////////////////////////////////////////////////
// Test data and code 
//

struct A
{
	int a;

	A()
		: a( 0 ) {}

	void Print()
	{
		printf( "a: %d\n", a );
	}
};

struct B
{
	A* pA;

	B()
		: pA( 0 ) {}

	void Print()
	{
		printf( "pA: %x\n", pA );
		pA->Print();
	}

};

void DoWork()
{
	B* pB = new B(); // but forget to initialize B::pA 

	pB->Print(); // here it should crash 
}


void fn_timer_timeout(int iInterval)
{
	cxPrompt() << CxTime::currentSystemTimeString();
	DoWork();
}

int main(int argc, const char* argv[])
{
	CxApplication::init(argc, argv);

	CxTimerManager::startTimer(fn_timer_timeout, 2000);
	
	return CxApplication::exec();
}


