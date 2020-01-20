#pragma once
#include <array>
#include <charconv>
#include <exception>
#include <fstream>
#include <functional>
#include <queue>
#include <string>
#include <vector>

void split_string(const std::string& source,
    std::vector<std::string_view>& dest,
    char delim = '\n');

namespace graph {

    class disjoint_set_union {
        std::vector<int> parent;
        std::vector<int> rank;

    public:
        explicit disjoint_set_union(std::size_t set_count) {
            parent.resize(set_count);
            rank.resize(set_count);

            for (int i = 0; i < set_count; i++) {
                parent[i] = i;
                rank[i] = 0;
            }
        }

        int find_set(int v) {
            if (v == parent[v]) return v;
            return parent[v] = find_set(parent[v]);
        }

        void union_sets(int a, int b) {
            a = find_set(a);
            b = find_set(b);
            if (a != b) {
                if (rank[a] < rank[b]) std::swap(a, b);
                parent[b] = a;
                if (rank[a] == rank[b]) rank[a]++;
            }
        }
    };

    class basic_graph {
        std::vector<std::vector<int>> matrix;
        std::vector<int> verticies;

        void traverse_depth_first(int start_vertex,
            std::function<void(int)>& visitor,
            std::vector<bool>& visited) {

            visited[start_vertex] = true;
            visitor(verticies[start_vertex]);

            int i = 0;
            for (auto element : matrix[start_vertex]) {
                if (element && !visited[i]) traverse_depth_first(i, visitor, visited);
                i++;
            }
        }

        struct edge {
            int start, end, weight;

            edge() : start(0), end(0), weight(0) {}
            edge(int start, int end, int weight) : start(start), end(end), weight(weight) {}
        };

    public:
        basic_graph() : matrix(), verticies() {}

        basic_graph(basic_graph&& other)
            : matrix(std::move(other.matrix)), verticies(std::move(other.verticies)) {}

        basic_graph(std::ifstream& file) : matrix(), verticies() {
            auto size = static_cast<std::size_t>(
                std::count(std::istreambuf_iterator(file), std::istreambuf_iterator<char>(), '\n'));

            // clear file state and rewind to the beginning after line counting
            file.clear();
            file.seekg(0);

            if (size == 0) return;

            matrix.resize(size);
            verticies.reserve(size);

            for (auto& vec : matrix) vec.reserve(size);

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
                    int weight;
                    std::from_chars(token.data(), token.data() + token.size(), weight);

                    matrix[row].push_back(weight);
                }

                row++;
                tokens.clear();
            }
        }

        void print_neighbours() {
            for (int i = 0; i < verticies.size(); i++) {
                std::cout << verticies[i] << ": ";

                int j = 0;
                for (auto el : matrix[i]) {
                    if (el) std::cout << verticies[j] << " ";

                    j++;
                }

                std::cout << std::endl;
            }
        }

        bool has_euler_cycle() {
            for (int i = 0; i < verticies.size(); i++) {
                int row_count = 0;

                for (auto el : matrix[i])
                    if (el) row_count++;

                if (row_count % 2 != 0 || row_count == 0) return false;
            }

            return true;
        }

        void traverse_depth_first(std::function<void(int)> visitor, int start_index = 0) {
            std::vector<bool> visited(verticies.size(), false);

            /*for (int i = 0; i < verticies.size(); i++)
                if (!visited[i])*/
            traverse_depth_first(start_index, visitor, visited);
        }

        void traverse_breadth_first(std::function<void(int)> visitor, int start_index = 0) {
            std::vector<bool> visited(verticies.size(), false);

            std::queue<int> to_process;

            visited[start_index] = true;
            to_process.push(start_index);

            while (!to_process.empty()) {
                int current = to_process.front();

                visitor(verticies[current]);
                to_process.pop();

                int i = 0;
                for (auto is_adjacent : matrix[current]) {
                    if (is_adjacent && !visited[i]) {
                        visited[i] = true;
                        to_process.push(i);
                    }
                    i++;
                }
            }
        }

        std::list<edge> minimal_spanning_tree() {
            std::list<edge> edges;
            // use only upper-right triangle of matrix to avoid creating two edges for each
            // direction
            for (int i = 0; i < matrix.size(); i++) {
                for (int j = 0; j < i; j++)
                    if (matrix[i][j] != 0) edges.emplace_back(i, j, matrix[i][j]);
            }
            edges.sort([](const edge& a, const edge& b) { return a.weight < b.weight; });

            disjoint_set_union dsu{edges.size()};
            std::list<edge> result;
            for (const auto& edge : edges) {
                if (dsu.find_set(edge.start) != dsu.find_set(edge.end)) {
                    result.push_back(edge);
                    dsu.union_sets(edge.start, edge.end);
                }
            }

            return result;
        }
    };
} // namespace graph
