## Condition Code Registers
CF: Carry flag. The most recent operation generated a carry out of the most significant bit. Used to detect overflow for unsigned operations.

ZF: Zero flag. The most recent operation yielded zero.

SF: Sign flag. The most recent operation yielded a negative value.

OF: Overflow flag. The most recent operation caused a two’s-complement
overflow—either negative or positive.

## Comparison and Test Instructions
These instructions set the conditional codes without changing any rigisters.

Note that `leaq` does not change any conditional code.
| Instruction | Based on     | Description         |
| :---------- | :----------- | :------------------ |
| `CMP` $S_1, S_2$      |  $S_2 - S_1$   | Compare             |
| `cmpb`      |  | Compare byte        |
| `cmpw`      |              | Compare word        |
| `cmpl`      |              | Compare double word |
| `cmpq`      |              | Compare quad word   |
| `TEST`  $S_1, S_2$    |  $S_1$ & $S_2$  | Test                |
| `testb`     |              | Test byte           |
| `testw`     |              | Test word           |
| `testl`     |              | Test double word    |
| `testq`     |              | Test quad word      |

## `set` Instructions
These instructions set a single byte to 0 or 1 based on
some combination of the condition codes. 
| Instruction | Synonym | Effect                  | Set condition               |
| :---------- | :------ | :---------------------- | :-------------------------- |
| `sete` D    | `setz`  | D $\leftarrow$ ZF       | Equal / zero                |
| `setne` D   | `setnz` | D $\leftarrow$ ~ZF      | Not equal / not zero        |
| `sets` D    |         | D $\leftarrow$ SF       | Negative                    |
| `setns` D   |         | D $\leftarrow$ ~SF      | Nonnegative                 |
| `setg` D    | `setnle`| D $\leftarrow$ ~(SF ^ OF) & ~ZF | Greater (signed >)          |
| `setge` D   | `setnl` | D $\leftarrow$ ~(SF ^ OF) | Greater or equal (signed >=) |
| `setl` D    | `setnge`| D $\leftarrow$ SF ^ OF | Less (signed <)             |
| `setle` D   | `setng` | D $\leftarrow$ (SF ^ OF) \| ZF | Less or equal (signed <=)   |
| `seta` D    | `setnbe`| D $\leftarrow$ ~CF & ~ZF | Above (unsigned >)          |
| `setae` D   | `setnb` | D $\leftarrow$ ~CF      | Above or equal (unsigned >=) |
| `setb` D    | `setnae`| D $\leftarrow$ CF       | Below (unsigned <)          |
| `setbe` D   | `setna` | D $\leftarrow$ CF \| ZF | Below or equal (unsigned <=) |

## Jump Instructions
| Instruction     | Synonym | Jump condition              | Description                |
| :-------------- | :------ | :-------------------------- | :------------------------- |
| Direct and Indirect jump
| `jmp` *Label* |         | 1                           | Direct jump                |
| `jmp` *Operand* |         | 1                           | Indirect jump              |
| Conditional jump
| `je` *Label* | `jz`    | ZF                          | Equal / zero               |
| `jne` *Label* | `jnz`   | ~ZF                         | Not equal / not zero       |
| `js` *Label* |         | SF                          | Negative                   |
| `jns` *Label* |         | ~SF                         | Nonnegative                |
| `jg` *Label* | `jnle`  | ~(SF ^ OF) & ~ZF            | Greater (signed >)         |
| `jge` *Label* | `jnl`   | ~(SF ^ OF)                  | Greater or equal (signed >=) |
| `jl` *Label* | `jnge`  | SF ^ OF                     | Less (signed <)            |
| `jle` *Label* | `jng`   | (SF ^ OF) \| ZF             | Less or equal (signed <=)  |
| `ja` *Label* | `jnbe`  | ~CF & ~ZF                   | Above (unsigned >)         |
| `jae` *Label* | `jnb`   | ~CF                         | Above or equal (unsigned >=) |
| `jb` *Label* | `jnae`  | CF                          | Below (unsigned <)         |
| `jbe` *Label* | `jna`   | CF \| ZF                    | Below or equal (unsigned <=) |

Indirect Jump: `jmp *Operand`, jump to a target read from register or memory, for example, Function Pointer Table. 

The conditional jumps can only be direct, using a specified lable as its jump target. 

## Understanding Jump Instructions In Disassembler

```
1 4004d0: 48 89 f8 mov %rdi,%rax
2 4004d3: eb 03 jmp 4004d8 <loop+0x8>
3 4004d5: 48 d1 f8 sar %rax
4 4004d8: 48 85 c0 test %rax,%rax
5 4004db: 7f f8 jg 4004d5 <loop+0x5>
6 4004dd: f3 c3 repz retq
```
The jump target on line 5, `4004d5`, is calculated by: add `0xf8`, the signed jump target (relative) to the address of the following instruction.

## Conditional Moves
| Instruction | Synonym | Move condition            | Description                  |
| :---------- | :------ | :------------------------ | :--------------------------- |
| `cmove` S, R | `cmovz` | ZF                        | Equal / zero                 |
| `cmovne` S, R | `cmovnz`| ~ZF                       | Not equal / not zero         |
| `cmovs` S, R |         | SF                        | Negative                     |
| `cmovns` S, R |         | ~SF                       | Nonnegative                  |
| `cmovg` S, R | `cmovnle`| ~(SF ^ OF) & ~ZF          | Greater (signed >)           |
| `cmovge` S, R | `cmovnl`| ~(SF ^ OF)                | Greater or equal (signed >=) |
| `cmovl` S, R | `cmovnge`| SF ^ OF                   | Less (signed <)              |
| `cmovle` S, R | `cmovng`| (SF ^ OF) \| ZF           | Less or equal (signed <=)    |
| `cmova` S, R | `cmovnbe`| ~CF & ~ZF                 | Above (unsigned >)           |
| `cmovae` S, R | `cmovnb`| ~CF                       | Above or equal (Unsigned >=) |
| `cmovb` S, R | `cmovnae`| CF                        | Below (unsigned <)           |
| `cmovbe` S, R | `cmovna`| CF \| ZF                  | Below or equal (unsigned <=) |

If the condition holds, move the value in `S` (both memory and register is OK) to `R`

The assembler can infer the operand length of a conditional move instruction from the name of the destination register, and so the same instruction name can be used for all operand lengths.

Conditional data transfers can outperform code based on conditional control transfers, as it not requires the processor to predict the outcome of the test.

