#include "algorithm.h"
#include <random>
#include <algorithm>
#include <numeric>


Solution Algorithms::Brute_Force(const Problem& p)
{
    using clk = chrono::high_resolution_clock;
    auto start = clk::now();

    int m = p.get_n_patch();
    if (m <= 0) return {{}, 0.0, 0};

    const auto& patches = p.get_patch();
    const auto& costs = p.get_cost();
    int n = p.get_n_vul();

    vector<uint64_t> mask(m, 0);
    for(int i = 0; i < m; i++)
        for(int v : patches[i])
            mask[i] |= (1ULL << v);

    uint64_t FULL = (1ULL << n) - 1;

    double best_cost = 1e18;
    vector<int> best_solution(m, 0);

    long long LIMIT = (1LL << m);

    for(long long s = 1; s < LIMIT; s++)
    {
        uint64_t cover = 0;
        double cost = 0;

        for(int i = 0; i < m; i++)
        {
            if(s & (1LL << i))
            {
                cover |= mask[i];
                cost += costs[i];
            }
        }

        if(cover == FULL && cost < best_cost)
        {
            best_cost = cost;
            for(int i = 0; i < m; i++)
            {
                best_solution[i] = (s >> i) & 1;
            }
        }
    }

    auto end = clk::now();

    return {
        best_solution,
        best_cost,
        chrono::duration<double>(end-start).count()
    };
}

Solution Algorithms::Greedy(const Problem& p)
{
    auto start = chrono::high_resolution_clock::now();

    int m = p.get_n_patch();
    int n = p.get_n_vul();

    const auto& patches = p.get_patch();
    const auto& costs = p.get_cost();

    vector<uint64_t> mask(m, 0);
    for(int i = 0; i < m; i++)
        for(int v : patches[i])
            mask[i] |= (1ULL << v);

    uint64_t FULL = (1ULL << n) - 1;

    uint64_t cover = 0;
    vector<int> current(m, 0);

    while(cover != FULL)
    {
        int best = -1;
        double best_ratio = 1e18;

        for(int i = 0; i < m; i++)
        {
            if(current[i]) continue;

            uint64_t new_bits = mask[i] & (~cover);
            int gain = __builtin_popcountll(new_bits);

            if(gain == 0) continue;

            double ratio = costs[i] / (double)gain;

            if(ratio < best_ratio)
            {
                best_ratio = ratio;
                best = i;
            }
        }

        if(best == -1) break;

        current[best] = 1;
        cover |= mask[best];
    }

    double cost = 0;
    for(int i = 0; i < m; i++)
        if(current[i]) cost += costs[i];

    auto end = chrono::high_resolution_clock::now();


    return {current, 
            cost,
            chrono::duration<double>(end-start).count()
        };
}

