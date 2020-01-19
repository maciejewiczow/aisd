#include <fstream>
#include <iostream>

#include "graph.h"

int mainZaj9() {
    std::ifstream file{"test_graf.txt"};

    if (!file) {
        std::cerr << "Could not open the file uwu" << std::endl;
        return -1;
    }

    graph::basic_graph gr{file};

    gr.print_neighbours();

    std::cout << std::endl;

    if (gr.has_euler_cycle()) {
        std::cout << "graf ma cykl eulera" << std::endl;
    }
    else {
        std::cout << "graf nie ma cyklu eulera" << std::endl;
    };

    return 0;
}
