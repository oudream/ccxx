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

extern std::string ginput ();

int test_check_sum11(int argc, char *argv[])
{

  unsigned char nCrc = 0;
  nCrc += 97;
  nCrc += 98;
  nCrc += 99;

  cout << nCrc << endl;
  cout << "nCrc end." << endl;

  return 0;
}
