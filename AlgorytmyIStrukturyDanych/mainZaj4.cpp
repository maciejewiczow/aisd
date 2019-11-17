#include <fstream>
#include <iostream>
#include <string>

#include "Tree.h"
#include "main.h"

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

    tr.traversePreorder([&](Tree<int>::Node& n, unsigned level) {
        static bool goright = false;

        if (!file.eof()) {
            int item;
            file >> item;

            if (goright) {
                n.addRight(item);
                goright = false;
            }
            else {
                n.addLeft(item);
                goright = true;
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
