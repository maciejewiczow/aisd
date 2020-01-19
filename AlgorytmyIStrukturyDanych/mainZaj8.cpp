#include <algorithm>
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <typename iter_T>
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
    while (*peak_position <= peak && peak_position != end)
        ++peak_position;

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

struct Record {
    double amount;
    std::string title, from, to;

    Record() : amount(0), title(), from(), to() {}
    Record(const std::vector<std::string_view>& tokens) {
        title = tokens[0];
        std::from_chars(tokens[1].data(), tokens[1].data() + tokens[1].size(), amount);
        from = tokens[2];
        to = tokens[3];
    }

    Record(const Record&) = default;
    Record& operator=(const Record&) = default;
    /* Record(Record&& other) = default;
     Record& operator=(Record&& rhs) = default;*/

    Record(Record&& other)
        : amount(other.amount), from(std::move(other.from)), to(std::move(other.to)),
          title(std::move(other.title)) {}

    Record& operator=(Record&& rhs) {
        if (&rhs != this) {
            amount = rhs.amount;
            from = std::move(rhs.from);
            to = std::move(rhs.to);
            title = std::move(rhs.title);
        }
        return *this;
    }

    ~Record() {
        std::cout << "Record destroyed" << std::endl;
    }
};

void split_string(const std::string& source,
    std::vector<std::string_view>& dest,
    char delim = '\n') {
    std::string_view source_view = source;

    auto start = source_view.cbegin();
    auto end = source_view.cend();

    char delim_arr[] = {delim, '\0'};

    while (start < end) {
        auto match_end = std::find_first_of(start, end, delim_arr, delim_arr + 1);

        auto match = source_view.substr(
            std::distance(source_view.cbegin(), start), std::distance(start, match_end));

        if (match.size() > 0) dest.push_back(match);

        if (match_end == end) break;
        start = std::next(match_end);
    }
}

void load_records_from_file(std::vector<Record>& dest, std::ifstream& file) {
    dest.reserve(
        std::count(std::istreambuf_iterator(file), std::istreambuf_iterator<char>(), '\n'));

    // clear file state and rewind to the beginning after line counting
    file.clear();
    file.seekg(0);

    std::string line;
    std::vector<std::string_view> tokens;
    tokens.reserve(4);

    while (std::getline(file, line)) {
        split_string(line, tokens, ',');

        dest.emplace_back(tokens);

        tokens.clear();
    }
}

int mainZaj8() {
    std::vector<double> args{
        -3.0, -2.2, -2.0, -1.8, -1.6, -1.0, -0.7, -0.4, 0.0, 0.4, 0.8, 1.0, 1.5, 2.0, 2.3};

    auto result = sort_quadratic_values(args.begin(), args.end(), 1, -1, 0);

    std::vector<Record> records;
    std::ifstream rec_file{"records.csv"};

    if (!rec_file) {
        std::cerr << "Could not open records file! Aborting" << std::endl;
        return -1;
    }

    load_records_from_file(records, rec_file);

    rec_file.close();

    std::nth_element(records.begin(),
        records.begin() + 1000,
        records.end(),
        [](const Record& a, const Record& b) { return a.amount > b.amount; });

    std::cout << "Top 1000 records:" << std::endl;
    std::cout << std::left << std::setw(30) << "kwota" << std::left << std::setw(40) << "od"
              << std::left << std::setw(40) << "do" << std::left << std::setw(100) << "tytulem "
              << std::endl;

    for (auto current = records.begin(); current != (records.begin() + 1000); ++current) {
        std::cout << std::left << std::setw(30) << std::fixed << std::setprecision(2)
                  << current->amount << std::left << std::setw(40) << current->from << std::left
                  << std::setw(40) << current->to << std::left << std::setw(100) << current->title
                  << std::endl;
    }

    return 0;
}
