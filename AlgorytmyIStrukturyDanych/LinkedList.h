#pragma once
#include <iostream>

template<typename T>
class LinkedList {
    struct Node {
        Node* next;
        T value;

        Node() : value(), next(nullptr){};
        Node(const T value) : value(value), next(nullptr){};
    };

    Node* head;

public:
    // STL container traits
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

private:
    size_type m_size;

    template<bool is_const = true>
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
        Iterator(const Iterator<true>& other) : current(other.current){};
        Iterator(const Iterator<false>& other) : current(other.current){};
        Iterator(Iterator&& other) : current(other.current){};

        Iterator& operator=(const Iterator& other) {
            current = other.current;
            return *this;
        }

        Iterator& operator=(Iterator&& other) {
            current = other.current;
            return *this;
        }

        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        };

        Iterator operator++(int) {
            Iterator temp(*this);
            if (current) current = current->next;
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

        pointer operator->() {
            return &current->value;
        }

        pointer operator->() const {
            return &current->value;
        }

        friend class Iterator<false>;
        friend class LinkedList<T>;
    };

    void erase() {
        while (head) {
            Node* next = head->next;
            delete head;
            head = next;
        }
        m_size = 0;
    }

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

    LinkedList(const LinkedList& other) {
        std::copy(other.begin(), other.end(), std::front_inserter(*this));
    }

    LinkedList(LinkedList&& other) : head(other.head), m_size(other.m_size) {
        other.head = nullptr;
        other.m_size = 0;
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) return *this;

        erase();

        std::copy(other.begin(), other.end(), std::front_inserter(*this));

        return *this;
    }

    LinkedList& operator=(LinkedList&& other) {
        if (this == &other) return *this;

        erase();

        head = other.head;

        return *this;
    }

    ~LinkedList() {
        erase();
    };

    size_type size() const {
        return m_size;
    }

    void push_front(const std::size_t count, const T& val) {
        for (std::size_t i = 0; i < count; i++) push_front(val);
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

    /* void push_back(const std::size_t count, const T& val) {
        for (std::size_t i = 0; i < count; i++) push_back(val);
    }

    void push_back(const std::initializer_list<T>& vals) {
        for (const auto& val : vals) push_back(val);
    }

    void push_back(const T& value) {
        if (!head) {
            // empty list, just push to front
            push_front(value);
            return;
        }

        if (last == begin()) {
            for (int i = 0; i < static_cast<int>(m_size) - 1; i++) {
                ++last;
            }
        }

        insert(last, value);
        ++last;
    }
    */

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

    void remove(const size_type index) {
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
};
