# 《计算机系统基础》期末模拟卷

> 满分 100 分 · 考试时间 90 分钟 · 闭卷

---

## 一、选择题（每题 3 分，共 30 分）

**1.** 冯·诺依曼结构最核心的思想是（　）
A. 二进制表示数据　　B. 存储程序
C. 流水线执行　　　　 D. 哈佛结构

**2.** 以下关于 MOV 与 LEA 的说法，正确的是（　）
A. MOV 不访存，LEA 访存
B. `MOV EAX, [EBX+4]` 和 `LEA EAX, [EBX+4]` 结果恒等
C. LEA 计算有效地址但不访问内存
D. MOV 不能将立即数送入寄存器

**3.** 8 位补码 `10010110` 的真值是（　）
A. −22　　B. −106　　C. 150　　D. −150

**4.** 以下哪个 IEEE 754 单精度十六进制值代表 `−0.0`？（　）
A. `0x00000000`　　B. `0x80000000`
C. `0xFF800000`　　D. `0x7F800000`

**5.** 在有符号整数比较 `CMP A, B` 之后，`JG` 的跳转条件是（　）
A. CF=0 且 ZF=0　　　　　B. SF=OF 且 ZF=0
C. SF≠OF　　　　　　　　 D. ZF=1

**6.** 以下 C 声明中，存放在 COMMON 节的是（　）
A. `int x = 5;` 　　　　B. `static int x;`
C. `int x;` 　　　　　　 D. `const int x = 5;`

**7.** 可重定位目标文件 `.o` 与可执行文件的区别，正确的是（　）
A. `.o` 文件有程序头表
B. 可执行文件中不含重定位条目
C. `.o` 文件的 `.bss` 节占磁盘空间
D. 可执行文件没有 `.symtab`

**8.** cdecl 调用约定中，清栈的责任在（　）
A. 被调函数，通过 `RET N` 实现
B. 调用者，通过 `ADD ESP, N` 实现
C. 操作系统自动清理
D. 前两个参数走寄存器，不需要清栈

**9.** 以下哪项**不是**缓冲区溢出的防御机制？（　）
A. 栈随机化 (ASLR)　　　　B. 栈破坏检测 (Stack Canary)
C. 栈不可执行 (NX)　　　　 D. 栈压缩 (Stack Compression)

**10.** switch 密集 case 值时，编译器使用跳转表实现 O(1) 分支。跳转表存储在哪个节？（　）
A. `.text`　　B. `.data`　　C. `.rodata`　　D. `.bss`

---

## 二、综合应用题（共 35 分）

### 题 1（12 分）—— IEEE 754 浮点数

已知 `float x = −10.625`。

(1)（6 分）请写出 x 的 IEEE 754 单精度二进制位模式（32 位全部写出）。  
(2)（3 分）将该位模式转为十六进制。  
(3)（3 分）若某个 IEEE 754 单精度值 `0xC1480000`，求真值（十进制）。

---

### 题 2（10 分）—— 补码运算与溢出判断

在 8 位补码系统中：

(1)（3 分）计算 `(−110) + (−60)`，写出二进制竖式。  
(2)（3 分）标明 Cn、C(n−1)、OF、CF 的值。  
(3)（2 分）运算结果是否溢出（有符号）？为什么？  
(4)（2 分）结果作为无符号数解释时 CF 的值说明了什么？

---

### 题 3（8 分）—— 结构体对齐

```c
struct S1 {
    char  a;
    int   b;
    short c;
};

struct S2 {
    short a;
    char  b;
    int   c;
};
```

在 IA-32 (32 位，`sizeof(int)=4, sizeof(short)=2`) 下：

(1)（5 分）计算 `sizeof(struct S1)` 和 `sizeof(struct S2)`，画出各自的内存布局（标注偏移量和填充）。  
(2)（3 分）简述为什么相同的成员类型，sizeof 可能不同。

---

### 题 4（5 分）—— Amdahl 定律

某程序总执行时间为 100 秒，其中 60 秒用于浮点运算。现升级浮点运算单元，使其速度提升为原来的 4 倍。  
(1)（3 分）计算整体加速比。  
(2)（2 分）浮点运算无论优化到多快，理论上整体加速比的上限是多少？

---

## 三、分析应用题（共 25 分）

### 题 5（15 分）—— 反汇编分析

以下是 IA-32 下某函数的反汇编（Intel 语法）：

```asm
func:
    push   ebp
    mov    ebp, esp
    sub    esp, 8
    push   ebx
    mov    DWORD PTR [ebp-4], 0
    mov    DWORD PTR [ebp-8], 1
    jmp    .L2
.L3:
    mov    eax, DWORD PTR [ebp-8]
    mov    ebx, DWORD PTR [ebp+12]
    cmp    eax, ebx
    jg     .L4
    mov    eax, DWORD PTR [ebp+8]
    mov    ebx, DWORD PTR [ebp-8]
    mov    eax, DWORD PTR [eax+ebx*4]
    cmp    DWORD PTR [ebp-4], eax
    jge    .L4
    mov    eax, DWORD PTR [ebp+8]
    mov    ebx, DWORD PTR [ebp-8]
    mov    eax, DWORD PTR [eax+ebx*4]
    mov    DWORD PTR [ebp-4], eax
.L4:
    add    DWORD PTR [ebp-8], 1
.L2:
    mov    eax, DWORD PTR [ebp-8]
    mov    ebx, DWORD PTR [ebp+12]
    cmp    eax, ebx
    jle    .L3
    mov    eax, DWORD PTR [ebp-4]
    pop    ebx
    mov    esp, ebp
    pop    ebp
    ret
```

(1)（4 分）分析 `[ebp+8]` 和 `[ebp+12]` 分别对应 C 函数的哪个参数？参数类型是什么？  
(2)（3 分）`[ebp-4]` 和 `[ebp-8]` 分别存储什么变量？  
(3)（5 分）写出对应的 C 函数（包括函数名和完整函数体）。  
(4)（3 分）`[eax+ebx*4]` 这种寻址方式对应 C 语言中的什么操作？

---

### 题 6（10 分）—— 重定位计算

链接前 `main.o` 的 `.text` 基址为 `0x4004d0`，偏移 `0x15` 处有一条 `call sum` 指令，该指令占 5 字节（操作码 1 字节 + 操作数 4 字节），操作数的 r_offset 为 `0x16`。`sum` 的最终地址经链接后为 `0x400540`。

已知该重定位类型为 `R_386_PC32`（PC 相对），操作数初始值 r_addend = −4。

(1)（3 分）求 P 的值。  
(2)（4 分）计算修正值并写出链接后该位置 4 字节的十六进制内容（小端序）。  
(3)（3 分）若该重定位为绝对重定位 `R_386_32` 且 r_addend = 0，修正值是多少？

---

## 四、简答题（每题 5 分，共 10 分）

### 题 7

简述强符号和弱符号的定义，以及链接器对同名符号的解析规则。请说明未初始化的全局变量在 `.o` 文件中存放在哪个节。

### 题 8

以下是编译一个 C 程序的一条命令：

```bash
gcc -c hello.c -o hello.o
```

(1)（2 分）这条命令完成的是编译流程中的哪个（或哪些）阶段？  
(2)（3 分）写出从 `hello.o` 查看符号表和反汇编 `.text` 节所需的完整命令（IA-32 平台，Intel 语法）。

---

> **考试结束**
