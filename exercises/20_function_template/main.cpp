#include "../exercise.h"
#include <cmath>
#include <limits>

// READ: 函数模板 <https://zh.cppreference.com/w/cpp/language/function_template>
// TODO: 将这个函数模板化
// int plus(int a, int b) {
//     return a + b;
// }
template <class T>
T plus(T a, T b) {
    std::cout << a+b <<std::endl;
    return a + b;
}

int main(int argc, char **argv) {
    ASSERT(plus(1, 2) == 3, "Plus two int");
    ASSERT(plus(1u, 2u) == 3u, "Plus two unsigned int");

    // THINK: 浮点数何时可以判断 ==？何时必须判断差值？
    ASSERT(plus(1.25f, 2.5f) == 3.75f, "Plus two float");
    ASSERT(plus(1.25, 2.5) == 3.75, "Plus two double");
    // TODO: 修改判断条件使测试通过
    // 0.1、0.2、0.3 在 double 里都不是精确的 0.1/0.2/0.3
    //   在二进制里 1/10 会循环，因为 10 = 2×5，含有因子 5，而二进制基底是 2。
    //   0.1 的二进制展开大概是： 0.110=0.00011001100110011…2, 0011 循环。
    // ASSERT(plus(0.1, 0.2) == 0.3, "How to make this pass?");
    const double eps = std::numeric_limits<double>::epsilon() * 10;
    ASSERT(std::fabs(plus(0.1, 0.2) - 0.3) <= eps, "How to make this pass?");

    return 0;
}
