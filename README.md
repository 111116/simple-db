# simple-db

本仓库为《大作业——简易数据库实现》（见`assignment1.pdf`）第一阶段的代码。

代码各组件结构与功能参见文档 `doc/docs.md` (`doc/docs.pdf`) 及代码注释。

基本的代码格式与命名规范见 `CONTRIBUTING.md`。

## 编译

```bash
git clone https://github.com/111116/simple-db
cd simple-db/src && make
```

程序在以下环境下测试通过。

* clang-1000.11.45.5 @ x86_64-apple-darwin18.2.0
* g++ 7.4.0 @ Ubuntu 18.04 (WSL/Windows 10.0.18362.86)
* 评测 OJ 环境

使用较低版本的 GCC 套件编译可能失败。
