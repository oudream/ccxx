
#include "global.h"

void testProbabiltyBirthday1()
{
    double dProbabiltyBir[23];
    for (int i = 0; i < sizeof(dProbabiltyBir)/ sizeof(double); ++i)
    {
        double d1 = 365-i;
        double d2 = 365;
        dProbabiltyBir[i] = d1 / d2;
    }
    double dProbabilty = 1;
    for (int i = 0; i < sizeof(dProbabiltyBir)/ sizeof(double); ++i)
    {
        dProbabilty *= dProbabiltyBir[i];
    }
    std::cout << dProbabilty << std::endl;
}