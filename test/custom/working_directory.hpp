#include <typeinfo>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <stddef.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

// VC++ 6.0
#include <stdio.h>
#include <stdlib.h>

using namespace std;

extern std::string ginput();

int test_wd1(int argc, char *argv[])
{
    cout << "test wd1 begin" << endl;

    string str;
    do
    {
        str = ginput();

        if (str[0] == 't')
        {
            if (str[1] == '1')
            {
                char path[_MAX_PATH];
                _getcwd(path, _MAX_PATH);
                printf("current working directory:\n%s\n", path);
            }
            else if (str[1] == '2')
            {
                char path[_MAX_PATH];
                if ((_chdir("c:/bin")) == 0)
                {
                    printf("modify working directory\n");
                    _getcwd(path, _MAX_PATH);
                    printf("current working directory:\n%s\n", path);
                }
                else
                {
                    perror("modify working directory fail");
                    exit(1);
                }
            }
            else if (str[1] == '3')
            {
                printf("current working directory:\n%s\n", argv[0]);
            }
        }
        else
        {
        }
    }
    while (str != "cc");

    cout << "test wd1 end." << endl;

    return 0;
}
#include <fcntl.h>
#include <dirent.h>
#include <dlfcn.h>

int main1(int argc, char *argv[])
{
    char path[_MAX_PATH];
    getcwd(path, _MAX_PATH);
    printf("current working directory:\n%s\n", path);
    if ((_chdir("d:\\visual c++")) == 0)
    {
        printf("modify working directory\n");
        _getcwd(path, _MAX_PATH);
        printf("current working directory:\n%s\n", path);
    }
    else
    {
        perror("modify working directory fail");
        exit(1);
    }
    return 0;
}

/*
#include <direct.h>

// vs2008
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

int main2(void)
{
  char *buffer;
  // 得到当前的工作路径
  if ((buffer = _getcwd(NULL, 0)) == NULL)
  {
    perror("_getcwd error");
  }
  else
  {
    printf("%s \nLength: %d\n", buffer, strnlen(buffer,999999));
    free(buffer);
  }
}
*/