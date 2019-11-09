#include "LinkedList.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <locale>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

struct Record {
    std::string name;
    unsigned year;
    std::string club;
    std::string country;

    Record(){};
    Record(std::string name, unsigned year, std::string club, std::string country)
        : name(name), year(year), club(club), country(country){};

    friend std::ostream& operator<<(std::ostream& out, const Record& r) {
        out << std::setw(30) << std::left << r.name << std::setw(10) << r.year << std::setw(30)
            << std::left << r.club << std::setw(30) << std::left << r.country;
        return out;
    }
};

std::ostream& operator<<(std::ostream& out, const LinkedList<Record>& list) {
    out << std::left << std::setw(30) << "NAME" << std::left << std::setw(10) << "YEAR"
        << std::setw(30) << std::left << "CLUB" << std::setw(30) << std::left << "COUNTRY"
        << std::endl;

    for (const auto& el : list) {
        out << el << std::endl;
    }

    return out;
}

static inline void printByCountry(const LinkedList<Record>& l, const std::string& country) {
    LinkedList<Record> result;

    std::copy_if(l.begin(), l.end(), std::front_inserter(result), [&country](const Record& r) {
        return r.country == country;
    });

    if (result.size())
        std::cout << result;
    else
        std::cout << "Nothin'" << std::endl;
}

static inline void printClubs(const LinkedList<Record>& list) {
    std::map<std::string, unsigned> member_counts;

    for (const auto& element : list) {
        auto iterator = member_counts.find(element.club);

        if (iterator == member_counts.end()) {
            member_counts.insert({element.club, 1});
        }
        else {
            iterator->second++;
        }
    }

    std::cout << std::left << std::setw(30) << "CLUB" << std::left << std::setw(15)
              << "MEMBER COUNT" << std::endl;
    for (const auto& entry : member_counts) {
        std::cout << std::left << std::setw(30) << entry.first << std::left << std::setw(4)
                  << entry.second << std::endl;
    }
}

static inline void printByYear(const LinkedList<Record>& list, unsigned year) {
    auto result =
        std::find_if(list.begin(), list.end(), [year](const Record& r) { return r.year == year; });

    if (result != list.end())
        std::cout << *result << std::endl;
    else
        std::cout << "Nothin'" << std::endl;
}

static inline void trim(std::string& s) {
    // trim left
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));

    // trim right
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(),
        s.end());
}

void loadFromFile(LinkedList<Record>& list, const char* filename) {
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Could not open the file" << std::endl;
        return;
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

    file.close();
}

int mainZaj3() {
    // set terminal encoding to display UTF=8 file contents properly
    std::system("chcp 65001 >NUL");

    LinkedList<Record> list;
    loadFromFile(list, "data/data3.txt");

    std::cout << "Wszycy czlonkowie:" << std::endl << list << std::endl;

    std::string input;
    std::cout << "Podaj kraj po ktorym przefiltrowac: ";
    std::getline(std::cin, input);
    std::cout << std::endl;
    printByCountry(list, input);

    std::cout << std::endl << "Kluby:" << std::endl;
    printClubs(list);

    std::cout << std::endl << "Podaj rok z ktorego wyswietlic zwyciezce: ";

    unsigned year;
    std::cin >> year;
    std::cout << std::endl;

    printByYear(list, year);

    return 0;
}
