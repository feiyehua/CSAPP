## Push and pop instructions
| Instruction | Effect                                     | Description    |
| :---------- | :----------------------------------------- | :------------- |
| `pushq` S   | R[%rsp] $\leftarrow$ R[%rsp] - 8;         | Push quad word |
|             | M[R[%rsp]] $\leftarrow$ S                   |                |
| `popq` D    | D $\leftarrow$ M[R[%rsp]];                | Pop quad word  |
|             | R[%rsp] $\leftarrow$ R[%rsp] + 8          |                |

Note that from the "bottom" of stack to the "top" of bottom, the address is decreasing, and each new element is added at the top of the stack, with a smaller address.

