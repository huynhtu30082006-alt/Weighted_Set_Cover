#include "algorithm.h"
#include <chrono>
#include <algorithm>
#include <numeric>
// Solution Algorithms::Brute_Force(const Problem& p)
// {
//     auto start = chrono::high_resolution_clock::now();

//     int m = p.get_n_patch();

//     if (m <= 0)
//         return {{}, 0.0, 0, 0.0};

//     if (m >= 60)
//     {
//         throw std::runtime_error("Brute force not feasible: The number of Patch too large");
//     }

//     double best_cost = 1e18;
//     vector<int> best_solution(m, 0);

//     long long total = (1LL << m);

//     for (long long mask = 1; mask < total; mask++)
//     {
//         vector<int> current(m, 0);

//         // convert mask → vector
//         for (int i = 0; i < m; i++)
//         {
//             if (mask & (1LL << i))
//             {
//                 current[i] = 1;
//             }
//         }

//         // check cover
//         if (is_covered(p, current))
//         {
//             double cost = current_cost(p, current);
//             if (cost < best_cost)
//             {
//                 best_cost = cost;
//                 best_solution = current;
//             }
//         }
//     }

//     auto end = chrono::high_resolution_clock::now();
//     double runtime = chrono::duration<double>(end - start).count();

//     int count = 0;
//     for (int x : best_solution)
//     {
//         count += x;
//     }

//     return {best_solution, best_cost, count, runtime};
// }

// Solution Algorithms::Greedy(const Problem& p)
// {
//     auto start = chrono::high_resolution_clock::now();

//     int m = p.get_n_patch();
//     int n = p.get_n_vul();

//     vector<int> current(m, 0);          // solution vector
//     vector<bool> covered(n, false);     // trạng thái coverage

//     while (true)
//     {
//         int best_idx = -1;
//         double best_ratio = 1e18;

//         for (int i = 0; i < m; i++)
//         {
//             if (current[i] == 1) continue;

//             int gain = 0;

//             for (int v : p.get_patch()[i])
//             {
//                 if (!covered[v])
//                 {
//                     gain++;
//                 }
//             }

//             if (gain == 0) continue;

//             double ratio = p.get_cost()[i] / gain;

//             if (ratio < best_ratio)
//             {
//                 best_ratio = ratio;
//                 best_idx = i;
//             }
//         }

//         if (best_idx == -1) break;

//         // chọn patch tốt nhất
//         current[best_idx] = 1;

//         // update covered
//         for (int v : p.get_patch()[best_idx])
//         {
//             covered[v] = true;
//         }

//         // nếu cover hết thì dừng
//         if (all_of(covered.begin(), covered.end(), [](bool x){ return x; }))
//         {
//             break;
//         }
//     }

//     auto end = chrono::high_resolution_clock::now();
//     double runtime = chrono::duration<double>(end - start).count();

//     double cost = current_cost(p, current);

//     int count = 0;
//     for (int x : current)
//     {
//         count += x;
//     }

//     return {current, cost, count, runtime};
// }

// bool Algorithms::is_covered(const Problem& p, const vector<int>& current)
// {
//     vector<bool> covered(p.get_n_vul(), false);
//     const auto& patches = p.get_patch();

//     for(int i = 0; i < p.get_n_patch(); i++)
//     {
//         if(current[i] == 1)
//         {
//             for(int v : patches[i])
//             {
//                 if(v >= 0 && v < covered.size())
//                     covered[v] = true;
//             }
//         }
//     }

//     for(bool x : covered)
//     {
//         if(!x) return false;
//     }

//     return true;
// }

// double Algorithms::current_cost(const Problem& p, const vector<int>& current) 
// {
//     double cost = 0;
//     for (int i = 0; i < p.get_n_patch(); i++)
//     {
//         if (current[i]) cost += p.get_cost()[i];
//     }
//     return cost;
// }

// double Algorithms::estimate_bound(const Problem& p, vector<int>& current)
// {
//     vector<bool> covered(p.get_n_vul(), false);

//     for(int i = 0; i < p.get_n_patch(); i++)
//     {
//         if(current[i] == true)
//         {
//             for(int v : p.get_patch()[i])
//             {
//                 covered[v] = true;
//             }
//         }
//     }

//     vector<pair<double,int>> ratio;

//     for(int i = 0; i < p.get_n_patch(); i++)
//     {
//         if(current[i] == true)
//         {
//             continue;
//         }

//         int gain = 0;
//         for(int v : p.get_patch()[i])
//         {
//             if(!covered[v])
//             {
//                 gain ++;
//             }
//         }
//         if(gain > 0)
//         {
//             ratio.push_back({p.get_cost()[i] / gain, i}); //greedy
//         }
//     }

//     sort(ratio.begin(), ratio.end()); //first choose patch has ratio lower 

//     double cost_est = 0;
//     vector<bool> temp = covered;

