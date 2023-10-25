#include "UnionFind.h"
using namespace std;

UnionFind::UnionFind(int size) {
    parent.resize(size);
    rank.resize(size, 0);
    for (int i = 0; i < size; i++) {
        parent[i] = i;
    }
}

int UnionFind::find(int x) {
    if (parent[x] == x) {
        return x;
    }
    return parent[x] = find(parent[x]);
}

void UnionFind::unionSet(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    if (rootX == rootY)
        return;

    if (rank[rootX] > rank[rootY])
        swap(rootX, rootY);

    parent[rootX] = rootY;

    if (rank[rootY] == rank[rootX])
        rank[rootX]++;
}

bool UnionFind::connected(int x, int y) {
    return find(x) == find(y);
}

// class UnionFind {
//   private:
//     vector<int> parent;
//     vector<int> rank;

//   public:
//     UnionFind(int size) {
//         parent.resize(size);
//         rank.resize(size, 0);
//         for (int i = 0; i < size; i++)
//             parent[i] = i;
//     }

//     int find(int x) {
//         if (parent[x] == x)
//             return x;

//         return parent[x] = find(parent[x]);
//     }

//     void unionSet(int x, int y) {
//         int rootX = find(x);
//         int rootY = find(y);
//         if (rootX == rootY)
//             return;

//         if (rank[rootY] > rank[rootX]) {
//             parent[rootX] = rootY;
//         } else if (rank[rootY] < rank[rootX]) {
//             parent[rootY] = rootX;
//         } else {
//             parent[rootY] = rootX;
//             rank[rootX]++;
//         }
//     }

//     bool isConnected(int x, int y) {
//         return find(x) == find(y);
//     }
// };