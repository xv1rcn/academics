# 计算机系统基础

Introduction to Computer Systems

## 内容

- `ex1`–`ex8`：课程习题，含 LaTeX 解答（`docs/`）与代码、图片
- `notes/`：章节笔记与复习资料，包括 ch01 概述、ch02 数据表示、ch03 机器级表示、ch04 链接，以及 IA-32 汇编参考、考试复习提纲、模拟试卷与答案

## 结构

练习位于 `exN/`：`exN.pdf` 为题面，`docs/` 为 LaTeX 解答，`src/` 或 `res/` 为代码与图片。

## 编译

```bash
cd exN/docs
xelatex -shell-escape document.tex
```
