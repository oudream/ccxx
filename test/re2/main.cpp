#include <typeinfo>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <iso646.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>


using namespace std;

int f_argc = 0;
char * * f_argv = NULL;

#include <re2/re2.h>
#include <util/util.h>
#include <util/flags.h>


#define TEST(x, y) \
    void x##y(void); \
    TestRegisterer r##x##y(x##y, # x "." # y); \
    void x##y(void)

void RegisterTest(void (*)(void), const char*);

class TestRegisterer {
public:
    TestRegisterer(void (*fn)(void), const char *s) {
        RegisterTest(fn, s);
    }
};

// TODO(rsc): Do a better job.
#define EXPECT_EQ CHECK_EQ
#define EXPECT_TRUE CHECK
#define EXPECT_LT CHECK_LT
#define EXPECT_GT CHECK_GT
#define EXPECT_LE CHECK_LE
#define EXPECT_GE CHECK_GE
#define EXPECT_FALSE(x) CHECK(!(x))

const bool UsingMallocCounter = false;
namespace testing {
class MallocCounter {
public:
    MallocCounter(int x) { }
    static const int THIS_THREAD_ONLY = 0;
    long long HeapGrowth() { return 0; }
    long long PeakHeapGrowth() { return 0; }
    void Reset() { }
};
}  // namespace testing

namespace re2 {
int64 VirtualProcessSize();
} // namespace re2









DEFINE_string(test_tmpdir, "/var/tmp", "temp directory");

struct Test {
    void (*fn)(void);
    const char *name;
};

static Test tests[10000];
static int ntests;

void RegisterTest(void (*fn)(void), const char *name) {
    tests[ntests].fn = fn;
    tests[ntests++].name = name;
}

namespace re2 {
int64 VirtualProcessSize() {
#ifdef WIN32
    return 0;
#else
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return (int64)ru.ru_maxrss*1024;
#endif
}
}  // namespace re2

int main(int argc, char **argv)
{
    f_argc = argc;
    f_argv = argv;

    cout << "begin test xxx : " << endl;








    for (int i = 0; i < ntests; i++) {
        printf("%s\n", tests[i].name);
        tests[i].fn();
    }
    printf("PASS\n");






    std::string sLine;
    std::getline (std::cin, sLine);

    cout << "this size :" << sLine.size() << endl;

    cout << "end test xxx !!!" << endl;

    return 0;
}

