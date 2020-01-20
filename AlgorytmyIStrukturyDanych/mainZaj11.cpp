#include <fstream>
#include <iostream>

#include "graph.h"

int mainZaj11() {
    std::ifstream file{"kruskalgraf.txt"};

    if (!file) {
        std::cerr << "Could not open the file uwu" << std::endl;
        return -1;
    }

    graph::basic_graph gr{file};

    auto result = gr.minimal_spanning_tree();



    return 0;
}
