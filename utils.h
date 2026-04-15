#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <random>   
#include <algorithm>
#include <iomanip>  

using namespace std;

// Định nghĩa cấu trúc dữ liệu
struct Patch {
    string name;        
    double cost;        
    vector<int> covers; 
};

struct Result {
    vector<string> solution; 
    double totalCost = 0;    // Nên gán bằng 0 để tránh rác dữ liệu
    int    numPatches = 0;
    double runtime = 0;          
};

inline vector<Patch> generateData(int n_vul, int n_patch, mt19937& rng) {
    vector<Patch> patches(n_patch);

    for (int i = 0; i < n_patch; i++) {
        patches[i].name = "P" + to_string(i);
        patches[i].cost = uniform_real_distribution<>(1.0, 20.0)(rng);
    }

    for (int v = 0; v < n_vul; v++) {
        int p = rng() % n_patch;
        patches[p].covers.push_back(v);
    }

    for (int i = 0; i < n_patch; i++) {
        int extra = rng() % max(1, n_vul / 3);
        for (int j = 0; j < extra; j++) {
            patches[i].covers.push_back(rng() % n_vul);
        }
        sort(patches[i].covers.begin(), patches[i].covers.end());
        patches[i].covers.erase(
            unique(patches[i].covers.begin(), patches[i].covers.end()),
            patches[i].covers.end()
        );
    }
    return patches;
}

inline void printResult(const string& algoName, const Result& r) {
    cout << algoName << ":\n";
    cout << "  Solution : [";
    for (int i = 0; i < (int)r.solution.size(); i++) {
        if (i) cout << ", ";
        cout << r.solution[i];
    }
    cout << "]\n";
    cout << fixed << setprecision(4);
    cout << "  Cost     : " << r.totalCost  << "\n";
    cout << "  Patches  : " << r.numPatches << "\n";
    cout << "  Time     : " << r.runtime    << "s\n\n";
}

inline void printAverage(const string& algoName,
                  double sumCost, double sumTime,
                  double sumPatches, int n) {
    cout << "── " << algoName << " (average " << n << " runs) ──\n";
    cout << fixed << setprecision(4);
    cout << "  Avg Cost    : " << (n > 0 ? sumCost / n : 0) << "\n";
    cout << "  Avg Patches : " << (n > 0 ? sumPatches / n : 0) << "\n";
    cout << "  Avg Time    : " << (n > 0 ? sumTime / n : 0) << "s\n\n";
}

#endif 