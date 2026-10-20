#pragma once
#include <memory>
#include <cstddef>
#include <vector>
namespace minitensor
{

    class Tensor
    {
    public:
        explicit Tensor(std::vector<size_t> shape);

        std::vector<size_t> shape() const;

        size_t size() const;

        void reshape(std::vector<size_t> new_shape);

        float *data() noexcept;

        const float *data() const noexcept;

        // 拷贝构造
        Tensor(const Tensor &other);

        // 拷贝赋值
        Tensor &operator=(const Tensor &other);

        // 移动构造
        Tensor(Tensor &&other) noexcept;

        // 移动赋值
        Tensor &operator=(Tensor &&other) noexcept;

        // 析构
        ~Tensor() = default;

        Tensor operator+(const Tensor &other) const;

    private:
        std::vector<size_t> shape_;
        std::size_t numel_ = 1;
        std::unique_ptr<float[]> data_;
    };

}