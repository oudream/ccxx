import Module1

def multiply(a,b):
    print("Will compute", a, "times", b)
    c = 0
    for i in range(0, a):
        c = c + b
    return c

def testModule1():
    print("Module1.gcd(110, 30) <330> = ",Module1.gcd(5, 6))
    print("Module1.in_mandel(6.6, 4.4, 2) <7.04> = ",Module1.in_mandel(6.6, 4.4, 2))
    print("Module1.divide(6, 4) <48, 10> = ",Module1.divide(6, 4))

testModule1()