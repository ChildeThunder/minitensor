#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>
#include <limits>
#include "minitensor/allocator.h"
#include "minitensor/tensor.h"
using namespace std;
int test_constructor()
{
    minitensor::Tensor t1({2, 3, 4});

    std::cout << "t1 size: "
              << t1.size() << std::endl;
    for (auto dim : t1.shape())
    {
        std::cout << dim << " ";
    }
    minitensor::Tensor t2({10});

    cout << "t2 size: " << t2.size() << endl;
    if (t1.size() != 24 || t2.size() != 10)
    {
        cerr << "Tensor size test failed!" << endl;
        return 1;
    }

    try
    {
        minitensor::Tensor t3({});
        cout << "FAIL: t3没有抛出异常" << endl;
        return 1;
    }
    catch (const invalid_argument &e)
    {
        cout << "PASS: t3 " << e.what() << endl;
    }

    try
    {
        minitensor::Tensor t4({2, 0, 3});
        cout << "FAIL: t4没有抛出异常" << endl;
        return 1;
    }
    catch (const invalid_argument &e)
    {
        cout << "PASS: t4 " << e.what() << endl;
    }

    try
    {
        minitensor::Tensor t5({SIZE_MAX, 2});
        cout << "FAIL: t5没有抛出异常" << endl;
        return 1;
    }
    catch (const overflow_error &e)
    {
        cout << "PASS: t5 " << e.what() << endl;
    }
    cout << endl;
    return 0;
}
int test_reshape()
{
    // 新增 reshape 测试
    minitensor::Tensor t({2, 3, 4});

    t.reshape({4, 6});

    if (t.size() != 24 ||
        t.shape() != vector<size_t>{4, 6})
    {
        cerr << "FAIL: reshape test" << endl;
        return 1;
    }

    cout << "PASS: reshape test" << endl;
    return 0;
}
int test_data()
{
    minitensor::Tensor t({2, 3});

    float *p = t.data();
    cout << p[0] << endl;

    p[0] = 1.5f;
    p[1] = 2.5f;
    p[2] = 3.5f;

    cout << t.data()[0];
    const minitensor::Tensor &ct = t;
    const float *p2 = ct.data();
    cout << p2[2] << endl;

    t.reshape({3, 2});
    if (t.data() != p || t.data()[0] != 1.5f)
    {
        return 1;
    }
    return 0;
}

using minitensor::Tensor;

// 1. 测试拷贝构造
int test_copy_constructor()
{
    Tensor a({2, 3});
    a.data()[0] = 1.5f;
    a.data()[5] = 6.5f;

    Tensor b(a);

    if (b.size() != 6 ||
        b.shape() != vector<size_t>{2, 3} ||
        b.data() == a.data() ||
        b.data()[0] != 1.5f ||
        b.data()[5] != 6.5f)
    {
        cerr << "FAIL: copy constructor" << endl;
        return 1;
    }

    // 修改副本不能影响原对象
    b.data()[0] = 100.0f;

    if (a.data()[0] != 1.5f)
    {
        cerr << "FAIL: deep copy" << endl;
        return 1;
    }

    cout << "PASS: copy constructor" << endl;
    return 0;
}

// 2. 测试拷贝赋值
int test_copy_assignment()
{
    Tensor a({2, 3});
    Tensor b({4, 5});

    a.data()[0] = 10.0f;
    a.data()[5] = 20.0f;

    // 测试返回值是否正确
    Tensor *result = &(b = a);

    if (result != &b ||
        b.size() != 6 ||
        b.shape() != vector<size_t>{2, 3} ||
        b.data() == a.data() ||
        b.data()[0] != 10.0f ||
        b.data()[5] != 20.0f)
    {
        cerr << "FAIL: copy assignment" << endl;
        return 1;
    }

    // 验证深拷贝
    b.data()[0] = 30.0f;
    if (a.data()[0] != 10.0f)
        return 1;

    // 测试自赋值
    b = b;

    if (b.size() != 6 || b.data()[0] != 30.0f)
        return 1;

    cout << "PASS: copy assignment" << endl;
    return 0;
}

