#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "Tree.h"

int mainZaj4() {
    Tree<int> tr;

    std::fstream file("./data/lista.txt");

    if (!file) {
        std::cerr << "Could not open the file" << std::endl;
        return -1;
    }

    int item;
    file >> item;
    tr.addRootNode(item);

    std::srand(static_cast<unsigned>(std::time(0)));

    tr.traversePreorder([&](Tree<int>::Node& n, unsigned level) {
        bool goRight = std::rand() % 2;

        if (!file.eof()) {
            int item;
            file >> item;

            if (goRight) {
                n.addRight(item);
            }
            else {
                n.addLeft(item);
            }
        }
    });

    tr.traverseInorder([](Tree<int>::Node& n, unsigned level) {
        for (unsigned i = 0; i < level; i++) {
            std::cout << "    ";
        }
        std::cout << n.value << std::endl;
    });

    return 0;
}
