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
    cin >> source >> target;
    if (source < 0 || source >= NV || target < 0 || target >= NV) {
        cerr << "Invalid node!" << endl;
        return 1;
    }

    deque<int> q;
    vector<bool> visited(NV, false);
    vector<int> pred(NV, -1);

    q.push_back(source);
    while (!q.empty()) {
        auto node = q.front();
        q.pop_front();

        // Lookup neighbours of [node]
        for (int i = IA[node]; i < IA[node + 1]; i++) {
            auto next = JA[i];
            if (visited[next])
                continue;
            pred[next] = node;
            visited[next] = true;
            q.push_back(next);
        }
    }

    for (auto node = target; node != source; node = pred[node])
        q.push_back(node);
    cout << source;
    while (!q.empty()) {
        cout << " -> " << q.back();
        q.pop_back();
    }
    cout << endl;

    return 0;
}
