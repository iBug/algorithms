#include <iostream>
#include <random>
#include <climits>
#include "rb-tree.hpp"

using namespace std;
using iBug::RBTree;

int main() {
    RBTree<int> tree;
    random_device rdev;
    mt19937 generator(rdev());
    uniform_int_distribution<mt19937::result_type> rand(0, INT_MAX);
    const int count = 10 + rand(generator) % 31;
    for (int i = 0; i < count; i++) {
        tree.insert(rand(generator) % 999 + 1);
    }
    tree.print(cout, 1);
    for (;;) {
        int num;
        cout << "Enter number to delete: ";
        cin >> num;
        if (!cin) {
            cout << "Exit" << endl;
            return 0;
        }
        auto *node = tree.search(num);
        if (node == nullptr)
            cout << "Not found" << endl;
        else {
            tree.pop(node);
            tree.print(cout, 1);
            if (tree.empty()) {
                cout << "Nothing left" << endl;
                return 0;
            }
        }
    }
    return 0;
}
