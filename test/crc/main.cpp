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

using namespace std;

void gprint (std::string msg)
{
  std::cout << msg << std::endl;
}

std::string ginput ()
{
  std::string msg;
  std::cin >> msg;
  return msg;
}

inline void TestVector21 ()
{
  string str;
  do
  {
    str = ginput ();

    if (str[0] == 'C')
    {
      if (str[1] == 'B')
      {
        if (str[2] == '0')
        {

        }
      }
      else if (str[1] == 'E')
      {
      }
      else if (str[1] == 'T')
      {
      }
      else if (str[1] == 'R')
      {
      }
      else if (str[1] == 'C')
      {
        break;
      }
    }
    else
    {
    }
  }
  while (str != "CC");
}

#include <windows.h>

#include "test_check_sum1.cpp"

int main (int argc, char *agrv[])
{
  gprint ("hello begin:");

//    for (int i = 0; i < 10; ++i)
//    {
//        std::cout << "tick : " << clock() << std::endl;
//        ::Sleep(1000);
//    }

  test_check_sum11 (argc, agrv);

  gprint ("hello end!");

  return 0;
}
