#include "minitensor/allocator.h"
#include <limits>
#include <stdexcept>
namespace minitensor
{
    using namespace std;
    unique_ptr<float[]> CPUAllocator::allocate(std::size_t count)
    {
        if (count == 0)
        {
            return nullptr;
        }
        if (count > numeric_limits<size_t>::max() / sizeof(float))
        {
            throw overflow_error("count过大，发生整数溢出");
        }
        unique_ptr<float[]> pf = make_unique<float[]>(count);
        return pf;
    }
}