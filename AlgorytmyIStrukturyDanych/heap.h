#include "basic_tree.h"
#include <functional>
#include <queue>
#include <vector>

namespace tree {
    template<typename T, template<class> class comparator_T = std::less>
    class heap : public basic_tree<T> {
    public:
        using Node = basic_tree<T>::Node;

    private:
        using super = basic_tree<T>;
        using super::root;

        comparator_T<T> compare;
        std::queue<Node*> insertion_q;
        unsigned m_size;

        template<bool is_const = true>
        class m_iterator {
            Node* current;
            Node* root;
            std::queue<Node*> traversal_path;

        public:
            // Iterator traits
            using iterator_category = std::forward_iterator_tag;
            using value_type = typename std::conditional<is_const, const T, T>::type;
            using difference_type = int;
            using pointer = typename std::conditional<is_const, const T*, T*>::type;
            using reference = typename std::conditional<is_const, const T&, T&>::type;

            m_iterator() : current(nullptr), root(nullptr), traversal_path() {}
            m_iterator(Node* ptr, const heap<T, comparator_T>* cont)
                : current(ptr), root(cont->root), traversal_path() {}
            m_iterator(const heap<T, comparator_T>& cont)
                : current(cont.root), root(cont.root), traversal_path() {}

            template<bool is_other_const>
            m_iterator(const m_iterator<is_other_const>& other)
                : current(other.current), root(other.root), traversal_path(other.traversal_path){};

            template<bool is_other_const>
            m_iterator(m_iterator<is_other_const>&& other)
                : current(other.current), root(other.root),
                  traversal_path(std::move(other.traversal_path)){};

            template<bool is_other_const>
            m_iterator& operator=(const m_iterator<is_other_const>& other) {
                if (this != &other) {
                    current = other.current;
                    traversal_path = other.traversal_path;
                    root = other.root;
                }
                return *this;
            }

            template<bool is_other_const>
            m_iterator& operator=(m_iterator<is_other_const>&& other) {
                if (this != &other) {
                    current = other.current;
                    traversal_path = std::move(other.traversal_path);
                    root = other.root;
                }
                return *this;
            }

            m_iterator& operator++() {
                if (traversal_path.size()) {

                    current = traversal_path.front();
                    traversal_path.pop();

                    if (current->left) traversal_path.push(current->left);
                    if (current->right) traversal_path.push(current->right);
                }
                else if (current == root && (root->left || root->right)) {
                    if (current->left) traversal_path.push(current->left);
                    if (current->right) traversal_path.push(current->right);
                }
                else {
                    current = nullptr;
                }

                return *this;
            }

            m_iterator operator++(int) {
                m_iterator copy{*this};

                this->operator++();

                return copy;
            }

            bool operator==(const m_iterator& other) {
                return current == other.current;
            }

            bool operator!=(const m_iterator& other) {
                return current != other.current;
            }

            reference operator*() {
                return current->value;
            }

            reference operator*() const {
                return current->value;
            }

            pointer operator->() {
                return &current->value;
            }

            pointer operator->() const {
                return &current->value;
            }

            friend class m_iterator<false>;
            friend class heap<T, comparator_T>;
        };

        Node* get_last_node() {
            Node* last = nullptr;
            auto begin_it = begin();
            auto end_it = end();

            for (; begin_it != end_it; ++begin_it) last = begin_it.current;

            if (last == nullptr) return root;
            return last;
        }

        void upheap_from(Node* inserted) {
            while (inserted->parent && compare(inserted->value, inserted->parent->value)) {
                std::swap(inserted->value, inserted->parent->value);
                inserted = inserted->parent;
            }
        }

        Node* max_node(Node* a, Node* b) {
            if (a == nullptr) return b;
            if (b == nullptr) return a;
            if (a == nullptr && b == nullptr) return nullptr;

            return compare(a->value, b->value) ? a : b;
        }

    public:
        using iterator = m_iterator<false>;
        using const_iterator = m_iterator<true>;
        using reverse_iterator = std::reverse_iterator<m_iterator<false>>;
        using const_reverse_iterator = std::reverse_iterator<m_iterator<true>>;

        heap() : super(), compare(), m_size(0){};
        heap(comparator_T<T> pred) : super(), compare(pred), m_size(0) {}

        template<typename iter_T>
        heap(iter_T begin, iter_T end) : super(), compare(), m_size(0) {
            insert(begin, end);
        }

        unsigned size() {
            return m_size;
        }

        void insert(T&& value) {
            m_size++;
            Node* insert_me = new Node{std::forward<T>(value)};

            if (!root)
                root = insert_me;
            else {
                Node* front = insertion_q.front();

                if (!front->left) {
                    front->add_left(insert_me);
                    upheap_from(insert_me);
                }
                else if (!front->right) {
                    front->add_right(insert_me);
                    upheap_from(insert_me);
                }

                if (front->left && front->right) insertion_q.pop();
            }

            insertion_q.push(insert_me);
        }

        void insert(const T& value) {
            m_size++;
            Node* insert_me = new Node{value};

            if (!root)
                root = insert_me;
            else {
                Node* front = insertion_q.front();

                if (!front->left) {
                    front->add_left(insert_me);
                    upheap_from(insert_me);
                }
                else if (!front->right) {
                    front->add_right(insert_me);
                    upheap_from(insert_me);
                }

                if (front->left && front->right) insertion_q.pop();
            }

            insertion_q.push(insert_me);
        }

        // iter_T = std::vector<int>::iterator
        template<typename iter_T>
        void insert(iter_T begin, iter_T end) {
            while (begin != end) insert(*begin++);
        }

        iterator begin() {
            return iterator{root, this};
        }

        const_iterator begin() const {
            return const_iterator{root, this};
        }

        iterator end() {
            return iterator{};
        }

        const_iterator end() const {
            return const_iterator{};
        }

        T pop_root() {
            if (!root) return T{};

            T temp{std::move(root->value)};

            Node* last = get_last_node();
            if (last != root) {
                root->value = std::move(last->value);

                // detatch node
                Node* parent = last->parent;
                if (last == parent->left) {
                    parent->left = nullptr;
                }
                else {
                    parent->right = nullptr;
                }

                Node* tmp = root;
                Node* max = root;

                while (tmp) {
                    max = max_node(tmp, tmp->left);
                    max = max_node(tmp->right, max);

                    if (max == tmp) break;

                    if (max) std::swap(tmp->value, max->value);

                    tmp = max;
                }

                delete last;
            }
            else {
                delete root;
                root = nullptr;
            }

            m_size--;

            return temp;
        }
    };
} // namespace tree
