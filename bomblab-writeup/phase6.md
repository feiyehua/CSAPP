# CS:APP bomblab Writeup
## Phase 6
Phase 6 first read a string and store the address in %rdi.
```
00000000004010f4 <phase_6>:
  4010f4:	41 56                	push   %r14
  4010f6:	41 55                	push   %r13
  4010f8:	41 54                	push   %r12
  4010fa:	55                   	push   %rbp
  4010fb:	53                   	push   %rbx
  4010fc:	48 83 ec 50          	sub    $0x50,%rsp
  401100:	49 89 e5             	mov    %rsp,%r13
  401103:	48 89 e6             	mov    %rsp,%rsi
  401106:	e8 51 03 00 00       	call   40145c <read_six_numbers> // Read 6 numbers to %rsp + offset, respectively.
  40110b:	49 89 e6             	mov    %rsp,%r14
  40110e:	41 bc 00 00 00 00    	mov    $0x0,%r12d
  401114:	4c 89 ed             	mov    %r13,%rbp // %rbp = %rsp + 0x4 in 2nd time
  401117:	41 8b 45 00          	mov    0x0(%r13),%eax
  40111b:	83 e8 01             	sub    $0x1,%eax // The 1st, 2nd, ..., 6th number should <= 6 and >= 1
  40111e:	83 f8 05             	cmp    $0x5,%eax // %eax <= 5 (unsigned)
  401121:	76 05                	jbe    401128 <phase_6+0x34>
  401123:	e8 12 03 00 00       	call   40143a <explode_bomb>
  401128:	41 83 c4 01          	add    $0x1,%r12d // 1 in the first time, 2 in the 2nd time...
  40112c:	41 83 fc 06          	cmp    $0x6,%r12d
  401130:	74 21                	je     401153 <phase_6+0x5f>
  401132:	44 89 e3             	mov    %r12d,%ebx // %ebx = 1
  401135:	48 63 c3             	movslq %ebx,%rax
  401138:	8b 04 84             	mov    (%rsp,%rax,4),%eax // %eax = the 2nd number in 1st time
  40113b:	39 45 00             	cmp    %eax,0x0(%rbp) // 1st != 2nd, 3rd, 4th, 5th, 6th number in the 1st time,
  // 2nd != 3rd, 4th, 5th, 6th, 3rd != 4th, 5th, 6th ...
  40113e:	75 05                	jne    401145 <phase_6+0x51>
  401140:	e8 f5 02 00 00       	call   40143a <explode_bomb>
  401145:	83 c3 01             	add    $0x1,%ebx // %ebx = 2
  401148:	83 fb 05             	cmp    $0x5,%ebx
  40114b:	7e e8                	jle    401135 <phase_6+0x41>
  40114d:	49 83 c5 04          	add    $0x4,%r13
  401151:	eb c1                	jmp    401114 <phase_6+0x20>
  // Ensures there is no duplicate number
```
In this part, the programm checks if there is duplicate number in the input numbers.

If so, explode the bomb.


```
  // Flip all the input numbers
  401153:	48 8d 74 24 18       	lea    0x18(%rsp),%rsi // %rsi = 0x18 + %rsp
  401158:	4c 89 f0             	mov    %r14,%rax // %rax = %rsp
  40115b:	b9 07 00 00 00       	mov    $0x7,%ecx // %ecx = 0x7
  401160:	89 ca                	mov    %ecx,%edx // %edx = 0x7
  401162:	2b 10                	sub    (%rax),%edx // %edx = 0x7 - the 1st number
  401164:	89 10                	mov    %edx,(%rax) // move back
  401166:	48 83 c0 04          	add    $0x4,%rax
  40116a:	48 39 f0             	cmp    %rsi,%rax // Boundary
  40116d:	75 f1                	jne    401160 <phase_6+0x6c>
  40116f:	be 00 00 00 00       	mov    $0x0,%esi // clear %esi, %esi as iterator
  401174:	eb 21                	jmp    401197 <phase_6+0xa3>
```
In this part, the program make number = 7 - number.

