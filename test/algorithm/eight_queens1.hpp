#include "global.h"
using namespace std;

#define Queens 8
/*(Output=0 or 1) 1 output 0 do not ouput*/
#define Output 1
int A[Queens*3],B[Queens*3],C[Queens*3],kData[Queens+1][Queens+1];
int inc,*a=&A[Queens],*b=&B[Queens],*c=&C[Queens];
void lay(int i)
{
    int j=0,t,u;
    while(++j<=Queens)
        if(a[j]+b[j-i]+c[j+i]==0)
        {
            kData[i][j]=a[j]=b[j-i]=c[j+i]=1;
            if(i<Queens)lay(i+1);
            else
            {
                ++inc;
                if(Output)
                {
                    for(printf("(%d)\n",inc),u=Queens+1; --u; printf("\n"))
                        for(t=Queens+1; --t;)kData[t][u]?printf("Q "):printf("+ ");
                    printf("\n\n\n");
                }
            }
            a[j]=b[j-i]=c[j+i]=kData[i][j]=0;
        }
}

int testEightQueens11(void)
{
    lay(1);
    printf("%d queens has %d anser!",Queens,inc);
    getchar();
    return 0;
}
