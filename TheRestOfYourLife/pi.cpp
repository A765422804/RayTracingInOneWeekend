#include "rtweekend.h"

#include <iostream>
#include <iomanip>

int main()
{
    std::cout << std::fixed << std::setprecision(12);

    int inside_circle = 0;
    int N = 1000000;
    int sqrt_N = 1000;

    for (int i = 0; i < sqrt_N; ++i)
    {
        for (int j = 0; j < sqrt_N; ++j)
        {
            auto x = 2 * ((i + random_double()) / sqrt_N) - 1;
            auto y = 2 * ((j + random_double()) / sqrt_N) - 1;
            if (x * x + y * y <= 1)
                ++inside_circle;
        }
    }

    std::cout << "Pi is approximately " << (4.0 * inside_circle) / N << std::endl;
}