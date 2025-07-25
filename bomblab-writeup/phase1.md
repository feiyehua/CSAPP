# CS:APP bomblab Writeup
## First Look: Running the Program
```
~/csapp-exp/bomb$ ./bomb 
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!
Error: Premature EOF on stdin
```
## Phase 1

First, disassemble the bomb executable, dump the symbol table and the string table.

As is known to all, the printable strings are keys to reverse engineering.

```
2338 Welcome to my fiendish little bomb. You have 6 phases with
2378 which to blow yourself up. Have a nice day!
23a8 Phase 1 defused. How about the next one?
```

In .s file:
```
400e1e:	bf 38 23 40 00       	mov    $0x402338,%edi
400e23:	e8 e8 fc ff ff       	call   400b10 <puts@plt>
400e28:	bf 78 23 40 00       	mov    $0x402378,%edi
400e2d:	e8 de fc ff ff       	call   400b10 <puts@plt>
400e32:	e8 67 06 00 00       	call   40149e <read_line>
400e37:	48 89 c7             	mov    %rax,%rdi
400e3a:	e8 a1 00 00 00       	call   400ee0 <phase_1>
400e3f:	e8 80 07 00 00       	call   4015c4 <phase_defused>
400e44:	bf a8 23 40 00       	mov    $0x4023a8,%edi
```

We can see that the function call `<phase_1>` and `<phase_defused>` plays a crucial role.

function `phase_1`:
```
0000000000400ee0 <phase_1>:
  400ee0:	48 83 ec 08          	sub    $0x8,%rsp
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi
  400ee9:	e8 4a 04 00 00       	call   401338 <strings_not_equal>
  400eee:	85 c0                	test   %eax,%eax
  400ef0:	74 05                	je     400ef7 <phase_1+0x17>
  400ef2:	e8 43 05 00 00       	call   40143a <explode_bomb>
  400ef7:	48 83 c4 08          	add    $0x8,%rsp
  400efb:	c3                   	ret
```

```
000000000040143a <explode_bomb>:
  40143a:	48 83 ec 08          	sub    $0x8,%rsp
  40143e:	bf a3 25 40 00       	mov    $0x4025a3,%edi
  401443:	e8 c8 f6 ff ff       	call   400b10 <puts@plt>
  401448:	bf ac 25 40 00       	mov    $0x4025ac,%edi
  40144d:	e8 be f6 ff ff       	call   400b10 <puts@plt>
  401452:	bf 08 00 00 00       	mov    $0x8,%edi
  401457:	e8 c4 f7 ff ff       	call   400c20 <exit@plt>
  ```
Related strings:
```
25a4 BOOM!!!
   25ac The bomb has blown up.
   ```
   This cause the bomb to explode.

Create a breakpoint at the entry of `explode_bomb`:

`break explode_bomb`

We can see that if the value in `%eax` is equal to 0, we will jump to 400ef7, therefore prevent the bomb from exploding.

The call to `strings_not_equal` is crucial. And before the call, address `0x402400` copied into `%esi`.

Turn back to the string table, we can see:

`   2400 Border relations with Canada have never been better.`

We can guess this is the passphrase for phase 1.

The test result in `gdb` confirmed this.

```
Border relations with Canada have never been better.
Phase 1 defused. How about the next one?
```