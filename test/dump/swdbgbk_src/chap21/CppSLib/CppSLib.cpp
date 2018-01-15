// CppSLib.cpp : sample code used to explore support libraries.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef vector<int> INTVECTOR;

int main(int argc, char* argv[])
{
    INTVECTOR iv;
	string s="Explore C++ Standard Library";
	int i;

    for (i = 0; i < s.length(); i++)
        iv.push_back(s.at(i));

    INTVECTOR::iterator iter;
    // Output contents of iv.
    cout << "[ " ;
    for (iter = iv.begin(); iter != iv.end();
         iter++)
    {
        cout << (char)*iter;
        if (iter != iv.end()-1)
			cout << "-";
    }
    cout << " ]" << endl ;
	cin>>i;
	return i;
}

