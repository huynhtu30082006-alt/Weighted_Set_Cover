#pragma once
#include "utils.h"
#include <set>


Result greedySolve(int n_vul, vector<Patch>& patches) {
    auto start = chrono::high_resolution_clock::now();

    set<int> uncovered;
    for (int i = 0; i < n_vul; i++)
        uncovered.insert(i);

    vector<bool> used(patches.size(), false);
    Result res;
    res.totalCost  = 0;
    res.numPatches = 0;

    while (!uncovered.empty()) {
        int    bestIdx   = -1;
        double bestRatio = 1e18;

        for (int i = 0; i < (int)patches.size(); i++) {
            if (used[i]) continue;

            int newCount = 0;
            for (int v : patches[i].covers)
                if (uncovered.count(v)) newCount++;

            if (newCount == 0) continue;

            double ratio = patches[i].cost / newCount;
            if (ratio < bestRatio) {
                bestRatio = ratio;
                bestIdx   = i;
            }
        }

        if (bestIdx == -1) break;

        used[bestIdx] = true;
        res.solution.push_back(patches[bestIdx].name);
        res.totalCost  += patches[bestIdx].cost;
        res.numPatches++;

        for (int v : patches[bestIdx].covers)
            uncovered.erase(v);
    }

    auto end    = chrono::high_resolution_clock::now();
    res.runtime = chrono::duration<double>(end - start).count();

    return res;
}