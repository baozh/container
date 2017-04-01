container
=========

### A Minimal Container Library

一个小型的容器库，封装了常用的 C++容器(如 darray,list,stack,queue,map,set,hashmap,hashset) 。

做这个项目主要想学习：

- 如何封装、设计接口。

	1. 简单、易用。接口名尽量简短并达意，接口参数尽量少，容易使用。
	2. 一致性。每个容器尽量提供相同的访问方法(如都有IsEmpty(),MakeEmpty())，且提供一致的迭代访问(如Map::Position,Set::Position,List::Position)。

- 复杂数据结构(如 hashtable, red-black tree) 的实现。

### 使用

直接包含include目录下的头文件即可。
