#include <functional>
#include <memory>
#include <string>

namespace tree {
    enum class traversal_mode {
        pre_order,
        in_order,
        post_order,
    };

    template<typename T>
    class basic_tree {
    public:
        // STL container member types
        using value_type = T;
        using size_type = std::size_t;
        using reference = T&;
        using const_reference = const T&;

        struct Node {
            Node* left;
            Node* right;
            Node* parent;
            value_type value;

            Node() : value(), left(nullptr), right(nullptr), parent(nullptr) {}
            Node(const_reference val)
                : value(val), left(nullptr), right(nullptr), parent(nullptr) {}

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
        };

        using visitor_fn = std::function<void(Node&, const unsigned)>;

        basic_tree() : root(nullptr), mode(traversal_mode::in_order) {}

        void traverse(visitor_fn visitor) {
            switch (mode) {
                case traversal_mode::pre_order:
                    traversePreorder(root, visitor, 0);
                    break;
                case traversal_mode::in_order:
                    traverseInorder(root, visitor, 0);
                    break;
                case traversal_mode::post_order:
                    traversePostorder(root, visitor, 0);
                    break;
                default:
                    break;
            }
        }

        ~basic_tree() {
            if (root) delete root;
        }

        void addRootNode(const_reference value) {
            root = new Node(value);
        }

        void set_traversal_mode(const traversal_mode t) {
            mode = t;
        }

        Node* getRoot() {
            return root;
        }

    private:
        Node* root;
        traversal_mode mode;

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

        template<bool is_const = true>
        class Iterator {
            Node* current;
            traversal_mode mode;

            void init() {
                // in preorder we start at the root
                if (mode == traversal_mode::pre_order) return;

                // else we start at the leftmost node
                while (current->left) {
                    current = current->left;
                }
            }

            Node* next_preorder() {
                if (!current) return nullptr;

                if (current->left) return current->left;
                if (current->right) return current->right;

                // now only leafs remain
                if (current->parent) {
                    Node* tmp = current;
                    // we are a left child
                    if (current->parent->left == current) {
                        // find next node upwards that has a right child
                        while (tmp && !tmp->right) {
                            tmp = tmp->parent;
                        }
                        return tmp->right;
                    }

                    // we are a right child
                    if (current->parent->right == current) {
                        // go up until next left turn
                        while (tmp && tmp->parent->right == tmp) {
                            tmp = tmp->parent;
                        }
                        tmp = tmp->parent;
                        // then find next node with a right child
                        while (tmp && !tmp->right) {
                            tmp = tmp->parent;
                        }

                        return tmp->right;
                    }
                }
            }
            // TODO: implement post and in-order
            Node* next_inorder() {
                return nullptr;
            }
            Node* next_postorder() {
                return nullptr;
            }

            Node* prev_preorder() {
                return nullptr;
            }
            Node* prev_inorder() {
                return nullptr;
            }
            Node* prev_postorder() {
                return nullptr;
            }

        public:
            // Iterator traits
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = typename std::conditional<is_const, const T, T>::type;
            using difference_type = int;
            using pointer = typename std::conditional<is_const, const T*, T*>::type;
            using reference = typename std::conditional<is_const, const T&, T&>::type;

            Iterator() : current(nullptr) {}
            Iterator(Node* ptr) : current(ptr) {
                if (current) init();
            }
            Iterator(Node* ptr, traversal_mode mode) : current(ptr), mode(mode) {
                if (current) init();
            }
            Iterator(traversal_mode mode) : current(nullptr), mode(mode) {}
            ~Iterator() {}

            // these ctors and assignments permit conversion from non-const to const iterator
            Iterator(const Iterator<false>& other) : current(other.current), mode(other.mode) {}
            Iterator(Iterator<false>&& other) : current(other.current), mode(other.mode) {}

            Iterator& operator=(const Iterator<false>& rhs) {
                current = rhs.current;
                mode = rhs.mode;
                return *this;
            }

            Iterator& operator=(Iterator<false>&& rhs) {
                current = rhs.current;
                mode = rhs.mode;
                return *this;
            }

            Iterator& operator++() {
                switch (mode) {
                    case traversal_mode::pre_order:
                        current = next_preorder();
                        break;
                    case traversal_mode::in_order:
                        current = next_inorder();
                        break;
                    case traversal_mode::post_order:
                        current = next_postorder();
                        break;
                }

                return *this;
            };
            Iterator operator++(int) {
                Iterator copy{*this};

                this->operator++();

                return copy;
            };

            Iterator& operator--() {
                switch (mode) {
                    case traversal_mode::pre_order:
                        current = prev_preorder();
                        break;
                    case traversal_mode::in_order:
                        current = prev_inorder();
                        break;
                    case traversal_mode::post_order:
                        current = prev_postorder();
                        break;
                }

                return *this;
            };
            Iterator operator--(int) {
                Iterator copy{*this};

                this->operator--();

                return copy;
            };

            bool operator==(const Iterator& rhs) const {
                return current == rhs.current;
            }

            bool operator!=(const Iterator& rhs) const {
                return current != rhs.current;
            }

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

            void insert_left(const T& val) {
                if (current) current->add_left(val);
            }

            void insert_right(const T& val) {
                if (current) current->add_right(val);
            }

            friend class Iterator<false>;
        };

    public:
        using iterator = Iterator<false>;
        using const_iterator = Iterator<true>;

        iterator begin() {
            return iterator{root, mode};
        }

        const_iterator begin() const {
            return const_iterator{root, mode};
        }

        iterator end() {
            return iterator{nullptr, mode};
        }

        const_iterator end() const {
            return const_iterator{nullptr, mode};
        }
    };
} // namespace tree
