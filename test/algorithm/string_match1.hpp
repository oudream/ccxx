
#include "global.h"

using namespace std;

//★用一种算法使通用字符串相匹配。
//ANSWER:
//    What is “Tongyongzifuchuan”... a string with “*” and “?”? If so, here is the code.
int match(char * str, char * ptn) {
    if (*ptn == '\0') return 1;
    if (*ptn == '*') {
        do {
            if (match(str++, ptn+1)) return 1;
        } while (*str != '\0');
        return 0;
    }
    if (*str == '\0') return 0;
    if (*str == *ptn || *ptn == '?') {
        return match(str+1, ptn+1);
    }
    return 0;
}