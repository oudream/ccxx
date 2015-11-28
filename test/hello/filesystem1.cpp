
#undef UNICODE

#include<windows.h>
#include<iostream>
#include<string>

using namespace std;

//只能处理目录:lpPath只能是路径
void find(char *lpPath)
{
    char szFind[MAX_PATH];
    char szFile[MAX_PATH];

    WIN32_FIND_DATA FindFileData;

    strcpy(szFind,lpPath);
    strcat(szFind,"//*.*");

    HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
    if(INVALID_HANDLE_VALUE == hFind)    return;
    while(TRUE)
    {
        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if(FindFileData.cFileName[0]!='.')
            {
                strcpy(szFile,lpPath);
                strcat(szFile,"//");
                strcat(szFile,FindFileData.cFileName);
                find(szFile);
            }
        }
        else
        {
            cout<<FindFileData.cFileName<<endl;
        }
        if(!FindNextFile(hFind,&FindFileData))
            break;
    }
    FindClose(hFind);
}
//可同时处理目录和文件:path可以是路径，也可以是文件名，或者文件通配符
void _find(string path)
{
    //取路径名最后一个"//"之前的部分,包括"//"
    string prefix=path.substr(0,path.find_last_of('//')+1);

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind=::FindFirstFile(path.c_str(),&FindFileData);
    if(INVALID_HANDLE_VALUE == hFind)
    {
        cout<<"文件通配符错误"<<endl;
        return;
    }
    while(TRUE)
    {
        //目录
        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            //不是当前目录，也不是父目录
            if(FindFileData.cFileName[0]!='.')
            {
                //查找下一级目录
                _find(prefix+FindFileData.cFileName+"//"+"*.*");
            }
        }
        //文件
        else
        {
            cout<<FindFileData.cFileName<<endl;
        }
        if(!FindNextFile(hFind,&FindFileData))
            break;
    }
    FindClose(hFind);
}

int main()
{
    find("c:/test");//目录：E盘
//    _fi0nd("c:/test//*.*");//E盘下所有文件

    return 0;
    string str=".//";
    string path;
    cout<<"请输入文件通配符："<<flush;
    cin>>path;
    str=str+path;
    find((char*)str.c_str());//可以处理"."，".." 不可以处理"*"，"..//*"
    _find(str);//可以处理"*"，"..//*" 不可以处理"."，".."
    return 0;
}

