#pragma once
#include <functional>
#include <iostream>

namespace tree {

    template<typename T>
    class BSTree {
        struct Node;

    public:
        using value_type = T;
        using size_type = std::size_t;
        using reference = T&;
        using const_reference = const T&;

        using node_t = Node;

        using visitor_fn = std::function<void(value_type&)>;

    private:
        struct Node {
            T value;

            Node() : value(), left(nullptr), right(nullptr), parent(nullptr) {}
            Node(const_reference value)
                : value(value), left(nullptr), right(nullptr), parent(nullptr) {}

            void add_left(const_reference value) {
                left = new Node(value);
                left->parent = this;
            }

            void add_right(const_reference value) {
                right = new Node(value);
                right->parent = this;
            }

            ~Node() {
                if (left) delete left;
                if (right) delete right;
            }

            bool is_detached() {
                return !left && !right && !parent;
            }

            Node* left;
            Node* right;
            Node* parent;
        };

        friend struct Node;

        Node* root;

        Node* min(Node* root_node) {
            while (root_node->left) {
                root_node = root_node->left;
            }

            return root_node;
        }

        Node* max(Node* root_node) {
            while (root_node->right) {
                root_node = root_node->right;
            }

            return root_node;
        }

        void traverseInorder(Node* current, visitor_fn& visitor) {
            if (current->left) traverseInorder(current->left, visitor);
            visitor(current->value);
            if (current->right) traverseInorder(current->right, visitor);
        }

    public:
        BSTree() : root(nullptr) {}
        ~BSTree() {
            if (root) delete root;
        }

        void insert(const_reference value) {
            if (!root) {
                root = new Node(value);
                return;
            }

            Node* parent = nullptr;
            Node* child = root;

            while (child) {
                parent = child;
                if (parent->value > value) {
                    child = parent->left;
                }
                else {
                    child = parent->right;
                }
            }

            if (parent->value > value)
                parent->add_left(value);
            else
                parent->add_right(value);
        }

        Node* find(const_reference value) {
            if (!root) return nullptr;

            Node* parent = nullptr;
            Node* child = root;
            while (child) {
                parent = child;

                if (parent->value > value) {
                    child = parent->left;
                }
                else {
                    child = parent->right;
                }
            }
        }

        void detach(Node* target) {
            if (target->is_detached()) return;

            if (target) Node* parent = target->parent;
        }

        void detach_min() {}

        Node* detach_max() {
            Node* max_node = max();

            if (max_node->left) {
                Node* max_from_subtree = max(max_node->parent->left);
                max_from_subtree->right = max_node->left;
            }

            max_node->parent->right = nullptr;

            max_node->left = max_node->right = max_node->parent = nullptr;

            return max_node;
        }

        void remove(Node* target) {
            detach(target);
            delete target;
        }

        Node* min() {
            return min(root);
        }

        Node* max() {
            return max(root);
        }

        Node* next(Node* target) {
            if (target->right) return min(target->right);
        }

        Node* prev(Node* target) {}

        void traverseInorder(visitor_fn visitor) {
            traverseInorder(root, visitor);
        }
    }; // namespace tree

    template<typename T>
    class priority_queue {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using reference = T&;
        using const_reference = const T&;

    private:
        struct Node {
            int priority;
            T value;

            Node() : value(), priority(0) {}
            Node(const_reference value, int priority) : value(value), priority(priority) {}
            Node(const Node& other) : value(other.value), priority(other.priority) {}
            Node(Node&& other) : value(std::move(other.value)), priority(other.priority) {}
            ~Node() {}

            bool operator>(const Node& rhs) {
                return priority > rhs.priority;
            }

            bool operator<(const Node& rhs) {
                return priority < rhs.priority;
            }

            bool operator>=(const Node& rhs) {
                return priority >= rhs.priority;
            }

            bool operator<=(const Node& rhs) {
                return priority <= rhs.priority;
            }

            bool operator==(const Node& rhs) {
                return priority == rhs.priority;
            }

            bool operator!=(const Node& rhs) {
                return priority != rhs.priority;
            }
        };

        BSTree<Node> data;

        using tree_node = typename BSTree<Node>::node_t;
        using queue_node = Node;

    public:
        priority_queue() {}
        ~priority_queue() {}

        void push(const_reference value, int priority) {
            data.insert(Node(value, priority));
        }

        value_type pop() {
            tree_node* max_n = data.detach_max();
            value_type n = std::move(max_n->value.value);
            delete max_n;
            return std::move(n);
        }

        friend std::ostream& operator<<(std::ostream& out, priority_queue<T>& q) {
            q.data.traverseInorder(
                [&](auto& node) { out << "(" << node.priority << ") " << node.value << "  "; });

            return out;
        }
    };
} // namespace tree
