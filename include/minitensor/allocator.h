#pragma once

#include <cstddef>
#include <memory>

namespace minitensor
{

    class CPUAllocator
    {
    public:
        static std::unique_ptr<float[]> allocate(std::size_t count);
    };

}