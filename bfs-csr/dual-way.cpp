#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <array>
#include <deque>
using namespace std;

#define NV 4039
#define NE 88234

const char *filename = "facebook_combined.txt";
array<int, NV + 1> IA;
array<int, 2 * NE> JA;

void read_data(ifstream& fin) {
    array<vector<int>, NV> edges;
    // Taking advantage of sorted input
    for (int i = 0; i < NE; i++) {
        int a, b;
        fin >> a >> b;
        edges[a].push_back(b);
        edges[b].push_back(a);
    }

    // Populate IA and JA
    IA[0] = 0;
    for (int i = 0; i < NV; i++)
        IA[i + 1] = IA[i] + edges[i].size();

    auto iJ = JA.begin();
    for (auto &v : edges)
        for (auto &e : v)
            *iJ++ = e;
}

int main() {
    ios_base::sync_with_stdio(false);
    ifstream fin(filename);
    read_data(fin);
    fin.close();

    int source, target;
    //cout << "Enter source and target: ";
    cin >> source >> target;
    if (source < 0 || source >= NV || target < 0 || target >= NV) {
        cerr << "Invalid node!" << endl;
        return 1;
    }

    deque<int> q;
    deque<int> modes; // 0 - forward, 1 - backward
    vector<int> pred(NV, -1);
    vector<int> next(NV, -1);
    pred[source] = source;
    next[target] = target;

    int intersect = -1;
    q.push_back(source);
    q.push_back(target);
    modes.push_back(0);
    modes.push_back(1);
    while (!q.empty()) {
        auto node = q.front();
        auto mode = modes.front();
        q.pop_front();
        modes.pop_front();

        // Lookup neighbours of [node]
        for (int i = IA[node]; i < IA[node + 1]; i++) {
            auto &n = JA[i];
            if (mode == 0) {
                if (pred[n] != -1)
                    continue;
                pred[n] = node;
            } else {
                if (next[n] != -1)
                    continue;
                next[n] = node;
            }
            if (pred[n] != -1 && next[n] != -1) {
                intersect = n;
                break;
            }
            q.push_back(n);
            modes.push_back(mode);
        }
        if (intersect != -1)
            break;
    }

    q.clear();
    int asd = 0;
    for (auto node = intersect; node != source; node = pred[node])
        q.push_back(node);
    for (auto node = next[intersect]; node != target; node = next[node])
        q.push_front(node);
    q.push_front(target);
    cout << source;
    while (!q.empty()) {
        cout << " -> " << q.back();
        q.pop_back();
    }
    cout << endl;

    return 0;
}
