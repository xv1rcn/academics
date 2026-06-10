# IA-32 汇编速查手册 + 练习册

> **用途**：考前快速查阅寄存器/指令/控制流映射，并完成 C↔汇编双向翻译练习。
> **语法约定**：本手册正文统一使用 **Intel 语法**（无 `%` 前缀、`[ ]` 表示内存地址）。`gcc -S` 和 `objdump -d` 默认输出 AT&T 语法（有 `%`/`$`、用 `( )`），查阅[寻址方式对比表](#三寻址方式8-种)可快速互译。用 `objdump -d -M intel` 可直接输出 Intel 语法。
> 配合 [第3章讲义](./ch03-machine-level.md) 和 [第4章链接](./ch04-linking.md) 使用。

---

## 一、寄存器全图

### 1.1 通用寄存器（8 个，32 位）

```
┌──────────────────────────────────────────────────────────────┐
│  EAX  │ 累加器    │ 函数返回值、算术运算默认操作数            │
│  EBX  │ 基址寄存器 │ 内存寻址基址，需调用者保存（被调函数用前压栈）│
│  ECX  │ 计数器    │ 循环计数(LOOP)、移位量(CL)               │
│  EDX  │ 数据寄存器 │ 乘除辅助(EDX:EAX)、I/O 端口              │
│  ESI  │ 源变址    │ 字符串/内存复制源指针                    │
│  EDI  │ 目的变址  │ 字符串/内存复制目标指针                   │
│  EBP  │ 帧指针    │ 指向当前栈帧基址，访问参数和局部变量       │
│  ESP  │ 栈指针    │ 指向栈顶，PUSH/POP/CALL/RET 自动调整      │
└──────────────────────────────────────────────────────────────┘
```

- 低 16 位可独立访问：AX, BX, CX, DX, SI, DI, BP, SP
- 高低 8 位也可独立：AH/AL, BH/BL, CH/CL, DH/DL（仅前四个）

### 1.2 特殊寄存器

| 寄存器 | 宽度 | 用途 |
|--------|:---:|------|
| **EIP** | 32 | 指令指针，指向**下一条**要执行的指令地址 |
| **EFLAGS** | 32 | 标志位集合（CF/ZF/SF/OF 等） |

### 1.3 段寄存器（6 个，16 位）

| 寄存器 | 默认用途 |
|--------|----------|
| CS | 代码段 |
| DS | 数据段 |
| SS | 栈段 |
| ES/FS/GS | 附加段 |

> IA-32 平坦内存模型下段寄存器通常由 OS 设定，用户程序不直接修改。

### 1.4 EFLAGS 关键标志位

| 标志 | 位 | 含义 | 触发条件 |
|------|:--:|------|----------|
| **CF** | 0 | 进位/借位 | 无符号运算溢出（ADD 时 cout⊕cin） |
| **ZF** | 6 | 零标志 | 结果 = 0 |
| **SF** | 7 | 符号标志 | 结果最高位 = 1（视为负数） |
| **OF** | 11 | 溢出标志 | Cn⊕C(n-1)，补码溢出 |

---

## 二、ELF 各节速查

| 节名 | 存储内容 | 权限 | 占磁盘？ | 例子 |
|------|---------|:---:|:---:|------|
| `.text` | 机器指令 | R+X | 是 | `mov %eax,%ebx` |
| `.rodata` | 只读数据 | R | 是 | `"hello"`, switch 跳转表 |
| `.data` | 已初始化全局/静态变量 | R+W | 是 | `int x=5;` |
| `.bss` | 未初始化全局/静态变量 | R+W | 否 | `int x;`（加载时清零） |
| `.symtab` | 符号表 | — | 是 | 函数名、变量名→地址映射 |
| `.rel.text` | .text 的重定位条目 | — | 是 | 链接时需要修正的代码地址 |
| `.rel.data` | .data 的重定位条目 | — | 是 | 链接时需要修正的数据地址 |
| `.strtab` | 字符串表 | — | 是 | 符号名称、"hello"等 |
| `.comment` | 编译器版本信息 | — | 是 | GCC 版本字符串 |

**C 声明 → 节归属速查：**

| C 声明 | 符号类型 | 所在节 |
|--------|:---:|:---:|
| `int x = 10;` | 全局强符号 | `.data` |
| `int y;` | 全局弱符号 | `COMMON` → 链接后 `.bss` |
| `static int z = 5;` | 局部 | `.data` |
| `static int w;` | 局部 | `.bss` |
| `const int c = 7;` | — | `.rodata` |
| `void f(){}` | 函数（强符号） | `.text` |
| `"abc"` | 字符串字面量 | `.rodata` |
| `char *p = "abc";` | p→`.data`, "abc"→`.rodata` |

---

## 三、寻址方式（8 种）

> **两种语法对照**：本手册以 Intel 语法为主（无 `%` 前缀、用 `[ ]`），`gcc -S` / `objdump -d` 默认输出 AT&T 语法（有 `%` `$` 前缀、用 `( )`）。操作方法：`objdump -d -M intel hello.o` 可切换为 Intel 输出。

| # | 方式 | Intel 语法 | AT&T 语法 | 有效地址计算 |
|:--:|------|-----------|-----------|-------------|
| 1 | 立即数 | `100` | `$100` | —（值直接写在指令里） |
| 2 | 寄存器 | `EAX` | `%eax` | —（值在寄存器里） |
| 3 | 直接 | `[0x8048000]` | `0x8048000` | 地址 = 给定常量 |
| 4 | 寄存器间接 | `[EAX]` | `(%eax)` | 地址 = EAX |
| 5 | 基址+位移 | `[EAX + 4]` | `4(%eax)` | 地址 = EAX + 4 |
| 6 | 变址 | `[EAX + EBX]` | `(%eax, %ebx)` | 地址 = EAX + EBX |
| 7 | 基址+变址+位移 | `[EAX + EBX + 4]` | `4(%eax, %ebx)` | 地址 = EAX + EBX + 4 |
| 8 | 比例变址 | `[EAX + EBX*4]` | `(%eax, %ebx, 4)` | 地址 = EAX + EBX×4 |
| 8+ | 完整SIB | `[EAX + EBX*4 + 8]` | `8(%eax, %ebx, 4)` | 地址 = EAX + EBX×4 + 8 |

**翻译规则**：

```
Intel → AT&T： 去 [ ] 换 ( )，寄存器前加 %，立即数前加 $，
               disp(base,index,scale) 顺序有讲究

AT&T → Intel： 去 % $，去 ( ) 换 [ ]，
               disp(base,index,scale) → [base + index*scale + disp]
```

**比例因子** 只能是 1/2/4/8（对应 char/short/int/double 的大小）。`[EBX + ESI*3]` 不合法——`i*3` 必须先用 LEA 单独算出。

---

## 四、指令速查手册

### 4.1 传送指令

| 指令 | 格式 | 功能 | 限制/要点 |
|------|------|------|-----------|
| **MOV** | `MOV dst, src` | dst ← src | **不能内存→内存**；两个操作数不能同为内存 |
| **MOVSX** | `MOVSX dst, src` | 符号扩展后传送 | 小→大，高位补符号位 |
| **MOVZX** | `MOVZX dst, src` | 零扩展后传送 | 小→大，高位补 0 |
| **LEA** | `LEA reg, mem` | reg ← 有效地址 | **不访存**，只算地址；`LEA eax,[ebx+4]` = `eax=ebx+4` |
| **PUSH** | `PUSH src` | ESP←ESP−4; [ESP]←src | 入栈 |
| **POP** | `POP dst` | dst←[ESP]; ESP←ESP+4 | 出栈 |
| **XCHG** | `XCHG op1, op2` | op1↔op2 | 交换 |

**LEA vs MOV 辨析（每年必考）：**

```asm
; 假设 EBX = 0x100，内存 [0x104] = 0xABCD
MOV  EAX, [EBX+4]   ; EAX = 0xABCD（访存，读内存内容）
LEA  EAX, [EBX+4]   ; EAX = 0x104（不访存，纯计算地址）
```

### 4.2 定点算术运算指令

| 指令 | 格式 | 功能 | 要点 |
|------|------|------|------|
| **ADD** | `ADD dst, src` | dst ← dst+src | 双操作数不能同为内存 |
| **SUB** | `SUB dst, src` | dst ← dst−src | 同上 |
| **INC** | `INC dst` | dst ← dst+1 | **不修改 CF** |
| **DEC** | `DEC dst` | dst ← dst−1 | **不修改 CF** |
| **NEG** | `NEG dst` | dst ← −dst | 取负（补码取反+1） |
| **IMUL** | `IMUL src` | EDX:EAX ← EAX×src | 单操作数格式 |
| | `IMUL reg, src` | reg ← reg×src | 双操作数，结果截断 |
| | `IMUL reg, src, imm` | reg ← src×imm | 三操作数 |
| **MUL** | `MUL src` | 同 IMUL 但无符号 | |
| **IDIV** | `IDIV src` | EAX←商, EDX←余 | 被除数在 EDX:EAX |
| **DIV** | `DIV src` | 同 IDIV 但无符号 | |
| **CMP** | `CMP op1, op2` | op1−op2，只设标志位 | 减法但不保存结果，**为 Jcc 做准备** |
| **TEST** | `TEST op1, op2` | op1 & op2，只设标志位 | 常用于判断某位是否为 0 |

**乘除寄存器约定（必须熟记）：**

```
乘法:              除法:
  EAX × src            EDX:EAX ÷ src
    │                      │
    ▼                      ├──→ EAX = 商
  EDX:EAX (64位积)        └──→ EDX = 余数
```

### 4.3 按位运算与移位指令

| 指令 | 功能 | 要点 |
|------|------|------|
| **AND** | dst ← dst & src | 任意位清零 |
| **OR** | dst ← dst \| src | 任意位置 1 |
| **XOR** | dst ← dst ^ src | 相同清 0、不同置 1；常用 `XOR EAX,EAX`（EAX 清零，比 MOV 更短） |
| **NOT** | dst ← ~dst | 按位取反 |
| **SHL** | dst << n | 逻辑左移，低位补 0 |
| **SHR** | dst >> n（逻辑） | 逻辑右移，高位补 0 |
| **SAR** | dst >> n（算术） | 算术右移，高位补符号位 |
| **SAL** | =SHL | 算术左移=逻辑左移 |

**移位量限制**：只能是 **立即数 1** 或 **CL 寄存器**中的值。

```
SHL EAX, 1     ; ✓ 立即数 1
MOV CL, 4
SHL EAX, CL    ; ✓ 用 CL
SHL EAX, 3     ; ✗ IA-32 不允许立即数 ≠1
```

> x86-64 放宽了此限制，允许 `SHL RAX, 3`。但课堂教的是 IA-32。

### 4.4 控制转移指令

#### 无条件跳转

| 指令 | 功能 |
|------|------|
| **JMP** addr | 无条件跳转到 addr |
| **CALL** addr | 压返回地址(下一条指令地址)入栈 → 跳转到 addr |
| **RET** | 弹栈→EIP，返回调用者 |
| **RET N** | 弹栈→EIP; 再 ESP+N（stdcall 清栈） |

#### 条件跳转（Jcc）— 与 CMP 后的标志位对应

```
CMP A, B  实际计算 A−B，根据结果设置标志位
```

**无符号比较：**

| 指令 | 条件 | 标志位判断 |
|------|------|-----------|
| **JE / JZ** | A == B | ZF=1 |
| **JNE / JNZ** | A != B | ZF=0 |
| **JA / JNBE** | A > B（无符号） | CF=0 且 ZF=0 |
| **JAE / JNB** | A >= B（无符号） | CF=0 |
| **JB / JNAE** | A < B（无符号） | CF=1 |
| **JBE / JNA** | A <= B（无符号） | CF=1 或 ZF=1 |

**有符号比较：**

| 指令 | 条件 | 标志位判断 |
|------|------|-----------|
| **JG / JNLE** | A > B（有符号） | SF=OF 且 ZF=0 |
| **JGE / JNL** | A >= B（有符号） | SF=OF |
| **JL / JNGE** | A < B（有符号） | SF≠OF |
| **JLE / JNG** | A <= B（有符号） | SF≠OF 或 ZF=1 |

**其他常用条件跳转：**

| 指令 | 跳转条件 |
|------|----------|
| JS | SF=1（结果为负） |
| JNS | SF=0 |
| JO | OF=1（溢出） |
| JNO | OF=0 |
| JCXZ | CX=0 |

**一条口诀：**

```
JG/JL 看 SF 和 OF —— 有符号
JA/JB 看 CF 和 ZF —— 无符号

无符号 5 > −1？−1=0xFFFFFFFF=最大无符号数，所以 JA 为假。
有符号 5 > −1？是真，JG 为真。
```

---

## 五、C 控制流 → 汇编对照

### 5.1 if-else

```c
if (x > 0)
    y = 1;
else
    y = -1;
```

```asm
    CMP  [x], 0        ; x 与 0 比较
    JLE  .L2           ; x <= 0 则跳转到 else 分支
    MOV  [y], 1        ; if 分支：y = 1
    JMP  .L3           ; 跳过 else
.L2:
    MOV  [y], -1       ; else 分支：y = -1
.L3:
    ; 继续执行
```

### 5.2 if-else if-else 链

```c
if (a > 0)      r = 1;
else if (a < 0) r = -1;
else            r = 0;
```

```asm
    CMP  [a], 0
    JLE  .L2          ; a>0 不成立，检查下一个
    MOV  [r], 1
    JMP  .L4
.L2:
    CMP  [a], 0
    JGE  .L3          ; a<0 不成立，进 else
    MOV  [r], -1
    JMP  .L4
.L3:
    MOV  [r], 0
.L4:
```

### 5.3 switch（跳转表）

```c
switch (x) {
    case 0: r = 10; break;
    case 1: r = 20; break;
    case 2: r = 30; break;
    case 3: r = 40; break;
    default:r = 0;  break;
}
```

```asm
    MOV  EAX, [x]
    CMP  EAX, 3
    JA   .Ldefault       ; x>3 → default
    JMP  *[.LJMP + EAX*4] ; 间接跳转：跳转表[EAX]

    .section .rodata
.LJMP:
    .long .L0             ; x=0 → 跳这里
    .long .L1             ; x=1 → 跳这里
    .long .L2             ; x=2 → 跳这里
    .long .L3             ; x=3 → 跳这里

.L0: MOV [r], 10; JMP .Lend
.L1: MOV [r], 20; JMP .Lend
.L2: MOV [r], 30; JMP .Lend
.L3: MOV [r], 40; JMP .Lend
.Ldefault: MOV [r], 0
.Lend:
```

> **关键特征**：`JMP *[table + index*4]` + `.rodata` 中的 `.long` 数组 = switch 跳转表。跳转表在只读数据节。

### 5.4 while 循环

```c
while (i < 10) {
    sum += i;
    i++;
}
```

```asm
    JMP  .L2           ; 先跳去判断条件
.L3:
    ADD  [sum], EAX    ; sum += i
    INC  [i]           ; i++
.L2:
    CMP  [i], 10       ; i < 10 ?
    JL   .L3           ; 是，跳回循环体
    ; 退出
```

while 先判断后执行 → 有一个 `JMP` 跳到末尾的条件判断。

### 5.5 do-while 循环

```c
do {
    sum += i;
    i++;
} while (i < 10);
```

```asm
.L1:
    ADD  [sum], EAX    ; sum += i
    INC  [i]           ; i++
    CMP  [i], 10       ; i < 10 ?
    JL   .L1           ; 是，回到循环体
```

do-while 先执行后判断 → **最紧凑**，没有额外 JMP。

### 5.6 for 循环

```c
for (i = 0; i < 10; i++) {
    sum += a[i];
}
```

```asm
    MOV  [i], 0        ; i=0
    JMP  .L2           ; 跳到条件
.L3:
    MOV  EAX, [i]
    MOV  EDX, [a + EAX*4]  ; a[i]（int 数组 4 字节每元素）
    ADD  [sum], EDX
    INC  [i]           ; i++
.L2:
    CMP  [i], 10       ; i < 10 ?
    JL   .L3
```

> for 和 while 的汇编结构几乎一样（先判断后执行），区别仅在于初始化部分在循环前。

### 5.7 三种循环结构对比

```
do-while       while / for
─────────      ────────────
  ↓              JMP → 条件判断
循环体             ↓ ← ┐
  ↓             条件判断  │
条件判断           ├─真─→ 循环体
├─真─→ 循环体    └─假─→ 退出
└─假─→ 退出

最紧凑(无JMP)    多一条 JMP（但先判断可避免空循环体）
```

---

## 六、函数调用与栈帧

### 6.1 栈帧结构图

```
调用者栈帧：
高地址  ┌─────────────────┐
        │  参数 n          │ [EBP+16]
        │  参数 2          │ [EBP+12]
        │  参数 1          │ [EBP+8]   ← 第一个参数
        │  返回地址        │ [EBP+4]   ← CALL 压入
EBP→ ──┼─────────────────┤
        │  旧 EBP 值       │ [EBP]     ← PUSH EBP
        │  局部变量 1      │ [EBP-4]
        │  局部变量 2      │ [EBP-8]
        │  ...             │
ESP→ ──┼─────────────────┤
低地址  │  (可用栈空间)    │
        └─────────────────┘
```

### 6.2 函数模板（标准开/闭幕）

```asm
; ── 函数入口（序幕）──
PUSH  EBP              ; 保存调用者的 EBP
MOV   EBP, ESP         ; 建立自己的帧指针
SUB   ESP, N           ; 分配 N 字节局部变量空间
PUSH  EBX              ; 保存需保留的寄存器（被调者保存）

; ── 函数体 ──
; [EBP+8]  = 第一个参数
; [EBP+12] = 第二个参数
; [EBP-4]  = 第一个局部变量
; ...

; ── 函数出口（闭幕）──
POP   EBX              ; 按逆序恢复寄存器
MOV   ESP, EBP         ; 销毁栈帧（或 LEAVE）
POP   EBP              ; 恢复调用者的 EBP
RET                    ; 返回
```

### 6.3 调用约定对比

| 约定 | 参数传递 | 清栈责任 | 清栈方式 | 变参支持 | 名称修饰 |
|------|:---:|:---:|------|:---:|------|
| **cdecl** | 栈，**从右向左**压 | **调用者** | `ADD ESP, N` (call后) | ✅ | `_func` |
| **stdcall** | 栈，**从右向左**压 | **被调者** | `RET N` | ❌ | `_func@N` |
| **fastcall** | ECX, EDX 传前两个，其余栈 | **被调者** | `RET N` | ❌ | `@func@N` |

**cdecl 调用现场：**

```asm
; 调用 sum(3, 5, 7)
PUSH  7              ; 从右向左压参数
PUSH  5
PUSH  3
CALL  sum            ; 压返回地址 + 跳转
ADD   ESP, 12        ; 调用者清栈（3 个参数 × 4 字节 = 12）
; EAX = 返回值
```

**stdcall 调用现场：**

```asm
PUSH  7
PUSH  5
PUSH  3
CALL  sum            ; sum 内部以 RET 12 结束，自己清栈
; EAX = 返回值
; 调用者不需要 ADD ESP
```

**识别要点**：
- `RET N`（N>0）→ 一定是 stdcall
- `CALL` 后紧跟 `ADD ESP, N` → 一定是 cdecl
- 前两个参数用 ECX/EDX → fastcall

---

## 七、数据结构访问

### 7.1 数组

```c
int a[5];       // a[i] 的地址 = a + i*4
short b[5];     // b[i] 的地址 = b + i*2
```

```asm
; 读 a[i] → EAX（假设 i 在 ECX，a 基址在 EBX）
MOV  EAX, [EBX + ECX*4]     ; 比例变址，比例因子=sizeof(int)

; 二维 int a[3][4]; 读 a[i][j]
; 地址 = a + (i*4 + j)*4
MOV  EAX, [EBX + ECX*16 + EDX*4]  ; i*16 + j*4
```

### 7.2 结构体

```c
struct S {
    char  c;   // 偏移 0（1 字节）
    // 3 字节填充
    int   i;   // 偏移 4（对齐到 4 的倍数）
    short s;   // 偏移 8（2 字节）
    // 2 字节尾部填充（使整体=最大成员 4 的倍数）
};  // sizeof = 12
```

```asm
; 假设 EBX = &s
MOV  AL, [EBX]        ; s.c（偏移 0）
MOV  EAX, [EBX+4]     ; s.i（偏移 4）
MOV  AX, [EBX+8]      ; s.s（偏移 8）
```

### 7.3 联合体

```c
union U {
    int   i;
    float f;
    char  c[4];
};  // sizeof = 4（最大成员）
```

```asm
; 所有成员从同一地址开始
MOV  EAX, [u]         ; u.i
MOV  [u], 0x3F800000  ; 同时改变 u.i, u.f, u.c[0..3]
```

### 7.4 对齐规则速算

```
(1) 每个成员的偏移量必须能被 自身大小 整除
(2) 结构体总大小必须能被 最大成员大小 整除

struct { char a; double b; char c; }
  a: 偏移 0(1字节) → 7字节填充 → b: 偏移 8(8字节) → c: 偏移 16(1字节)
  → 尾部填充 7字节使总长=24（8的倍数）
  sizeof = 24
```

---

## 八、练习：C → 汇编

### 题 1（基础——数组遍历）

将以下 C 函数翻译为 IA-32 汇编：

```c
int sum_array(int a[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++)
        sum += a[i];
    return sum;
}
```

### 题 2（基础——if-else）

```c
int abs(int x) {
    if (x >= 0)
        return x;
    else
        return -x;
}
```

### 题 3（中等——循环+条件）

```c
int count_positive(int a[], int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] > 0)
            count++;
    }
    return count;
}
```

### 题 4（中等——结构体）

```c
struct Point { int x; int y; };

int manhattan(struct Point *p) {
    return abs(p->x) + abs(p->y);
}
```

---

## 九、练习：汇编 → C

### 题 5（基础——简单函数）

```asm
func5:
    PUSH  EBP
    MOV   EBP, ESP
    MOV   EAX, [EBP+8]     ; 读第一个参数
    ADD   EAX, [EBP+12]    ; 加第二个参数
    POP   EBP
    RET
```

请还原 C 函数。

### 题 6（基础——条件）

```asm
func6:
    PUSH  EBP
    MOV   EBP, ESP
    MOV   EAX, [EBP+8]     ; x
    CMP   EAX, 0
    JG    .L1
    MOV   EAX, 0           ; return 0
    JMP   .L2
.L1:
    MOV   EAX, 1           ; return 1
.L2:
    POP   EBP
    RET
```

### 题 7（中等——循环）

```asm
func7:
    PUSH  EBP
    MOV   EBP, ESP
    MOV   ECX, [EBP+8]     ; ECX = n
    MOV   EAX, 1            ; result = 1
    CMP   ECX, 1
    JLE   .L2
.L1:
    IMUL  EAX, ECX          ; result *= n
    DEC   ECX               ; n--
    CMP   ECX, 1
    JG    .L1
.L2:
    POP   EBP
    RET
```

### 题 8（中等——数组+循环）

```asm
func8:
    PUSH  EBP
    MOV   EBP, ESP
    MOV   ECX, [EBP+8]          ; ECX = a (数组指针)
    MOV   EDX, [EBP+12]         ; EDX = n
    MOV   EAX, [ECX]            ; max = a[0]
    MOV   EBX, 1                ; i = 1
    JMP   .L2
.L1:
    MOV   ESI, [ECX + EBX*4]    ; ESI = a[i]
    CMP   ESI, EAX
    JLE   .L3
    MOV   EAX, ESI              ; max = a[i]
.L3:
    INC   EBX                   ; i++
.L2:
    CMP   EBX, EDX
    JL    .L1
    POP   EBP
    RET
```

---

## 九题答案与详细解析

### 题 1 答案

```asm
sum_array:
    PUSH  EBP
    MOV   EBP, ESP
    SUB   ESP, 8            ; 分配局部: [EBP-4]=sum, [EBP-8]=i
    MOV   DWORD [EBP-4], 0  ; sum = 0
    MOV   DWORD [EBP-8], 0  ; i = 0
    JMP   .Lcond
.Lbody:
    MOV   EAX, [EBP-8]      ; i
    MOV   EDX, [EBP+8]      ; a
    MOV   EDX, [EDX + EAX*4]; a[i]
    ADD   [EBP-4], EDX      ; sum += a[i]
    INC   DWORD [EBP-8]     ; i++
.Lcond:
    MOV   EAX, [EBP-8]
    CMP   EAX, [EBP+12]     ; i < n ?
    JL    .Lbody
    MOV   EAX, [EBP-4]      ; return sum
    MOV   ESP, EBP
    POP   EBP
    RET
```

### 题 2 答案

```asm
abs:
    PUSH  EBP
    MOV   EBP, ESP
    MOV   EAX, [EBP+8]     ; x
    CMP   EAX, 0
    JGE   .Lpos             ; x >= 0 → return x
    NEG   EAX               ; x < 0 → return -x
.Lpos:
    POP   EBP
    RET
```

### 题 3 答案

```asm
count_positive:
    PUSH  EBP
    MOV   EBP, ESP
    PUSH  EBX
    MOV   ECX, [EBP+8]     ; ECX = a
    MOV   EDX, [EBP+12]    ; EDX = n
    XOR   EAX, EAX          ; count = 0
    XOR   EBX, EBX          ; i = 0
    JMP   .Lcond
.Lbody:
    CMP   DWORD [ECX + EBX*4], 0
    JLE   .Lskip            ; a[i] <= 0 → 跳过
    INC   EAX               ; count++
.Lskip:
    INC   EBX               ; i++
.Lcond:
    CMP   EBX, EDX
    JL    .Lbody
    POP   EBX
    POP   EBP
    RET
```

### 题 4 答案

```asm
manhattan:
    PUSH  EBP
    MOV   EBP, ESP
    SUB   ESP, 4
    MOV   ECX, [EBP+8]     ; ECX = p
    MOV   EAX, [ECX]       ; p->x
    ; 手动内联 abs：
    CMP   EAX, 0
    JGE   .L1
    NEG   EAX
.L1:
    MOV   [EBP-4], EAX     ; 暂存 abs(p->x)
    MOV   EAX, [ECX+4]     ; p->y
    CMP   EAX, 0
    JGE   .L2
    NEG   EAX
.L2:
    ADD   EAX, [EBP-4]     ; abs(p->x) + abs(p->y)
    MOV   ESP, EBP
    POP   EBP
    RET
```

### 题 5 答案

**汇编分析**：读 [EBP+8]（参数1）和 [EBP+12]（参数2），相加，返回。

```c
int func5(int a, int b) {
    return a + b;
}
```

### 题 6 答案

**汇编分析**：`CMP ... JG` 判断参数 > 0，大于返回 1，否则返回 0。

```c
int func6(int x) {
    if (x > 0)
        return 1;
    else
        return 0;
}
```

### 题 7 答案

**汇编分析**：参数 n→ECX，result 初值 1。循环 ECX 从 n 递减到 2，每次 `result *= ECX`。

```c
int func7(int n) {
    int result = 1;
    while (n > 1) {
        result *= n;
        n--;
    }
    return result;
}
// 等价于 factorial(n)
```

### 题 8 答案

**汇编分析**：`[ECX]` = a[0] 初始化为 max；循环 `i` 从 1 到 n−1，`[ECX+EBX*4]` 访问 a[i]；`CMP ... JLE` 判断是否更新 max。

```c
int func8(int a[], int n) {
    int max = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] > max)
            max = a[i];
    }
    return max;
}
// 等价于 find_max
```

---

## 附录：常见反汇编特征识别速查

| 你看到的 | 它一定是 |
|----------|:---:|
| `JMP *[table + reg*4]` + `.rodata` 中的 `.long` 数组 | switch 跳转表 |
| `PUSH ..., PUSH ..., CALL ..., ADD ESP, N` | cdecl 调用 |
| `PUSH ..., PUSH ..., CALL ...,` (无 ADD ESP) 且被调函数 `RET N` | stdcall |
| `PUSH EBP; MOV EBP,ESP; SUB ESP,N` | 函数入口（序幕） |
| `LEAVE` (= `MOV ESP,EBP; POP EBP`) | 函数出口（闭幕） |
| `XOR EAX,EAX` | `return 0` 或清零 |
| `[EBP+8]`, `[EBP+12]` | 函数参数 |
| `[EBP-4]`, `[EBP-8]` | 局部变量 |
| `MOVSX` （符号扩展传送） | 小整数→大整数（char→int 等） |
| `MOVZX` （零扩展传送） | 无符号小整数→大整数 |
| `CALL` 前多个 `PUSH`，无 `ADD ESP` 但 `RET N` | stdcall |
| 只有 `CMP` + `JL/JG` 无 `JA/JB` | 全是 signed 操作 |
| `CMP EAX, N; JA .Ldefault` | switch 边界检查 |
| `SAR` 不是 `SHR` | 有符号右移 |
