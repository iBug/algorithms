#ifndef __RB_TREE_HXX
#define __RB_TREE_HXX

#if __cplusplus < 201103L
#error "C++11 required"
#endif // Version check

namespace iBug {
    enum RBColorT {RB_BLACK = 0, RB_RED = 1};
    using RBColor = enum RBColorT;

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
            bool operator!=(const Node& other) const { return !(*this == other); }
        };

        private:
        Node *nil, *root;

        public:
        RBTree(void);
        ~RBTree(void);
        RBTree(const RBTree<T>&) = delete;
        RBTree& operator=(const RBTree<T>&) = delete;

        RBTree& leftRotate(Node& x);
        RBTree& rightRotate(Node& x);
        RBTree& insert(Node& x);
        RBTree& insert(const T& value) { return insert(*(new Node(value))); }
        void insertFixup(Node& x);
    };

    template <typename T> RBTree<T>::RBTree(void) {
        nil = new Node;
        nil->color = RB_BLACK;
    }

    template <typename T> RBTree<T>::~RBTree() {
        delete nil;
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
    }
}

#endif /* __RB_TREE_HXX */
