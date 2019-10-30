#include "CommandInterpreter.h"
#include "PriorityQueue.h"
#include "Queue.h"
#include "Stack.h"
#include <cstdlib>
#include <iostream>
#include <string>

using argv_it = CommandInterpreter::argv_iterator;
using Handler = CommandInterpreter::Handler;

template <class ContT>
CommandInterpreter::callback pushValues(ContT& container) {
    return [&](argv_it begin, argv_it end) {
        for (; begin != end; ++begin) {
            container.push(*begin);
        }
    };
}

template <class ContT>
CommandInterpreter::callback popValue(ContT& container) {
    return [&](argv_it b, argv_it e) { container.pop(); };
}

template <class ContT>
CommandInterpreter::callback clearContainer(ContT& container) {
    return [&](argv_it b, argv_it e) {
        while (container.size() > 0)
            container.pop();
    };
};

int mainZaj2() {
    Queue<std::string> q;
    PriorityQueue<std::string> pq;
    Stack<std::string> s;

    CommandInterpreter::callback push_to_p_queue = [&](argv_it begin, argv_it end) {
        for (; begin != end; ++begin) {
            std::string item = *begin;

            if (++begin == end)
                throw CommandInterpreter::HandlerException(
                    "Error, no priority found for last item");

            try {
                const int priority = std::stoi(*begin);
                pq.push(item, priority);
            }
            catch (std::invalid_argument&) {
                throw CommandInterpreter::HandlerException("Invalid priority value");
            }
        }
    };

    CommandInterpreter stack_subtree{
        {"push", Handler(pushValues(s), "dodaje elementy na szczyt stosu", "")},
        {"pop", Handler(popValue(s), "zdejmuje element ze stosu", "")},
        {"clear", Handler(clearContainer(s), "usuwa wszystkie elementy ze stosu", "")},
    };

    CommandInterpreter queue_subtree{
        {"push", Handler(pushValues(q), "dodaje element do kolejki", "")},
        {"pop", Handler(popValue(q), "usuwa element z kolejki", "")},
        {"clear", Handler(clearContainer(q), "usuwa wszystkie elementy z kolejki", "")},
    };

    CommandInterpreter priority_queue_subtree{
        {"push", Handler(push_to_p_queue, "dodaje element do kolejki priorytetowej", "")},
        {"pop", Handler(popValue(pq), "usuwa element z kolejki priorytetowej", "")},
        {"clear",
            Handler(clearContainer(pq), "usuwa wszystkie elementy z kolejki priorytetowej", "")},
    };

    CommandInterpreter root;

    root.onCommandSuccess([&](const std::string& cmd_name) {
        if (cmd_name == "help") return;

        std::system("cls");

        std::cout << "Queue:" << std::endl << "\t" << q << std::endl;
        std::cout << "Priotrity queue:" << std::endl << "\t" << pq << std::endl;
        std::cout << "Stack:" << std::endl << "\t" << s << std::endl << std::endl;
    });

    root.on("stack", stack_subtree, "steruje stosem");
    root.on("queue", queue_subtree, "steruje kolejka");
    root.on("p_queue", priority_queue_subtree, "steruje kolejka priorytetowa");

    root.promptUntilExit();

    return 0;
}