Solution Algorithms::GA(const Problem& p)
{
    auto start = chrono::high_resolution_clock::now();

    int m = p.get_n_patch();
    int n = p.get_n_vul();
    const auto& patches = p.get_patch();
    const auto& costs = p.get_cost();

    using ull = uint64_t;

    // ===== Precompute mask =====
    vector<ull> mask(m, 0);
    for (int i = 0; i < m; i++)
        for (int v : patches[i])
            mask[i] |= (1ULL << v);

    ull FULL = (1ULL << n) - 1;

    // ===== GA params =====
    int pop_size = 80;
    int generations = 300;
    double mutation_rate = 0.03;

    mt19937 rng(random_device{}());
    uniform_int_distribution<int> bit(0, 1);
    uniform_real_distribution<double> prob(0.0, 1.0);

    // ===== Init population =====
    vector<vector<int>> population(pop_size, vector<int>(m));

    for (int i = 0; i < pop_size; ++i)
        for (int j = 0; j < m; ++j)
            population[i][j] = bit(rng);

    // seed greedy
    population[0] = Greedy(p).patch_selected;

    vector<int> best_global;
    double best_global_cost = 1e18;

    // ===== Helper: evaluate =====
    auto evaluate = [&](const vector<int>& sol, ull& cover_out) {
        ull cover = 0;
        double cost = 0;

        for (int j = 0; j < m; ++j)
        {
            if (sol[j])
            {
                cover |= mask[j];
                cost += costs[j];
            }
        }

        int missing = __builtin_popcountll(FULL ^ cover);
        cost += missing * 1000; // penalty mềm

        cover_out = cover;
        return cost;
    };

    // ===== GA loop =====
    for (int gen = 0; gen < generations; ++gen)
    {
        vector<pair<double, int>> fitness(pop_size);

        for (int i = 0; i < pop_size; ++i)
        {
            ull cover;
            double cost = evaluate(population[i], cover);

            fitness[i] = {cost, i};

            if (cover == FULL && cost < best_global_cost)
            {
                best_global_cost = cost;
                best_global = population[i];
            }
        }

        sort(fitness.begin(), fitness.end());

        // ===== Tournament selection =====
        auto select_parent = [&](int k = 3)
        {
            int best = uniform_int_distribution<int>(0, pop_size - 1)(rng);
            for (int i = 1; i < k; i++)
            {
                int r = uniform_int_distribution<int>(0, pop_size - 1)(rng);
                if (fitness[r].first < fitness[best].first)
                    best = r;
            }
            return best;
        };

        // ===== Elitism =====
        vector<vector<int>> new_population;
        int elite_count = pop_size / 10;

        for (int i = 0; i < elite_count; ++i)
            new_population.push_back(population[fitness[i].second]);

        // ===== Crossover =====
        while ((int)new_population.size() < pop_size)
        {
            int p1 = select_parent();
            int p2 = select_parent();

            vector<int> child(m);

            // uniform crossover
            for (int j = 0; j < m; ++j)
                child[j] = (bit(rng) ? population[p1][j] : population[p2][j]);

            // ===== Mutation =====
            for (int j = 0; j < m; ++j)
            {
                if (prob(rng) < mutation_rate)
                    child[j] ^= 1;
            }

            // ===== Repair (optional but strong) =====
            ull cover = 0;
            for (int j = 0; j < m; ++j)
                if (child[j]) cover |= mask[j];

            if (cover != FULL)
            {
                while (cover != FULL)
                {
                    int best = -1;
                    double best_ratio = 1e18;

                    for (int j = 0; j < m; ++j)
                    {
                        if (!child[j])
                        {
                            int gain = __builtin_popcountll(mask[j] & (~cover));
                            if (gain == 0) continue;

                            double ratio = costs[j] / gain;
                            if (ratio < best_ratio)
                            {
                                best_ratio = ratio;
                                best = j;
                            }
                        }
                    }

                    if (best == -1) break;

                    child[best] = 1;
                    cover |= mask[best];
                }
            }

            new_population.push_back(child);
        }

        population = new_population;
    }

    // ===== fallback nếu GA fail =====
    if (best_global.empty())
    {
        best_global = Greedy(p).patch_selected;
        double cost = 0;
        for (int i = 0; i < m; i++)
            if (best_global[i]) cost += costs[i];

        best_global_cost = cost;
    }

    auto end = chrono::high_resolution_clock::now();

    return {
        best_global,
        best_global_cost,
        chrono::duration<double>(end - start).count()
    };
}