// 3. 测试移动构造
int test_move_constructor()
{
    Tensor a({2, 3});
    a.data()[0] = 10.0f;
    a.data()[5] = 20.0f;

    float *old_ptr = a.data();

    Tensor b = std::move(a);

    if (b.data() != old_ptr ||
        b.size() != 6 ||
        b.shape() != vector<size_t>{2, 3} ||
        b.data()[0] != 10.0f ||
        b.data()[5] != 20.0f)
    {
        cerr << "FAIL: move constructor" << endl;
        return 1;
    }

    // 验证源对象处于约定的移后空状态
    if (a.data() != nullptr ||
        a.size() != 0 ||
        !a.shape().empty())
    {
        cerr << "FAIL: moved-from state" << endl;
        return 1;
    }

    cout << "PASS: move constructor" << endl;
    return 0;
}

// 4. 测试移动赋值
int test_move_assignment()
{
    Tensor a({2, 3});
    Tensor b({4, 5});

    a.data()[0] = 11.0f;
    a.data()[5] = 22.0f;

    float *old_ptr = a.data();

    Tensor *result = &(b = std::move(a));

    if (result != &b ||
        b.data() != old_ptr ||
        b.size() != 6 ||
        b.shape() != vector<size_t>{2, 3} ||
        b.data()[0] != 11.0f ||
        b.data()[5] != 22.0f)
    {
        cerr << "FAIL: move assignment" << endl;
        return 1;
    }

    if (a.data() != nullptr ||
        a.size() != 0 ||
        !a.shape().empty())
    {
        cerr << "FAIL: moved-from state" << endl;
        return 1;
    }

    // 测试自移动赋值
    b = std::move(b);

    if (b.data() != old_ptr ||
        b.size() != 6 ||
        b.data()[0] != 11.0f)
    {
        cerr << "FAIL: self move assignment" << endl;
        return 1;
    }

    cout << "PASS: move assignment" << endl;
    return 0;
}
int test_allocator()
{
    auto empty = minitensor::CPUAllocator::allocate(0);
    if (empty != nullptr)
    {
        cerr << "FAIL: allocate(0)" << endl;
        return 1;
    }
    // 测试字节数溢出
    try
    {
        size_t count = numeric_limits<size_t>::max();

        auto p = minitensor::CPUAllocator::allocate(count);

        cerr << "FAIL: 没有抛出 overflow_error" << endl;
        return 1;
    }
    catch (const overflow_error &e)
    {
        cout << "PASS: overflow test: " << e.what() << endl;
    }
    catch (const exception &e)
    {
        cerr << "FAIL: 错误的异常类型: " << e.what() << endl;
        return 1;
    }
    Tensor a({10});
    for (int i = 0; i < 10; i++)
    {
        if (a.data()[i] != 0.0f)
        {
            return 1;
        }
    }
    a.data()[6] = 6.6;
    if (a.data()[6] != 6.6f)
    {
        return 1;
    }

    return 0;
}
int test_add()
{
    Tensor a({2, 3});
    Tensor b({2, 3});
    for (int i = 0; i < 6; i++)
    {
        a.data()[i] = i + 1;
        b.data()[i] = 10;
    }
    Tensor c = a + b;
    if (c.data()[0] != 11 || c.data()[1] != 12)
    {
        return 1;
    }
    Tensor d({3, 2});
    try
    {
        a + d;
        cerr << "FAIL: 不同形状没有抛出异常" << endl;
        return 1;
    }
    catch (const invalid_argument &e)
    {
        cout << e.what() << endl;
    }
    Tensor f({2, 6});
    try
    {
        a + f;
        cerr << "FAIL: 不同形状没有抛出异常" << endl;
        return 1;
    }
    catch (const invalid_argument &e)
    {
        cout << e.what() << endl;
    }
    const Tensor a1({2, 3});
    const Tensor a2({2, 3});
    c = a1 + a2;
    return 0;
}
int main()
{
    int failures = 0;
    failures += test_add();
    failures += test_allocator();
    failures += test_constructor();
    failures += test_reshape();
    failures += test_data();

    failures += test_copy_constructor();
    failures += test_copy_assignment();
    failures += test_move_constructor();
    failures += test_move_assignment();

    if (failures != 0)
    {
        cerr << "Some tests failed!" << endl;
        return 1;
    }

    cout << "All tests passed!" << endl;
    return 0;
}