## Data move instructions
| Instruction | Effect      | Description           |
| :---------- | :---------- | :-------------------- |
| `mov D S`       | $S\leftarrow D$      | Move                  |
| `movb`      |             | Move byte             |
| `movw`      |             | Move word             |
| `movl`      |             | Move double word      |
| `movq`      |             | Move quad word        |
| `movabsq I R`   | $I\leftarrow R$      | Move absolute quad word |

move data between registers and registers, or between registers and memory. It requires 2 instructions to move data from a memory adress to another memory adress.

### Why `movabsq`?
The regular movq instruction can only have immediate source operands
that can be represented as 32-bit two’s-complement numbers. This value is then
sign extended to produce the 64-bit value for the destination. The movabsq in-
struction can have an arbitrary 64-bit immediate value as its source operand and
can only have a register as a destination.

## Zero extending data movement instructions
| Instruction | Effect           | Description                       |
| :---------- | :--------------- | :-------------------------------- |
| `movz S R`      |$R\leftarrow S(with\ zero-extend)$           | Move with zero extension          |
| `movzbw`    |                  | Move zero-extended byte to word   |
| `movzbl`    |                  | Move zero-extended byte to double word |
| `movzwl`    |                  | Move zero-extended word to double word |
| `movzbq`    |                  | Move zero-extended byte to quad word |
| `movzwq`    |                  | Move zero-extended word to quad word |

These instructions can have either registers or memory as source, but can only have register as its destination.

note: there is no `movzlq`, because `movel` is enough. This technique takes advantage of the property that an instruction generating a 4-byte value with a register as the destination will fill the upper 4 bytes with zeros. 

Usually, `gcc` would use `movebl` instead of `movebq` for the same perpose, for a better efficiency.

It's generally more efficient for the CPU to load a full word (4 bytes) or quadword (8 bytes) into a register in one go, even if you only need a portion of it.

## Sign-extending data movement instructions
| Instruction | Effect           | Description                           |
| :---------- | :--------------- | :------------------------------------ |
| `movs S R`      | $R \leftarrow S(with\ Sign-extend$           | Move with sign extension              |
| `movsbw`    |                  | Move sign-extended byte to word       |
| `movsbl`    |                  | Move sign-extended byte to double word |
| `movswl`    |                  | Move sign-extended word to double word |
| `movsbq`    |                  | Move sign-extended byte to quad word  |
| `movswq`    |                  | Move sign-extended word to quad word  |
| `movslq`    |                  | Move sign-extended double word to quad word |
| `cltq`      | $\%rax \leftarrow \text{SignExtend}(\%eax)$ | Sign-extend %eax to %rax     |

Same as Zero-extending instructions, These instructions can have either registers or memory as source, but can only have register as its destination.

`cltq` has the same effect as `movslq %eax, %rax`, but has a more compact encoding.

Recall that when performing a cast that involves both a size change and a
change of “signedness” in C, the operation should change the size first (Section
2.2.6).