Solution Algorithms::SA(const Problem& p)
{
    auto start = chrono::high_resolution_clock::now();

    int m = p.get_n_patch();
    int n = p.get_n_vul();
    const auto& patches = p.get_patch();
    const auto& costs = p.get_cost();

    using ull = uint64_t;

    // ===== Precompute mask =====
    vector<ull> mask(m, 0);
    for (int i = 0; i < m; i++)
        for (int v : patches[i])
            mask[i] |= (1ULL << v);

    ull FULL = (1ULL << n) - 1;

    // ===== RNG tốt hơn rand() =====
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> pick_idx(0, m - 1);
    uniform_real_distribution<double> prob(0.0, 1.0);

    // ===== Init: dùng Greedy (rất quan trọng) =====
    vector<int> current = Greedy(p).patch_selected;

    // compute cost + cover ban đầu
    auto eval = [&](const vector<int>& sol, ull& cover_out){
        ull cover = 0;
        double cost = 0;
        for (int i = 0; i < m; i++)
        {
            if (sol[i])
            {
                cover |= mask[i];
                cost += costs[i];
            }
        }
        int missing = __builtin_popcountll(FULL ^ cover);
        cost += missing * 1000;            // penalty mềm
        cover_out = cover;
        return cost;
    };

    ull cur_cover;
    double current_cost = eval(current, cur_cover);

    vector<int> best_solution = current;
    double best_cost = current_cost;

    // ===== Nhiệt độ =====
    double T = current_cost;     // adaptive tốt hơn 1000
    double cooling_rate = 0.997; // chậm hơn để explore nhiều

    // ===== SA loop =====
    while (T > 1e-3)
    {
        vector<int> neighbor = current;

        // ===== Neighbor mạnh hơn: flip 2–3 bit =====
        int flips = 2 + (pick_idx(rng) % 2); // 2 hoặc 3
        for (int k = 0; k < flips; k++)
        {
            int idx = pick_idx(rng);
            neighbor[idx] ^= 1;
        }

        ull cover;
        double neighbor_cost = eval(neighbor, cover);

        double delta = neighbor_cost - current_cost;

        // ===== Acceptance =====
        if (delta < 0 || prob(rng) < exp(-delta / T))
        {
            current = neighbor;
            current_cost = neighbor_cost;

            if (cover == FULL && current_cost < best_cost)
            {
                best_cost = current_cost;
                best_solution = current;
            }
        }

        T *= cooling_rate;
    }

    // ===== fallback nếu không tìm được nghiệm hợp lệ =====
    if (best_cost >= 1e18)
    {
        best_solution = Greedy(p).patch_selected;
        best_cost = 0;
        for (int i = 0; i < m; i++)
            if (best_solution[i]) best_cost += costs[i];
    }

    auto end = chrono::high_resolution_clock::now();

    return {
        best_solution,
        best_cost,
        chrono::duration<double>(end - start).count()
    };
}

void Algorithms::DFS_ILP(size_t i, uint64_t cover, double cost, const vector<uint64_t>& mask, const vector<int>& costs, uint64_t FULL, vector<int>& current, vector<int>& best_solution, double& best_cost, chrono::high_resolution_clock::time_point start)
{

    // prune
    if(cost >= best_cost) return;

    // đủ cover
    if(cover == FULL)
    {
        best_cost = cost;
        best_solution = current;
        return;
    }

    if(i == mask.size()) return;

    // chọn patch
    current[i] = 1;
    DFS_ILP(i+1, cover | mask[i], cost + costs[i],
            mask, costs, FULL, current, best_solution, best_cost,
            start);

    // không chọn
    current[i] = 0;
    DFS_ILP(i+1, cover, cost,
            mask, costs, FULL, current, best_solution, best_cost,
            start);
}

Solution Algorithms::ILP(const Problem& p)
{
    auto start = chrono::high_resolution_clock::now();

    int m = p.get_n_patch();
    int n = p.get_n_vul();

    const auto& patches = p.get_patch();
    const auto& costs = p.get_cost();

    vector<uint64_t> mask(m, 0);
    for(int i = 0; i < m; i++)
        for(int v : patches[i])
            mask[i] |= (1ULL << v);

    uint64_t FULL = (1ULL << n) - 1;

    vector<int> current(m, 0);
    vector<int> best_solution(m, 0);
    double best_cost = 1e18;

    DFS_ILP(
        0,
        0,
        0.0,
        mask,
        costs,
        FULL,
        current,
        best_solution,
        best_cost,
        start
    );

    auto end = chrono::high_resolution_clock::now();

    return {
        best_solution,
        best_cost,
        chrono::duration<double>(end - start).count()
    };
}

