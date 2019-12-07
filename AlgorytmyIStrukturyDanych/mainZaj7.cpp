#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <vector>

template<typename iter_T>
void heap_sort(iter_T begin, iter_T end) {}

int mainZaj7() {
    srand(time(0));

    // fill vector with 30 random values
    std::vector<int> vec(30, 0);
    std::for_each(vec.begin(), vec.end(), [](int& x) { x = rand() % 200; });

    return 0;
}
