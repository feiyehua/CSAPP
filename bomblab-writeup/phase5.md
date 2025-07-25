# CS:APP bomblab Writeup
## Phase 5
```
0000000000401062 <phase_5>:
  401062:	53                   	push   %rbx
  401063:	48 83 ec 20          	sub    $0x20,%rsp
  401067:	48 89 fb             	mov    %rdi,%rbx // Input string in %rbx
  40106a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax // Moves a canary number to %rax
  401071:	00 00 
  401073:	48 89 44 24 18       	mov    %rax,0x18(%rsp) // Moves the canary number to 0x18(%rsp)
  401078:	31 c0                	xor    %eax,%eax
  40107a:	e8 9c 02 00 00       	call   40131b <string_length> // Get the length of the string
  40107f:	83 f8 06             	cmp    $0x6,%eax // The length of string should equal to 6
  401082:	74 4e                	je     4010d2 <phase_5+0x70>
  401084:	e8 b1 03 00 00       	call   40143a <explode_bomb>
  401089:	eb 47                	jmp    4010d2 <phase_5+0x70>
  40108b:	0f b6 0c 03          	movzbl (%rbx,%rax,1),%ecx // Jump to here after checking the length of string
  // %rax is the iterator, and check the string byte by byte (store the byte in %ecx)
  40108f:	88 0c 24             	mov    %cl,(%rsp)
  401092:	48 8b 14 24          	mov    (%rsp),%rdx
  401096:	83 e2 0f             	and    $0xf,%edx // AND 0xf to make sure %rdx is smaller than 16
  401099:	0f b6 92 b0 24 40 00 	movzbl 0x4024b0(%rdx),%edx // Move a byte(with offset) to register %edx
  4010a0:	88 54 04 10          	mov    %dl,0x10(%rsp,%rax,1) // Move the byte to 0x10 + %rsp + %rax
  4010a4:	48 83 c0 01          	add    $0x1,%rax // Add %rax
  4010a8:	48 83 f8 06          	cmp    $0x6,%rax // Compare %rax with 6
  4010ac:	75 dd                	jne    40108b <phase_5+0x29>
  4010ae:	c6 44 24 16 00       	movb   $0x0,0x16(%rsp) // Add 0x0 as the end of string
  4010b3:	be 5e 24 40 00       	mov    $0x40245e,%esi // Passphrase: flyers
  4010b8:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
  4010bd:	e8 76 02 00 00       	call   401338 <strings_not_equal>
  4010c2:	85 c0                	test   %eax,%eax
  4010c4:	74 13                	je     4010d9 <phase_5+0x77>
  4010c6:	e8 6f 03 00 00       	call   40143a <explode_bomb>
  4010cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  4010d0:	eb 07                	jmp    4010d9 <phase_5+0x77>
  4010d2:	b8 00 00 00 00       	mov    $0x0,%eax // Clear %eax
  4010d7:	eb b2                	jmp    40108b <phase_5+0x29>
  4010d9:	48 8b 44 24 18       	mov    0x18(%rsp),%rax // Move the canary number back to register
  4010de:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax // Check if the canary number is broken, we shouldn't break this number
  4010e5:	00 00 
  4010e7:	74 05                	je     4010ee <phase_5+0x8c>
  4010e9:	e8 42 fa ff ff       	call   400b30 <__stack_chk_fail@plt>
  4010ee:	48 83 c4 20          	add    $0x20,%rsp
  4010f2:	5b                   	pop    %rbx
  4010f3:	c3                   	ret
```
Before calling `phase_5`, the caller read a line from stdin and store the address in %rdi.

`000024b0: 6d61 6475 6965 7273 6e66 6f74 7662 796c  maduiersnfotvbyl`

So the offset required is 9,15,14,5,6,7, respectively.

We can get passphrase `IONEFG`.

`Good work!  On to the next...`