
#include <iostream>
#include <vector>
#include <chrono>
#include <utility>
#include <random>
#include "minitensor/tensor.h"

using namespace std;
using namespace std::chrono;
using minitensor::Tensor;

// 方案 A：先深拷贝，再执行加法
Tensor add_copy(const Tensor &a, const Tensor &b)
{
    if (a.shape() != b.shape())
    {
        throw invalid_argument("两个tensor形状不同，不能运算");
    }
    Tensor c = a;
    float *out = c.data();
    const float *rhs = b.data();
    for (size_t i = 0; i < c.size(); i++)
    {
        out[i] += rhs[i];
    }
    return c;
}

// 方案 B：直接使用你实现的 operator+
// 不需要额外定义函数，直接调用 a + b 即可

int main()
{
    constexpr size_t N = 1'000'000;
    constexpr int WARMUP = 5;
    constexpr int REPEAT = 50;

    // 1. 创建 Tensor
    Tensor a({N});
    Tensor b({N});

    // 2. 初始化 a 和 b 的数据
    for (int i = 0; i < N; i++)
    {
        a.data()[i] = i % 1000;
        b.data()[i] = i % 1000;
    }
    // 3. 预热两种加法实现，各执行 WARMUP 次

    for (int i = 0; i < WARMUP; i++)
    {
        Tensor c = a + b;
        Tensor d = add_copy(a, b);
    }

    // 4. 正式测试方案 A
    double total_copy_ms = 0.0;
    volatile float sink = 0.0f;
    auto start = steady_clock::now();

    for (int i = 0; i < REPEAT; i++)
    {
        Tensor c = add_copy(a, b);
        sink = c.data()[i % N];
    }
    auto end = steady_clock::now();
    total_copy_ms = duration<double, milli>(end - start).count();
    // TODO: 循环 REPEAT 次，统计耗时

    // 5. 正式测试方案 B
    double total_direct_ms = 0.0;
    sink = 0.0f;
    start = steady_clock::now();

    for (int i = 0; i < REPEAT; i++)
    {
        Tensor d = a + b;
        sink = d.data()[i % N];
    }
    end = steady_clock::now();
    total_direct_ms = duration<double, milli>(end - start).count();
    // 6. 输出平均耗时
    cout << "Copy Add: "
         << total_copy_ms / REPEAT
         << " ms" << endl;

    cout << "Direct Add: "
         << total_direct_ms / REPEAT
         << " ms" << endl;
    cout << "Sink:" << sink << endl;
    return 0;
}
