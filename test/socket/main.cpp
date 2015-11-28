
#include <ccxx/ccxx.h>

using namespace std;

int main(int argc, char *argv[])
{
    vector<string> sIps = CxNetwork::getLocalIps();
    for (size_t i = 0; i < sIps.size(); ++i)
    {
        cout << sIps[i] << endl;
    }


    return 0;
}
