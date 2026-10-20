#include <memory>

int main()
{
    auto data = std::make_unique<int[]>(4);

    for (int i = 0; i < 4; ++i)
    {
        data[i] = i;
    }

    data[4] = 100; // 故意越界

    return 0;
}