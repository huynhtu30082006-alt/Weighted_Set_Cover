#pragma once
#include "utils.h"
#include <set>

// ─────────────────────────────────────────
//  GREEDY (cost / coverage)
//  Mỗi bước chọn patch có ratio nhỏ nhất:
//  ratio = cost / số_lỗ_hổng_mới_vá_được
// ─────────────────────────────────────────

Result greedySolve(int n_vul, vector<Patch>& patches) {
    auto start = chrono::high_resolution_clock::now();

    // Tập lỗ hổng chưa được vá
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

            // Đếm số lỗ hổng MỚI patch i vá được
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

        // Không còn patch nào vá được → dừng
        if (bestIdx == -1) break;

        // Chọn patch tốt nhất
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