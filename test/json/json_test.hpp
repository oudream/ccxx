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

#include <ccxx/rapidjson/document.h>
#include <ccxx/rapidjson/stringbuffer.h>
#include <ccxx/rapidjson/writer.h>
#include <ccxx/rapidjson/pointer.h>
#include <ccxx/rapidjson/filereadstream.h>
#include <ccxx/rapidjson/filewritestream.h>

using namespace rapidjson;
using namespace std;

int testDocument11() {
    //1生成 {"hello":"world","t":true,"f":false,"n":null,"i":123,"pi":3.1416,"a":[0,1,2,3]}
    Document d;
    Value::AllocatorType & a = d.GetAllocator();
    d.SetObject();
    d.AddMember("hello", "world", a)
        .AddMember("t", Value(true).Move(), a)
        .AddMember("f", Value(false).Move(), a)
        .AddMember("n", Value(kNullType).Move(), a)
        .AddMember("i", 123, a)
        .AddMember("pi", 3.1416, a)
        .AddMember("a", Value(kArrayType).Move().PushBack(0, a).PushBack(1, a).PushBack(2, a).PushBack(3, a), a);

    StringBuffer buffer;
    Writer<StringBuffer> writer1(buffer);
    d.Accept(writer1);
    const char* output = buffer.GetString();

    cout << output << endl;


    //2生成 {"hello":"world","t":true,"f":false,"n":null,"i":123,"pi":3.1416,"a":[0,1,2,3]}
    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartObject();
    writer.String("hello");
    writer.String("world");
    writer.String("t");
    writer.Bool(true);
    writer.String("f");
    writer.Bool(false);
    writer.String("n");
    writer.Null();
    writer.String("i");
    writer.Uint(123);
    writer.String("pi");
    writer.Double(3.1416);
    writer.String("a");
    writer.StartArray();
    for (unsigned i = 0; i < 4; i++)
        writer.Uint(i);
    writer.EndArray();
    writer.EndObject();

    cout << s.GetString() << endl;

    return 0;
}

int testPointer11()
{
// ...
    Document d;
// 使用 Set() 创建 DOM
    Pointer("/project").Set(d, "RapidJSON");
    Pointer("/stars").Set(d, 10);
// { "project" : "RapidJSON", "stars" : 10 }
// 使用 Get() 访问 DOM。若该值不存在则返回 nullptr。
    if (Value *stars = Pointer("/stars").Get(d))
    {
        stars->SetInt(stars->GetInt() + 1);
    }
// { "project" : "RapidJSON", "stars" : 11 }
// Set() 和 Create() 自动生成父值（如果它们不存在）。
    Pointer("/a/b/0").Create(d);
// { "project" : "RapidJSON", "stars" : 11, "a" : { "b" : [ null ] } }
// GetWithDefault() 返回引用。若该值不存在则会深拷贝缺省值。
    Value &hello = Pointer("/hello").GetWithDefault(d, "world");
// { "project" : "RapidJSON", "stars" : 11, "a" : { "b" : [ null ] }, "hello" : "world" }
// Swap() 和 Set() 相似
    Value x("C++");
    Pointer("/hello").Swap(d, x);
// { "project" : "RapidJSON", "stars" : 11, "a" : { "b" : [ null ] }, "hello" : "C++" }
// x 变成 "world"
// 删去一个成员或元素，若值存在返回 true
//    bool success = Pointer("/a").Erase(d);
//    assert(success);
// { "project" : "RapidJSON", "stars" : 10 }
    if (Value *a = Pointer("/a/b").Get(d))
    {
        Value v("{}");
        a->SetInt(99);
    }

//    Document d;
//    d.Parse(json);
// ...
    FILE *fp = fopen("f:/001.json", "wb"); // 非 Windows 平台使用 "w"
    char writeBuffer[65536];
    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    Writer<FileWriteStream> writer(os);
    d.Accept(writer);
    fclose(fp);

    return 0;
}

int testPointer12()
{
    Document d;

    //*加载文件
    {
        FILE *fp = fopen("f:/000.json", "rb"); // 非 Windows 平台使用 "r"
        char readBuffer[65536];
        FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        d.ParseStream(is);
        fclose(fp);
    }

    //*查找数据
    //以 '/' 为分隔符，以下是：查找 根对象的属性为"tables"数组index为0的...
    //eg: {"tables":[{"rows":[{"ServerId":6}]}]}
    if (Value* stars = Pointer("/tables/0/rows/0/ServerId").Get(d))
    {
        //*删除数据
        stars->RemoveMember("a");
        //*修改数据
        stars->SetObject();
        stars->AddMember("i1", 11, d.GetAllocator());
        stars->AddMember("f1", Value(1.23), d.GetAllocator());
        stars->AddMember("s1", "s_-s\"", d.GetAllocator());
        stars->AddMember("b1", true, d.GetAllocator());
    }

    //*查找数据
    if (Value* stars = Pointer("/tables/0/rows/0/DBType").Get(d))
    {
        //*修改数据
        stars->SetInt(999);
        stars->SetDouble(999.999);
        stars->SetString("s1");
        string s1("MySQL1"); stars->SetString(s1.c_str(), s1.size()); // error : stars->SetString(std::string("s1"));
        stars->SetBool(false);
    }

    //*创建对象
    {
        Value stars(kObjectType); //or : Value stars;stars.SetObject();
        stars.AddMember("i1", 11, d.GetAllocator());
        stars.AddMember("f1", Value(1.23), d.GetAllocator());
        stars.AddMember("s1", "s_-s\"", d.GetAllocator());
        stars.AddMember("b1", true, d.GetAllocator());
        SetValueByPointer(d, "/tables/-", stars);
    }

    //*创建数组
    {
        Value stars(kArrayType); //or : Value stars;stars.SetArray();
        stars.PushBack(11, d.GetAllocator());
        stars.PushBack(Value(1.23), d.GetAllocator());
        stars.PushBack("s_-s\"", d.GetAllocator());
        stars.PushBack(true, d.GetAllocator());
        SetValueByPointer(d, "/tables/-", stars);
    }


    //*保存文件
    {
        FILE *fp = fopen("f:/100.json", "wb"); // 非 Windows 平台使用 "w"
        char writeBuffer[65536];
        FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
        Writer<FileWriteStream> writer(os);
        d.Accept(writer);
        fclose(fp);
    }

    return 0;
}


