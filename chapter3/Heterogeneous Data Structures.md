# Heterogeneous Data Structures
## Structures
All the offsets of struct elements are completed in compile time. The machine code contains no information
about the field declarations or the names of the fields.

## Unions
Rather than
having the different fields reference different blocks of memory (like structs), unions all reference
the same block.
```c
union U3 {
    char c;
    int i[2];
    double v;
};
``` 
The address of c, i, and v is the address of the beginning of the data structure.

```c
double uu2double(unsigned word0, unsigned word1)
{
    union
    {
        double d;
        unsigned u[2];
    } temp;
    temp.u[0] = word0;
    temp.u[1] = word1;
    return temp.d;
}
```
As unions operate memory directly, code above behaves differently on big- and little-endian machines.

## Data Alignment
The alignment rule on x86 systems is based on the principle that any **primitive**
object of K bytes must have an address that is a multiple of K.

As a result, if a small and a big type co-exist in a struct, sometimes the compiler must insert paddings into structs to satisfy the padding rules.


