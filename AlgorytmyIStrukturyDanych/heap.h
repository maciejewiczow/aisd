#include "Tree.h"
#include <functional>
#include <vector>

namespace tree {
    template<typename T, class compare_T = std::less<T>>
    class heap : public Tree<T> {
        using super = Tree<T>;
        using Node = super::Node;

        compare_T compare;

        void swap(Node* a, Node* b) {
            std::swap(a->left, b->left);
            std::swap(a->right, b->right);
            std::swap(a->parent, b->parent);
        }

        Node* max(Node* a, Node* b) {
            return compare(a->value, b->value) ? a : b;
        }

        void transform_into_heap(Node* root) {
            super::visitor_fn heapify = [this](Node& n, unsigned ignore) {
                // skip leaf nodes
                if (!n.right && !n.left) return;

                Node* max_node = nullptr;

                if (!n.left) {
                    max_node = max(&n, n.right);
                }
                else if (!n.right) {
                    max_node = max(&n, n.left);
                }
                else {
                    max_node = max(&n, n.left);
                    max_node = max(max_node, n.right);
                }

                if (max_node != &n) swap(max_node, &n);
            };

            super::traversePostorder(root, heapify, 0);
        }

    public:
        heap() : super(){};
        heap(compare_T pred) : super(), compare(pred) {}

        // iter_T = std::vector<int>::iterator
        template<typename iter_T>
        void insert(iter_T begin, iter_T end) {
            if (begin == end) return;

            super::addRootNode(*begin++);

            super::traversePreorder([&](Node& n, unsigned level) {

            });
        }
    };
} // namespace tree
