#include "utils.h"
#include "brute_force.h"
#include "greedy.h"

int main() {
    // ── Nhập input ──────────────────────────
    int n_vul, n_patch, n_runs;

    cout << "WEIGHTED SET COVER PROBLEM\n";20

    cout << "Nhap n_vul   (20-50) : "; cin >> n_vul;
    cout << "Nhap n_patch (10-30) : "; cin >> n_patch;
    cout << "Nhap so lan chay     : "; cin >> n_runs;
    cout << "\n";

    // Giới hạn Brute Force
    bool runBF = (n_patch <= 20);
    if (!runBF)
        cout << "[!] n_patch > 20 -> Bo qua Brute Force (qua cham)\n\n";

    // ── Biến tích luỹ average ────────────────
    double bf_cost = 0, bf_time = 0, bf_patches = 0;
    double gr_cost = 0, gr_time = 0, gr_patches = 0;

    mt19937 rng(42); // seed cố định → kết quả lặp lại được

    // ── Chạy n_runs lần ─────────────────────
    for (int run = 1; run <= n_runs; run++) {
        cout << "Run " << run << "/" << n_runs;

        auto patches = generateData(n_vul, n_patch, rng);

        // Brute Force
        if (runBF) {
            Result bf = bruteForceSolve(n_vul, patches);
            printResult("Brute Force", bf);
            bf_cost    += bf.totalCost;
            bf_time    += bf.runtime;
            bf_patches += bf.numPatches;
        }

        // Greedy
        Result gr = greedySolve(n_vul, patches);
        printResult("Greedy", gr);
        gr_cost    += gr.totalCost;
        gr_time    += gr.runtime;
        gr_patches += gr.numPatches;
    }

    cout << "AVERAGE RESULTS:\n";
    

    if (runBF)
        printAverage("Brute Force", bf_cost, bf_time, bf_patches, n_runs);

    printAverage("Greedy", gr_cost, gr_time, gr_patches, n_runs);

    return 0;
}