```
  // When %ecx = 1 ... 6, %rdx = 0x006032d0, 0x006032e0, 0x006032f0, 0x00603300, 0x00603310, 0x00603320, respectively.
  // Node 1 ... 6
  401176:	48 8b 52 08          	mov    0x8(%rdx),%rdx 
  // %ecx > 1, meaning no original number equals to 6, save the number at 0x6032d8 in %rdx
  40117a:	83 c0 01             	add    $0x1,%eax // %eax = 2
  40117d:	39 c8                	cmp    %ecx,%eax // %ecx = 1st, 2nd, 3rd..., identify %ecx's value
  40117f:	75 f5                	jne    401176 <phase_6+0x82> 
  401181:	eb 05                	jmp    401188 <phase_6+0x94>
  401183:	ba d0 32 60 00       	mov    $0x6032d0,%edx // jump to here if %ecx  <= 1
  401188:	48 89 54 74 20       	mov    %rdx,0x20(%rsp,%rsi,2) // %rsp + 2 * %rsi + 20 = 0x6032d0
  40118d:	48 83 c6 04          	add    $0x4,%rsi // Add iterator
  401191:	48 83 fe 18          	cmp    $0x18,%rsi // Boundary
  401195:	74 14                	je     4011ab <phase_6+0xb7>
  401197:	8b 0c 34             	mov    (%rsp,%rsi,1),%ecx // %ecx = 1st, 2nd, 3rd...
  40119a:	83 f9 01             	cmp    $0x1,%ecx // <= 1, meaning the number equals to 1, the original number equals to 6
  40119d:	7e e4                	jle    401183 <phase_6+0x8f>
  40119f:	b8 01 00 00 00       	mov    $0x1,%eax // jump to here if > 1, then %eax = 1
  4011a4:	ba d0 32 60 00       	mov    $0x6032d0,%edx
  4011a9:	eb cb                	jmp    401176 <phase_6+0x82>
  // In summary, no matter what is the six numbers, // %rsp + 2 * %rsi + 20 = 0x6032d0 will always be executed
```
In this part, the program will edit an array of type node*, whose starting address is %rsp + 0x20.

Base on the input and flipped number, the value will be set to 0x006032d0, 0x006032e0, 0x006032f0, 0x00603300, 0x00603310,or 0x00603320.

They are the address of node 1 ... 6, respectively.

```
  // All possible values of %rdx and %rcx values: 0x006032d0, 0x006032e0, 0x006032f0, 0x00603300, 0x00603310, 0x00603320
  // This creates a list by editing Node.next.
  4011ab:	48 8b 5c 24 20       	mov    0x20(%rsp),%rbx // %rbx = 0x20(%rsp) 
  4011b0:	48 8d 44 24 28       	lea    0x28(%rsp),%rax // %rax = 0x28 + %rsp, the iterator
  4011b5:	48 8d 74 24 50       	lea    0x50(%rsp),%rsi // %rsi = 0x50 + %rsp, the boundary
  4011ba:	48 89 d9             	mov    %rbx,%rcx // %rcx = 0x20(%rsp)
  4011bd:	48 8b 10             	mov    (%rax),%rdx // jump to here, %rdx = 0x28(%rsp) value
  4011c0:	48 89 51 08          	mov    %rdx,0x8(%rcx) // 
  4011c4:	48 83 c0 08          	add    $0x8,%rax // %rax += 0x8
  4011c8:	48 39 f0             	cmp    %rsi,%rax // The boundary
  4011cb:	74 05                	je     4011d2 <phase_6+0xde>
  4011cd:	48 89 d1             	mov    %rdx,%rcx
  4011d0:	eb eb                	jmp    4011bd <phase_6+0xc9>
  4011d2:	48 c7 42 08 00 00 00 	movq   $0x0,0x8(%rdx)
  4011d9:	00 
```
Next, the program edits node 1 ... 6, using the pointer saved in stack.

Lets examine the data structure node. Node is consist of two part: At offset 0, there is a integer (Node.x), and at offset 8, there is a pointer of type (Node.next of type Node*).

The programm edits node.next to the next node address stored in stack.

```
  // 
  4011da:	bd 05 00 00 00       	mov    $0x5,%ebp // %ebp = 5
  4011df:	48 8b 43 08          	mov    0x8(%rbx),%rax // %rbx = 0x20(%rsp), %rax = 0x8(%rbx) = 0x28(%rsp)
  4011e3:	8b 00                	mov    (%rax),%eax
  4011e5:	39 03                	cmp    %eax,(%rbx)
  4011e7:	7d 05                	jge    4011ee <phase_6+0xfa> // 0x20(%rsp) >= 0x28(%rsp)
  4011e9:	e8 4c 02 00 00       	call   40143a <explode_bomb>
  4011ee:	48 8b 5b 08          	mov    0x8(%rbx),%rbx
  4011f2:	83 ed 01             	sub    $0x1,%ebp
  4011f5:	75 e8                	jne    4011df <phase_6+0xeb> // Iterate 5 times
  4011f7:	48 83 c4 50          	add    $0x50,%rsp
  4011fb:	5b                   	pop    %rbx
  4011fc:	5d                   	pop    %rbp
  4011fd:	41 5c                	pop    %r12
  4011ff:	41 5d                	pop    %r13
  401201:	41 5e                	pop    %r14
  401203:	c3                   	ret
```
Here comes the final check. 

In every iteration the programm compares node.x and (node.next)->x. The address of node.x is stored in %rbx.

We can see node.x using `gdb`:
```
(gdb) x/w 6304464
0x6032d0 <node1>:       332
(gdb) x/w 0x6032e0
0x6032e0 <node2>:       168
(gdb) x/w 0x6032f0
0x6032f0 <node3>:       924
(gdb) x/w 0x603300
0x603300 <node4>:       691
(gdb) x/w 0x603310
0x603310 <node5>:       477
(gdb) x/w 0x603320
0x603320 <node6>:       443
```

We can get the correct order of node is node 3,4,5,6,1,2.

Therefore, we can get the passphrase `4 3 2 1 6 5`.

`Congratulations! You've defused the bomb!`