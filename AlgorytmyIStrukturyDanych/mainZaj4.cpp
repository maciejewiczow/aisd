#include <fstream>
#include <iostream>
#include <string>

#include "Tree.h"
#include "main.h"

int mainZaj4() {
    // std::fstream file("./data/lista.txt");

    /*if (!file) {
        std::cerr << "Could not open the file" << std::endl;
        return -1;
    }

    int item;
    file >> item;
    dżewo.addRootNode(item);

    while (!file.eof()) {
        file >> item;
    }*/

    tree::basic_tree<std::string> dżewo;
    dżewo.addRootNode("root");

    auto root = dżewo.getRoot();

    root->add_left("A");
    root->add_right("B");

    root->left->add_left("C");
    root->left->left->add_left("E");
    root->left->left->left->add_left("e");
    root->left->left->left->add_right("f");
    root->left->left->left->right->add_right("g");
    root->left->left->left->right->right->add_left("p");
    root->left->left->left->right->right->left->add_left("a");
    root->left->left->left->right->right->left->left->add_left("p");
    root->left->left->left->right->right->left->left->left->add_left("i");
    root->left->left->left->right->right->left->left->left->left->add_left("e");
    root->left->left->left->right->right->left->left->left->left->left->add_left("rz");
    root->left->left->left->right->right->add_right("h");
    root->left->add_right("D");
    root->left->right->add_right("DD");

    root->right->add_left("E");
    root->right->add_right("F");

    dżewo.set_traversal_mode(tree::traversal_mode::in_order);

    /*for (auto it = dżewo.begin(), end = dżewo.end(); it != end; ++it) {
    }*/

    dżewo.traverse([&](auto& n, unsigned level) {
        for (unsigned i = 0; i < level; i++) {
            std::cout << "    ";
        }

        std::cout << n.value << std::endl;
    });

    return 0;
}
