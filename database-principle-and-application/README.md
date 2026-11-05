# 数据库原理与应用

Database: Principles and Applications

## 内容

- `ex1`–`ex5`：课程习题，含 LaTeX 解答、SQL 与实验截图
- `course-design/`：MedQueue 门诊排队叫号系统（FastAPI + PostgreSQL + Vue 3）
- `notes/`：17 份《数据库系统概论》课件与 1 份补考复习参考

## 结构

练习位于 `exN/`：`exN.pdf` 为题面，`docs/` 为 LaTeX 解答，`src/` 或 `res/` 为代码与图片。

## 编译

```bash
cd exN/docs
xelatex -shell-escape document.tex
```
