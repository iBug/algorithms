#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <array>
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

    return 0;
}
