#include "rtweekend.h"
#include "vec3.h"

#include <iostream>
#include <iomanip>

double pdf(const vec3& x)
{
    return x.z() / pi;
}

double f(const vec3& x)
{
    auto cos_theta = x.z();
    return cos_theta * cos_theta * cos_theta;
}

int main()
{
    int N = 1000000;
    double sum = 0.0;

    for (int i = 0 ; i < N; ++i)
    {
        vec3 d = random_cosine_direction();

        sum += f(d) / pdf(d);
    }

    std::cout << "Estimate: " << sum / N << '\n';
    std::cout<<"PI/2 "<<pi /2.0<<'\n';
}