#include <iostream>

void write_value(int *ptr)
{
    *ptr = 100;
}

int main()
{
    int *p = nullptr;

    write_value(p);

    std::cout << "Finished!" << std::endl;

    return 0;
}