## Operand Specifiers
| Type       | Form                  | Operand value                  | Name                |
| :--------- | :-------------------- | :----------------------------- | :------------------ |
| Immediate  | $\$Imm$                | $Imm$                           | Immediate           |
| Register   | $r_a$                 | R[$r_a$]                        | Register            |
| Memory     | $Imm$                 | M[$Imm$]                        | Absolute            |
| Memory     | $(r_a)$               | M[R[$r_a$]]                     | Indirect            |
| Memory     | $Imm(r_b)$            | M[$Imm$ + R[$r_b$]]             | Base + displacement |
| Memory     | $(r_b, r_i)$          | M[R[$r_b$] + R[$r_i$]]          | Indexed             |
| Memory     | $Imm(r_b, r_i)$       | M[$Imm$ + R[$r_b$] + R[$r_i$]]  | Indexed             |
| Memory     | $(, r_i, s)$          | M[R[$r_i$] $\cdot s$]            | Scaled indexed      |
| Memory     | $Imm(, r_i, s)$       | M[$Imm$ + R[$r_i$] $\cdot s$]    | Scaled indexed      |
| Memory     | $(r_b, r_i, s)$       | M[R[$r_b$] + R[$r_i$] $\cdot s$] | Scaled indexed      |
| Memory     | $Imm(r_b, r_i, s)$    | M[$Imm$ + R[$r_b$] + R[$r_i$] $\cdot s$] | Scaled indexed      |



