#include <iostream>
#include <cstdlib>

int main() {
    // 初次设置固定的种子值
    std::srand(42);

    std::cout << "First sequence:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << std::rand() << std::endl;
    }

    // 重新设置同一个种子值
    std::srand(42);

    std::cout << "Second sequence (should be the same as the first):" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << std::rand() << std::endl;
    }

    return 0;
}