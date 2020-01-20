#include <fstream>
#include <iostream>

#include "graph.h"

int mainZaj10() {
    // std::ifstream file{"graf_przechodzenie.txt"};
    std::ifstream file{"test_graf.txt"};

    if (!file) {
        std::cerr << "Could not open the file uwu" << std::endl;
        return -1;
    }

    graph::basic_graph gr{file};
    auto printing_visitor = [](int vert) { std::cout << vert << " "; };

    std::cout << "Depth first:\n";
    gr.traverse_depth_first(printing_visitor, 4);
    std::cout << "\n\n";

    std::cout << "Breadth first:\n";
    gr.traverse_breadth_first(printing_visitor);
    std::cout << "\n";

    return 0;
}
