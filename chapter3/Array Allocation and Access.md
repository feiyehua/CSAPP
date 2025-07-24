# Array Allocation and Access

## Basic Array and Pointer Operations 
The basic operations like dereferencing the pointer, getting the value of a arbitrary element in a array is quite simple.
You just need to translate it into operand specifiers, and combine them with instructions like `mov` and `leaq`.

## Nested Arrays
Nested arrays follows the same principles as arrays.
```
int A[5][3];
```
is equivalent to the declaration
```
typedef int row3_t[3];
row3_t A[5];
```
There, `sizeof(row3_t)` evaluate to 12.