#pragma once
#include <charconv>
#include <exception>
#include <fstream>
#include <string>
#include <vector>

void split_string(const std::string& source,
    std::vector<std::string_view>& dest,
    char delim = '\n');

namespace graph {

    class basic_graph {
        std::vector<std::vector<bool>> matrix;
        std::vector<int> verticies;

    public:
        basic_graph() : matrix(), verticies() {}

        basic_graph(basic_graph&& other)
            : matrix(std::move(other.matrix)), verticies(std::move(other.verticies)) {}

        basic_graph(std::ifstream& file) : matrix(), verticies() {
            int size =
                std::count(std::istreambuf_iterator(file), std::istreambuf_iterator<char>(), '\n');

            // clear file state and rewind to the beginning after line counting
            file.clear();
            file.seekg(0);

            if (size == 0) return;

            matrix.resize(size);

            for (auto& row : matrix) {
                row.reserve(size);
            }

            std::string line;
            std::getline(file, line);

            std::vector<std::string_view> tokens;
            tokens.reserve(size);

            split_string(line, tokens, ' ');
            for (const auto& token : tokens) {
                int value;
                std::from_chars(token.data(), token.data() + token.size(), value);
                verticies.push_back(value);
            }

            if (size < verticies.size())
                throw std::length_error("File does not contain entries for all verticies");

            tokens.clear();

            int row = 0;
            while (std::getline(file, line)) {
                split_string(line, tokens, ' ');

                for (const auto& token : tokens) {
                    int value;
                    std::from_chars(token.data(), token.data() + token.size(), value);

                    matrix[row].push_back(value == 1 ? true : false);
                }

                row++;
                tokens.clear();
            }
        }

        void print_neighbours() {
            for (int i = 0; i < verticies.size(); i++) {
                std::cout << verticies[i] << ": ";

                for (int j = 0; j < verticies.size(); j++) {
                    if (matrix[i][j]) {
                        std::cout << verticies[j] << " ";
                    }
                }

                std::cout << std::endl;
            };
        }

        bool has_euler_cycle() {
            for (int i = 0; i < verticies.size(); i++) {
                int row_count = 0;

                for (const auto& el : matrix[i])
                    if (el) row_count++;

                if (row_count % 2 != 0 || row_count == 0) return false;
            }

            return true;
        }
    };
} // namespace graph
