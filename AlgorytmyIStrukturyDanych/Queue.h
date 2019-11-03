#pragma once
#include "LinkedList.h"

template<typename T>
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

    typename LinkedList<T>::size_type size() const {
        return data.size();
    }

    ~Queue(){};

    friend std::ostream& operator<<(std::ostream& out, const Queue<T>& q) {
        out << "in -> ";
        for (const auto& el : q.data) {
            out << "[ " << el << " ] ";
        }
        out << " -> out";
        return out;
    }
};
