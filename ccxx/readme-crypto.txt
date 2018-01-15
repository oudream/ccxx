/**
//单个字符异或运算
char MakecodeChar(char c,int key)
{
    return c=c^key;
}
//单个字符解密
char CutcodeChar(char c,int key)
{
    return c^key;
}
//加密
void Encode(char *pstr,int *pkey)
{
    int len=strlen(pstr);//获取长度
    for(int i=0;i<len;i++)
        *(pstr+i)=MakecodeChar(*(pstr+i),pkey[i%5]);
}
//解密
void Decode(char *pstr,int *pkey)
{
    int len=strlen(pstr);
    for(int i=0;i<len;i++)
        *(pstr+i)=CutcodeChar(*(pstr+i),pkey[i%5]);
}

int main(void)
{
    printf("Hello World!\n");

    //示例代码
    int key[]={1,2,3,4,5};//加密字符 5位
    char s[]="www.xiaozhuanggushi.com";
    char *p=s;
    cout<<"Encode before:"<<p<<endl;
    Encode(s,key);//加密
    cout<<"Encode after:"<<p<<endl;
    Decode(s,key);//解密
    cout<<"Decode:"<<p<<endl;

    return 0;
}
 */


