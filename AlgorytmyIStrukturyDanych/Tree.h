#include <functional>
#include <iostream>

template<typename T>
class Tree {
public:
    struct Node {
        Node* left;
        Node* right;
        Node* parent;

        T value;
        Node() : value(), left(nullptr), right(nullptr), parent(nullptr) {}
        Node(const T& val) : value(val), left(nullptr), right(nullptr), parent(nullptr) {}

        void addLeft(const T& value) {
            left = new Node(value);
            left->parent = this;
        }

        void addRight(const T& value) {
            right = new Node(value);
            right->parent = this;
        }

        ~Node() {
            if (left) delete left;
            if (right) delete right;
        }
    };

    using visitor_fn = std::function<void(Node&, const unsigned)>;

    Tree() : root(nullptr) {}

    void traverseInorder(visitor_fn visitor) {
        traverseInorder(root, visitor, 0);
    }
    void traversePostorder(visitor_fn visitor) {
        traversePostorder(root, visitor, 0);
    }
    void traversePreorder(visitor_fn visitor) {
        traversePreorder(root, visitor, 0);
    }

    ~Tree() {
        if (root) delete root;
    }

    void addRootNode(const T& value) {
        root = new Node(value);
    }

    Node* getRoot() {
        return root;
    }

private:
    Node* root;

    void traverseInorder(Node* current, visitor_fn& visitor, const unsigned level) {
        if (current->left) traverseInorder(current->left, visitor, level + 1);
        visitor(*current, level);
        if (current->right) traverseInorder(current->right, visitor, level + 1);
    }

    void traversePostorder(Node* current, visitor_fn& visitor, const unsigned level) {
        if (current->left) traversePostorder(current->left, visitor, level + 1);
        if (current->right) traversePostorder(current->right, visitor, level + 1);
        visitor(*current, level);
    }

    void traversePreorder(Node* current, visitor_fn& visitor, const unsigned level) {
        visitor(*current, level);
        if (current->left) traversePreorder(current->left, visitor, level + 1);
        if (current->right) traversePreorder(current->right, visitor, level + 1);
    }
};
