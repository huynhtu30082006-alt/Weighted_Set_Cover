#include "GA.h"
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <random>

using namespace std;

Solution GeneticAlgorithm::run(const Problem& p)
{
    auto start = chrono::high_resolution_clock::now();

    int m = p.get_n_patch();
    int n = p.get_n_vul();
    const auto& patches = p.get_patch();
    const auto& costs = p.get_cost();

    vector<uint64_t> mask(m, 0);
    for (int i = 0; i < m; i++)
        for (int v : patches[i])
            mask[i] |= (1ULL << v);

    uint64_t FULL = (1ULL << n) - 1;

    int pop_size = 50;
    int generations = 200;

    vector<vector<int>> population(pop_size, vector<int>(m, 1));
    vector<int> best_global(m, 1);
    double best_global_cost = 1e18;

    for (int gen = 0; gen < generations; ++gen) {
        vector<pair<double, int>> fitness(pop_size);

        for (int i = 0; i < pop_size; ++i) {
            uint64_t cover = 0;
            double cost = 0;
            for (int j = 0; j < m; ++j) {
                if (population[i][j]) {
                    cover |= mask[j];
                    cost += costs[j];
                }
            }

            if (cover != FULL) cost += 1e9;

            fitness[i] = { cost, i };

            if (cost < best_global_cost && cover == FULL) {
                best_global_cost = cost;
                best_global = population[i];
            }
        }

        sort(fitness.begin(), fitness.end());

        vector<vector<int>> new_population;

        int elite_count = pop_size / 10;
        for (int i = 0; i < elite_count; ++i) {
            new_population.push_back(population[fitness[i].second]);
        }

        while (new_population.size() < pop_size) {
            int p1 = fitness[rand() % (pop_size / 2)].second;
            int p2 = fitness[rand() % (pop_size / 2)].second;
            int cross_pt = rand() % m;

            vector<int> child(m);
            for (int j = 0; j < m; ++j) {
                child[j] = (j < cross_pt) ? population[p1][j] : population[p2][j];
            }
            new_population.push_back(child);
        }

        for (int i = elite_count; i < pop_size; ++i) {
            if (rand() % 100 < 5) {
                int flip = rand() % m;
                new_population[i][flip] = 1 - new_population[i][flip];
            }
        }
        population = new_population;
    }

    auto end = chrono::high_resolution_clock::now();
    int count = accumulate(best_global.begin(), best_global.end(), 0);

    return { best_global, best_global_cost, count, chrono::duration<double>(end - start).count() };
}