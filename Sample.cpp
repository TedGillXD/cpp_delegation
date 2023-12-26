// Delegation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Delegation.h"
#include <iostream>

DECLARE_FUNCTION_DELEGATE_WITH_RETURN_VALUE(bool, DelegateA, int, int);

class A {
public:
    DelegateA delegateA;

    bool onEvent(int a, int b) {
        return delegateA.Execute(a, b);
    }
};

class B {
public:
    bool function(int a, int b) {
        return a > b;
    }
};

int main()
{
    A a;
    B b;

    a.delegateA.Bind(&b, &B::function);

    //A can call a function that declare in B without knowing B
	std::cout << a.onEvent(10, 20) << std::endl;
	std::cout << a.onEvent(20, 10) << std::endl;

    return 0;
}
