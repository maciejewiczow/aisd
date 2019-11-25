#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

template<typename iter_T>
std::vector<typename iter_T::value_type>
    merge(const iter_T begin_first, const iter_T middle, const iter_T end) {

    std::vector<typename iter_T::value_type> buffer;
    buffer.reserve(std::distance(begin_first, end));

    iter_T left{begin_first};
    iter_T right{middle};

    while (left != middle && right != end) {
        if (*left <= *right)
            buffer.push_back(std::move(*left++));
        else
            buffer.push_back(std::move(*right++));
    }

    buffer.insert(buffer.end(), left, middle);
    buffer.insert(buffer.end(), right, end);

    return buffer;
}

template<typename iter_T>
void merge_sort(const iter_T begin, const iter_T end) {
    typename iter_T::difference_type length = std::distance(begin, end);

    if (length == 1) return;

    iter_T middle = std::next(begin, length / 2);

    merge_sort(begin, middle);
    merge_sort(middle, end);
    auto&& merged = merge(begin, middle, end);

    std::move(merged.begin(), merged.end(), begin);
}

int mainZaj6() {
    std::vector<int> vec(30, 0);

    srand(time(0));
    std::for_each(vec.begin(), vec.end(), [](int& item) { item = rand() % 150; });

    for (const auto& item : vec) {
        std::cout << item << " ";
    }

    std::cout << std::endl;

    merge_sort(vec.begin(), vec.end());

    for (auto& item : vec) {
        std::cout << item << " ";
    }

    std::cout << std::endl;

    return 0;
}
