/*---------------------------------------------------------------------
CallConv.cpp - Sample with most types of calling conventions.  
Software Debugging by Raymond Zhang, All rights reserved. 
---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

enum MEAL {BREAKFAST, LUNCH, SUPPER};
#define NAME_LENGTH 20
class Cat
{
	char m_szName[NAME_LENGTH];
public:
	Cat(){m_szName[0]=0;}
	Cat(char* sz);
	char* ChooseFood(MEAL i, ...);
	Cat GetChild(int n);
	char * Name(){return m_szName;}
};
Cat::Cat(char * sz)
{
	strncpy(m_szName,sz,NAME_LENGTH-1);
	m_szName[NAME_LENGTH-1]=0;
}
char* Cat::ChooseFood(MEAL meal, ...)
{
	char * szPrefer=NULL;
    va_list foods;
    va_start(foods,meal);

	szPrefer=va_arg(foods,char *);

    va_end(foods);
	return szPrefer;
}
/* parameter n is to inspect passing order of the implicit
   para for return value */
Cat Cat::GetChild(int n)
{
	Cat c("Caty");
	return c;
}
int __fastcall FastCallFunc(float f, int a, Cat c, int b)
{
	printf("FastCallFunc:%f,%d,%s,%d",f,a,c.Name(),b);
	return 30;
}
void main()
{
	Cat cat("Looi"),cat2("");
	printf("Cat choose food %s.",
		cat.ChooseFood(BREAKFAST,"meat","beaf","rice"));
	cat2=cat.GetChild(2);
	printf("%s's child is %s",cat.Name(),cat2.Name());
	FastCallFunc(1.5,10,cat,20);
}
