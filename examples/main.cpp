#include <iostream>
#include "minitensor/tensor.h"
using namespace std;
int main()
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

    
    try
    {
        minitensor::Tensor t3({});
        cout << "FAIL: t3没有抛出异常" << endl;
    }
    catch (const invalid_argument &e)
    {
        cout << "PASS: t3 " << e.what() << endl;
    }

    try
    {
        minitensor::Tensor t4({2, 0, 3});
        cout << "FAIL: t4没有抛出异常" << endl;
    }
    catch (const invalid_argument &e)
    {
        cout << "PASS: t4 " << e.what() << endl;
    }

    try
    {
        minitensor::Tensor t5({SIZE_MAX, 2});
        cout << "FAIL: t5没有抛出异常" << endl;
    }
    catch (const overflow_error &e)
    {
        cout << "PASS: t5 " << e.what() << endl;
    }
    cout << endl;

    return 0;
}