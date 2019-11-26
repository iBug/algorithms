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
    for (int i = 0; i < 10 + rand(generator) % 31; i++) {
        tree.insert(rand(generator) % 1000);
    }
    tree.print(cout, 1);
    return 0;
}
