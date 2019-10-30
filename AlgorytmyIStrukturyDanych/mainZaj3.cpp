#include "CommandInterpreter.h"
#include "LinkedList.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

struct Record {
    std::string name;
    unsigned year;
    std::string city;
    std::string country;

    Record(){};
    Record(std::string name, unsigned year, std::string city, std::string country)
        : name(name), year(year), city(city), country(country){};

    friend std::ostream& operator<<(std::ostream& out, const Record& r) {
        out << "name: " << r.name << ", year: " << r.year << ", city: " << r.city
            << ", country: " << r.country;

        return out;
    }
};

void printNamesByCountry(const LinkedList<Record>& l, const std::string& country) {
    for (const auto& record : l) {
        if (record.country == country) {
            std::cout << record << std::endl;
        }
    }
}

static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(),
        s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

using argv_it = CommandInterpreter::argv_iterator;
using Handler = CommandInterpreter::Handler;

int mainZaj3() {
    LinkedList<Record> list;
    std::ifstream file("./data/data3.txt");

    if (!file) {
        std::cerr << "Could not open the file";
        return -1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream line_stream(line);
        std::string current_token;

        std::string tokens[4];
        for (int i = 0; i < 3; i++) {
            if (!std::getline(line_stream, current_token, ',')) break;
            trim(current_token);
            tokens[i] = current_token;
        }
        std::getline(line_stream, tokens[3]);
        trim(tokens[3]);
        list.push_front(Record(tokens[0], std::stoul(tokens[1]), tokens[2], tokens[3]));
    }

    CommandInterpreter root{
        {"print_by_country",
            Handler(
                [&](argv_it begin, argv_it end) {
                    if (begin == end)
                        throw CommandInterpreter::HandlerException(
                            "This command needs country name as an argument");

                    printNamesByCountry(list, *begin);
                },
                "",
                "")},
        {"print_all", Handler([&](argv_it begin, argv_it end) { std::cout << list; }, "", "")},
    };

    root.promptUntilExit();

    return 0;
}
