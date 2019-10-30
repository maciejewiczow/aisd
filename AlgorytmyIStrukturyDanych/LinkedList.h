#pragma once
#include <iostream>

template <typename T>
class LinkedList {
    struct Node {
        Node* next;
        T value;

        Node(const T value) : value(value), next(nullptr){};
    };

    Node* head;

public:
    // STL container traits
    using value_type = T;
    using size_type = unsigned;
    using reference = T&;
    using const_reference = const T&;

private:
    size_type m_size;

    template <bool is_const = true>
    class Iterator {
        Node* current;

    public:
        // iterator traits
        using iterator_category = std::forward_iterator_tag;
        using value_type = typename std::conditional<is_const, const T, T>::type;
        using difference_type = int;
        using pointer = typename std::conditional<is_const, const T*, T*>::type;
        using reference = typename std::conditional<is_const, const T&, T&>::type;

        Iterator(Node* ptr) : current(ptr){};
        Iterator(const Iterator<false>& other) : current(other.current){};

        Iterator<is_const>& operator++() {
            if (current) current = current->next;
            return *this;
        };

        Iterator operator++(int) {
            Iterator temp(*this);
            current = current->next;
            return temp;
        };

        bool operator==(const Iterator& rhs) const {
            return current == rhs.current;
        };

        bool operator!=(const Iterator& rhs) const {
            return current != rhs.current;
        };

        reference operator*() {
            return current->value;
        };

        reference operator*() const {
            return current->value;
        }

        reference operator->() {
            return current->value;
        }

        reference operator->() const {
            return current->value;
        }

        friend class Iterator<false>;
        friend class LinkedList<T>;
    };

public:
    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    LinkedList() : head(nullptr), m_size(0){};

    LinkedList(const std::initializer_list<T> values) {
        push_front(values);
    }

    explicit LinkedList(const std::size_t count, const value_type val) {
        push_front(count, val);
    }

    ~LinkedList() {
        while (head) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    };

    size_type size() const {
        return m_size;
    }

    void push_front(const std::size_t count, const value_type val) {
        for (int i = 0; i < count; i++)
            push_front(val);
    }

    void push_front(const std::initializer_list<T> values) {
        for (auto it = std::rbegin(values), end = std::rend(values); it != end; ++it)
            push_front(*it);
    }

    void push_front(const T& value) {
        Node* tmp = new Node(value);
        tmp->next = head;
        head = tmp;
        m_size++;
    };

    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }

    const_iterator begin() const {
        return const_iterator(head);
    }

    const_iterator end() const {
        return const_iterator(nullptr);
    }

    void insert(const iterator& it, const value_type& val) {
        if (!it.current) {
            push_front(val);
            return;
        }

        Node* insertMe = new Node(val);

        insertMe->next = it.current->next;
        it.current->next = insertMe;
        m_size++;
    }

    void remove(const int index) {
        if (!head) return;

        Node* previous = nullptr;
        Node* current_head = head;

        int i;
        for (i = 0; (i < index) && current_head->next; i++) {
            previous = current_head;
            current_head = current_head->next;
        }

        if (index > i) return;

        Node* next = current_head->next;
        delete current_head;
        m_size--;
        if (previous) previous->next = next;
        if (index == 0) head = next;
    };

    friend std::ostream& operator<<(std::ostream& out, const LinkedList<T>& l) {
        for (auto& value : l) {
            out << value << std::endl;
        }
        return out;
    };
};
