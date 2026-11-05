# Java 与 Web 开发

Java and Web Development

## 内容

- `ex1`–`ex8`、`ex2p2`：课程习题，含 LaTeX 解答与代码
- `course-design/`：CloneScope 分布式代码克隆检测系统，基于 Master-Worker 与 gRPC，支持 Winnowing、CodeBERT / GraphCodeBERT 与 LLM 判别

## 结构

练习位于 `exN/`：`exN.pdf` 为题面，`docs/` 为 LaTeX 解答，`src/` 或 `res/` 为代码与图片。

## 编译

```bash
cd exN/docs
xelatex -shell-escape document.tex
```
