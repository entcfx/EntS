### Overview

**EntS** is a simple C like non object oriented language.
The goal of EntS is to provide an easy and fast way for developers to write programs in a similar style as they would with languages such as
C, while providing a unique syntax and low level access to hardware. This manual aims to fully explain the language's syntax and features.

## Syntax

# Chapter 1 > Data Types

In EntS, a concise set of base data types lays the foundation for variable declarations. To expand this repertoire, users can define additional types, a concept further explored in Chapter 8 > Typdef.

*int* -> int is a 32 bit numerical value, capable of representing decimal from **-2,147,483,648** to **2,147,483,647** or hex of **-0x80000000** to **0x7FFFFFFF** for negative and positive values respectively.
*uint* -> uint is a 32 bit unsigned numerical value, uncapable of representing values less than 0. It can represent values from **0** to **4,294,967,295** in decimal and **0xFFFFFFFF** in hex.\
*float* -> float is a 32 bit floating-point data type.
*double* -> double is a 64 bit floating-point data type.
*char* -> char is a single byte data type, capable of representing a single character.
*bool* -> bool is a one byte value, capable of representing true ( 1 ) or false ( 0 ).
*NULL* -> NULL is a special type representing nothing.

* Fixed-Size Integer Types
*int8_t* -> int8_t is an 8 bit signed integer data type.
*uint8_t* -> uint8_t is an 8 bit unsigned integer data type.
*int16_t* -> int16_t is a 16 bit signed integer data type.
*uint16_t* -> uint16_t is a 16 bit unsigned integer data type.

## * Pointers
Pointers are variables that store the memory address of another variable. They provide a way to indirectly access the value of a variable. 
In EntS, a pointer is defined by the name of the data type it is pointing to, followed by an star and the pointer variable name.

### Syntax of pointers
The general syntax for declaring a pointer in EntS is as follows:

```c
datatype* pointerName;
```
Here, datatype is the type of the variable that the pointer points to, and pointerName is the name of the pointer variable.

***Example:***
```c
int* pointerToInt;
char* pointerToChar;
```
In the above examples, pointerToInt is a pointer to an integer, and pointerToChar is a pointer to a character.

### Initilisation
Pointers can be initialized by assigning the memory address of a variable to them. The address-of operator & is used to obtain the memory address of a variable.

***Example:***
```c
int main() {
    int variable = 42;
    int* pointerToVariable = &variable;
    return 0;
}
```
In this example, pointerToVariable is initialized with the memory address of the variable.

### Dereferencing
Dereferencing a pointer means accessing the value at the memory address the pointer is pointing to. The dereference operator * is used for this purpose.

***Example:***
```c
int main() {
    int variable = 42;
    int* pointerToVariable = &variable;

    // Dereferencing the pointer
    int value = *pointerToVariable;

    return 0;
}
```
In this example, value will be assigned the value of the variable variable through the dereferenced pointer.

### Null Pointers
A pointer that does not point to any valid memory location is called a null pointer. It is a good practice to initialize pointers to null when they are declared.

Example:
```c
int* nullPointer = NULL;
```

# Chapter 2 > Flow control

The program's entry point is a function called main() located in the primary file. This function is executed at the start and returning from it
will end the program. Below is a simple example of a main() function.

```c
main() {
    // Code here
} int;
```

Unlike other C like languages, the return value is not listed before the function name, but after the block ( A block is the part of a function 
that include commands that should be executed. It is enclosed by `{}` brackets ) followed by a semicolon to mark the end.
If the code above was to be compiled and ran, it would do nothing. The reason for this is because there are no commands in the `{}` brackets. 
The main() function must return a type `int`. If it doesn't, it is not taken as a valid entry point into the program.

As the programmer, you can define other functions, following the same general syntax as the main function, but with more freedom. A function's
name must start with a alphabetical letter, a valid return type ( or `void`) must be specified followed by a semicolon. A function may also
take in arguments, in the brackets `()`. In the brackets, the variable type and name must be specified. Variables are seperated by a comma `,`.
Here is a example of a custom function that a programmer can add to their code:

```c
returnA(int a, int b) {
    return(a);
} int;
```
As we can see, there is a new command we have not talked about. The `return` keyword, takes in one parameter. A variable that will be returned
to the caller function, as well as flow control. A function can be called with the following syntax:
```c
// Code A
ourAVariable = call(returnA(10, 24));
// Code B
```
Here, we are calling the function returnA with two parameters, 10, which will be recived as `a` into the function and 24, which will be recived as `b`
This means, that after Code A is executed, the program will jump into our returnA function, with the parameters `a` and `b`. Once it reaches the return(a);
the program will jump back, to the end of our call ( at the end of `a = returnA(10, 24);` ), assign the value returned into ourAVariable and continue to execute
Code B.

In any function, the return type must be stated. If not, the compiler will throw an error and terminate. Here is a example of a code that will **not compile**
```c
returnA(int a, int b) {
    return a;
};
```

