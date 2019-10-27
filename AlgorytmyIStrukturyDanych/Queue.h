#pragma once
#include "LinkedList.h"

template <typename T>
class Queue {
    LinkedList<T> data;

public:
    Queue() : data(){};
    Queue(std::initializer_list<T> args) : data(args){};

    void push(T val) {
        data.push_front(val);
    }

    void push(std::initializer_list<T> vals) {
        data.push_front(vals);
    }

    void pop() {
        data.remove(data.size() - 1);
    }

    unsigned size() const {
        return data.size();
    }

    ~Queue(){};

    friend std::ostream& operator<<(std::ostream& out, const Queue<T>& q) {
        out << "in -> " << q.data << " -> out";
        return out;
    }
};
