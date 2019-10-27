#pragma once
#include "LinkedList.h"
#include <stack>

template <typename T>
class Stack {
    LinkedList<T> data;

public:
    Stack() : data(){};
    Stack(std::initializer_list<T> args) : data(args) {}

    void pop() {
        data.remove(0);
    }

    void push(T val) {
        data.push_front(val);
    }

    void push(std::initializer_list<T> vals) {
        data.push_front(vals);
    }

    T& top() {
        return *data.begin();
    }

    unsigned size() const {
        return data.size();
    }

    ~Stack(){};

    friend std::ostream& operator<<(std::ostream& out, const Stack<T>& s) {
        out << s.data;
        return out;
    }
};
