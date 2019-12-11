#include <algorithm>
#include <vector>

template<typename iter_T>
std::vector<typename iter_T::value_type>
    sort_quadratic_values(iter_T begin, iter_T end, double a, double b, double c) {
    using value_T = typename iter_T::value_type;

    std::vector<value_T> buffer;
    buffer.reserve(std::distance(begin, end));

    auto quadratic = [a, b, c](value_T x) { return a * x * x + b * x + c; };

    if (a == 0) {
        if (b >= 0) {
            // copy as-is, function is ether constant or increasing
            std::copy(begin, end, std::back_inserter(buffer));
        }
        else {
            // copy in reverse order, function is decreasing
            std::copy(std::make_reverse_iterator(end),
                std::make_reverse_iterator(begin),
                std::back_inserter(buffer));
        }

        return buffer;
    }

    // calculate x positon of quadratic fn peak
    double peak = -b / (2 * a);

    // find
    iter_T peak_position{begin};
    while (*peak_position <= peak && peak_position != end) ++peak_position;

    iter_T copying_begin, copying_end;
    std::reverse_iterator<iter_T> copying_rbegin, copying_rend;

    if (a > 0) {
        // copy values before the peak as-is
        copying_begin = begin;
        copying_end = peak_position;

        // copy values after the peak in reverse order
        copying_rend = std::make_reverse_iterator(peak_position);
        copying_rbegin = std::make_reverse_iterator(end);
    }
    else {
        // copy values before the peak in reverse order
        copying_rend = std::make_reverse_iterator(begin);
        copying_rbegin = std::make_reverse_iterator(peak_position);

        // copy values after the peak as-is
        copying_begin = peak_position;
        copying_end = end;
    }

    while (copying_rbegin != copying_rend && copying_begin != copying_end) {
        if (quadratic(*copying_rbegin) >= quadratic(*copying_begin))
            buffer.push_back(*copying_rbegin++);
        else
            buffer.push_back(*copying_begin++);
    }

    buffer.insert(buffer.end(), copying_begin, copying_end);
    buffer.insert(buffer.end(), copying_rbegin, copying_rend);

    std::for_each(buffer.begin(), buffer.end(), [&](auto& x) { x = quadratic(x); });
    return buffer;
}

int mainZaj8() {
    std::vector<double> args{
        -3.0, -2.2, -2.0, -1.8, -1.6, -1.0, -0.7, -0.4, 0.0, 0.4, 0.8, 1.0, 1.5, 2.0, 2.3};

    auto result = sort_quadratic_values(args.rbegin(), args.rend(), 1, -1, 0);

    return 0;
}
