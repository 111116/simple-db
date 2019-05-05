# simple-db

## 代码格式

- 文件名使用小写

- 类名格式如 `Database`, `Table`, `Entry`, `cond_t`, `data_t`（基本数据结构类名采用小写加 `_t` 后缀，复杂类名首字母大写）

- 大括号换行

- 适当写注释

- 用 exception 而不用返回值指示异常

## 组件

`Client` 解析用户输入，管理全局选定数据库

`Database` 数据库类

`Table` 表类

`Entry` 记录（行）类

`data_t` 数据类型抽象类，派生出各种数据类型

`cond_t` 接收`Entry`返回`bool`的函数类，用于WHERE筛选

`set_t` 接收`Entry&`的函数类，用于SET子句修改数据

详情请参考文档 docs.md (docs.pdf) 或代码内注释。

## 测试环境

程序在以下环境下测试通过。

* * Apple LLVM version 10.0.0 (clang-1000.11.45.5)
  * Target: x86_64-apple-darwin18.2.0
* * g++ 7.3.0
  * Ubuntu 18.04 (WSL/Windows 10.0.18362.86)
* 评测 OJ 环境

编译选项：-std=c++14 -O2

在 Windows 上使用较低版本的 GCC 套件编译时可能会出现 Compile Error。推荐使用 WSL :-)
