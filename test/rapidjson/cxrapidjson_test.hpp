#include <ccxx/cxrapidjson.h>

using namespace std;
using namespace rapidjson;

int f_argc = 0;
char * * f_argv = NULL;

int testCxRapidJsonDataType11(int argc, char * argv[])
{
    f_argc = argc;
    f_argv = argv;

    cout << "begin test xxx : " << endl;


    // 1. Parse a JSON string into DOM.
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10,\"stars1\":-10123456,\"stars2\":10.0}";
    Document d;
    d.Parse(json);
    cout << "d.HasParseError=" << d.HasParseError() << endl;

    Value& stars1 = d["stars1"];
    cout << "stars1.IsInt()=" << stars1.IsInt() << endl;
    cout << "stars1.IsUint()=" << stars1.IsUint() << endl;
    cout << "stars1.IsInt64()=" << stars1.IsInt64() << endl;
    cout << "stars1.IsUint64()=" << stars1.IsUint64() << endl;
    cout << "stars1.IsDouble()=" << stars1.IsDouble() << endl;

    Value& stars2 = d["stars2"];
    cout << "stars2.IsInt()=" << stars2.IsInt() << endl;
    cout << "stars2.IsUint()=" << stars2.IsUint() << endl;
    cout << "stars2.IsDouble()=" << stars2.IsDouble() << endl;

    cout << d.HasParseError() << endl;
    cout << d.HasParseError() << endl;

    // 2. Modify it by DOM.
    Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;


    std::string sLine;
    std::getline (std::cin, sLine);

    cout << "this size :" << d.MemberCount() << endl;

    cout << "this size :" << sLine.size() << endl;

    cout << "end test xxx !!!" << endl;

    return 0;
}

int testCxRapidJsonPointer11()
{
    Document d;

    //*加载文件
    if (! CxRapidJson::load("f:/000.json", d) )
    {
        cout << "load json fail." << endl;
        return 0;
    }
    else
    {
        cout << "load json success." << endl;
    }

    //*查找数据
    //以 '/' 为分隔符，以下是：查找 根对象的属性为"tables"数组index为0的...
    //eg: {"tables":[{"rows":[{"ServerId":6}]}]}
    if (Value* stars = Pointer("/tables/0/rows/0/ServerId").Get(d))
    {
        //*删除数据
        if (stars->IsObject())
        {
            stars->RemoveMember("a");
        }
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
//        stars->SetDouble(999.999);
        stars->SetString("s1", 2);

        string s2 = "abc如果有长度123d";
        const char * cstr = s2.c_str();
        size_t cstr_len = s2.size();                 // 如果有长度
//        Value s;
// s.SetString(cstr);                  // 这不能通过编译
//        s.SetString(StringRef(cstr));          // 可以，假设它的生命周期安全，并且是以空字符结尾的
//        s = StringRef(cstr);                   // 上行的缩写
//        s.SetString(StringRef(cstr, cstr_len));// 更快，可处理空字符
//        s = StringRef(cstr, cstr_len);         // 上行的缩写

//        string s1("ZeroSource2");
        stars->SetString(StringRef(cstr, cstr_len), d.GetAllocator()); // error : stars->SetString(std::string("s1"));
//        stars->SetInt(999);
//        stars->SetBool(false);
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
        stars.PushBack("s_-s12.23\"", d.GetAllocator());
        stars.PushBack(true, d.GetAllocator());
        SetValueByPointer(d, "/tables/-", stars);
    }


    //*保存文件
    if (! CxRapidJson::save("f:/100.json", d) )
    {
        cout << "load json fail." << endl;
    }
    else
    {
        cout << "load json success." << endl;
    }

    return 0;
}


int testCxRapidJsonPointer12()
{
    Document d;

    //*加载文件
    if (! CxRapidJson::load("/fff/tmp/100.json", d) )
    {
        cout << "load json fail." << endl;
        return 0;
    }
    else
    {
        cout << "load json success." << endl;
    }

    //*查找数据
    //以 '/' 为分隔符，以下是：查找 根对象的属性为"tables"数组index为0的...
    //eg: {"tables":[{"rows":[{"ServerId":6}]}]}
    if (Pointer("/tables/0/rows/0/ServerId").Get(d))
    {
        cout << "has!";
        return FALSE;
    }
    else
    {
        Value* stars = & Pointer("/tables/0/rowss/0/ServerId").Create(d);
        //*删除数据
        if (stars->IsObject())
        {
            stars->RemoveMember("a");
        }
        //*修改数据
        stars->SetObject();
        stars->AddMember("i1", 11, d.GetAllocator());
        stars->AddMember("f1", Value(1.23), d.GetAllocator());
        stars->AddMember("f1", Value(1.23), d.GetAllocator());
        stars->AddMember("s1", "s_-s\"", d.GetAllocator());
        stars->AddMember("b1", true, d.GetAllocator());
    }

    //*保存文件
    if (! CxRapidJson::save("/fff/tmp/100.json", d) )
    {
        cout << "load json fail." << endl;
    }
    else
    {
        cout << "load json success." << endl;
    }

    return 0;
}


