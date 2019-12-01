#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

template<typename T>
using comparator_callback = std::function<bool(const T&, const T&)>;

template<typename iter_T>
void print_range(iter_T start, iter_T end) {
    for (; start != end; ++start) {
        std::cout << *start << " ";
    }

    std::cout << std::endl;
}

template<typename iter_T>
std::vector<typename iter_T::value_type> merge(const iter_T begin,
    const iter_T middle,
    const iter_T end,
    comparator_callback<typename iter_T::value_type>& comparator) {

    std::vector<typename iter_T::value_type> buffer;
    buffer.reserve(std::distance(begin, end));

    iter_T left{begin};
    iter_T right{middle};

    std::cout << "MERGE left range: ";
    print_range(begin, middle);

    std::cout << "MERGE right range: ";
    print_range(middle, end);

    while (left != middle && right != end) {
        if (comparator(*left, *right))
            buffer.push_back(std::move(*left++));
        else
            buffer.push_back(std::move(*right++));
    }

    // move unprocessed part of longer range to the end of the result buffer
    // we dont know whitch one it is, but one is empty
    buffer.insert(buffer.end(), std::make_move_iterator(left), std::make_move_iterator(middle));
    buffer.insert(buffer.end(), std::make_move_iterator(right), std::make_move_iterator(end));

    std::cout << "MERGE final buffer: ";
    print_range(buffer.begin(), buffer.end());
    std::cout << std::endl;

    return buffer;
}

template<typename iter_T>
void merge_sort(const iter_T begin, const iter_T end) {
    merge_sort(begin, end, [](auto a, auto b) { return a <= b; });
}

template<typename iter_T>
void merge_sort(const iter_T begin,
    const iter_T end,
    comparator_callback<typename iter_T::value_type> comparator) {

    typename iter_T::difference_type length = std::distance(begin, end);

    if (length == 1) return;

    iter_T middle = std::next(begin, length / 2);

    merge_sort(begin, middle, comparator);
    merge_sort(middle, end, comparator);
    auto&& merged = merge(begin, middle, end, comparator);

    std::move(merged.begin(), merged.end(), begin);
}

int mainZaj6() {
    std::vector<int> vec(40, 0);

    srand(static_cast<unsigned>(time(0)));
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

    std::vector<std::string> s_vec{"abz", "ab", "z", "zz", "mmb", "d", "za", "e", "mma", "blel"};

    merge_sort(s_vec.begin(), s_vec.end(), [](const std::string& a, const std::string& b) {
        int i = 0;
        for (; a[i] != '\0' && b[i] != '\0' && a[i] == b[i]; i++)
            ;

        return a[i] < b[i];
    });

    for (const auto& str : s_vec) {
        std::cout << str << "  ";
    }

    return 0;
}
