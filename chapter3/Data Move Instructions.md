## Data move instructions
| Instruction | Effect      | Description           |
| :---------- | :---------- | :-------------------- |
| `mov`       | $S\leftarrow D$      | Move                  |
| `movb`      |             | Move byte             |
| `movw`      |             | Move word             |
| `movl`      |             | Move double word      |
| `movq`      |             | Move quad word        |
| `movabsq`   | $I\leftarrow R$      | Move absolute quad word |

move data between registers and registers, or between registers and memory. It requires 2 instructions to move data from a memory adress to another memory adress.

## Zero extending data movement instructions
| Instruction | Effect           | Description                       |
| :---------- | :--------------- | :-------------------------------- |
| `movz`      | $S\leftarrow R$           | Move with zero extension          |
| `movzbw`    |                  | Move zero-extended byte to word   |
| `movzbl`    |                  | Move zero-extended byte to double word |
| `movzwl`    |                  | Move zero-extended word to double word |
| `movzbq`    |                  | Move zero-extended byte to quad word |
| `movzwq`    |                  | Move zero-extended word to quad word |


note: there is no `movzlq`, because `movel` is enough. This technique takes advantage of the property that an instruction generating a 4-byte value with a register as the destination will fill the upper 4 bytes with zeros. 

Usually, `gcc` would use `movebl` instead of `movebq` for the same perpose, for a better efficiency.

It's generally more efficient for the CPU to load a full word (4 bytes) or quadword (8 bytes) into a register in one go, even if you only need a portion of it.

## Sign-extending data movement instructions
| Instruction | Effect           | Description                           |
| :---------- | :--------------- | :------------------------------------ |
| `movs`      | $S \leftarrow R$           | Move with sign extension              |
| `movsbw`    |                  | Move sign-extended byte to word       |
| `movsbl`    |                  | Move sign-extended byte to double word |
| `movswl`    |                  | Move sign-extended word to double word |
| `movsbq`    |                  | Move sign-extended byte to quad word  |
| `movswq`    |                  | Move sign-extended word to quad word  |
| `movslq`    |                  | Move sign-extended double word to quad word |
| `cltq`      | $\%rax \leftarrow \text{SignExtend}(\%eax)$ | Sign-extend %eax to %rax     |

Recall that when performing a cast that involves both a size change and a
change of “signedness” in C, the operation should change the size first (Section
2.2.6).