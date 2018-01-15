
#include "global.h"

//#include "create_process1.cpp"
#include "Noexcept1.hpp"
#include "probability_birthday1.hpp"

int test(int argc, char *agrv[])
{
    gprint("hello begin:");

//    testMain(argc, agrv);

    while (1)
    {
        std::cout << "tick : " << clock() << std::endl;

        string str = ginput();

        if (str[0] == 'q' || str[0] == 'Q')
        {
            break;
        }

//        ::Sleep(100);
    }

    gprint("hello end!");

	return 1;
};

int main()
{
//	testNoexcept();

    testProbabiltyBirthday1();

    cout << ginput() << endl;

	return 1;
}