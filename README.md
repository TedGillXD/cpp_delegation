# Delegation
This is a simple delegation system implementation in C++.

## Features
- Support member functions
- Support global functions
- Support multiple delegation

## How to use
Add the delegation.h file into your project first.
Add a new delegation in class. For example, I want to declare a delegation that return void with 2 integer inputs.

```C++
//declare delegation on the top of the file
DECLARE_FUNCTION_DELEGATE_WITH_RETURN_VALUE(int, DelegationName, int, int)
```

Then we can use it in class or struct
```C++
class Class {
public:
    DelegationName delegation;
}
```

To bind static function
```C++
int function(int a, int b) {
    a + b;
}

delegation.Bind(&function);
```

To bind dynamic function
```C++
class A {
public:
    int function(int a, int b) {
        a + b;
    }
}

A a;
delegation.Bind(a, &A::function);
```

To execute delegation
```C++
int a = 0, b = 0;
int returnValue = delegation.Execute(a, b);
```

There are three different macros for declaring delegations.
```C++
//declare single delegation with no return value
DECLARE_FUNCTION_DELEGATE(DelegationName, ...)
//declare single delegation with return type
DECLARE_FUNCTION_DELEGATE_WITH_RETURN_VALUE(ReturnType, DelegationName, ...)
//decalre multiple delegation(multiple delegation connot have return type)
DECLARE_MULTICAST_FUNCTION_DELEGATE(DelegationName, ...)
```
