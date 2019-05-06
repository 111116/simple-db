# simple-db

## 组件

`Client` 解析用户输入，管理全局选定数据库

`Database` 数据库类

`Table` 表类

`Entry` 记录（行）类

`data_t` 数据类型抽象类，派生出各种数据类型

`cond_t` 接收`Entry`返回`bool`的函数类，用于WHERE筛选

`set_t` 接收`Entry&`的函数类，用于SET子句修改数据

详情请参考文档 `doc/docs.md` (`doc/docs.pdf`) 及代码注释。

## 编译

```bash
git clone https://github.com/111116/simple-db
cd simple-db/src && make
```

程序在以下环境下测试通过。

* clang-1000.11.45.5 @ x86_64-apple-darwin18.2.0
* g++ 7.3.0 @ Ubuntu 18.04 (WSL/Windows 10.0.18362.86)
* 评测 OJ 环境

使用较低版本的 GCC 套件编译可能失败。
