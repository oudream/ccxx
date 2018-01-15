#include "test_timer1.cpp"
#include "test_timer2.cpp"


using namespace std;


long   __stdcall   callback(_EXCEPTION_POINTERS*   excp)
  {
  MessageBox(0,"Error","error",MB_OK);
  printf("Error   address   %x\n",excp->ExceptionRecord->ExceptionAddress);
  printf("CPU   register:\n");
  printf("eax   %x   ebx   %x   ecx   %x   edx   %x\n",excp->ContextRecord->Eax,
  excp->ContextRecord->Ebx,excp->ContextRecord->Ecx,
  excp->ContextRecord->Edx);
  return   EXCEPTION_EXECUTE_HANDLER;
  }

int main(int argc,const char * argv[])
{
    ::SetUnhandledExceptionFilter(callback);

    std::cout << "begin\n";

    CxApplication::init(argc, argv);

    tm dt3 = CxTime::localTm();

    std::cout << CxTime::toString(dt3.tm_hour) << endl;

    msepoch_t dt1 = CxTime::currentDayEnd();

    msepoch_t dt2 = CxTime::currentSystemTime();

    std::cout << CxTime::toString(dt1) << endl;

    std::cout << CxTime::toString(dt2) << endl;

    std::cout << CxTime::toString(dt1 + 1000) << endl;

    TimerPrintFunction::start();
    TimerPrintObject to1;
//    TimerPrintObject to2;
//    TimerPrintObject to3;

    std::cout << "end\n";

    int iResult = 0;

//    try
//    {
//        iResult = CxApplication::exec();
//    }
//    catch (...)
//    {
//        cout << "CxApplication::exec() except" << endl;
//    }

    iResult = CxApplication::exec();

//    std::cin >> iResult;

    return iResult;
}

