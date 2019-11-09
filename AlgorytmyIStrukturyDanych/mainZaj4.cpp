#include <cmath>
#include <iostream>
#include <string>

#include "Tree.h"
#include "main.h"

int mainZaj4() {
    Tree<std::string> tr;

    tr.addRootNode("root");

    tr.getRoot()->addLeft("A");
    tr.getRoot()->addRight("B");

    tr.getRoot()->left->addLeft("C");
    tr.getRoot()->left->addRight("D");

    tr.getRoot()->right->addLeft("E");
    tr.getRoot()->right->addRight("F");

    tr.traversePostorder([](Tree<std::string>::Node& n, unsigned level) {
        for (unsigned i = 0; i < level; i++) {
            std::cout << "    ";
        }
        std::cout << n.value << std::endl;
    });

    return 0;
}
