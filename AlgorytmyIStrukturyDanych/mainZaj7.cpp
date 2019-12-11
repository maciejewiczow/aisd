#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

#include "heap.h"

template<typename iter_T, template<class> class order_T = std::less>
void heap_sort(iter_T begin, iter_T end) {
    tree::heap<typename iter_T::value_type, order_T> heap{
        std::make_move_iterator(begin), std::make_move_iterator(end)};

    while (heap.size()) *begin++ = heap.pop_root();
}

int mainZaj7() {
    srand(time(0));

    // vector with 30 random values
    std::vector<std::string> vec{
        "maciej",
        "zzc",
        "adamus",
        "zza",
        "zb",
        "kurcze",
        "pieczone",
        "ab",
        "aaaaaaaad",
        "aaab",
    };

    for (const auto& it : vec) std::cout << it << "  ";
    std::cout << std::endl;

    heap_sort(vec.begin(), vec.end());

    for (const auto& it : vec) std::cout << it << "  ";
    std::cout << std::endl;

    return 0;
}