//     for(auto [r, i] : ratio)
//     {
//         int gain = 0;
//         for(int v : p.get_patch()[i])
//         {
//             if(!temp[v])
//             {
//                 gain++;
//             }
//         }
//         if(gain == 0)
//         {
//             continue;
//         }
//         cost_est += p.get_cost()[i];

//         for(int v : p.get_patch()[i])
//         {
//             temp[v] = true;
//         }
//         if(all_of(temp.begin(), temp.end(), [](bool x){ return x; }))
//         {
//             break;
//         }
//     }
//     return cost_est;
// }

// void Algorithms::DFS_ILP(const Problem& p, int i, vector<int>& current, vector<int>& best_solution, double& best_cost)
// {
//     if(i == p.get_n_patch())
//     {
//         if(is_covered(p, current) == true)
//         {
//             double cost = current_cost(p, current);
//             if(cost < best_cost)
//             {
//                 best_cost = cost;
//                 best_solution = current;
//             }
//         }
//         return;

//     }

//     double cost_now = current_cost(p, current);
//     double bound = cost_now + estimate_bound(p, current);

//     if(bound >= best_cost)
//     {
//         return; //prune
//     }

//     //choose
//     current[i] = 1;
//     DFS_ILP(p, i+1, current, best_solution, best_cost);

//     //not choose
//     current[i] = 0;
//     DFS_ILP(p, i+1, current, best_solution, best_cost);

// }

void Algorithms::DFS_ILP(size_t i, uint64_t cover, double cost, const vector<uint64_t>& mask, const vector<int>& costs, uint64_t FULL, vector<int>& current, vector<int>& best_solution, double& best_cost)
{
    // prune sớm
    if(cost >= best_cost) return;

    // đã cover đủ
    if(cover == FULL)
    {
        best_cost = cost;
        best_solution = current;
        return;
    }

    // hết patch
    if(i == mask.size())
    {
        return;
    } 

    // chọn patch i
    current[i] = 1;
    DFS_ILP(i+1, cover | mask[i], cost + costs[i], mask, costs, FULL, current, best_solution, best_cost);

    // không chọn
    current[i] = 0;
    DFS_ILP(i+1, cover, cost, mask, costs, FULL, current, best_solution, best_cost);
}

Solution Algorithms::ILP(const Problem& p)
{
    auto start = chrono::high_resolution_clock::now();

    int m = p.get_n_patch();
    int n = p.get_n_vul();

    const auto& patches = p.get_patch();
    const auto& costs = p.get_cost();

    // precompute mask
    vector<uint64_t> mask(m, 0);
    for(int i = 0; i < m; i++)
    {
        for(int v : patches[i])
            mask[i] |= (1ULL << v);
    }

    uint64_t FULL = (1ULL << n) - 1;

    vector<int> current(m, 0);
    vector<int> best_solution(m, 0);
    double best_cost = 1e18;

    // 🔥 gọi DFS mới
    DFS_ILP(
        0,          // i
        0,          // cover
        0.0,        // cost
        mask,
        costs,
        FULL,
        current,
        best_solution,
        best_cost
    );

    auto end = chrono::high_resolution_clock::now();

    int count = 0;
    for(int x : best_solution) count += x;

    return {
        best_solution,
        best_cost,
        count,
        chrono::duration<double>(end - start).count()
    };
}

// Solution Algorithms::ILP(const Problem& p)
// {
//     auto start = chrono::high_resolution_clock::now();

//     vector<int> current(p.get_n_patch(), 0); //patch have choose in search process 
//     vector <int> best_solution; //patch have choose
//     double best_cost = 1e18; //simulate infinitive

//     DFS_ILP(p, 0, current, best_solution, best_cost);

//     auto end = chrono::high_resolution_clock::now();
//     double runtime = chrono::duration<double>(end-start).count();

//     int count = 0;
//     for(int x : best_solution)
//     {
//         count += x;
//     }

//     return {best_solution, best_cost, count, runtime};
// }

Solution Algorithms::Brute_Force(const Problem& p)
{
    auto start = chrono::high_resolution_clock::now();

    int m = p.get_n_patch();
    if (m <= 0) return {{}, 0.0, 0, 0.0};

    const auto& patches = p.get_patch();
    const auto& costs = p.get_cost();
    int n = p.get_n_vul();

    vector<uint64_t> mask(m, 0);
    for(int i = 0; i < m; i++)
        for(int v : patches[i])
            mask[i] |= (1ULL << v);

    // int FULL = (1 << n) - 1;
    uint64_t FULL = (1ULL << n) - 1;

    double best_cost = 1e18;
    vector<int> best_solution(m, 0);

    for(long long s = 1; s < (1LL << m); s++)
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
                best_solution[i] = (s >> i) & 1;
        }
    }

    auto end = chrono::high_resolution_clock::now();

    int count = accumulate(best_solution.begin(), best_solution.end(), 0);

    return {best_solution, best_cost, count,
            chrono::duration<double>(end-start).count()};
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

    int count = accumulate(current.begin(), current.end(), 0);

    return {current, cost, count,
            chrono::duration<double>(end-start).count()};
}