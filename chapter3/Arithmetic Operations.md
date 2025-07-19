# General Arithmetic Operations
| Instruction | Effect           | Description            |
| :---------- | :--------------- | :--------------------- |
| Load Effective Address
| `leaq` S,D  | D $\leftarrow$ &S | Load effective address |
| Unary Operations    
| `INC` D     | D $\leftarrow$ D+1 | Increment              |
| `DEC` D     | D $\leftarrow$ D-1 | Decrement              |
| `NEG` D     | D $\leftarrow$ -D | Negate                 |
| `NOT` D     | D $\leftarrow$ ~D | Complement             |
| Binary Operations
| `ADD` S,D   | D $\leftarrow$ D+S | Add                    |
| `SUB` S,D   | D $\leftarrow$ D-S | Subtract               |
| `IMUL` S,D  | D $\leftarrow$ D*S | Multiply               |
| `XOR` S,D   | D $\leftarrow$ D^S | Exclusive-or           |
| `OR` S,D    | D $\leftarrow$ D\|S | Or                     |
| `AND` S,D   | D $\leftarrow$ D&S | And                    |
| Shift Operations
| `SAL` k,D   | D $\leftarrow$ D<<k | Left shift             |
| `SHL` k,D   | D $\leftarrow$ D<<k | Left shift (same as SAL) |
| `SAR` k,D   | D $\leftarrow$ D>>$_A$ k | Arithmetic right shift |
| `SHR` k,D   | D $\leftarrow$ D>>$_L$ k | Logical right shift    |

Each class includes 4 variants, for example, for `add`, there is `addb`, `addw`, `addl`, and
`addq`



### leaq
Here, a number are stored in registers, such as %rax. By using `leaq 4(%rax) %rbx`, we store value `%rax+4` to `%rbx`, regardless of weather `(%rax)` is accessible. With only one instruction, we complete a data move and a integar add.

Contrary to the case requiring memory access, the scaling factors in leaq can be an arbitrary number other than 1,2,4,8.

Note: the destination must be a register.

### Unary Operations  
Only one operand, serving as both source and destination

The operand can be either register or memory address

### Binary Operations
Note that the second operand serves is used as source and destination. This can be a little counter-intuitive.

The first operand the be immediate, register or memory location, and the second operand can be either register or memory address.

However, if both the first and the second are memory locations, the processor must read the value from corresponding memory location, perform the operation, then store the result back to memory.

### Shift Operations
The shift amount can either be an immediate value or with the single-byte register %cl.

Only the lower few bits (depending on the size of the first operand) will be used to perform shift, and the upper bits will be omitted.

# Special Arithmetic Operations
| Instruction | Effect                                   | Description           |
| :---------- | :--------------------------------------- | :-------------------- |
| `imulq` S   | R[%rdx]:R[%rax] $\leftarrow$ S $\times$ R[%rax] | Signed full multiply  |
| `mulq` S    | R[%rdx]:R[%rax] $\leftarrow$ S $\times$ R[%rax] | Unsigned full multiply |
| `cqto`      | R[%rdx]:R[%rax] $\leftarrow$ SignExtend(R[%rax]) | Convert to oct word   |
| `idivq` S   | R[%rdx] $\leftarrow$ R[%rdx]:R[%rax] mod S; R[%rax] $\leftarrow$ R[%rdx]:R[%rax] $\div$ S | Signed divide         |
| `divq` S    | R[%rdx] $\leftarrow$ R[%rdx]:R[%rax] mod S; R[%rax] $\leftarrow$ R[%rdx]:R[%rax] $\div$ S | Unsigned divide       |

Note: These operations are intended for 128 bit number operations.

## Multiply
`mulq`(for unsigned) and `imulq` are the single-operand version of `mulq`. One operand is `%rax`, by default, and another is given by the instruction source operand. 

The product is then stored in registers `%rdx` (high-order 64 bits) and `%rax` (low-order 64 bits). Note that when storing the result into memory, the bits in `%rdx` are positions in a higher address (on a little-endian machine, as the least significant bit takes the lowest address).

## Division
The signed division instruction idivl takes as its dividend the 128-bit quantity
in registers %rdx (high-order 64 bits) and %rax (low-order 64 bits). The divisor is
given as the instruction operand. The instruction stores the quotient in register
%rax and the remainder in register %rdx.

The general 64 bit division uses the same instruction as 128 bit does.
However, it uses instruction `cqto` to reads the sign bit from %rax and copies it across all of %rdx.

