<a id="sec3-0"></a>
# 第3章 程序的机器级表示

> **本章地位**：全书核心章节，考试分值占比约 25%-30%。本章将 C 代码与 CPU 执行的机器指令关联起来——过程调用如何用栈帧实现、分支和循环如何翻译为跳转指令、数组/结构体在内存中如何布局、缓冲区溢出如何发生及如何防御。第4章链接、第5章优化、第8章异常控制流均依赖于本章的机器级视角。

---

## 3.1 机器级程序基础

<a id="sec3-1-1"></a>
### 3.1.1 机器指令及汇编指令 ★★★

**📌 考点概括**：机器指令是 CPU 能直接执行的二进制编码；汇编指令是机器指令的文本助记符，两者一一对应（双射关系）。

**📖 详细讲解**

```
机器指令（十六进制）： 01 D8
汇编指令（文本）：     ADD EAX, EBX
含义：                 EAX ← EAX + EBX
```

翻译方向：汇编指令 —汇编器→ 机器指令 —反汇编器→ 汇编指令。这种一一对应使反汇编总是可行的——与高级语言编译不同，一条 C 语句可对应多条机器指令，但一条汇编指令只产生一条机器指令。

**✏️ 真题训练**

> **题1**：以下说法错误的是？ A. 一条汇编指令对应唯一一条机器指令 B. 一条C语句可能对应多条机器指令 C. 反汇编器可还原汇编指令 D. 同一机器码在所有平台上相同
> **答案**：D。不同 ISA（x86、ARM）的机器指令格式完全不同。

---

<a id="sec3-1-2"></a>
### 3.1.2 指令集体系结构 ★★★

**📌 考点概括**：ISA 定义程序员可见的机器抽象——包含指令集、寄存器组、寻址方式、数据类型。ISA 是**硬件/软件的分界面**。

**📖 详细讲解**

| 组成部分 | IA-32 实例 |
|---------|-----------|
| 指令集 | MOV, ADD, JMP, CALL 等 |
| 寄存器组 | EAX/EBX/ECX/EDX/ESI/EDI/EBP/ESP + EIP + EFLAGS |
| 寻址方式 | 立即、寄存器、直接、间接、基址、变址、基址+变址、比例变址（共8种） |
| 数据类型 | byte(8)/word(16)/dword(32)/qword(64) |

同一 ISA 可有不同微架构实现（Intel Core vs AMD Ryzen 都实现 x86 ISA），ISA 抽象使软件兼容成为可能。流水线深度、缓存大小等微架构细节对程序员不可见，不属于 ISA。

**✏️ 真题训练**

> **题2**：ISA 通常不包括？A. 指令编码格式 B. 寄存器集合 C. 流水线深度 D. 寻址方式
> **答案**：C。流水线深度属微架构实现细节。

---

<a id="sec3-1-3"></a>
### 3.1.3 生成机器代码的过程 ★★

**📌 考点概括**：C 源码到可执行文件经历预处理→编译→汇编→链接四个阶段。本章聚焦**编译阶段**——C 结构如何翻译为汇编指令。

**📖 详细讲解**

```
hello.c ──[预处理 cpp]──▶ hello.i ──[编译 cc1]──▶ hello.s ──[汇编 as]──▶ hello.o ──[链接 ld]──▶ hello
```

| 阶段 | 输入 | 输出 | 做了什么 |
|------|------|------|---------|
| 预处理 | `.c` | `.i` | 宏展开、#include插入 |
| 编译 | `.i` | `.s` | C → 汇编（含优化） |
| 汇编 | `.s` | `.o` | 汇编→机器码（一对一） |
| 链接 | `.o`+库 | 可执行文件 | 符号解析+重定位 |

