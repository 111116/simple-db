# simple-db

## 代码格式

- 文件名使用小写

- 类名格式如 `Database`, `Table`, `Entry`, `cond_t`, `data_t`

- 大括号换行

- 适当写注释
	- 函数说明类的注释考虑使用 Javadoc 风格

## 类型

`Client` 解析用户输入，管理全局选定数据库

`Database`

`Table`

`Entry`

`data_t` 数据类型抽象类，派生出各种数据类型

`cond_t` 接收`Entry`返回`bool`的函数类，用于WHERE筛选

`set_t` 接收`Entry&`的函数类，用于SET子句修改数据

