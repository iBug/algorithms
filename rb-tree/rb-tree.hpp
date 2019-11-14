#ifndef __RB_TREE_HXX
#define __RB_TREE_HXX

#if __cplusplus < 201103L
#error "C++11 required"
#endif // Version check

#include <stack>

namespace iBug {
    typedef enum RBColor {RB_BLACK = 0, RB_RED = 1} RBColor;

    template <typename T>
    class RBTree {
        private:
        struct Node {
            RBColor color;
            T key;
            Node *left, *right, *p;
            Node() = default;
            Node(const T& value) : key(value) {}
            ~Node() = default;
            bool operator==(const Node& other) const {
                return reinterpret_cast<const char*>(this) ==
                       reinterpret_cast<const char*>(&other);
            }
            inline bool operator!=(const Node& other) const { return !(*this == other); }
            T& operator T(void) const { return key; }
        };

        private:
        Node *nil, *root;

        public:
        RBTree(void);
        ~RBTree(void);
        RBTree(const RBTree<T>&) = delete;
        RBTree& operator=(const RBTree<T>&) = delete;

        Node& minimum(const Node& x);
        Node& minimum() { return minimum(this->root); };
        Node& maximum(const Node& x);
        Node& maximum() { return maximum(this->root); };

        RBTree& leftRotate(Node&);
        RBTree& rightRotate(Node&);
        RBTree& insert(Node&);
        RBTree& insert(const T& value) { return insert(*(new Node(value))); }
        void insertFixup(Node&);
        RBTree& transplant(Node&, Node&);
        RBTree& pop(Node&);
        void deleteFixup(Node&);
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
            auto *top = stack.pop();
            if (top->left != nil)
                stack.push(top->left);
            if (top->right != nil)
                stack.push(top->right);
            delete top;
        }
        delete nil;
    }

    template <typename T> typename RBTree<T>::Node& RBTree<T>::minimum(const Node& x) {
        if (&x == nil)
            return *nil;
        auto *px = &x;
        while (px->left != nil)
            px = px->left;
        return *px;
    }

    template <typename T> typename RBTree<T>::Node& RBTree<T>::maximum(const Node& x) {
        if (&x == nil)
            return *nil;
        auto *px = &x;
        while (px->right != nil)
            px = px->right;
        return *px;
    }

    template <typename T> RBTree<T>& RBTree<T>::leftRotate(Node& x) {
        auto &y = *x.right;
        x.right = y.left;
        if (y.left != nil)
            y.left->p = x;
        y.p = x.p;
        if (x.p == nil)
            root = y;
        else if (x == x.p->left)
            x.p->left = y;
        else
            x.p->right = y;
        y.left = x;
        x.p = y;
        return *this;
    }

    template <typename T> RBTree<T>& RBTree<T>::rightRotate(Node& x) {
        auto &y = *x.left;
        x.left = y.right;
        if (y.right != nil)
            y.right->p = x;
        y.p = x.p;
        if (x.p == nil)
            root = y;
        else if (x == x.p->right)
            x.p->right = y;
        else
            x.p->left = y;
        y.right = x;
        x.p = y;
        return *this;
    }

    template <typename T> RBTree<T>& RBTree<T>::insert(Node& z) {
        auto *y = nil, *x = root;
        while (x != nil) {
            y = x;
            if (z.key < x->key)
                x = x->left;
            else
                x = x->right;
        }
        z.p = y;
        if (y == nil)
            root = &z;
        else if (z.key < y->key)
            y->left = &z;
        else
            y->right = &z;
        z.left = nil;
        z.right = nil;
        z.color = RB_RED;
        insertFixup(z);
        return *this;
    }

    template <typename T> void RBTree<T>::insertFixup(Node& z) {
        auto *pz = &z;
        while (pz->p->color == RB_RED) {
            if (pz->p == pz->p->p->left) {
                auto &y = pz->p->p->right;
                if (y.color == RB_RED) {
                    pz->p->color = RB_BLACK;
                    y.color = RB_BLACK;
                    pz->p->p->color = RB_RED;
                    pz = pz->p->p;
                } else {
                    if (pz == pz->p->right) {
                        pz = pz->p;
                        leftRotate(*pz);
                    }
                    pz->p->color = RB_BLACK;
                    pz->p->p->color = RB_RED;
                    rightRotate(*pz->p->p);
                }
            } else {
                auto &y = pz->p->p->left;
                if (y.color == RB_RED) {
                    pz->p->color = RB_BLACK;
                    y.color = RB_BLACK;
                    pz->p->p->color = RB_RED;
                    pz = pz->p->p;
                } else {
                    if (pz == pz->p->left) {
                        pz = pz->p;
                        rightRotate(*pz);
                    }
                    pz->p->color = RB_BLACK;
                    pz->p->p->color = RB_RED;
                    leftRotate(*pz->p->p);
                }
            }
            root->color = RB_BLACK;
        }
    }

    template <typename T> RBTree<T>& RBTree<T>::transplant(Node& u, Node& v) {
        if (*u.p == *nil)
            root = v;
        else if (u == *u.p->left)
            u.p->left = v;
        else
            u.p->right = v;
        v.p = u.p;
    }

    template <typename T> RBTree<T>& RBTree<T>::pop(Node& z) {
        auto y_original_color = z.color;
        auto *px = &z;
        if (z.left == nil) {
            px = z.right;
            transplant(z, *z.right);
        } else if (z.right == nil) {
            px = z.left;
            transplant(z, *z.left);
        } else {
            auto &y = minimum(z);
            y_original_color = y.color;
            auto &x = *y.right;
            if (y.p == &z)
                x.p = &y;
            else {
                transplant(y, *y.right);
                y.right = z.right;
                y.right->p = &y;
            }
            transplant(z, y);
            y.left = z.left;
            y.left->p = &y;
            y.color = z.color;
        }
        if (y_original_color == RB_BLACK)
            deleteFixup(*px);
        return *this;
    }

    template <typename T> void RBTree<T>::deleteFixup(Node& x) {
        auto *px = &x;
        while (px != root && px->color == RB_BLACK) {
            if (px == px->p->left) {
                auto *pw = px->p->right;
                if (pw->color == RB_RED) {
                    pw->color = RB_BLACK;
                    px->p->color = RB_RED;
                    leftRotate(*px->p);
                    pw = px->p->right;
                }
                if (pw->left->color == RB_BLACK && pw->right->color == RB_BLACK) {
                    pw->color = RB_RED;
                    px = px->p;
                } else {
                    if (pw->right->color == RB_BLACK) {
                        pw->left->color = RB_BLACK;
                        pw->color = RB_RED;
                        rightRotate(*pw);
                        pw = px->p->right;
                    }
                    pw->color = px->p->color;
                    px->p->color = RB_BLACK;
                    pw->right->color = RB_BLACK;
                    leftRotate(*px->p);
                    px = root;
                }
            } else {
                auto *pw = px->p->left;
                if (pw->color == RB_RED) {
                    pw->color = RB_BLACK;
                    px->p->color = RB_RED;
                    rightRotate(*px->p);
                    pw = px->p->left;
                }
                if (pw->right->color == RB_BLACK && pw->left->color == RB_BLACK) {
                    pw->color = RB_RED;
                    px = px->p;
                } else {
                    if (pw->left->color == RB_BLACK) {
                        pw->right->color = RB_BLACK;
                        pw->color = RB_RED;
                        leftRotate(*pw);
                        pw = px->p->left;
                    }
                    pw->color = px->p->color;
                    px->p->color = RB_BLACK;
                    pw->left->color = RB_BLACK;
                    rightRotate(*px->p);
                    px = root;
                }
            }
        }
        px->color = RB_BLACK;
    }
}

#endif /* __RB_TREE_HXX */