详见 [第1章编译链接四阶段](./ch01-overview.md#sec1-2-2)。

---

## 3.2 IA-32 指令系统基础

<a id="sec3-2-1"></a>
### 3.2.1 数据类型及其格式 ★★

**📌 考点概括**：IA-32 按宽度分 byte(1B)/word(2B)/dword(4B)/qword(8B)；多字节采用小端存储（低字节→低地址）。

**📖 详细讲解**

```
值 0x12345678 在地址 0x100 处的小端存储：
地址:  0x100   0x101   0x102   0x103
内容:   78      56      34      12
        ↑低字节(低地址)     ↑高字节(高地址)
```

记忆口诀："小端 = 低低对齐"（低字节→低地址）。端序基础概念参见[第2章端序](./ch02-data-representation.md#sec2-6-2)。

**✏️ 真题训练**

> **题3**：小端机上，32位整数 0xDEADBEEF 在 0x1000 处，地址 0x1002 的字节内容是？A. DE B. AD C. BE D. EF
> **答案**：B。小端存储：0x1000→EF, 0x1001→BE, 0x1002→AD, 0x1003→DE。

---

<a id="sec3-2-2"></a>
### 3.2.2 寄存器组织和寻址方式 ★★★

**📌 考点概括**：8 个通用寄存器 + 6 个段寄存器 + EIP + EFLAGS；8 种寻址方式及有效地址计算公式。

**📖 详细讲解**

**通用寄存器 ASCII 框图**：

```
┌──────────────────────────────────────────────┐
│           IA-32 通用寄存器 (32位)              │
│                                               │
│  EAX ┌──────────────┬──────────────────┐      │
│      │     AX(16)   │                  │      │
│      ├──────┬───────┤    累加器/返回值   │      │
│      │ AH(8)│ AL(8) │                  │      │
│      └──────┴───────┘                  │      │
│  EBX ┌──────┬───────┐  基址寄存器       │      │
│  ECX ┌──────┬───────┐  计数器/移位量    │      │
│  EDX ┌──────┬───────┐  数据/除法余数    │      │
│  ESI ───────────────  源变址            │      │
│  EDI ───────────────  目的变址          │      │
│  EBP ───────────────  栈帧基址指针      │      │
│  ESP ───────────────  栈顶指针          │      │
│                                               │
│  EIP ───────────────────  指令指针(下一条指令地址)  │
│  EFLAGS ┌──┬──┬──┬──┬──┬──┐                 │
│          │OF│SF│ZF│CF│...│                  │
│          └──┴──┴──┴──┴──┴──┘                 │
└──────────────────────────────────────────────┘
```

**EFLAGS 关键标志位**：OF(溢出)、SF(符号)、ZF(零)、CF(进位/借位)。

**8种寻址方式**：

| 编号 | 寻址方式 | 格式 | EA 计算公式 |
|------|---------|------|-----------|
| ① | 立即寻址 | `$Imm` | 操作数=Imm |
| ② | 寄存器寻址 | `Reg` | 操作数=Reg内容 |
| ③ | 直接寻址 | `[Addr]` | EA=Addr |
| ④ | 寄存器间接 | `[Reg]` | EA=Reg |
| ⑤ | 基址寻址 | `[Reg+disp]` | EA=Reg+disp |
| ⑥ | 变址寻址 | `[Reg1+Reg2]` | EA=Reg1+Reg2 |
| ⑦ | 基址+变址 | `[Reg1+Reg2+disp]` | EA=Reg1+Reg2+disp |
| ⑧ | 比例变址 | `[Base+Index*S+disp]` | EA=Base+Index×S+disp |
> S ∈ {1, 2, 4, 8}

**🔍 举例推演**

```asm
; 已知 EBX=0x100, ESI=0x10, ECX=4
MOV EAX, [EBX + ESI*4 + 0x20]
; EA = 0x100 + 0x10*4 + 0x20 = 0x100 + 0x40 + 0x20 = 0x160
; EAX = 内存[0x160]的值
```

**✏️ 真题训练**

> **题4**：以下寻址方式中不涉及内存访问的是？A. 直接寻址 B. 寄存器间接 C. 寄存器寻址 D. 基址变址
> **答案**：C。操作数在寄存器内，不产生总线访问。

---

<a id="sec3-2-3"></a>
### 3.2.3 机器指令格式 ★★

**📌 考点概括**：IA-32 采用不定长编码（1~15 字节），格式为：前缀(可选)+操作码+ModR/M+SIB+位移+立即数。

**📖 详细讲解**

```
┌────────┬────────┬────────┬─────┬──────────┬──────────┐
│ 前缀   │ 操作码  │ ModR/M │ SIB │ 位移     │ 立即数    │
│ 0~4B   │ 1~3B   │ 0~1B   │ 0~1B│ 0/1/2/4B │ 0/1/2/4B │
└────────┴────────┴────────┴─────┴──────────┴──────────┘
```

- ModR/M 编码寻址方式和寄存器（Mod 2位 + Reg 3位 + R/M 3位）
- SIB 支持比例寻址（Scale 2位 + Index 3位 + Base 3位）
- 正因不定长，x86 译码器极其复杂；而 ARM/RISC-V 定长32位编码译码简单。

---

## 3.3 常用汇编指令

<a id="sec3-3-1"></a>
### 3.3.1 传送指令 ★★★

**📌 考点概括**：MOV（禁内存→内存）、LEA（传地址不访存）、PUSH/POP（自动调整 ESP）。

**📖 详细讲解**

**MOV 限制**：

| 合法 | 非法 |
|------|------|
| `MOV Reg, Imm` / `MOV Reg, Reg` | `MOV Mem, Mem` ← 绝不允许！ |
| `MOV Reg, Mem` / `MOV Mem, Reg` | 若需 Mem→Mem，必须通过寄存器中转 |
| `MOV Mem, Imm` | — |

**LEA vs MOV**：

```asm
; 假设 EBX=0x1000, ESI=4
LEA EAX, [EBX + ESI*4 + 8]   ; EAX = 0x1000+16+8 = 0x1018 (算地址，不访存!)
MOV EAX, [EBX + ESI*4 + 8]   ; EAX = 内存[0x1018] (读取内存)
```

LEA 常用于快速算术（如 `LEA EAX, [EBX*2+EBX]` 得 EAX=EBX×3）。

**PUSH/POP**：

```
PUSH Reg ≡ SUB ESP, 4; MOV [ESP], Reg    (ESP下移，写入值)
POP  Reg ≡ MOV Reg, [ESP]; ADD ESP, 4    (读出值，ESP上移)
```

**🔍 举例推演**

```c
int a=10, b=20; int c=a+b;
```

```asm
MOV DWORD PTR [EBP-4], 10      ; a = 10
MOV DWORD PTR [EBP-8], 20      ; b = 20
MOV EAX, DWORD PTR [EBP-4]     ; EAX = a
ADD EAX, DWORD PTR [EBP-8]     ; EAX += b
MOV DWORD PTR [EBP-12], EAX    ; c = EAX
```

**✏️ 真题训练**

> **题5**：非法指令是？A. `MOV EAX,[EBX]` B. `MOV [ESI],EDI` C. `MOV [0x100],[0x200]` D. `MOV AL,0x41`
> **答案**：C。两个操作数都在内存中。  
> **题6**：`LEA EAX,[EBX+ECX*2]` 后 EAX 存的是？A. 内存值 B. EBX+ECX×2 C. EBX D. 不确定
> **答案**：B。

---

<a id="sec3-3-2"></a>
### 3.3.2 定点算术运算指令 ★★★

**📌 考点概括**：ADD/SUB 双操作数；IMUL 有 1/2/3 操作数形式；IDIV 被除数在 EDX:EAX，商→EAX、余数→EDX，除法前必须 CDQ 符号扩展。INC/DEC 不修改 CF。

**📖 详细讲解**

| 指令 | 关键要点 |
|------|---------|
| ADD/SUB | `ADD Dst,Src` / `SUB Dst,Src`，影响 OF/SF/ZF/CF |
| INC/DEC | 操作数+1/-1，**不影响 CF**（考试常考！） |
| IMUL | 单操作数：结果在 DX:AX 或 EDX:EAX；双操作数：`IMUL Reg,Src`；三操作数：`IMUL Reg,Src,Imm` |
| IDIV | 被除数隐式在 EDX:EAX(64位)，`IDIV Src` → EAX=商, EDX=余数 |
| CDQ | 将 EAX 符号扩展到 EDX:EAX，**IDIV 前必须执行！** |
| NEG | 取负，影响 OF/SF/ZF/CF |

**🔍 举例推演**

```c
int a=100, b=7; int q=a/b; int r=a%b;
```

```asm
MOV  EAX, 100       ; 被除数低32位
CDQ                  ; EDX:EAX = 100 (符号扩展, EDX=0)
MOV  ECX, 7
IDIV ECX            ; EAX=14(商), EDX=2(余数)
MOV  [q], EAX       ; q = 14
MOV  [r], EDX       ; r = 2
```

**✏️ 真题训练**

> **题7**：关于 INC 指令，错误的是？A. 不修改 CF B. 会影响 ZF C. 可带两个操作数 D. 操作数+1
> **答案**：C。INC 是单操作数指令。  
> **题8**：IDIV 前缺 CDQ 的后果是？A. 编译错误 B. EDX 为随机值导致商错误 C. 自动补0 D. 溢出异常
> **答案**：B。EDX:EAX 是被除数(64位)，EDX 如果未正确设置，除法结果错误。

---

<a id="sec3-3-3"></a>
### 3.3.3 按位运算指令 ★★

**📌 考点概括**：AND/OR/XOR/NOT 按位逻辑；TEST 做 AND 只设标志位不保存；移位 SHL/SHR/SAR 的移位量只能为 1 或 CL。

**📖 详细讲解**

| 指令 | 效果 | 注意 |
|------|------|------|
| AND/OR/XOR | 按位与/或/异或 | `XOR EAX,EAX` 清零（比 `MOV EAX,0` 快） |
| NOT | 按位取反 | 单操作数 |
| TEST | Dst & Src，设 ZF/SF/PF，**丢弃结果** | 测试某位用 TEST，不破坏原值 |
| SHL/SHR/SAR | 移位 | **Count 只能是 1 或 CL！** |
| SHL | 逻辑左移，右补0 | 左移n位 = 乘 2ⁿ |
| SHR | 逻辑右移，左补0 | 无符号数右移 |
| SAR | 算术右移，左补符号位 | 有符号数右移（保持正负） |

**✏️ 真题训练**

> **题9**：不破坏 EAX 前提下测试 bit3 是否为1，最佳指令？A. AND EAX,8 B. TEST EAX,8 C. CMP EAX,8 D. OR EAX,8
> **答案**：B。TEST 只设标志位不修改操作数。  
> **题10**：移位指令的正确用法？A. `SHL EAX,4` B. `SAR EAX,CL` C. `SHR EAX,EAX` D. `SHL EAX,5`
> **答案**：B。移位量只能是1或CL。

---

<a id="sec3-3-4"></a>
### 3.3.4 控制转移指令 ★★★

**📌 考点概括**：CMP+Jcc 实现条件分支；JMP 无条件跳转；CALL 压返回地址/RET 弹返回地址。条件跳转用 below/above 表无符号比较，less/greater 表有符号比较（标志位原理见 [第2章补码比较](./ch02-data-representation.md#sec2-7-4)）。

**📖 详细讲解**

**CMP** = 做 SUB 只设标志位，不保存结果。

**Jcc 条件跳转**：

| 有符号 | 无符号 | 条件 | 标志位条件 |
|--------|--------|------|-----------|
| JE/JZ | JE/JZ | == / 结果=0 | ZF=1 |
| JNE/JNZ | JNE/JNZ | != | ZF=0 |
| JL/JNGE | JB/JNAE | < | SF≠OF / CF=1 |
| JLE/JNG | JBE/JNA | ≤ | ZF=1或SF≠OF / CF=1或ZF=1 |
| JG/JNLE | JA/JNBE | > | ZF=0且SF=OF / CF=0且ZF=0 |
| JGE/JNL | JAE/JNB | ≥ | SF=OF / CF=0 |

**CALL/RET**：

```
CALL target ≡ PUSH 下一条指令地址; JMP target
RET        ≡ POP EIP
RET N      ≡ POP EIP; ADD ESP, N    (stdcall, 被调用者清理参数)
```

**🔍 举例推演**

```c
if (a > b) c = a; else c = b;
```

```asm
MOV  EAX, [a]
CMP  EAX, [b]        ; a - b, 设标志位
JLE  ELSE_BLOCK      ; a<=b 则跳到 else
MOV  [c], EAX        ; c = a
JMP  EXIT
ELSE_BLOCK:
MOV  EBX, [b]
MOV  [c], EBX        ; c = b
EXIT:
```

**✏️ 真题训练**

> **题11**：无符号比较 `if(a>=b)` 应用？A. JL B. JGE C. JB D. JAE
> **答案**：D。无符号≥ 用 JAE（CF=0）。  
> **题12**：CALL 执行时压栈的是？A. 目标函数地址 B. CALL下一条指令地址 C. EBP D. EFLAGS
> **答案**：B。CALL 压入返回地址（CALL 下一条指令的 EIP）。

---

## 3.4 过程调用与程序结构

<a id="sec3-4-1"></a>
### 3.4.1 过程调用的机器级表示 ★★★

**📌 考点概括**：栈帧是过程调用的核心数据结构。必须能画出栈帧结构图、计算参数和局部变量偏移量、区分 cdecl/stdcall/fastcall 三种调用约定的栈清理规则。

**📖 详细讲解**

**栈帧结构**（逐行增长方向，[第1章编译链接](./ch01-overview.md#sec1-2-2) 回顾编译→汇编→链接流程）：

```
              高地址
┌──────────────────────────┐
│  调用者栈帧               │
├──────────────────────────┤
│  参数n                    │ ← EBP + 8 + 4*(n-1)
│  ...                     │
│  参数2                    │ ← EBP + 12
│  参数1                    │ ← EBP + 8
├──────────────────────────┤
│  返回地址 (CALL自动压入)   │ ← EBP + 4
├──────────────────────────┤
│  旧EBP (PUSH EBP保存)     │ ← EBP (当前帧基址, EBP指向此处)
├──────────────────────────┤
│  局部变量1                │ ← EBP - 4
│  局部变量2                │ ← EBP - 8
│  ...                     │
├──────────────────────────┤
│  被调用者保存寄存器(可选)  │
├──────────────────────────┤
│  调用参数区(为调用其他函数)│
├──────────────────────────┤
│                          │ ← ESP (栈顶)
└──────────────────────────┘
              低地址

规矩：参数用 [EBP + 正偏移]，局部变量用 [EBP - 负偏移]。
```

**函数标准序言和结语**：

```asm
; ===== 序言 (Prologue) =====
PUSH EBP                ; 保存调用者栈帧基址
MOV  EBP, ESP           ; 建立自己的栈帧基址
SUB  ESP, N             ; 为局部变量分配N字节

; ===== 函数体 =====
; [EBP+8]=参数1, [EBP+12]=参数2, [EBP-4]=局部变量1...

; ===== 结语 (Epilogue) =====
MOV  ESP, EBP           ; 释放局部变量 (或 LEAVE)
POP  EBP                ; 恢复调用者EBP
RET                     ; 返回 (cdecl: 调用者清理参数)
```

或使用 LEAVE 指令代替 `MOV ESP,EBP; POP EBP`。

**三种调用约定**：

| 特征 | cdecl | stdcall | fastcall |
|------|-------|---------|----------|
| 参数入栈顺序 | 右→左 | 右→左 | ECX,EDX传前2个, 其余右→左入栈 |
| 栈清理者 | **调用者** | **被调用者**(RET N) | 被调用者 |
| 可变参数 | ✅ | ❌ | ❌ |
| 使用场景 | C默认 | Win32 API | 性能优化 |

**🔍 举例推演——完整函数调用**

```c
int sum(int x, int y) {
    int s = x + y;
    return s;
}
int main() {
    int a=10, b=20;
    int r = sum(a, b);
    return 0;
}
```

```asm
; ===== sum (cdecl) =====
_sum:
    PUSH EBP
    MOV  EBP, ESP
    SUB  ESP, 4               ; 为局部变量 s 分配4字节
    MOV  EAX, [EBP+8]         ; EAX = x (参数1)
    ADD  EAX, [EBP+12]        ; EAX += y (参数2)
    MOV  [EBP-4], EAX         ; s = EAX
    ; 返回值已在 EAX 中
    MOV  ESP, EBP             ; 或 LEAVE
    POP  EBP
    RET

; ===== main =====
_main:
    PUSH EBP
    MOV  EBP, ESP
    SUB  ESP, 12              ; a, b, r (3×4=12字节)
    MOV  DWORD PTR [EBP-4], 10
    MOV  DWORD PTR [EBP-8], 20
    ; 调用 sum(a, b)
    PUSH DWORD PTR [EBP-8]    ; b (右→左，先压b)
    PUSH DWORD PTR [EBP-4]    ; a
    CALL _sum
    ADD  ESP, 8               ; cdecl: 调用者清理8字节参数
    MOV  [EBP-12], EAX        ; r = 返回值
    XOR  EAX, EAX             ; return 0
    MOV  ESP, EBP
    POP  EBP
    RET
```

**✏️ 真题训练**

> **题13**：cdecl 约定下，函数 `foo(int a,int b,int c)` 被调用时参数在栈中从低地址到高地址是？A. a,b,c,返回地址 B. 返回地址,a,b,c C. 返回地址,c,b,a D. c,b,a,返回地址
> **答案**：B。右→左压栈：c最先入栈(高地址), a最后入栈(低地址)；CALL再压返回地址(最低地址)。  
> **题14**：分析下列代码推算参数个数和局部变量个数：
> ```asm
> PUSH EBP
> MOV  EBP, ESP
> SUB  ESP, 16
> MOV  EAX, [EBP+8]
> ADD  EAX, [EBP+12]
> MOV  [EBP-4], EAX
> MOV  [EBP-8], EBX
> ```
> A. 1参数,2局部变量 B. 2参数,2局部变量 C. 2参数,4局部变量 D. 4参数,2局部变量
> **答案**：B。访问了 [EBP+8] 和 [EBP+12]→2个参数；使用了[EBP-4]和[EBP-8]→2个局部变量（SUB ESP,16分配了16字节但实际只用8字节）。

---

<a id="sec3-4-2"></a>
### 3.4.2 选择语句的机器级表示 ★★★

**📌 考点概括**：if-else 编译为 CMP+Jcc；switch 密集 case 时用跳转表实现 O(1) 间接跳转。if-else 链 O(n)，跳转表 O(1)。

**📖 详细讲解**

**if-else 编译模式**：

```asm
    计算条件, 设标志位
    Jcc  ELSE_LABEL         ; 条件不成立 → 跳到 else
    ; then 分支代码
    JMP  END_LABEL
ELSE_LABEL:
    ; else 分支代码
END_LABEL:
```

**switch 跳转表**（case 密集时）：

```c
switch (x) {
    case 1: y=10; break;
    case 2: y=20; break;
    case 3: y=30; break;
    case 4: y=40; break;
    default: y=0; break;
}
```

```asm
    CMP  EAX, 4             ; 范围检查
    JA   DEFAULT
    SUB  EAX, 1             ; 归一化 x-1 → 索引(0~3)
    JMP  [JumpTable + EAX*4]; 间接跳转 O(1)!

JumpTable:                  ; 在 .rodata 段
    DD  CASE1               ; 索引0 → case 1
    DD  CASE2               ; 索引1 → case 2
    DD  CASE3
    DD  CASE4
```

**效率对比**：if-else 链 O(n)，跳转表 O(1)。编译器自动选择：case 密集→跳转表；case 稀疏→if-else 链。

**✏️ 真题训练**

> **题15**：switch 编译为跳转表的前提是？A. case>10个 B. case值密集 C. 有default D. case为偶数
> **答案**：B。  
> **题16**：逆向汇编：
> ```asm
> CMP EAX,2; JE L1; CMP EAX,4; JE L2; CMP EAX,6; JE L3; JMP DEFAULT
> L1: MOV EAX,1; JMP END
> L2: MOV EAX,2; JMP END
> L3: MOV EAX,3; JMP END
> DEFAULT: XOR EAX,EAX
> ```
> 对应 C 代码为？A. 跳转表switch B. if-else链switch（case:2,4,6） C. 嵌套if D. while循环
> **答案**：B。case 值稀疏(2,4,6)，编译为 if-else 链。

---

<a id="sec3-4-3"></a>
### 3.4.3 循环结构的机器级表示 ★★

**📌 考点概括**：do-while/while/for 最终归约为条件测试+条件跳转。do-while 最接近机器执行（先执行后判断）；while/for 需额外跳转实现"先判断"。

**📖 详细讲解**

```asm
; === do-while (先执行, 后判断) ===
LOOP:  /* 循环体 */; CMP ...; Jxx LOOP

; === while (先判断, 后执行) ===
    JMP TEST
LOOP: /* 循环体 */
TEST: CMP ...; Jxx LOOP

; === for 同 while ===
    MOV ECX, 0; JMP TEST
LOOP: /* 循环体 */; INC ECX
TEST: CMP ECX, N; JL LOOP
```

**🔍 举例推演**

```c
int sum=0;
for (int i=1; i<=100; i++) sum += i;
```

```asm
    XOR  EAX, EAX          ; sum = 0
    MOV  ECX, 1            ; i = 1
    JMP  TEST
LOOP:
    ADD  EAX, ECX          ; sum += i
    INC  ECX               ; i++
TEST:
    CMP  ECX, 100
    JLE  LOOP              ; i<=100 继续
; 循环结束, EAX = 5050
```

---

## 3.5 数据结构的机器级表示

<a id="sec3-5-1"></a>
### 3.5.1 数组的分配和访问 ★★★

**📌 考点概括**：数组连续存储、行优先布局。一维 `&A[i]=A+i*sizeof(element)`；二维 `&A[i][j]=A+(i×N+j)×sizeof(element)`（N=列数）。

**📖 详细讲解**

```c
int A[2][3] = {{1,2,3},{4,5,6}};
```

行优先内存布局：

```
地址:  0x1000  0x1004  0x1008  0x100C  0x1010  0x1014
       A[0][0] A[0][1] A[0][2] A[1][0] A[1][1] A[1][2]
        1       2       3       4       5       6
```

汇编访问：

```asm
; A[i][j], i在EAX, j在EBX, 列数N=3
IMUL ECX, EAX, 3        ; ECX = i * 3
ADD  ECX, EBX           ; ECX = i*3 + j
MOV  EDX, [A + ECX*4]   ; EDX = A[i][j]
```

行优先遍历（先j后i）具有更好的空间局部性——相邻元素地址连续，缓存命中率高。

**✏️ 真题训练**

> **题17**：`short B[5][6]` (short=2B)，B[3][4] 相对首地址偏移？A. 20 B. 22 C. 44 D. 34
> **答案**：C。偏移 = (3×6+4)×2 = 22×2 = 44。

---

<a id="sec3-5-2"></a>
### 3.5.2 结构体数据的分配和访问 ★★★

**📌 考点概括**：结构体按声明顺序分配，成员满足自身对齐要求，尾部填充至最大成员对齐的倍数。sizeof 计算是高频考点。

**📖 详细讲解**

**对齐规则**：① 成员起始偏移 ≡ 0 (mod 自身大小)；② 总大小 ≡ 0 (mod 最大成员大小)。

```c
struct S1 {
    char  a;     // 1字节
    int   b;     // 4字节
    short c;     // 2字节
};
// sizeof = 12
// 布局: |a|XX|XX|XX| b(4B) | c(2B) |XX|XX|
// 偏移   0  1  2  3   4~7     8~9   10~11
// 解释: a占偏移0; 填充3B使b从偏移4开始; c占8~9; 尾部填充2B使总大小12=4的倍数
```

**成员顺序影响大小**：

```c
struct S2 { char a; short c; int b; };
// sizeof = 8 (更紧凑)
// 布局: |a|c|c|XX| b(4B) |
```

汇编访问：

```asm
; struct Point { int x; int y; } p;
; p.x = 10; p.y = 20;
MOV DWORD PTR [EBP-8], 10   ; p.x (偏移0)
MOV DWORD PTR [EBP-4], 20   ; p.y (偏移4)
```

**✏️ 真题训练**

> **题18**：IA-32/Linux 下 `sizeof(struct{char a; double b; short c;})` = ? A. 11 B. 16 C. 24 D. 32
> **答案**：C。a偏移0(1B)→填充7B(使b从8开始)→b占8B(8~15)→c偏移16(2B)→尾部填充6B(总大小24为8的倍数)。

---

<a id="sec3-5-3"></a>
### 3.5.3 联合体数据的分配和访问 ★★

**📌 考点概括**：联合体所有成员共享同一块内存，sizeof = max(成员大小)，对齐到最大基本类型。

**📖 详细讲解**

```c
union Data { int i; float f; char s[5]; };
// sizeof = 8 (max=5, 对齐到int/float的4→8)
```

内存布局：所有成员从偏移0开始共用空间。任一时刻只有一个成员有效。

| 特性 | struct | union |
|------|--------|-------|
| sizeof | ≥各成员之和(含填充) | =max(成员), 对齐后 |
| 同时有效 | 全部有效 | 仅一个有效 |

**✏️ 真题训练**

> **题19**：小端机上 `union{int a; char b[2];}u; u.a=0x12345678; printf("%x",u.b[0]);` 输出？A. 12 B. 34 C. 56 D. 78
> **答案**：D。小端存储 0x78 在最低地址，b[0] 读取该地址字节。

---

<a id="sec3-5-4"></a>
### 3.5.4 数据的对齐 ★★

**📌 考点概括**：对齐为了内存访问效率——不对齐需要多次总线传输并拼接数据，性能下降。`#pragma pack` 可改变对齐规则。

**📖 详细讲解**

**为什么对齐？**

```
32位总线一次读4B:
对齐访问(0x1004): |xx|xx|xx|xx|A0|A1|A2|A3| → 一次读完
非对齐(0x1005):   |xx|xx|xx|xx|xx|A0|A1|A2| + |A3|xx|xx|xx| → 两次读取+拼接!
```

某些 RISC 架构（SPARC）非对齐访问直接触发硬件异常。

`#pragma pack(1)` 取消对齐，压缩大小但降低访问效率。`.rodata` 节中的跳转表需遵循对齐，参见 [第4章 ELF 节](./ch04-linking.md#sec4-2-2)。

**✏️ 真题训练**

> **题20**：关于对齐，错误的是？A. 对齐提高访存效率 B. 某些架构强制对齐 C. 对齐会增加结构体大小 D. `#pragma pack(1)` 不能减小结构体
> **答案**：D。

---

## 3.6 缓冲区溢出与安全

<a id="sec3-6-1"></a>
### 3.6.1 缓冲区溢出 ★★★

**📌 考点概括**：向固定大小栈缓冲区写入超长数据，覆盖相邻内存（特别是返回地址），导致程序行为异常或安全漏洞。

**📖 详细讲解**

```c
void echo() {
    char buf[8];        // 8字节栈缓冲区
    gets(buf);          // 危险！不限制输入长度
    puts(buf);
}
```

溢出过程示意：

```
正常栈帧:              输入超长字符串后:
高地址                 高地址
┌──────────┐          ┌──────────┐
│ 返回地址  │          │ 0xDEAD   │ ← 被覆盖! RET跳转到攻击者指定地址
├──────────┤          ├──────────┤
│ 旧EBP    │          │ "CCCC"   │ ← 被覆盖!
├──────────┤          ├──────────┤
│ buf[4~7] │          │ "BBBB"   │
│ buf[0~3] │          │ "AAAA"   │
├──────────┤          ├──────────┤
│          │ ← ESP    │          │
└──────────┘          └──────────┘
低地址                 低地址
```

> 返回地址被覆盖后的重定位问题，参见 [第4章重定位](./ch04-linking.md#sec4-4-2)。

---

<a id="sec3-6-2"></a>
### 3.6.2 缓冲区溢出攻击 ★★

**📌 考点概括**：攻击载荷 = shellcode + 覆盖返回地址为 shellcode 入口。NX 位阻止栈执行时，ROP 攻击利用程序中已有的代码片段（gadget）绕过。

**📖 详细讲解**

**经典 shellcode 注入**：

```
攻击载荷: [NOP sled...][shellcode][shellcode地址×N]
                                         ↑ 覆盖返回地址
函数RET → EIP = shellcode地址 → 执行shellcode → execve("/bin/sh")
```

**ROP 攻击**：NX 使栈不可执行后，攻击者不再注入代码，而是串联程序中已有的以 RET 结尾的指令片段（gadget）：

```
ROP链(栈上): | gadget1地址 | gadget2地址 | gadget3地址 | ...
              ↑ 返回地址
每个gadget执行后RET弹出下一个gadget地址，形成控制流劫持。
```

---

<a id="sec3-6-3"></a>
### 3.6.3 缓冲区溢出攻击的防范 ★★★

**📌 考点概括**：三大防御机制——ASLR（地址随机化）、Stack Canary（金丝雀值检测）、NX/XD（栈不可执行）。必须掌握各机制的**局限性**。

**📖 详细讲解**

| 防御机制 | 原理 | 防御什么 | **局限性** |
|---------|------|---------|-----------|
| **ASLR** | 每次运行栈/堆/库基址随机化 | 硬编码地址攻击 | 32位熵不足可暴力破解；信息泄露可绕过 |
| **Stack Canary** | 返回地址前插入随机值，返回前校验 | 栈溢出覆盖返回地址 | 不保护非栈缓冲区；不防御修改局部变量攻击；信息泄露可获取canary |
| **NX/XD** | 栈页标记不可执行 | shellcode注入 | ROP攻击可绕过(复用已有代码) |
| **PIE** | 代码段基址随机化 | ROP固定gadget地址 | 需信息泄露配合绕过 |

**Stack Canary 工作流程**：

```
函数入口: 从全局随机源取canary放入栈 (在旧EBP与局部变量之间)
函数返回前: 比较栈上canary与全局值
  → 匹配: 正常返回
  → 不匹配: 调用 __stack_chk_fail() 终止程序
```

**编译选项**：`-fno-stack-protector`(关canary)、`-z execstack`(允许栈执行)、`-no-pie`(关PIE)。

**✏️ 真题训练**

> **题21**：Stack Canary 不能防御？A. 覆盖返回地址 B. 覆盖函数指针变量 C. 修改局部变量(如is_admin=1) D. 覆盖旧EBP
> **答案**：C。canary只检测是否有人"翻围墙"去改返回地址；不越过canary的局部变量修改无法触发检测。  
> **题22**：ROP攻击主要绕过？A. Canary B. ASLR C. NX D. PIE
> **答案**：C。ROP不注入新代码，复用已有代码片段跳过执行限制。  
> **题23**：最强防护组合是？A. ASLR+NX B. ASLR+NX+Canary+PIE C. Canary D. NX
> **答案**：B。纵深防御：各层互补。

---

## 本章速查表

### 通用寄存器

| 寄存器 | 用途 | 16位 | 高8位 | 低8位 |
|--------|------|------|-------|-------|
| EAX | 累加器/返回值 | AX | AH | AL |
| EBX | 基址寄存器 | BX | BH | BL |
| ECX | 计数器/移位量 | CX | CH | CL |
| EDX | 数据/除法余数 | DX | DH | DL |
| ESI | 源变址 | SI | — | — |
| EDI | 目的变址 | DI | — | — |
| EBP | 栈帧基址指针 | BP | — | — |
| ESP | 栈顶指针 | SP | — | — |
| EIP | 指令指针 | — | — | — |
| EFLAGS | 标志寄存器(OF/SF/ZF/CF) | — | — | — |

### 常用指令速查

| 类别 | 指令 | 关键点 |
|------|------|--------|
| 传送 | MOV | 禁Mem→Mem |
| 取地址 | LEA | 仅计算地址，不访存 |
| 栈操作 | PUSH/POP | ESP自动±4 |
| 加法 | ADD | Dst+=Src |
| +1/-1 | INC/DEC | **不影响CF** |
| 乘法 | IMUL | 1/2/3操作数形式 |
| 除法 | IDIV | 被除数在EDX:EAX，**先CDQ** |
| 测试 | TEST | AND后只设标志，丢弃结果 |
| 移位 | SHL/SHR/SAR | 移位量**只能是1或CL** |
| 比较 | CMP | SUB后只设标志 |
| 调用/返回 | CALL/RET | CALL压返回地址；RET弹到EIP |

### 调用约定

| 特征 | cdecl | stdcall | fastcall |
|------|-------|---------|----------|
| 参数顺序 | 右→左 | 右→左 | ECX,EDX传前2个 |
| 谁清栈 | 调用者 | 被调用者(RET N) | 被调用者 |
| 可变参数 | ✅ | ❌ | ❌ |

### 关键公式

| 用途 | 公式 |
|------|------|
| 有效地址 EA | Base + Index×S(1/2/4/8) + Disp |
| 第k个参数地址 | EBP + 8 + (k-1)×4 |
| 局部变量m地址 | EBP - 4×m |
| 数组 A[i][j] 地址 | A + (i×N + j)×sizeof(element) |
| sizeof(struct) | 成员和+成员间填充+尾部填充(对齐最大成员) |
| sizeof(union) | max(成员大小), 对齐到最大基本类型 |

### 函数模板

```asm
; 序言
PUSH EBP
MOV  EBP, ESP
SUB  ESP, local_size

; 结语 (cdecl)
MOV  ESP, EBP        ; 或 LEAVE
POP  EBP
RET
```
