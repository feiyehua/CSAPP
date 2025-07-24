## The Run-Time Stack
The stack is divided into stack frames. The frame of calling procedure `P` includes arguments (passed to the next procedure if needed), and also the return address, which is pushed into the stack when `P` called another procedure, which indicate where within P the program should resume execution once `Q` returns.

The frame for executing function `Q` includes local variables, saved registers, and also preserved argument build area.

Procedure P can pass up to six integral values (i.e.,
pointers and integers) on the stack, but if Q requires more arguments, these can
be stored by P within its stack frame prior to the call.

Some functions that does not call other functions at all and hold all variables in registers does not require a stack frame.

## Control Transfer
| Instruction     | Description    |
| :-------------- | :------------- |
| `call` *Label* | Procedure call |
| `call` *Operand*| Procedure call |
| `ret`           | Return from call |

`call Q` will push address `A`, the return location, which is the address of the instruction following `call`, into the run-time stack, and set programe counter to the start of `Q`.

`ret` will pop the corresponding return address `A` out of stack, and set programe counter to `A`.

## Data Transfer
In `x86-64`, up to six integral arguments can be passed via registers. The registers are placed in a specified order.

| Operand size (bits) | 1      | 2      | 3      | 4      | 5      | 6      |
| :------------------ | :----- | :----- | :----- | :----- | :----- | :----- |
| 64                  | %rdi   | %rsi   | %rdx   | %rcx   | %r8    | %r9    |
| 32                  | %edi   | %esi   | %edx   | %ecx   | %r8d   | %r9d   |
| 16                  | %di    | %si    | %dx    | %cx    | %r8w   | %r9w   |
| 8                   | %dil   | %sil   | %dl    | %cl    | %r8b   | %r9b   |

If a function needs more than 6 arguments, the other ones are passed on the stack.

The 7th argument is on the top of the stack.

When passing parameters
on the stack, all data sizes are rounded up to be multiples of eight.
Assume that the return address is `A`, than the address of the 7th argument is `A+8`, and the address of the 8th argument is `A+16`.

## Local Storage on the Stack
In some cases, local data must be stored in memory.
+ There are not enough registers to hold all of the local data.
+ The address operator ‘&’ is applied to a local variable, and hence we must be
able to generate an address for it.
+ Some of the local variables are arrays or structures and hence must be accessed
by array or structure references.

This result in `Local variables` in stack frame.

The local variables will be manually positioned in the stack. The positioning does not need 8-byte-align.

## Local Storage in Registers
By convention, registers %rbx, %rbp, and %r12–%r15 are classified as callee-saved registers. This means the callee must save the values stored in these registers, and restore the values before returning to the caller.

All the other registers are caller-saved registers, which means that the caller must save the values in these registers, as the callee can freely altering the values saved in these registers.

Saving register values make the portion of the stack frame labeled “Saved registers”

## Recursive Procedures
The first-in-last-out principal allow recursive procedures, as each procedure has its private storage on stack.

