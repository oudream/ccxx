#include <ccxx/cxcontainer.h>
#include <ccxx/cxresource.h>


using namespace std;

void testStdRemove01()
{
    vector<int> ints1;
    vector<int> ints2;
    vector<int> ints3;

    clock_t t1 = clock();

    for (size_t i = 0; i < 1000 * 100; ++i)
    {
        ints1.push_back(i);
        ints2.push_back(i);
        if (i % 9 == 0)
            ints3.push_back(i);
    }

    clock_t t2 = clock();

    vector<int>::iterator it1 = ints1.end();
    for (size_t i = 0; i < ints3.size(); ++i)
    {
        it1 = std::remove(ints1.begin(), it1, ints3.at(i));
    }
    if (it1 != ints1.end())
        ints1.erase(it1, ints1.end());

    clock_t t3 = clock();

//    std::vector<int> haystack;

//    // set some values:        haystack: 10 20 30 40 50 60 70 80 90
//    for (int i=0; i<10; i++) haystack.push_back(i*10);

//    // using default comparison:
//    int needle1[] = {40,50,60,70};
//    std::vector<int>::iterator it;
//    it = std::search (haystack.begin(), haystack.end(), needle1, needle1+4);


    for(vector<int>::iterator it = ints2.begin(); it != ints2.end();)
    {
        if (std::find(ints3.begin(), ints3.end(), * it) != ints3.end())
        {
            ints2.erase(it);
            continue;
        }
        ++it;
    }

    clock_t t4 = clock();

    cout << "t2-t1=" << t2-t1 << " t3-t2=" << t3-t2 << " t4-t3=" << t4-t3 << endl;
}

void testStdRemove02()
{
    vector<int> ints1;
    vector<int> ints2;
    vector<int> ints3;

    clock_t t1 = clock();

    for (size_t i = 0; i < 1000 * 30; ++i)
    {
        ints1.push_back(i);
        ints2.push_back(i);
        if (i % 9 == 0)
            ints3.push_back(i);
    }

    clock_t t2 = clock();

    CxContainer::remove(ints1, ints3);

    clock_t t3 = clock();

    CxContainer::remove2(ints2, ints3);

    clock_t t4 = clock();

    cout << "t2-t1=" << t2-t1 << " t3-t2=" << t3-t2 << " t4-t3=" << t4-t3 << endl;
    cout << "ints1.size=" << ints1.size() << endl;
    cout << "ints2.size=" << ints2.size() << endl;

    std::map<std::string, int> _localStructTypies; //UserInfo=1
    _localStructTypies["a"] = 1;
    _localStructTypies["b"] = 2;
    vector<string> sReports;
//    CxContainer::joinToString(_localStructTypies, std::string("="), std::string(","));
    sReports.push_back(std::string("LocalStructTypies: ") + CxContainer::joinToString(_localStructTypies, std::string("="), std::string(",")));

}

//template<typename _Tp>
//static void sort(std::vector<_Tp> & vec1)
//{
//    std::sort(vec1.begin(), vec1.end(), CxContainer::compare<_Tp>);
//}

//template<typename _Tp>
//static int compare(const _Tp& v1, const _Tp& v2)
//{
//    return v1 < v2;
//}


void test_container_001()
{
    vector<string> ss;
    ss.push_back("a");
    ss.push_back("b1");
    ss.push_back("c");
    ss.push_back("1");
    ss.push_back("2");
    ss.push_back("3");
    ss.push_back("01");
    ss.push_back("02");
    ss.push_back("1");
    ss.push_back("b2");
    ss.push_back("02");

    CxContainer::sort(ss);
//std::sort(vec1.begin(), vec1.end(), CxContainer::compare<_Tp>);

    for (size_t i = 0; i < ss.size(); ++i)
    {
        cout << ss[i] << endl;
    }
}

int main()
{
    cout << "Hello World!" << endl;

    test_container_001();

    CxResource::start();

    return 0;
}
