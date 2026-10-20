#include "minitensor/tensor.h"
#include "minitensor/allocator.h"
#include <utility>
#include <stdexcept>
#include <limits>

namespace minitensor
{
    using namespace std;
    Tensor::Tensor(vector<size_t> shape)
    {
        if (shape.size() == 0)
        {
            throw invalid_argument("tensor为空");
        }
        for (auto i : shape)
        {
            if (i == 0)
                throw invalid_argument("tensor存在维度为0");
        }
        this->shape_ = move(shape);
        for (auto i : this->shape_)
        {
            if (this->numel_ > numeric_limits<size_t>::max() / i)
            {
                throw overflow_error("Tensor的size过大");
            }
            this->numel_ *= i;
        }
        if (numel_ > numeric_limits<size_t>::max() / sizeof(float))
        {
            throw overflow_error("Tensor数据大小溢出");
        }
        this->data_ = CPUAllocator::allocate(this->numel_);
    }

    vector<size_t> Tensor::shape() const
    {
        return this->shape_;
    }
    size_t Tensor::size() const
    {
        return this->numel_;
    }
    void Tensor::reshape(std::vector<size_t> new_shape)
    {
        if (new_shape.size() == 0)
        {
            throw invalid_argument("new_shape为空");
        }
        for (auto i : new_shape)
        {
            if (i == 0)
                throw invalid_argument("new_shape存在维度为0");
        }
        size_t new_size = 1;
        for (auto i : new_shape)
        {
            if (new_size > numeric_limits<size_t>::max() / i)
            {
                throw overflow_error("Tensor的size过大");
            }
            new_size *= i;
        }
        if (this->numel_ != new_size)
        {
            throw invalid_argument("new_shape元素数量与原有tensor不一致");
        }
        this->shape_ = move(new_shape);
    }
    float *Tensor::data() noexcept
    {

        return this->data_.get();
    }

    const float *Tensor::data() const noexcept
    {
        return this->data_.get();
    }
    // 拷贝构造
    Tensor::Tensor(const Tensor &other)
    {
        this->data_ = make_unique<float[]>(other.numel_);
        this->numel_ = other.size();
        this->shape_ = other.shape();
        for (size_t i = 0; i < this->numel_; i++)
        {
            this->data_[i] = other.data()[i];
        }
    }
    // 拷贝赋值
    Tensor &Tensor::operator=(const Tensor &other)
    {
        if (this == &other)
        {
            return *this;
        }
        Tensor temp(other);
        swap(*this, temp);

        return *this;
    }
    // 移动构造
    Tensor::Tensor(Tensor &&other) noexcept
    {
        this->data_ = move(other.data_);
        this->numel_ = other.size();
        this->shape_ = move(other.shape_);
        // 删除other
        other.numel_ = 0;
        other.shape_ = {};
    }
    // 移动赋值
    Tensor &Tensor::operator=(Tensor &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        this->data_ = move(other.data_);
        this->numel_ = other.size();
        this->shape_ = move(other.shape_);
        // 删除other
        other.numel_ = 0;
        other.shape_ = {};
        return *this;
    }
    Tensor Tensor::operator+(const Tensor &other) const
    {
        if (this->shape_ != other.shape_)
        {
            throw invalid_argument("两个tensor形状不同，不能运算");
        }
        Tensor c(this->shape_);
        for (int i = 0; i < c.numel_; i++)
        {
            c.data_[i] = this->data()[i] + other.data()[i];
        }
        return c;
    }

}
