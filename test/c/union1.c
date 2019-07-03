#include <stdio.h>
#include <string.h>

union Data
{
    int i;
    float f;
    char  str[20];
};





struct TYPEA {
    char data[30]; // or whatever
};

struct TYPEB {
    double x, y; // or whatever
};

struct some_info {
    int type; // set accordingly
    union {
        struct TYPEA a;
        struct TYPEB b;
    } data; // access with some_info_object.data.a or some_info_object.data.b
};





union {
    struct  {
        char ax[2];
        char ab[2];
    } s;
    struct  {
        int a;
        int b;
    } st;
//} u = { .s = { {'a', 'b'}, { 'c', 'd' } } };
} u = { .st = {11,12} };

//printf("%d %d", u.st.a, u.st.b);



int main( )
{
    union Data data;

    data.i = 10;
    printf( "data.i : %d\n", data.i);

    data.f = 220.5;
    printf( "data.f : %f\n", data.f);

    strcpy( data.str, "C Programming");
    printf( "data.str : %s\n", data.str);

//    some_info si = {};

//    u = { .s = { {'a', 'b'}, { 'c', 'd' } } }

//    u = { .st = {11,12}};

    printf("%d - %d", u.s.ax[0], u.s.ab[0]);

    return 0;
}