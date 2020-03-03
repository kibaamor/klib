# klib

A sort of C++11 utility functions.

一些C++11的实用函数。

## 实用函数列表

* btea算法
* 屏蔽字检测
* crc32
* 易扩展的序列化和反序列化

## 编译环境

* CMake 3.1及以上
* Visual Studio 2015及以上(Windows)
* Gcc 4.9及以上(Linux)

## 如何使用

Windows和Linux下都可以使用下面的命令

### 编译

```bash
# 进入源码根目录
cd knet

# 生成工程
cmake . -B build

# 编译Relase
cmake --build build --config Release
```

### 测试

```bash
# 进入源码根目录下的build目录
cd build

# 运行测试
ctest -C Release
```
