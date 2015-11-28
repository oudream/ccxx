#ifndef CXMATH_H
#define CXMATH_H

#include "cxglobal.h"

class CxMath
{
public:
    //最大公约数
    static int gcd(int a,int b);

    //最小公倍数
    static int lcm(int a,int b);

    //随机整数
    static int random(int iMax = 0);

    //随机整数
    static int divRound(int iDivisor, int iDivisored);

};

/*** number math

rand        Generate random number (function )
srand       Initialize random number generator (function )
abs         Absolute value (function )
div         Integral division (function )
labs        Absolute value (function )
ldiv        Integral division (function )
ceil        Round up value (function )
floor       Round down value (function )


cos         Compute cosine (function )
sin         Compute sine (function )
tan         Compute tangent (function )
acos        Compute arc cosine (function )
asin        Compute arc sine (function )
atan        Compute arc tangent (function )
atan2       Compute arc tangent with two parameters (function )
cosh        Compute hyperbolic cosine (function )
sinh        Compute hyperbolic sine (function )
tanh        Compute hyperbolic tangent (function )
exp         Compute exponential function (function )
frexp       Get significand and exponent (function )
ldexp       Generate value from significand and exponent (function )
log         Compute natural logarithm (function )
log10       Compute common logarithm (function )
modf        Break into fractional and integral parts (function )
pow         Raise to power (function )
sqrt        Compute square root (function )
fmod        Compute remainder of division (function )
copysign    Copy sign (function )
NAN         Not-A-Number (constant )
nextafter   Next representable value (function )
nexttoward  Next representable value toward precise value (function )
fdim        Positive difference (function )
fmax        Maximum value (function )
fmin        Minimum value (function )

 */


/*** limit
 *
CHAR_BIT    Number of bits in a char object (byte)
8 or greater*

SCHAR_MIN   Minimum value for an object of type signed char
-127 (-27+1) or less*

SCHAR_MAX   Maximum value for an object of type signed char
127 (27-1) or greater*

UCHAR_MAX   Maximum value for an object of type unsigned char
255 (28-1) or greater*

CHAR_MIN    Minimum value for an object of type char
either SCHAR_MIN or 0

CHAR_MAX    Maximum value for an object of type char
either SCHAR_MAX or UCHAR_MAX

MB_LEN_MAX  Maximum number of bytes in a multibyte character, for any locale
1 or greater*

SHRT_MIN    Minimum value for an object of type short int
-32767 (-215+1) or less*

SHRT_MAX    Maximum value for an object of type short int
32767 (215-1) or greater*

USHRT_MAX   Maximum value for an object of type unsigned short int
65535 (216-1) or greater*

INT_MIN     Minimum value for an object of type int
-32767 (-215+1) or less*

INT_MAX     Maximum value for an object of type int
32767 (215-1) or greater*

UINT_MAX    Maximum value for an object of type unsigned int
65535 (216-1) or greater*

LONG_MIN    Minimum value for an object of type long int
-2147483647 (-231+1) or less*

LONG_MAX    Maximum value for an object of type long int
2147483647 (231-1) or greater*

ULONG_MAX   Maximum value for an object of type unsigned long int
4294967295 (232-1) or greater*

LLONG_MIN   Minimum value for an object of type long long int
-9223372036854775807 (-263+1) or less*

LLONG_MAX   Maximum value for an object of type long long int
9223372036854775807 (263-1) or greater*

ULLONG_MAX  Maximum value for an object of type unsigned long long int
18446744073709551615 (264-1) or greater*
 */

#endif // CXMATH_H
