#include <fstream>
#include <iostream>
#include <string>

#include "Tree.h"
#include "main.h"

int mainZaj4() {
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
    root->left->left->left->right->right->add_left("u");
    root->left->left->left->right->right->left->add_left("w");
    root->left->left->left->right->right->left->left->add_left("a");
    root->left->left->left->right->right->left->left->left->add_left("g");
    root->left->left->left->right->right->left->left->left->left->add_left("a");
    root->left->left->left->right->right->left->left->left->left->left->add_left("aa");
    root->left->left->left->right->right->add_right("h");
    root->left->add_right("D");
    root->left->right->add_right("DD");

    root->right->add_left("E");
    root->right->add_right("F");

    auto print_in_row = [](auto& n, unsigned level) { std::cout << n.value << " "; };

    dżewo.set_traversal_mode(tree::traversal_mode::pre_order);
    dżewo.traverse(print_in_row);
    std::cout << std::endl;

    dżewo.set_traversal_mode(tree::traversal_mode::post_order);
    dżewo.traverse(print_in_row);
    std::cout << std::endl;

    /*for (const auto& val : dżewo) {
        std::cout << val << " ";
    }*/

    return 0;
}
