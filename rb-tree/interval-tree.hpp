#ifndef __RB_TREE_HXX
#define __RB_TREE_HXX

#if __cplusplus < 201103L
#error "C++11 required"
#endif // Version check

#include <iostream>
#include <vector>
#include <stack>

namespace iBug {
    typedef enum RBColor {RB_BLACK = 0, RB_RED = 1} RBColor;

    template <typename T>
    struct Node {
        RBColor color;
        T key, upper, maxr;
        Node *left, *right, *p;
        Node() : color(RB_BLACK), left(nullptr), right(nullptr), p(nullptr) {}
        Node(const T& value, const T& upper) : key(value), upper(upper), maxr(upper) {}
        Node(const T& value) : Node(value, value) {}
        ~Node() = default;
        bool operator==(const Node& other) const {
            return reinterpret_cast<const char*>(this) ==
                   reinterpret_cast<const char*>(&other);
        }
        inline bool operator!=(const Node& other) const { return !(*this == other); }
        operator T&(void) { return key; }
    };

    template <typename T>
    class IntervalTree {
        public:
        using Node = ::iBug::Node<T>;

        private:
        Node *nil, *root;

        public:
        IntervalTree(void);
        ~IntervalTree(void);
        IntervalTree(const IntervalTree<T>&) = delete;
        IntervalTree& operator=(const IntervalTree<T>&) = delete;

        Node* getRoot() const { return root; }
        bool empty() const { return root == nil; }

        Node* search(Node*, const T&);
        Node* search(const T& value) { return search(root, value); };
        Node* minimum(Node*);
        Node* minimum() { return minimum(this->root); };
        Node* maximum(Node*);
        Node* maximum() { return maximum(this->root); };
        Node* predecessor(Node*);
        Node* successor(Node*);

        IntervalTree& leftRotate(Node*);
        IntervalTree& rightRotate(Node*);
        IntervalTree& insert(Node*);
        IntervalTree& insert(const T& value, const T& upper) { return insert(new Node(value, upper)); }
        IntervalTree& transplant(Node*, Node*);
        IntervalTree& pop(Node*);

        std::ostream& print(std::ostream&, bool color = false) const;

        private:
        void insertFixup(Node*);
        void deleteFixup(Node*);

        void print(std::ostream&, const Node*, std::vector<int>&, bool) const;
    };

    template <typename T> IntervalTree<T>::IntervalTree(void) {
        nil = new Node;
        nil->color = RB_BLACK;
        root = nil;
    }

    template <typename T> IntervalTree<T>::~IntervalTree() {
        std::stack<Node*> stack;
        if (root != nil)
            stack.push(root);
        while (!stack.empty()) {
            auto *top = stack.top();
            stack.pop();
            if (top->left != nil)
                stack.push(top->left);
            if (top->right != nil)
                stack.push(top->right);
            delete top;
        }
        delete nil;
    }

    template <typename T> typename IntervalTree<T>::Node* IntervalTree<T>::search(Node *x, const T& value) {
        if (x == nil)
            return nullptr;
        if (x->key == value)
            return x;
        if (x->key < value)
            return search(x->right, value);
        return search(x->left, value);
    }

    template <typename T> typename IntervalTree<T>::Node* IntervalTree<T>::minimum(Node* x) {
        if (x == nil)
            return nullptr;
        while (x->left != nil)
            x = x->left;
        return x;
    }

    template <typename T> typename IntervalTree<T>::Node* IntervalTree<T>::maximum(Node* x) {
        if (x == nil)
            return nullptr;
        while (x->right != nil)
            x = x->right;
        return x;
    }

    template <typename T> typename IntervalTree<T>::Node* IntervalTree<T>::predecessor(Node* x) {
        if (x->left != nil)
            return maximum(x->left);
        auto *y = x->p;
        while (y != nil && x == y->left) {
            x = y;
            y = y->p;
        }
        if (y == nil)
            return nullptr;
        return y;
    }

    template <typename T> typename IntervalTree<T>::Node* IntervalTree<T>::successor(Node* x) {
        if (x->right != nil)
            return minimum(x->right);
        auto *y = x->p;
        while (y != nil && x == y->right) {
            x = y;
            y = y->p;
        }
        if (y == nil)
            return nullptr;
        return y;
    }

    template <typename T> IntervalTree<T>& IntervalTree<T>::leftRotate(Node* x) {
        auto y = x->right;
        x->right = y->left;
        if (y->left != nil)
            y->left->p = x;
        y->p = x->p;
        if (x->p == nil)
            root = y;
        else if (x == x->p->left)
            x->p->left = y;
        else
            x->p->right = y;
        y->left = x;
        x->p = y;
        y->maxr = x->maxr;
        x->maxr = x->upper;
        if (x->left != nil && x->left->maxr > x->maxr)
            x->maxr = x->left->maxr;
        if (x->right != nil && x->right->maxr > x->maxr)
            x->maxr = x->right->maxr;
        return *this;
    }

    template <typename T> IntervalTree<T>& IntervalTree<T>::rightRotate(Node* x) {
        auto y = x->left;
        x->left = y->right;
        if (y->right != nil)
            y->right->p = x;
        y->p = x->p;
        if (x->p == nil)
            root = y;
        else if (x == x->p->right)
            x->p->right = y;
        else
            x->p->left = y;
        y->right = x;
        x->p = y;
        y->maxr = x->maxr;
        x->maxr = x->upper;
        if (x->right != nil && x->right->maxr > x->maxr)
            x->maxr = x->right->maxr;
        if (x->left != nil && x->left->maxr > x->maxr)
            x->maxr = x->left->maxr;
        return *this;
    }

