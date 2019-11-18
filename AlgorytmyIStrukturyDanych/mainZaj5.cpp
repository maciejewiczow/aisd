#include <iostream>
#include <string>

#include "BST.h"

int mainZaj5() {
    tree::priority_queue<std::string> q;

    q.push("a", 2);
    q.push("b", 1);
    q.push("bleble", 40);

    std::cout << q << std::endl;
    std::cout << "popped: " << q.pop() << std::endl;
    std::cout << q << std::endl;

    return 0;
}
