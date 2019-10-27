#pragma once
#include "LinkedList.h"
#include <algorithm>

template <typename T>
class PriorityQueue {
    LinkedList<std::pair<int, T>> data;

public:
    PriorityQueue(){};
    ~PriorityQueue(){};

    void push(T& value, const int priority) {
        auto previous = LinkedList<std::pair<int, T>>::iterator(nullptr);
        auto current = data.begin();
        auto end = data.end();

        for (; current != end; ++current) {
            if ((*current).first < priority) break;
            previous = current;
        }

        data.insert(previous, {priority, value});
    }

    void pop() {
        data.remove(data.size() - 1);
    }

    unsigned size() const {
        return data.size();
    }

    friend std::ostream& operator<<(std::ostream& out, const PriorityQueue<T>& q) {
        out << "in -> ";
        for (auto el : q.data) {
            out << "(" << el.first << ")[ " << el.second << " ] ";
        }
        out << " -> out";
        return out;
    }
};