    template <typename T> IntervalTree<T>& IntervalTree<T>::insert(Node* z) {
        auto *y = nil, *x = root;
        while (x != nil) {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
            if (z->upper > y->maxr)
                y->maxr = z->upper;
        }
        z->p = y;
        if (y == nil)
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;
        z->left = nil;
        z->right = nil;
        z->color = RB_RED;
        insertFixup(z);
        return *this;
    }

    template <typename T> void IntervalTree<T>::insertFixup(Node* z) {
        while (z->p->color == RB_RED) {
            if (z->p == z->p->p->left) {
                auto y = z->p->p->right;
                if (y->color == RB_RED) {
                    z->p->color = RB_BLACK;
                    y->color = RB_BLACK;
                    z->p->p->color = RB_RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->right) {
                        z = z->p;
                        leftRotate(z);
                    }
                    z->p->color = RB_BLACK;
                    z->p->p->color = RB_RED;
                    rightRotate(z->p->p);
                }
            } else {
                auto y = z->p->p->left;
                if (y->color == RB_RED) {
                    z->p->color = RB_BLACK;
                    y->color = RB_BLACK;
                    z->p->p->color = RB_RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->left) {
                        z = z->p;
                        rightRotate(z);
                    }
                    z->p->color = RB_BLACK;
                    z->p->p->color = RB_RED;
                    leftRotate(z->p->p);
                }
            }
        }
        root->color = RB_BLACK;
    }

    template <typename T> IntervalTree<T>& IntervalTree<T>::transplant(Node* u, Node* v) {
        if (u->p == nil)
            root = v;
        else if (u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;
        v->p = u->p;
        return *this;
    }

    template <typename T> IntervalTree<T>& IntervalTree<T>::pop(Node* z) {
        auto y_original_color = z->color;
        auto x = z;
        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        } else {
            auto y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->p == z)
                x->p = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->p = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
        }
        delete z;
        for (z = x->p; z != nil; z = z->p) {
            z->maxr = z->upper;
            if (z->left != nil && z->left->maxr > z->maxr)
                z->maxr = z->left->maxr;
            if (z->right != nil && z->right->maxr > z->maxr)
                z->maxr = z->right->maxr;
        }
        if (y_original_color == RB_BLACK)
            deleteFixup(x);
        return *this;
    }

    template <typename T> void IntervalTree<T>::deleteFixup(Node* x) {
        while (x != root && x->color == RB_BLACK) {
            if (x == x->p->left) {
                auto w = x->p->right;
                if (w->color == RB_RED) {
                    w->color = RB_BLACK;
                    x->p->color = RB_RED;
                    leftRotate(x->p);
                    w = x->p->right;
                }
                if (w->left->color == RB_BLACK && w->right->color == RB_BLACK) {
                    w->color = RB_RED;
                    x = x->p;
                } else {
                    if (w->right->color == RB_BLACK) {
                        w->left->color = RB_BLACK;
                        w->color = RB_RED;
                        rightRotate(w);
                        w = x->p->right;
                    }
                    w->color = x->p->color;
                    x->p->color = RB_BLACK;
                    w->right->color = RB_BLACK;
                    leftRotate(x->p);
                    x = root;
                }
            } else {
                auto w = x->p->left;
                if (w->color == RB_RED) {
                    w->color = RB_BLACK;
                    x->p->color = RB_RED;
                    rightRotate(x->p);
                    w = x->p->left;
                }
                if (w->right->color == RB_BLACK && w->left->color == RB_BLACK) {
                    w->color = RB_RED;
                    x = x->p;
                } else {
                    if (w->left->color == RB_BLACK) {
                        w->right->color = RB_BLACK;
                        w->color = RB_RED;
                        leftRotate(w);
                        w = x->p->left;
                    }
                    w->color = x->p->color;
                    x->p->color = RB_BLACK;
                    w->left->color = RB_BLACK;
                    rightRotate(x->p);
                    x = root;
                }
            }
        }
        x->color = RB_BLACK;
    }

    template <typename T>
    std::ostream& IntervalTree<T>::print(std::ostream& os, bool color) const {
        std::vector<int> v;  // 0 - left, 1 - right
        print(os, root, v, color);
        return os;
    }

    template <typename T>
    void IntervalTree<T>::print(std::ostream& os, const Node* node, std::vector<int>& v, bool color) const {
        if (node && node != nil) {
            v.push_back(0);
            print(os, node->left, v, color);
            v.pop_back();
            for (int i = 1; i < v.size(); i++) {
                if (v[i] == v[i - 1])
                    os << "   ";
                else
                    os << "|  ";
            }
            if (v.size())
                os << "+--";
            if (color) {
                os << (node->color == RB_BLACK ? "\x1B[1m" : "\x1B[31;1m") << "[" << node->key << ", " << node->upper << "]" << "\x1B[0m" << " (" << node->maxr << ")" << std::endl;
            } else {
                os << "[" << node->key << ", " << node->upper << "] (" << node->maxr << ")" << std::endl;
            }
            v.push_back(1);
            print(os, node->right, v, color);
            v.pop_back();
        }
    }
}

#endif /* __RB_TREE_HXX */
