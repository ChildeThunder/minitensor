# MiniTensor：CMake 编译与运行命令总结

## 一、首先区分 Debug 和 Release

| 类型     | Debug                | Release             |
| -------- | -------------------- | ------------------- |
| 主要用途 | 开发、调试、定位 Bug | 性能测试、发布      |
| 编译优化 | 通常关闭（`-O0`）    | 通常开启（`-O3`）   |
| 调试信息 | 通常包含 `-g`        | 默认通常不包含 `-g` |
| 构建目录 | `build/`             | `build-release/`    |

所有命令都从项目根目录执行：

```
cd ~/coding/MiniTensor
```

### 1. Debug 模式：配置并编译

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

### 2. Release 模式：配置并编译

```
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release -j
```

三个关键参数：

- `-S .`：指定源码目录为当前目录。
- `-B build`：指定构建目录。
- `-DCMAKE_BUILD_TYPE=Debug/Release`：指定构建类型。

注意：第一次配置完成后，如果只是修改了 `.cpp` 文件，通常不需要再次执行 `cmake -S`，直接 `cmake --build` 即可。

## 二、四种程序的启动方式

### 1. main.cpp：普通 Demo

你的 `examples/main.cpp` 在 CMake 中对应的可执行程序是 `minitensor_demo`，不是 `main`。

Debug：

```
./build/minitensor_demo
```

Release：

```
./build-release/minitensor_demo
```

主要用于验证 Tensor 创建、reshape 和基本功能。

### 2. CTest：运行单元测试

CTest 不是你自己编写的可执行程序，而是 CMake 提供的测试工具。

Debug：

```
ctest --test-dir build --output-on-failure
```

Release：

```
ctest --test-dir build-release --output-on-failure
```

参数解释：

- `--test-dir`：指定在哪个构建目录中查找测试。
- `--output-on-failure`：测试失败时输出错误信息。

如果需要查看所有测试的详细输出，可以使用：

```
ctest --test-dir build -V
```

### 3. bench_add.cpp：性能基准测试

Debug：

```
./build/bench_add
```

Release：

```
./build-release/bench_add
```

用于比较 Copy Add 和 Direct Add 的执行时间。

性能测试应该以 Release 版本为主，因为 Debug 模式通常未开启编译优化，测出的时间不代表正常优化构建的性能。

### 4. debug_lab.cpp：调试实验

这个程序是我们故意设计的空指针访问实验。

Debug 直接运行：

```
./build/debug_lab
```

Release 直接运行：

```
./build-release/debug_lab
```

但这个程序包含未定义行为，因此不能保证两种构建都表现为段错误。

如果使用 GDB 调试：

Debug + GDB（推荐）：

```
gdb ./build/debug_lab
```

Release + GDB：

```
gdb ./build-release/debug_lab
```

Release 默认经过优化，部分变量可能被优化掉，并且可能缺少调试符号，因此通常没有 Debug 容易分析。

## 三、最常用的完整命令速查表

| 操作           | Debug                                          | Release                                                  |
| -------------- | ---------------------------------------------- | -------------------------------------------------------- |
| 配置           | `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug` | `cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release` |
| 编译           | `cmake --build build -j`                       | `cmake --build build-release -j`                         |
| 运行 main      | `./build/minitensor_demo`                      | `./build-release/minitensor_demo`                        |
| 运行 CTest     | `ctest --test-dir build --output-on-failure`   | `ctest --test-dir build-release --output-on-failure`     |
| 运行 Benchmark | `./build/bench_add`                            | `./build-release/bench_add`                              |
| 运行 debug_lab | `./build/debug_lab`                            | `./build-release/debug_lab`                              |
| GDB 调试       | `gdb ./build/debug_lab`                        | `gdb ./build-release/debug_lab`                          |