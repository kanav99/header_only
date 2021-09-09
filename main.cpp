#include <vector>
#include <string>
#include <set>
#include "include/print.hpp"
#include "include/integer.hpp"
#include <cassert>

int main()
{
    std::vector<int> v {2, 3};
    std::println(v);
    
    uinteger<25> x1 = 23;
    uinteger<20> x2 = 10;
    std::cout << x1 << std::endl;
    auto x3 = x1 + x2;
    assert(x3.value == x1.value + x2.value);
    assert(x3.bitsize() == std::max(x1.bitsize(), x2.bitsize()));
    auto x4 = x3 * 2;
    assert(x4.value == x3.value * 2);
    assert(x4.bitsize() == x3.bitsize() + sizeof(int));
    x1 = 3;
    assert(x1.value == 3);
    int j = (int)x3;
    assert(j == x3.value);

    // Desirable, but fail as of now    
    // auto p = 1 + x1;

    // These should fail
    // uinteger<54, uint32_t> f1; // cannot fit
}