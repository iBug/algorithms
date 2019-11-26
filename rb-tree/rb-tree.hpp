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
        T key;
        Node *left, *right, *p;
        Node() : color(RB_BLACK), left(nullptr), right(nullptr), p(nullptr) {};
        Node(const T& value) : key(value) {}
        ~Node() = default;
        bool operator==(const Node& other) const {
            return reinterpret_cast<const char*>(this) ==
                   reinterpret_cast<const char*>(&other);
        }
        inline bool operator!=(const Node& other) const { return !(*this == other); }
        operator T&(void) { return key; }
    };

    template <typename T>
    class RBTree {
        public:
        using Node = ::iBug::Node<T>;

        private:
        Node *nil, *root;

        public:
        RBTree(void);
        ~RBTree(void);
        RBTree(const RBTree<T>&) = delete;
        RBTree& operator=(const RBTree<T>&) = delete;

        const Node* getRoot() const { return root; }

        Node* minimum(const Node* x);
        Node* minimum() { return minimum(this->root); };
        Node* maximum(const Node* x);
        Node* maximum() { return maximum(this->root); };

        RBTree& leftRotate(Node*);
        RBTree& rightRotate(Node*);
        RBTree& insert(Node*);
        RBTree& insert(const T& value) { return insert(new Node(value)); }
        RBTree& transplant(Node*, Node*);
        RBTree& pop(Node*);

        std::ostream& print(std::ostream&, bool color = false) const;

        private:
        void insertFixup(Node*);
        void deleteFixup(Node*);

        void print(std::ostream&, const Node*, std::vector<int>&, bool) const;
    };

    template <typename T> RBTree<T>::RBTree(void) {
        nil = new Node;
        nil->color = RB_BLACK;
        root = nil;
    }

    template <typename T> RBTree<T>::~RBTree() {
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

    template <typename T> typename RBTree<T>::Node* RBTree<T>::minimum(const Node* x) {
        if (x == nil)
            return nullptr;
        while (x->left != nil)
            x = x->left;
        return x;
    }

    template <typename T> typename RBTree<T>::Node* RBTree<T>::maximum(const Node* x) {
        if (x == nil)
            return nullptr;
        while (x->right != nil)
            x = x->right;
        return x;
    }

    template <typename T> RBTree<T>& RBTree<T>::leftRotate(Node* x) {
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
        return *this;
    }

    template <typename T> RBTree<T>& RBTree<T>::rightRotate(Node* x) {
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
        return *this;
    }

    template <typename T> RBTree<T>& RBTree<T>::insert(Node* z) {
        auto *y = nil, *x = root;
        while (x != nil) {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
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

    template <typename T> void RBTree<T>::insertFixup(Node* z) {
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

    template <typename T> RBTree<T>& RBTree<T>::transplant(Node* u, Node* v) {
        if (u->p == nil)
            root = v;
        else if (u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;
        v->p = u->p;
    }

    template <typename T> RBTree<T>& RBTree<T>::pop(Node* z) {
        auto y_original_color = z->color;
        auto x = z;
        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        } else {
            auto y = minimum(z);
            y_original_color = y->color;
            auto x = y->right;
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
        if (y_original_color == RB_BLACK)
            deleteFixup(x);
        return *this;
    }

    template <typename T> void RBTree<T>::deleteFixup(Node* x) {
        while (x != root && x->color == RB_BLACK) {
            if (x == x->p->left) {
                auto pw = x->p->right;
                if (pw->color == RB_RED) {
                    pw->color = RB_BLACK;
                    x->p->color = RB_RED;
                    leftRotate(x->p);
                    pw = x->p->right;
                }
                if (pw->left->color == RB_BLACK && pw->right->color == RB_BLACK) {
                    pw->color = RB_RED;
                    x = x->p;
                } else {
                    if (pw->right->color == RB_BLACK) {
                        pw->left->color = RB_BLACK;
                        pw->color = RB_RED;
                        rightRotate(pw);
                        pw = x->p->right;
                    }
                    pw->color = x->p->color;
                    x->p->color = RB_BLACK;
                    pw->right->color = RB_BLACK;
                    leftRotate(x->p);
                    x = root;
                }
            } else {
                auto pw = x->p->left;
                if (pw->color == RB_RED) {
                    pw->color = RB_BLACK;
                    x->p->color = RB_RED;
                    rightRotate(x->p);
                    pw = x->p->left;
                }
                if (pw->right->color == RB_BLACK && pw->left->color == RB_BLACK) {
                    pw->color = RB_RED;
                    x = x->p;
                } else {
                    if (pw->left->color == RB_BLACK) {
                        pw->right->color = RB_BLACK;
                        pw->color = RB_RED;
                        leftRotate(pw);
                        pw = x->p->left;
                    }
                    pw->color = x->p->color;
                    x->p->color = RB_BLACK;
                    pw->left->color = RB_BLACK;
                    rightRotate(x->p);
                    x = root;
                }
            }
        }
        x->color = RB_BLACK;
    }

    template <typename T>
    std::ostream& RBTree<T>::print(std::ostream& os, bool color) const {
        std::vector<int> v;  // 0 - left, 1 - right
        print(os, root, v, color);
        return os;
    }

    template <typename T>
    void RBTree<T>::print(std::ostream& os, const Node* node, std::vector<int>& v, bool color) const {
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
                os << (node->color == RB_BLACK ? "\x1B[1m" : "\x1B[31;1m") << node->key << "\x1B[0m" << std::endl;
            } else {
                os << node->key << std::endl;
            }
            v.push_back(1);
            print(os, node->right, v, color);
            v.pop_back();
        }
    }
}

#endif /* __RB_TREE_HXX */
