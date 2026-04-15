#pragma once
#include "utils.h"
#include <set>

Result bruteForceSolve(int n_vul, vector<Patch>& patches) {
    auto start = chrono::high_resolution_clock::now();

    int    m        = patches.size();
    double bestCost = 1e18;
    int    bestMask = -1;

    for (int mask = 1; mask < (1 << m); mask++) {

        set<int> covered;
        double   cost  = 0;

        for (int i = 0; i < m; i++) {
            if (mask & (1 << i)) {
                for (int v : patches[i].covers)
                    covered.insert(v);
                cost += patches[i].cost;
            }
        }

        bool valid = ((int)covered.size() == n_vul);

        if (valid && cost < bestCost) {
            bestCost = cost;
            bestMask = mask;
        }
    }

    Result res;
    res.totalCost  = (bestMask == -1) ? -1 : bestCost;
    res.numPatches = 0;

    if (bestMask != -1) {
        for (int i = 0; i < m; i++) {
            if (bestMask & (1 << i)) {
                res.solution.push_back(patches[i].name);
                res.numPatches++;
            }
        }
    }

    auto end    = chrono::high_resolution_clock::now();
    res.runtime = chrono::duration<double>(end - start).count();

    return res;
}