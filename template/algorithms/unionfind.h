// UnionFind.h
#pragma once

#include <vector>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;
public:
    UnionFind(int size);
    int find(int x);
    void unionSet(int x, int y);
    bool connected(int x, int y);
};
